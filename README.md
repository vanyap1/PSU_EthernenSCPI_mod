
# Power Supply Control Web Interface

## Overview

This repository contains a web interface for controlling a power supply unit (PSU) through a network. The interface allows you to monitor and adjust various settings and control different devices connected to the PSU.

## Command Descriptions

### 1. Get HTML Page
- **URL:** `GET /`
- **Description:** Retrieves the main HTML page of the device.

**Example URL:**
```
http://<your-device-ip>/
```

### 2. Set Values
- **URL:** `GET /set_vals?amp=<value>&volt=<value>`
- **Description:** Sets new values for current (ampere) and voltage.

**Example URL:**
```
http://<your-device-ip>/set_vals?amp=2.50&volt=12.00
```

### 3. Control Devices
- **URL:** `GET /control?device=<device>&action=<action>`
- **Description:** Controls various devices (fan, remote control, PSU).

**Examples:**

1. **Fan Control**
   - **Turn On Fan:**
     ```
     http://<your-device-ip>/control?device=fan&action=on
     ```
   - **Turn Off Fan:**
     ```
     http://<your-device-ip>/control?device=fan&action=off
     ```

2. **Remote Control**
   - **Enable Remote Control:**
     ```
     http://<your-device-ip>/control?device=rem&action=on
     ```
   - **Disable Remote Control:**
     ```
     http://<your-device-ip>/control?device=rem&action=off
     ```

3. **Power Supply Unit (PSU) Control**
   - **Turn On PSU:**
     ```
     http://<your-device-ip>/control?device=psu&action=on
     ```
   - **Turn Off PSU:**
     ```
     http://<your-device-ip>/control?device=psu&action=off
     ```

### 4. Get Values
- **URL:** `GET /get_vals`
- **Description:** Retrieves current values of all parameters from the device.

**Example URL:**
```
http://<your-device-ip>/get_vals
```

## How to Use

1. **Access the Web Interface:**
   Open a web browser and navigate to `http://<your-device-ip>/` to view the main control page.

2. **Set Values:**
   Use the `/set_vals` endpoint to update current and voltage settings. Replace `<value>` with the desired values for ampere and voltage.

3. **Control Devices:**
   Use the `/control` endpoint to manage different devices. Replace `<device>` with the device you want to control and `<action>` with the action to perform (e.g., `on`, `off`).

4. **Retrieve Values:**
   Use the `/get_vals` endpoint to fetch current values from the device.

## Example Responses

### Successful Response for Set Values
```
HTTP/1.1 200 OK
Content-Type: application/json

{"success":true}
```

### Successful Response for Control
```
HTTP/1.1 200 OK
Content-Type: application/json

{"success":true}
```

### JSON Response for Get Values
```
HTTP/1.1 200 OK
Content-Type: application/json

{
    "amp": 2.50,
    "volt": 12.00,
    "watt": 30.00,
    "ampDMM": 2.48,
    "voltDMM": 11.98,
    "outState": 1,
    "rem": 1,
    "err": 0,
    "in0": 1,
    "in1": 1,
    "in2": 1
}
```

Replace `<your-device-ip>` with the IP address of your device to use these commands.

---

Feel free to copy and paste this content into your README file on GitHub.