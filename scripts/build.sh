cmake -S . -B build -G "Ninja"
cmake --build build --verbose
ls -lh build/esp/kernel.elf
ls -lh build/esp/EFI/BOOT/BOOTX64.EFI