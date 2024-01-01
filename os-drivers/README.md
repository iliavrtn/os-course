# Kernel Module Readme

## Overview

This Linux kernel module, named "driver" acts as a character device driver, enabling 
user-space applications to interact with the kernel. It supports essential operations
such as reading, writing, and an IOCTL command for dynamic configuration.

## Usage

1. **Compilation:**
    ```bash
    make
    ```

2. **Installation:**
    ```bash
    sudo insmod driver.ko
    ```

3. **Device Interaction:**
    - Create a device file:
        ```bash
        sudo mknod /dev/driver c <major_device_number> 0
        ```
    - Write to the device:
        ```bash
        echo "Hello, Device!" > /dev/driver
        ```
    - Read from the device:
        ```bash
        cat /dev/driver
        ```

4. **IOCTL Command:**
    - Dynamically set buffer size (`N`):
        ```bash
        sudo ioctl /dev/driver <new_buffer_size>
        ```

5. **Uninstallation:**
    ```bash
    sudo rmmod driver
    ```

## Notes

- The initial buffer size (`N`) is set to 100 during device file creation.
- Ensure appropriate permissions for device interactions.
