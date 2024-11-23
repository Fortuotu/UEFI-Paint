#!/usr/bin/bash

# Muuda see endal ara.
IMAGE_CREATOR_PATH="$HOME/Development/Tools/UEFI-GPT-image-creator"

THIS_PATH="$(dirname "$0")"

EFI_IMAGE_NAME="./paint.efi"

cp "$EFI_IMAGE_NAME" "$IMAGE_CREATOR_PATH/BOOTX64.EFI"

cd "$IMAGE_CREATOR_PATH"
./write_gpt
./qemu.sh

cd "$THIS_PATH"
