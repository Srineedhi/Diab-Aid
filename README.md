# Diab-Aid — SmartSense FootGuard

### Smart Footwear Monitoring for Diabetic Foot Care

## Overview

Diab-Aid, also known as SmartSense FootGuard, is a sensor-assisted footwear monitoring system designed to track selected foot pressure and temperature parameters. The system aims to improve awareness of unusual readings and encourage timely foot inspection among people with diabetes.

The prototype combines pressure sensors, temperature sensing, an ESP32 microcontroller, and a web-based dashboard to collect, process, and visualize monitoring data.

**Note:** This project is intended for monitoring and educational purposes. It is not a medical diagnostic device and does not replace professional medical advice.

## Objectives

- Monitor pressure distribution across selected regions of the foot.
- Measure foot-related temperature variations.
- Collect sensor readings using an ESP32 microcontroller.
- Present monitoring data through a web-based dashboard.
- Encourage regular foot inspection and awareness of potentially unusual readings.

## Key Features

- **Pressure monitoring:** Uses 24 force-sensitive resistor (FSR) sensors to capture pressure-related measurements.
- **Temperature monitoring:** Includes a temperature sensor to record temperature readings.
- **Embedded processing:** Uses an ESP32 for sensor interfacing and data communication.
- **Web dashboards:** Provides separate interfaces for patient and administrative monitoring.
- **Data visualization:** Displays collected readings to support observation of pressure and temperature patterns.
- **Expandable architecture:** Provides a foundation for future alerts, historical analysis, and personalized monitoring.

## System Architecture

1. **Sensing Layer:** FSR sensors and a temperature sensor collect measurements.
2. **Embedded Layer:** The ESP32 interfaces with the sensors and handles data acquisition.
3. **Communication Layer:** Transfers supported readings to the application backend.
4. **Application Layer:** Web interfaces present available monitoring information.
5. **Data Layer:** Supabase can be used for supported data storage and retrieval, depending on the configured deployment.

## Technology Stack

- **Hardware:** ESP32, 24 FSR sensors, temperature sensor
- **Frontend:** HTML, CSS, JavaScript
- **Backend / Integration:** JavaScript and Python utilities
- **Database / Services:** Supabase
- **Firmware:** Arduino-compatible ESP32 firmware

## Project Structure

```text
DIAb-supabase/
├── admin-dashboard.html
├── app.js
├── esp32-firmware.ino
├── footguard-dashboard.html
├── index.html
├── patient-dashboard.html
├── styles.css
├── supabase-config.js
├── thermal_foot_heatmap.png
├── update_dashboards.py
├── update_sc_dashboards.py
└── src/
```

## Getting Started

### Prerequisites

- A modern web browser
- Visual Studio Code
- Node.js and npm, if required by the project setup
- Arduino IDE or compatible tooling for ESP32 firmware
- A configured Supabase project, if using its backend services

### Run the Web Application

1. Clone or download this repository.
2. Open the project folder in Visual Studio Code.
3. Review `supabase-config.js` and configure the required Supabase connection using appropriate public client credentials.
4. Do not commit private keys, service-role keys, passwords, or `.env` files.
5. Open `index.html` using a local development server, such as the VS Code Live Server extension, if supported by the application.

### ESP32 Setup

1. Open `esp32-firmware.ino` in Arduino IDE.
2. Install the required board support and libraries.
3. Configure the appropriate network and backend settings.
4. Connect the sensors according to the actual circuit and pin assignments.
5. Upload the firmware to the ESP32 and verify sensor readings.

Refer to the source code for the exact pin configuration, communication method, and backend requirements.

## Expected Outcome

The prototype is intended to collect foot-pressure and temperature measurements and present supported readings through a web dashboard. It provides a basis for observing measurement patterns and developing future monitoring capabilities.

Any thresholds, alerts, or interpretations require appropriate validation. The prototype must not be used to diagnose diabetic foot complications.

## Limitations

- Sensor accuracy depends on calibration, placement, and contact conditions.
- Temperature readings can be affected by ambient conditions.
- The system does not independently diagnose foot ulcers or predict their occurrence.
- Clinical effectiveness and alert thresholds have not been established by this project description.
- Data availability depends on the configured hardware and backend integration.

## Future Enhancements

- Historical pressure and temperature trend analysis.
- Configurable alerts for unusual readings after suitable validation.
- Improved sensor calibration and repeatability testing.
- Enhanced mobile-responsive dashboards.
- Secure authentication and access control.
- Validation with qualified healthcare professionals and appropriate study protocols.

## Contributors

**Srineedhi R** and project team.

## Disclaimer

Diab-Aid / SmartSense FootGuard is a prototype intended for educational and research purposes. It is not a substitute for medical assessment, diagnosis, or treatment. Users should consult qualified healthcare professionals regarding diabetic foot care.

## License

## License

This project is licensed under the MIT License.

Copyright (c) 2026 Srineedhi R and project contributors.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files, to deal in the Software without restriction, including the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, subject to the inclusion of the copyright and permission notices.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT.
