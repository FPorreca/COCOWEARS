import time
import threading
import queue
import pandas as pd
from pylsl import StreamInlet, resolve_stream
import numpy as np
from scipy.signal import welch  # Per il calcolo della PSD
import pygame
from datetime import datetime
from pylsl import local_clock
import psutil
import os
from scipy.signal import butter, filtfilt, iirnotch
from scipy.signal import welch
import pickle



fs = 256  # EEG sampling frequency in Hz
lowcut = 6  # Low cut-off frequency (in Hz)
highcut = 30  # High cut-off frequency (in Hz)
notch_freq = 50.0  # Frequency to be removed (Hz)
quality_factor = 30.0  # Quality factor of the notch filter
model = pickle.load(open("lda_model.pkl", "rb"))

frequencies = [10, 12, 8.57]  # Frequenze specifiche
colors = [(255, 255, 255)] * 3  # Inizialmente bianchi

# Dimensione quadrati
square_size = 150
# Posizioni estreme dei quadrati (massima distanza)
positions = [
    (WIDTH // 2 - square_size // 2, HEIGHT // 10),  # Alto al centro
    (WIDTH // 4, HEIGHT - square_size - HEIGHT // 5),  # Sinistra in basso
    (WIDTH - square_size - WIDTH // 4, HEIGHT - square_size - HEIGHT // 5),  # Destra in basso
]

# Define constants
sampling_rate = 256  # Hz
block_size = 2 * sampling_rate     # 2 seconds of data (512 samples)
shift_samples = round(0.5 * sampling_rate)   # 0.1 sec shift (26 samples at 256 Hz)

timestamps_buffer = []
eeg_buffer_global = []  # Buffer globale che raccoglie tutti i dati
running = True  # Controllo per il thread


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

def process_block(df):
    """Function to process a block (e.g., filtering, PSD, ML)."""
    print(f"Processing block with start time: {df['Timestamp'].iloc[0]}")
    data_block = df.iloc[:, 9:]  # PO7, O1, O2, PO8    verificare come i dati vengono forniti nello standard LSL
    n_channels = data_block.shape[1]
    X_psd_block = []
    for channel in range(n_channels):
        eeg_signal = data_block.iloc[:,channel]
        filtered_signal = bandpass_filter(eeg_signal, lowcut, highcut, fs)
        filtered_signal = notch_filter(filtered_signal, fs, notch_freq, quality_factor)
        freqs, psd = welch(filtered_signal, fs=fs, nperseg=256, noverlap=192)
        X_psd_block.append(psd[0:50])
    X_psd = np.array(X_psd_block).T
    X_psd = X_psd.reshape(1, -1)
    y_block_pred = model.predict(X_psd)
    print("PREDICTION: ",y_block_pred )

def process_blocks_from_queue():
    """Continuously process blocks in the correct order."""
    while True:
        timestamp, df = block_queue.get()  # Retrieve blocks in timestamp order
        process_block(df)
        block_queue.task_done()

# ======== Funzione per ricezione EEG e gestione blocchi con sovrapposizione =========
def receive_eeg():
    global running, eeg_buffer_global, timestamps_buffer
    time.sleep(2)  # Delay execution inside the thread
    block_start = 0  # Tempo di inizio del primo blocco
    start_idx = 0
    start = 0
    while running:
        chunk = inlet.pull_chunk(timeout=2.05, max_samples=block_size)

        if chunk:  # Ensure we received data
            samples, timestamps = chunk  # Unpack data

            if len(samples) > 0:  # Ensure non-empty chunk
                eeg_buffer_global.extend(samples)
                timestamps_buffer.extend(timestamps)

                # Maintain a rolling buffer of data (keep last 5 blocks worth)
                max_buffer_size = block_size * 5
                if len(eeg_buffer_global) > max_buffer_size:
                    eeg_buffer_global = eeg_buffer_global[-max_buffer_size:]
                    timestamps_buffer = timestamps_buffer[-max_buffer_size:]
                    start_idx = start_idx - block_size

                # Process blocks based on index shifting

                while start_idx + block_size <= len(eeg_buffer_global):
                    # Extract block data
                    block_data = eeg_buffer_global[start_idx: start_idx + block_size]
                    block_timestamps = timestamps_buffer[start_idx: start_idx + block_size]

                    # Convert to DataFrame
                    df = pd.DataFrame(block_data)
                    df.insert(0, "Timestamp", block_timestamps)

                    # Add block to queue for ordered processing
                    print(f"Adding block with start time: {block_timestamps[0]} (Queue size: {block_queue.qsize()})")
                    #block_queue.put(df)
                    block_queue.put((block_timestamps[0], df))  # Insert tuple (timestamp, DataFrame)

                    # Move to the next block with the shift
                    start_idx += shift_samples

def flickering_task(index, frequency):
    """Handles the flickering of a square at the given index and frequency."""
    global running
    interval = 1 / (2 * frequency)  # Toggle interval
    while running:
        time.sleep(interval)  # Wait based on flickering frequency
        with color_lock:  # Ensure thread safety while updating colors
            colors[index] = (0, 0, 0) if colors[index] == (255, 255, 255) else (255, 255, 255)


# Ottieni l'ID del processo corrente
pid = os.getpid()

# Cambia la priorità a "HIGH"
p = psutil.Process(pid)
p.nice(psutil.HIGH_PRIORITY_CLASS)

# Inizializza Pygame
pygame.init()

# Imposta la finestra a schermo intero
screen = pygame.display.set_mode((0, 0), pygame.FULLSCREEN)
WIDTH, HEIGHT = screen.get_size()  # Ottieni dimensioni dello schermo
pygame.display.set_caption("SSVEP Stimulator")


# ======== Connessione a LSL =========
print("Cercando flussi EEG tramite LSL...")
streams = resolve_stream('type', 'EEG')  # Trova flussi EEG disponibili
inlet = StreamInlet(streams[0])  # Connetti al primo flusso trovato
print("Connesso a LSL!")

info = inlet.info()
print(info)

# Thread synchronization
lock = threading.Lock()

# Queue for ordered block processing
block_queue = queue.PriorityQueue()  # Instead of queue.Queue()

# Start processing thread
processing_thread = threading.Thread(target=process_blocks_from_queue, daemon=True)
processing_thread.start()

# ======== Avvia il thread per la ricezione EEG =========
eeg_thread = threading.Thread(target=receive_eeg, daemon=True)
eeg_thread.start()


# Timer per il lampeggio
timers = [time.perf_counter()] * 3  # Independent timers
phases = [0, np.pi / 2, np.pi]  # Phase shift in radians
# Lock for thread safety
color_lock = threading.Lock()

# Start flickering threads
flicker_threads = []
for i, freq in enumerate(frequencies):
    thread = threading.Thread(target=flickering_task, args=(i, freq), daemon=True)
    thread.start()
    flicker_threads.append(thread)

# Main loop
clock = pygame.time.Clock()
while running:
    screen.fill((0, 0, 0))  # Black background

    # Draw squares
    with color_lock:  # Ensure safe access to colors
        for i in range(3):
            pygame.draw.rect(screen, colors[i], (*positions[i], square_size, square_size))

    pygame.display.flip()
    clock.tick(60)  # Keep main loop running smoothly

    # Event handling
    for event in pygame.event.get():
        if event.type == pygame.QUIT or (event.type == pygame.KEYDOWN and event.key == pygame.K_ESCAPE):
            running = False  # Exit on ESC or window close

pygame.quit()
