// copyright 2021-2022 Mirza Ana-Maria 311CA

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Hashmap.h"
#include "library.h"
#include "users.h"

#define MAX 255
#define BMAX 40

int add_user(hashtable_t **users)
{
    user_info_t *user = malloc(sizeof(*user));
    scanf("%s", user->name);

    // check if user is in system
    if (ht_has_key(*users, user->name)) {
        printf("User is already registered.\n");
        free(user);
        return 1;
    }

    // check if library needs resize
    if ((*users)->size > (*users)->hmax) {
        *users = resize(*users, sizeof(user_info_t), 0);
        if (!(*users))
            return 0;
    }

    // create user
    user->days_borrowed = 0;
    user->points = 100;
    user->banned = 0;
    user->borrowed = 0;
    memcpy(user->book_borrowed, "\0", 1);

    ht_put(*users, user->name, strlen(user->name) + 1,
            user, sizeof(*user));
    free(user);
    return 1;
}

int borrow(hashtable_t *library, hashtable_t *users)
{
    char *user_name, *book, buffer[MAX];
    int days;

    fgets(buffer, MAX, stdin);

    // parse input
    char *rest;
    char *token = strtok_r(buffer, " ", &rest);
    user_name = token;
    token = strtok_r(rest, "\"", &rest);
    book = token;
    token = strtok_r(rest, " \n", &rest);
    days = atoi(token);

    // check if user is registered
    if (!ht_has_key(users, user_name)) {
        printf("You are not registered yet.\n");
        return 1;
    }
    user_info_t *user = ht_get(users, user_name);
    // check if user is banned
    if (user->banned) {
        printf("You are banned from this library.\n");
        return 1;
    }
    // check if user already borrowed a book
    if (user->borrowed) {
        printf("You have already borrowed a book.\n");
        return 1;
    }
    // check if book is in library
    if (!ht_has_key(library, book)) {
        printf("The book is not in the library.\n");
        return 1;
    }
    book_value_t *b = ht_get(library, book);
    // check if book is borrowed
    if (!b->available) {
        printf("The book is borrowed.\n");
        return 1;
    }

    // borrow book operations
    memcpy(user->book_borrowed, book, strlen(book) + 1);
    user->days_borrowed = days;
    user->borrowed = 1;
    b->available = 0;

    return 1;
}

int return_book(hashtable_t *library, hashtable_t *users)
{
    char user_name[20], book[BMAX], buffer[MAX];
    int days;
    float rating;

    fgets(buffer, MAX, stdin);

    // parse input
    char *rest;
    char *token = strtok_r(buffer, " ", &rest);
    memcpy(user_name, token, strlen(token) + 1);
    token = strtok_r(rest, "\"", &rest);
    memcpy(book, token, strlen(token) + 1);
    token = strtok_r(rest, " ", &rest);
    days = atoi(token);
    token = strtok_r(rest, " \n", &rest);
    rating = (float) atof(token);
    user_info_t *user = ht_get(users, user_name);

    // check if user is banned
    if (user->banned) {
        printf("You are banned from this library.\n");
        return 1;
    }
    // check if user wants to return the book he borrowed
    if (strcmp(book, user->book_borrowed)) {
        printf("You didn't borrow this book.\n");
        return 1;
    }

    // return book
    book_value_t *b = ht_get(library, book);
    b->available = 1;
    user->borrowed = 0;
    b->sum_rating += rating;
    b->purchases++;
    b->rating = (float) b->sum_rating / b->purchases;


    // add or subtract points user points
    int dif = user->days_borrowed - days;
    if (dif < 0) {
        user->points += 2 * dif;
    } else {
        user->points += dif;
    }

    // ban user if score is negative
    if (user->points < 0) {
        user->banned = 1;
        printf("The user %s has been banned from this library.\n", user_name);
        return 1;
    }
    return 1;
}

int lost(hashtable_t *library, hashtable_t *users)
{
    char user_name[20], book[BMAX], buffer[MAX];
    fgets(buffer, MAX, stdin);

    // parse input
    char *rest;
    char *token = strtok_r(buffer, " ", &rest);
    memcpy(&user_name, token, strlen(token) + 1);
    token = strtok_r(rest, "\"", &rest);
    memcpy(&book, token, strlen(token) + 1);

    // check if user is registered
    if (!ht_has_key(users, &user_name)) {
        printf("You are not registered yet.\n");
        return 1;
    }
    // check if user is banned
    user_info_t *user = ht_get(users, &user_name);
    if (user->banned) {
        printf("You are banned from this library.\n");
        return 1;
    }

    // delete book from library
    book_value_t *b = ht_get(library, &book);
    ht_free(b->content);
    free(b->name);
    ht_remove_entry(library, &book);

    // subtratct points from user and ban if score gets negative
    user->points -= 50;
    if (user->points < 0) {
        user->banned = 1;
        printf("The user %s has been banned from this library.\n", user_name);
    }
    user->borrowed = 0;

    return 1;
}
