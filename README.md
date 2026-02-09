# SAUVC-STM32

## Install Micro-ROS

Follow: [Setting up micro-ROS on any STM32 microcontroller](https://github.com/lFatality/stm32_micro_ros_setup?tab=readme-ov-file#setting-up-micro-ros-on-any-stm32-microcontroller)

## Generate Code

Use STM32CubeMX 6.12.0 to generate code. Note that STM32CubeMX 6.12.1 seems to have bugs that generates code causing an error when running `make`.

## Build

Execute the Micro-ROS static library generation tool. See [micro_ros_stm32cubemx_utils/README.md](micro_ros_stm32cubemx_utils/README.md).

```bash
docker pull microros/micro_ros_static_library_builder:humble
docker run -it --rm -v $(pwd):/project --env MICROROS_LIBRARY_FOLDER=micro_ros_stm32cubemx_utils/microros_static_library microros/micro_ros_static_library_builder:humble
```

Compile the program.

```bash
make -j$(nproc)
```

## Note

- The timeouts of HAL_I2C_Master_Receive and HAL_I2C_Master_Transmit in MS5837.c might jam the whole system.

Using the CMSIS_V2 ARM_I2C_MasterTransmit and ARM_I2C_MasterReceive functions could be a solution.
Reference: [https://arm-software.github.io/CMSIS_6/v6.0.0/Driver/group__i2c__interface__gr.html](https://arm-software.github.io/CMSIS_6/v6.0.0/Driver/group__i2c__interface__gr.html), [https://github.com/Open-CMSIS-Pack/CMSIS-Driver_STM32/blob/main/Drivers/I2C_STM32.c](https://github.com/Open-CMSIS-Pack/CMSIS-Driver_STM32/blob/main/Drivers/I2C_STM32.c)
