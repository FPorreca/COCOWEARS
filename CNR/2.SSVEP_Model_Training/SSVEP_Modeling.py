import pandas as pd
import numpy as np
import setuptools.dist
import tensorflow as tf
from tensorflow.keras import optimizers
from sklearn.impute import SimpleImputer
from tensorflow.keras.models import Sequential
from sklearn.preprocessing import StandardScaler
from sklearn.model_selection import train_test_split
from tensorflow.keras.callbacks import EarlyStopping
from sklearn.model_selection import cross_val_predict, cross_val_score, StratifiedKFold
from sklearn.metrics import accuracy_score, classification_report
from scipy.signal import butter, filtfilt, iirnotch
from scipy.signal import welch
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score
from sklearn.discriminant_analysis import LinearDiscriminantAnalysis
from sklearn.svm import SVC
from sklearn.ensemble import RandomForestClassifier
from sklearn.decomposition import PCA
from sklearn.neighbors import KNeighborsClassifier
from sklearn.tree import DecisionTreeClassifier
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Conv1D, MaxPooling1D, Flatten, Dense, Dropout






# Define the local file paths for your CSV files
training_data = [
    r'Path_to_training_file_1.csv',
    r'Path_to_training_file_2.csv',
    r'Path_to_training_file_n.csv'
]

# EEG signal parameters
fs = 256  # Sampling frequency in Hz (common sampling rate for EEG signals)
T = 1.0 / fs  # Sample interval
lowcut = 6  # Low cut-off frequency (in Hz)
highcut = 30  # High cut-off frequency (in Hz)
notch_freq = 50.0  # Frequency to be removed (Hz)
quality_factor = 30.0  # Quality factor of the notch filter
window_shift = 0.25  # shift in secondi da una finesta a quella successiva
window_width = 2   # larghezza in secondi della finestra su cui effettuare la classificazione



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


def compute_psd(eeg_data, fs=256, nperseg=256):
    """
    Compute the Power Spectral Density (PSD) for each channel of EEG data.
    :param eeg_data: EEG data of shape (batch_size, time_steps, input_features)
    :param fs: Sampling frequency (Hz)
    :return: PSD of shape (batch_size, psd_freq_bins, input_features)

    :welch params nperseg and noverlap refers to: Length of each segment and onverlap between segments for Welch's method
    """
    batch_size, time_steps, input_features = eeg_data.shape
    psd_list = []

    for i in range(batch_size):
        psd_channels = []
        for ch in range(input_features):
            freqs, psd = welch(eeg_data[i, :, ch], fs=fs, nperseg=256, noverlap=192)
            psd_channels.append(psd[0:50])
        psd_list.append(np.array(psd_channels).T)  # Shape (psd_freq_bins, input_features)

    psd_array = np.array(psd_list)  # Shape (batch_size, psd_freq_bins, input_features)
    return psd_array


def cross_validation_report(model, X, y):
    # Define cross-validation strategy
    cv = StratifiedKFold(n_splits=5, shuffle=True, random_state=42)

    # Calculate overall accuracy using cross_val_score
    overall_accuracies = cross_val_score(model, X, np.ravel(y), cv=cv, scoring='accuracy')
    print(f"\nOverall Accuracy Across Folds: {overall_accuracies.mean():.2%}")

    # Obtain predictions using cross_val_predict for class-wise accuracy
    y_pred = cross_val_predict(model, X, np.ravel(y), cv=cv)

    # Compute class-wise accuracy
    classes = np.unique(np.ravel(y))
    class_accuracies = {}

    for cls in classes:
        # True samples of the current class
        true_samples = np.ravel(y) == cls
        # Correct predictions for the current class
        correct_predictions = (y_pred == cls) & true_samples
        # Class-wise accuracy
        class_accuracies[cls] = correct_predictions.sum() / true_samples.sum()

    # Display results
    print("\nClass-Wise Accuracy Across All Folds:")
    for cls, acc in class_accuracies.items():
        print(f"Class {cls}: {acc:.2%}")

    # Classification report (optional)
    print("\nDetailed Classification Report Across All Folds:")
    print(classification_report(y, y_pred))


