// copyright 2021-2022 Mirza Ana-Maria 311CA

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "Hashmap.h"

#define MAX_STRING_SIZE	256
#define HMAX 10

linked_list_t *
ll_create(unsigned int data_size)
{
    linked_list_t* ll;

    ll = malloc(sizeof(*ll));

    ll->head = NULL;
    ll->data_size = data_size;
    ll->size = 0;

    return ll;
}

void
ll_add_nth_node(linked_list_t* list, unsigned int n, const void* new_data)
{
    ll_node_t *prev, *curr;
    ll_node_t* new_node;

    if (!list) {
        return;
    }

    if (n > list->size) {
        n = list->size;
    }

    curr = list->head;
    prev = NULL;
    while (n > 0) {
        prev = curr;
        curr = curr->next;
        --n;
    }

    new_node = malloc(sizeof(*new_node));
    new_node->data = malloc(list->data_size);
    memcpy(new_node->data, new_data, list->data_size);

    new_node->next = curr;
    if (prev == NULL) {
        /* Adica n == 0. */
        list->head = new_node;
    } else {
        prev->next = new_node;
    }

    list->size++;
}

ll_node_t *
ll_remove_nth_node(linked_list_t* list, unsigned int n)
{
    ll_node_t *prev, *curr;

    if (!list || !list->head) {
        return NULL;
    }

    if (n > list->size - 1) {
        n = list->size - 1;
    }

    curr = list->head;
    prev = NULL;
    while (n > 0) {
        prev = curr;
        curr = curr->next;
        --n;
    }

    if (prev == NULL) {
        /* Adica n == 0. */
        list->head = curr->next;
    } else {
        prev->next = curr->next;
    }

    list->size--;

    return curr;
}

unsigned int
ll_get_size(linked_list_t* list)
{
     if (!list) {
        return -1;
    }

    return list->size;
}

void
ll_free(linked_list_t** pp_list)
{
    ll_node_t* currNode;

    if (!pp_list || !*pp_list) {
        return;
    }

    while (ll_get_size(*pp_list) > 0) {
        currNode = ll_remove_nth_node(*pp_list, 0);
        free(currNode->data);
        currNode->data = NULL;
        free(currNode);
        currNode = NULL;
    }

    free(*pp_list);
    *pp_list = NULL;
}

void
ll_print_int(linked_list_t* list)
{
    ll_node_t* curr;

    if (!list) {
        return;
    }

    curr = list->head;
    while (curr != NULL) {
        printf("%d ", *((int*)curr->data));
        curr = curr->next;
    }

    printf("\n");
}

void
ll_print_string(linked_list_t* list)
{
    ll_node_t* curr;

    if (!list) {
        return;
    }

    curr = list->head;
    while (curr != NULL) {
        printf("%s ", (char*)curr->data);
        curr = curr->next;
    }

    printf("\n");
}

int
compare_function_ints(void *a, void *b)
{
	int int_a = *((int *)a);
	int int_b = *((int *)b);

	if (int_a == int_b) {
		return 0;
	} else if (int_a < int_b) {
		return -1;
	} else {
		return 1;
	}
}

int
compare_function_strings(void *a, void *b)
{
	char *str_a = (char *)a;
	char *str_b = (char *)b;

	return strcmp(str_a, str_b);
}

unsigned int
hash_function_int(void *a)
{
	/*
	 * Credits: https://stackoverflow.com/a/12996028/7883884
	 */
	unsigned int uint_a = *((unsigned int *)a);

	uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
	uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
	uint_a = (uint_a >> 16u) ^ uint_a;
	return uint_a;
}

unsigned int
hash_function_string(void *a)
{
	/*
	 * Credits: http://www.cse.yorku.ca/~oz/hash.html
	 */
	unsigned char *puchar_a = (unsigned char*) a;
	int64_t hash = 5381;
	int c;

	while ((c = *puchar_a++))
		hash = ((hash << 5u) - hash) + c; /* hash * 31 + c */

	return hash;
}

