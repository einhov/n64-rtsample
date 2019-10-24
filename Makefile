LD=mips-elf-ld
CC=mips-elf-gcc
CXX=mips-elf-g++
OBJCOPY=mips-elf-objcopy
CHECKSUM=/home/grachev/src/n64chain/tools/bin/checksum
header=header.bin

warnflags=-Wall -Wextra -Wno-main
optflags=-O2 -ffunction-sections -fdata-sections -flto
CFLAGS=-g ${optflags} ${warnflags} -march=vr4300 -mtune=vr4300
CXXFLAGS=${CFLAGS} -std=c++17 -fno-exceptions -fno-rtti -Ifmt/include -Iglm
ASFLAGS=-march=vr4300

LDLIBS=-lc -lfmt
LDFLAGS=-Trom.ld -Tmemmap.ld -Wl,--gc-sections -Lfmt/build

%.elf :
	${LINK.cc} $^ ${LOADLIBES} ${LDLIBS} -o $@

%.z64 :
	${OBJCOPY} -O binary $^ $@
	${CHECKSUM} ${header} $@

test.z64 : test.elf
test.elf : crt0.o crt1.o main.o

