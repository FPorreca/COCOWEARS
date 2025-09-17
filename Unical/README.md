This repository encapsulates the distilled outcome of research (COCOWEARS) conducted to develop a modular and scalable framework for next-generation wearable systems.
As a primary use case, the project focuses on a system for indoor localization and monitoring of individuals, leveraging Ultra-Wideband (UWB) positioning and inertial sensing technologies. However, the architecture has been deliberately designed to support extensibility, enabling the development of a wide range of wearable applications beyond the initial scope.
By integrating hardware acceleration on FPGA-based platforms and organizing the project into distinct functional modules—including firmware for embedded devices, IP core generation for computational tasks, and control libraries for peripheral management—the framework simplifies the development process while enhancing system performance and adaptability.
Beyond offering a functional implementation, this repository serves as a robust foundation for ongoing research and prototyping in advanced wearable technology applications.


📂 arduino_fw
This directory contains firmware components intended for deployment on ESP32-based boards by Makerfabs, used for UWB-based localization and inertial data acquisition.
-	ANCHOR_fw
Firmware for anchor nodes, designed for Makerfabs ESP32 UWB boards. These anchors serve as fixed reference points in the localization infrastructure.
-	TAG_fw
Firmware for mobile tags, compatible with Makerfabs ESP32 UWB Pro boards equipped with OLED displays. Tags collect positioning data and transmit it via SPI to the Zynq platform.
⚠️ Note: Both anchor and tag firmware require configuration prior to deployment. They are not plug-and-play and must be adapted to the specific infrastructure layout and operational parameters.
-	datalogger
A firmware module responsible for logging raw data from the BNO055 sensor (magnetometer, accelerometer, gyroscope) to an SD card. The resulting dataset is essential for training and evaluating the k-Nearest Neighbors (kNN) classification algorithm.
-	kNN
A firmware module designed to execute the k-Nearest Neighbors (kNN) classification algorithm directly on ESP32-based boards. It processes the sensor data acquired from the BNO055 module and performs real-time classification tasks, enabling on-board decision-making without the need for external computation.


📂 HW_accelerator
This directory contains the VHDL-based hardware accelerators developed to offload computational tasks from the Zynq processor, improving performance and efficiency.

-	kNN
Includes all necessary files for generating an IP core that performs kNN classification based on data acquired from the BNO055 sensor via I²C.
- The dataset.coe file contains the preprocessed dataset exported from the datalogger and formatted for BRAM initialization.
- The testbench subfolder provides a complete VHDL testbench for validating the IP core functionality.
-	Trilateration
Contains all files required to generate an IP core for trilateration-based position estimation using UWB data received from the TAG via SPI.
- The testbench subfolder includes the VHDL testbench for simulation and verification.
- The results subfolder stores output data from testbench runs, useful for performance analysis and validation.
-	IIR Filter
This module contains multiple hardware implementations of Infinite Impulse Response (IIR) filters, designed for deployment on FPGA platforms. The folder is organized into four subdirectories, each representing a distinct architectural approach:
•	DFI: Implements the IIR filter using Direct Form I, a straightforward structure suitable for general-purpose filtering tasks.
•	SOS: Implements the filter using Second-Order Sections, which improve numerical stability.
•	SOS_LAT: A variant of the SOS architecture that incorporates scattered lookahead optimization, enhancing parallelism and throughput in hardware execution.
•	test: Contains stimulus files used to validate the correctness and performance of each filter architecture.
These implementations are designed to be modular and reusable, allowing seamless integration into larger signal processing pipelines. The inclusion of multiple design strategies enables comparative analysis in terms of resource usage, latency, and numerical precision, making this module a valuable asset for researchers and developers working on embedded DSP systems.


📂 Library for Zynq
This folder includes custom-developed drivers and control libraries for interfacing with peripheral modules:
- BNO055 (MIMU sensor)
- SSD1306 (OLED display)
These libraries are based on official APIs and adapted from Adafruit’s Arduino libraries, ensuring compatibility and ease of integration within the Zynq ecosystem.


📂 zynq_fw
Firmware modules for the Zynq platform, enabling execution of localization and classification algorithms either on the processing system (PS) or via programmable logic (PL).
- only_zynq
Implements trilateration and kNN algorithms entirely on the Zynq processing system, without relying on external hardware accelerators. 
- with_hw_acc
Integrates hardware accelerators from the HW_accelerator directory to execute trilateration and kNN computations on dedicated IP cores.
- 🛠️ Note: To use this configuration, users must first generate the IP cores, register them in the IP catalog, and construct a Block Design that includes all required components for system operation.


🧠 Project Scope and Contributions

This repository reflects a substantial engineering effort encompassing:

•	Embedded firmware development for UWB and inertial sensing platforms;
•	Real-time data acquisition and logging;
•	Hardware acceleration via custom VHDL IP cores;
•	Integration with Zynq SoC platforms using both PS and PL;
•	Development of reusable libraries for sensor and display control;
•	Simulation and validation through structured testbenches;
•	Dataset preparation and formatting for machine learning applications;

The modular architecture ensures scalability and adaptability to various applications, making this repository a valuable resource for researchers and developers working in wearable systems. 

