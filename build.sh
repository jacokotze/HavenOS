nasm -f elf32 loader.s

ld -T link.ld -melf_i386 loader.o -o kernel.elf

mkdir -p iso/boot/grub              # create the folder structure
cp stage2_eltorito iso/boot/grub/   # copy the bootloader
cp kernel.elf iso/boot/             # copy the kernel

cat > iso/boot/grub/menu.lst <<- EOM
default=0
timeout=0

title os
kernel /boot/kernel.elf
EOM


genisoimage -R                              \
            -b boot/grub/stage2_eltorito    \
            -no-emul-boot                   \
            -boot-load-size 4               \
            -A os                           \
            -input-charset utf8             \
            -quiet                          \
            -boot-info-table                \
            -o os.iso                       \
            iso

mkdir -p bochs/config
cat > bochs/config/bochsrc.txt <<- EOM
megs:            32
display_library: sdl2
romimage:        file=/usr/share/bochs/BIOS-bochs-legacy
vgaromimage:     file=/usr/share/bochs/VGABIOS-lgpl-latest
ata0-master:     type=cdrom, path=os.iso, status=inserted
boot:            cdrom
log:             bochslog.txt
clock:           sync=realtime, time0=local
cpu:             count=1, ips=1000000
EOM

bochs -qf bochs/config/bochsrc.txt