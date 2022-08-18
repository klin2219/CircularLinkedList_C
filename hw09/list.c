/**
 * CS 2110 - Spring 2022 - Homework 9
 *
 * @author Kevin Lin
 *
 * list.c: Complete the functions!
 */

/**
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!-IMPORTANT-!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * For any function that must use malloc, if malloc returns NULL, the function
 * itself should return FAILURE if it returns an int or NULL otherwise
 */

// Do not add ANY additional includes!!!
#include "list.h"

/* You should NOT have any global variables. */

static struct user *create_user(char *name, enum user_type type, union user_data data);
static int create_student(int num_classes, double *grades, struct student *dataOut); //completed for you
static int create_instructor(double salary, struct instructor *dataOut); //completed for you
static int user_equal(const struct user *user1, const struct user *user2);

/** create_user
 *
 * Helper function that creates a struct user by allocating memory for it on the heap
 * and initializing with the passed in data.
 * 
 * When we malloc, sizeof some struct it will make enough space for everything that the struct contains.
 * If you refer to the diagram in the pdf, you will see that some information is contained within the struct
 * while other information in the struct is a pointer to space elsewhere on the heap.
 * That means after mallocing a struct, check if there are any pointers within it, that need to be 
 * malloced for additional space. You will notice they are distinctly placed on the diagram.
 * Hint: if you make use of the helper functions mentioned later in this comment, then you should
 * only have to malloc one additional pointer (in addition to mallocing the `struct user`)
 * 
 * It is permitted for the user's name to be NULL, if that is passed into the function.
 * In those cases, leave name as a pointer to NULL.
 * Otherwise, if we get a valid string for the name, dynamically allocate space for that string.
 * Recall our rules for string functions, notably that strlen does NOT include the null terminator
 * However, we must null terminate all strings. Therefore, you need to malloc space for the strlen 
 * in addition to the null terminator.
 * 
 * You MUST create a deep copy of the data union. This means malloc'ing space for copies of each field.
 * You should call create_student() or create_instructor() in this function
 * to create a deep copy of the union user_data depending on the enum user_type
 *
 * If malloc returns NULL, this function should return NULL as well.
 *
 * If creating a student/instructor fails, you must free everything you've allocated
 * and return NULL
 *
 * (hint: you cannot just assign the parameter data to the data part of user)
 *
 * Remember to set the next pointer to NULL.
 *
 * @param the fields of the struct user struct
 * @return a struct user, return NULL if malloc fails
 */
static struct user *create_user(char *name, enum user_type type, union user_data data)
{
    struct user *user = malloc(sizeof(struct user));
    if (!user) { free(user); return NULL; }

    // Allocate space for the name and perform a copy
    if (!name) { user->name = NULL; }
    else {
        user->name = malloc(strlen(name) + 1);
        if (!user->name) { free(user); return NULL; }
        strncpy(user->name, name, strlen(name) + 1);
    }
    user->type = type;

    // Allocate space for the data
    if (type == STUDENT) {
        if (create_student(data.student.num_classes, data.student.grades, &user->data.student) == FAILURE) { 
            free(user->name); free(user); return NULL;
        }
    }
    else if (type == INSTRUCTOR) {
        if (create_instructor(data.instructor.salary, &user->data.instructor) == FAILURE) {
            free(user->name); free(user); return NULL;
        }
    }

    // Set next pointer to null
    user->next = NULL;
    return user;
}

/** create_student
 *
 * Helper function that creates a struct student and allocates memory for the grade array on the heap.
 * 
 * @param num_classes int for the number of classes a student is taking.
 * @param grades a pointer to array of doubles containing the student's grades.
 * @param dataOut a pointer to an existing heap allocation of a student struct.
 * Note:  Refer to the pdf if you are unsure about the dataOut technique
 * 
 * @return FAILURE if malloc fails, SUCCESS otherwise.
 */
static int create_student(int num_classes, double *grades, struct student *dataOut)
{
    /***do not change anything in this function***/
    dataOut->num_classes = num_classes;
    dataOut->grades = NULL;
    if (grades != NULL) {
        if (!(dataOut->grades = (double *) malloc(sizeof(double)*num_classes))) return FAILURE;
        memcpy(dataOut->grades, grades, sizeof(double)*num_classes);
    }
    return SUCCESS;
}