def cross_validation_report_CNN(model_cross, X, y):
    kf = StratifiedKFold(n_splits=5, shuffle=True, random_state=42)

    overall_accuracies = []
    all_y_true = []
    all_y_pred = []

    for train_idx, test_idx in kf.split(X, y):
        X_train, X_test = X[train_idx], X[test_idx]
        y_train, y_test = y[train_idx], y[test_idx]

        scaler = StandardScaler()
        scaler.fit_transform(X_train)
        scaler.transform(X_test)

        X_train = X_train.reshape(X_train.shape[0], psd_eeg.shape[1], psd_eeg.shape[2])
        X_test = X_test.reshape(X_test.shape[0], psd_eeg.shape[1], psd_eeg.shape[2])

        CNN_model = tf.keras.models.clone_model(model_cross)
        CNN_model.compile(optimizer='adam', loss='sparse_categorical_crossentropy', metrics=['accuracy'])
        CNN_model.fit(X_train, y_train, epochs=20, batch_size=4, verbose=0)  # Reduce epochs for testing

        y_pred_proba = CNN_model.predict(X_test)
        y_pred = np.argmax(y_pred_proba, axis=1)

        accuracy = np.mean(np.ravel(y_pred) == np.ravel(y_test))
        overall_accuracies.append(accuracy)

        all_y_true.extend(np.ravel(y_test))
        all_y_pred.extend(np.ravel(y_pred))

    print(f"\nOverall Accuracy Across Folds: {np.mean(overall_accuracies):.4f}")

    # Compute class-wise accuracy
    classes = np.unique(np.ravel(y))
    class_accuracies = {}

    for cls in classes:
        # True samples of the current class
        true_samples = np.array(np.ravel(all_y_true)) == cls
        # Correct predictions for the current class
        correct_predictions = (np.array(np.ravel(all_y_pred)) == cls) & true_samples
        # Class-wise accuracy
        class_accuracies[cls] = correct_predictions.sum() / true_samples.sum()

    # Display results
    print("\nClass-Wise Accuracy Across All Folds:")
    for cls, acc in class_accuracies.items():
        print(f"Class {cls}: {acc:.2%}")


# Load data from CSV files
data = []
training_index = 0
for path in training_data:
    df = pd.read_csv(path, index_col=0)
    df.index = df.index + training_index * 1000
    print(f"Loaded data from: {path}")
    data.append(df)
    training_index = training_index + 1


eeg_dataset_orig = pd.concat(data)
eeg_dataset_orig.rename(columns={'Event Id': 'event_id'}, inplace=True)
eeg_dataset_orig.index.name = 'timestamp'
eeg_dataset_orig = eeg_dataset_orig.drop(['Epoch', 'Event Date', 'Event Duration'], axis=1)
eeg_stimuli_dataset = eeg_dataset_orig[eeg_dataset_orig['event_id'].isin([33024, 33025, 33026, 33027])]
eeg_stimuli_dataset = eeg_stimuli_dataset.loc[:, ['event_id']]

columns = ['AF7', 'Fp1', 'Fp2', 'AF8', 'F3', 'F4', 'P3', 'P4', 'PO7', 'O1', 'O2', 'PO8', 'event_id']
frame = []
y_list = []
for timestamp_real, row in eeg_stimuli_dataset.iterrows():
    frame_ = eeg_dataset_orig.iloc[(eeg_dataset_orig.index>=timestamp_real+1) & (eeg_dataset_orig.index<timestamp_real+8)]
    frame_.columns = columns
    if int(row['event_id']) == 33025:
        y_list.append(1)
    elif int(row['event_id']) == 33026:
        y_list.append(2)
    elif int(row['event_id']) == 33027:
        y_list.append(3)
    else:
        y_list.append(0)

    frame.append(frame_)


