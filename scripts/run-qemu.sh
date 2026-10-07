#!/usr/bin/env bash

set -e

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="$PROJECT_ROOT/build"
ESP_DIR="$BUILD_DIR/esp"

QEMU="$(command -v qemu-system-x86_64)"

if [[ -z "$QEMU" ]]; then
    echo "Error: qemu-system-x86_64 tidak ditemukan."
    exit 1
fi

QEMU_BIN_DIR="$(cd "$(dirname "$QEMU")" && pwd)"
QEMU_ROOT="$(cd "$QEMU_BIN_DIR/.." && pwd)"

FIRMWARE="$QEMU_ROOT/share/qemu/edk2-x86_64-code.fd"

if [[ ! -f "$FIRMWARE" ]]; then
    echo "Error: firmware UEFI tidak ditemukan:"
    echo "  $FIRMWARE"
    exit 1
fi

EFI_BOOT="$ESP_DIR/EFI/BOOT/BOOTX64.EFI"

if [[ ! -f "$EFI_BOOT" ]]; then
    echo "Error: BOOTX64.EFI belum dibuat:"
    echo "  $EFI_BOOT"
    echo "Jalankan:"
    echo "  cmake --build build"
    exit 1
fi

FIRMWARE_WIN="$(cygpath -w "$FIRMWARE")"
ESP_WIN="$(cygpath -w "$ESP_DIR")"

echo "QEMU:      $QEMU"
echo "Firmware:  $FIRMWARE_WIN"
echo "ESP:       $ESP_WIN"
echo

MSYS_NO_PATHCONV=1 exec qemu-system-x86_64 \
    -machine q35 \
    -drive "if=pflash,format=raw,readonly=on,file=$FIRMWARE_WIN" \
    -drive "format=raw,file=fat:rw:$ESP_WIN"