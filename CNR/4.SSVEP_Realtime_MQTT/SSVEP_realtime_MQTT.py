import json
from contextlib import ExitStack
import csv
import sys
import time
import os
from scipy.signal import butter, filtfilt, iirnotch
from scipy.signal import welch
import pickle
import numpy as np
from scipy.signal import welch
import pandas as pd
import paho.mqtt.client as mqtt
from collections import Counter

from bbt import Signal, Device, SensorType, ImpedanceLevel


broker_address = "XXX.XXX.XXX.XXX"
broker_usr="xxx"
broker_pwd="xxx"
port = "XXX" #"1883"

# EEG signal parameters
fs = 256  # Sampling frequency in Hz (common sampling rate for EEG signals)
lowcut = 6  # Low cut-off frequency (in Hz)
highcut = 30  # High cut-off frequency (in Hz)
notch_freq = 50.0  # Frequency to be removed (Hz)
quality_factor = 30.0  # Quality factor of the notch filter
model = pickle.load(open("lda_model.pkl", "rb"))

block = []

def butter_bandpass(lowcut, highcut, fs, order=4):
    nyquist = 0.5 * fs  # Nyquist Frequency
    low = lowcut / nyquist
    high = highcut / nyquist
    b, a = butter(order, [low, high], btype='band')  # Design a Butterworth band-pass filter
    return b, a

def bandpass_filter(data, lowcut, highcut, fs, order=4):
    b, a = butter_bandpass(lowcut, highcut, fs, order=order)
    y = filtfilt(b, a, data)  # Apply the filter
    return y

def notch_filter(data, fs, notch_freq=50.0, quality_factor=30.0):
    b, a = iirnotch(notch_freq, quality_factor, fs)
    y = filtfilt(b, a, data)  # Apply the notch filter
    return y

def try_to(condition, action, tries, message=None):
        t = 0
        while (not condition() and t < tries):
            t += 1
            if message:
                print("{} ({}/{})".format(message, t, tries))
            action()
        return condition()


def config_signals(device):
    signals = device.get_signals()
    for s in signals:
        s.set_mode(1)

def csv_filename(signal_number, signal_type):
    return f"signal_{signal_number}({signal_type}).csv"

def process_block(df):
    """Function to process a block (e.g., filtering, PSD, ML)."""
    print(f"Processing block ")
    data_block = df.iloc[:, 9:-1]   # O1, O2
    n_channels = data_block.shape[1]
    index = 0
    window_shift = 0.1  # shift in secondi da una finesta a quella successiva
    window_width = 2  # larghezza in secondi della finestra su cui effettuare la classificazione
    y_block_pred_list = []

    while (index * round(window_shift * fs) + window_width * fs) <= len(data_block):
        X_psd_block = []
        for channel in range(n_channels):
            eeg_signal = data_block.iloc[index * round(window_shift * fs): window_width * fs + index * round(window_shift * fs), channel]
            filtered_signal = bandpass_filter(eeg_signal, lowcut, highcut, fs)
            filtered_signal = notch_filter(filtered_signal, fs, notch_freq, quality_factor)
            freqs, psd = welch(filtered_signal, fs=fs, nperseg=256, noverlap=192)
            X_psd_block.append(psd[0:50])
        X_psd = np.array(X_psd_block).T
        X_psd = X_psd.reshape(1, -1)
        y_block_pred = model.predict(X_psd)
        y_block_pred_list.append(int(y_block_pred[0]))
        index += 1
    conteggio = Counter(y_block_pred_list)
    elemento_piu_frequente, frequenza = conteggio.most_common(1)[0]
    print(f"Elemento più presente: {elemento_piu_frequente}, con {frequenza} occorrenze")

    if frequenza >= 12:
        if elemento_piu_frequente != 0:
            mqtt_pub(elemento_piu_frequente, round(time.time()))

def record_one(device, csvs, signals):
    sequence, battery, flags, data = device.read()
    ts = time.time_ns()
    common_data = [ts, sequence, battery, flags]
    data_offset = 0
    for csv, s in zip(csvs, signals):
        n_values = s.channels()*s.samples()
        signal_data_slice = data[data_offset:data_offset + n_values]
        if data_offset == 0:
            for c in range(s.samples()):
                block.append(signal_data_slice[c::s.samples()])
        for i in range(s.samples()):
            csv.writerow(common_data + signal_data_slice[i::s.samples()])
        data_offset += n_values

def record_data(device, length):
    #create the csv files
    with ExitStack() as stack:
        active_signals = [s for s in device.get_signals() if s.mode() != 0]
        #open csv files
        files = [stack.enter_context(open(csv_filename(i, s.type()), 'w', newline='')) for i,s in enumerate(active_signals)]

        #write headers
        writers = [csv.writer(f) for f in files]
        for (s, w) in zip(active_signals, writers):
            common_header = ["timestamp", "sequence", "battery", "flags"]
            channels_header = [ f"channel_{i}" for i in range(s.channels())]
            w.writerow(common_header + channels_header)

        #record data
        device.start()

        f = int(device.get_frequency())
        for i in range(length * f):
            record_one(device, writers, active_signals)
        device.stop()
        df = pd.DataFrame(block)
        process_block(df)
    print ("Stopped: ", not device.is_running())

def mqtt_pub(value, time):
    global client
    try:
        topic = "topic/topicxxx"
        Data = {}
        Data['Date'] = time
        Data['value'] = value
        json_data = json.dumps(Data)
        client.publish(topic, json_data, retain=True)
    except Exception as e:
        print("ERROR MQTT: ", str(e))

def on_connect(client, userdata, flags, rc):
    print("Connected with result code " + str(rc) + str(client))

def on_disconnect(client, userdata, rc=0):
    print("DisConnected from broker with result code: " + str(rc) + str(client))

def on_log(client, userdata, level, buf):
    print("log: ", buf)

def on_message(client, userdata, message):
    print("message received:", str(message.payload.decode("utf-8")))
    print("message topic =", message.topic)
    print("message qos =", message.qos)
    print("message retain flag =", message.retain)
    print("message client =", client)

def mqtt_init():
    global client
    print("creating new instance")
    client = mqtt.Client()  # create new instance
    client.on_message = on_message  # attach function to callback
    client.on_log = on_log
    client.on_connect = on_connect
    client.on_disconnect = on_disconnect

    print("connecting to broker")
    client.username_pw_set(broker_usr, broker_pwd)
    client.connect(broker_address, 1883)  # connect to broker
    time.sleep(4)  # wait

    client.loop_start()


if __name__ == "__main__":

    try:
        mqtt_init()
    except Exception as exc:
        print(exc)

    length = 10
    name = "BBT-E12-AAB075"
    with Device.create_bluetooth_device(name) as device:
        if not try_to(device.is_connected, device.connect, 10, "Connecting to {}".format(name)):
            print("unable to connect")
            exit(1)
        print ("Connected")

        print(f"Recording {length} seconds of data into csv files from device {name}")

        config_signals(device)
        for x in range (10):
            block = []
            record_data(device, 4)

        if not try_to(lambda: not device.is_connected(), device.disconnect, 10):
            print("unable to disconnect")
            exit(1)        
        print ("Connected")    
    
        
