#!/bin/bash

# Set the time interval for data collection (e.g., 10 minutes)
INTERVAL=600

while true
do
    # Compile and run the C program
    gcc main.c -o weather_program -lcurl -ljson-c
    if [[ "$OSTYPE" == "msys" || "$OSTYPE" == "win32" ]]; then
        ./weather_program.exe
    else
        ./weather_program
    fi

    # Wait for the next interval
    sleep $INTERVAL
done

