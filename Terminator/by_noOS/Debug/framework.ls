   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Parser V4.10.2 - 02 Nov 2011
   3                     ; Generator (Limited) V4.3.7 - 29 Nov 2011
2578                     .const:	section	.text
2579  0000               L7461_DEVICE_INFO:
2580  0000 7b202020204e  	dc.b	"{    NAME: ",34
2581  000c 536d61727420  	dc.b	"Smart Sauna",34
2582  0018 2c2020202054  	dc.b	",    TYPE:SET,",9
2583  0027 090920202020  	dc.b	9,9,32,32,32,32
2584  002d 4348494c443a  	dc.b	"CHILD:{[    ",9
2585  003a 7b2020202009  	dc.b	"{    ",9
2586  0040 094e414d453a  	dc.b	9,78,65,77,69,58
2587  0046 2022          	dc.b	" ",34
2588  0048 4c6967687422  	dc.b	"Light",34
2589  004e 2c2020202009  	dc.b	",    ",9
2590  0054 09545950453a  	dc.b	9,84,89,80,69,58
2591  005a 5345542c09    	dc.b	"SET,",9
2592  005f 090920202020  	dc.b	9,9,32,32,32,32
2593  0065 09094348494c  	dc.b	9,9,67,72,73,76
2594  006b 443a7b5b2020  	dc.b	"D:{[    ",9
2595  0074 09097b202020  	dc.b	9,9,123,32,32,32
2596  007a 2009          	dc.b	" ",9
2597  007c 0909094e414d  	dc.b	9,9,9,78,65,77
2598  0082 453a2022      	dc.b	"E: ",34
2599  0086 52656164204c  	dc.b	"Read Light",34
2600  0091 2c2020202009  	dc.b	",    ",9
2601  0097 090909545950  	dc.b	9,9,9,84,89,80
2602  009d 453a5445524d  	dc.b	"E:TERMINATOR,",9
2603  00ab 090920202020  	dc.b	9,9,32,32,32,32
2604  00b1 090909094445  	dc.b	9,9,9,9,68,69
2605  00b7 56545950453a  	dc.b	"VTYPE:SWITCH    ",9
2606  00c8 09097d2c2020  	dc.b	9,9,125,44,32,32
2607  00ce 202009        	dc.b	"  ",9
2608  00d1 09097b202020  	dc.b	9,9,123,32,32,32
2609  00d7 2009          	dc.b	" ",9
2610  00d9 0909094e414d  	dc.b	9,9,9,78,65,77
2611  00df 453a2022      	dc.b	"E: ",34
2612  00e3 4465636f7261  	dc.b	"Decorate Light",34
2613  00f2 2c2020202009  	dc.b	",    ",9
2614  00f8 090909545950  	dc.b	9,9,9,84,89,80
2615  00fe 45            	dc.b	"E"
2616  00ff 3a5445524d49  	dc.b	":TERMINATOR,",9
2617  010c 090920202020  	dc.b	9,9,32,32,32,32
2618  0112 090909094445  	dc.b	9,9,9,9,68,69
2619  0118 56545950453a  	dc.b	"VTYPE:SWITCH    ",9
2620  0129 09097d202020  	dc.b	9,9,125,32,32,32
2621  012f 2009          	dc.b	" ",9
2622  0131 095d7d202020  	dc.b	9,93,125,32,32,32
2623  0137 2009          	dc.b	" ",9
2624  0139 7d2c20202020  	dc.b	"},    ",9
2625  0140 7b2020202009  	dc.b	"{    ",9
2626  0146 094e414d453a  	dc.b	9,78,65,77,69,58
2627  014c 2022          	dc.b	" ",34
2628  014e 434420506c61  	dc.b	"CD Player",34
2629  0158 2c2020202009  	dc.b	",    ",9
2630  015e 09545950453a  	dc.b	9,84,89,80,69,58
2631  0164 205445524d49  	dc.b	" TERMINATOR,",9
2632  0171 090920202020  	dc.b	9,9,32,32,32,32
2633  0177 090944455654  	dc.b	9,9,68,69,86,84
2634  017d 5950453a5357  	dc.b	"YPE:SWITCH    ",9
2635  018c 7d202020205d  	dc.b	"}    ]}}",0
2668                     ; 41 void init(void)
2668                     ; 42 {
2670                     	switch	.text
2671  0000               _init:
2675                     ; 44     driver_init();
2677  0000 cd0000        	call	_driver_init
2679                     ; 46     SendPkgData('C',(U8*)DEVICE_INFO,sizeof(DEVICE_INFO));
2681  0003 ae0195        	ldw	x,#405
2682  0006 89            	pushw	x
2683  0007 ae0000        	ldw	x,#L7461_DEVICE_INFO
2684  000a 89            	pushw	x
2685  000b a643          	ld	a,#67
2686  000d cd0000        	call	_SendPkgData
2688  0010 5b04          	addw	sp,#4
2689                     ; 47 }
2692  0012 81            	ret
2695                     	bsct
2696  0000               L7661_i:
2697  0000 0000          	dc.w	0
2698                     	xref	_report_system_state
2735                     ; 55 void SystemEnterPer1ms(void)
2735                     ; 56 {
2736                     	switch	.text
2737  0013               _SystemEnterPer1ms:
2741                     ; 60     i++;
2743  0013 be00          	ldw	x,L7661_i
2744  0015 1c0001        	addw	x,#1
2745  0018 bf00          	ldw	L7661_i,x
2746                     ; 62     if(i%233==0)SYSTEM_RUNNING_IND();
2748  001a be00          	ldw	x,L7661_i
2749  001c 90ae00e9      	ldw	y,#233
2750  0020 cd0000        	call	c_idiv
2752  0023 51            	exgw	x,y
2753  0024 a30000        	cpw	x,#0
2754  0027 2604          	jrne	L7071
2757  0029 901a5005      	bcpl	_indicator
2758  002d               L7071:
2759                     ; 64     if(i==1000){
2761  002d be00          	ldw	x,L7661_i
2762  002f a303e8        	cpw	x,#1000
2763  0032 2610          	jrne	L5171
2764                     ; 65         i=0;
2766  0034 5f            	clrw	x
2767  0035 bf00          	ldw	L7661_i,x
2768                     ; 66         myprintf("T=%d",sys.current_temp);
2770  0037 be06          	ldw	x,_sys+6
2771  0039 89            	pushw	x
2772  003a ae0195        	ldw	x,#L7171
2773  003d cd0000        	call	_myprintf
2775  0040 85            	popw	x
2776                     ; 67         report_system_state();
2778  0041 cd0000        	call	_report_system_state
2780  0044               L5171:
2781                     ; 69     drive_device();
2783  0044 cd0000        	call	_drive_device
2785                     ; 70 }
2788  0047 81            	ret
2791                     	bsct
2792  0002               L1271_system_state:
2793  0002 00            	dc.b	0
2869                     ; 81 void SetProgrameStae(PRG_STATE state)
2869                     ; 82 {
2870                     	switch	.text
2871  0048               _SetProgrameStae:
2875                     ; 83     system_state = state;
2877  0048 b702          	ld	L1271_system_state,a
2878                     ; 84 }
2881  004a 81            	ret
2906                     ; 92 PRG_STATE GetProgrameStae(void)
2906                     ; 93 {
2907                     	switch	.text
2908  004b               _GetProgrameStae:
2912                     ; 94     return system_state;
2914  004b b602          	ld	a,L1271_system_state
2917  004d 81            	ret
2952                     	xdef	_SystemEnterPer1ms
2953                     	xdef	_init
2954                     	xdef	_GetProgrameStae
2955                     	xdef	_SetProgrameStae
2956                     	xref	_SendPkgData
2957                     	xref.b	_sys
2958                     	xref	_myprintf
2959                     	xref	_drive_device
2960                     	xref	_driver_init
2961                     	switch	.const
2962  0195               L7171:
2963  0195 543d256400    	dc.b	"T=%d",0
2983                     	xref	c_idiv
2984                     	end
