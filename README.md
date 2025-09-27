# PicoBoyOS

Operating system for the PicoBoy Project

## WiFi Credentials Notes

This project uses a `secrets.h` file to store your WiFi SSID and password. For security, this file is not included in the repository.

**Setup instructions:**

1. Rename `src/secrets.example.h` to `src/secrets.h`.
2. Edit `src/secrets.h` and enter your actual WiFi credentials:
    ```cpp
    const char* ssid = "YOUR_WIFI_SSID";
    const char* password = "YOUR_WIFI_PASSWORD";
    ```
3. Do not commit your real `secrets.h` to the repository.

Your code will not connect to WiFi until you update `secrets.h` with your credentials.
