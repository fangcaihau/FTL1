#include "init.h"


void AllocAssert(void *p,char *s)//断言
{
	if(p!=NULL) {
		//printf("malloc %s successful!\n",s);
		return;
		}
	printf("malloc %s error\n",s);
	//getchar();
	system("pause");
	exit(-1);
}


struct ftlInfo *Initiation(struct ftlInfo *ftl){

	//struct parameter_value *parameters;

	printf("initiation start\n");

	LoadParameters(ftl);	
	InitChannel(ftl);
	InitDram(ftl);
	InitControl(ftl);
		
	return ftl;
}

int LoadParameters(struct ftlInfo *ftl){
	FILE * fp,*ferr;
	char buf[200];
	int i;
	int pre_eql,next_eql;
	int res_eql;
	char *ptr;

	ftl->parameter = (struct parameterValue *)malloc(sizeof(struct parameterValue));
	AllocAssert(ftl->parameter ,"parameterValue");
	memset(ftl->parameter ,0,sizeof(struct parameterValue));

	if((ferr = fopen_s(&fp,"parameter_file.p","r"))!= 0)
	{	
		printf("the file parameter_file error!\n");	
		return XST_FAIL;
	}
	
	printf("the file parameter_file is  true !\n");
	
	while(fgets(buf,200,fp)){
	if(buf[0] =='#' || buf[0] == ' ') continue;
	ptr=strchr(buf,'=');
	if(!ptr) continue; 
	
	pre_eql = ptr - buf;
	next_eql = pre_eql + 1;
	while(buf[pre_eql-1] == ' ') pre_eql--;
	buf[pre_eql] = 0;
	if((res_eql=strcmp(buf,"chip number")) ==0){			
		sscanf(buf + next_eql,"%d",&ftl->parameter ->chip_num);
	}else if((res_eql=strcmp(buf,"dram capacity")) ==0){
		sscanf(buf + next_eql,"%d",&ftl->parameter ->dram_capacity);
	}else if((res_eql=strcmp(buf,"buffer ratio")) ==0){
		sscanf(buf + next_eql,"%d",&ftl->parameter ->buffer_ratio);
	}else if((res_eql=strcmp(buf,"map ratio")) ==0){
		sscanf(buf + next_eql,"%d",&ftl->parameter ->map_ratio);
	}else if((res_eql=strcmp(buf,"channel number")) ==0){
		sscanf(buf + next_eql,"%d",&ftl->parameter ->channel_number); 
	}else if((res_eql=strcmp(buf,"chip channel")) ==0){
		sscanf(buf + next_eql,"%d",&ftl->parameter ->chip_channel);
	}else if((res_eql=strcmp(buf,"die number")) ==0){
		sscanf(buf + next_eql,"%d",&ftl->parameter ->die_chip); 
	}else if((res_eql=strcmp(buf,"plane number")) ==0){
		sscanf(buf + next_eql,"%d",&ftl->parameter ->plane_die); 
	}else if((res_eql=strcmp(buf,"block number")) ==0){
		sscanf(buf + next_eql,"%d",&ftl->parameter ->block_plane); 
	}else if((res_eql=strcmp(buf,"page number")) ==0){
		sscanf(buf + next_eql,"%d",&ftl->parameter ->page_block); 
	}else if((res_eql=strcmp(buf,"page capacity")) ==0){
		sscanf(buf + next_eql,"%d",&ftl->parameter ->page_capacity); 
	}else if((res_eql=strcmp(buf,"overprovide")) ==0){
		sscanf(buf + next_eql,"%f",&ftl->parameter ->overprovide); 
	}else if((res_eql=strcmp(buf,"gc threshold")) ==0){
		sscanf(buf + next_eql,"%f",&ftl->parameter ->gc_threshold); 
	}else if((res_eql=strcmp(buf,"gc hard threshold")) ==0){
		sscanf(buf + next_eql,"%f",&ftl->parameter ->gc_hard_threshold); 
	}else if((res_eql=strcmp(buf,"allocation")) ==0){
		sscanf(buf + next_eql,"%d",&ftl->parameter ->allocation_scheme); 
	}else if((res_eql=strcmp(buf,"static_allocation")) ==0){
		sscanf(buf + next_eql,"%d",&ftl->parameter ->static_allocation); 
	}else if((res_eql=strcmp(buf,"dynamic_allocation")) ==0){
		sscanf(buf + next_eql,"%d",&ftl->parameter ->dynamic_allocation); 
	}else if((res_eql=strcmp(buf,"queue_length")) ==0){
		sscanf(buf + next_eql,"%d",&ftl->parameter ->queue_length); 
	}else{
		printf("don't match\t %s\n",buf);
	}
		
	//memset(buf,0,BUFSIZE);
		
	}
	fclose(fp);
	return XST_SUCCESS;
}

