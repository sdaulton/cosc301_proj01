/*
 * Sam Daulton
 * 26 Sept 2014
 * COSC 301 
 * Project 1
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include "list.h"
#include <sys/time.h>
#include <sys/resource.h>


/*
 * Modified version of tokenify from Lab02.  Takes a string as a parameter and
 * returns a pointer to an array of ints parsed from the string.  Note:
 * ints parsed must have whitespace on either side of the integer(or beginning
 * of line).  If there is a comment character (#) everything after that char
 * is ignored.  
*/
int* tokenify(const char *s) {
    int num_tokens = 10; // number of tokens that can be stored in the initial array
    int *tokens = malloc((num_tokens + 1) * sizeof(int)); // array of integers (tokens)
    int *new_tokens; //just incase array needs resizing
    char *next = NULL;
    bool isInt = true;
    char *cpy = strdup(s);
    //search for the comment sign, and replace it with the null termination char if it exists
    char *p_comm = strchr(cpy, '#');
    if (p_comm != NULL) {
        *p_comm = '\0';
    }
    next = strtok(cpy, " \t\r\n\v\f");
    // Note I am storing the actual number of tokens in the array in tokens[0]
    int i = 1;
    tokens[0] = 0;
    while (next != NULL){
        if (i >= num_tokens+1) {
            // need to resize array
            // create new array that can store twice as many tokens
            new_tokens = malloc((2 * num_tokens + 1) * sizeof(int ));
            for (int j = 0; j < num_tokens+1; j++) {
                new_tokens[j] = tokens[j];
            }
            free(tokens);
            tokens = new_tokens;
            num_tokens *= 2;
        }
        //check if next token is an integer
        for (int idx = 0; idx < strlen(next); idx++) {
            if (idx == 0) {
                //account for negative numbers
                if (next[idx] == '-') {
                    continue;
                }
            }
            if (!isdigit(next[idx])) {
                isInt = false;
            }
        }
        // if all characters are digits in the token, and the token is not the empty string
        if (isInt && (strlen(next) > 0)) {
            tokens[i] = atoi(next);
            i++;
            tokens[0]++;
        }
        isInt = true;
        next = strtok(NULL, " \t\r\n\v\f");
    }
    free(cpy);
    return tokens;
}


void process_data(FILE *input_file) {
    // !! your code should start here.  the input_file parameter
    // is an already-open file.  you can read data from it using
    // the fgets() C library function.  close it with the fclose()
    // built-in function
    struct node *list = NULL; 
    size_t *line_size = malloc(sizeof(size_t));
    *line_size = 1000 * sizeof(char); // ptr to the initial size of the string to hold the next line
    char *next_line = malloc(1000*sizeof(char)); // ptr to the string containing the next line
    int *numbers = NULL;
    while (getline(&next_line, line_size, input_file) != -1) {
        numbers = tokenify(next_line);
        // recall: numbers[0] contains the number of ints in the array
        for (int i = 1; i <= numbers[0]; i++) {
            list_append(numbers[i], &list);
        }    
        free(numbers);
    }
    free(line_size);
    free(next_line);
    list_sort(&list);
    list_print(list);
    list_clear(list);
    
    // now some usage time statistics
    struct rusage usage_time;
    if (getrusage(RUSAGE_SELF, &usage_time) != -1) {
        struct timeval sys_time = usage_time.ru_stime;
        struct timeval user_time = usage_time.ru_utime;
        printf("System Time: %lf\n", sys_time.tv_sec + sys_time.tv_usec * .0000001);
        printf("User Time: %lf\n", user_time.tv_sec+ user_time.tv_usec * .0000001);
    } else  {
        perror("getrusage");
        exit(1);
    }
}


void usage(char *program) {
    fprintf(stderr, "usage: %s [<datafile>]\n", program);
    exit(1);
}

#ifndef AUTOTEST
int main(int argc, char **argv) {
    FILE *datafile = NULL;

    /* find out how we got invoked and deal with it */
    switch (argc) {
        case 1:
            /* only one program argument (the program name) */ 
            /* just equate stdin with our datafile */
            datafile = stdin;        
            break;

        case 2:
            /* two arguments: program name and input file */
            /* open the file, assign to datafile */
            datafile = fopen(argv[1], "r");
            if (datafile == NULL) {
                printf("Unable to open file %s: %s\n", argv[1], strerror(errno));
                exit(-1);
            }
            break;

        default:
            /* more than two arguments?  throw hands up in resignation */
            usage(argv[0]);
    }

    /* 
     * you should be able to just read from datafile regardless 
     * whether it's stdin or a "real" file.
     */
    process_data(datafile);
    fclose(datafile);
    return 0;
}
#endif
