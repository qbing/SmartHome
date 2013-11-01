   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Parser V4.10.2 - 02 Nov 2011
   3                     ; Generator (Limited) V4.3.7 - 29 Nov 2011
2610                     ; 35 void driver_init(void)
2610                     ; 36 {
2612                     	switch	.text
2613  0000               _driver_init:
2617                     ; 37   io_port_init();
2619  0000 ad60          	call	L7461_io_port_init
2621                     ; 38   time_init();
2623  0002 cd00bb        	call	L1561_time_init
2625                     ; 39   UART_Init();
2627  0005 ad02          	call	L3561_UART_Init
2629                     ; 40   INTERUPT_ENABLE();
2632  0007 9a            rim
2634                     ; 41 }
2637  0008 81            	ret
2669                     ; 44 static void UART_Init(void)
2669                     ; 45 {
2670                     	switch	.text
2671  0009               L3561_UART_Init:
2675                     ; 46   	PC_DDR |= 0x20;             // C7CXC5C4 C3CXCXCX 	 1111 1000
2677  0009 721a500c      	bset	_PC_DDR,#5
2678                     ; 47 	PC_CR1 |= 0x20;              	
2680  000d 721a500d      	bset	_PC_CR1,#5
2681                     ; 48 	PC_CR2 &= ~0x20;             // Output speed up to 10MHz
2683  0011 721b500e      	bres	_PC_CR2,#5
2684                     ; 60     UART1_SR  = 0;
2686  0015 725f5230      	clr	_UART1_SR
2687                     ; 61     UART1_CR1 = 0;    		//M[4]=0：1个起始位，8个数据位
2689  0019 725f5234      	clr	_UART1_CR1
2690                     ; 62 	UART1_CR3 = 0;    		//STOP: 停止位 00：1个停止位； 
2692  001d 725f5236      	clr	_UART1_CR3
2693                     ; 63 	UART1_BRR2 = 0x02;      // BaudRate=9600, 00d0	先写UART1_BRR2再写UART1_BRR1
2695  0021 35025233      	mov	_UART1_BRR2,#2
2696                     ; 64 	UART1_BRR1 = 0x68;      //16M/9600=1666=0x0682
2698  0025 35685232      	mov	_UART1_BRR1,#104
2699                     ; 65     UART1_CR2 = 0x2e;
2701  0029 352e5235      	mov	_UART1_CR2,#46
2702                     ; 67 }
2705  002d 81            	ret
2743                     ; 86 void drive_device(void)
2743                     ; 87 {
2744                     	switch	.text
2745  002e               _drive_device:
2749                     ; 93   PD_DDR |= 0x0c;
2751  002e c65011        	ld	a,_PD_DDR
2752  0031 aa0c          	or	a,#12
2753  0033 c75011        	ld	_PD_DDR,a
2754                     ; 94   PD_CR1 |= 0x0c;
2756  0036 c65012        	ld	a,_PD_CR1
2757  0039 aa0c          	or	a,#12
2758  003b c75012        	ld	_PD_CR1,a
2759                     ; 95   PD_CR1 &= ~0x0c;
2761  003e c65012        	ld	a,_PD_CR1
2762  0041 a4f3          	and	a,#243
2763  0043 c75012        	ld	_PD_CR1,a
2764                     ; 96   lamb1Led = sys.lamb1_state;
2766                     	btst		_sys+8,#0
2767  004b 9017500f      	bccm	_lamb1Led
2768                     ; 97   lamb2Led = sys.lamb2_state;
2770                     	btst		_sys+8,#1
2771  0054 9015500f      	bccm	_lamb2Led
2772                     ; 98   cdPlayer = sys.cdplayer_state;
2774                     	btst		_sys+8,#2
2775  005d 9013500f      	bccm	_cdPlayer
2776                     ; 120 }
2779  0061 81            	ret
2813                     ; 122 static void io_port_init(void)
2813                     ; 123 {
2814                     	switch	.text
2815  0062               L7461_io_port_init:
2819                     ; 124     CLK_CKDIVR = 0x00;    			// 16MHz +16M CPU
2821  0062 725f50c6      	clr	_CLK_CKDIVR
2822                     ; 126     PA_DDR |= 0x04;
2824  0066 72145002      	bset	_PA_DDR,#2
2825                     ; 127     PA_CR1 |= 0x04;
2827  006a 72145003      	bset	_PA_CR1,#2
2828                     ; 128     PA_CR2 &= ~0x04;
2830  006e 72155004      	bres	_PA_CR2,#2
2831                     ; 130     PA_DDR |= 0x02;
2833  0072 72125002      	bset	_PA_DDR,#1
2834                     ; 131     PA_CR1 |= 0x02;
2836  0076 72125003      	bset	_PA_CR1,#1
2837                     ; 132     PA_CR2 &= ~0x02;
2839  007a 72135004      	bres	_PA_CR2,#1
2840                     ; 134     PB_DDR |= 0x20;
2842  007e 721a5007      	bset	_PB_DDR,#5
2843                     ; 135     PB_CR1 |= 0x20;
2845  0082 721a5008      	bset	_PB_CR1,#5
2846                     ; 136     PB_CR1 &= ~0x20;
2848  0086 721b5008      	bres	_PB_CR1,#5
2849                     ; 138     PD_DDR |= 0x1C;
2851  008a c65011        	ld	a,_PD_DDR
2852  008d aa1c          	or	a,#28
2853  008f c75011        	ld	_PD_DDR,a
2854                     ; 139     PD_CR1 |= 0x1C;
2856  0092 c65012        	ld	a,_PD_CR1
2857  0095 aa1c          	or	a,#28
2858  0097 c75012        	ld	_PD_CR1,a
2859                     ; 140     PD_CR1 &= ~0x1C;
2861  009a c65012        	ld	a,_PD_CR1
2862  009d a4e3          	and	a,#227
2863  009f c75012        	ld	_PD_CR1,a
2864                     ; 142 	PC_DDR |= 0xF8;             // C7CXC5C4 C3CXCXCX 	 1111 1000
2866  00a2 c6500c        	ld	a,_PC_DDR
2867  00a5 aaf8          	or	a,#248
2868  00a7 c7500c        	ld	_PC_DDR,a
2869                     ; 143 	PC_CR1 |= 0xF8;              	
2871  00aa c6500d        	ld	a,_PC_CR1
2872  00ad aaf8          	or	a,#248
2873  00af c7500d        	ld	_PC_CR1,a
2874                     ; 144 	PC_CR2 &= ~0xF8;             // Output speed up to 10MHz
2876  00b2 c6500e        	ld	a,_PC_CR2
2877  00b5 a407          	and	a,#7
2878  00b7 c7500e        	ld	_PC_CR2,a
2879                     ; 145 }
2882  00ba 81            	ret
2911                     ; 147 static void time_init(void)
2911                     ; 148 {
2912                     	switch	.text
2913  00bb               L1561_time_init:
2917                     ; 149   	TIM1_CR1 &= ~0x01; //关闭TIM1 
2919  00bb 72115250      	bres	_TIM1_CR1,#0
2920                     ; 157     TIM4_SR     = 0x00;                       // 清除更新标志
2922  00bf 725f5344      	clr	_TIM4_SR
2923                     ; 158     TIM4_CR1    = 0x00;
2925  00c3 725f5340      	clr	_TIM4_CR1
2926                     ; 159     TIM4_IER    = 0x00;
2928  00c7 725f5343      	clr	_TIM4_IER
2929                     ; 160     TIM4_PSCR   = 0x07;
2931  00cb 35075347      	mov	_TIM4_PSCR,#7
2932                     ; 161     TIM4_ARR    = 123;//124
2934  00cf 357b5348      	mov	_TIM4_ARR,#123
2935                     ; 162     TIM4_CR1    |= 0x01;// 设置控制器，启动定时器
2937  00d3 72105340      	bset	_TIM4_CR1,#0
2938                     ; 163     TIM4_IER    = 0x01;
2940  00d7 35015343      	mov	_TIM4_IER,#1
2941                     ; 170 }
2944  00db 81            	ret
2947                     	switch	.ubsct
2948  0000               L3371_recv_data:
2949  0000 000000000000  	ds.b	33
3045                     ; 175 @far @interrupt void UART1_RX_DATA_FULL(void)
3045                     ; 176 {
3047                     	switch	.text
3048  00dc               f_UART1_RX_DATA_FULL:
3050  00dc 8a            	push	cc
3051  00dd 84            	pop	a
3052  00de a4bf          	and	a,#191
3053  00e0 88            	push	a
3054  00e1 86            	pop	cc
3055       00000003      OFST:	set	3
3056  00e2 3b0002        	push	c_x+2
3057  00e5 be00          	ldw	x,c_x
3058  00e7 89            	pushw	x
3059  00e8 3b0002        	push	c_y+2
3060  00eb be00          	ldw	x,c_y
3061  00ed 89            	pushw	x
3062  00ee 5203          	subw	sp,#3
3065                     ; 180   c = UART1_DR; // 接收到数据了
3067  00f0 c65231        	ld	a,_UART1_DR
3068  00f3 6b03          	ld	(OFST+0,sp),a
3070  00f5               L1102:
3071                     ; 181   while((UART1_SR & 0x80) == 0x00);
3073  00f5 c65230        	ld	a,_UART1_SR
3074  00f8 a580          	bcp	a,#128
3075  00fa 27f9          	jreq	L1102
3076                     ; 183   if(c>'a' && c<'z')c=c+32;
3078  00fc 7b03          	ld	a,(OFST+0,sp)
3079  00fe a162          	cp	a,#98
3080  0100 250c          	jrult	L5102
3082  0102 7b03          	ld	a,(OFST+0,sp)
3083  0104 a17a          	cp	a,#122
3084  0106 2406          	jruge	L5102
3087  0108 7b03          	ld	a,(OFST+0,sp)
3088  010a ab20          	add	a,#32
3089  010c 6b03          	ld	(OFST+0,sp),a
3090  010e               L5102:
3091                     ; 185   if(MAX_RECV_BUFFER_SIZE>recv_data.recved_size)
3093  010e b620          	ld	a,L3371_recv_data+32
3094  0110 a120          	cp	a,#32
3095  0112 240e          	jruge	L7102
3096                     ; 187      recv_data.recv_buffer[recv_data.recved_size++]=c;
3098  0114 b620          	ld	a,L3371_recv_data+32
3099  0116 97            	ld	xl,a
3100  0117 3c20          	inc	L3371_recv_data+32
3101  0119 9f            	ld	a,xl
3102  011a 5f            	clrw	x
3103  011b 97            	ld	xl,a
3104  011c 7b03          	ld	a,(OFST+0,sp)
3105  011e e700          	ld	(L3371_recv_data,x),a
3107  0120 2002          	jra	L1202
3108  0122               L7102:
3109                     ; 190     recv_data.recved_size = 0;
3111  0122 3f20          	clr	L3371_recv_data+32
3112  0124               L1202:
3113                     ; 193     S8 used_len = recived_data_handler(&recv_data);
3115  0124 ae0000        	ldw	x,#L3371_recv_data
3116  0127 cd0000        	call	_recived_data_handler
3118  012a 6b02          	ld	(OFST-1,sp),a
3119                     ; 194     U8 i = 0;
3121                     ; 195     ASSERT(used_len>=0);
3123  012c 9c            	rvf
3124  012d 7b02          	ld	a,(OFST-1,sp)
3125  012f a100          	cp	a,#0
3126  0131 2f05          	jrslt	L02
3127  0133 ae0001        	ldw	x,#1
3128  0136 2001          	jra	L22
3129  0138               L02:
3130  0138 5f            	clrw	x
3131  0139               L22:
3132                     ; 197     if(used_len<recv_data.recved_size)
3134  0139 9c            	rvf
3135  013a 7b02          	ld	a,(OFST-1,sp)
3136  013c 5f            	clrw	x
3137  013d 4d            	tnz	a
3138  013e 2a01          	jrpl	L42
3139  0140 53            	cplw	x
3140  0141               L42:
3141  0141 97            	ld	xl,a
3142  0142 b620          	ld	a,L3371_recv_data+32
3143  0144 905f          	clrw	y
3144  0146 9097          	ld	yl,a
3145  0148 90bf00        	ldw	c_y,y
3146  014b b300          	cpw	x,c_y
3147  014d 2e2f          	jrsge	L3202
3148                     ; 199       U8 move_len = recv_data.recved_size - used_len;
3150  014f b620          	ld	a,L3371_recv_data+32
3151  0151 1002          	sub	a,(OFST-1,sp)
3152  0153 6b01          	ld	(OFST-2,sp),a
3153                     ; 200       for(i=0;i<move_len;i++){
3155  0155 0f03          	clr	(OFST+0,sp)
3157  0157 2019          	jra	L1302
3158  0159               L5202:
3159                     ; 201         recv_data.recv_buffer[i] = recv_data.recv_buffer[used_len+i];
3161  0159 7b03          	ld	a,(OFST+0,sp)
3162  015b 5f            	clrw	x
3163  015c 97            	ld	xl,a
3164  015d 89            	pushw	x
3165  015e 7b04          	ld	a,(OFST+1,sp)
3166  0160 5f            	clrw	x
3167  0161 4d            	tnz	a
3168  0162 2a01          	jrpl	L62
3169  0164 53            	cplw	x
3170  0165               L62:
3171  0165 1b05          	add	a,(OFST+2,sp)
3172  0167 2401          	jrnc	L03
3173  0169 5c            	incw	x
3174  016a               L03:
3175  016a 02            	rlwa	x,a
3176  016b e600          	ld	a,(L3371_recv_data,x)
3177  016d 85            	popw	x
3178  016e e700          	ld	(L3371_recv_data,x),a
3179                     ; 200       for(i=0;i<move_len;i++){
3181  0170 0c03          	inc	(OFST+0,sp)
3182  0172               L1302:
3185  0172 7b03          	ld	a,(OFST+0,sp)
3186  0174 1101          	cp	a,(OFST-2,sp)
3187  0176 25e1          	jrult	L5202
3188                     ; 203       recv_data.recved_size = move_len;
3190  0178 7b01          	ld	a,(OFST-2,sp)
3191  017a b720          	ld	L3371_recv_data+32,a
3193  017c 2002          	jra	L5302
3194  017e               L3202:
3195                     ; 205       recv_data.recved_size = 0;
3197  017e 3f20          	clr	L3371_recv_data+32
3198  0180               L5302:
3199                     ; 210 }
3202  0180 5b03          	addw	sp,#3
3203  0182 85            	popw	x
3204  0183 bf00          	ldw	c_y,x
3205  0185 320002        	pop	c_y+2
3206  0188 85            	popw	x
3207  0189 bf00          	ldw	c_x,x
3208  018b 320002        	pop	c_x+2
3209  018e 80            	iret
3211                     	bsct
3212  0000               L7302_isSending:
3213  0000 00            	dc.b	0
3275                     ; 235 void sendData(U8* data,U8 size)
3275                     ; 236 {
3277                     	switch	.text
3278  018f               _sendData:
3280  018f 89            	pushw	x
3281  0190 88            	push	a
3282       00000001      OFST:	set	1
3285                     ; 237     U8 i=0;
3287                     ; 240     if(isSending)return;//!!!!!!!!!ignor current send!!!!!!!!!!
3289  0191 3d00          	tnz	L7302_isSending
3290  0193 262e          	jrne	L43
3293                     ; 243     isSending = 1;            
3295  0195 35010000      	mov	L7302_isSending,#1
3296                     ; 248     for(i=0;i<size;i++){
3298  0199 0f01          	clr	(OFST+0,sp)
3300  019b 201e          	jra	L1012
3301  019d               L7012:
3302                     ; 249     	while((UART1_SR & 0x80) == 0x00);			
3304  019d c65230        	ld	a,_UART1_SR
3305  01a0 a580          	bcp	a,#128
3306  01a2 27f9          	jreq	L7012
3307                     ; 259     	UART1_DR = *data++;
3309  01a4 1e02          	ldw	x,(OFST+1,sp)
3310  01a6 1c0001        	addw	x,#1
3311  01a9 1f02          	ldw	(OFST+1,sp),x
3312  01ab 1d0001        	subw	x,#1
3313  01ae f6            	ld	a,(x)
3314  01af c75231        	ld	_UART1_DR,a
3316  01b2               L7112:
3317                     ; 260     	while((UART1_SR & 0x80) == 0x00);
3319  01b2 c65230        	ld	a,_UART1_SR
3320  01b5 a580          	bcp	a,#128
3321  01b7 27f9          	jreq	L7112
3322                     ; 248     for(i=0;i<size;i++){
3324  01b9 0c01          	inc	(OFST+0,sp)
3325  01bb               L1012:
3328  01bb 7b01          	ld	a,(OFST+0,sp)
3329  01bd 1106          	cp	a,(OFST+5,sp)
3330  01bf 25dc          	jrult	L7012
3331                     ; 262     isSending = 0;
3333  01c1 3f00          	clr	L7302_isSending
3334                     ; 267 }
3335  01c3               L43:
3338  01c3 5b03          	addw	sp,#3
3339  01c5 81            	ret
3463                     ; 281 void operate(DEVICE_ID dev_id,OP_TYPE op_type,void* op_data)
3463                     ; 282 {
3464                     	switch	.text
3465  01c6               _operate:
3467  01c6 89            	pushw	x
3468       00000000      OFST:	set	0
3471                     ; 283   if(ID_INDICATOR == dev_id){
3473  01c7 9e            	ld	a,xh
3474  01c8 a104          	cp	a,#4
3475  01ca 2606          	jrne	L7222
3476                     ; 284     indicator = ~indicator;
3478  01cc 901a5005      	bcpl	_indicator
3480  01d0 201e          	jra	L1322
3481  01d2               L7222:
3482                     ; 286   switch(op_type)
3484  01d2 7b02          	ld	a,(OFST+2,sp)
3486                     ; 326       break;
3487  01d4 4d            	tnz	a
3488  01d5 2708          	jreq	L3212
3489  01d7 4a            	dec	a
3490  01d8 2726          	jreq	L3312
3491  01da 4a            	dec	a
3492  01db 2744          	jreq	L3412
3493  01dd 2011          	jra	L1322
3494  01df               L3212:
3495                     ; 288     case OP_OPEN:
3495                     ; 289       switch(dev_id){
3497  01df 7b01          	ld	a,(OFST+1,sp)
3499                     ; 298        break;
3500  01e1 4d            	tnz	a
3501  01e2 2708          	jreq	L5212
3502  01e4 4a            	dec	a
3503  01e5 270b          	jreq	L7212
3504  01e7 4a            	dec	a
3505  01e8 270e          	jreq	L1312
3506  01ea 2004          	jra	L1322
3507  01ec               L5212:
3508                     ; 290         case ID_LAMB1:
3508                     ; 291         sys.lamb1_state = 1;
3510  01ec 72100008      	bset	_sys+8,#0
3511                     ; 292         break;
3512  01f0               L1322:
3513                     ; 328 }
3516  01f0 85            	popw	x
3517  01f1 81            	ret
3518  01f2               L7212:
3519                     ; 293         case ID_LAMB2:
3519                     ; 294         sys.lamb2_state = 1;
3521  01f2 72120008      	bset	_sys+8,#1
3522                     ; 295         break;
3524  01f6 20f8          	jra	L1322
3525  01f8               L1312:
3526                     ; 296         case ID_CDPLAYER:
3526                     ; 297         sys.cdplayer_state = 1;
3528  01f8 72140008      	bset	_sys+8,#2
3529                     ; 298        break;
3531  01fc 20f2          	jra	L1322
3532  01fe               L1422:
3533                     ; 300       break;
3535  01fe 20f0          	jra	L1322
3536  0200               L3312:
3537                     ; 301     case OP_CLOSE:
3537                     ; 302       switch(dev_id){
3539  0200 7b01          	ld	a,(OFST+1,sp)
3541                     ; 311         break;
3542  0202 4d            	tnz	a
3543  0203 2708          	jreq	L5312
3544  0205 4a            	dec	a
3545  0206 270b          	jreq	L7312
3546  0208 4a            	dec	a
3547  0209 270e          	jreq	L1412
3548  020b 20e3          	jra	L1322
3549  020d               L5312:
3550                     ; 303         case ID_LAMB1:
3550                     ; 304         sys.lamb1_state = 0;
3552  020d 72110008      	bres	_sys+8,#0
3553                     ; 305         break;
3555  0211 20dd          	jra	L1322
3556  0213               L7312:
3557                     ; 306         case ID_LAMB2:
3557                     ; 307         sys.lamb2_state = 0;
3559  0213 72130008      	bres	_sys+8,#1
3560                     ; 308         break;
3562  0217 20d7          	jra	L1322
3563  0219               L1412:
3564                     ; 309         case ID_CDPLAYER:
3564                     ; 310         sys.cdplayer_state = 0;
3566  0219 72150008      	bres	_sys+8,#2
3567                     ; 311         break;
3569  021d 20d1          	jra	L1322
3570  021f               L5422:
3571                     ; 313       break;
3573  021f 20cf          	jra	L1322
3574  0221               L3412:
3575                     ; 314     case OP_NOT:
3575                     ; 315       switch(dev_id){
3577  0221 7b01          	ld	a,(OFST+1,sp)
3579                     ; 324         break;
3580  0223 4d            	tnz	a
3581  0224 2708          	jreq	L5412
3582  0226 4a            	dec	a
3583  0227 2711          	jreq	L7412
3584  0229 4a            	dec	a
3585  022a 271a          	jreq	L1512
3586  022c 20c2          	jra	L1322
3587  022e               L5412:
3588                     ; 316         case ID_LAMB1:
3588                     ; 317         sys.lamb1_state = ~lamb1Led;
3590                     	btst		_lamb1Led
3591  0233 8c            	ccf
3592  0234 90110008      	bccm	_sys+8,#0
3593                     ; 318         break;
3595  0238 20b6          	jra	L1322
3596  023a               L7412:
3597                     ; 319         case ID_LAMB2:
3597                     ; 320         sys.lamb2_state = ~lamb2Led;
3599                     	btst		_lamb2Led
3600  023f 8c            	ccf
3601  0240 90130008      	bccm	_sys+8,#1
3602                     ; 321         break;
3604  0244 20aa          	jra	L1322
3605  0246               L1512:
3606                     ; 322         case ID_CDPLAYER:
3606                     ; 323         sys.cdplayer_state = ~sys.cdplayer_state;
3608  0246 90140008      	bcpl		_sys+8,#2
3609                     ; 324         break;
3611  024a 20a4          	jra	L1322
3612  024c               L1522:
3613                     ; 326       break;
3615  024c 20a2          	jra	L1322
3616  024e               L5322:
3617  024e 20a0          	jra	L1322
3679                     ; 340 U8 generate_state_buffer(U8* dest)
3679                     ; 341 {
3680                     	switch	.text
3681  0250               _generate_state_buffer:
3683  0250 89            	pushw	x
3684  0251 5207          	subw	sp,#7
3685       00000007      OFST:	set	7
3688                     ; 342   U8* p = dest;
3690  0253 1f06          	ldw	(OFST-1,sp),x
3691                     ; 344   myitoa(temperature_str,sys.current_temp,10);
3693  0255 4b0a          	push	#10
3694  0257 be06          	ldw	x,_sys+6
3695  0259 cd0000        	call	c_itolx
3697  025c be02          	ldw	x,c_lreg+2
3698  025e 89            	pushw	x
3699  025f be00          	ldw	x,c_lreg
3700  0261 89            	pushw	x
3701  0262 96            	ldw	x,sp
3702  0263 1c0006        	addw	x,#OFST-1
3703  0266 cd0000        	call	_myitoa
3705  0269 5b05          	addw	sp,#5
3706                     ; 345   *p= '\0';
3708  026b 1e06          	ldw	x,(OFST-1,sp)
3709  026d 7f            	clr	(x)
3710                     ; 346   p = mystrcat(p,"{CMD:TICK");
3712  026e ae0144        	ldw	x,#L1032
3713  0271 89            	pushw	x
3714  0272 1e08          	ldw	x,(OFST+1,sp)
3715  0274 cd0000        	call	_mystrcat
3717  0277 5b02          	addw	sp,#2
3718  0279 1f06          	ldw	(OFST-1,sp),x
3719                     ; 347   p = mystrcat(p,",TEMP:");
3721  027b ae013d        	ldw	x,#L3032
3722  027e 89            	pushw	x
3723  027f 1e08          	ldw	x,(OFST+1,sp)
3724  0281 cd0000        	call	_mystrcat
3726  0284 5b02          	addw	sp,#2
3727  0286 1f06          	ldw	(OFST-1,sp),x
3728                     ; 348   p = mystrcat(p,temperature_str);
3730  0288 96            	ldw	x,sp
3731  0289 1c0001        	addw	x,#OFST-6
3732  028c 89            	pushw	x
3733  028d 1e08          	ldw	x,(OFST+1,sp)
3734  028f cd0000        	call	_mystrcat
3736  0292 5b02          	addw	sp,#2
3737  0294 1f06          	ldw	(OFST-1,sp),x
3738                     ; 349   p = mystrcat(p,",LEMB1:");
3740  0296 ae0135        	ldw	x,#L5032
3741  0299 89            	pushw	x
3742  029a 1e08          	ldw	x,(OFST+1,sp)
3743  029c cd0000        	call	_mystrcat
3745  029f 5b02          	addw	sp,#2
3746  02a1 1f06          	ldw	(OFST-1,sp),x
3747                     ; 350   p = mystrcat(p,lamb1Led?"ON":"OFF");
3749                     	btst	_lamb1Led
3750  02a8 2405          	jruge	L24
3751  02aa ae0132        	ldw	x,#L7032
3752  02ad 2003          	jra	L44
3753  02af               L24:
3754  02af ae012e        	ldw	x,#L1132
3755  02b2               L44:
3756  02b2 89            	pushw	x
3757  02b3 1e08          	ldw	x,(OFST+1,sp)
3758  02b5 cd0000        	call	_mystrcat
3760  02b8 5b02          	addw	sp,#2
3761  02ba 1f06          	ldw	(OFST-1,sp),x
3762                     ; 351   p = mystrcat(p,",LEMB2:");
3764  02bc ae0126        	ldw	x,#L3132
3765  02bf 89            	pushw	x
3766  02c0 1e08          	ldw	x,(OFST+1,sp)
3767  02c2 cd0000        	call	_mystrcat
3769  02c5 5b02          	addw	sp,#2
3770  02c7 1f06          	ldw	(OFST-1,sp),x
3771                     ; 352   p = mystrcat(p,lamb2Led?"ON":"OFF");
3773                     	btst	_lamb2Led
3774  02ce 2405          	jruge	L64
3775  02d0 ae0132        	ldw	x,#L7032
3776  02d3 2003          	jra	L05
3777  02d5               L64:
3778  02d5 ae012e        	ldw	x,#L1132
3779  02d8               L05:
3780  02d8 89            	pushw	x
3781  02d9 1e08          	ldw	x,(OFST+1,sp)
3782  02db cd0000        	call	_mystrcat
3784  02de 5b02          	addw	sp,#2
3785  02e0 1f06          	ldw	(OFST-1,sp),x
3786                     ; 353   p = mystrcat(p,",CDPLAYER:");
3788  02e2 ae011b        	ldw	x,#L5132
3789  02e5 89            	pushw	x
3790  02e6 1e08          	ldw	x,(OFST+1,sp)
3791  02e8 cd0000        	call	_mystrcat
3793  02eb 5b02          	addw	sp,#2
3794  02ed 1f06          	ldw	(OFST-1,sp),x
3795                     ; 354   p = mystrcat(p,cdPlayer?"ON":"OFF");
3797                     	btst	_cdPlayer
3798  02f4 2405          	jruge	L25
3799  02f6 ae0132        	ldw	x,#L7032
3800  02f9 2003          	jra	L45
3801  02fb               L25:
3802  02fb ae012e        	ldw	x,#L1132
3803  02fe               L45:
3804  02fe 89            	pushw	x
3805  02ff 1e08          	ldw	x,(OFST+1,sp)
3806  0301 cd0000        	call	_mystrcat
3808  0304 5b02          	addw	sp,#2
3809  0306 1f06          	ldw	(OFST-1,sp),x
3810                     ; 355   p = mystrcat(p,",}");
3812  0308 ae0118        	ldw	x,#L7132
3813  030b 89            	pushw	x
3814  030c 1e08          	ldw	x,(OFST+1,sp)
3815  030e cd0000        	call	_mystrcat
3817  0311 5b02          	addw	sp,#2
3818  0313 1f06          	ldw	(OFST-1,sp),x
3819                     ; 356   return p-dest;
3821  0315 1e06          	ldw	x,(OFST-1,sp)
3822  0317 72f008        	subw	x,(OFST+1,sp)
3823  031a 9f            	ld	a,xl
3826  031b 5b09          	addw	sp,#9
3827  031d 81            	ret
3878                     ; 367 U16 GetADC(U8 chanel)
3878                     ; 368 {
3879                     	switch	.text
3880  031e               _GetADC:
3882  031e 5204          	subw	sp,#4
3883       00000004      OFST:	set	4
3886                     ; 372 	U16	x  = 0;
3888                     ; 376     PC_DDR &= ~0x10;
3890  0320 7219500c      	bres	_PC_DDR,#4
3891                     ; 377     PC_CR1 &= ~0x10;
3893  0324 7219500d      	bres	_PC_CR1,#4
3894                     ; 378     PC_CR2 &= ~0x10;
3896  0328 7219500e      	bres	_PC_CR2,#4
3897                     ; 379     ADC_CSR = chanel;
3899  032c c75400        	ld	_ADC_CSR,a
3900                     ; 380 	ADC_CR1 |= 0x01;
3902  032f 72105401      	bset	_ADC_CR1,#0
3903                     ; 382 	delay(100);
3905  0333 ae0064        	ldw	x,#100
3906  0336 cd0000        	call	_delay
3908                     ; 383 	ADC_CR1 |= 0x01;
3910  0339 72105401      	bset	_ADC_CR1,#0
3912  033d               L5432:
3913                     ; 384 	while( (ADC_CSR&0x80)==0 );
3915  033d c65400        	ld	a,_ADC_CSR
3916  0340 a580          	bcp	a,#128
3917  0342 27f9          	jreq	L5432
3918                     ; 387     x=ADC_DRH;x<<=2;x+=ADC_DRL;
3920  0344 c65404        	ld	a,_ADC_DRH
3921  0347 5f            	clrw	x
3922  0348 97            	ld	xl,a
3923  0349 1f03          	ldw	(OFST-1,sp),x
3926  034b 0804          	sll	(OFST+0,sp)
3927  034d 0903          	rlc	(OFST-1,sp)
3928  034f 0804          	sll	(OFST+0,sp)
3929  0351 0903          	rlc	(OFST-1,sp)
3932  0353 c65405        	ld	a,_ADC_DRL
3933  0356 5f            	clrw	x
3934  0357 97            	ld	xl,a
3935  0358 1f01          	ldw	(OFST-3,sp),x
3936  035a 1e03          	ldw	x,(OFST-1,sp)
3937  035c 72fb01        	addw	x,(OFST-3,sp)
3938  035f 1f03          	ldw	(OFST-1,sp),x
3939                     ; 388 	ADC_CSR  &=  ~0x80;//清除标志
3941  0361 721f5400      	bres	_ADC_CSR,#7
3942                     ; 390     return x;    
3944  0365 1e03          	ldw	x,(OFST-1,sp)
3947  0367 5b04          	addw	sp,#4
3948  0369 81            	ret
3951                     .const:	section	.text
3952  0000               L1532_TEMP_TABLE:
3953  0000 007f          	dc.w	127
3954  0002 0084          	dc.w	132
3955  0004 008a          	dc.w	138
3956  0006 0090          	dc.w	144
3957  0008 0096          	dc.w	150
3958  000a 009d          	dc.w	157
3959  000c 00a3          	dc.w	163
3960  000e 00aa          	dc.w	170
3961  0010 00b1          	dc.w	177
3962  0012 00b8          	dc.w	184
3963  0014 00bf          	dc.w	191
3964  0016 00c7          	dc.w	199
3965  0018 00ce          	dc.w	206
3966  001a 00d6          	dc.w	214
3967  001c 00de          	dc.w	222
3968  001e 00e6          	dc.w	230
3969  0020 00ee          	dc.w	238
3970  0022 00f6          	dc.w	246
3971  0024 00ff          	dc.w	255
3972  0026 0107          	dc.w	263
3973  0028 0110          	dc.w	272
3974  002a 0119          	dc.w	281
3975  002c 0122          	dc.w	290
3976  002e 012b          	dc.w	299
3977  0030 0134          	dc.w	308
3978  0032 013d          	dc.w	317
3979  0034 0147          	dc.w	327
3980  0036 0150          	dc.w	336
3981  0038 015a          	dc.w	346
3982  003a 0163          	dc.w	355
3983  003c 016d          	dc.w	365
3984  003e 0177          	dc.w	375
3985  0040 0180          	dc.w	384
3986  0042 018a          	dc.w	394
3987  0044 0194          	dc.w	404
3988  0046 019e          	dc.w	414
3989  0048 01a8          	dc.w	424
3990  004a 01b2          	dc.w	434
3991  004c 01bb          	dc.w	443
3992  004e 01c5          	dc.w	453
3993  0050 01cf          	dc.w	463
3994  0052 01d9          	dc.w	473
3995  0054 01e3          	dc.w	483
3996  0056 01ed          	dc.w	493
3997  0058 01f6          	dc.w	502
3998  005a 0200          	dc.w	512
3999  005c 020a          	dc.w	522
4000  005e 0213          	dc.w	531
4001  0060 021d          	dc.w	541
4002  0062 0226          	dc.w	550
4003  0064 022f          	dc.w	559
4004  0066 0239          	dc.w	569
4005  0068 0242          	dc.w	578
4006  006a 024b          	dc.w	587
4007  006c 0254          	dc.w	596
4008  006e 025d          	dc.w	605
4009  0070 0265          	dc.w	613
4010  0072 026e          	dc.w	622
4011  0074 0277          	dc.w	631
4012  0076 027f          	dc.w	639
4013  0078 0287          	dc.w	647
4014  007a 028f          	dc.w	655
4015  007c 0298          	dc.w	664
4016  007e 029f          	dc.w	671
4017  0080 02a7          	dc.w	679
4018  0082 02af          	dc.w	687
4019  0084 02b6          	dc.w	694
4020  0086 02be          	dc.w	702
4021  0088 02c5          	dc.w	709
4022  008a 02cc          	dc.w	716
4023  008c 02d3          	dc.w	723
4024  008e 02da          	dc.w	730
4025  0090 02e1          	dc.w	737
4026  0092 02e8          	dc.w	744
4027  0094 02ee          	dc.w	750
4028  0096 02f4          	dc.w	756
4029  0098 02fb          	dc.w	763
4030  009a 0301          	dc.w	769
4031  009c 0307          	dc.w	775
4032  009e 030d          	dc.w	781
4033  00a0 0312          	dc.w	786
4034  00a2 0318          	dc.w	792
4035  00a4 031d          	dc.w	797
4036  00a6 0323          	dc.w	803
4037  00a8 0328          	dc.w	808
4038  00aa 032d          	dc.w	813
4039  00ac 0332          	dc.w	818
4040  00ae 0337          	dc.w	823
4041  00b0 033c          	dc.w	828
4042  00b2 0340          	dc.w	832
4043  00b4 0345          	dc.w	837
4044  00b6 0349          	dc.w	841
4045  00b8 034e          	dc.w	846
4046  00ba 0352          	dc.w	850
4047  00bc 0356          	dc.w	854
4048  00be 035a          	dc.w	858
4049  00c0 035e          	dc.w	862
4050  00c2 0362          	dc.w	866
4051  00c4 0366          	dc.w	870
4052  00c6 0369          	dc.w	873
4053  00c8 036d          	dc.w	877
4054  00ca 0370          	dc.w	880
4055  00cc 0374          	dc.w	884
4056  00ce 0377          	dc.w	887
4057  00d0 037a          	dc.w	890
4058  00d2 037d          	dc.w	893
4059  00d4 0381          	dc.w	897
4060  00d6 0384          	dc.w	900
4061  00d8 0386          	dc.w	902
4062  00da 0389          	dc.w	905
4063  00dc 038c          	dc.w	908
4064  00de 038f          	dc.w	911
4065  00e0 0391          	dc.w	913
4066  00e2 0394          	dc.w	916
4067  00e4 0396          	dc.w	918
4068  00e6 0399          	dc.w	921
4069  00e8 039b          	dc.w	923
4070  00ea 039e          	dc.w	926
4071  00ec 03a0          	dc.w	928
4072  00ee 03a2          	dc.w	930
4073  00f0 03a4          	dc.w	932
4074  00f2 03a6          	dc.w	934
4075  00f4 03a8          	dc.w	936
4076  00f6 03aa          	dc.w	938
4077  00f8 03ac          	dc.w	940
4078  00fa 03ae          	dc.w	942
4079  00fc 03b0          	dc.w	944
4080  00fe 03b2          	dc.w	946
4081  0100 03b4          	dc.w	948
4082  0102 03b5          	dc.w	949
4083  0104 03b7          	dc.w	951
4084  0106 03b9          	dc.w	953
4085  0108 03ba          	dc.w	954
4086  010a 03bc          	dc.w	956
4087  010c 03bd          	dc.w	957
4088  010e 03bf          	dc.w	959
4089  0110 03c0          	dc.w	960
4090  0112 03c2          	dc.w	962
4091  0114 03c3          	dc.w	963
4092  0116 03c4          	dc.w	964
4153                     ; 411 static S16 calc_temperature_by_adc(U32 adc)//计算温度值
4153                     ; 412 {
4154                     	switch	.text
4155  036a               L3532_calc_temperature_by_adc:
4157  036a 5206          	subw	sp,#6
4158       00000006      OFST:	set	6
4161                     ; 416     S16 high = sizeof(TEMP_TABLE)/sizeof(TEMP_TABLE[0]);
4163  036c ae008c        	ldw	x,#140
4164  036f 1f01          	ldw	(OFST-5,sp),x
4165                     ; 417     S16 low = 0;
4167  0371 5f            	clrw	x
4168  0372 1f03          	ldw	(OFST-3,sp),x
4169                     ; 418     S16 mid = 0;
4171  0374 5f            	clrw	x
4172  0375 1f05          	ldw	(OFST-1,sp),x
4174  0377 2038          	jra	L3142
4175  0379               L7042:
4176                     ; 425         mid = (high + low)>>1;
4178  0379 1e01          	ldw	x,(OFST-5,sp)
4179  037b 72fb03        	addw	x,(OFST-3,sp)
4180  037e 57            	sraw	x
4181  037f 1f05          	ldw	(OFST-1,sp),x
4182                     ; 426         if(TEMP_TABLE[mid]>adc)
4184  0381 1e05          	ldw	x,(OFST-1,sp)
4185  0383 58            	sllw	x
4186  0384 de0000        	ldw	x,(L1532_TEMP_TABLE,x)
4187  0387 cd0000        	call	c_uitolx
4189  038a 96            	ldw	x,sp
4190  038b 1c0009        	addw	x,#OFST+3
4191  038e cd0000        	call	c_lcmp
4193  0391 2307          	jrule	L7142
4194                     ; 427             high = mid - 1;
4196  0393 1e05          	ldw	x,(OFST-1,sp)
4197  0395 5a            	decw	x
4198  0396 1f01          	ldw	(OFST-5,sp),x
4200  0398 2017          	jra	L3142
4201  039a               L7142:
4202                     ; 428         else if(TEMP_TABLE[mid]<adc)
4204  039a 1e05          	ldw	x,(OFST-1,sp)
4205  039c 58            	sllw	x
4206  039d de0000        	ldw	x,(L1532_TEMP_TABLE,x)
4207  03a0 cd0000        	call	c_uitolx
4209  03a3 96            	ldw	x,sp
4210  03a4 1c0009        	addw	x,#OFST+3
4211  03a7 cd0000        	call	c_lcmp
4213  03aa 240c          	jruge	L5142
4214                     ; 429             low = mid +1;
4216  03ac 1e05          	ldw	x,(OFST-1,sp)
4217  03ae 5c            	incw	x
4218  03af 1f03          	ldw	(OFST-3,sp),x
4220  03b1               L3142:
4221                     ; 423     while(low<=high)
4223  03b1 9c            	rvf
4224  03b2 1e03          	ldw	x,(OFST-3,sp)
4225  03b4 1301          	cpw	x,(OFST-5,sp)
4226  03b6 2dc1          	jrsle	L7042
4227  03b8               L5142:
4228                     ; 433     return (mid-20);
4230  03b8 1e05          	ldw	x,(OFST-1,sp)
4231  03ba 1d0014        	subw	x,#20
4234  03bd 5b06          	addw	sp,#6
4235  03bf 81            	ret
4260                     ; 436 S16 GetT(void)
4260                     ; 437 {
4261                     	switch	.text
4262  03c0               _GetT:
4266                     ; 438   return calc_temperature_by_adc(GetADC(ENVIROMENT_TEMP_CHANEL));
4268  03c0 a602          	ld	a,#2
4269  03c2 cd031e        	call	_GetADC
4271  03c5 cd0000        	call	c_uitolx
4273  03c8 be02          	ldw	x,c_lreg+2
4274  03ca 89            	pushw	x
4275  03cb be00          	ldw	x,c_lreg
4276  03cd 89            	pushw	x
4277  03ce ad9a          	call	L3532_calc_temperature_by_adc
4279  03d0 5b04          	addw	sp,#4
4282  03d2 81            	ret
4310                     ; 448 void report_system_state(void)
4310                     ; 449 {
4311                     	switch	.text
4312  03d3               _report_system_state:
4316                     ; 450   SYSTEM_RUNNING_IND();
4318  03d3 901a5005      	bcpl	_indicator
4319                     ; 452   sys.current_temp = GetT();
4321  03d7 ade7          	call	_GetT
4323  03d9 bf06          	ldw	_sys+6,x
4324                     ; 454   sendData((U8*)&sys,sizeof(sys));
4326  03db 4b09          	push	#9
4327  03dd ae0000        	ldw	x,#_sys
4328  03e0 cd018f        	call	_sendData
4330  03e3 84            	pop	a
4331                     ; 455 }
4334  03e4 81            	ret
4389                     	xdef	_report_system_state
4390                     	xdef	_GetADC
4391                     	xdef	f_UART1_RX_DATA_FULL
4392                     	xdef	_GetT
4393                     	xdef	_generate_state_buffer
4394                     	xdef	_operate
4395                     	xdef	_drive_device
4396                     	xdef	_driver_init
4397                     	xref	_recived_data_handler
4398                     	xref.b	_sys
4399                     	xref	_myitoa
4400                     	xref	_mystrcat
4401                     	xdef	_sendData
4402                     	xref	_delay
4403                     	switch	.const
4404  0118               L7132:
4405  0118 2c7d00        	dc.b	",}",0
4406  011b               L5132:
4407  011b 2c4344504c41  	dc.b	",CDPLAYER:",0
4408  0126               L3132:
4409  0126 2c4c454d4232  	dc.b	",LEMB2:",0
4410  012e               L1132:
4411  012e 4f464600      	dc.b	"OFF",0
4412  0132               L7032:
4413  0132 4f4e00        	dc.b	"ON",0
4414  0135               L5032:
4415  0135 2c4c454d4231  	dc.b	",LEMB1:",0
4416  013d               L3032:
4417  013d 2c54454d503a  	dc.b	",TEMP:",0
4418  0144               L1032:
4419  0144 7b434d443a54  	dc.b	"{CMD:TICK",0
4420                     	xref.b	c_lreg
4421                     	xref.b	c_x
4422                     	xref.b	c_y
4442                     	xref	c_lcmp
4443                     	xref	c_uitolx
4444                     	xref	c_itolx
4445                     	end