int InitChannel(struct ftlInfo *ftl){
	unsigned int i,j;
	struct channelInfo * p_channel;
	struct chipInfo * p_chip;

	ftl->channel=(struct channelInfo*)malloc(ftl->parameter->channel_number * sizeof(struct channelInfo));
	AllocAssert(ftl->channel,"ftl->channel");
	memset(ftl->channel,0,ftl->parameter->channel_number * sizeof(struct channelInfo));
	
	// set the parameter of each channel
	for (i = 0; i< ftl->parameter->channel_number; i++)
	{	
		p_channel = &(ftl->channel[i]);
		p_channel->chip_num = ftl->parameter->chip_channel;
		p_channel->token = 0;
		
		p_channel->chip = (struct chipInfo *)malloc(ftl->parameter->chip_channel* sizeof(struct chipInfo));
		AllocAssert(p_channel->chip,"p_channel->chip");
		memset(p_channel->chip,0,ftl->parameter->chip_channel* sizeof(struct chipInfo));

		for (j = 0; j< ftl->parameter->chip_channel; j++)
		{
			p_chip = &(p_channel->chip[j]);
			InitChip(p_chip,ftl);
		}
	}

	return XST_SUCCESS;
}

int InitChip(struct chipInfo *p_chip,struct ftlInfo *ftl){
	unsigned int i;
	struct dieInfo *p_die;

	p_chip->die_num = ftl->parameter->die_chip;
	p_chip->token = 0;
	
	p_chip->die = (struct dieInfo *)malloc(ftl->parameter->die_chip * sizeof(struct dieInfo));
	AllocAssert(p_chip->die,"p_chip->die");
	memset(p_chip->die,0,ftl->parameter->die_chip * sizeof(struct dieInfo));

	for (i = 0; i<ftl->parameter->die_chip; i++)
	{
		p_die = &(p_chip->die[i]);
		InitDie( p_die,ftl);	
	}
	
	return XST_SUCCESS;
}

int InitDie(struct dieInfo * p_die,struct ftlInfo *ftl){
	unsigned int i;
	struct planeInfo * p_plane;

	p_die->token = 0;
	
	p_die->plane = (struct planeInfo*)malloc(ftl->parameter->plane_die * sizeof(struct planeInfo));
	AllocAssert(p_die->plane,"p_die->plane");
	memset(p_die->plane,0,ftl->parameter->plane_die * sizeof(struct planeInfo));

	for (i = 0; i<ftl->parameter->plane_die; i++)
	{
		p_plane = &(p_die->plane[i]);
		InitPlane(p_plane,ftl->parameter );
	}

	return XST_SUCCESS;

}

int InitPlane(struct planeInfo * p_plane,struct ftlInfo *ftl){
	p_plane->token = 0;
	p_plane->free_page = 0;
	p_plane->erase_block= 0;
	p_plane->active_block= 0;

	return XST_SUCCESS;
}

int InitDram(struct ftlInfo *ftl){

	ftl->dram = (struct dramInfo *)malloc(sizeof(struct dramInfo));
	AllocAssert(ftl->dram,"ftl->dram");
	memset(ftl->dram,0,sizeof(struct dramInfo));

	ftl->dram->dram_capacity =  ftl->parameter->dram_capacity;
	InitBuffer(ftl->dram,ftl);	
	InitMap(ftl->dram,ftl);

	return XST_SUCCESS;
}

int InitBuffer(struct dramInfo *dram,struct ftlInfo *ftl){
	unsigned int i;
	struct bufferInfo *p_buffer;
	
	dram->buffer = (struct bufferInfo **)malloc(ftl->parameter->channel_number*sizeof(struct bufferInfo *));
	AllocAssert(dram->buffer,"dram->buffer");
	memset(dram->buffer,0,ftl->parameter->channel_number*sizeof(struct bufferInfo *));

	for(i = 0;i < ftl->parameter->channel_number; ++i){
		dram->buffer[i] = (struct bufferInfo *)malloc(sizeof(struct bufferInfo));
		dram->buffer[i]->max_page_count = dram->dram_capacity*ftl->parameter->buffer_ratio/(ftl->parameter->page_capacity*ftl->parameter->channel_number);
		dram->buffer[i]->free_page_count = dram->buffer[i]->max_page_count ;
	}
	return XST_SUCCESS;
}

int InitMap(struct dramInfo *dram,struct ftlInfo *ftl){
	dram->map = (struct mapInfo *)malloc(sizeof(struct mapInfo));
	AllocAssert(dram->map,"dram->map");
	memset(dram->map,0,sizeof(struct mapInfo));
	
	dram->map->max_page_count = ftl->dram->dram_capacity*ftl->parameter->map_ratio/ftl->parameter->page_capacity;
	dram->map->free_page_count = ftl->dram->map->max_page_count ;
	return XST_SUCCESS;
}

