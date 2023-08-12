cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(DARM_Pico)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

add_executable(pico_device
    pico_device.cpp
    stepper.cpp
)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(pico_device pico_stdlib hardware_i2c)

# enable usb output, disable uart output
pico_enable_stdio_usb(pico_device 1)
pico_enable_stdio_uart(pico_device 0)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(pico_device)



add_executable(app
    app.cpp
    stepper.cpp
)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(app pico_stdlib)

# enable usb output, disable uart output
pico_enable_stdio_usb(app 1)
pico_enable_stdio_uart(app 0)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(app)