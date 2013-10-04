#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "list.h"

struct lnode {
	char *word; 
	int line;
	int count;
	struct lnode *next;
}; 

/*
 * Returns a new linked list node filled in with the given word and line, and
 * sets the count to be 1. Make sure to duplicate the word, as the original word
 * may be modified by the calling function.
 */

struct lnode* newNode (char* word, int line) {
  char *alword; 
  alword = (char *) malloc(sizeof(char)*(strlen(word) + 1));
  alword = strcpy(alword,word);
  struct lnode *newNode;
  newNode = (struct lnode*) malloc(sizeof(struct lnode));
  if (newNode == NULL) {
  	return NULL;
  }
  newNode->word = alword;
  newNode->line = line;
  newNode->count = 1;
  newNode->next = NULL;
  return newNode;
}

/**
 * In a linked list with *head as the head pointer, adds the given node to the
 * front of the list.
 */

//make node the head, make *head point to node, give node the value of head

void pushNode(struct lnode** head, struct lnode* node) {
	struct lnode *temp = (struct lnode*) malloc(sizeof(struct lnode));
	*temp = **head;
	node->next = *head;
	*head = node;
	node = temp;
	free(temp);
}

/**
 * Returns the pointer to the node containing the given word in the linked list
 * with head as the head pointer. If a node with the given word cannot be found,
 * the function returns NULL.
 */

struct lnode* getNode(struct lnode *head, char* word) {
	struct lnode *ptr = (struct lnode*) malloc(sizeof(struct lnode));
	for(ptr = head; ptr->word != word && ptr != NULL;ptr=ptr->next) {
		printf("%s",ptr->word);
	}
	if (ptr == NULL) {
		free(ptr);
		return NULL;
	}
	free(ptr);
	return ptr;
}

/**
 * Removes the specified node from the list, and frees all memory the node is
 * using. Remember if *head is the node being deleted, it must be updated.
 */

void deleteNode(struct lnode **head, struct lnode* node) {
	/*if (*head == node) {
		pushNode(head,*head->next);
		*head->next = *head->next->next;
		free(node);
		return;
	}
	struct lnode *ptr = (struct lnode*) malloc(sizeof(struct lnode));
	for(ptr = *head; ptr != NULL; ptr = ptr->next) {
		if (ptr == node) {
			(ptr - 1)->next = (ptr + 1);
		}
	}
	free(ptr);
	*/
	abort();
}

/**
 * Simply returns the next node in the list, or NULL if there are no further nodes.
 */

struct lnode *nodeGetNext(struct lnode *node) {
    return node->next;
}

/**
 * Simply returns the word in the given node.
 */

char *nodeGetWord(struct lnode *node) {
    return node->word;
}

/**
 * Simply returns the line in the given node.
 */

int nodeGetLine(struct lnode *node) {
    return node->line;
}

/**
 * Simply returns the count in the given node.
 */

int nodeGetCount(struct lnode *node) {
    return node->count;
}

/**
 * Set the count in the node to be the given count.
 */

void nodeSetCount(struct lnode *node, int count) {
	node->count = count;
}

/**
 * Set the line in the node to be the given line.
 */

void nodeSetLine(struct lnode *node, int line) {
	node->line = line;
}


/**
 * Deletes every node in the list with *head as the head pointer. After calling
 * this function, all memory used by the list should be freed, and *head
 * should be NULL.
 */

void deleteList(struct lnode **head) {
	struct lnode *ptr = (struct lnode*) malloc(sizeof(struct lnode));
	for(ptr = *head; ptr != NULL; ptr = ptr->next) {
		free(ptr);			
	}
	head = NULL;
}

void printList(struct lnode **head) {
	struct lnode *temp = malloc(sizeof(struct lnode));
	temp = *head;
	while(temp != NULL) {
		printf("node.word is %s\n",temp->word);
		temp = temp->next;
	}
}

int main() {
	struct lnode *inhead = newNode("node1",1);
	struct lnode *node = newNode("node2",2);
	struct lnode *node2 = newNode("node3",3);
	pushNode(&inhead,node);
	pushNode(&inhead,node2);
	printList(&inhead);
}
