/**
 * @file gl_mem.c
 * General and portable implementation of malloc and free.
 * The dynamic memory monitoring is also supported.
 */


#include "gl_mem.h"
#include <string.h>

#define GL_MEM_SIZE (102400)
/*The size of this union must be 4/8 bytes (uint32_t/uint64_t)*/
typedef union
{
    struct
    {
        uint32_t used : 1;    /* 1: if the entry is used*/
        uint32_t d_size : 31; /* Size of the data*/
    } s;
    uint32_t header; /* The header (used + d_size)*/
} gl_mem_header_t;

typedef struct {
    gl_mem_header_t header;
    uint8_t first_data; /*First data byte in the allocated data (Just for easily create a pointer)*/
} gl_mem_ent_t;

#define ALIGN_MASK 0x3

#define MEM_BUF_SMALL_SIZE 16

static gl_mem_ent_t * ent_get_next(gl_mem_ent_t * act_e);
static void * ent_alloc(gl_mem_ent_t * e, size_t size);
static void ent_trunc(gl_mem_ent_t * e, size_t size);

static uint8_t * work_mem;

static uint32_t zero_mem; /*Give the address of this variable if 0 byte should be allocated*/
static uint32_t mem_max_size; /*Tracks the maximum total size of memory ever used from the internal heap*/

static uint8_t mem_buf1_32[MEM_BUF_SMALL_SIZE];
static uint8_t mem_buf2_32[MEM_BUF_SMALL_SIZE];

static gl_mem_buf_t mem_buf_small[] =
{
        {.p = mem_buf1_32, .size = MEM_BUF_SMALL_SIZE, .used = 0},
        {.p = mem_buf2_32, .size = MEM_BUF_SMALL_SIZE, .used = 0}
};
static uint8_t work_mem_int[GL_MEM_SIZE] = {0};

static gl_mem_buf_arr_t _gl_mem_buf;

#define COPY32 *d32 = *s32; d32++; s32++;
#define COPY8 *d8 = *s8; d8++; s8++;
#define SET32(x) *d32 = x; d32++;
#define SET8(x) *d8 = x; d8++;
#define REPEAT8(expr) expr expr expr expr expr expr expr expr


/**
 * Initialize the dyn_mem module (work memory and other variables)
 */
void _gl_mem_init(void)
{
    /*Allocate a large array to store the dynamically allocated data*/
    work_mem = (uint8_t *)work_mem_int;
    gl_mem_ent_t * full = (gl_mem_ent_t *)work_mem;
    full->header.s.used = 0;
    /*The total mem size reduced by the first header and the close patterns */
    full->header.s.d_size = GL_MEM_SIZE - sizeof(gl_mem_header_t);
}

/**
 * Clean up the memory buffer which frees all the allocated memories.
 * @note It work only if `GL_MEM_CUSTOM == 0`
 */
void _gl_mem_deinit(void)
{
    gl_mem_ent_t * full = (gl_mem_ent_t *)work_mem;
    full->header.s.used = 0;
    /*The total mem size reduced by the first header and the close patterns */
    full->header.s.d_size = GL_MEM_SIZE - sizeof(gl_mem_header_t);
}

/**
 * Allocate a memory dynamically
 * @param size size of the memory to allocate in bytes
 * @return pointer to the allocated memory
 */
void * gl_mem_alloc(size_t size)
{
    if(size == 0) {
        return &zero_mem;
    }

    /*Round the size up to ALIGN_MASK*/
    size = (size + ALIGN_MASK) & (~ALIGN_MASK);
    void * alloc = NULL;

    /*Use the built-in allocators*/
    gl_mem_ent_t * e = NULL;

    /* Search for a appropriate entry*/
    do {
        /* Get the next entry*/
        e = ent_get_next(e);

        /*If there is next entry then try to allocate there*/
        if(e != NULL) {
            alloc = ent_alloc(e, size);
        }
        /* End if there is not next entry OR the alloc. is successful*/
    } while(e != NULL && alloc == NULL);

    if(alloc == NULL)
    {
        // Couldn't allocate memory
    }
    else
    {
        /* just a safety check, should always be true */
        if((uintptr_t) alloc > (uintptr_t) work_mem) {
            if((((uintptr_t) alloc - (uintptr_t) work_mem) + size) > mem_max_size) {
                mem_max_size = ((uintptr_t) alloc - (uintptr_t) work_mem) + size;
            }
        }
    }

    return alloc;
}

