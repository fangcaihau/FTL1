
#ifndef __HOST__
#define __HOST__

#include "init.h"

#define	IDE_CMD_DEV_READ	1
#define	IDE_CMD_DEV_WRITE	0


typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

struct hostReq{
	u64 lpn;
	u64 buffer_address;
	u16 count;
	u16 op;

	struct hostReq *next;
};


struct subReq{
	u64 lpn;
	u64 buffer_address;
	u16 count;
	u16 op;

	struct subReq *next;
};

struct hostReq *CreatSubRequest(u64 lpn,u64 buffer_address,u16 op);

struct hostReq *TakeReq(struct ftlInfo *ftl);

struct bufferGroup *TranBuffer2Req(struct bufferGroup *);

struct devCmd *TranReq2Cmd(struct ftlInfo *ftl,struct hostReq *sub_request);
#endif
