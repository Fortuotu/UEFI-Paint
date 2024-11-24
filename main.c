#include <efi.h>
#include <efilib.h>

EFI_GUID gEfiSimplePointerProtocolGuid = EFI_SIMPLE_POINTER_PROTOCOL_GUID;

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    EFI_SIMPLE_POINTER_PROTOCOL *PointerProtocol;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsProtocol;
    EFI_SIMPLE_POINTER_STATE PointerState;
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *ModeInfo;
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL RedPixel = {0x00, 0xFF, 0xFF, 0x00};
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL BlackPixel = {0x00, 0x00, 0x00, 0x00};
    UINT32 ScreenWidth, ScreenHeight;
    UINTN RedSquareSize = 20;
    INT32 MouseX = 0, MouseY = 0;

    InitializeLib(ImageHandle, SystemTable);

    if (uefi_call_wrapper(BS->LocateProtocol, 3, &gEfiGraphicsOutputProtocolGuid, NULL, (void **)&GraphicsProtocol) != EFI_SUCCESS) {
        Print(L"Failed to locate Graphics Output Protocol.\n");
        return EFI_UNSUPPORTED;
    }

    ModeInfo = GraphicsProtocol->Mode->Info;
    ScreenWidth = ModeInfo->HorizontalResolution;
    ScreenHeight = ModeInfo->VerticalResolution;

    if (uefi_call_wrapper(BS->LocateProtocol, 3, &gEfiSimplePointerProtocolGuid, NULL, (void **)&PointerProtocol) != EFI_SUCCESS) {
        Print(L"Failed to locate Simple Pointer Protocol.\n");
        return EFI_UNSUPPORTED;
    }

    uefi_call_wrapper(PointerProtocol->Reset, 2, PointerProtocol, TRUE);
    
    while (1) {
        if (uefi_call_wrapper(PointerProtocol->GetState, 2, PointerProtocol, &PointerState) == EFI_SUCCESS) {
            MouseX += (INT32)(PointerState.RelativeMovementX);
            MouseY += (INT32)(PointerState.RelativeMovementY);

            if (MouseX < 0) MouseX = 0;
            if (MouseX >= (INT32)ScreenWidth) MouseX = ScreenWidth - 1;
            if (MouseY < 0) MouseY = 0;
            if (MouseY >= (INT32)ScreenHeight) MouseY = ScreenHeight - 1;

            if (PointerState.LeftButton) {
                uefi_call_wrapper(GraphicsProtocol->Blt, 10,
                                  GraphicsProtocol,
                                  &RedPixel,
                                  EfiBltVideoFill,
                                  0, 0,
                                  MouseX, MouseY,
                                  RedSquareSize, RedSquareSize,
                                  0
                );
            }
        }

        EFI_INPUT_KEY Key;

        if (uefi_call_wrapper(SystemTable->ConIn->ReadKeyStroke, 2, SystemTable->ConIn, &Key) == EFI_SUCCESS) {
            if (Key.UnicodeChar == L'c') {
                uefi_call_wrapper(GraphicsProtocol->Blt, 10,
                                  GraphicsProtocol,
                                  &BlackPixel,
                                  EfiBltVideoFill,
                                  0, 0,
                                  0, 0,
                                  GraphicsProtocol->Mode->Info->HorizontalResolution, GraphicsProtocol->Mode->Info->VerticalResolution,
                                  0
                );
            } else if (Key.UnicodeChar == L'q') {
                break;
            }
        }

        uefi_call_wrapper(BS->Stall, 1, 5000);
    }

    return EFI_SUCCESS;
}
