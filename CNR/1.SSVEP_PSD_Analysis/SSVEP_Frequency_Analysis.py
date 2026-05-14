import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns
from scipy.signal import butter, filtfilt, iirnotch
from scipy.signal import welch



def butter_bandpass(lowcut, highcut, fs, order=4):
    nyquist = 0.5 * fs  # Nyquist Frequency
    low = lowcut / nyquist
    high = highcut / nyquist
    b, a = butter(order, [low, high], btype='band')  # Design a Butterworth band-pass filter
    return b, a

def bandpass_filter(data, lowcut, highcut, fs, order=5):
    b, a = butter_bandpass(lowcut, highcut, fs, order=order)
    y = filtfilt(b, a, data)  # Apply the filter
    return y


def notch_filter(data, fs, notch_freq=50.0, quality_factor=30.0):
    b, a = iirnotch(notch_freq, quality_factor, fs)
    y = filtfilt(b, a, data)  # Apply the notch filter
    return y


def bandpower(psd, band, freqs):
    """Compute the average power of the signal x in a specific frequency band.

    Parameters
    ----------
    data : 1d-array
        Input signal in the time-domain.
    sf : float
        Sampling frequency of the data.
    band : list
        Lower and upper frequencies of the band of interest.
    window_sec : float
        Length of each window in seconds.
        If None, window_sec = (1 / min(band)) * 2
    relative : boolean
        If True, return the relative power (= divided by the total power of the signal).
        If False (default), return the absolute power.

    Return
    ------
    bp : float
        Absolute or relative band power.
    """

    from scipy.integrate import simpson, cumulative_simpson
    band = np.asarray(band)
    low, high = band

    # Frequency resolution
    freq_res = freqs[1] - freqs[0]

    # Find closest indices of band in frequency vector
    idx_band = np.logical_and(freqs >= low, freqs <= high)

    # Integral approximation of the spectrum using Simpson's rule.
    bp = simpson(psd[idx_band], dx=freq_res)
    return bp


system_frequencies = [20, 15, 12]

# EEG signal parameters
fs = 256  # Sampling frequency in Hz (common sampling rate for EEG signals)
T = 1.0 / fs  # Sample interval

# Salvataggio csv PSD e figure
path = 'Path_to_save_folder'

lowcut = 3  # Low cut-off frequency (in Hz)
highcut = 50  # High cut-off frequency (in Hz)
notch_freq = 50.0  # Frequency to be removed (Hz)
quality_factor = 30.0  # Quality factor of the notch filter
delta_frequency = 0.25

frame_number = 0
psd_final = pd.DataFrame(columns=['Frame', 'Target_frequency', 'Frequency', 'AF7', 'Fp1', 'Fp2', 'AF8', 'F3', 
                                      'F4', 'P3', 'P4', 'PO7', 'O1', 'O2', 'PO8'])


eeg_dataset_orig = pd.read_csv('path_to_training_file.csv',  index_col=0)
eeg_dataset_orig.rename(columns={'Event Id': 'event_id'}, inplace=True)
eeg_dataset_orig.index.name = 'timestamp'
eeg_dataset_orig = eeg_dataset_orig.drop(['Epoch', 'Event Date', 'Event Duration'], axis=1)
eeg_stimuli_dataset = eeg_dataset_orig[eeg_dataset_orig['event_id'].isin([33025, 33026, 33027])]
eeg_stimuli_dataset = eeg_stimuli_dataset.loc[:, ['event_id']]

columns = ['AF7', 'Fp1', 'Fp2', 'AF8', 'F3', 'F4', 'P3', 'P4', 'PO7', 'O1', 'O2', 'PO8', 'event_id']
frame = []

for timestamp_real, row in eeg_stimuli_dataset.iterrows():
    frame_ = eeg_dataset_orig.iloc[(eeg_dataset_orig.index>=timestamp_real+1) & (eeg_dataset_orig.index<timestamp_real+8)]
    print(frame_.columns)
    frame_.columns = columns
    print(frame_.columns)
    if int(row['event_id']) == 33025:
        frame_['event_id'] = 20
    elif int(row['event_id']) == 33026:
        frame_['event_id'] = 15
    else:
        frame_['event_id'] = 12
    frame.append(frame_)


