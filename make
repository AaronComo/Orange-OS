make realclean
orb make everything CFLAGS="-I include/ -I include/sys/ -c -fno-builtin -Wall -m32 -fno-stack-protector -g"
x86_64-elf-objcopy --only-keep-debug kernel.bin kernel.debug
x86_64-elf-objcopy --strip-debug kernel.bin

orb make image CFLAGS="-I include/ -I include/sys/ -c -fno-builtin -Wall -m32 -fno-stack-protector"

# bochs
# qemu-system-i386 -drive format=raw,file=a.img,if=floppy -drive format=raw,file=80m.img,if=ide -boot a
