#ifdef DEBUG

#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <stdbool.h>
#ifndef _HAS_MEMMEM
#include "memmem.h"
#endif

#include <stdlib.h>
#include "memory-manager.h"

#define MEM_MNG_PREAMBLE	0xDEADBEEFBAADBEEF
#define DEFAULT_BUFFER_SIZE	(1<<18)
#define PREV_ARRAY_SIZE		1025

#pragma pack(push)
#pragma pack(1)

struct block_header{
	uint64_t preamble;
	uint32_t size;
};
typedef struct block_header block_header;

struct buffer_header{
	void* next_buffer_start_ptr;
	void* this_buffer_data_end_ptr;
	int this_buffer_size; /*<- Pure size, without this header*/
	int size_remain;
};
typedef struct buffer_header buffer_header;

#pragma pack(pop)

static buffer_header* start_buffer=NULL;
static void* prev_array[PREV_ARRAY_SIZE]; /*<- Raw pointers. They point at bh, not data*/
static MEM_MNG_ERROR n_access_status;

MEM_MNG_ERROR n_errno(){
	return n_access_status;
}

static void prev_init(){
	memset(&prev_array, 0, sizeof(prev_array));
}

static MEM_MNG_ERROR prev_allocation(void** dataPtr, uint32_t size){

	if (size<PREV_ARRAY_SIZE){
		if (NULL==prev_array[size])
			return MEM_MNG_ERROR_ALLOCATION_IMPOSSIBLE;
		void *res=prev_array[size];
		prev_array[size]=NULL;
		block_header *bh = (block_header*)res;
		bh->preamble=MEM_MNG_PREAMBLE;
		bh->size=size;
		*dataPtr = (void*)(bh+1);
		return MEM_MNG_ERROR_OK;
	}
	return MEM_MNG_ERROR_ALLOCATION_IMPOSSIBLE;
}

static MEM_MNG_ERROR prev_freeing(void* data){
	block_header *bh = ((block_header*)data)-1;
	int size=bh->size;
	if (0==(size-1&size)){ /*power of 2*/
		uint32_t s=size, i;
		for (i=0; i<10; i++){
			s=s>>1;
			if (0==s)
				break;
		}
		if (i<10){
			if (NULL!=prev_array[i])
				return MEM_MNG_OTHER_INTERNAL_ERROR;
			prev_array[i]=bh;
			return MEM_MNG_ERROR_OK;
		}
	}
	return MEM_MNG_OTHER_INTERNAL_ERROR;
}

static MEM_MNG_ERROR recursive_alloc(buffer_header* buffer, void** dataPtr, uint32_t size){
	if (size+sizeof(block_header)<buffer->size_remain){
		void* res = buffer->this_buffer_data_end_ptr;
		block_header *bh = (block_header*)res;
		bh->preamble=MEM_MNG_PREAMBLE;
		bh->size=size;
		*dataPtr = (void*)(bh+1);
		buffer->this_buffer_data_end_ptr=(uint8_t*)buffer->this_buffer_data_end_ptr+size+sizeof(block_header);
		buffer->size_remain-=size+sizeof(block_header);
		return MEM_MNG_ERROR_OK;
	}
	else{
		if (NULL==buffer->next_buffer_start_ptr){
			int s = DEFAULT_BUFFER_SIZE+sizeof(buffer_header);
			if (size>DEFAULT_BUFFER_SIZE)
				s=size;
			if ((buffer->next_buffer_start_ptr=(buffer_header*)malloc(s))==NULL)
				return MEM_MNG_ERROR_INTERNAL_ALLOCATION_ERROR;
			buffer_header* next_bh=buffer->next_buffer_start_ptr;
			next_bh->next_buffer_start_ptr=NULL;
			next_bh->this_buffer_size=DEFAULT_BUFFER_SIZE;
			next_bh->this_buffer_data_end_ptr = (uint8_t*)buffer+sizeof(buffer_header);
			next_bh->size_remain=DEFAULT_BUFFER_SIZE;
			recursive_alloc(next_bh, dataPtr, size);
		}
		else{
			buffer_header* next_bh=buffer->next_buffer_start_ptr;
			recursive_alloc(next_bh, dataPtr, size);
		}
	}
	return MEM_MNG_ERROR_OK;
}