/*初始化一些例如位图的控制信息*/
int InitControl(struct ftlInfo *ftl){

	InitReq();
	InitPageBitmap();
	InitBlockBitmap();
	return XST_SUCCESS;
}

 int InitReq(struct ftlInfo *ftl){
 	unsigned int i;

	ftl->request_info = (struct hostReqInfo *)malloc(sizeof(struct hostReqInfo));
	AllocAssert(ftl->request_info,"ftl->request");
	memset(ftl->request_info,0,sizeof(struct hostReqInfo));
	
	ftl->sub_request_info = (struct subReqInfo**)malloc(ftl->parameter->channel_number*sizeof(struct subReqInfo *));
	AllocAssert(ftl->sub_request_info,"ftl->sub_request");
	memset(ftl->sub_request_info,0,ftl->parameter->channel_number*sizeof(struct subReqInfo*));

	for(i = 0;i<ftl->parameter->channel_number;++i){
		ftl->sub_request_info[i] = (struct subReqInfo *)malloc(sizeof(struct subReqInfo));
		AllocAssert(ftl->sub_request_info[i],"ftl->sub_request_info[i]");
		memset(ftl->sub_request_info[i],0,sizeof(struct subReqInfo));
	}
	
	return XST_SUCCESS;
 }

 int InitPageBitmap(struct ftlInfo *ftl){
 	unsigned long int page_num;

	page_num = ftl->parameter->channel_number*ftl->parameter->chip_channel*ftl->parameter->die_chip*ftl->parameter->plane_die*\
				ftl->parameter->block_plane*ftl->parameter->page_block;
	
	ftl->page_bit_map = (struct pageBitmap *)malloc(sizeof(struct pageBitmap));
	AllocAssert(ftl->page_bit_map,"ftl->page_bit_map");
	memset(ftl->page_bit_map,0,sizeof(struct pageBitmap));

	ftl->page_bit_map->Is_alloc = (char *)malloc(page_num/8*sizeof(char));
	AllocAssert( ftl->page_bit_map->Is_alloc ," ftl->page_bit_map->Is_alloc ");
	memset(ftl->page_bit_map->Is_alloc ,0,page_num/BIT_PER_CHAR*sizeof(char));
	
	ftl->page_bit_map->Is_invalid = (char *)malloc(sizeof(char)*page_num/BIT_PER_CHAR);
	AllocAssert(ftl->page_bit_map->Is_invalid,"ftl->page_bit_map->Is_invalid");
	memset(ftl->page_bit_map->Is_invalid,0,sizeof(char)*page_num/BIT_PER_CHAR);

	return XST_SUCCESS;
	 
 }
 
  int InitBlockBitmap(struct ftlInfo *ftl){
	unsigned long int block_num;

	block_num = ftl->parameter->channel_number*ftl->parameter->chip_channel*ftl->parameter->die_chip*ftl->parameter->plane_die*\
				ftl->parameter->block_plane;
	
	ftl->block_bit_map = (struct blockBitmap *)malloc(sizeof(struct blockBitmap));
	AllocAssert(ftl->block_bit_map,"ftl->block_bit_map");
	memset(ftl->block_bit_map,0,sizeof(struct blockBitmap));	

	ftl->block_bit_map->Is_bad_block = (char *)malloc(sizeof(char)*block_num/BIT_PER_CHAR);
	AllocAssert(ftl->block_bit_map->Is_bad_block,"ftl->block_bit_map->Is_bad_block ");
	memset(ftl->block_bit_map->Is_bad_block,0,sizeof(char)*block_num/BIT_PER_CHAR);

	ftl->block_bit_map->Is_dirty_block =  (char *)malloc(sizeof(char)*block_num/BIT_PER_CHAR);
	AllocAssert(ftl->block_bit_map->Is_dirty_block ,"ftl->block_bit_map->Is_dirty_block ");
	memset(ftl->block_bit_map->Is_dirty_block ,0,sizeof(char)*block_num/BIT_PER_CHAR);

	ftl->block_bit_map->invalid_page_num = (char *)malloc(sizeof(char)*block_num);
	AllocAssert(ftl->block_bit_map->invalid_page_num ,"ftl->block_bit_map->invalid_page_num");
	memset(ftl->block_bit_map->invalid_page_num,0,sizeof(char)*block_num);

	ftl->block_bit_map->active_page_num = (char *)malloc(sizeof(char)*block_num);
	AllocAssert(ftl->block_bit_map->active_page_num ,"ftl->block_bit_map->active_page_num");
	memset(ftl->block_bit_map->active_page_num,0,sizeof(char)*block_num);

	ftl->block_bit_map->erase_count =  (char *)malloc(sizeof(char)*block_num);
	AllocAssert(ftl->block_bit_map->erase_count ,"ftl->block_bit_map->erase_count");
	memset(ftl->block_bit_map->erase_count,0,sizeof(char)*block_num);

	return XST_SUCCESS;
	 
 }








