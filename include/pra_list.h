/**
 * @file
 * @brief Implementation of a dynamically sized array list.
 */

#pragma once

#include <stddef.h>
#include <stdbool.h>
#include "pra_lib.h"
#include "pra_kmalloc.h"

static const size_t INITIAL_LIST_SIZE = 32;

typedef struct
{
    void **items;
    size_t size;
    size_t capacity;
} list_t;

typedef bool (*list_compare_callback_t)(void *this, void *other);

/**
 * @brief Creates a new list with default initial size.
 * @return newly created list
 */
list_t* list_create();

/**
 * @brief Creates a new list with specified initial size.
 * @param initial_size initial size of a list
 * @return newly created list
 */
list_t* list_create_with_initial_size(size_t initial_size);

/**
 * @brief Returns the number of elements in the list.
 * @param list the list
 * @return size of the list
 */
size_t list_get_size(list_t *list);

/**
 * @brief Returns the current remaining available space in the list.
 * @param list the list
 * @return capacity of the list
 */
size_t list_get_capacity(list_t *list);

/**
 * @brief Destroys the created list and frees all allocated memory.
 * @param list the list
 */
void list_destroy(list_t *list);

/**
 * @brief Appends element to the list. If the list if full, it reallocates the list to make a new space.
 * @param list the list
 * @param data element to append
 */
void list_append(list_t *list, void *data);

/**
 * @brief Replaces the element in the list at a specified position.
 * @param list the list
 * @param index index of the element we want to replace
 * @param data element to replace
 * @return true if the operation was successfull, otherwise false
 */
bool list_set(list_t *list, size_t index, void *data);

/**
 * @brief Returns the element in the list at a specified position.
 * @param list the list
 * @param index index of the element we want to get
 * @return element at specified position or NULL if there is not item at specified position
 */
void* list_get(list_t *list, size_t index);

/**
 * @brief Inserts the element into the list at a specified position.
 * @param list the list
 * @param index index where we want to insert the element
 * @param data element to insert
 * @return true if the insertion was successfull, otherwise false
 */
bool list_insert(list_t *list, size_t index, void *data);

/**
 * @brief Removes the element from a specified position.
 * @param list the list
 * @param index index of element we want to remove
 * @return true if element was successfully removed, otherwise false
 */
bool list_remove(list_t *list, size_t index);

/**
 * @brief Find an element in the list and returns its index.
 * @param list the list
 * @param data the element to search
 * @return index of found element, or -1 if element was not found
 */
int list_index_of(list_t *list, void *data);

/**
 * @brief Find an element in the list by using custom comparison function and returns its index.
 * @param list the list
 * @param data the element to search
 * @param callback the comparison function that will handle the comparisons
 * @return index of found element, or -1 if element was not found
 */
int list_index_of_by_comparison(list_t *list, void *data, list_compare_callback_t callback);