path_PSD = path + 'PSD\\'
for eeg_signal_ in frame:
    target_frequency = eeg_signal_['event_id'].iloc[0]
    print(target_frequency)
    no_target_frequencies = [x for x in system_frequencies if x != target_frequency]
    print(no_target_frequencies)
    
    n_channels = eeg_signal_.shape[1] - 1
    L = len(eeg_signal_)  # Number of samples in the signal
    t = np.linspace(0.0, L*T, L, endpoint=False)  # Time vector
    psd_list = []

    target_psd_list = [frame_number, target_frequency, target_frequency]
    no_target_1_psd_list = [frame_number, target_frequency]
    no_target_2_psd_list = [frame_number, target_frequency]
    j = 0

    # Loop over each EEG channel
    for channel in range(n_channels):
        # Apply Welch's method to each channel
        eeg_signal = (eeg_signal_.iloc[:, channel])
        print(eeg_signal_.columns.values[channel])
        filtered_signal = bandpass_filter(eeg_signal, lowcut, highcut, fs)
        filtered_signal = notch_filter(filtered_signal, fs, notch_freq, quality_factor)
        eeg_signal = filtered_signal
        frequencies, psd_ = welch(eeg_signal, fs, nperseg=1024, noverlap=768, window='hann', scaling='density')
        
        # Store the PSD result
        psd_list.append(psd_)
    
        # Save PSD figures
        plt.figure(figsize=(14, 6))
        plt.plot(frequencies, psd_)
        plt.title('Power Spectral Density (PSD) - ' + 'Channel: ' + eeg_signal_.columns.values[channel] + ' - Target Frequency: ' + str(target_frequency) + ' Hz')
        plt.xlabel('Frequency [Hz]')
        plt.ylabel('PSD [uV**2/Hz]')
        plt.xlim(0, 50)
        plt.xticks(np.arange(0, 50, 1.0))
        plt.grid(True)
        plt.savefig(path_PSD + 'PSD_' + eeg_signal_.columns.values[channel] + '_' + str(target_frequency) + '_' + str(frame_number) + '.png')
        plt.close()

        # Extract the top 5 frequencies and their corresponding amplitudes
        top_5_indices_w = np.argsort(psd_)[-5:][::-1]
        top_5_frequencies_w = frequencies[top_5_indices_w]
        top_5_amplitudes_w = psd_[top_5_indices_w]
        
        # Print the top 5 frequencies with the maximum amplitudes
        print("Top 5 Frequencies with Maximum Amplitudes WELCH:")
        for i in range(5):
            print(f"Frequency {i+1}: {top_5_frequencies_w[i]:.2f} Hz, Amplitude: {top_5_amplitudes_w[i]:.2f}")
        freq_indices_t = np.where((frequencies >= target_frequency - delta_frequency) & (frequencies <= target_frequency + delta_frequency))
        freq_indices_t_h1 = np.where((frequencies >= 2 * target_frequency - delta_frequency) & (frequencies <= 2 * target_frequency + delta_frequency)) 
        freq_indices_target = np.concatenate((freq_indices_t, freq_indices_t_h1), axis=None)
        
        # Calculate the total power in the desired frequency range
        total_power_target = bandpower(psd_, [target_frequency - delta_frequency, target_frequency + delta_frequency], frequencies) + bandpower(psd_, [2 * target_frequency - delta_frequency, 2 * target_frequency + delta_frequency], frequencies)
        print("total power TARGET", target_frequency, total_power_target)
        target_psd_list.append(total_power_target)
        
        i = 0
        
        for f in no_target_frequencies:
            freq_indices_f = np.where((frequencies >= f - delta_frequency) & (frequencies <= f + delta_frequency))
            freq_indices_h1 = np.where((frequencies >= 2 * f - delta_frequency) & (frequencies <= 2 * f + delta_frequency)) 
            freq_indices = np.concatenate((freq_indices_f, freq_indices_h1), axis=None)

            no_target_power = bandpower(psd_, [f - delta_frequency, f + delta_frequency], frequencies) + bandpower(psd_, [2 * f - delta_frequency, 2 * f + delta_frequency], frequencies)
            print("no target power", f, no_target_power)
            if i == 0:
                if j == 0:
                    no_target_1_psd_list.append(f)
                    j = j + 1
                no_target_1_psd_list.append(no_target_power)
            else:
                if j == 1:
                    no_target_2_psd_list.append(f)
                    j = j + 1
                no_target_2_psd_list.append(no_target_power)
            
            i = i + 1
    
    psd_final.loc[len(psd_final)] = target_psd_list
    psd_final.loc[len(psd_final)] = no_target_1_psd_list
    psd_final.loc[len(psd_final)] = no_target_2_psd_list
    
    frame_number =  frame_number + 1
    
    
psd_final.to_csv(path + 'psd_training_' + str(delta_frequency) + '.csv', index = False)

path_bp = path + 'Bandpower\\'
for p in range (24):
    plt.figure(figsize=(14, 6))
    plt.plot(psd_final.loc[p * 3][3:15], 'b', label = 'Target frequency: ' + str(int(psd_final.iloc[p * 3]['Frequency'])) + ' Hz')
    plt.plot(psd_final.loc[p * 3 + 1][3:15], 'r', label = 'No target frequency: ' + str(int(psd_final.iloc[p * 3 + 1]['Frequency'])) + ' Hz')  
    plt.plot(psd_final.loc[p * 3 + 2][3:15], 'g', label = 'No target frequency: ' + str(int(psd_final.iloc[p * 3 + 2]['Frequency'])) + ' Hz')  
    plt.title('Bandpower')
    plt.xlabel('Channels')
    plt.ylabel('uV**2')
    plt.grid(True)
    plt.legend()
    plt.savefig(path_bp + 'bandpower_' + str(int(psd_final.iloc[p * 3]['Frequency'])) + '_' + str(p) + '.png')
    plt.close()



