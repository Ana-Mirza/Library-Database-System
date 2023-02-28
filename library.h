// copyright 2021-2022 Mirza Ana-Maria 311CA

#ifndef LIBRARY_H_
#define LIBRARY_H_

extern char *strtok_r(char *, const char *, char **);
extern char *strdup(const char *);

/* struct of a book value*/
typedef struct book_value_t
{
    int available;
    int purchases;
    int sum_rating;
    float rating;
    char *name;
    hashtable_t *content;
} book_value_t;

/* function that creates a new hashtable that is double the size of
   the original, copies elements from the old hashtable in the new one,
   and frees memory of old hashtable; data_size is used to determine the
   size of the value associated to the key, and string is used to determine
   if the key has a structure associated as value or a string (for book content)*/
hashtable_t* resize(hashtable_t *library, int data_size, int string);

/* function that adds a book in the library hashtable;
   the function also creates a hashtable for the book
   in which definitions are added; if the given book is
   already in the library, the content of the book is freed
   and updated with the new information*/
int add_book(hashtable_t **library);

/* function that prints name, rating, and purchases of a book*/
int get_book(hashtable_t *library);

/* function that removes a book from the library hashfunction;
   first, the hashtable with the book content is freed, and then
   the whole entrance*/
int rmv_book(hashtable_t *library);

/* function that adds a definition in a book, by adding it
   in the content hashtable of a book*/
int add_def(hashtable_t *library);

/* function that returns the value associated with the 
   given definition in the hashtable of a book*/
int get_def(hashtable_t *library);

/* function that removes the definition of a word from
   the content hashtable of a book*/
int rmv_def(hashtable_t *library);

/* function that compares two books based on their rating,
   purchases, and name*/
int cmp_books(const void *a, const void *b);

/* function that sorts books and prints them in order*/
int top_books(hashtable_t *library);

/* function that compares users based on points and their names*/
int cmp_users(const void *a, const void *b);

/* function that sorts and prints users in order*/
int top_users(hashtable_t *users);

/* function that frees memory of the library hashtable
   by freeing the content hashtables of books and then the
   book keys and balues*/
void free_library(hashtable_t *library);

/* function that ends the program by calling the printing funcitons
   for books and users, and freeing the memory occupied by library
   user hashtables*/
void exit_program(hashtable_t *library, hashtable_t *users);

#endif  // LIBRARY_H_
