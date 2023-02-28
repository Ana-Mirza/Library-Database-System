// copyright 2021-2022 Mirza Ana-Maria 311CA

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Hashmap.h"
#include "users.h"
#include "library.h"

#define MAX 255
#define HMAX 10
#define BMAX 40
#define DMAX 20

void free_library(hashtable_t *library);

hashtable_t* resize(hashtable_t *ht, int data_size, int string)
{
    // create new library double in size
    hashtable_t *new_ht = ht_create(ht->hmax * 2, ht->hash_function,
                                    ht->compare_function);
    if (!new_ht) {
        printf("rezise failed\n");
        return NULL;
    }
    // move books in new library
    for (unsigned int i = 0; i < ht->hmax; i++) {
        ll_node_t *node = ht->buckets[i]->head;
        while (node != NULL) {
            char *key = ((info *)node->data)->key;
            void *value = ht_get(ht, key);
            if (!string) {
                ht_put(new_ht, key, strlen(key) + 1, value, data_size);
            } else  {
                ht_put(new_ht, key, strlen(key) + 1, value, strlen(value) + 1);
            }
            node = node->next;
        }
    }
    ht_free(ht);
    return new_ht;
}

int add_book(hashtable_t **library)
{
    char buffer[MAX];
    char *book_name;
    int def_num;

    // read input and parse
    fgets(buffer, MAX, stdin);
    buffer[0] = '\"';

    char *rest;
    char *token = strtok_r(buffer, "\"", &rest);
    book_name = token;
    token = strtok_r(rest, " ,\n", &rest);
    def_num = atoi(token);

    // check if library needs resize
    if ((*library)->size > (*library)->hmax) {
        *library = resize(*library, sizeof(book_value_t), 0);
        if (!(*library))
            return 0;
    }

    // introduce book in library or rewrite if existent
    if (ht_has_key(*library, book_name)) {
        book_value_t *b = ht_get(*library, book_name);
        ht_free(b->content);
        free(b->name);
    }
    book_value_t *b_value = malloc(sizeof(*b_value));
    if (!b_value) {
        return 0;
    }

    // initialize values for book
    b_value->name = malloc(strlen(book_name) + 1);
    memcpy(b_value->name, book_name, strlen(book_name) +1);
    b_value->available = 1;
    b_value->purchases = 0;
    b_value->rating = 0;
    b_value->sum_rating = 0;
    b_value->content = ht_create(HMAX, hash_function_string,
                        compare_function_strings);
    int key_len = strlen(book_name) + 1;

    // read definitions and add in book
    for (int i = 0; i < def_num; i++) {
        char def_key[DMAX], val_key[DMAX];

        scanf("%s %s", def_key, val_key);
        ht_put(b_value->content, def_key, strlen(def_key) + 1,
                val_key, strlen(val_key) + 1);
    }

    // add book with definitions in library
    ht_put(*library, book_name, key_len, b_value, sizeof(*b_value));
    free(b_value);

    return 1;
}

int get_book(hashtable_t *library)
{
    char buffer[MAX], *book_name;
    // read input and parse
    fgets(buffer, MAX, stdin);
    buffer[0] = '\"';

    char *rest;
    char *token = strtok_r(buffer, "\"", &rest);
    book_name = token;

    if (!ht_has_key(library, book_name)) {
        printf("The book is not in the library.\n");
        return 1;
    }

    book_value_t *b_value = ht_get(library, book_name);
    printf("Name:%s Rating:%.3f Purchases:%d\n", book_name,
            b_value->rating, b_value->purchases);

    return 1;
}

int rmv_book(hashtable_t *library)
{
    // read book name
    char buffer[MAX], *book_name;
    fgets(buffer, MAX, stdin);
    buffer[0] = '\"';

    char *rest;
    char *token = strtok_r(buffer, "\"", &rest);
    book_name = token;

    // check if book is in library
    if (!ht_has_key(library, book_name)) {
        printf("The book is not in the library.\n");
        return 1;
    }

    // delete content of book
    book_value_t *b_value = ht_get(library, book_name);
    ht_free(b_value->content);
    free(b_value->name);
    // delete book from library
    ht_remove_entry(library, book_name);
    return 1;
}

int add_def(hashtable_t *library)
{
    // read book name and def
    char buffer[MAX], *def_key, *def_val, *book_name;
    fgets(buffer, MAX, stdin);
    buffer[0] = '\"';

    // parse input
    char *rest;
    char *token = strtok_r(buffer, "\"", &rest);
    book_name = token;
    token = strtok_r(rest, "\n ", &rest);
    def_key = token;
    token = strtok_r(rest, " \n", &rest);
    def_val = token;

    // check if book is in library
    if (!ht_has_key(library, book_name)) {
        printf("The book is not in the library.\n");
        return 1;
    }

    // check if book needs resize
    book_value_t *b = ht_get(library, book_name);
    if (b->content->size > b->content->hmax) {
        b->content = resize(b->content, 0, 1);
        if (!b->content)
            return 0;
    }

    // add definition in book
    ht_put(b->content, def_key, strlen(def_key) + 1,
            def_val, strlen(def_val) + 1);

    return 1;
}

