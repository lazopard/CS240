/**
 * list.c
 *
 * Define the functions used for a linked list.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "list.h"

int getOrderId (OrderPtr orderData) {
	return orderData->id;
}

/**
 * Return the side character stored in an order struct.
 */
char getOrderSide (OrderPtr orderData) {
	return orderData->side;
}

/**
 * Return the quantity stored in an order struct.
 */
int getOrderQty (OrderPtr orderData) {
	return orderData->quantity;
}

/**
 * Return the price stored in an order struct.
 */
double getOrderPrice (OrderPtr orderData) {
	return orderData->price;
}


/**
 * Return the stock symbol stored in an order struct.
 */
char* getOrderSymbol (OrderPtr orderData) {
	return orderData->symbol;
}


/**
 * Set the id in an order struct to be the value of id.
 */

void setOrderId (OrderPtr orderData, int id) {
	orderData->id = id;
}


/**
 * Set the side symbol in an order struct to be the value of side.
 */

void setOrderSide (OrderPtr orderData, char side) {
	orderData->side = side;
}


/**
 * Set the quantity in an order struct to be the value of newQty.
 */

void setOrderQty (OrderPtr orderData, int newQty) {
	orderData->quantity = newQty;
}

/**
 * Set the price in an order struct to be the value of newPrice.
 */


void setOrderPrice (OrderPtr orderData, double newPrice) {
	orderData->price = newPrice;
}

/**
 * Copy the string in newSymbol to the member symbol in an order struct.
 * The maximum length of the symbol is MAX_SYMBOL_LENGTH.
 */

void clearStr(char *string);

void setOrderSymbol (OrderPtr orderData, char* newSymbol) {
	clearStr(orderData->symbol);
	strcpy(orderData->symbol, newSymbol);
}

/** 
 * Return the next node of the given node. 
 * IMPORTANT: Return NULL if node is NULL. 
 */ 

NodePtr getNextNode (NodePtr node) {
	if (node == NULL) {
		return NULL;
	}
	return node->next;
}

/** 
 * Return the previous node of the given node. 
 * IMPORTANT: Return NULL if node is NULL. 
 */ 

NodePtr getPrevNode (NodePtr node) {
	if (node == NULL) {
		return NULL;
	}
	return node->prev;
}

//returns the data field of the node

OrderPtr getOrderData (NodePtr node) {
	if (node == NULL) {
		return NULL;
	}
	return node->data;
}


/**
 * Default print function. Custom printers should adhere to this format.
 */

void printOrderData(OrderPtr orderData, FILE *out){
		fprintf(out, "%d %c %s %d %f\n", 
						getOrderId(orderData), 
						getOrderSide(orderData),
						getOrderSymbol(orderData),
						getOrderQty(orderData), 
						getOrderPrice(orderData));
}

NodePtr newNode (OrderPtr data) {
	OrderPtr newOrderPtr = malloc(sizeof(struct order));
	setOrderId(newOrderPtr, getOrderId(data));
	setOrderSymbol(newOrderPtr, getOrderSymbol(data));
	setOrderSide(newOrderPtr, getOrderSide(data));
	setOrderQty(newOrderPtr, getOrderQty(data));
	setOrderPrice(newOrderPtr, getOrderPrice(data));
	NodePtr newNode = (NodePtr) malloc(sizeof(struct onode));
	newNode->data = newOrderPtr;
	newNode->prev = NULL;
	newNode->next = NULL;
	return newNode;
}

/** 
 * In a linked list with *head as the head pointer, add the given node to the front of the list. 
 */ 

void pushNode (NodePtr* head, NodePtr node) {
		if (head == NULL) {
				node->prev = NULL;
				(*head) = node;
				return;
		}
		(*head)->prev = node;
		node->next = (*head);
		(*head) = node;
		return;
}

/** 
 * In a linked list with *head as the hhNode(struct lnode** head, struct lnode* node) {
ad pointer, return the onode with the given order id. 
*/ 

NodePtr getOrderNode (NodePtr head, int id) {
	NodePtr currentNode = head;
	while(currentNode != NULL) {
		if(getOrderId(getOrderData(currentNode)) == id) {
			return currentNode;
		}
		currentNode = getNextNode(currentNode);
	}
	return NULL;
}

/*Insert the given node (insertingNode) after the node prevNode into the list with
 * head pointer head. If the prevNode is NULL, then the node insertingNode is inserted
 * at the front of the list.
 */