/** create_instructor
 *
 * Helper function that creates an struct instructor.
 *
 * @param salary a double indicating the salary of the instructor.
 * @param dataOut a pointer to an existing heap allocation of a student struct.
 * Note:  Refer to the pdf if you are unsure about the dataOut technique
 * 
 * @return always SUCCESS, since there is no malloc call
 */
static int create_instructor(double salary, struct instructor *dataOut)
{
    /***do not edit anything in this function***/
    dataOut->salary = salary;
    return SUCCESS;
}

/** student_equal
 *
 * Helper function to compare two struct students. You may find it useful to call this helper
 * function from the user_equal function that you have to implement.
 * 
 * If both students
 *
 * @param student1 a pointer to the first student to be compared
 * @param student2 a pointer to the second student to be compared
 * 
 * @return 1 if equal, 0 otherwise
 */
static int student_equal(const struct student *student1, const struct student *student2)
{
    if (student1->num_classes != student2->num_classes) return 0;
    if (student1->grades != student2->grades)
    {
        if (student1->grades == NULL || student2->grades == NULL) return 0;
        if (memcmp(student1->grades, student2->grades,
                student1->num_classes * sizeof(double)))
        {
            return 0;
        }
    }
    return 1;
}

/** user_equal
 * Helper function to help you compare two user structs.
 *
 * If the name, type, and union fields are all equal, you should return 1.
 * Otherwise, return 0.
 * 
 * Make sure to handle the case if both pointers are the same, then they are equal.
 * If both user pointers are null, then they are equal too
 *
 * NOTE: struct members that are pointers may be null!
 * If two users both have null for a certain pointer, the pointers are
 * considered equal.
 * However, if only one of the pointers is NULL, you should return 0.
 * ie: if both names are NULL pointers, then they are EQUAL
 *
 * You may find it useful to call the student_equal function defined above.
 * Otherwise, if the type is instructor just compare the instructor's salary
 *
 * Make sure you're using the right function when comparing the name.
 * Remember that you are allowed to use functions from string.h
 *
 * This is meant to be used as a helper function in 'contains'; it is not tested
 * by the autograder.
 *
 * @param the two struct user structs to be compared
 * @return 1 if equal, 0 otherwise
 */
static int user_equal(const struct user *user1, const struct user *user2)
{
    if (!user1 && !user2) { return 1; }
    if ((!user1 && user2) || (user1 && !user2)) { return 0; }

    if (user1->type != user2->type) { return 0; }
    if (student_equal(&user1->data.student, &user2->data.student) == 1) { return 1; }
    return 0;
}

/** create_list
 *
 * Creates a struct user_list by allocating memory for it on the heap.
 * Be sure to initialize size to zero, head and tail to NULL.
 *
 * If malloc returns NULL, you should return NULL to indicate failure.
 *
 * @return a pointer to a new struct list or NULL on failure
 */
