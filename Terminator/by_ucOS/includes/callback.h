#ifndef _CALLBACK_
#define _CALLBACK_
typedef struct callback{
	uchar taskid;
	@far void (*CALL_BACK_FUN)(uchar arg);
}callbackfun;
#endif

