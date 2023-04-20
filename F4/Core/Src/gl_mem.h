/**
 * @file gl_mem.h
 *
 */

#ifndef GL_MEM_H
#define GL_MEM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

#ifndef GL_MEM_BUF_MAX_NUM
#define GL_MEM_BUF_MAX_NUM    16
#endif

/**
 * Heap information structure.
 */
typedef struct
{
    uint32_t total_size; /**< Total heap size */
    uint32_t free_cnt;
    uint32_t free_size; /**< Size of available memory */
    uint32_t free_biggest_size;
    uint32_t used_cnt;
    uint32_t max_used; /**< Max size of Heap memory used */
    uint8_t used_pct; /**< Percentage used */
    uint8_t frag_pct; /**< Amount of fragmentation */
} gl_mem_monitor_t;

typedef struct
{
    void * p;
    uint16_t size;
    uint8_t used    : 1;
} gl_mem_buf_t;

typedef gl_mem_buf_t gl_mem_buf_arr_t[GL_MEM_BUF_MAX_NUM];



/**
 * Initialize the dyn_mem module (work memory and other variables)
 */
void _gl_mem_init(void);

/**
 * Clean up the memory buffer which frees all the allocated memories.
 * @note It work only if `GL_MEM_CUSTOM == 0`
 */
void _gl_mem_deinit(void);

/**
 * Allocate a memory dynamically
 * @param size size of the memory to allocate in bytes
 * @return pointer to the allocated memory
 */
void * gl_mem_alloc(size_t size);

/**
 * Free an allocated data
 * @param data pointer to an allocated memory
 */
void gl_mem_free(const void * data);

/**
 * Reallocate a memory with a new size. The old content will be kept.
 * @param data pointer to an allocated memory.
 * Its content will be copied to the new memory block and freed
 * @param new_size the desired new size in byte
 * @return pointer to the new memory
 */
void * gl_mem_realloc(void * data_p, size_t new_size);

/**
 * Join the adjacent free memory blocks
 */
void gl_mem_defrag(void);

/**
 *
 * @return
 */
bool gl_mem_test(void);

/**
 * Give information about the work memory of dynamic allocation
 * @param mon_p pointer to a dm_mon_p variable,
 *              the result of the analysis will be stored here
 */
void gl_mem_monitor(gl_mem_monitor_t * mon_p);

/**
 * Give the size of an allocated memory
 * @param data pointer to an allocated memory
 * @return the size of data memory in bytes
 */
uint32_t _gl_mem_get_size(const void * data);

/**
 * Get a temporal buffer with the given size.
 * @param size the required size
 */
void * gl_mem_buf_get(uint32_t size);

/**
 * Release a memory buffer
 * @param p buffer to release
 */
void gl_mem_buf_release(void * p);

/**
 * Free all memory buffers
 */
void _gl_mem_buf_free_all(void);

//! @cond Doxygen_Suppress

/**
 * Wrapper for the standard memcpy
 * @param dst pointer to the destination buffer
 * @param src pointer to the source buffer
 * @param len number of byte to copy
 */
static inline void * gl_memcpy(void * dst, const void * src, size_t len)
{
    return memcpy(dst, src, len);
}

/**
 * Wrapper for the standard memcpy
 * @param dst pointer to the destination buffer
 * @param src pointer to the source buffer
 * @param len number of byte to copy
 */
static inline void * gl_memcpy_small(void * dst, const void * src, size_t len)
{
    return memcpy(dst, src, len);
}

/**
 * Wrapper for the standard memset
 * @param dst pointer to the destination buffer
 * @param v value to set [0..255]
 * @param len number of byte to set
 */
static inline void gl_memset(void * dst, uint8_t v, size_t len)
{
    memset(dst, v, len);
}

/**
 * Wrapper for the standard memset with fixed 0x00 value
 * @param dst pointer to the destination buffer
 * @param len number of byte to set
 */
static inline void gl_memset_00(void * dst, size_t len)
{
    memset(dst, 0x00, len);
}

/**
 * Wrapper for the standard memset with fixed 0xFF value
 * @param dst pointer to the destination buffer
 * @param len number of byte to set
 */
static inline void gl_memset_ff(void * dst, size_t len)
{
    memset(dst, 0xFF, len);
}


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*GL_MEM_H*/
