/*
 * Sam Daulton
 * 26 Sept 2014
 * COSC 301 
 * Project 1
 */



#include <stdio.h>
#include <stdlib.h>
#include "list.h"

/* your list function definitions */

struct node {
    int value;
    struct node *next;
};

// clears this list of all values and frees the memory
void list_clear(struct node *list) {
    while (list != NULL) {
        struct node *tmp = list;
        list = list->next;
        free(tmp);
    }
}


/*
 * Takes a pointer to the head of a list and prints the values in the list 
 */
void list_print(const struct node *list) {
    int i = 0;
    printf("In list_print\n");
    while (list != NULL) {
        printf("List item %d: %d\n", i++, list->value);
        list = list->next;
    }
}



/*
 * Takes a pointer to a pointer to the head of a list of struct nodes and a new value to append to the list
 * as parameters and appends the new value to the list
 *
 */
void list_append(int value, struct node **head) {
    struct node *list = *head;
    struct node *last = list;
    while(list != NULL) {
        last = list;
        list = list->next;
    }
    struct node *new_node = malloc(sizeof(struct node));
    new_node->value = value;
    new_node->next = NULL;
    if (last == NULL) {
        //there were no nodes in the list
        *head = new_node;
    } else{
        last->next = new_node;
    }
}


/*
 * Takes a pointer to a pointer to the head of a list of struct nodes as a parameter 
 * and uses bubble sort to sort the list of words.  It updates the head
 *
 */
void list_sort(struct node **head) {
    struct node *next_node = *head;
    struct node *last = *head;
    struct node *current = *head;
    int is_sorted = 0;  //indicates if list is sorted
    int is_head = 1;  //indicates if we are at the head of the list
    if (current != NULL) {
        // make sure the list is not empty
        next_node = current->next;

        while (!is_sorted) {

            is_sorted = 1;
            is_head = 1;
            while (next_node != NULL) {
                if (next_node->value < current->value) {
                    // next_node value is less than current value, so swap them
                    if (is_head) {
                        *head = next_node;
                    } else {
                        last->next = next_node;
                    }
                    current->next = next_node->next;
                    next_node->next = current;
                    // then line up node names again, last then current then next_node
                    current = next_node;
                    next_node = next_node->next;
                    is_sorted = 0;
                }
            next_node = next_node->next;
            last = current;
            current = current->next;
            is_head = 0;
            }
            last = *head;
            current = *head;
            next_node = current->next;
        }
    }
}



