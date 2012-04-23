lab2.hex: lab2.asm 
	gpasm lab2.asm 

program: lab2.hex
	pyk --bootload lab2.hex

clean:
	rm -f *.o *.hex *.lst *.map *.cod *.err *~
