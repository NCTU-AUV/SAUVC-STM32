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
