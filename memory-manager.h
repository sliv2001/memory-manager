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
	MEM_MNG_ERROR_ZERO_MEM_ALLOCATED,   /**< MEM_MNG_ERROR_ZERO_MEM_ALLOCATED you provide size of allocated block equal 0 */
	MEM_MNG_ERROR_INTERNAL_ALLOCATION_ERROR, /**< MEM_MNG_ERROR_INTERNAL_ALLOCATION_ERROR MM could not get memory by calling malloc */
	MEM_MNG_OTHER_INTERNAL_ERROR /**< MEM_MNG_OTHER_INTERNAL_ERROR something happened */
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
 * @fn void n_access_ptr*(void*, void*)
 * @brief Function performs access to an object if it's valid
 *
 * @param object
 * @param accessed_data
 */
void* n_access_ptr(void* object, void* accessed_data);

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

/**
 * @fn MEM_MNG_ERROR n_free_all()
 * @brief Memory freeing with leakage check
 *
 * @warning Do not call this function. It's called automatically.
 *
 * @return Error code
 */
MEM_MNG_ERROR n_free_all();

/**
 * @def exit
 * @brief Replacement for usual exit() with mem leakage checks
 *
 */
#define exit(x) {\
	n_free_all();\
	exit(x);}

#endif /* MEMORY_MANAGER_H_ */
