#include "linked_list.h"

// Function pointers to (potentially) custom malloc() and
// free() functions.
//
static void * (*malloc_fptr)(size_t size) = NULL;
static void   (*free_fptr)(void* addr)    = NULL; 

// (void)(malloc_fptr);
// (void)(free_fptr);



// Creates a new linked_list.
// PRECONDITION: Register malloc() and free() functions via the
//               linked_list_register_malloc() and
//               linked_list_register_free() functions.
// POSTCONDITION: An empty linked_list has its head point to NULL.
// Returns a new linked_list on success, NULL on failure.
//
struct linked_list * linked_list_create(void){

    //check if malloc_fptr is NULL
    if(malloc_fptr == NULL){
        return NULL;
    }

    //allocate linked_list
    struct linked_list *ll = (struct linked_list *)malloc_fptr(sizeof(struct linked_list));

    //exit if allocation wasn't successful
    if(ll == NULL){
        return NULL;
    }

    //if allocation successful, set head to NULL and return
    ll->head = NULL;

    return ll;
}

// Deletes a linked_list and frees all memory assoicated with it.
// \param ll : Pointer to linked_list to delete
// Returns TRUE on success, FALSE otherwise.
//
bool linked_list_delete(struct linked_list * ll){

    //check if input is NULL and free_fptr is NULL
    if(ll == NULL || free_fptr == NULL){
        return false;
    }

    //create pointers to head and next nodes
    //and free each node by iterating
    //through the entire list
    struct node* curr = ll->head;
    struct node* next = NULL;
    while(curr != NULL){
        //store next node
        next = curr->next;
        //free curr node
        free_fptr(curr);
        //traverse to next
        curr = next;
    }

    //finally, free the linked_list
    free_fptr(ll);

    return true;
}

// Returns the size of a linked_list.
// \param ll : Pointer to linked_list.
// Returns size on success, SIZE_MAX on failure.
//
size_t linked_list_size(struct linked_list * ll){

    //check if input is NULL
    if(ll == NULL){
        return SIZE_MAX;
    }

    //counter to hold linked_list size
    size_t size = 0;

    //iterate over the list and increment the counter
    struct node* curr = ll->head;
    while(curr != NULL){
        size++;
        curr = curr->next;
    }

    //return counter
    return size;

}

// Inserts an element at the end of the linked_list.
// \param ll   : Pointer to linked_list.
// \param data : Data to insert.
// Returns TRUE on success, FALSE otherwise.
//
bool linked_list_insert_end(struct linked_list * ll,
                            unsigned int data){

    //check if input is NULL and malloc_fptr is NULL
    if(ll == NULL || malloc_fptr == NULL){
        return false;
    }

    //iterate over the list uses a pointer to a pointer
    //helps avoid conditional branching in the edge case,
    //potentially optimization CPU pipeline instructions throughput
    struct node** curr = &ll->head;
    while(*curr != NULL){
        //pointer to the next pointer for traversal
        curr = &(*curr)->next;
    }

    //create new node
    struct node* new_node = (struct node*)malloc_fptr(sizeof(struct node));
    //exit if allocation wasn't successful
    if(new_node == NULL){
        return false;
    }
    //set the new_node data and next to NULL
    //as its the last node
    new_node->data = data;
    new_node->next = NULL;
    //set value of curr to new_node
    *curr = new_node;

    return true;
}

// Inserts an element at the front of the linked_list.
// \param ll   : Pointer to linked_list.
// \param data : Data to insert.
// Returns TRUE on success, FALSE otherwise.
//
bool linked_list_insert_front(struct linked_list * ll,
                              unsigned int data){

    //check if input is NULL and malloc_fptr is NULL
    if(ll == NULL || malloc_fptr == NULL){
        return false;
    }

    //create new node
    struct node* new_node = (struct node*)malloc_fptr(sizeof(struct node));
    //exit if allocation wasn't successful
    if(new_node == NULL){
        return false;
    }
    //set the new_node data and next to ll->head
    new_node->data = data;
    new_node->next = ll->head;
    //update ll->head
    ll->head = new_node;

    return true;
}

// Inserts an element at a specified index in the linked_list.
// \param ll    : Pointer to linked_list.
// \param index : Index to insert data at.
// \param data  : Data to insert.
// Returns TRUE on success, FALSE otherwise.
//
bool linked_list_insert(struct linked_list * ll,
                        size_t index,
                        unsigned int data){

    //check if input is NULL and malloc_fptr/free_fptr are NULL
    if(ll == NULL || malloc_fptr == NULL || free_fptr == NULL){
        return false;
    }

    //create new node
    struct node* new_node = (struct node*)malloc_fptr(sizeof(struct node));
    //exit if allocation wasn't successful
    if(new_node == NULL){
        return false;
    }

    //counter to track current index
    size_t curr_idx = 0;

    //iterate over the list uses a pointer to a pointer
    //helps avoid conditional branching in the edge case,
    //potentially optimization CPU pipeline instructions throughput
    struct node** curr = &ll->head;
    while(*curr != NULL && curr_idx < index){
        curr = &(*curr)->next;
        curr_idx++;
    }

    //not enough elements in list
    if(curr_idx != index){
        free_fptr(new_node);
        return false;
    }
    //set the node data
    new_node->data = data;
    //set new_node->next to *curr
    new_node->next = *curr;
    //set value of curr to new_node
    *curr = new_node;

    return true;
}

