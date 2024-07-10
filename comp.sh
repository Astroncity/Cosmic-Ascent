# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[0;33m'
RESET='\033[0m'
ACTION="$YELLOW [ACTION] $RESET"
INFO="$BLUE [INFO] $RESET"

# Directory containing your source code (.cpp files)
SOURCE_DIR="src/"

# Output directories
BUILD_DIR="build/"
BIN_DIR="bin/"
OUTPUT_NAME="main"

# Compiler flags
CXXFLAGS="-Wall -Wextra -Werror -Wno-unused-variable -std=c99 -O3 -L lib/ -I include/ -lraylib -lm"

# Create build and bin directories if they don't exist
mkdir -p "$BUILD_DIR"
mkdir -p "$BIN_DIR"

# Find all .cpp files in SOURCE_DIR
CPP_FILES=$(find "$SOURCE_DIR" -name "*.c")

# Compile each .cpp file individually
for file in $CPP_FILES; do
    OBJ_FILE="$BUILD_DIR$(basename ${file%.c}.o)"
    printf "$ACTION Compiling $file to $OBJ_FILE...\n"
    gcc "$file" -o "$OBJ_FILE" -c $CXXFLAGS
done

# Combine all object files into one executable
printf "$ACTION Linking object files...\n"
gcc $BUILD_DIR*.o -o "$BIN_DIR$OUTPUT_NAME" $CXXFLAGS

printf "$INFO Compilation complete. Executable '$OUTPUT_NAME' created in $BIN_DIR."
