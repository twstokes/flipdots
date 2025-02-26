#!/bin/sh

pio ci ../examples/Demo -b nodemcuv2 -l ../ && pio ci ../examples/ESP8266_Clock -b nodemcuv2 -l ../
