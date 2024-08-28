# ESP32AP

This is an ESP32 Access Point code.

## Description

This project sets up an ESP32 microcontroller as a Wi-Fi access point, allowing other devices to connect to it. It can be used for various IoT applications where a local network is required without relying on external Wi-Fi infrastructure.

You can use the ![ESP32TCP-IP](https://github.com/Xtalism/ESP32TCP-IP) code I have in my repository so you can connect to the access point whenever you like. This code also works with multiple ESP32'S. If you'd like to add more than 2 ESP32'S you'd need to change the ![ESP32AP](https://github.com/Xtalism/ESP32AP) code in order to add as much clients as you'd like it to be.

## Documentation

You can look up the ![WiFi API](https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html) documentation which is the one I based my code on.

You can also see the ![TCP/IP Adapter](https://docs.espressif.com/projects/esp-idf/en/release-v3.3/api-reference/network/tcpip_adapter.html) for more details.