/**
 * Free an allocated data
 * @param data pointer to an allocated memory
 */
void gl_mem_free(const void * data)
{
    if(data == &zero_mem) return;
    if(data == NULL) return;
    /*e points to the header*/
    gl_mem_ent_t * e = (gl_mem_ent_t *)((uint8_t *)data - sizeof(gl_mem_header_t));
    e->header.s.used = 0;
}

/**
 * Reallocate a memory with a new size. The old content will be kept.
 * @param data pointer to an allocated memory.
 * Its content will be copied to the new memory block and freed
 * @param new_size the desired new size in byte
 * @return pointer to the new memory
 */
void * gl_mem_realloc(void * data_p, size_t new_size)
{
    /*Round the size up to ALIGN_MASK*/
    new_size = (new_size + ALIGN_MASK) & (~ALIGN_MASK);

    /*data_p could be previously freed pointer (in this case it is invalid)*/
    if(data_p != NULL) {
        gl_mem_ent_t * e = (gl_mem_ent_t *)((uint8_t *)data_p - sizeof(gl_mem_header_t));
        if(e->header.s.used == 0) {
            data_p = NULL;
        }
    }

    uint32_t old_size = _gl_mem_get_size(data_p);
    if(old_size == new_size) return data_p; /*Also avoid reallocating the same memory*/

    /* Truncate the memory if the new size is smaller. */
    if(new_size < old_size) {
        gl_mem_ent_t * e = (gl_mem_ent_t *)((uint8_t *)data_p - sizeof(gl_mem_header_t));
        ent_trunc(e, new_size);
        return &e->first_data;
    }


    void * new_p;
    new_p = gl_mem_alloc(new_size);
    if(new_p == NULL)
    {
        return NULL;
    }

    if(data_p != NULL) {
        /*Copy the old data to the new. Use the smaller size*/
        if(old_size != 0 && new_size != 0) {
            gl_memcpy(new_p, data_p, GL_MIN(new_size, old_size));
        }
        gl_mem_free(data_p);
    }

    return new_p;
}

/**
 * Join the adjacent free memory blocks
 */
void gl_mem_defrag(void)
{
    gl_mem_ent_t * e_free;
    gl_mem_ent_t * e_next;
    e_free = ent_get_next(NULL);

    while(1) {
        /*Search the next free entry*/
        while(e_free != NULL) {
            if(e_free->header.s.used != 0) {
                e_free = ent_get_next(e_free);
            }
            else {
                break;
            }
        }

        if(e_free == NULL) return;

        /*Joint the following free entries to the free*/
        e_next = ent_get_next(e_free);
        while(e_next != NULL) {
            if(e_next->header.s.used == 0) {
                e_free->header.s.d_size += e_next->header.s.d_size + sizeof(e_next->header);
            }
            else {
                break;
            }

            e_next = ent_get_next(e_next);
        }

        if(e_next == NULL) return;

        /*Continue from the lastly checked entry*/
        e_free = e_next;
    }
}

bool gl_mem_test(void)
{
    gl_mem_ent_t * e;
    e = ent_get_next(NULL);
    while(e) {
        if(e->header.s.d_size > GL_MEM_SIZE) {
            return false;
        }
        uint8_t * e8 = (uint8_t *) e;
        if(e8 + e->header.s.d_size > work_mem + GL_MEM_SIZE) {
            return false;
        }
        e = ent_get_next(e);
    }

    return true;
}

