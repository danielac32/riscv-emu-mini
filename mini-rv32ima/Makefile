all : mini-rv32ima mini-rv32ima.flt



riscv:
	gcc mini-rv32ima.c  mem.c -o mini-rv32ima 


	
mini-rv32ima : mini-rv32ima.c mem.c  mini-rv32ima.h mem.h
	# for debug
	gcc -o $@ $< -g -O2 -Wall
	gcc -o $@.tiny $< -Os -ffunction-sections -fdata-sections -Wl,--gc-sections -s


clean :
	rm -rf mini-rv32ima mini-rv32ima.flt