frame_filtered = []
frame_number = 0
y_ = []

for frame_test in frame:
    y_value = y_list[frame_number]
    n_channels = frame_test.shape[1] - 1
    index = 0
    

    while (index * round(window_shift * fs) + window_width * fs) <= 1792:
        frame_filtered_ = pd.DataFrame(columns = ['AF7', 'Fp1', 'Fp2', 'AF8', 'F3', 'F4', 'P3', 'P4', 'PO7', 'O1', 'O2', 'PO8', 'event_id'])
        for channel in range(n_channels):
            eeg_signal = frame_test.iloc[index * round(window_shift * fs): window_width * fs + index * round(window_shift * fs), channel]
            filtered_signal = bandpass_filter(eeg_signal, lowcut, highcut, fs)
            filtered_signal = notch_filter(filtered_signal, fs, notch_freq, quality_factor)
            frame_filtered_[frame_test.columns.values[channel]] = filtered_signal
        frame_filtered.append(frame_filtered_)
        y_.append(y_list[frame_number])
        index += 1
    frame_number += 1


frame_df = pd.concat(frame_filtered)

# Select X data based on electrodes of interest and window_width
X = frame_df.iloc[:, 8:-1].values.reshape(-1, 512, 4)   # PO7, 01, 02, PO8
#X = frame_df.iloc[:, 9:-2].values.reshape(-1, 512, 2)   # 01, 02
#X = frame_df.iloc[:, 9:-3].values.reshape(-1, 512, 1)   # 01
#X = frame_df.iloc[:, 10:-2].values.reshape(-1, 512, 1)   # 02
#X = frame_df.iloc[:, :-1].values.reshape(-1, 1792, 12)  # All channels
#X = frame_df.iloc[:, 8:-1].values.reshape(-1, 1792, 4)   # PO7, 01, 02, PO8
#X = frame_df.iloc[:, 8:-1].values.reshape(-1, 256, 4)   # PO7, 01, 02, PO8
#X = frame_df.iloc[:, 9:-3].values.reshape(-1, 1792, 1)   # 01
#X = frame_df.iloc[:, 10:-2].values.reshape(-1, 1792, 1)   # 02

y = np.array(y_).reshape(len(y_), 1)

# Compute the PSD for each acquisition
psd_eeg = compute_psd(X)
X_psd = psd_eeg.reshape(psd_eeg.shape[0], -1)
 #X = X_psd.copy()
X_train, X_test, y_train, y_test = train_test_split(X_psd, y, test_size=0.2, stratify=y, random_state=42)


# Train and evaluate LDA Model
lda = LinearDiscriminantAnalysis()
lda.fit(X_train, np.ravel(y_train))
y_pred = lda.predict(X_test)
accuracy = accuracy_score(np.ravel(y_test), y_pred)
print(f"LDA Accuracy: {accuracy * 100:.2f}%")
cross_validation_report(LinearDiscriminantAnalysis(), X, y)

# Train and evaluate SVM Model
svm = SVC(kernel='rbf', C=5)
svm.fit(X_train, np.ravel(y_train))
y_pred = svm.predict(X_test)
accuracy = accuracy_score(np.ravel(y_test), y_pred)
print(f"SVM Accuracy: {accuracy * 100:.2f}%")
cross_validation_report(SVC(kernel='rbf', C=5), X, y)


# PCA dimensionality reduction
pca = PCA(n_components=10)  # Adjust based on desired variance explained
X_train_pca = pca.fit_transform(X_train)
X_test_pca = pca.transform(X_test)

# Train and evaluate LDA Model with PCA-reduced features
lda_pca = LinearDiscriminantAnalysis()
lda_pca.fit(X_train_pca, np.ravel(y_train))
y_pred_pca = lda_pca.predict(X_test_pca)
accuracy_pca = accuracy_score(np.ravel(y_test), y_pred_pca)
print(f"LDA with PCA Accuracy: {accuracy_pca * 100:.2f}%")
cross_validation_report(LinearDiscriminantAnalysis(), pca.fit_transform(X), y)

