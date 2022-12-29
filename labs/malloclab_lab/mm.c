/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "team",
    /* First member's full name */
    "GentleCold",
    /* First member's email address */
    "1952173800@qq.com",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8
/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~0x7)

// My define
#define WSIZE 4
#define DSIZE 8
#define FSIZE 16
#define CHUNK 1 << 10

#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define PARSE(v) ((v) & ~0x7)
#define PACK(v, a) ((v) | (a))

#define HEAD(bp) ((byte *)(bp) - WSIZE)
#define FOOT(bp) ((byte *)(bp) + SIZE(bp))
#define SIZE(bp) (PARSE(GET(HEAD(bp))))
#define ALLOC(bp) (GET(HEAD(bp)) & 0x1)

#define GET(p) (*(word *)(p))
#define SET(p, v) (*(word *)(p) = (v))

#define NEXT(bp) (FOOT(bp) + DSIZE)
#define PREV(bp) ((byte *)(bp) - PARSE(GET((bp) - DSIZE)) - DSIZE)

typedef unsigned int word;
typedef char byte;

// mark the front and tail pos
void *front_p = NULL;
void *tail_p = NULL;

// My func
/**
 * add a blank chunk and coalesce
 * will update tail_p
 * @param size align by 8, excluding head and foot
 * @return new bp
 */
static void *_extend(size_t size);

/**
 * coalesce blank chunk before and after bp
 * @param bp loaded point
 * @return bp after coalesce
 */
static void *_coalesce(void *bp);
static void *__coalesce_prev(void *bp);
static void *__coalesce_next(void *bp);
static void *__coalesce_all(void *bp);

/**
 * traverse and find first fit, then place in
 * @param size align by 8, excluding head and foot
 * @return
 */
static void *_first_fit(size_t size);

/**
 * allocate the block and cut sometimes
 * @param size align by 8, excluding head and foot
 */
static void _place(void *ptr, size_t size);
// end

/**
 * initialize the malloc package.
 * get a new chunk, set front_p and tail_p
 */
int mm_init(void) {
    if ((front_p = mem_sbrk(WSIZE)) == (void *) - 1) return -1; // blank
    front_p += DSIZE; // first chunk
    if (!_extend(CHUNK)) return -1;
    return 0;
}

/**
 * find first fit or extend
 */
void *mm_malloc(size_t size) {
    size_t adjust_size = ALIGN(size);
    size_t extend_size;
    void *bp;
    if ((bp = _first_fit(adjust_size)) != NULL) {
        return bp;
    } else {
        extend_size = adjust_size;
        if (!ALLOC(tail_p)) {
            extend_size -= (SIZE(tail_p) + DSIZE);
        }
        bp = _extend(MAX(extend_size, CHUNK));
        if (bp == NULL) return bp;
        _place(bp, adjust_size);
        return bp;
    }
}

/**
 * free a block and coalesce immediately
 */
void mm_free(void *ptr) {
    size_t size = SIZE(ptr);
    SET(HEAD(ptr), PACK(size, 0));
    SET(FOOT(ptr), PACK(size, 0));
    _coalesce(ptr);
}

/**
 * implemented simply in terms of mm_malloc and mm_free
 * if size <= old_size, then cut instead of malloc and coalesce
 * if the rest blank are enough for the size, then coalesce
 * if too big, then cut again
 */
