# IoT Water Quality Monitoring System

This repository contains the source code and documentation for an IoT water quality monitoring system. The system utilizes various sensors to measure water quality parameters such as gravity TDS (Total Dissolved Solids), turbidity, pH level, and temperature. The collected data is sent to a remote server using a SIM800L module every 15 minutes.

## Features

- Measures water quality parameters using multiple sensors.
- Arduino Uno used for processing and data collection.
- SIM800L module for sending data to a remote server.
- Digital temperature sensor (DS18B20) for measuring water temperature.
- Regular data transmission to ensure up-to-date monitoring.

## Components

- Arduino Uno
- Gravity TDS Sensor
- Turbidity Sensor
- pH Meter
- DS18B20 Digital Temperature Sensor
- SIM800L GSM/GPRS Module

## Usage

1. **Hardware Setup:** Connect the sensors (TDS, turbidity, pH, and DS18B20) to the Arduino Uno according to the provided specifications.

2. **Software Setup:** Upload the Arduino sketch provided in this repository to your Arduino Uno using the Arduino IDE.

3. **Configuration:** Modify the code to set the appropriate parameters such as server details, data transmission interval, and sensor calibration.

4. **Remote Server:** Ensure that you have a remote server ready to receive the data transmitted by the system.

5. **Power Supply:** Provide a suitable power supply for the Arduino and SIM800L module.

6. **Monitor Data:** The system will collect and send water quality data to the remote server at the specified interval. Monitor the data on the server to assess water quality.

## Contributions

Contributions to this project are welcome! If you have suggestions for improvements, open an issue or submit a pull request.

## License

This project is licensed under the [MIT License](LICENSE).

## Acknowledgments

- The project makes use of various open-source libraries and resources.
- Special thanks to the Arduino community for their valuable contributions and support.

---

Feel free to customize the above template with more specific details about your project, such as installation instructions, troubleshooting, and any additional features or functionalities. Add images, diagrams, and code snippets to enhance the documentation and make it more informative for users who visit your GitHub repository.
