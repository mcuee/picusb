lab2.hex: lab2.asm 
	gpasm lab2.asm 2>&1 | tee lab2.err
#	gplink -s bootload.lkr -m -o lab2.hex lab2.o
clean:
	rm *.o *.hex
