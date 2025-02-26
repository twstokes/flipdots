#!/bin/sh

pio ci examples/demo -b nodemcuv2 -l . && pio ci examples/clock -b nodemcuv2 -l .