# Train and evaluate SVM Model with PCA-reduced features
svm_pca = SVC(kernel='rbf', C=5)
svm_pca.fit(X_train_pca, np.ravel(y_train))
y_pred_svm_pca = svm_pca.predict(X_test_pca)
accuracy_svm_pca = accuracy_score(np.ravel(y_test), y_pred_svm_pca)
print(f"SVM with PCA Accuracy: {accuracy_svm_pca * 100:.2f}%")
cross_validation_report(SVC(kernel='rbf', C=5), pca.fit_transform(X), y)

# Train and evaluate KNN Model
knn = KNeighborsClassifier(n_neighbors=5)
knn.fit(X_train, np.ravel(y_train))
y_pred_knn = knn.predict(X_test)
accuracy_knn = accuracy_score(y_test, y_pred_knn)
print(f"kNN Accuracy: {accuracy_knn * 100:.2f}%")
cross_validation_report(KNeighborsClassifier(n_neighbors=5), X, y)

# Train and evaluate DECISION TREE Model
tree_clf = DecisionTreeClassifier(max_depth=20)  # Adjust max_depth for control over complexity
tree_clf.fit(X_train, np.ravel(y_train))
y_pred = tree_clf.predict(X_test)
accuracy = accuracy_score(np.ravel(y_test), y_pred)
print(f"Decision Tree Accuracy: {accuracy * 100:.2f}%")
cross_validation_report(DecisionTreeClassifier(max_depth=20), X, y)

# Train and evaluate RANDOM FOREST Model
rf_clf = RandomForestClassifier(n_estimators=300, max_depth=10, random_state=42)  # n_estimators is the number of trees
rf_clf.fit(X_train, np.ravel(y_train))
y_pred_rf = rf_clf.predict(X_test)
accuracy_rf = accuracy_score(y_test, y_pred_rf)
print(f"Random Forest Accuracy: {accuracy_rf * 100:.2f}%")
cross_validation_report(RandomForestClassifier(n_estimators=300, max_depth=10, random_state=42), X, y)



# Train and evaluate 1D-CNN Model
scaler = StandardScaler()
scaler.fit_transform(X_train)
scaler.transform(X_test)
X_train = X_train.reshape(X_train.shape[0], psd_eeg.shape[1], psd_eeg.shape[2])
X_test = X_test.reshape(X_test.shape[0], psd_eeg.shape[1], psd_eeg.shape[2])

# Define the CNN model for PSD input
model_psd = Sequential()
# Input shape: (psd_freq_bins, input_features) where input_features are the EEG channels
model_psd.add(Conv1D(filters=32, kernel_size=3, activation='relu', input_shape=(psd_eeg.shape[1], psd_eeg.shape[2])))
# MaxPooling to reduce the feature map size
model_psd.add(MaxPooling1D(pool_size=2))
# Additional Conv1D layer
model_psd.add(Conv1D(filters=64, kernel_size=3, activation='relu'))
# MaxPooling
model_psd.add(MaxPooling1D(pool_size=2))
# Dropout for regularization
model_psd.add(Dropout(0.5))
# Flatten the output
model_psd.add(Flatten())
# Fully connected layer
model_psd.add(Dense(128, activation='relu'))
# Output layer for multi-class classification (e.g., 4 classes)
model_psd.add(Dense(4, activation='softmax'))
# Compile the model
model_psd.compile(optimizer='adam', loss='sparse_categorical_crossentropy', metrics=['accuracy'])
# Print model summary
model_psd.summary()
CNN_model_cross = tf.keras.models.clone_model(model_psd)
model_psd.fit(X_train, y_train, epochs=20, batch_size=4)

loss, accuracy = model_psd.evaluate(psd_eeg_test, y_test, verbose=1)
print(f'Model - Accuracy: {accuracy}')

