#pragma once

#include <stddef.h>
#include <stdbool.h>
#include <lib.h>
#include <kmalloc.h>

static const size_t INITIAL_LIST_SIZE = 8;

typedef struct
{
    void **items;
    size_t size;
    size_t capacity;
} list_t;

typedef bool (*list_compare_callback_t)(void *this, void *other);

list_t* list_create();
list_t* list_create_with_initial_size(size_t initial_size);
void list_destroy(list_t *list);
void list_append(list_t *list, void *data);
bool list_set(list_t *list, size_t index, void *data);
bool list_insert(list_t *list, size_t index, void *data);
bool list_remove(list_t *list, size_t index);
int list_index_of(list_t *list, void *data);
int list_index_of_by_comparison(list_t *list, void *data, list_compare_callback_t callback);
