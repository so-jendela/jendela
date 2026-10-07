# Sistem Operasi Jendela

Spesifikasi dan konfigurasi dasar proyek Sistem Operasi Jendela.

## Identitas

-   Nama: Jendela
-   Nama lengkap: Sistem Operasi Jendela
-   Singkatan: SO Jendela
-   Status: Pengembangan awal

## Target Platform

-   Arsitektur CPU: x86_64
-   Mode boot: UEFI
-   Target awal: QEMU
-   Target jangka panjang: QEMU dan komputer fisik x86_64
-   Firmware virtualisasi: EDK2/TianoCore

## Bahasa Pemrograman

-   C
-   Assembly
-   Sintaks Assembly: AT&T

## Arsitektur Kernel

-   Jenis kernel: Monolithic
-   Kernel dan bootloader dipisahkan
-   Kernel menggunakan format executable ELF64
-   Bootloader menggunakan format PE32+ EFI Application
-   Kode spesifik x86_64 ditempatkan di `arch/x86_64/`
-   Kernel tidak bergantung langsung pada UEFI

## Toolchain

-   Host: Windows 64-bit
-   Environment: MSYS2 melalui Git Bash
-   Toolchain: MSYS2 CLANG64
-   Compiler: Clang
-   Linker: LLD
-   Assembler: LLVM Integrated Assembler
-   Build system: CMake
-   Build generator: Ninja
-   Emulator: QEMU
-   Firmware: EDK2/TianoCore

## Versi Toolchain Saat Ini

-   Clang: 22.1.8
-   LLD: 22.1.8
-   CMake: 4.4.4
-   QEMU: 11.1.2

## Target Compiler

### Kernel

``` text
x86_64-unknown-elf
```

Kernel dibangun sebagai freestanding ELF64.

### Bootloader

``` text
x86_64-pc-windows-coff
```

Bootloader dibangun sebagai COFF lalu dilink menjadi PE32+ EFI
Application.

## Boot Flow

``` text
UEFI
  ↓
Jendela Bootloader
BOOTX64.EFI
  ↓
Jendela Kernel
kernel.elf
  ↓
Awal Dari Sistem Operasi Jendela
```

## Struktur Proyek

``` text
jendela/
├── CMakeLists.txt
├── README.md
├── boot/
│   └── main.c
├── kernel/
│   └── src/
│       └── kernel.c
├── arch/
│   └── x86_64/
│       ├── boot.S
│       └── linker.ld
├── include/
│   └── bootinfo.h
├── scripts/
│   └── run-qemu.sh
└── build/
```

Direktori `build/` merupakan hasil build dan tidak termasuk source
project.

## Output Utama

Kernel:

``` text
build/jendela_kernel.elf
```

Bootloader:

``` text
build/BOOTX64.EFI
```

UEFI ESP:

``` text
build/esp/
└── EFI/
    └── BOOT/
        └── BOOTX64.EFI
```

## Milestone 1

Status: selesai.

Fungsi:

-   Build kernel ELF64 x86_64
-   Build bootloader PE32+ x86_64
-   Menjalankan bootloader melalui UEFI
-   Menjalankan bootloader pada QEMU
-   Menampilkan:

``` text
Awal Dari Bootloader Sistem Operasi Jendela
```

## Milestone Berikutnya

Urutan pengembangan:

1.  Bootloader membuka `kernel.elf`
2.  Bootloader melakukan parsing ELF64
3.  Bootloader memuat `PT_LOAD`
4.  Bootloader mendapatkan informasi framebuffer melalui GOP
5.  Bootloader membangun `BootInfo`
6.  Bootloader mendapatkan memory map
7.  Bootloader menjalankan `ExitBootServices()`
8.  Bootloader menyerahkan kontrol kepada kernel
9.  Kernel menggunakan `BootInfo`
10. Kernel menampilkan:

``` text
Awal Dari Sistem Operasi Jendela
```

## Prinsip Pengembangan

-   Kernel tidak bergantung langsung pada UEFI.
-   Bootloader bertanggung jawab terhadap layanan UEFI.
-   Detail arsitektur x86_64 dipisahkan dari kode kernel generik.
-   Setiap milestone harus dapat dibangun dan dijalankan ulang secara
    reproducible.
-   QEMU digunakan sebagai target pengujian utama sebelum pengujian pada
    perangkat fisik.
