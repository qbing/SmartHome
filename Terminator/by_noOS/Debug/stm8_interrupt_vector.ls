   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Parser V4.10.2 - 02 Nov 2011
   3                     ; Generator (Limited) V4.3.7 - 29 Nov 2011
2578                     	bsct
2579  0000               L7461_i:
2580  0000 0000          	dc.w	0
2623                     ; 11 @far @interrupt void EnterPer1ms(void)
2623                     ; 12 {
2624                     	switch	.text
2625  0000               f_EnterPer1ms:
2627  0000 8a            	push	cc
2628  0001 84            	pop	a
2629  0002 a4bf          	and	a,#191
2630  0004 88            	push	a
2631  0005 86            	pop	cc
2632  0006 3b0002        	push	c_x+2
2633  0009 be00          	ldw	x,c_x
2634  000b 89            	pushw	x
2635  000c 3b0002        	push	c_y+2
2636  000f be00          	ldw	x,c_y
2637  0011 89            	pushw	x
2640                     ; 21     if(i++==500){SystemEnterPer1ms();i=0;}
2642  0012 be00          	ldw	x,L7461_i
2643  0014 1c0001        	addw	x,#1
2644  0017 bf00          	ldw	L7461_i,x
2645  0019 1d0001        	subw	x,#1
2646  001c a301f4        	cpw	x,#500
2647  001f 2606          	jrne	L5761
2650  0021 cd0000        	call	_SystemEnterPer1ms
2654  0024 5f            	clrw	x
2655  0025 bf00          	ldw	L7461_i,x
2656  0027               L5761:
2657                     ; 25 }
2660  0027 85            	popw	x
2661  0028 bf00          	ldw	c_y,x
2662  002a 320002        	pop	c_y+2
2663  002d 85            	popw	x
2664  002e bf00          	ldw	c_x,x
2665  0030 320002        	pop	c_x+2
2666  0033 80            	iret
2689                     ; 32 @far @interrupt void NonHandledInterrupt (void)
2689                     ; 33 {
2690                     	switch	.text
2691  0034               f_NonHandledInterrupt:
2695                     ; 37 	return;
2698  0034 80            	iret
2700                     .const:	section	.text
2701  0000               __vectab:
2702  0000 82            	dc.b	130
2704  0001 00            	dc.b	page(__stext)
2705  0002 0000          	dc.w	__stext
2706  0004 82            	dc.b	130
2708  0005 34            	dc.b	page(f_NonHandledInterrupt)
2709  0006 0034          	dc.w	f_NonHandledInterrupt
2710  0008 82            	dc.b	130
2712  0009 34            	dc.b	page(f_NonHandledInterrupt)
2713  000a 0034          	dc.w	f_NonHandledInterrupt
2714  000c 82            	dc.b	130
2716  000d 34            	dc.b	page(f_NonHandledInterrupt)
2717  000e 0034          	dc.w	f_NonHandledInterrupt
2718  0010 82            	dc.b	130
2720  0011 34            	dc.b	page(f_NonHandledInterrupt)
2721  0012 0034          	dc.w	f_NonHandledInterrupt
2722  0014 82            	dc.b	130
2724  0015 34            	dc.b	page(f_NonHandledInterrupt)
2725  0016 0034          	dc.w	f_NonHandledInterrupt
2726  0018 82            	dc.b	130
2728  0019 34            	dc.b	page(f_NonHandledInterrupt)
2729  001a 0034          	dc.w	f_NonHandledInterrupt
2730  001c 82            	dc.b	130
2732  001d 34            	dc.b	page(f_NonHandledInterrupt)
2733  001e 0034          	dc.w	f_NonHandledInterrupt
2734  0020 82            	dc.b	130
2736  0021 34            	dc.b	page(f_NonHandledInterrupt)
2737  0022 0034          	dc.w	f_NonHandledInterrupt
2738  0024 82            	dc.b	130
2740  0025 34            	dc.b	page(f_NonHandledInterrupt)
2741  0026 0034          	dc.w	f_NonHandledInterrupt
2742  0028 82            	dc.b	130
2744  0029 34            	dc.b	page(f_NonHandledInterrupt)
2745  002a 0034          	dc.w	f_NonHandledInterrupt
2746  002c 82            	dc.b	130
2748  002d 34            	dc.b	page(f_NonHandledInterrupt)
2749  002e 0034          	dc.w	f_NonHandledInterrupt
2750  0030 82            	dc.b	130
2752  0031 34            	dc.b	page(f_NonHandledInterrupt)
2753  0032 0034          	dc.w	f_NonHandledInterrupt
2754  0034 82            	dc.b	130
2756  0035 34            	dc.b	page(f_NonHandledInterrupt)
2757  0036 0034          	dc.w	f_NonHandledInterrupt
2758  0038 82            	dc.b	130
2760  0039 34            	dc.b	page(f_NonHandledInterrupt)
2761  003a 0034          	dc.w	f_NonHandledInterrupt
2762  003c 82            	dc.b	130
2764  003d 34            	dc.b	page(f_NonHandledInterrupt)
2765  003e 0034          	dc.w	f_NonHandledInterrupt
2766  0040 82            	dc.b	130
2768  0041 34            	dc.b	page(f_NonHandledInterrupt)
2769  0042 0034          	dc.w	f_NonHandledInterrupt
2770  0044 82            	dc.b	130
2772  0045 00            	dc.b	page(f_EnterPer1ms)
2773  0046 0000          	dc.w	f_EnterPer1ms
2774  0048 82            	dc.b	130
2776  0049 34            	dc.b	page(f_NonHandledInterrupt)
2777  004a 0034          	dc.w	f_NonHandledInterrupt
2778  004c 82            	dc.b	130
2780  004d 34            	dc.b	page(f_NonHandledInterrupt)
2781  004e 0034          	dc.w	f_NonHandledInterrupt
2782  0050 82            	dc.b	130
2784  0051 00            	dc.b	page(f_UART1_RX_DATA_FULL)
2785  0052 0000          	dc.w	f_UART1_RX_DATA_FULL
2786  0054 82            	dc.b	130
2788  0055 34            	dc.b	page(f_NonHandledInterrupt)
2789  0056 0034          	dc.w	f_NonHandledInterrupt
2790  0058 82            	dc.b	130
2792  0059 34            	dc.b	page(f_NonHandledInterrupt)
2793  005a 0034          	dc.w	f_NonHandledInterrupt
2794  005c 82            	dc.b	130
2796  005d 00            	dc.b	page(f_UART1_RX_DATA_FULL)
2797  005e 0000          	dc.w	f_UART1_RX_DATA_FULL
2798  0060 82            	dc.b	130
2800  0061 34            	dc.b	page(f_NonHandledInterrupt)
2801  0062 0034          	dc.w	f_NonHandledInterrupt
2802  0064 82            	dc.b	130
2804  0065 00            	dc.b	page(f_EnterPer1ms)
2805  0066 0000          	dc.w	f_EnterPer1ms
2806  0068 82            	dc.b	130
2808  0069 34            	dc.b	page(f_NonHandledInterrupt)
2809  006a 0034          	dc.w	f_NonHandledInterrupt
2810  006c 82            	dc.b	130
2812  006d 34            	dc.b	page(f_NonHandledInterrupt)
2813  006e 0034          	dc.w	f_NonHandledInterrupt
2814  0070 82            	dc.b	130
2816  0071 34            	dc.b	page(f_NonHandledInterrupt)
2817  0072 0034          	dc.w	f_NonHandledInterrupt
2818  0074 82            	dc.b	130
2820  0075 34            	dc.b	page(f_NonHandledInterrupt)
2821  0076 0034          	dc.w	f_NonHandledInterrupt
2822  0078 82            	dc.b	130
2824  0079 34            	dc.b	page(f_NonHandledInterrupt)
2825  007a 0034          	dc.w	f_NonHandledInterrupt
2826  007c 82            	dc.b	130
2828  007d 34            	dc.b	page(f_NonHandledInterrupt)
2829  007e 0034          	dc.w	f_NonHandledInterrupt
2880                     	xdef	__vectab
2881                     	xref	__stext
2882                     	xdef	f_NonHandledInterrupt
2883                     	xdef	f_EnterPer1ms
2884                     	xref	_SystemEnterPer1ms
2885                     	xref	f_UART1_RX_DATA_FULL
2886                     	xref.b	c_x
2887                     	xref.b	c_y
2906                     	end
