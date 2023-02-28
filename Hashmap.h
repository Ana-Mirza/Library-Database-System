// copyright 2021-2022 Mirza Ana-Maria 311CA

#ifndef HASHMAP_H_
#define  HASHMAP_H_

/* stuct for a node in a list*/
typedef struct ll_node_t
{
    void* data;
    struct ll_node_t* next;
} ll_node_t;

/* struct for a linked list*/
typedef struct linked_list_t
{
    ll_node_t* head;
    unsigned int data_size;
    unsigned int size;
} linked_list_t;

/* struct for data in a hashtable*/
typedef struct info {
	void *key;
	void *value;
} info;

/* struct for a hashtable*/
typedef struct hashtable_t hashtable_t;
struct hashtable_t {
	linked_list_t **buckets; /* Array de liste simplu-inlantuite. */
	/* Nr. total de noduri existente curent in toate bucket-urile. */
	unsigned int size;
	unsigned int hmax; /* Nr. de bucket-uri. */
	/* (Pointer la) Functie pentru a calcula valoarea hash asociata cheilor. */
	unsigned int (*hash_function)(void*);
	/* (Pointer la) Functie pentru a compara doua chei. */
	int (*compare_function)(void*, void*);
};


/* function that creates a simple linked list*/
linked_list_t *
ll_create(unsigned int data_size);

/* function that adds a node in a linked list on a given position.
   If the position is greater than the list's lize, the node is
   is added at the end. Indexation starts from 0, that is, the first
   node is on position 0. If n < 0, error.*/
void
ll_add_nth_node(linked_list_t* list, unsigned int n, const void* new_data);

/* function that removes a node from a liked list on a given position.
   If the given position is greater than the list's size, the last element
   in the list is eliminated. The caller is responsible to free the memory.*/
ll_node_t *
ll_remove_nth_node(linked_list_t* list, unsigned int n);

/* function that returns the size of a linked list*/
unsigned int
ll_get_size(linked_list_t* list);

/* function that frees the memory of a linked list*/
void
ll_free(linked_list_t** pp_list);

/* function that prints values of a linked list
   that stores data of type int*/
void
ll_print_int(linked_list_t* list);

/* function that prints values of a linked list
   that stores strings*/
void
ll_print_string(linked_list_t* list);

/* fuction that compares two int keys*/
int
compare_function_ints(void *a, void *b);

/* function that compares two string keys*/
int
compare_function_strings(void *a, void *b);

/* fuction that determines hash of an int value*/
unsigned int
hash_function_int(void *a);

/* function that determines hash of a string*/
unsigned int
hash_function_string(void *a);

/* function that creates a hashtable*/
hashtable_t *
ht_create(unsigned int hmax, unsigned int (*hash_function)(void*),
		int (*compare_function)(void*, void*));

/* function that determines if a key is in the hashtable or not.
   the function returns 1, if there is a value associated with the key,
   or 0, otherwise.*/
int
ht_has_key(hashtable_t *ht, void *key);

/* function that returns value of a certain key in the hashtable*/
void *
ht_get(hashtable_t *ht, void *key);

/* function that creates a key and value association in the hashtable
   by copying the actual bytes from the given address, so that the information
   cannot be lost or modified*/
void
ht_put(hashtable_t *ht, void *key, unsigned int key_size,
	void *value, unsigned int value_size);

/* function that removes a key and its value from the hashtable*/
void
ht_remove_entry(hashtable_t *ht, void *key);

/* function that frees memory occupied by hashtable;
   the buckets and list of buckets are freed*/
void
ht_free(hashtable_t *ht);

/* functtion that returns the size of a hashtable*/
unsigned int
ht_get_size(hashtable_t *ht);

/* function that returns the maximum capacity of a hashtable*/
unsigned int
ht_get_hmax(hashtable_t *ht);

#endif  // HASHMAP_H_
