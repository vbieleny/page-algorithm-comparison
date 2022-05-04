#include <list.h>

static bool list_grow(list_t *list, size_t new_capacity)
{
    if (new_capacity <= list->capacity)
        return true;
    void **new_items = kernel_memory_reallocate(list->items, new_capacity * sizeof(void*), 1);
    if (!new_items)
        return false;
    list->items = new_items;
    list->capacity = new_capacity;
    return true;
}

list_t* list_create()
{
    return list_create_with_initial_size(INITIAL_LIST_SIZE);
}

list_t* list_create_with_initial_size(size_t initial_size)
{
    list_t *list = kernel_memory_allocate(sizeof(list_t), 1);
    memset(list, 0, sizeof(list_t));
    list->items = kernel_memory_allocate(initial_size * sizeof(void*), 1);
    memset(list->items, 0, initial_size * sizeof(void*));
    list->size = 0;
    list->capacity = initial_size;
    return list;
}

void list_destroy(list_t *list)
{
    kernel_memory_free(list->items);
    kernel_memory_free(list);
}

void list_append(list_t *list, void *data)
{
    if (list->size + 1 > list->capacity)
        list_grow(list, list->capacity * 2);
    list->items[list->size++] = data;
}

bool list_set(list_t *list, size_t index, void *data)
{
    if (index >= list->size)
        return false;
    list->items[index] = data;
    return true;
}

bool list_insert(list_t *list, size_t index, void *data)
{
    if (index >= list->size)
        return false;
    if (list->size + 1 > list->capacity)
        list_grow(list, list->capacity * 2);
    for (size_t i = list->size - 1; i >= index; i--)
        list->items[i + 1] = list->items[i];
    list->items[index] = data;
    list->size++;
    return true;
}

bool list_remove(list_t *list, size_t index)
{
    if (index >= list->size)
        return false;
    for (size_t i = index; i < list->size; i++)
        list->items[i] = list->items[i + 1];
    list->size--;
    return true;
}

int list_index_of(list_t *list, void *data)
{
    for (int i = 0; i < list->size; i++)
    {
        if (list->items[i] == data)
            return i;
    }
    return -1;
}

int list_index_of_by_comparison(list_t *list, void *data, list_compare_callback_t callback)
{
    for (int i = 0; i < list->size; i++)
    {
        if (list->items[i] == data && callback(data, list->items[i]))
            return i;
    }
    return -1;
}
