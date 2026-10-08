#include <stdint.h>

typedef uint64_t EFI_STATUS;
typedef void *EFI_HANDLE;
typedef uint16_t CHAR16;
typedef uint64_t EFI_TPL;
typedef uint64_t EFI_PHYSICAL_ADDRESS;
typedef uint64_t UINTN;

typedef struct {
    uint32_t Data1;
    uint16_t Data2;
    uint16_t Data3;
    uint8_t Data4[8];
} EFI_GUID;

typedef struct {
    uint64_t signature;
    uint32_t revision;
    uint32_t header_size;
    uint32_t crc32;
    uint32_t reserved;
} EFI_TABLE_HEADER;

struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;
struct EFI_FILE_PROTOCOL;

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

typedef EFI_STATUS (*EFI_HANDLE_PROTOCOL)(
    EFI_HANDLE handle,
    EFI_GUID *protocol,
    void **interface
);

typedef struct {
    EFI_TABLE_HEADER Hdr;

    void *RaiseTPL;
    void *RestoreTPL;
    void *AllocatePages;
    void *FreePages;
    void *GetMemoryMap;
    void *AllocatePool;
    void *FreePool;
    void *CreateEvent;
    void *SetTimer;
    void *WaitForEvent;
    void *SignalEvent;
    void *CloseEvent;
    void *CheckEvent;
    void *InstallProtocolInterface;
    void *ReinstallProtocolInterface;
    void *UninstallProtocolInterface;

    EFI_HANDLE_PROTOCOL HandleProtocol;
} EFI_BOOT_SERVICES;

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
    EFI_BOOT_SERVICES *BootServices;

    UINTN NumberOfTableEntries;
    void *ConfigurationTable;
} EFI_SYSTEM_TABLE;

typedef struct {
    EFI_GUID DevicePathProtocol;
    EFI_GUID FilePath;
} EFI_DEVICE_PATH;

typedef struct {
    uint32_t Revision;
    EFI_HANDLE ParentHandle;
    EFI_SYSTEM_TABLE *SystemTable;
    EFI_HANDLE DeviceHandle;
    EFI_DEVICE_PATH *FilePath;
    void *LoadOptions;
    uint32_t LoadOptionsSize;
} EFI_LOADED_IMAGE_PROTOCOL;

struct EFI_FILE_PROTOCOL;

typedef EFI_STATUS (*EFI_FILE_OPEN)(
    struct EFI_FILE_PROTOCOL *this,
    struct EFI_FILE_PROTOCOL **new_handle,
    CHAR16 *file_name,
    uint64_t open_mode,
    uint64_t attributes
);

typedef EFI_STATUS (*EFI_FILE_CLOSE)(
    struct EFI_FILE_PROTOCOL *this
);

typedef EFI_STATUS (*EFI_FILE_READ)(
    struct EFI_FILE_PROTOCOL *this,
    UINTN *buffer_size,
    void *buffer
);

typedef struct EFI_FILE_PROTOCOL {
    uint64_t Revision;

    EFI_FILE_OPEN Open;
    EFI_FILE_CLOSE Close;

    void *Delete;
    EFI_FILE_READ Read;

    void *Write;
    void *GetPosition;
    void *SetPosition;
    void *GetInfo;
    void *SetInfo;
    void *Flush;
    void *OpenEx;
    void *CloseEx;
    void *DeleteEx;
    void *ReadEx;
    void *WriteEx;
    void *FlushEx;
} EFI_FILE_PROTOCOL;

typedef struct EFI_SIMPLE_FILE_SYSTEM_PROTOCOL {
    uint64_t Revision;

    EFI_STATUS (*OpenVolume)(
        struct EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *this,
        EFI_FILE_PROTOCOL **root
    );
} EFI_SIMPLE_FILE_SYSTEM_PROTOCOL;


/*
 * EFI protocol GUIDs
 */

static EFI_GUID gEfiLoadedImageProtocolGuid = {
    0x5B1B31A1,
    0x9562,
    0x11D2,
    {0x8E, 0x3F, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B}
};

static EFI_GUID gEfiSimpleFileSystemProtocolGuid = {
    0x964E5B22,
    0x6459,
    0x11D2,
    {0x8E, 0x39, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B}
};


/*
 * EFI constants
 */

#define EFI_SUCCESS 0

#define EFI_FILE_MODE_READ 0x0000000000000001ULL