// Finds the first occurrence of data and returns its index.
// \param ll   : Pointer to linked_list.
// \param data : Data to find.
// Returns index of the first index with that data, SIZE_MAX otherwise.
//
size_t linked_list_find(struct linked_list * ll,
                        unsigned int data){

    //if input is NULL then return SIZE_MAX
    if(ll == NULL){
        return SIZE_MAX;
    }

    //counter to track current index
    size_t curr_idx = 0;

    //iterate over the list and return if a match is found
    struct node* curr = ll->head;
    while(curr != NULL){
        if(curr->data == data){
            return curr_idx;
        }
        curr = curr->next;
        curr_idx++;
    }

    //no match found so return SIZE_MAX
    return SIZE_MAX;
}

// Removes a node from the linked_list at a specific index.
// \param ll    : Pointer to linked_list.
// \param index : Index to remove node.
// Returns TRUE on success, FALSE otherwise.
//
bool linked_list_remove(struct linked_list * ll,
                        size_t index){

    //check if input is NULL and free_fptr is NULL
    if(ll == NULL || free_fptr == NULL){
        return false;
    }

    //counter to track current index
    size_t curr_idx = 0;

    //iterate over the list till we hit index
    struct node** curr = &ll->head;
    // struct node* prev = NULL;
    while(*curr != NULL && curr_idx < index){
        curr = &(*curr)->next;
        curr_idx++;
    }

    //not enough elements in list
    if(curr_idx != index || *curr == NULL){
        return false;
    }

    //curr is a pointer that points to the node to be deleted
    //create a temp pointer to the actual node
    struct node* temp = *curr;
    //update the pointer to skip the node being deleted
    *curr = (*curr)->next;

    //free the temp pointer
    free_fptr(temp);

    return true;
}

// Creates an iterator struct at a particular index.
// \param linked_list : Pointer to linked_list.
// \param index       : Index of the linked list to start at.
// Returns pointer to an iterator on success, NULL otherwise.
//
struct iterator * linked_list_create_iterator(struct linked_list * ll,
                                              size_t index){

    //check if input is NULL and malloc_fptr/free_fptr are NULL
    if(ll == NULL || malloc_fptr == NULL || free_fptr == NULL){
        return NULL;
    }

    //allocate iterator
    struct iterator* iterator = malloc_fptr(sizeof(struct iterator));
    //exit if allocation wasn't successful
    if(iterator == NULL){
        return NULL;
    }

    //counter for index tracker
    size_t curr_idx = 0;

    //traverse to node to set the iterator
    struct node* curr = ll->head;
    while(curr != NULL && curr_idx < index){
        curr = curr->next;
        curr_idx++;
    }

    //not enough elements in list
    if(curr_idx != index || curr == NULL){
        //free allocated iterator
        free_fptr(iterator);
        return NULL;
    }

    //set the iterator fields
    iterator->ll = ll;
    iterator->current_node = curr;
    iterator->current_index = index;
    iterator->data = curr->data;

    //return the iterator
    return iterator;
}

// Deletes an iterator struct.
// \param iterator : Iterator to delete.
// Returns TRUE on success, FALSE otherwise.
//
bool linked_list_delete_iterator(struct iterator * iter){

    //check if input iter is NULL and free_fptr is is NULL
    if(iter == NULL || free_fptr == NULL){
        return false;
    }

    //free the input iterator
    free_fptr(iter);

    return true;
}

// Iterates to the next node in the linked_list.
// \param iterator: Iterator to iterate on.
// Returns TRUE when next node is present, FALSE once end of list is reached.
//
bool linked_list_iterate(struct iterator * iter){

    //check if input iterator, iter->current_node and iter->current_node->next are NULL
    if(iter == NULL || iter->current_node == NULL || iter->current_node->next == NULL){
        return false;
    }

    //update iterator to the next node
    iter->current_node = iter->current_node->next;
    iter->current_index++;
    iter->data = iter->current_node->data;

    return true;
}

// Registers malloc() function.
// \param malloc : Function pointer to malloc()-like function.
// Returns TRUE on success, FALSE otherwise.
//
bool linked_list_register_malloc(void * (*malloc)(size_t)){

    //exit if input is NULL
    if(malloc == NULL){
        return false;
    }

    //set malloc_fptr to input function ptr and return
    malloc_fptr = malloc;
    return true;
}

// Registers free() function.
// \param free : Function pointer to free()-like function.
// Returns TRUE on success, FALSE otherwise.
//
bool linked_list_register_free(void (*free)(void*)){

    //exit if input is NULL
    if(free == NULL){
        return false;
    }
    //set free_fptr to input function ptr and return
    free_fptr = free;
    return true;
}