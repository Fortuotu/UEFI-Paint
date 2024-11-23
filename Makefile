CC = gcc
LD = ld
OBJCOPY = objcopy

# See muuda endal ara.
GNU_EFI_DIR = /home/ott/Development/Tools/gnu-efi

CFLAGS = -I$(GNU_EFI_DIR)/inc -fpic -ffreestanding -fno-stack-protector -fno-stack-check -fshort-wchar \
         -mno-red-zone -maccumulate-outgoing-args

LDFLAGS = -shared -Bsymbolic -L$(GNU_EFI_DIR)/x86_64/lib -L$(GNU_EFI_DIR)/x86_64/gnuefi \
          -T$(GNU_EFI_DIR)/gnuefi/elf_x86_64_efi.lds

LIBS = -lgnuefi -lefi

# Pead uue lahtefaili loomisel selle siia ka panema.
SRC_FILES = main.c

OBJ_FILES = $(SRC_FILES:.c=.o)
SO_FILE = main.so
TARGET = main.efi
CRT0_FILE = $(GNU_EFI_DIR)/x86_64/gnuefi/crt0-efi-x86_64.o

all: $(TARGET)

$(OBJ_FILES): %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SO_FILE): $(OBJ_FILES)
	$(LD) $(LDFLAGS) $(CRT0_FILE) $(OBJ_FILES) -o $(SO_FILE) $(LIBS)

$(TARGET): $(SO_FILE)
	$(OBJCOPY) -j .text -j .sdata -j .data -j .rodata -j .dynamic -j .dynsym \
               -j .rel -j .rela -j .rel.* -j .rela.* -j .reloc --target efi-app-x86_64 \
               --subsystem=10 $(SO_FILE) $(TARGET)

clean:
	rm -f $(OBJ_FILES) $(SO_FILE) $(TARGET)

.PHONY: all clean
