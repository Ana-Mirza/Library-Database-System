// copyright 2021-2022 Mirza Ana-Maria 311CA

#ifndef USERS_H_
#define USERS_H_

/* struct of a user value*/
typedef struct user_info_t
{
    char name[20];
    char book_borrowed[40];
    int days_borrowed;
    int borrowed;
    int points;
    int banned;
} user_info_t;

/* function that adds a user in the system by adding
   user in the user hashtable; every new user gets 100
   points*/
int add_user(hashtable_t **users);

/* function that makes the neccessary modifications in the
   book and user systems so that a book is borrowed;
   the user cannot borrow another book until he return the
   book, and the book is unreachable for other users*/
int borrow(hashtable_t *library, hashtable_t *users);

/* function that makes modifications in the system so that
   a book is returned and availabe to be borrowed again;
   a user's points increase or decrease based on how quickly
   he returns the book; if score gets negative, the user is
   banned from the library*/
int return_book(hashtable_t *library, hashtable_t *users);

/* function that erases a book from the librart and subtracts
   50 points from the user that lost the book; if points of the
   user get negative, the user is banned from the library*/
int lost(hashtable_t *library, hashtable_t *users);

#endif  // USERS_H_
