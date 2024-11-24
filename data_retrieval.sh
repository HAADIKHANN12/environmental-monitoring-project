#!/bin/bash

# Constants
PROGRAM_NAME="weather_program"
INTERVAL=600 # Interval in seconds (use only if running in a loop)

# Function to compile the program
compile_program() {
    if [[ ! -f /mnt/c/updated/main.c ]]; then
        echo "Error: main.c not found in /mnt/c/updated/. Exiting..."
        exit 1
    fi

    if [[ ! -f $PROGRAM_NAME ]]; then
        echo "Compiling the C program..."
        gcc /mnt/c/updated/main.c -o $PROGRAM_NAME -lcurl -ljson-c
        if [[ $? -ne 0 ]]; then
            echo "Compilation failed. Exiting..."
            exit 1
        fi
    else
        echo "Program already compiled."
    fi
}

# Function to run the program
run_program() {
    echo "Running the program..."
    if [[ "$OSTYPE" == "msys" || "$OSTYPE" == "win32" ]]; then
        ./$PROGRAM_NAME.exe
    else
        ./$PROGRAM_NAME
    fi

    if [[ $? -ne 0 ]]; then
        echo "Execution failed. Exiting..."
        exit 1
    fi
}

# Main function to execute tasks
main() {
    compile_program
    run_program
}

main
