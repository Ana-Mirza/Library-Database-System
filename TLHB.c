// copyright 2021-2022 Mirza Ana-Maria 311CA

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Hashmap.h"
#include "library.h"
#include "users.h"

#define MAX 255
#define HMAX 10

int main() {
    char command[MAX];
    scanf("%s", command);

    // create hashtable for library
    hashtable_t *library = ht_create(HMAX, hash_function_string,
                compare_function_strings);
    // create hashtable for users
    hashtable_t *users = ht_create(HMAX, hash_function_string,
				compare_function_strings);

    while (1) {
        if (strcmp(command, "ADD_BOOK") == 0) {
            int ok = add_book(&library);
            if (!ok) {
                printf("error func add_book()\n");
            }
        } else if (strcmp(command, "GET_BOOK") == 0) {
            int ok = get_book(library);
            if (!ok) {
                printf("error func get_book()\n");
            }
        } else if (strcmp(command, "RMV_BOOK") == 0) {
            int ok = rmv_book(library);
            if (!ok) {
                printf("error func rmv_book()\n");
            }
        } else if (strcmp(command, "ADD_DEF") == 0) {
            int ok = add_def(library);
            if (!ok) {
                printf("error func add_def()\n");
            }
        } else if (strcmp(command, "GET_DEF") == 0) {
            int ok = get_def(library);
            if (!ok) {
                printf("error func get_def()\n");
            }
        } else if (strcmp(command, "RMV_DEF") == 0) {
            int ok = rmv_def(library);
            if (!ok) {
                printf("error func rmv_def()\n");
            }
        } else if (strcmp(command, "ADD_USER") == 0) {
            int ok = add_user(&users);
            if (!ok) {
                printf("error func add_user()\n");
            }
        } else if (strcmp(command, "BORROW") == 0) {
            int ok = borrow(library, users);
            if (!ok) {
                printf("error func borrow()\n");
            }
        } else if (strcmp(command, "RETURN") == 0) {
            int ok = return_book(library, users);
            if (!ok) {
                printf("error func return_book()\n");
            }
        } else if (strcmp(command, "LOST") == 0) {
            int ok = lost(library, users);
            if (!ok) {
                printf("error func lost()\n");
            }
        } else if (strcmp(command, "EXIT") == 0) {
            exit_program(library, users);
            break;
        } else {
            char buff[MAX];
            fgets(buff, MAX, stdin);
        }

        // read next command
        scanf("%s", command);
    }
    return 0;
}
