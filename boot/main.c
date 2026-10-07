#include <stdint.h>

typedef uint64_t EFI_STATUS;
typedef void *EFI_HANDLE;
typedef uint16_t CHAR16;

typedef struct {
    uint64_t signature;
    uint32_t revision;
    uint32_t header_size;
    uint32_t crc32;
    uint32_t reserved;
} EFI_TABLE_HEADER;

struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

typedef EFI_STATUS (*EFI_TEXT_RESET)(
    struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *this,
    uint8_t extended_verification
);

typedef EFI_STATUS (*EFI_TEXT_OUTPUT_STRING)(
    struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *this,
    CHAR16 *string
);

typedef struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
    EFI_TEXT_RESET Reset;
    EFI_TEXT_OUTPUT_STRING OutputString;
} EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

typedef struct {
    EFI_TABLE_HEADER Hdr;

    CHAR16 *FirmwareVendor;
    uint32_t FirmwareRevision;

    EFI_HANDLE ConsoleInHandle;
    void *ConIn;

    EFI_HANDLE ConsoleOutHandle;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut;

    EFI_HANDLE StandardErrorHandle;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *StdErr;

    void *RuntimeServices;
    void *BootServices;

    uint64_t NumberOfTableEntries;
    void *ConfigurationTable;
} EFI_SYSTEM_TABLE;

EFI_STATUS efi_main(
    EFI_HANDLE image_handle,
    EFI_SYSTEM_TABLE *system_table
)
{
    (void)image_handle;

    static CHAR16 message[] = {
        'A', 'w', 'a', 'l', ' ',
        'D', 'a', 'r', 'i', ' ',
        'B', 'o', 'o', 't', 'l', 'o', 'a', 'd', 'e', 'r',
        ' ', 'S', 'i', 's', 't', 'e', 'm', ' ',
        'O', 'p', 'e', 'r', 'a', 's', 'i', ' ',
        'J', 'e', 'n', 'd', 'e', 'l', 'a',
        '\r', '\n',
        0
    };

    system_table->ConOut->OutputString(
        system_table->ConOut,
        message
    );

    for (;;) {
        __asm__ volatile ("hlt");
    }

    return 0;
}