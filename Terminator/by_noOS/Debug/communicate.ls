   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Parser V4.10.2 - 02 Nov 2011
   3                     ; Generator (Limited) V4.3.7 - 29 Nov 2011
2578                     	bsct
2579  0000               _sys:
2580  0000 51            	dc.b	81
2581  0001 0009          	dc.w	9
2582  0003 42            	dc.b	66
2583  0004 42            	dc.b	66
2584  0005 00            	dc.b	0
2585  0006 000000        	ds.b	3
2750                     ; 42 U8 consultation_handler(void* bin)
2750                     ; 43 {
2752                     	switch	.text
2753  0000               _consultation_handler:
2755  0000 89            	pushw	x
2756       00000002      OFST:	set	2
2759                     ; 44 	CMD_PKG* cmd_ptr = (CMD_PKG*)bin;
2761  0001 1f01          	ldw	(OFST-1,sp),x
2762                     ; 45 	operate(cmd_ptr->dev_id,cmd_ptr->op_type,0);
2764  0003 5f            	clrw	x
2765  0004 89            	pushw	x
2766  0005 1e03          	ldw	x,(OFST+1,sp)
2767  0007 f6            	ld	a,(x)
2768  0008 97            	ld	xl,a
2769  0009 1603          	ldw	y,(OFST+1,sp)
2770  000b 90e601        	ld	a,(1,y)
2771  000e 95            	ld	xh,a
2772  000f cd0000        	call	_operate
2774  0012 85            	popw	x
2775                     ; 46 	return sizeof(CMD_PKG);			
2777  0013 a603          	ld	a,#3
2780  0015 85            	popw	x
2781  0016 81            	ret
2830                     ; 58 U8 binary_data_handler(void* bin)
2830                     ; 59 {
2831                     	switch	.text
2832  0017               _binary_data_handler:
2834  0017 89            	pushw	x
2835       00000002      OFST:	set	2
2838                     ; 60 	CMD_PKG* cmd_ptr = (CMD_PKG*)bin;
2840  0018 1f01          	ldw	(OFST-1,sp),x
2841                     ; 61 	operate(cmd_ptr->dev_id,cmd_ptr->op_type,0);
2843  001a 5f            	clrw	x
2844  001b 89            	pushw	x
2845  001c 1e03          	ldw	x,(OFST+1,sp)
2846  001e f6            	ld	a,(x)
2847  001f 97            	ld	xl,a
2848  0020 1603          	ldw	y,(OFST+1,sp)
2849  0022 90e601        	ld	a,(1,y)
2850  0025 95            	ld	xh,a
2851  0026 cd0000        	call	_operate
2853  0029 85            	popw	x
2854                     ; 62 	return sizeof(CMD_PKG);			
2856  002a a603          	ld	a,#3
2859  002c 85            	popw	x
2860  002d 81            	ret
2979                     ; 74 U8 recived_data_handler(RECV_DATA_T* r_data)
2979                     ; 75 {
2980                     	switch	.text
2981  002e               _recived_data_handler:
2983  002e 89            	pushw	x
2984  002f 89            	pushw	x
2985       00000002      OFST:	set	2
2988                     ; 76 	HeadForRecv* head = (HeadForRecv*)r_data->recv_buffer;
2990  0030 1f01          	ldw	(OFST-1,sp),x
2991                     ; 77 	if(head->magic1 == MAGIC1){
2993  0032 1e01          	ldw	x,(OFST-1,sp)
2994  0034 f6            	ld	a,(x)
2995  0035 a151          	cp	a,#81
2996  0037 2675          	jrne	L5602
2997                     ; 78 		if(head->pkg_len <= r_data->recved_size){
2999  0039 1e01          	ldw	x,(OFST-1,sp)
3000  003b 1603          	ldw	y,(OFST+1,sp)
3001  003d 90e620        	ld	a,(32,y)
3002  0040 905f          	clrw	y
3003  0042 9097          	ld	yl,a
3004  0044 90bf00        	ldw	c_y,y
3005  0047 9093          	ldw	y,x
3006  0049 90ee01        	ldw	y,(1,y)
3007  004c 90b300        	cpw	y,c_y
3008  004f 225a          	jrugt	L7602
3009                     ; 79 			if(head->magic2 == MAGIC2)
3011  0051 1e01          	ldw	x,(OFST-1,sp)
3012  0053 e603          	ld	a,(3,x)
3013  0055 a142          	cp	a,#66
3014  0057 264c          	jrne	L1702
3015                     ; 81 			    if(GetProgrameStae()==PRG_STATE_CONSULTATION)
3017  0059 cd0000        	call	_GetProgrameStae
3019  005c a101          	cp	a,#1
3020  005e 261e          	jrne	L3702
3021                     ; 83                     switch(head->content_type)
3023  0060 1e01          	ldw	x,(OFST-1,sp)
3024  0062 e604          	ld	a,(4,x)
3025  0064 a143          	cp	a,#67
3026  0066 260b          	jrne	L7771
3029  0068               L5771:
3030                     ; 85         				case 'C':
3030                     ; 86         					//Consultation
3030                     ; 87         					return consultation_handler(r_data->recv_buffer+sizeof(HeadForRecv))+sizeof(HeadForRecv);
3032  0068 1e03          	ldw	x,(OFST+1,sp)
3033  006a 1c0006        	addw	x,#6
3034  006d ad91          	call	_consultation_handler
3036  006f ab06          	add	a,#6
3038  0071 2008          	jra	L21
3039  0073               L7771:
3040                     ; 88         				default:
3040                     ; 89         					//Unsurported content type
3040                     ; 90         					DBG("Unsurported content type 0X%02X when consultatin!",head->content_type);
3042  0073 1e01          	ldw	x,(OFST-1,sp)
3043  0075 e604          	ld	a,(4,x)
3044                     ; 91         					return head->pkg_len;
3046  0077 1e01          	ldw	x,(OFST-1,sp)
3047  0079 e602          	ld	a,(2,x)
3049  007b               L21:
3051  007b 5b04          	addw	sp,#4
3052  007d 81            	ret
3053  007e               L3702:
3054                     ; 95     				switch(head->content_type)
3056  007e 1e01          	ldw	x,(OFST-1,sp)
3057  0080 e604          	ld	a,(4,x)
3059                     ; 107     					return head->pkg_len;
3060  0082 a042          	sub	a,#66
3061  0084 270d          	jreq	L1002
3062  0086 4a            	dec	a
3063  0087 2716          	jreq	L3002
3064  0089               L5002:
3065                     ; 104     				default:
3065                     ; 105     					//Unsurported content type
3065                     ; 106     					DBG("Unsurported content type 0X%02X!",head->content_type);
3067  0089 1e01          	ldw	x,(OFST-1,sp)
3068  008b e604          	ld	a,(4,x)
3069                     ; 107     					return head->pkg_len;
3071  008d 1e01          	ldw	x,(OFST-1,sp)
3072  008f e602          	ld	a,(2,x)
3074  0091 20e8          	jra	L21
3075  0093               L1002:
3076                     ; 97     				case 'B':
3076                     ; 98     					//handle
3076                     ; 99     					return binary_data_handler(r_data->recv_buffer+sizeof(HeadForRecv))+sizeof(HeadForRecv);
3078  0093 1e03          	ldw	x,(OFST+1,sp)
3079  0095 1c0006        	addw	x,#6
3080  0098 cd0017        	call	_binary_data_handler
3082  009b ab06          	add	a,#6
3084  009d 20dc          	jra	L21
3085  009f               L3002:
3086                     ; 100     				case 'C':
3086                     ; 101     					//Consultation
3086                     ; 102     					DBG("Unsurported content type 'C' when not consultatin!");
3088                     ; 103     					return head->pkg_len;
3090  009f 1e01          	ldw	x,(OFST-1,sp)
3091  00a1 e602          	ld	a,(2,x)
3093  00a3 20d6          	jra	L21
3094  00a5               L1702:
3095                     ; 112 				DBG("MAGIC2 is not right!!!!");
3097                     ; 113 				return head->pkg_len;
3099  00a5 1e01          	ldw	x,(OFST-1,sp)
3100  00a7 e602          	ld	a,(2,x)
3102  00a9 20d0          	jra	L21
3103  00ab               L7602:
3104                     ; 116 			return 0;
3106  00ab 4f            	clr	a
3108  00ac 20cd          	jra	L21
3109  00ae               L5602:
3110                     ; 118 	}else if(r_data->recved_size >= sizeof(HeadForRecv)){
3112  00ae 1e03          	ldw	x,(OFST+1,sp)
3113  00b0 e620          	ld	a,(32,x)
3114  00b2 a106          	cp	a,#6
3115  00b4 2504          	jrult	L5312
3116                     ; 119 		DBG("data error!!! Skip one byte!!");
3118                     ; 120 		return 1;//判定为出错，跳过一个字节
3120  00b6 a601          	ld	a,#1
3122  00b8 20c1          	jra	L21
3123  00ba               L5312:
3124                     ; 122 		return 0;
3126  00ba 4f            	clr	a
3128  00bb 20be          	jra	L21
3131                     .const:	section	.text
3132  0000               L1512_head:
3133  0000 51            	dc.b	81
3134  0001 0006          	dc.w	6
3135  0003 42            	dc.b	66
3136  0004 42            	dc.b	66
3137  0005 00            	dc.b	0
3199                     ; 133 void SendPkgData(S8 type,U8* data,U16 len)
3199                     ; 134 {
3200                     	switch	.text
3201  00bd               _SendPkgData:
3203  00bd 88            	push	a
3204  00be 5206          	subw	sp,#6
3205       00000006      OFST:	set	6
3208                     ; 135     HeadForSend head = {'Q',sizeof(head),'B','B',0};
3210  00c0 96            	ldw	x,sp
3211  00c1 1c0001        	addw	x,#OFST-5
3212  00c4 90ae0000      	ldw	y,#L1512_head
3213  00c8 a606          	ld	a,#6
3214  00ca cd0000        	call	c_xymvx
3216                     ; 136     head.pkg_len = sizeof(head) + len;
3218  00cd 1e0c          	ldw	x,(OFST+6,sp)
3219  00cf 1c0006        	addw	x,#6
3220  00d2 1f02          	ldw	(OFST-4,sp),x
3221                     ; 137     sendData((U8*)&head,sizeof(head));
3223  00d4 4b06          	push	#6
3224  00d6 96            	ldw	x,sp
3225  00d7 1c0002        	addw	x,#OFST-4
3226  00da cd0000        	call	_sendData
3228  00dd 84            	pop	a
3229                     ; 138     sendData(data,len);
3231  00de 7b0d          	ld	a,(OFST+7,sp)
3232  00e0 88            	push	a
3233  00e1 1e0b          	ldw	x,(OFST+5,sp)
3234  00e3 cd0000        	call	_sendData
3236  00e6 84            	pop	a
3237                     ; 139 }
3240  00e7 5b07          	addw	sp,#7
3241  00e9 81            	ret
3320                     	xdef	_binary_data_handler
3321                     	xdef	_consultation_handler
3322                     	xdef	_recived_data_handler
3323                     	xref	_GetProgrameStae
3324                     	xdef	_SendPkgData
3325                     	xdef	_sys
3326                     	xref	_sendData
3327                     	xref	_operate
3328                     	switch	.const
3329  0006               L7312:
3330  0006 646174612065  	dc.b	"data error!!! Skip"
3331  0018 206f6e652062  	dc.b	" one byte!!",0
3332  0024               L1212:
3333  0024 4d4147494332  	dc.b	"MAGIC2 is not righ"
3334  0036 742121212100  	dc.b	"t!!!!",0
3335  003c               L7012:
3336  003c 556e73757270  	dc.b	"Unsurported conten"
3337  004e 742074797065  	dc.b	"t type 'C' when no"
3338  0060 7420636f6e73  	dc.b	"t consultatin!",0
3339                     	xref.b	c_x
3340                     	xref.b	c_y
3360                     	xref	c_xymvx
3361                     	end
