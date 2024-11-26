#!/usr/bin/bash

UEFI_PAINT_ROOT_DIR=$(dirname "$(readlink -f "$0")")

export TOOLS_DIR="$UEFI_PAINT_ROOT_DIR/tools"
export MOUNTPOINT_DIR="$TOOLS_DIR/mountpoint"

export EFI_IMAGE_PATH="$UEFI_PAINT_ROOT_DIR/paint.efi"

# Need muuda endal ara
export UEFI_IMAGE_CREATOR_PATH="$HOME/Development/Tools/UEFI-GPT-image-creator"
export USB_EFI_SYSTEM_PARTITION="/dev/sdd1"
