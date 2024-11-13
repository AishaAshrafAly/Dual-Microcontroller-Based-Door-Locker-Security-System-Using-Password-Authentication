# Dual-Microcontroller-Based-Door-Locker-Security-System-Using-Password-Authentication
This project implements a smart door security system using two microcontrollers (HMI_ECU and Control_ECU), with UART communication between them. The system utilizes password authentication, motorized door control, and several additional features like a PIR sensor, EEPROM for password storage, and a buzzer for alerts.

The system allows users to set, change, and verify passwords via an LCD and keypad interface. It controls a motor for unlocking/locking the door, uses a PIR sensor to detect motion, and provides an enhanced security mechanism through password protection and time-based lockout after multiple failed attempts.

Key Features:
- Password Authentication: Set, verify, and change passwords using an LCD and keypad.
- UART Communication: Data transfer between HMI_ECU and Control_ECU for secure operation.
- External EEPROM Storage: Secure storage of password data.
- Motorized Door Control: Uses H-bridge motor control to operate the door.
- PIR Motion Sensor: Detects motion to control door locking and unlocking.
- Security Lockout: System locks for 1 minute after 3 consecutive incorrect password attempts.
- Buzzer Alert: Alerts for failed attempts and system warnings.

Hardware Components:
- Microcontroller: ATmega32
- Keypad (4x4)
- LCD (2x16)
- External EEPROM (I2C)
- H-Bridge Motor Driver
- PIR Sensor
- DC Motor
- Buzzer

Software Requirements:
- UART Communication: For data exchange between microcontrollers.
- EEPROM: For password storage using I2C.
- PWM: For motor control.
- Timer: For timed actions like motor operation and lockout duration.

System Architecture:
The system follows a layered architecture with the following components:

1. HMI_ECU: Handles user interaction (keypad, LCD).
2. Control_ECU: Manages motor control, security logic, PIR sensor input, and EEPROM communication.

Driver Requirements:
- GPIO, UART, I2C, PWM, Timer, Buzzer, PIR, DC Motor, and EEPROM drivers.

Operation Flow:
1. Create a system password.
2. Unlock the door with password authentication.
3. Change the system password (after re-authentication).
4. Lock the system for 1 minute after 3 failed password attempts.
5. Use PIR sensor to detect motion and control door status.
