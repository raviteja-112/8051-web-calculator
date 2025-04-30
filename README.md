# Web-Based 8051 Timer Calculator Using ESP8266

## Abstract

This project presents a web-based solution for calculating timer register values (TH, TL, TMOD) and frequency for the 8051 microcontroller, hosted on an ESP8266 Wi-Fi module. The system features a user-friendly website that allows users to input a desired frequency to compute the corresponding TH, TL, and TMOD values for the 8051's timer configuration, or to input TH, TL, and timer mode to determine the output frequency. Leveraging the ESP8266's ability to host a web server, the platform provides real-time calculations accessible via any Wi-Fi-enabled device. The solution aims to eliminate manual computation errors, reduce configuration time, and serve as an educational tool for students and hobbyists learning 8051 microcontroller programming. This project integrates embedded systems, web development, and microcontroller fundamentals, demonstrating a practical application of IoT technology.

## Features

*   **Frequency to Timer Values:** Input crystal frequency, timer selection, mode, and desired frequency to calculate TH, TL, and TMOD registers.
*   **Timer Values to Frequency:** Input crystal frequency, timer selection, mode, TH, and TL values to calculate the resulting output frequency.
*   **Web Interface:** Accessible via any web browser on a device connected to the ESP8266's Wi-Fi Access Point.
*   **Real-time Calculation:** Computes values directly on the ESP8266.
*   **User-Friendly:** Simple interface with clear input fields and results display.
*   **Input Validation:** Basic checks for valid frequency and hexadecimal inputs.

## Technology Stack

*   **Hardware:** ESP8266 Wi-Fi Module
*   **Firmware:** C++ (Arduino Core for ESP8266)
*   **Web Server:** ESP8266WebServer library
*   **Frontend:** HTML, CSS, JavaScript

## How to Use

1.  **Power Up:** Supply power to the ESP8266 module flashed with the project code.
2.  **Connect Wi-Fi:** On your computer or mobile device, connect to the Wi-Fi network named `MyESP8266_AP` with the password `password123`.
3.  **Access Web Page:** Open a web browser and navigate to the IP address `http://192.168.4.1`.
4.  **Use Calculator:**
    *   Select either "Frequency To TH/TL" or "TH/TL To Frequency" tab.
    *   Enter the required parameters (Crystal Frequency, Timer, Mode, Desired Frequency or TH/TL values).
    *   Click "Calculate" to see the results.
    *   Use "Reset" to clear the fields for the current tab.
