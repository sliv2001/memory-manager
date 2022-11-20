#include <stdlib.h>
#include <memory.h>
#include "memory-manager.h"

#define MEM_MNG_PREAMBLE 0xDEADBEEFBAADBEEF

#pragma pack(push)
#pragma pack(1)

struct block_header{
	uint64_t preamble;
	uint32_t size;
};
typedef struct block_header block_header;

#pragma pack(pop)

MEM_MNG_ERROR n_alloc(void** dataPtr, uint32_t size){

	if (0==size)
		return MEM_MNG_ERROR_ZERO_MEM_ALLOCATED;

	void* res = malloc(size+sizeof(block_header));
	if (NULL==res)
		return MEM_MNG_ERROR_ALLOCATION_IMPOSSIBLE;
	block_header *bh = (block_header*)res;
	bh->preamble=MEM_MNG_PREAMBLE;
	bh->size=size;
	*dataPtr = (void*)(bh+1);
	return MEM_MNG_ERROR_OK;
}

MEM_MNG_ERROR n_free(void* data){

	if (NULL==data)
		return MEM_MNG_ERROR_IS_NULL;
	block_header *bh = ((block_header*)data)-1;
	if (MEM_MNG_PREAMBLE!=bh->preamble)
		return MEM_MNG_ERROR_BLOCK_NOT_ALLOCATED;
	bh->preamble=0;
	free(bh);
	return MEM_MNG_ERROR_OK;
}

MEM_MNG_ERROR n_read(void* data, void* buffer, uint32_t startPoint, uint32_t size){

	if (NULL==data||NULL==buffer||0==size)
		return MEM_MNG_ERROR_IS_NULL;

	block_header *bh = ((block_header*)data)-1;
	if (MEM_MNG_PREAMBLE!=bh->preamble)
		return MEM_MNG_ERROR_BLOCK_NOT_ALLOCATED;
	if (startPoint+size>=bh->size)
		return MEM_MNG_ERROR_OUT_OF_RANGE;
	memcpy((uint8_t*)buffer, (uint8_t*)data+startPoint, size);

	return MEM_MNG_ERROR_OK;
}


MEM_MNG_ERROR n_write(void* data, const void* buffer, uint32_t startPoint, uint32_t size){

	if (NULL==data||NULL==buffer||0==size)
		return MEM_MNG_ERROR_IS_NULL;

	block_header *bh = ((block_header*)data)-1;
	if (MEM_MNG_PREAMBLE!=bh->preamble)
		return MEM_MNG_ERROR_BLOCK_NOT_ALLOCATED;
	if (startPoint+size>=bh->size)
		return MEM_MNG_ERROR_OUT_OF_RANGE;
	memcpy((uint8_t*)data+startPoint, (uint8_t*)buffer, size);

	return MEM_MNG_ERROR_OK;
}