/**
 * Give information about the work memory of dynamic allocation
 * @param mon_p pointer to a dm_mon_p variable,
 *              the result of the analysis will be stored here
 */
void gl_mem_monitor(gl_mem_monitor_t * mon_p)
{
    /*Init the data*/
    gl_memset(mon_p, 0, sizeof(gl_mem_monitor_t));

    gl_mem_ent_t * e;

    e = ent_get_next(NULL);

    while(e != NULL) {
        if(e->header.s.used == 0) {
            mon_p->free_cnt++;
            mon_p->free_size += e->header.s.d_size;
            if(e->header.s.d_size > mon_p->free_biggest_size) {
                mon_p->free_biggest_size = e->header.s.d_size;
            }
        }
        else {
            mon_p->used_cnt++;
        }

        e = ent_get_next(e);
    }
    mon_p->total_size = GL_MEM_SIZE;
    mon_p->max_used = mem_max_size;
    mon_p->used_pct = 100 - (100U * mon_p->free_size) / mon_p->total_size;
    if(mon_p->free_size > 0) {
        mon_p->frag_pct = mon_p->free_biggest_size * 100U / mon_p->free_size;
        mon_p->frag_pct = 100 - mon_p->frag_pct;
    }
    else
    {
        mon_p->frag_pct = 0; /*no fragmentation if all the RAM is used*/
    }
}

/**
 * Give the size of an allocated memory
 * @param data pointer to an allocated memory
 * @return the size of data memory in bytes
 */
uint32_t _gl_mem_get_size(const void * data)
{
    if(data == NULL) return 0;
    if(data == &zero_mem) return 0;

    gl_mem_ent_t * e = (gl_mem_ent_t *)((uint8_t *)data - sizeof(gl_mem_header_t));

    return e->header.s.d_size;
}

/**
 * Get a temporal buffer with the given size.
 * @param size the required size
 */
void * gl_mem_buf_get(uint32_t size)
{
    if(size == 0) return NULL;

    /*Try small static buffers first*/
    uint8_t i;
    if(size <= MEM_BUF_SMALL_SIZE) {
        for(i = 0; i < sizeof(mem_buf_small) / sizeof(mem_buf_small[0]); i++)
        {
            if(mem_buf_small[i].used == 0)
            {
                mem_buf_small[i].used = 1;
                return mem_buf_small[i].p;
            }
        }
    }

    /*Try to find a free buffer with suitable size */
    int8_t i_guess = -1;
    for(i = 0; i < GL_MEM_BUF_MAX_NUM; i++)
    {
        if((_gl_mem_buf[i]).used == 0 && _gl_mem_buf[i].size >= size)
        {
            if((_gl_mem_buf[i]).size == size) {
                (_gl_mem_buf[i]).used = 1;
                return (_gl_mem_buf[i]).p;
            }
            else if(i_guess < 0) {
                i_guess = i;
            }
            /*If size of `i` is closer to `size` prefer it*/
            else if(_gl_mem_buf[i].size < _gl_mem_buf[i_guess].size) {
                i_guess = i;
            }
        }
    }

    if(i_guess >= 0) {
        (_gl_mem_buf[i_guess]).used = 1;
        return (_gl_mem_buf[i_guess]).p;
    }

    /*Reallocate a free buffer*/
    for(i = 0; i < GL_MEM_BUF_MAX_NUM; i++) {
        if(_gl_mem_buf[i].used == 0) {
            /*if this fails you probably need to increase your GL_MEM_SIZE/heap size*/
            void * buf = gl_mem_realloc(_gl_mem_buf[i].p, size);
            if(buf == NULL)
            {
                // Out of memory, can't allocate a new buffer (increase your GL_MEM_SIZE/heap size)
                return NULL;
            }
            _gl_mem_buf[i].used = 1;
            _gl_mem_buf[i].size = size;
            _gl_mem_buf[i].p    = buf;
            return _gl_mem_buf[i].p;
        }
    }

    // No free buffer. Increase GL_MEM_BUF_MAX_NUM.
    return NULL;
}

