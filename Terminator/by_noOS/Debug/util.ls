   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Parser V4.10.2 - 02 Nov 2011
   3                     ; Generator (Limited) V4.3.7 - 29 Nov 2011
2618                     ; 23 void putc(U8 byte)
2618                     ; 24 {
2620                     	switch	.text
2621  0000               _putc:
2623  0000 88            	push	a
2624       00000000      OFST:	set	0
2627                     ; 25     sendData(&byte,1);
2629  0001 4b01          	push	#1
2630  0003 96            	ldw	x,sp
2631  0004 1c0002        	addw	x,#OFST+2
2632  0007 cd0000        	call	_sendData
2634  000a 84            	pop	a
2635                     ; 26 }
2638  000b 84            	pop	a
2639  000c 81            	ret
2684                     ; 27 void puts(U8 *string)
2684                     ; 28 {
2685                     	switch	.text
2686  000d               _puts:
2688  000d 89            	pushw	x
2689  000e 89            	pushw	x
2690       00000002      OFST:	set	2
2693                     ; 29     int i=0;
2695  000f 5f            	clrw	x
2696  0010 1f01          	ldw	(OFST-1,sp),x
2698  0012 2007          	jra	L1271
2699  0014               L5171:
2700                     ; 30     while(string[i])i++;
2702  0014 1e01          	ldw	x,(OFST-1,sp)
2703  0016 1c0001        	addw	x,#1
2704  0019 1f01          	ldw	(OFST-1,sp),x
2705  001b               L1271:
2708  001b 1e01          	ldw	x,(OFST-1,sp)
2709  001d 72fb03        	addw	x,(OFST+1,sp)
2710  0020 7d            	tnz	(x)
2711  0021 26f1          	jrne	L5171
2712                     ; 31     sendData(string,i);
2714  0023 7b02          	ld	a,(OFST+0,sp)
2715  0025 88            	push	a
2716  0026 1e04          	ldw	x,(OFST+2,sp)
2717  0028 cd0000        	call	_sendData
2719  002b 84            	pop	a
2720                     ; 32 }
2723  002c 5b04          	addw	sp,#4
2724  002e 81            	ret
2778                     ; 34 S8 mystrcmp(const char* str1,const char* str2,S8 len)
2778                     ; 35 {
2779                     	switch	.text
2780  002f               _mystrcmp:
2782  002f 89            	pushw	x
2783       00000000      OFST:	set	0
2786                     ; 36   if(len<=0){len = 128;}
2788  0030 9c            	rvf
2789  0031 7b07          	ld	a,(OFST+7,sp)
2790  0033 a100          	cp	a,#0
2791  0035 2c16          	jrsgt	L7571
2794  0037 a680          	ld	a,#128
2795  0039 6b07          	ld	(OFST+7,sp),a
2796  003b 2010          	jra	L7571
2797  003d               L5571:
2798                     ; 40     len--  ;
2800  003d 0a07          	dec	(OFST+7,sp)
2801                     ; 41     str1++ ;
2803  003f 1e01          	ldw	x,(OFST+1,sp)
2804  0041 1c0001        	addw	x,#1
2805  0044 1f01          	ldw	(OFST+1,sp),x
2806                     ; 42     str2++ ;
2808  0046 1e05          	ldw	x,(OFST+5,sp)
2809  0048 1c0001        	addw	x,#1
2810  004b 1f05          	ldw	(OFST+5,sp),x
2811  004d               L7571:
2812                     ; 38   while(len>0  && *str1 == *str2 && *str1!='\0')
2814  004d 9c            	rvf
2815  004e 7b07          	ld	a,(OFST+7,sp)
2816  0050 a100          	cp	a,#0
2817  0052 2d0d          	jrsle	L3671
2819  0054 1e01          	ldw	x,(OFST+1,sp)
2820  0056 f6            	ld	a,(x)
2821  0057 1e05          	ldw	x,(OFST+5,sp)
2822  0059 f1            	cp	a,(x)
2823  005a 2605          	jrne	L3671
2825  005c 1e01          	ldw	x,(OFST+1,sp)
2826  005e 7d            	tnz	(x)
2827  005f 26dc          	jrne	L5571
2828  0061               L3671:
2829                     ; 44   return *str1-*str2;
2831  0061 1e05          	ldw	x,(OFST+5,sp)
2832  0063 f6            	ld	a,(x)
2833  0064 1e01          	ldw	x,(OFST+1,sp)
2834  0066 f0            	sub	a,(x)
2835  0067 40            	neg	a
2838  0068 85            	popw	x
2839  0069 81            	ret
2885                     ; 48 U8* mystrcat(char* str1,char* str2)
2885                     ; 49 {
2886                     	switch	.text
2887  006a               _mystrcat:
2889  006a 89            	pushw	x
2890       00000000      OFST:	set	0
2893  006b 2007          	jra	L3102
2894  006d               L1102:
2895                     ; 50   while(*str1)str1++;
2897  006d 1e01          	ldw	x,(OFST+1,sp)
2898  006f 1c0001        	addw	x,#1
2899  0072 1f01          	ldw	(OFST+1,sp),x
2900  0074               L3102:
2903  0074 1e01          	ldw	x,(OFST+1,sp)
2904  0076 7d            	tnz	(x)
2905  0077 26f4          	jrne	L1102
2907  0079 2016          	jra	L1202
2908  007b               L7102:
2909                     ; 51   while(*str2)*str1++=*str2++;
2911  007b 1e05          	ldw	x,(OFST+5,sp)
2912  007d 1c0001        	addw	x,#1
2913  0080 1f05          	ldw	(OFST+5,sp),x
2914  0082 1d0001        	subw	x,#1
2915  0085 f6            	ld	a,(x)
2916  0086 1e01          	ldw	x,(OFST+1,sp)
2917  0088 1c0001        	addw	x,#1
2918  008b 1f01          	ldw	(OFST+1,sp),x
2919  008d 1d0001        	subw	x,#1
2920  0090 f7            	ld	(x),a
2921  0091               L1202:
2924  0091 1e05          	ldw	x,(OFST+5,sp)
2925  0093 7d            	tnz	(x)
2926  0094 26e5          	jrne	L7102
2927                     ; 52   *str1='\0';
2929  0096 1e01          	ldw	x,(OFST+1,sp)
2930  0098 7f            	clr	(x)
2931                     ; 53   return str1;
2933  0099 1e01          	ldw	x,(OFST+1,sp)
2936  009b 5b02          	addw	sp,#2
2937  009d 81            	ret
3027                     ; 56 U8 myitoa(char* dest,long d,U8 r)
3027                     ; 57 {
3028                     	switch	.text
3029  009e               _myitoa:
3031  009e 89            	pushw	x
3032  009f 5219          	subw	sp,#25
3033       00000019      OFST:	set	25
3036                     ; 59     int i = 0;
3038  00a1 5f            	clrw	x
3039  00a2 1f15          	ldw	(OFST-4,sp),x
3040                     ; 60     int j = -1;
3042  00a4 aeffff        	ldw	x,#65535
3043  00a7 1f17          	ldw	(OFST-2,sp),x
3044                     ; 61     char digtal=0;
3046                     ; 62     if(d==0)
3048  00a9 96            	ldw	x,sp
3049  00aa 1c001e        	addw	x,#OFST+5
3050  00ad cd0000        	call	c_lzmp
3052  00b0 2614          	jrne	L3702
3053                     ; 64         dest[i++]='0';
3055  00b2 1e15          	ldw	x,(OFST-4,sp)
3056  00b4 1c0001        	addw	x,#1
3057  00b7 1f15          	ldw	(OFST-4,sp),x
3058  00b9 1d0001        	subw	x,#1
3059  00bc 72fb1a        	addw	x,(OFST+1,sp)
3060  00bf a630          	ld	a,#48
3061  00c1 f7            	ld	(x),a
3063  00c2 ac520152      	jpf	L3012
3064  00c6               L3702:
3065                     ; 65     }else if(d<0){
3067  00c6 9c            	rvf
3068  00c7 9c            	rvf
3069  00c8 0d1e          	tnz	(OFST+5,sp)
3070  00ca 2f03          	jrslt	L22
3071  00cc cc0152        	jp	L3012
3072  00cf               L22:
3073                     ; 66         dest[i++]='-';
3075  00cf 1e15          	ldw	x,(OFST-4,sp)
3076  00d1 1c0001        	addw	x,#1
3077  00d4 1f15          	ldw	(OFST-4,sp),x
3078  00d6 1d0001        	subw	x,#1
3079  00d9 72fb1a        	addw	x,(OFST+1,sp)
3080  00dc a62d          	ld	a,#45
3081  00de f7            	ld	(x),a
3082                     ; 67         d=-d;
3084  00df 96            	ldw	x,sp
3085  00e0 1c001e        	addw	x,#OFST+5
3086  00e3 cd0000        	call	c_lgneg
3088  00e6 206a          	jra	L3012
3089  00e8               L1012:
3090                     ; 70         digtal = (d%r);
3092  00e8 7b22          	ld	a,(OFST+9,sp)
3093  00ea b703          	ld	c_lreg+3,a
3094  00ec 3f02          	clr	c_lreg+2
3095  00ee 3f01          	clr	c_lreg+1
3096  00f0 3f00          	clr	c_lreg
3097  00f2 96            	ldw	x,sp
3098  00f3 1c0001        	addw	x,#OFST-24
3099  00f6 cd0000        	call	c_rtol
3101  00f9 96            	ldw	x,sp
3102  00fa 1c001e        	addw	x,#OFST+5
3103  00fd cd0000        	call	c_ltor
3105  0100 96            	ldw	x,sp
3106  0101 1c0001        	addw	x,#OFST-24
3107  0104 cd0000        	call	c_lmod
3109  0107 b603          	ld	a,c_lreg+3
3110  0109 6b19          	ld	(OFST+0,sp),a
3111                     ; 71         dest_temp[++j] = (digtal<10)?digtal+'0':digtal+'A';
3113  010b 7b19          	ld	a,(OFST+0,sp)
3114  010d a10a          	cp	a,#10
3115  010f 2406          	jruge	L61
3116  0111 7b19          	ld	a,(OFST+0,sp)
3117  0113 ab30          	add	a,#48
3118  0115 2004          	jra	L02
3119  0117               L61:
3120  0117 7b19          	ld	a,(OFST+0,sp)
3121  0119 ab41          	add	a,#65
3122  011b               L02:
3123  011b 96            	ldw	x,sp
3124  011c 1c0005        	addw	x,#OFST-20
3125  011f 1f03          	ldw	(OFST-22,sp),x
3126  0121 1e17          	ldw	x,(OFST-2,sp)
3127  0123 1c0001        	addw	x,#1
3128  0126 1f17          	ldw	(OFST-2,sp),x
3129  0128 72fb03        	addw	x,(OFST-22,sp)
3130  012b f7            	ld	(x),a
3131                     ; 72         d/=r;
3133  012c 7b22          	ld	a,(OFST+9,sp)
3134  012e b703          	ld	c_lreg+3,a
3135  0130 3f02          	clr	c_lreg+2
3136  0132 3f01          	clr	c_lreg+1
3137  0134 3f00          	clr	c_lreg
3138  0136 96            	ldw	x,sp
3139  0137 1c0001        	addw	x,#OFST-24
3140  013a cd0000        	call	c_rtol
3142  013d 96            	ldw	x,sp
3143  013e 1c001e        	addw	x,#OFST+5
3144  0141 cd0000        	call	c_ltor
3146  0144 96            	ldw	x,sp
3147  0145 1c0001        	addw	x,#OFST-24
3148  0148 cd0000        	call	c_ldiv
3150  014b 96            	ldw	x,sp
3151  014c 1c001e        	addw	x,#OFST+5
3152  014f cd0000        	call	c_rtol
3154  0152               L3012:
3155                     ; 69     while(d){
3157  0152 96            	ldw	x,sp
3158  0153 1c001e        	addw	x,#OFST+5
3159  0156 cd0000        	call	c_lzmp
3161  0159 268d          	jrne	L1012
3163  015b 2022          	jra	L1112
3164  015d               L7012:
3165                     ; 76         dest[i++]=dest_temp[j--];
3167  015d 96            	ldw	x,sp
3168  015e 1c0005        	addw	x,#OFST-20
3169  0161 1f03          	ldw	(OFST-22,sp),x
3170  0163 1e17          	ldw	x,(OFST-2,sp)
3171  0165 1d0001        	subw	x,#1
3172  0168 1f17          	ldw	(OFST-2,sp),x
3173  016a 1c0001        	addw	x,#1
3174  016d 72fb03        	addw	x,(OFST-22,sp)
3175  0170 f6            	ld	a,(x)
3176  0171 1e15          	ldw	x,(OFST-4,sp)
3177  0173 1c0001        	addw	x,#1
3178  0176 1f15          	ldw	(OFST-4,sp),x
3179  0178 1d0001        	subw	x,#1
3180  017b 72fb1a        	addw	x,(OFST+1,sp)
3181  017e f7            	ld	(x),a
3182  017f               L1112:
3183                     ; 74     while(j>=0)
3185  017f 9c            	rvf
3186  0180 1e17          	ldw	x,(OFST-2,sp)
3187  0182 2ed9          	jrsge	L7012
3188                     ; 78     dest[i]='\0';
3190  0184 1e15          	ldw	x,(OFST-4,sp)
3191  0186 72fb1a        	addw	x,(OFST+1,sp)
3192  0189 7f            	clr	(x)
3193                     ; 79     return i;
3195  018a 7b16          	ld	a,(OFST-3,sp)
3198  018c 5b1b          	addw	sp,#27
3199  018e 81            	ret
3202                     .const:	section	.text
3203  0000               L7112_res:
3204  0000 00            	dc.b	0
3205  0001 000000000000  	ds.b	15
3206  0010               L3212_res:
3207  0010 00            	dc.b	0
3208  0011 000000000000  	ds.b	15
3209  0020               L7212_res:
3210  0020 00            	dc.b	0
3211  0021 000000000000  	ds.b	15
3343                     ; 82 S16 myprintf(char *fmt, ...){ 
3344                     	switch	.text
3345  018f               _myprintf:
3347  018f 89            	pushw	x
3348  0190 5218          	subw	sp,#24
3349       00000018      OFST:	set	24
3352                     ; 83     const char *str = fmt;  
3354  0192 1f15          	ldw	(OFST-3,sp),x
3355                     ; 85     va_start(args, fmt);  
3357  0194 96            	ldw	x,sp
3358  0195 1c001d        	addw	x,#OFST+5
3359  0198 1f17          	ldw	(OFST-1,sp),x
3361  019a accc02cc      	jpf	L5222
3362  019e               L3222:
3363                     ; 88         if(*str != '%') {                       //遇到非'%'字符  
3365  019e 1e15          	ldw	x,(OFST-3,sp)
3366  01a0 f6            	ld	a,(x)
3367  01a1 a125          	cp	a,#37
3368  01a3 2711          	jreq	L1322
3369                     ; 89             putc(*str);  
3371  01a5 1e15          	ldw	x,(OFST-3,sp)
3372  01a7 f6            	ld	a,(x)
3373  01a8 cd0000        	call	_putc
3375                     ; 90             ++str;  
3377  01ab 1e15          	ldw	x,(OFST-3,sp)
3378  01ad 1c0001        	addw	x,#1
3379  01b0 1f15          	ldw	(OFST-3,sp),x
3380                     ; 91             continue;  
3382  01b2 accc02cc      	jpf	L5222
3383  01b6               L1322:
3384                     ; 93             ++str;  
3386  01b6 1e15          	ldw	x,(OFST-3,sp)
3387  01b8 1c0001        	addw	x,#1
3388  01bb 1f15          	ldw	(OFST-3,sp),x
3389                     ; 94             switch(*str) {  
3391  01bd 1e15          	ldw	x,(OFST-3,sp)
3392  01bf f6            	ld	a,(x)
3394                     ; 128                     break;  
3395  01c0 a063          	sub	a,#99
3396  01c2 2603          	jrne	L62
3397  01c4 cc029e        	jp	L1312
3398  01c7               L62:
3399  01c7 4a            	dec	a
3400  01c8 2760          	jreq	L1212
3401  01ca a008          	sub	a,#8
3402  01cc 2712          	jreq	L5112
3403  01ce a007          	sub	a,#7
3404  01d0 2603          	jrne	L03
3405  01d2 cc02b2        	jp	L3312
3406  01d5               L03:
3407  01d5 a005          	sub	a,#5
3408  01d7 2603          	jrne	L23
3409  01d9 cc0264        	jp	L5212
3410  01dc               L23:
3411  01dc acc502c5      	jpf	L7322
3412  01e0               L5112:
3413                     ; 96                     if(*(str+1)=='d'){
3415  01e0 1e15          	ldw	x,(OFST-3,sp)
3416  01e2 e601          	ld	a,(1,x)
3417  01e4 a164          	cp	a,#100
3418  01e6 2603          	jrne	L43
3419  01e8 cc02c5        	jp	L7322
3420  01eb               L43:
3421                     ; 97                         break;
3423                     ; 99                         long val = va_arg(args, long);  
3425  01eb 1e17          	ldw	x,(OFST-1,sp)
3426  01ed 1c0004        	addw	x,#4
3427  01f0 1f17          	ldw	(OFST-1,sp),x
3428  01f2 1e17          	ldw	x,(OFST-1,sp)
3429  01f4 1d0004        	subw	x,#4
3430  01f7 9093          	ldw	y,x
3431  01f9 ee02          	ldw	x,(2,x)
3432  01fb 1f03          	ldw	(OFST-21,sp),x
3433  01fd 93            	ldw	x,y
3434  01fe fe            	ldw	x,(x)
3435  01ff 1f01          	ldw	(OFST-23,sp),x
3436                     ; 100                         char res[16] = {0};  
3438  0201 96            	ldw	x,sp
3439  0202 1c0005        	addw	x,#OFST-19
3440  0205 90ae0000      	ldw	y,#L7112_res
3441  0209 a610          	ld	a,#16
3442  020b cd0000        	call	c_xymvx
3444                     ; 101                         myitoa(res,val, 10);  
3446  020e 4b0a          	push	#10
3447  0210 1e04          	ldw	x,(OFST-20,sp)
3448  0212 89            	pushw	x
3449  0213 1e04          	ldw	x,(OFST-20,sp)
3450  0215 89            	pushw	x
3451  0216 96            	ldw	x,sp
3452  0217 1c000a        	addw	x,#OFST-14
3453  021a cd009e        	call	_myitoa
3455  021d 5b05          	addw	sp,#5
3456                     ; 102                         puts(res); 
3458  021f 96            	ldw	x,sp
3459  0220 1c0005        	addw	x,#OFST-19
3460  0223 cd000d        	call	_puts
3462  0226 acc502c5      	jpf	L7322
3463  022a               L1212:
3464                     ; 107                     int val = va_arg(args, int);  
3466  022a 1e17          	ldw	x,(OFST-1,sp)
3467  022c 1c0002        	addw	x,#2
3468  022f 1f17          	ldw	(OFST-1,sp),x
3469  0231 1e17          	ldw	x,(OFST-1,sp)
3470  0233 5a            	decw	x
3471  0234 5a            	decw	x
3472  0235 fe            	ldw	x,(x)
3473  0236 1f03          	ldw	(OFST-21,sp),x
3474                     ; 108                     char res[16] = {0};  
3476  0238 96            	ldw	x,sp
3477  0239 1c0005        	addw	x,#OFST-19
3478  023c 90ae0010      	ldw	y,#L3212_res
3479  0240 a610          	ld	a,#16
3480  0242 cd0000        	call	c_xymvx
3482                     ; 109                     myitoa(res,val, 10);  
3484  0245 4b0a          	push	#10
3485  0247 1e04          	ldw	x,(OFST-20,sp)
3486  0249 cd0000        	call	c_itolx
3488  024c be02          	ldw	x,c_lreg+2
3489  024e 89            	pushw	x
3490  024f be00          	ldw	x,c_lreg
3491  0251 89            	pushw	x
3492  0252 96            	ldw	x,sp
3493  0253 1c000a        	addw	x,#OFST-14
3494  0256 cd009e        	call	_myitoa
3496  0259 5b05          	addw	sp,#5
3497                     ; 110                     puts(res);  
3499  025b 96            	ldw	x,sp
3500  025c 1c0005        	addw	x,#OFST-19
3501  025f cd000d        	call	_puts
3503                     ; 111                     break;  
3505  0262 2061          	jra	L7322
3506  0264               L5212:
3507                     ; 114                     int val = va_arg(args, int);  
3509  0264 1e17          	ldw	x,(OFST-1,sp)
3510  0266 1c0002        	addw	x,#2
3511  0269 1f17          	ldw	(OFST-1,sp),x
3512  026b 1e17          	ldw	x,(OFST-1,sp)
3513  026d 5a            	decw	x
3514  026e 5a            	decw	x
3515  026f fe            	ldw	x,(x)
3516  0270 1f03          	ldw	(OFST-21,sp),x
3517                     ; 115                     char res[16] = {0};  
3519  0272 96            	ldw	x,sp
3520  0273 1c0005        	addw	x,#OFST-19
3521  0276 90ae0020      	ldw	y,#L7212_res
3522  027a a610          	ld	a,#16
3523  027c cd0000        	call	c_xymvx
3525                     ; 116                     myitoa(res,val, 16);  
3527  027f 4b10          	push	#16
3528  0281 1e04          	ldw	x,(OFST-20,sp)
3529  0283 cd0000        	call	c_itolx
3531  0286 be02          	ldw	x,c_lreg+2
3532  0288 89            	pushw	x
3533  0289 be00          	ldw	x,c_lreg
3534  028b 89            	pushw	x
3535  028c 96            	ldw	x,sp
3536  028d 1c000a        	addw	x,#OFST-14
3537  0290 cd009e        	call	_myitoa
3539  0293 5b05          	addw	sp,#5
3540                     ; 117                     puts(res);  
3542  0295 96            	ldw	x,sp
3543  0296 1c0005        	addw	x,#OFST-19
3544  0299 cd000d        	call	_puts
3546                     ; 118                     break;  
3548  029c 2027          	jra	L7322
3549  029e               L1312:
3550                     ; 121                     char ch = va_arg(args, char);  
3552  029e 1e17          	ldw	x,(OFST-1,sp)
3553  02a0 1c0001        	addw	x,#1
3554  02a3 1f17          	ldw	(OFST-1,sp),x
3555  02a5 1e17          	ldw	x,(OFST-1,sp)
3556  02a7 5a            	decw	x
3557  02a8 f6            	ld	a,(x)
3558  02a9 6b14          	ld	(OFST-4,sp),a
3559                     ; 122                     putc(ch);  
3561  02ab 7b14          	ld	a,(OFST-4,sp)
3562  02ad cd0000        	call	_putc
3564                     ; 123                     break;  
3566  02b0 2013          	jra	L7322
3567  02b2               L3312:
3568                     ; 126                     char *res = va_arg(args, char*);  
3570  02b2 1e17          	ldw	x,(OFST-1,sp)
3571  02b4 1c0002        	addw	x,#2
3572  02b7 1f17          	ldw	(OFST-1,sp),x
3573  02b9 1e17          	ldw	x,(OFST-1,sp)
3574  02bb 5a            	decw	x
3575  02bc 5a            	decw	x
3576  02bd fe            	ldw	x,(x)
3577  02be 1f13          	ldw	(OFST-5,sp),x
3578                     ; 127                     puts(res);  
3580  02c0 1e13          	ldw	x,(OFST-5,sp)
3581  02c2 cd000d        	call	_puts
3583                     ; 128                     break;  
3585  02c5               L7322:
3586                     ; 131             ++str;  
3588  02c5 1e15          	ldw	x,(OFST-3,sp)
3589  02c7 1c0001        	addw	x,#1
3590  02ca 1f15          	ldw	(OFST-3,sp),x
3591  02cc               L5222:
3592                     ; 87     while(*str != '\0') {                       //循环遍历格式字符串  
3594  02cc 1e15          	ldw	x,(OFST-3,sp)
3595  02ce 7d            	tnz	(x)
3596  02cf 2703          	jreq	L63
3597  02d1 cc019e        	jp	L3222
3598  02d4               L63:
3599                     ; 134     va_end(args);  
3601                     ; 135     return 0;  
3603  02d4 5f            	clrw	x
3606  02d5 5b1a          	addw	sp,#26
3607  02d7 81            	ret
3650                     ; 138 void delay(U16 y)
3650                     ; 139 {
3651                     	switch	.text
3652  02d8               _delay:
3654  02d8 89            	pushw	x
3655  02d9 89            	pushw	x
3656       00000002      OFST:	set	2
3659                     ; 141 	for(x=0;x<y;x++);
3661  02da 5f            	clrw	x
3662  02db 1f01          	ldw	(OFST-1,sp),x
3664  02dd 2007          	jra	L3722
3665  02df               L7622:
3669  02df 1e01          	ldw	x,(OFST-1,sp)
3670  02e1 1c0001        	addw	x,#1
3671  02e4 1f01          	ldw	(OFST-1,sp),x
3672  02e6               L3722:
3675  02e6 1e01          	ldw	x,(OFST-1,sp)
3676  02e8 1303          	cpw	x,(OFST+1,sp)
3677  02ea 25f3          	jrult	L7622
3678                     ; 142 }
3681  02ec 5b04          	addw	sp,#4
3682  02ee 81            	ret
3695                     	xdef	_mystrcmp
3696                     	xdef	_myitoa
3697                     	xdef	_mystrcat
3698                     	xref	_sendData
3699                     	xdef	_myprintf
3700                     	xdef	_puts
3701                     	xdef	_putc
3702                     	xdef	_delay
3703                     	xref.b	c_lreg
3704                     	xref.b	c_x
3723                     	xref	c_itolx
3724                     	xref	c_xymvx
3725                     	xref	c_ldiv
3726                     	xref	c_lmod
3727                     	xref	c_rtol
3728                     	xref	c_ltor
3729                     	xref	c_lgneg
3730                     	xref	c_lzmp
3731                     	end
