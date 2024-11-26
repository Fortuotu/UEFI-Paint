#!/usr/bin/bash

cp "$EFI_IMAGE_PATH" "$UEFI_IMAGE_CREATOR_PATH/BOOTX64.EFI"

cd "$UEFI_IMAGE_CREATOR_PATH"
./write_gpt
./qemu.sh
