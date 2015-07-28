#ifndef __INIT__
#define __INIT__

//#include "xil_types.h"
//#include "xil_assert.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "host.h"

#define XST_SUCCESS 1
#define XST_FAIL 0
#define BIT_PER_CHAR 8

#define BUFSIZE 200 

/*#define	LBA_SIZE	512
#define	SUBPAGE_PER_PAGE         4
 /* Definitions for NAND Flash 
/*page
#define	PAGE_SIZE	0x800
//#define	PAGE_SHIFT	 11
#define	PAGE_PER_BLOCK		0x40
//#define	PAGE_PER_BLOCK_SHIFT	6

/*block
#define	BLOCK_SIZE		(PAGE_SIZE*PAGE_PER_BLOCK	)
#define	BLOCK_PER_PLANE		0x800

#define	PAGE_PER_PLANE		(PAGE_PER_BLOCK*BLOCK_PER_PLANE)
#define	PAGE_PER_PLANE_SHIFT	17

#define	PLANE_SIZE		(PAGE_SIZE*PAGE_PER_PLANE)

#define	PLANE_PER_DIE		0x04

#define	BLOCK_PER_DIE		(BLOCK_PER_PLANE*PLANE_PER_DIE)

#define	PAGE_PER_DIE	(PAGE_PER_BLOCK*BLOCK_PER_DIE)

#define	PAGE_PER_DIE_SHIFT	19

#define	DIE_SIZE		(PLANE_SIZE*PLANE_PER_DIE)

#define	DIE_PER_CHIP		0x01

#define	PLANE_PER_CHIP		(PLANE_PER_DIE*DIE_PER_CHIP)

#define	BLOCK_PER_CHIP		(BLOCK_PER_PLANE*PLANE_PER_CHIP)

#define	PAGE_PER_CHIP	 (PAGE_PER_BLOCK*BLOCK_PER_CHIP)
#define	PAGE_PER_CHIP_SHIFT	 19

#define	CHIP_SIZE	(DIE_SIZE*	DIE_PER_CHIP)

#define	CHIP_PER_CHANNEL		0x01

#define	DIE_PER_CHANNEL		(DIE_PER_CHIP*CHIP_PER_CHANNEL)

#define	PLANE_PER_CHANNEL		(PLANE_PER_DIE*DIE_PER_CHANNEL)

#define	BLOCK_PER_CHANNEL		(BLOCK_PER_PLANE*PLANE_PER_CHANNEL)

#define	PAGE_PER_CHANNEL			(PAGE_PER_BLOCK*BLOCK_PER_CHANNEL)
#define	PAGE_PER_CHANNEL_SHIFT	 19

#define	CHANNEL_SIZE	(CHIP_SIZE*CHIP_PER_CHANNEL)

#define	CHANNEL_PER_DEVICE	XPAR_AXI_NFC_NUM_INSTANCES

#define	CHIP_PER_DEVICE		(CHIP_PER_CHANNEL*CHANNEL_PER_DEVICE)

#define	DIE_PER_DEVICE		(DIE_PER_CHIP*CHIP_PER_DEVICE)

#define	PLANE_PER_DEVICE		(PLANE_PER_DIE*DIE_PER_DEVICE)

#define	BLOCK_PER_DEVICE		(BLOCK_PER_PLANE*PLANE_PER_DEVICE)

#define	PAGE_PER_DEVICE		(PAGE_PER_BLOCK*BLOCK_PER_DEVICE)
#define	PAGE_PER_DEVICE_SHIFT	19

#define	DEVICE_CAPACITY	(PAGE_SIZE*PAGE_PER_DEVICE)*/
 
struct ftlInfo{

	struct channelInfo *channel;
	struct dramInfo *dram;
	struct parameterValue *parameter;
	struct hostReqInfo *request_info;
	struct subReqInfo **sub_request_info;

	struct pageBitmap *page_bit_map;
	struct blockBitmap *block_bit_map;
};

struct channelInfo{
	u8 token;
	u8 chip_num;	
	u8 current_state; 
	u8 next_state;

	struct chipInfo *chip;  
};

struct chipInfo{
	u8 token;		/*Up to 256 chips per channel*/
	u8 die_num;	/*reserve space*/
	u8 current_state; 
	u8 next_state;

	struct dieInfo *die;
	unsigned long read_count;	//how many read count in the process of workload
	unsigned long program_count;
	unsigned long erase_count;
};

