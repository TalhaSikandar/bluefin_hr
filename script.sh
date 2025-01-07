#!/bin/bash

set -xe  # Enable debugging and exit on error

# Define directories and files
OUTPUT_DIR="output"
MAIN_PROGRAM_SRC="main.c"
HR_SRC="hr.c"
MAIN_PROGRAM_EXEC="$OUTPUT_DIR/main"

# Library source files and their corresponding shared objects
FLASHES=("library1.c" "library2.c")
LIBRARIES=("library1.so" "library2.so")

# Create the output directory if it doesn't exist
mkdir -p "$OUTPUT_DIR"

# Compile the main program (linking with hr.c)
gcc -o "$MAIN_PROGRAM_EXEC" "$MAIN_PROGRAM_SRC" "$HR_SRC" -ldl -pthread

# Compile each library
for i in "${!FLASHES[@]}"; do
    gcc -fPIC -shared -o "$OUTPUT_DIR/${LIBRARIES[$i]}" "${FLASHES[$i]}"
done

# Set LD_LIBRARY_PATH to include the shared libraries' directory
export LD_LIBRARY_PATH="$OUTPUT_DIR:$LD_LIBRARY_PATH"

# Run the main program
cd "$OUTPUT_DIR"
./main
