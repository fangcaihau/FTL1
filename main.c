#include <stdio.h>
#include <stdlib.h>
#include "init.h"
#include "host.h"




int main(){
	u32 ppn;
	int i=3;
	struct hostReq *request;
	struct ftlInfo *ftl;

	ftl=(struct ftlInfo*)malloc(sizeof(struct ftlInfo));
	AllocAssert(ftl,"ftl");
	memset(ftl ,0,sizeof(struct ftlInfo));

	ftl = Initiation(ftl);
	
	while(i)
	{
		//GetHostReq(ftl);
	//	ReqHandle(ftl);
		printf("%dfchfchfch\n",i);
		i--;
		getchar();getchar();
	}
}




