void *mm_realloc(void *ptr, size_t size) {
    if (ptr == NULL) return mm_malloc(size);
    if (size == 0) return NULL;

    void *new_ptr;
    size_t old_size = SIZE(ptr);
    size_t adjust_size = ALIGN(size);
    if (adjust_size <= old_size) {
        if (old_size - adjust_size >= FSIZE) {
            _place(ptr, adjust_size); // just cut
            if (NEXT(ptr) != tail_p && !ALLOC(NEXT(NEXT(ptr)))) __coalesce_next(NEXT(ptr));
        }
        return ptr;
    } else {
        if (ptr != tail_p && !ALLOC(NEXT(ptr)) && SIZE(ptr) + SIZE(NEXT(ptr)) + DSIZE >= adjust_size) {
            if (NEXT(ptr) == tail_p) tail_p = ptr;
            size_t new_size = SIZE(ptr) + SIZE(NEXT(ptr)) + DSIZE;
            SET(HEAD(ptr), PACK(new_size, 1));
            SET(FOOT(ptr), PACK(new_size, 1));
            if (SIZE(ptr) - adjust_size >= FSIZE) {
                _place(ptr, adjust_size);
            }
            return ptr;
        }

        if ((new_ptr = mm_malloc(size)) == NULL) return NULL;
        memcpy(new_ptr, ptr, old_size);
        mm_free(ptr);
        return new_ptr;
    }
}

// my func
static void *_extend(size_t size) {
    void *bp;
    if ((bp = mem_sbrk(size + DSIZE)) == (void *) - 1) return NULL;
    // init chunk
    SET(bp, PACK(size, 0));
    bp += WSIZE;
    SET(FOOT(bp), PACK(size, 0));
    // init mark point
    tail_p = bp;
    return _coalesce(bp);
}

static void *_coalesce(void *bp) {
    // one chunk
    if (bp == front_p && bp == tail_p) return bp;
    if (bp == front_p || ALLOC(PREV(bp))) {
        if (bp == tail_p || ALLOC(NEXT(bp))) return bp;
        return __coalesce_next(bp);
    } else if (bp == tail_p || ALLOC(NEXT(bp))) {
        return __coalesce_prev(bp);
    } else {
        return __coalesce_all(bp);
    }
}

static void *__coalesce_prev(void *bp) {
    void *prev = PREV(bp);
    size_t new_size = SIZE(prev) + SIZE(bp) + DSIZE;
    SET(HEAD(prev), PACK(new_size, 0));
    SET(FOOT(bp), PACK(new_size, 0));
    if (bp == tail_p) tail_p = prev;
    return prev;
}

static void *__coalesce_next(void *bp) {
    void *next = NEXT(bp);
    size_t new_size = SIZE(next) + SIZE(bp) + DSIZE;
    SET(HEAD(bp), PACK(new_size, 0));
    SET(FOOT(next), PACK(new_size, 0));
    if (next == tail_p) tail_p = bp; // should also change
    return bp;
}

static void *__coalesce_all(void *bp) {
    void *prev = PREV(bp);
    void *next = NEXT(bp);
    size_t new_size = SIZE(prev) + SIZE(bp) + SIZE(next) + FSIZE;
    SET(HEAD(prev), PACK(new_size, 0));
    SET(FOOT(next), PACK(new_size, 0));
    if (next == tail_p) tail_p = prev;
    return prev;
}

static void *_first_fit(size_t size) {
    void *bp = front_p;
    void *after_p = NEXT(tail_p);
    while (bp != after_p) {
        if (!ALLOC(bp) && SIZE(bp) >= size) {
            _place(bp, size);
            return bp;
        }
        bp = NEXT(bp);
    }
    return NULL;
}

static void _place(void *ptr, size_t size) {
    size_t p_size = SIZE(ptr);
    if (p_size - size >= FSIZE) {
        SET(HEAD(ptr), PACK(size, 1));
        SET(FOOT(ptr), PACK(size, 1));
        // DSIZE adjust
        size_t adjust_size = p_size - size - DSIZE;
        SET(HEAD(NEXT(ptr)), PACK(adjust_size, 0));
        SET(FOOT(NEXT(ptr)), PACK(adjust_size, 0));
        if (ptr == tail_p) tail_p = NEXT(ptr);
    } else {
        SET(HEAD(ptr), PACK(p_size, 1));
        SET(FOOT(ptr), PACK(p_size, 1));
    }
}
