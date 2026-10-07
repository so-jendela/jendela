cmake_minimum_required(VERSION 4.4)

project(
    Jendela
    VERSION 0.0.0
    LANGUAGES C ASM
)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Common
set(JENDELA_TARGET x86_64-unknown-elf)

# Kernel
set(JENDELA_KERNEL_SOURCES
    kernel/src/kernel.c
    arch/x86_64/boot.S
)

add_library(jendela_kernel_objects OBJECT
    ${JENDELA_KERNEL_SOURCES}
)

target_include_directories(jendela_kernel_objects PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_compile_options(jendela_kernel_objects PRIVATE
    --target=${JENDELA_TARGET}

    -ffreestanding
    -fno-stack-protector
    -fno-pic
    -fno-pie

    -mno-red-zone
    -m64

    -Wall
    -Wextra
)

add_custom_command(
    OUTPUT
        ${CMAKE_BINARY_DIR}/jendela_kernel.elf

    COMMAND
        ld.lld
        -m
        elf_x86_64
        -T
        ${CMAKE_SOURCE_DIR}/arch/x86_64/linker.ld
        -o
        ${CMAKE_BINARY_DIR}/jendela_kernel.elf

        $<TARGET_OBJECTS:jendela_kernel_objects>

    DEPENDS
        jendela_kernel_objects
        ${CMAKE_SOURCE_DIR}/arch/x86_64/linker.ld

    COMMAND_EXPAND_LISTS
    VERBATIM
)

add_custom_target(jendela_kernel ALL
    DEPENDS
        ${CMAKE_BINARY_DIR}/jendela_kernel.elf
)

# UEFI Bootloader
set(JENDELA_BOOTLOADER_SOURCE
    boot/main.c
)

add_library(jendela_bootloader_object OBJECT
    ${JENDELA_BOOTLOADER_SOURCE}
)

target_compile_options(jendela_bootloader_object PRIVATE
    --target=x86_64-pc-windows-coff

    -ffreestanding
    -fno-stack-protector
    -fno-stack-check
    -fno-builtin

    -mno-red-zone

    -Wall
    -Wextra
)

add_custom_command(
    OUTPUT
        ${CMAKE_BINARY_DIR}/BOOTX64.EFI

    COMMAND
        lld-link
        /subsystem:efi_application
        /entry:efi_main
        /nodefaultlib
        /fixed:no
        /out:${CMAKE_BINARY_DIR}/BOOTX64.EFI

        $<TARGET_OBJECTS:jendela_bootloader_object>

    DEPENDS
        jendela_bootloader_object

    COMMAND_EXPAND_LISTS
    VERBATIM
)

add_custom_target(jendela_bootloader ALL
    DEPENDS
        ${CMAKE_BINARY_DIR}/BOOTX64.EFI
)


# UEFI ESP directory
set(JENDELA_ESP_DIR
    ${CMAKE_BINARY_DIR}/esp
)

set(JENDELA_EFI_BOOT_DIR
    ${JENDELA_ESP_DIR}/EFI/BOOT
)

add_custom_command(
    OUTPUT
        ${JENDELA_EFI_BOOT_DIR}/BOOTX64.EFI

    COMMAND
        ${CMAKE_COMMAND} -E make_directory
        ${JENDELA_EFI_BOOT_DIR}

    COMMAND
        ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_BINARY_DIR}/BOOTX64.EFI
        ${JENDELA_EFI_BOOT_DIR}/BOOTX64.EFI

    DEPENDS
        jendela_bootloader
)

add_custom_target(jendela_esp ALL
    DEPENDS
        ${JENDELA_EFI_BOOT_DIR}/BOOTX64.EFI
)