static void print(
    EFI_SYSTEM_TABLE *system_table,
    CHAR16 *message
)
{
    system_table->ConOut->OutputString(
        system_table->ConOut,
        message
    );
}


EFI_STATUS efi_main(
    EFI_HANDLE image_handle,
    EFI_SYSTEM_TABLE *system_table
)
{
    static CHAR16 first_message[] = {
        'A', 'w', 'a', 'l', ' ',
        'D', 'a', 'r', 'i', ' ',
        'B', 'o', 'o', 't', 'l', 'o', 'a', 'd', 'e', 'r',
        ' ', 'S', 'i', 's', 't', 'e', 'm', ' ',
        'O', 'p', 'e', 'r', 'a', 's', 'i', ' ',
        'J', 'e', 'n', 'd', 'e', 'l', 'a',
        '\r', '\n',
        0
    };

    print(system_table, first_message);
    
    EFI_BOOT_SERVICES *boot_services =
        system_table->BootServices;

    EFI_STATUS status;

    EFI_LOADED_IMAGE_PROTOCOL *loaded_image = 0;

    status = boot_services->HandleProtocol(
        image_handle,
        &gEfiLoadedImageProtocolGuid,
        (void **)&loaded_image
    );

    if (status != EFI_SUCCESS) {
        static CHAR16 message[] = {
            'E', 'r', 'r', 'o', 'r', ':', ' ',
            'g', 'a', 'g', 'a', 'l', ' ', 'l', 'o', 'a', 'd',
            'e', 'd', ' ', 'i', 'm', 'a', 'g', 'e',
            '\r', '\n',
            0
        };

        print(system_table, message);

        for (;;) {
            __asm__ volatile ("hlt");
        }
    }


    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *filesystem = 0;

    status = boot_services->HandleProtocol(
        loaded_image->DeviceHandle,
        &gEfiSimpleFileSystemProtocolGuid,
        (void **)&filesystem
    );

    if (status != EFI_SUCCESS) {
        static CHAR16 message[] = {
            'E', 'r', 'r', 'o', 'r', ':', ' ',
            'g', 'a', 'g', 'a', 'l', ' ', 'f', 'i', 'l', 'e',
            's', 'y', 's', 't', 'e', 'm',
            '\r', '\n',
            0
        };

        print(system_table, message);

        for (;;) {
            __asm__ volatile ("hlt");
        }
    }


    EFI_FILE_PROTOCOL *root = 0;

    status = filesystem->OpenVolume(
        filesystem,
        &root
    );

    if (status != EFI_SUCCESS) {
        static CHAR16 message[] = {
            'E', 'r', 'r', 'o', 'r', ':', ' ',
            'g', 'a', 'g', 'a', 'l', ' ', 'O', 'p', 'e', 'n',
            'V', 'o', 'l', 'u', 'm', 'e',
            '\r', '\n',
            0
        };

        print(system_table, message);

        for (;;) {
            __asm__ volatile ("hlt");
        }
    }


    static CHAR16 kernel_path[] = {
        '\\',
        'k', 'e', 'r', 'n', 'e', 'l', '.', 'e', 'l', 'f',
        0
    };

    EFI_FILE_PROTOCOL *kernel_file = 0;

    status = root->Open(
        root,
        &kernel_file,
        kernel_path,
        EFI_FILE_MODE_READ,
        0
    );

    if (status != EFI_SUCCESS) {
        static CHAR16 message[] = {
            'E', 'r', 'r', 'o', 'r', ':', ' ',
            'k', 'e', 'r', 'n', 'e', 'l', '.', 'e', 'l', 'f',
            ' ', 't', 'i', 'd', 'a', 'k', ' ', 'd', 'i', 't', 'e', 'm', 'u', 'k', 'a', 'n',
            '\r', '\n',
            0
        };

        print(system_table, message);

        for (;;) {
            __asm__ volatile ("hlt");
        }
    }


    static CHAR16 message[] = {
        'K', 'e', 'r', 'n', 'e', 'l', ' ',
        'J', 'e', 'n', 'd', 'e', 'l', 'a',
        ' ', 'd', 'i', 't', 'e', 'm', 'u', 'k', 'a', 'n',
        '\r', '\n',
        0
    };

    print(system_table, message);

    kernel_file->Close(kernel_file);

    for (;;) {
        __asm__ volatile ("hlt");
    }

    return 0;
}