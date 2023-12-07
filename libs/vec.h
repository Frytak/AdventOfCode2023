#include <corecrt.h>
#include <stdbool.h>

typedef enum {
    // The searched value is to the left
    VBS_COMP_LEFT,

    // The searched value is to the right
    VBS_COMP_RIGHT,

    // Found the serched value
    VBS_COMP_FOUND,
} VBS_COMP;

typedef enum {
    // No errors, value was found
    VBS_OK,

    // The value doesn't exist in the specified boundries
    VBS_NOT_FOUND,

    // `beg` or `end` are larger than the vector length
    VBS_OUT_OF_BOUNDS,

    // `beg` is bigger than `end`
    VBS_INVALID_INPUT,

    // the `comp` function returns a value that is not -1, 0 or 1
    VBS_COMP_INVALID_OUTPUT,
} VBS_RET;

typedef enum {
    // No errors, value was found
    CF_OK,

    // The value doesn't exist in the specified boundries
    CF_NOT_FOUND,

    // `beg` or `end` are larger than the vector length
    CF_OUT_OF_BOUNDS,

    // `beg` is bigger than `end`
    CF_INVALID_INPUT,

    // the `comp` function returns a value that is boolean (true, false) (1, 0)
    CF_COMP_INVALID_OUTPUT,
} COMP_FUNC_RET;

typedef struct Vector {
    size_t len;
    size_t cap;
    size_t size;
    void *data;
} Vector;

int _vec_alloc(Vector *vec, size_t cap);

int _vec_double(Vector *vec);

VBS_RET vec_binary_search(Vector *vec, VBS_COMP (*comp)(void *vec_item), size_t beg, size_t end, size_t *found);

int vec_push(Vector *vec, void *data);

void *vec_get_unchecked(Vector *vec, size_t index);

void *vec_get(Vector *vec, size_t index);

COMP_FUNC_RET vec_find_first(Vector *vec, bool (*comp)(void *vec_item), size_t beg, size_t end, size_t *index);

COMP_FUNC_RET vec_contains(Vector *vec, bool (*comp)(void *vec_item, void *provided_item), void *item, size_t beg, size_t end, size_t *index);

int vec_init(Vector *vec, size_t size, void *data, size_t amount);

errno_t vec_read_file(Vector *vec, char file_name[], size_t *bytes_written, bool minimize);

void p_vec_info(Vector *vec);
