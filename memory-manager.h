#ifndef MEMORY_MANAGER_H_
#define MEMORY_MANAGER_H_

#include <stdint.h>

/**
 * @enum MEM_MNG_ERROR
 * @brief List of error codes
 *
 */
enum MEM_MNG_ERROR {
	MEM_MNG_ERROR_OK,                   /**< MEM_MNG_ERROR_OK No errors */
	MEM_MNG_ERROR_ALLOCATION_IMPOSSIBLE,/**< MEM_MNG_ERROR_ALLOCATION_IMPOSSIBLE malloc() returned wrong response */
	MEM_MNG_ERROR_OUT_OF_RANGE,         /**< MEM_MNG_ERROR_OUT_OF_RANGE reading or writing is out of memory range */
	MEM_MNG_ERROR_BLOCK_NOT_ALLOCATED,  /**< MEM_MNG_ERROR_BLOCK_NOT_ALLOCATED block was not allocated so it can not be used */
	MEM_MNG_ERROR_IS_NULL,              /**< MEM_MNG_ERROR_IS_NULL provided pointer is NULL*/
	MEM_MNG_ERROR_ZERO_MEM_ALLOCATED    /**< MEM_MNG_ERROR_ZERO_MEM_ALLOCATED you provide size of allocated block equal 0 */
};
typedef enum MEM_MNG_ERROR MEM_MNG_ERROR;

/**
 * @fn MEM_MNG_ERROR n_alloc(void**, uint32_t)
 * @brief Function allocates a block of memory of a given size or generates an error if the allocation is not possible
 *
 * @param dataPtr Pointer to data where pointer to memory is stored
 * @param size Size of data to be allocated in bytes
 * @return Error code
 */
MEM_MNG_ERROR n_alloc(void** dataPtr, uint32_t size);

/**
 * @fn MEM_MNG_ERROR n_read(void*, void*, uint32_t)
 * @brief Function performs reading of a specified number of bytes at a given address
 *
 * @param data Address in memory where reading is performed from
 * @param buffer Address of buffer, which is filled with data read. buffer is supposed to be on stack
 * @param startPoint Specifies which offset should be taken
 * @param size how many bytes to copy
 * @return Error code
 */
MEM_MNG_ERROR n_read(void* data, void* buffer, uint32_t startPoint, uint32_t size);

/**
 * @fn MEM_MNG_ERROR n_write(void*, void*, uint32_t)
 * @brief Function performs writing of a specified number of bytes at a given address
 *
 * @param data Address in memory where writing is performed to
 * @param buffer Address of buffer, which is filled with data read
 * @param startPoint Specifies which offset should be taken
 * @param size how many bytes to copy
 * @return Error code
 */
MEM_MNG_ERROR n_write(void* data, const void* buffer, uint32_t startPoint, uint32_t size);

/**
 * @fn MEM_MNG_ERROR n_free(void*)
 * @brief Function performs reading of a specified number of bytes at a given address
 *
 * @param data Address in memory to be freed
 * @return Error code
 *
 * @warning If you try to free custom pointer, not by MM, then SegFault is guaranteed
 */
MEM_MNG_ERROR n_free(void* data);

#endif /* MEMORY_MANAGER_H_ */