void insertNode (NodePtr* head, NodePtr prevNode, NodePtr insertingNode) {
	NodePtr currentNode = *head;
	while(currentNode != NULL) {
		if (currentNode == prevNode) {
			insertingNode->next = getNextNode(prevNode);
			prevNode->next = insertingNode;
			insertingNode->prev = prevNode;
			return;
		}
		currentNode = getNextNode(currentNode);
	}
}

/**
 * Evict the specified node from the list, but do not free its memory.
 * The node is guaranteed to be on the given linked list.
 */

void evictNode (NodePtr* head, NodePtr node) {
	if ((*head) == node) {
		head = &(node->next);
		(getNextNode(node))->prev = NULL;
		node->next = NULL;
		return;
	}
	if (getNextNode(node) == NULL) {
		(getNextNode(node))->next = NULL;
		node->prev = NULL;
		return;
	}
	(getPrevNode(node))->next = getNextNode(node);
	(getNextNode(node))->prev = getPrevNode(node);
	node->next = NULL;
	node->prev = NULL;
	return;
}

/**
 * Remove the specified node from the list, and free all memory the node is using.
 * Note: if *head points to node, then it should be updated to point to 
 * the new list head (or NULL if the list is empty).
 * The node is guaranteed to be on the given linked list.
 */

void deleteNode (NodePtr* head, NodePtr node) {
	if (head == NULL) {
		printf("invalid operation\n");
	}
	if (node == *head) {
		if (getNextNode(*head) == NULL) { 
			free(getOrderData(*head));
			free(*head);
			head = NULL;
			return;
		}
		(node->next)->prev = NULL;
		head = &(node->next);
		free(getOrderData(node));
		free(node);
		node = NULL;
		return;
	}
	evictNode(head, node);
	free(getOrderData(node));
	free(node);
	node = NULL;
	return;
}

/**
 * Swap nodes n1 and n2 in the linked list pointed to by *head.
 * Eg., if the list looks like: -- n1 -- n2 -- , then after the swapping, 
 * it should look like: -- n2 -- n1 --.
 * The nodes are guaranteed to be on the given linked list.
 */

void swapNodes (NodePtr* head, NodePtr n1, NodePtr n2) {
		if (n1 == n2) {
				return;
		}
		NodePtr temp1 = getPrevNode(n1);
		evictNode(head, n1);
		insertNode(head,getPrevNode(n2), n1);
		if (temp1 != n2) {
				evictNode(head, n2);
				insertNode(head, temp1, n2);
		}
		return;
}


void deleteList (NodePtr* head) {
	if (getNextNode(*head) == NULL) {
		free(getOrderData(*head));
		free(*head);
		head = NULL;
		return;
	}
	NodePtr currentNode = getNextNode(*head);
	NodePtr prevNode;
	while(currentNode != NULL) {
		prevNode = getPrevNode(currentNode);
		if (getNextNode(currentNode) == NULL) {
			free(getOrderData(currentNode));
			free(currentNode);
			currentNode = NULL;
			free(getOrderData(prevNode));
			free(prevNode);
			prevNode = NULL;
			head = NULL;
			return;
		}
		free(getOrderData(prevNode));
		free(prevNode);
		prevNode = NULL;
		currentNode = getNextNode(currentNode);;
	}
	return;
}

/**
 * Print the list to out starting from the parameter node.
 * The format to print a single item is defined by printItem.
 */

void printList (NodePtr node, void (*printItem)(OrderPtr, FILE *), FILE *out) {
	NodePtr currentNode = node;
	while(currentNode != NULL) {
		printItem(getOrderData(currentNode), out);
		currentNode = getNextNode(currentNode);
	}
}

void clearStr(char *string) {
	int i = 0;
	for(i; i < strlen(string); i++) {
		string[i] = '\0';
	}
}

/*
int main() {
	struct order newOrder1;
	newOrder1.id = 1;
	newOrder1.side = 'a';
	strcpy(newOrder1.symbol, "AAPL");
	newOrder1.quantity = 1;
	newOrder1.price = 1.1;
	NodePtr newNode1 = newNode(&newOrder1);
	NodePtr *head = NULL;
	pushNode(head, newNode1);
//	struct order newOrder2;
//	newOrder2.id = 2;
//	newOrder2.side = 'a';
//	strcpy(newOrder2.symbol, "AAPL");
//	newOrder2.quantity = 2;
//	newOrder2.price = 2.2;
//	NodePtr newNode2 = newNode(&newOrder2);
//	struct order newOrder3;
//	newOrder3.id = 3;
//	newOrder3.side = 'a';
//	strcpy(newOrder3.symbol, "AAPL");
//	newOrder3.quantity = 3;
//	newOrder3.price = 3.3;
//	NodePtr newNode3 = newNode(&newOrder3);
}

*/
