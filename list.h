/*
 * Sam Daulton
 * 26 Sept 2014
 * COSC 301 
 * Project 1
 */



#ifndef __LIST_H__
#define __LIST_H__

/* your list data structure declarations */
struct node;

/* your function declarations associated with the list */

// prints out each value in the list
void list_print(const struct node *list);

// appends value to the end of the list
void list_append(int value, struct node **head);

// sorts the list using bubble sort
void list_sort(struct node **head);

// clears this list of all values and frees the memory
void list_clear(struct node *list);


#endif // __LIST_H__
