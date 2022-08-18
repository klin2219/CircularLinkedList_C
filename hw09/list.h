/**
 * CS 2110 - Summer 2022 - Homework 9
 *
 * Do not modify this file!
 *
 * list.h
 */

#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <string.h>

#define FAILURE 1
#define SUCCESS 0

// This is just to remove compiler warnings associated with unused variables.
// Delete calls to this as you implement functions.
#define UNUSED(x) ((void)(x))

/*
 *  student struct found in the user_data union
 *      num_classes is a integer for the number of classes a student is taking
 *      grades is a pointer to an array of doubles holding class grades
 * 
 *  Note: there should be a grade for each class, so num_classes is also the length of the grades array
 */
struct student
{
    int num_classes;
    double *grades;
};

/*
 *  instructor struct found in the user_data union
 *      salary is a double storing the salary of the teacher
 */
struct instructor
{
    double salary;
};

union user_data {
  struct student student;
  struct instructor instructor;
};

/*
 * enumeration for the type of user
 */
enum user_type {
    STUDENT,
    INSTRUCTOR
};

/*
 *  user struct is the datatype of the nodes in the circular singly-linked list data structure
 *      next is a pointer to the next node (of struct user type) in the data structure
 *      name is a char pointer to a string representing the name of the user
 *      type is the enumeration for the user
 *      data is the user_data that corresponds to the type of user (either student or instructor)
 */
struct user
{
    struct user *next;
    char *name;
    enum user_type type;
    union user_data data;
};

/*
 *  user_list is a struct that contains the meta-data for the entire circular singly-linked list data structure
 *      head is a pointer to the head node OR NULL if the list is empty 
 *      tail is a pointer to the tail node OR NULL if the list is empty
 *      size is an integer representing the length of the linked list
 */
struct user_list
{
    struct user *head;
    struct user *tail;
    int size;
};

/***************************************************
** Prototypes for struct list library functions.  **
**                                                **
** For more details on their functionality,       **
** check struct list.c.                           **
***************************************************/

/* Creating */
struct user_list *create_list(void);

/* Adding */
int push_front(struct user_list *list, char *name, enum user_type type, union user_data data);
int push_back(struct user_list *list, char *name, enum user_type type, union user_data data);
int add_at_index(struct user_list *list, int index, char *name, enum user_type type, union user_data data);

/* Querying */
int get(struct user_list *list, int index, struct user **dataOut);
int contains(struct user_list *list, struct user *data, struct user **dataOut);

/* Removing */
int pop_front(struct user_list *list, struct user **dataOut);
int pop_back(struct user_list *list, struct user **dataOut);
int remove_at_index(struct user_list *list, struct user **dataOut, int index);
void empty_list(struct user_list *list);

/* Analysis */
int get_highest_paid(struct user_list *list, struct user **dataOut);
int is_passing_all_classes(struct user_list *list, char *name, int *dataOut);
int end_semester(struct user_list *list);

#endif