/**
 * Release a memory buffer
 * @param p buffer to release
 */
void gl_mem_buf_release(void * p)
{
    uint8_t i;

    /*Try small static buffers first*/
    for(i = 0; i < sizeof(mem_buf_small) / sizeof(mem_buf_small[0]); i++) {
        if(mem_buf_small[i].p == p) {
            mem_buf_small[i].used = 0;
            return;
        }
    }

    for(i = 0; i < GL_MEM_BUF_MAX_NUM; i++) {
        if((_gl_mem_buf[i]).p == p) {
            (_gl_mem_buf[i]).used = 0;
            return;
        }
    }

    /* gl_mem_buf_release: p is not a known buffer */
}

/**
 * Free all memory buffers
 */
void _gl_mem_buf_free_all(void)
{
    uint8_t i;
    for(i = 0; i < sizeof(mem_buf_small) / sizeof(mem_buf_small[0]); i++)
    {
        mem_buf_small[i].used = 0;
    }

    for(i = 0; i < GL_MEM_BUF_MAX_NUM; i++)
    {
        if((_gl_mem_buf[i]).p)
        {
            gl_mem_free(_gl_mem_buf[i].p);
            (_gl_mem_buf[i]).p = NULL;
            (_gl_mem_buf[i]).used = 0;
            (_gl_mem_buf[i]).size = 0;
        }
    }
}

/**
 * Give the next entry after 'act_e'
 * @param act_e pointer to an entry
 * @return pointer to an entry after 'act_e'
 */
static gl_mem_ent_t * ent_get_next(gl_mem_ent_t * act_e)
{
    gl_mem_ent_t * next_e = NULL;

    if(act_e == NULL) { /*NULL means: get the first entry*/
        next_e = (gl_mem_ent_t *)work_mem;
    }
    else {   /*Get the next entry */
        uint8_t * data = &act_e->first_data;
        next_e         = (gl_mem_ent_t *)&data[act_e->header.s.d_size];

        if(&next_e->first_data >= &work_mem[GL_MEM_SIZE]) next_e = NULL;
    }

    return next_e;
}

/**
 * Try to do the real allocation with a given size
 * @param e try to allocate to this entry
 * @param size size of the new memory in bytes
 * @return pointer to the allocated memory or NULL if not enough memory in the entry
 */
static void * ent_alloc(gl_mem_ent_t * e, size_t size)
{
    void * alloc = NULL;
    /*If the memory is free and big enough then use it */
    if(e->header.s.used == 0 && e->header.s.d_size >= size) {
        /*Truncate the entry to the desired size */
        ent_trunc(e, size);
        e->header.s.used = 1;

        /*Save the allocated data*/
        alloc = &e->first_data;
    }

    return alloc;
}

/**
 * Truncate the data of entry to the given size
 * @param e Pointer to an entry
 * @param size new size in bytes
 */
static void ent_trunc(gl_mem_ent_t * e, size_t size)
{
    /*Round the size up to ALIGN_MASK*/
    size = (size + ALIGN_MASK) & (~ALIGN_MASK);

    /*Don't let empty space only for a header without data*/
    if(e->header.s.d_size == size + sizeof(gl_mem_header_t)) {
        size = e->header.s.d_size;
    }

    /* Create the new entry after the current if there is space for it */
    if(e->header.s.d_size != size) {
        uint8_t * e_data             = &e->first_data;
        gl_mem_ent_t * after_new_e   = (gl_mem_ent_t *)&e_data[size];
        after_new_e->header.s.used   = 0;
        after_new_e->header.s.d_size = (uint32_t)e->header.s.d_size - size - sizeof(gl_mem_header_t);

        /* Set the new size for the original entry */
        e->header.s.d_size = (uint32_t)size;
    }
}