MEM_MNG_ERROR n_alloc(void** dataPtr, uint32_t size){

	if (0==size)
		return MEM_MNG_ERROR_ZERO_MEM_ALLOCATED;

	if (NULL==start_buffer){
		int s = DEFAULT_BUFFER_SIZE+sizeof(buffer_header);
		if ((start_buffer=(buffer_header*)malloc(s))==NULL)
			return MEM_MNG_ERROR_INTERNAL_ALLOCATION_ERROR;
		start_buffer->next_buffer_start_ptr=NULL;
		start_buffer->this_buffer_size=DEFAULT_BUFFER_SIZE;
		start_buffer->this_buffer_data_end_ptr = (uint8_t*)start_buffer+sizeof(buffer_header);
		start_buffer->size_remain=DEFAULT_BUFFER_SIZE;
		prev_init();
	}

	if (MEM_MNG_ERROR_OK == prev_allocation(dataPtr, size))
		return MEM_MNG_ERROR_OK;

	return recursive_alloc(start_buffer, dataPtr, size);
}

MEM_MNG_ERROR n_free(void* data){
	if (NULL==data)
		return MEM_MNG_ERROR_IS_NULL;
	block_header *bh = ((block_header*)data)-1;
	if (MEM_MNG_PREAMBLE!=bh->preamble)
		return MEM_MNG_ERROR_BLOCK_NOT_ALLOCATED;
	bh->preamble=0;
	prev_freeing(data);

	return MEM_MNG_ERROR_OK;
}

void* n_access_ptr(void* object, void* accessed_data){
	if (NULL==object||NULL==accessed_data)
		n_access_status = MEM_MNG_ERROR_IS_NULL;

	block_header *bh = ((block_header*)object)-1;
	if (MEM_MNG_PREAMBLE!=bh->preamble){
		n_access_status = MEM_MNG_ERROR_BLOCK_NOT_ALLOCATED;
		return NULL;
	}
	if (((uint8_t*)accessed_data-(uint8_t*)object)>=bh->size){
		n_access_status = MEM_MNG_ERROR_OUT_OF_RANGE;
		return NULL;
	}
	if (((uint8_t*)accessed_data-(uint8_t*)object)<0){
		n_access_status = MEM_MNG_ERROR_OUT_OF_RANGE;
		return NULL;
	}
	return accessed_data;
}

static MEM_MNG_ERROR recursive_free_all(buffer_header* buffer){
	if (buffer->next_buffer_start_ptr!=NULL)
		recursive_free_all(buffer->next_buffer_start_ptr);
	void* res;
	int64_t preamble=MEM_MNG_PREAMBLE;
	res=memmem(buffer, buffer->this_buffer_size, &preamble, sizeof(preamble));
	while (res!=NULL){
		printf("Some data leakage at address %llx\n", (uint64_t)res+sizeof(block_header));
		int further_length=buffer->this_buffer_size-((uint8_t*)res-(uint8_t*)buffer);
		res=memmem((uint8_t*)res+sizeof(preamble), further_length, &preamble, sizeof(preamble));
		/*If you found first mem leakage you still want to test for others */
	}
	free(buffer);
	return MEM_MNG_ERROR_OK;
}

MEM_MNG_ERROR n_free_all(){
	return recursive_free_all(start_buffer);
}

#else

#include <stdlib.h>
#include "memory-manager.h"

MEM_MNG_ERROR n_alloc(void** dataPtr, uint32_t size){
	*dataPtr = malloc(size);
	return MEM_MNG_ERROR_OK;
}

void* n_access_ptr(void* object, void* accessed_data){
	return accessed_data;
}

MEM_MNG_ERROR n_free(void* data){
	free(data);
	return MEM_MNG_ERROR_OK;
}

MEM_MNG_ERROR n_free_all(){
	return MEM_MNG_ERROR_OK;
}

#endif
