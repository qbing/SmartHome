   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Parser V4.10.2 - 02 Nov 2011
   3                     ; Generator (Limited) V4.3.7 - 29 Nov 2011
2608                     ; 23 main()
2608                     ; 24 {
2610                     	switch	.text
2611  0000               _main:
2615                     ; 26   init();
2617  0000 cd0000        	call	_init
2619                     ; 27   INTERUPT_ENABLE();
2622  0003 9a            rim
2624  0004               L5661:
2625                     ; 30     ;
2627  0004 20fe          	jra	L5661
2640                     	xdef	_main
2641                     	xref	_init
2660                     	end
