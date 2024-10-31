add_executable(pwmfan
        pwmfan.c
        )

pico_enable_stdio_usb(pwmfan 1)
pico_enable_stdio_uart(pwmfan 1)

# pull in common dependencies and additional pwm and adc hardware support
target_link_libraries(pwmfan pico_stdlib hardware_adc hardware_pwm)

# create map/bin/hex file etc.
pico_add_extra_outputs(pwmfan)

# add url via pico_set_program_url
example_auto_set_url(pwmfan)