hashtable_t *
ht_create(unsigned int hmax, unsigned int (*hash_function)(void*),
		int (*compare_function)(void*, void*))
{
	hashtable_t *hashtable = malloc(sizeof(*hashtable));
    hashtable->size = 0;
    hashtable->hmax = hmax;
    hashtable->compare_function = compare_function;
    hashtable->hash_function = hash_function;
    hashtable->buckets = malloc(hmax * sizeof(linked_list_t *));
    for (unsigned int i = 0; i < hmax; i++)
        hashtable->buckets[i] = ll_create(sizeof(info));
    return hashtable;
}

int
ht_has_key(hashtable_t *ht, void *key)
{
	unsigned int h_id = ht->hash_function(key);
    unsigned int bucket_id = h_id % ht->hmax;
    ll_node_t *node = ht->buckets[bucket_id]->head;
    while (node != NULL) {
        if ((*(ht->compare_function))(key, ((info *)node->data)->key) == 0)
            return 1;
        node = node->next;
    }

	return 0;
}

void *
ht_get(hashtable_t *ht, void *key)
{
	unsigned int h_id = ht->hash_function(key);
    unsigned int bucket_id = h_id % ht->hmax;
    ll_node_t *node = ht->buckets[bucket_id]->head;
    while (node != NULL) {
        if (ht->compare_function(key, ((info *)node->data)->key) == 0)
            return (((info *)node->data)->value);
        node = node->next;
    }

	return NULL;
}

void
ht_put(hashtable_t *ht, void *key, unsigned int key_size,
	void *value, unsigned int value_size)
{
    unsigned int hash_id = (*(ht->hash_function))(key);
    unsigned int bucket_id = hash_id % ht->hmax;
    linked_list_t *list = ht->buckets[bucket_id];

    if (list->size != 0) {
        ll_node_t *node = list->head;
        while (node != 0) {
            if ((*(ht->compare_function))(key,
                ((info *)node->data)->key) == 0) {
                free(((info *)node->data)->value);
                ((info *)node->data)->value = malloc(value_size);
                memcpy(((info *)node->data)->value, value, value_size);
                return;
            }
            node = node->next;
        }
    }

    info new_info;
    new_info.key = malloc(key_size);
    memcpy(new_info.key, key, key_size);
    new_info.value = malloc(value_size);
    memcpy(new_info.value, value, value_size);

    ll_add_nth_node(list, list->size, &new_info);
    ht->size++;
}

void
ht_remove_entry(hashtable_t *ht, void *key)
{
	unsigned int hash_id = (*(ht->hash_function))(key);
    unsigned int bucket_id = hash_id % ht->hmax;
    linked_list_t *list = ht->buckets[bucket_id];
    ll_node_t *node = ht->buckets[bucket_id]->head;
    int cont = 0;
    while ((*(ht->compare_function))(key, ((info*)node->data)->key) != 0) {
        node = node->next;
        cont++;
    }
    free(((info *)node->data)->key);
    free(((info *)node->data)->value);

    ll_node_t *remove = ll_remove_nth_node(list, cont);
    free(remove->data);
    free(remove);
    ht->size--;
}

void
ht_free(hashtable_t *ht)
{
    for (unsigned int i = 0; i < ht->hmax; i++) {
        ll_node_t *node = ht->buckets[i]->head;
        while (node != NULL) {
            free(((info *)node->data)->key);
            free(((info *)node->data)->value);
            node = node->next;
        }
        ll_free(&ht->buckets[i]);
    }
    free(ht->buckets);
    free(ht);
}

unsigned int
ht_get_size(hashtable_t *ht)
{
	if (ht == NULL)
		return 0;

	return ht->size;
}

unsigned int
ht_get_hmax(hashtable_t *ht)
{
	if (ht == NULL)
		return 0;

	return ht->hmax;
}
