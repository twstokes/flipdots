#!/bin/sh

pio ci examples/Demo -b nodemcuv2 -l . --project-option="lib_deps=Adafruit GFX Library" && 
pio ci examples/ESP8266_Clock -b nodemcuv2 -l . --project-option="lib_deps=Adafruit GFX Library"