struct dieInfo{
	unsigned int token;
	struct planeInfo *plane;
};

struct planeInfo{
	unsigned int token;
	unsigned int free_page; 
	unsigned int active_block; 
	unsigned int erase_block; 
};

struct mapEntry{
	int *data_map;

};

struct metadataMap{
	int metadata_map_size;		//映射表占的页数
	int *metadata_map;			//映射表所在ppn
};

struct dramInfo{
	unsigned int dram_capacity; 
	struct mapInfo *map;			
	struct bufferInfo **buffer; 
};

struct mapInfo{
/*	unsigned long read_hit;                      /*这里的hit都表示sector的命中次数或是没命中的次数
	unsigned long read_miss_hit;  
	unsigned long write_hit;   
	unsigned long write_miss_hit;
*/

	struct mapGroup *group_head;            /*as LRU head which is most recently used*/
	struct mapGroup *group_tail;            /*as LRU tail which is least recently used*/
	unsigned int max_page_count;				
	unsigned int free_page_count;	
};

struct mapGroup{
	struct bufferGroup *LRU_link_next;	// next node in LRU list
	struct bufferGroup *LRU_link_pre;	// previous node in LRU list
	
	u64 buffer_address;	
	u32 index;
	u8 is_dirty;
	u8 reserve_1;/*保留字段*/
	u16 reserve_2;
	
/*	u8 is_dirty;
	u8 reserve_1;/*保留字段
	u16 reserve_2;
	u32 reserve_3;
*/	
};

struct bufferInfo{
/*	unsigned long read_hit;                      /*这里的hit都表示sector的命中次数或是没命中的次数
	unsigned long read_miss_hit;  
	unsigned long write_hit;   
	unsigned long write_miss_hit;
*/
	struct bufferGroup *buffer_head;            /*as LRU head which is most recently used*/
	struct bufferGroup *buffer_tail;            /*as LRU tail which is least recently used*/

	unsigned int max_page_count;				
	unsigned int free_page_count;			
};

struct bufferGroup{
	struct bufferGroup *LRU_link_next;	// next node in LRU list
	struct bufferGroup *LRU_link_pre;	// previous node in LRU list
	u64 buffer_address;	
	u64 lpn;
	
	
/*	u8 is_dirty;
	u8 reserve_1;/*保留字段
	u16 reserve_2;
	u32 reserve_3;
*/	
};


struct pageBitmap{
	char *Is_alloc;
	char *Is_invalid ;
};

struct blockBitmap{
	char *Is_bad_block;
	char *Is_dirty_block;

	char *invalid_page_num;
	char *active_page_num;
	unsigned int *erase_count;
};

struct parameterValue{
	unsigned int chip_num;          //记录一个SSD中有多少个颗粒
	unsigned int dram_capacity;     //记录SSD中DRAM capacity
	unsigned int channel_number;    //记录SSD中有多少个通道，每个通道是单独的bus
	unsigned int chip_channel; //设置SSD中channel数和每channel上颗粒的数量
	unsigned int die_chip;    
	unsigned int plane_die;
	unsigned int block_plane;
	unsigned int page_block;
	unsigned int page_capacity;
	unsigned int buffer_ratio;
	unsigned int map_ratio;
	
	float overprovide;				//SSD会预留空间作垃圾回收
	float gc_threshold;             //当达到这个阈值时，开始GC操作，在主动写策略中，开始GC操作后可以临时中断GC操作，服务新到的请求；在普通策略中，GC不可中断
	float gc_hard_threshold;        //普通策略中用不到该参数，只有在主动写策略中，当满足这个阈值时，GC操作不可中断
	int allocation_scheme;          //记录分配方式的选择，0表示动态分配，1表示静态分配
	int static_allocation;          //记录是那种静态分配方式，如ICS09那篇文章所述的所有静态分配方式
	int dynamic_allocation;         //记录动态分配的方式
	int advanced_commands;  
	int queue_length;               //请求队列的长度限制

};

struct hostReqInfo{
	struct hostReq *reqHead;
	struct hostReq *reqTail;
};

struct subReqInfo{
	struct subReq *subreqHead;
	struct subReq *subreqTail;
};


struct ftlInfo *Initiation(struct ftlInfo *ftl);
void AllocAssert(void *p,char *s);



#endif

















