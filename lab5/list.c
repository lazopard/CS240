#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "list.h"

struct lnode {
  /* FILL THIS IN */
};

/**
 * Returns a new linked list node filled in with the given word and line, and
 * sets the count to be 1. Make sure to duplicate the word, as the original word
 * may be modified by the calling function.
 */
struct lnode* newNode (char* word, int line) {
  /* FILL THIS IN */
  abort(); // Please remove the abort() when you implement this function
}

/**
 * In a linked list with *head as the head pointer, adds the given node to the
 * front of the list.
 */
void pushNode (struct lnode** head, struct lnode* node) {
  /* FILL THIS IN */
  abort(); // Please remove the abort() when you implement this function
}


/**
 * Returns the pointer to the node containing the given word in the linked list
 * with head as the head pointer. If a node with the given word cannot be found,
 * the function returns NULL.
 */
struct lnode* getNode (struct lnode* head, char* word) {
  /* FILL THIS IN */
  abort(); // Please remove the abort() when you implement this function
}

/**
 * Removes the specified node from the list, and frees all memory the node is
 * using. Remember if *head is the node being deleted, it must be updated.
 */
void deleteNode (struct lnode** head, struct lnode* node);
  /* FILL THIS IN */
  abort(); // Please remove the abort() when you implement this function
}

/**
 * Simply returns the next node in the list, or NULL if there are no further nodes.
 */
struct lnode *nodeGetNext(struct lnode *node) {
    /* FILL THIS IN */
    abort(); // Please remove the abort() when you implement this function
}

/**
 * Simply returns the word in the given node.
 */
char *nodeGetWord(struct lnode *node) {
    /* FILL THIS IN */
    abort(); // Please remove the abort() when you implement this function
}

/**
 * Simply returns the line in the given node.
 */
int nodeGetLine(struct lnode *node) {
    /* FILL THIS IN */
    abort(); // Please remove the abort() when you implement this function
}

/**
 * Simply returns the count in the given node.
 */
int nodeGetCount(struct lnode *node) {
    /* FILL THIS IN */
    abort(); // Please remove the abort() when you implement this function
}

/**
 * Set the count in the node to be the given count.
 */
void nodeSetCount(struct lnode *node, int count) {
    /* FILL THIS IN */
    abort(); // Please remove the abort() when you implement this function
}

/**
 * Set the line in the node to be the given line.
 */
void nodeSetLine(struct lnode *node, int line) {
    /* FILL THIS IN */
    abort(); // Please remove the abort() when you implement this function
}


/**
 * Deletes every node in the list with *head as the head pointer. After calling
 * this function, all memory used by the list should be freed, and *head
 * should be NULL.
 */
void deleteList(struct lnode **head) {
    /* FILL THIS IN */
    abort(); // Please remove the abort() when you implement this function
}
