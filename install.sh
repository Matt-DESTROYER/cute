#!/bin/sh
set -eu

REPO_URL="https://github.com/Matt-DESTROYER/cute.git"

SOURCE_DIR=""
TEMP_SOURCE_DIR=""
BUILD_DIR=""

cleanup() {
    if [ -n "$BUILD_DIR" ] && [ -d "$BUILD_DIR" ]; then
        rm -rf "$BUILD_DIR" 2>/dev/null || true
    fi

    if [ -n "$TEMP_SOURCE_DIR" ] && [ -d "$TEMP_SOURCE_DIR" ]; then
        rm -rf "$TEMP_SOURCE_DIR" 2>/dev/null || true
    fi
}

trap cleanup EXIT INT TERM

error() {
    echo "Error: $*" >&2
    exit 1
}

# ---------------------------------------------------------------------------
# Check prerequisites
# ---------------------------------------------------------------------------

if ! command -v git >/dev/null 2>&1; then
    error "git is required but was not found in PATH."
fi

if ! command -v cmake >/dev/null 2>&1; then
    error "cmake is required but was not found in PATH."
fi

# ---------------------------------------------------------------------------
# Find source
# ---------------------------------------------------------------------------

if [ -f "CMakeLists.txt" ]; then
    SOURCE_DIR="$(pwd)"

    echo "Using cute repository in the current directory."

elif [ -f "cute/CMakeLists.txt" ]; then
    SOURCE_DIR="$(cd cute && pwd)"

    echo "Using cute repository in ./cute."

else
    TEMP_SOURCE_DIR="$(mktemp -d "${TMPDIR:-/tmp}/cute-source.XXXXXX")"

    echo "Downloading cute..."

    git clone \
        --depth 1 \
        "$REPO_URL" \
        "$TEMP_SOURCE_DIR/cute"

    SOURCE_DIR="$TEMP_SOURCE_DIR/cute"
fi

# ---------------------------------------------------------------------------
# Build
# ---------------------------------------------------------------------------

# Keep the build directory separate from the source tree.
#
# This means an existing checkout's build directory is left untouched and
# temporary clones do not contain their build directory.
BUILD_DIR="$(mktemp -d "${TMPDIR:-/tmp}/cute-build.XXXXXX")"

echo "Building cute..."

cmake \
    -S "$SOURCE_DIR" \
    -B "$BUILD_DIR" \
    -DCMAKE_BUILD_TYPE=Release

cmake \
    --build "$BUILD_DIR" \
    --config Release

# ---------------------------------------------------------------------------
# Install
# ---------------------------------------------------------------------------

INSTALL_PREFIX="$HOME/.local"
INSTALL_BIN_DIR="$INSTALL_PREFIX/bin"

echo "Installing cute..."

mkdir -p "$INSTALL_BIN_DIR"

cmake \
    --install "$BUILD_DIR" \
    --prefix "$INSTALL_PREFIX"

# ---------------------------------------------------------------------------
# Verify installation
# ---------------------------------------------------------------------------

CUTE_PATH="$INSTALL_BIN_DIR/cute"

if [ ! -f "$CUTE_PATH" ]; then
    error "cute was not found after installation at: $CUTE_PATH"
fi

# ---------------------------------------------------------------------------
# Configure PATH
# ---------------------------------------------------------------------------

PATH_LINE='export PATH="$HOME/.local/bin:$PATH"'

add_path_to_file() {
    file="$1"

    if [ ! -f "$file" ]; then
        touch "$file"
    fi

    if ! grep -Fqx "$PATH_LINE" "$file" 2>/dev/null; then
        echo "" >> "$file"
        echo "# cute" >> "$file"
        echo "$PATH_LINE" >> "$file"
    fi
}

# ~/.profile is the portable place to configure PATH for login shells.
add_path_to_file "$HOME/.profile"

# Also configure zsh explicitly if the user already has a .zshrc.
if [ -f "$HOME/.zshrc" ]; then
    add_path_to_file "$HOME/.zshrc"
fi

# Update PATH for this installer process.
#
# This cannot modify the environment of the parent shell that launched the
# script, so a new terminal is still required for the persistent change.
export PATH="$INSTALL_BIN_DIR:$PATH"

# ---------------------------------------------------------------------------
# Success
# ---------------------------------------------------------------------------

echo
echo "cute has been installed successfully."
echo "Location: $CUTE_PATH"
echo
echo "Open a new terminal for the PATH change to take effect."
echo
echo "To use cute immediately in this shell, run:"
echo '  export PATH="$HOME/.local/bin:$PATH"'

