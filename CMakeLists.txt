cmake_minimum_required(VERSION 3.22)
set(BOARD xiao_esp32c3)

find_package(Zephyr)
project(alarmclock)

target_sources(app PRIVATE src/main.c src/oled.c src/time.c src/state_machine.c)
