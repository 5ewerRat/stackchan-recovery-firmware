# StackChan M5Stack CoreS3 Comprehensive User Manual

## Table of Contents
1. [Hardware Overview](#hardware-overview)
2. [System Specifications](#system-specifications)
3. [Firmware Development Setup](#firmware-development-setup)
4. [Safe Deployment Procedures](#safe-deployment-procedures)
5. [Bootloader Recovery Procedures](#bootloader-recovery-procedures)
6. [Communication Protocols](#communication-protocols)
7. [Memory Management](#memory-management)
8. [Troubleshooting Guide](#troubleshooting-guide)
9. [Maintenance Procedures](#maintenance-procedures)

---

## 1. Hardware Overview

### Main Controller
- **Processor**: ESP32-S3 SoC
  - Xtensa® dual-core 32-bit LX7 architecture
  - 240 MHz maximum clock speed
  - 8MB Quad PSRAM (PSRAM)
  - 16MB Flash memory

### Display System
- **Touch Display**: 2.0-inch capacitive touch screen
  - High-strength glass cover
  - Resolution: 320x240 pixels
  - Integrated touch controller

### Sensors and Components
- **Camera**: 0.3 MP camera module
- **IMU**: 9-axis motion sensor (accelerometer + gyroscope + magnetometer)
- **Ambient Light/Proximity Sensor**: Integrated environmental sensing
- **Audio**: Dual microphones and 1W speaker
- **Storage**: microSD card slot

### Robot Body Interface
- **Power Interface**: USB-C port for power and data
- **Battery**: 550 mAh internal battery
- **Servos**: Two feedback servos
  - Horizontal axis: 360-degree continuous rotation
  - Vertical axis: 90-degree range
- **LED System**: 12 RGB LEDs (2 rows, 6 LEDs each)
- **Communication**: Infrared transmitter and receiver
- **Touch Panel**: Three-zone touch interface
- **NFC**: Full-featured NFC module

### Wireless Connectivity
- **Wi-Fi**: 2.4 GHz, IEEE 802.11 b/g/n
- **Bluetooth**: Bluetooth® 5 Low Energy
- **Custom Protocol**: ESP-NOW wireless remote control support

### Physical Dimensions
- **Main Unit**: Compact CoreS3 form factor
- **Robot Body**: Lightweight construction with touch panels
- **Weight**: Optimized for desktop deployment

---

## 2. System Specifications

### Memory Specifications
| Component | Capacity | Type | Purpose |
|-----------|----------|------|---------|
| Flash Memory | 16MB | External | Firmware storage, OTA updates |
| PSRAM | 8MB | Quad PSRAM | Runtime memory, large buffers |
| SRAM | 512KB | Internal | Runtime stack and heap |

### Power Requirements
- **Input**: USB-C 5V/2A (recommended)
- **Battery**: 550 mAh Li-Po (internal)
- **Power Button**: Short press for on, 6-second hold for off
- **Reset Button**: RST button for system reset (microSD card slot area)

### I/O Interface Specifications
| Interface | Pins | Voltage Level | Protocol |
|----------|------|---------------|----------|
| UART | 1x (COM3) | 3.3V | Serial communication |
| I2C | 1x | 3.3V | Two-wire communication |
| SPI | 1x | 3.3V | High-speed peripheral interface |
| GPIO | Multiple | 3.3V | Digital I/O operations |
| ADC | 1x | 0-3.3V | Analog input |
| DAC | 1x | 0-3.3V | Analog output |

### Boot Configuration
- **Boot Mode Selection**: GPIO pin states during boot
- **Default Boot**: Application firmware
- **Download Mode**: Firmware flashing mode
- **Factory Reset**: Hardware recovery mode

---

## 3. Firmware Development Setup

### Prerequisites
- **Development Environment**: ESP-IDF v4.4 or later
- **Programming Languages**: C++ for firmware, Python for tooling
- **Build System**: CMake-based build configuration
- **IDE Support**: VSCode with ESP-IDF extension or standalone toolchain

### Required Tools
- **M5Burner**: Official M5Stack firmware flashing tool
- **ESP-IDF**: Espressif IoT Development Framework
- **ESP Flash Tool**: For direct firmware updates
- **Serial Terminal**: COM3 communication (115200 baud default)

### Project Structure
```
StackChan/firmware/
├── main/                    # Main application code
│   ├── CMakeLists.txt       # Build configuration
│   ├── main.cpp             # Application entry point
│   ├── hal/                 # Hardware abstraction layer
│   │   ├── hal.cpp          # HAL implementation
│   │   ├── board/           # Board-specific configuration
│   │   └── drivers/         # Hardware drivers
│   └── assets/              # Static assets
└── ...                      # Additional modules
```

### Build Configuration
- **Compiler**: esp32ulpgcc with C++17 support
- **Linker**: Custom linker script for memory allocation
- **Debug Symbols**: Enabled by default for development builds
- **Release Builds**: Optimized, minimal debug information

### Hardware Abstraction Layer (HAL)
The HAL provides unified interfaces for:
- **Display**: M5Display() API compatibility
- **Servo Control**: Dual-axis servo management
- **Sensor Interface**: Unified sensor access
- **Communication**: Serial, I2C, SPI abstraction

---

## 4. Safe Deployment Procedures

### Firmware Preparation
1. **Build Requirements**:
   - ESP-IDF toolchain installed
   - M5Stack BSP (Board Support Package)
   - Proper environment variables set

2. **Build Commands**:
   ```bash
   # Configure for StackChan
   idf.py menuconfig
   
   # Build the firmware
   idf.py build
   
   # Generate flash partition
   idf.py partition-table
   ```

### Flashing Procedure
1. **Prerequisites**:
   - StackChan connected via USB-C
   - COM3 port recognized in system
   - Proper drivers installed

2. **Safe Flashing Steps**:
   ```bash
   # Clean build before flashing
   idf.py clean
   
   # Flash with verify
   idf.py flash -p COM3 --verify
   
   # Monitor output
   idf.py monitor -p COM3
   ```

### Update Protocols
- **OTA Updates**: Over-the-air firmware updates
- **USB Updates**: Direct flashing via USB-C
- **M5Burner Updates**: GUI-based flashing tool

### Baud Rate Configuration
- **Default**: 115200 baud
- **Bootloader**: 921600 baud for fast flashing
- **Debug Mode**: 115200 baud with verbose output

---

## 5. Bootloader Recovery Procedures

### Common Boot Modes
1. **Normal Boot**: Application firmware execution
2. **Download Boot**: Firmware flashing mode
3. **Safe Boot**: Minimal firmware execution
4. **Factory Reset**: Configuration reset

### Recovery Triggers
- **GPIO Boot Pins**: GPIO0/GPIO12 combinations
- **UART Commands**: Special serial sequences
- **Button Combinations**: Physical button combinations

### Safe Recovery Process
1. **Download Mode Recovery**:
   ```bash
   # Hold GPIO0 while powering on
   # ESP32-S3 enters bootloader mode
   # Use ESP Flash Tool to re-flash
   ```

2. **UART Recovery**:
   ```bash
   # Send special boot sequence via COM3
   # 0xAA 0x55 0x53 0x45 0x42
   # Triggers bootloader reset
   ```

3. **Factory Recovery**:
   - Triple press power button
   - Hold reset button for 10 seconds
   - Automatic firmware restoration

### Bootloader Configuration
- **Boot Partitions**: Dual boot partition support
- **OTA Support**: Update mechanism preservation
- **Fallback**: Automatic rollback on failure

---

## 6. Communication Protocols

### Serial Communication
- **UART Interface**: COM3 port at 115200 baud
- **Data Format**: 8N1 (8 data bits, no parity, 1 stop bit)
- **Flow Control**: Hardware RTS/CTS supported
- **Timeout**: 100ms default read timeout

### Communication Commands
| Command | Format | Description |
|---------|--------|-------------|
| System Status | `STATUS\r\n` | Get system information |
| Sensor Data | `SENSOR:ALL\r\n` | Request all sensor data |
| Servo Control | `SERVO:H:90\r\n` | Set horizontal servo |
| Firmware Update | `UPDATE:START\r\n` | Begin firmware update |

### Wireless Communication
- **ESP-NOW**: Direct device-to-device communication
- **Wi-Fi**: TCP/UDP networking
- **Bluetooth**: BLE for mobile connectivity

### Protocol Implementation
- **Message Framing**: Start/stop byte encapsulation
- **Error Detection**: CRC-16 checksums
- **Data Compression**: LZ77 compression for large transfers

---

## 7. Memory Management

### Memory Layout
- **IRAM**: 320KB instruction RAM
- **DRAM**: 512KB data RAM
- **PSRAM**: 8MB external PSRAM
- **Flash**: 16MB external flash

### Allocation Strategies
1. **Static Allocation**: Critical system components
2. **Dynamic Allocation**: Runtime variable data
3. **Memory Pools**: Pre-allocated buffers for performance

### Memory Optimization
- **Stack Sizes**: Configurable stack sizes for tasks
- **Heap Management**: FreeRTOS heap management
- **Fragmentation Prevention**: Coalescing free blocks

### Memory Debugging
```cpp
// Memory debugging helpers
esp_err_t memory_status_check();
void heap_dump(Print* output);
void memory_leak_detection();
```

### Performance Monitoring
- **Memory Usage Tracking**: Real-time monitoring
- **Allocation Histogram**: Pattern analysis
- **Leak Detection**: Automated memory leak detection

---

## 8. Troubleshooting Guide

### Common Boot Issues

#### Issue: Boot Loop
**Symptoms**: Device continuously reboots without reaching application
**Causes**: Firmware corruption, memory issues, hardware failure
**Solutions**:
```bash
# Check for boot messages
idf.py monitor -p COM3

# Enter bootloader mode
# Hold GPIO0 while pressing reset

# Use ESP Flash Tool to re-flash
```

#### Issue: COM3 Communication Failure
**Symptoms**: Serial port not detected or not responding
**Causes**: Driver issues, cable problems, port configuration
**Solutions**:
```bash
# Check device manager for COM3 port
# Update USB drivers
# Try different cable/port
```

#### Issue: RAM Allocation Conflicts
**Symptoms**: Memory allocation failures, crashes
**Causes**: Insufficient memory, fragmentation, leaks
**Solutions**:
```cpp
// Increase heap size in sdkconfig
CONFIG_ESP_MAIN_TASK_STACK_SIZE=8192
CONFIG_FREERTOS_UNICORE=y

// Check memory usage
heap_caps_get_free_size(MALLOC_CAP_DEFAULT);
```

### Hardware Issues

#### Issue: Servo Malfunction
**Symptoms**: Servos not responding, erratic movement
**Causes**: Power issues, configuration problems, hardware failure
**Solutions**:
```cpp
// Check servo power supply
// Verify PWM frequency
// Test with minimal movement
```

#### Issue: Display Problems
**Symptoms**: Blank screen, flickering, incorrect output
**Causes**: Display driver issues, configuration problems
**Solutions**:
```cpp
// Check display initialization
// Verify pin configuration
// Test with basic display functions
```

### Firmware Issues

#### Issue: M5Display() API Compatibility
**Symptoms**: Display rendering errors, crashes
**Causes**: API version conflicts, initialization issues
**Solutions**:
```cpp
// Include proper headers
#include <M5Display.h>

// Check display initialization
if (!M5.Lcd.begin()) {
    // Handle error
}
```

### Error Recovery Procedures
1. **Automatic Recovery**: System attempts self-recovery
2. **Manual Recovery**: User intervention required
3. **Factory Reset**: Complete system restoration

---

## 9. Maintenance Procedures

### Regular Maintenance Tasks
1. **Firmware Updates**: Quarterly security patches
2. **Driver Updates**: Hardware driver improvements
3. **Performance Tuning**: Memory and CPU optimization
4. **Configuration Backup**: Regular configuration backups

### System Health Checks
```cpp
// System diagnostic functions
void system_health_check();
void hardware_self_test();
void firmware_integrity_check();
```

### Calibration Procedures
1. **Servo Calibration**: Neutral position calibration
2. **Sensor Calibration**: IMU and sensor baseline setup
3. **Display Calibration**: Touch screen calibration

### Backup and Restore
1. **Configuration Backup**: Save current settings
2. **Firmware Backup**: Store current firmware
3. **System Restore**: Restore from backup

### Maintenance Schedule
| Task | Frequency | Description |
|------|-----------|-------------|
| Firmware Update | Quarterly | Security patches and bug fixes |
| Hardware Check | Monthly | Physical inspection and testing |
| System Diagnostics | Weekly | Automated health checks |
| Configuration Backup | Daily | Settings preservation |

---

## 10. Emergency Procedures

### System Crash Recovery
1. **Power Cycle**: Complete shutdown and restart
2. **Bootloader Entry**: Force bootloader mode
3. **Firmware Restoration**: Re-install firmware

### Hardware Failure Handling
1. **Component Isolation**: Identify failed component
2. **Safe Mode**: Disable failed functionality
3. **Replacement**: Component replacement procedure

### Communication Loss Recovery
1. **Network Restart**: Reboot wireless interfaces
2. **Serial Reset**: Reset communication interfaces
3. **Factory Reset**: Complete system restoration

---

## Appendix

### Technical References
- [ESP32-S3 Datasheet](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/get-started.html)
- [M5Stack Documentation](https://docs.m5stack.com/)
- [ESP-IDF Programming Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/api-reference/index.html)

### Support Resources
- **Official Support**: M5Stack forums and GitHub issues
- **Community**: StackChan community resources
- **Documentation**: Online documentation and tutorials

### Version History
- **v1.0.0**: Initial firmware release
- **v1.1.0**: Bug fixes and improvements
- **v1.2.0**: Feature enhancements and stability improvements

### License
This documentation is part of the StackChan project and is subject to the project's license terms.

---

*This manual will be updated regularly with new firmware versions and system improvements. Always refer to the latest version for accurate information.*