int get_def(hashtable_t *library)
{
    // read book name and def
    char buffer[MAX], *def_key, *book_name;
    fgets(buffer, MAX, stdin);
    buffer[0] = '\"';

    // parse input
    char *rest;
    char *token = strtok_r(buffer, "\"", &rest);
    book_name = token;
    token = strtok_r(rest, " \n", &rest);
    def_key = token;

    // check if book is in library
    if (!ht_has_key(library, book_name)) {
        printf("The book is not in the library.\n");
        return 1;
    }
    book_value_t *b = ht_get(library, book_name);

    // check if definiton is in book
    if (!ht_has_key(b->content, def_key)) {
        printf("The definition is not in the book.\n");
        return 1;
    }
    printf("%s\n", (char *)ht_get(b->content, def_key));

    return 1;
}

int rmv_def(hashtable_t *library)
{
    // read book name and def
    char buffer[MAX], *def_key, *book_name;
    fgets(buffer, MAX, stdin);
    buffer[0] = '\"';

    // parse input
    char *rest;
    char *token = strtok_r(buffer, "\"", &rest);
    book_name = token;
    token = strtok_r(rest, " \n", &rest);
    def_key = token;

    // check if book is in library
    if (!ht_has_key(library, book_name)) {
        printf("The book is not in the library.\n");
        return 1;
    }

    book_value_t *b = ht_get(library, book_name);

    // check if definiton is in book
    if (!ht_has_key(b->content, def_key)) {
        printf("The definition is not in the book.\n");
        return 1;
    }

    // delete definition from book
    ht_remove_entry(b->content, def_key);
    return 1;
}

int cmp_books(const void *a, const void *b) {
    book_value_t *book_a = *((book_value_t **) a);
    book_value_t *book_b = *((book_value_t **) b);

    if (book_b->rating > book_a->rating) {
        return 1;
    } else if (book_b->rating < book_a->rating) {
        return 0;
    } else if (book_b->purchases > book_a->purchases) {
        return 1;
    } else if (book_b->purchases < book_a->purchases) {
        return 0;
    }
    return strcmp(book_a->name, book_b->name);
}

int top_books(hashtable_t *library)
{
    // determine array of book names
    int size = 0;
    book_value_t **top = calloc(library->size, sizeof(book_value_t *));
    if (!top) {
        printf("calloc function failed\n");
        return 0;
    }

    // go through every bucket in the library hashtable
    for (unsigned int i = 0; i < library->hmax; i++) {
        ll_node_t *node = library->buckets[i]->head;
        // go through every node of a bucket
        while (node != NULL) {
            book_value_t *book = ((info *)node->data)->value;

            // save book in array
            top[size++] = book;
            node = node->next;
        }
    }
    if (size > 1)
        qsort(top, size, sizeof(book_value_t *), cmp_books);

    // print books in order
    printf("Books ranking:\n");
    for (int i = 0; i < size; i++) {
        printf("%d. Name:%s Rating:%.3f Purchases:%d\n",
                i + 1, top[i]->name, top[i]->rating, top[i]->purchases);
    }
    free(top);

    return 1;
}

int cmp_users(const void *a, const void *b)
{
    user_info_t *user_a = *((user_info_t **) a);
    user_info_t *user_b = *((user_info_t **) b);

    if (user_b->points > user_a->points) {
        return 1;
    } else if (user_b->points < user_a->points) {
        return 0;
    }
    return strcmp(user_a->name, user_b->name);
}

int top_users(hashtable_t *users)
{
    int size = 0;
    user_info_t **top = calloc(users->size, sizeof(user_info_t *));
    if (!top) {
        printf("calloc function failed\n");
        return 0;
    }

    // go through every bucket in the library hashtable
    for (unsigned int i = 0; i < users->hmax; i++) {
        ll_node_t *node = users->buckets[i]->head;
        // go through every node of a bucket
        while (node != NULL) {
            user_info_t *user = ((info *)node->data)->value;

            // save user in array if not banned
            if (!user->banned) {
                top[size++] = user;
            }
            node = node->next;
        }
    }
    // sort array of books
    if (size > 1)
        qsort(top, size, sizeof(user_info_t *), cmp_users);

    // print sorted users
    printf("Users ranking:\n");
    for (int i = 0; i < size; i++) {
        printf("%d. Name:%s Points:%d\n", i + 1, top[i]->name, top[i]->points);
    }
    free(top);
    return 1;
}

void free_library(hashtable_t *library)
{
    // delete content of books and then the books itselves
    for (unsigned int i  = 0; i < library->hmax; i++) {
        ll_node_t *node = library->buckets[i]->head;
        while (node != NULL) {
            // delete content of book
            book_value_t *b;
            b = (book_value_t *)((info *)node->data)->value;
            ht_free(b->content);
            free(b->name);

            // delete book key and value from library
            free(((info *)node->data)->key);
            free(((info *)node->data)->value);
            node = node->next;
        }
        ll_free(&library->buckets[i]);
    }
    // delete library
    free(library->buckets);
    free(library);
}

void exit_program(hashtable_t *library, hashtable_t *users)
{
    // check if library exists
    if (!library)
        return;

    // print books and users sorted
    top_books(library);
    top_users(users);

    // free memory occupied by books and users
    free_library(library);
    ht_free(users);
}