struct user_list *create_list(void)
{
    struct user_list *list = malloc(sizeof(struct user_list));
    if (!list) { free(list); return NULL; }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

/** push_front
 *
 * Adds the element at the front of the struct user_list.
 *
 * @param list a pointer to the struct user_list structure.
 * @param      the fields of the struct user struct
 * @return FAILURE if the struct user_list is NULL or if allocating the new user fails,
 * SUCCESS if successful.
 */
int push_front(struct user_list *list, char *name, enum user_type type, union user_data data)
{
    if (!list) { return FAILURE; }
    struct user *newNode = create_user(name, type, data);
    //struct user *newNode = create_user(list->head->name, list->head->type, list->head->data);
    if (!newNode) { return FAILURE; }

    // Account for empty list case
    if (list->head == NULL || list->tail == NULL || list->size == 0) {
        list->head = newNode;
        list->tail = newNode;
        list->tail->next = list->head;
    } else {
        
        list->tail->next = newNode;
        newNode->next = list->head;
        list->head = list->tail->next;

       /*
       newNode->next = list->head->next;
       list->head->next = newNode;
       list->head->name = name;
       list->head->type = type;
       list->head->data = data;
       */
    }
    list->size++;
    return SUCCESS;
}

/** push_back
 *
 * Adds the element to the back of the struct user_list.
 *
 * @param list a pointer to the struct user_list structure.
 * @param      the fields of the struct user struct
 * @return FAILURE if the struct user_list is NULL
 *         or malloc fails (do not add the data in this case)
 *         otherwise return SUCCESS
 */
int push_back(struct user_list *list, char *name, enum user_type type, union user_data data)
{
    int condition = push_front(list, name, type, data);
    if (condition == FAILURE) { return FAILURE; }
    list->tail = list->head;
    list->head = list->head->next;
    return SUCCESS;
}

/** add_at_index
 *
 * Add the element at the specified index in the struct user_list. This index must lie in
 * the inclusive range [0,size].
 * For example, if you have no elements in the struct user_list,
 * you should be able to add to index 0 but no further.
 * If you have two elements in the struct user_list,
 * you should be able to add to index 2 but no further.
 *
 * @param list a pointer to the struct user_list structure
 * @param index 0-based, starting from the head in the inclusive range
 *              [0,size]
 * @param the fields of the struct user struct
 * @return FAILURE if the index is out of bounds or the struct user_list is NULL
 *         or malloc fails (do not add the data in this case)
 *         otherwise return SUCCESS
 */
int add_at_index(struct user_list *list, int index, char *name, enum user_type type, union user_data data)
{
    if (!list || (index < 0 || index > list->size)) { return FAILURE; }
    if (index == 0) { return push_front(list, name, type, data); }
    else if (index == list->size) { return push_back(list, name, type, data); }
    else {
        struct user *curr = list->head;
        int counter = 0;
        while (counter < index - 1) {
            counter++;
            curr = curr->next;
        }
        struct user *newNode = create_user(name, type, data);
        newNode->next = curr->next;
        curr->next = newNode;
        (list->size)++;
    }
    return SUCCESS;
}

/** get
 *
 * Gets the data at the specified index in the struct user_list
 *
 * @param list a pointer to the struct user_list structure
 * @param index 0-based, starting from the head.
 * @param dataOut A pointer to a pointer used to return the data from the
 *        specified index in the struct user_list or NULL on failure.
 * @return FAILURE if dataOut is NULL or index is out of range of the struct user_list or
 *         the struct user_list is NULL, SUCCESS otherwise
 */
int get(struct user_list *list, int index, struct user **dataOut)
{
    if (!list || (index < 0 || index >= list->size) || !dataOut) { return FAILURE; }

    int counter = 0;
    struct user *curr = list->head;
    while (counter != index) {
        curr = curr->next;
        counter++;
    }
    *dataOut = curr;

    return SUCCESS;
}

/** contains
  *
  * Traverses the struct user_list, trying to see if the struct user_list contains some
  * data. We say the list contains some input if there exists some user with
  * equal data as the input.
  *
  * You should use 'user_equal' here to compare the data. Note that pointers are
  * allowed to be null!
  *
  * If there are multiple pieces of data in the struct user_list which are equal to
  * the "data" parameter, return the one at the lowest index.
  *
  *
  * @param list a pointer to the struct user_list structure
  * @param data The data, to see if it exists in the struct user_list
  * @param dataOut A pointer to a pointer used to return the data contained in
  *                the struct user_list or NULL on failure
  * @return int    FAILURE if dataOut is NULL, the list is NULL, or the list
  *                does not contain data, else SUCCESS
  */
int contains(struct user_list *list, struct user *data, struct user **dataOut)
{
    if (!list || !dataOut) { return FAILURE; }
    struct user *curr = list->head;
    for (int i = 0; i < list->size; i++) {
        if (user_equal(curr, data)) {
            *dataOut = curr;
            return SUCCESS;
        }
        curr = curr->next;
    }
    *dataOut = NULL;
    return FAILURE;
}

/** pop_front
  *
  * Removes the user at the front of the struct user_list, and returns its data.
  * We are returning data to be used by the caller, so do NOT free it from the heap.
  * Rather, we just make sure the linked list does not reference it anymore.
  *
  * @param list a pointer to the struct user_list.
  * @param dataOut A pointer to a pointer used to return the data in the first
  *                struct user or NULL if the struct user_list is NULL or empty
  * @return FAILURE if dataOut is NULL (the struct user_list is NULL or empty),
  *         else SUCCESS
  */
int pop_front(struct user_list *list, struct user **dataOut)
{
    if (!dataOut || !list || list->size == 0) { return FAILURE; }
    *dataOut = list->head;
    
    if (list->size == 1) {
        list->head = NULL;
        list->tail = NULL;
    } else {
        list->tail->next = list->head->next;
        list->head = list->tail->next;
    }
    list->size--;
    return SUCCESS;
}

/** pop_back
  *
  * Removes the user at the back of the struct user_list, and returns its data.
  * We are returning data to be used by the caller, so do NOT free it from the heap.
  * Rather, we just make sure the linked list does not reference it anymore.
  *
  * @param list a pointer to the struct user_list.
  * @param dataOut A pointer to a pointer used to return the data in the last
  *                struct user or NULL if the struct user_list is NULL or empty
  * @return FAILURE if dataOut is NULL (the struct user_list is NULL or empty),
  *         else SUCCESS
  */
int pop_back(struct user_list *list, struct user **dataOut)
{
    if (!dataOut || !list || list->size == 0) { return FAILURE; }
    *dataOut = list->tail;

    if (list->size == 1) {
        list->head = NULL;
        list->tail = NULL;
    } else {
        struct user *curr = list->head;
        for (int i = 0; i < (list->size) - 2; i++) {
            curr = curr->next;
        }
        curr->next = list->head;
        list->tail = curr;
        /*
        list->tail = curr;
        list->tail->next = list->
        */
    }
    list->size--;
    return SUCCESS;
}


/** remove_at_index
 *
 * Remove the element at the specified index in the struct user_list.
 * We are returning data to be used by the caller, so do NOT free it from the heap.
 * Rather, we just make sure the linked list does not reference it anymore.
 *
 * @param list a pointer to the struct user_list structure
 * @param dataOut A pointer to a pointer used to return the data in the 
 *                struct user at index or NULL if the struct user_list is NULL or empty
 * @param index 0-based, starting from the head in the inclusive range
 *              [0,size-1]
 * @return FAILURE if the index is out of bounds, the struct user_list is NULL or
 *         the dataOut is NULL
 *         otherwise return SUCCESS
 */
int remove_at_index(struct user_list *list, struct user **dataOut, int index)
{
    if (!list || !dataOut || (index < 0 || index >= list->size)) { return FAILURE; }

    if (index == 0) { pop_front(list, dataOut); }
    else if (index == (list->size) - 1) { pop_back(list, dataOut); } 
    else {
        struct user *curr = list->head;
        for (int i = 0; i < index - 1; i++) {
            curr = curr->next;
        }
        *dataOut = curr->next;
        curr->next = curr->next->next;
        list->size--;
    }
    return SUCCESS;
}

/** empty_list
 *
 * Empties the struct user_list.
 * After this is called, the struct user_list should be
 * empty, and the space on the heap should be de-allocated.
 * This does NOT free the meta-data `struct user_list struct`
 * itself, but rather just all `struct user` nodes and data within.
 * 
 * You should free pointers from the inside out. That means before
 * freeing anything. Check that it doesn't contain any heap allocated
 * pointers with in it. If we free a pointer to an allocation,
 * that had more pointers to the heap within it, then those are lost
 * forever, and there will be a memory leak. Using the diagram provided
 * might help you visualize these scenarios.
 * 
 * Make sure to check that the list is not NULL before using it
 *
 * You may call free on a char pointer as well as a struct pointer.
 *
 * If 'list' is NULL, don't do anything.
 *
 * @param list a pointer to the struct user_list structure
 */
void empty_list(struct user_list *list)
{
    if (!list || list->size == 0) { return; }
    struct user *curr = list->head;

    // Clear each and every node
    for (int i = 0; i < list->size; i++) {
        free(curr->name);
        if (curr->type == STUDENT) { free(curr->data.student.grades); }
        struct user *nxt = curr->next;
        free(curr);
        curr = nxt;
    }
    // Reset the list
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

/** get_highest_paid
 *
 * Traverses the struct user_list, determining the highest of all instructor salaries and
 * returning the struct user corresponding to the highest paid instructor.
 *
 * If there are multiple highest paid instructors in the list, return the one that shows up
 * earlier in the list (closer to head). Hint: use the > operator for your comparisons.
 *
 * You should make sure your code only looks at instructors, as students do not have salaries!
 *
 * If there are no instructors in the list, place NULL at dataOut.
 *
 * @param list a pointer to the struct user_list structure
 * @param dataOut A pointer to pointer to struct user used to return the highest paid instructor.
 *                  or NULL if there are no instructors or the list is NULL or empty
 * @return FAILURE if the struct user_list is NULL or empty, else SUCCESS
 */
int get_highest_paid(struct user_list *list, struct user **dataOut)
{
    if (!list || list->size == 0) { *dataOut = NULL; return FAILURE; }
    double max = list->head->data.instructor.salary;
    struct user *curr = list->head;
    struct user *targetPos = NULL;
    int count = 0;

    for (int i = 0; i < list->size; i++) {
        if (curr->type == INSTRUCTOR && curr->data.instructor.salary > max) {
            max = curr->data.instructor.salary;
            targetPos = curr;
            count++;
        }
        curr = curr->next;
    }

    if (count == 0) { *dataOut = NULL; }
    else { *dataOut = targetPos; }

    return SUCCESS;
}


/** is_passing_all_classes
 *
 * Traverses the struct user_list, finding the student with the given name and 
 * determining whether they're passing all their classes.
 * 
 * We say a student is passing a class if their grade is greater than 
 * or equal to 60.
 *
 * You should make sure your code only considers students, even if there is 
 * an instructor of the same name!
 *
 * If a student is not taking any classes, they are NOT considered to be passing.
 *
 * @param list a pointer to the struct user_list structure
 * @param dataOut A pointer to int used to return whether or not the student 
 *                is passing their classes. Insert 1 if they are passing, 
 *                0 if they are not, or -1 if the user_list is NULL or empty 
 *                or does not contain the student with the given name.
 * @return FAILURE if the struct user_list is NULL or empty or does not contain
 *                 the student name passed in as a parameter, else SUCCESS
 */
int is_passing_all_classes(struct user_list *list, char *name, int *dataOut)
{
    if (!list || list->size == 0) { *dataOut = -1; return FAILURE; }

    struct user *curr = list->head;
    int found = 0;

    for (int i = 0; i < list->size; i++) {
        if (curr->type == STUDENT && strcmp(curr->name, name) == 0) {
            found = 1;
            if (curr->data.student.num_classes > 0) {
                int j = 0;
                while (j < curr->data.student.num_classes) {
                    if (curr->data.student.grades[j] < 60) { *dataOut = 0; return SUCCESS; }
                    j++;
                }
                *dataOut = 1;
                return SUCCESS;
            } else {
                *dataOut = 0;
                //return SUCCESS;
            }
        }
        curr = curr->next;
    }
    if (found) { return SUCCESS; }
    *dataOut = -1;
    return FAILURE;
}

/** end_semester
 *
 * Traverses the struct user_list, performing an operation on each user.
 *
 * For students:
 *    - All classes should be dropped (num_classes = 0)
 *    - The grades array should be updated accordingly (set to NULL)
 *    - You should not leak memory while doing this
 * For instructors:
 *    - You should add 10000 to their current salary.
 *
 * @param list a pointer to the struct user_list structure
 * @return FAILURE if the struct user_list is NULL or empty,
 *                else SUCCESS
 */
int end_semester(struct user_list *list)
{
    if (!list || list->size == 0) { return FAILURE; }

    struct user *curr = list->head;
    for (int i = 0; i < list->size; i++) {
        if (curr->type == STUDENT) {
            curr->data.student.num_classes = 0;
            free(curr->data.student.grades);
            curr->data.student.grades = NULL;
        } else {
            (curr->data.instructor.salary) += 10000;
        }
        curr = curr->next;
    }

    return SUCCESS;
}
