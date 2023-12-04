#include <corecrt.h>

typedef struct {
    size_t len;
    size_t cap;
    size_t size;
    void *data;
} Vector;

int _vec_double(Vector *vec);

int vec_push(Vector *vec, void *data);

void *vec_get(Vector *vec, size_t index);

int vec_init(Vector *vec, size_t size, void *data, size_t amount);

errno_t vec_read_file(Vector *vec, char file_name[], size_t *bytes_written, bool minimize);

void p_vec_info(Vector *vec);
