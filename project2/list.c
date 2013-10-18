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
	strncpy(orderData->symbol, newSymbol, MAX_SYMBOL_LENGTH);
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
	int newId, newQuantity, i;
	char newSide;
	double newPrice;
	newId = data->id;
	newQuantity = data->quantity;
	newSide = data->side;
	newPrice = data->price;
	OrderPtr newData = malloc(sizeof(data));
	newData->id = newId;
	strcpy(newData->symbol, data->symbol);
	newData->side = newSide;
	newData->quantity = newQuantity;
	newData->price = newPrice;
	NodePtr newNode = malloc(sizeof(NodePtr));
	newNode->data = newData;
	newNode->prev = NULL;
	newNode->next = NULL;
	return newNode;
}

/** 
 * In a linked list with *head as the head pointer, add the given node to the front of the list. 
 */ 

void pushNode (NodePtr* head, NodePtr node) {

		if ((head) == NULL) {
				head = &node;
				return;
		}
		node->next = (*head);
		head = &node;
		(*head)->next->prev = (*head);
		return;
}

/** 
 * In a linked list with *head as the hhNode(struct lnode** head, struct lnode* node) {
ad pointer, return the onode with the given order id. 
*/ 

NodePtr getOrderNode (NodePtr head, int id) {
	NodePtr currentNode = head;
	while(currentNode != NULL) {
		if(currentNode->data->id == id) {
			return currentNode;
		}
		currentNode = currentNode->next;
	}
	return NULL;
}

/** 
 * Return the next node of the given node. 
 * IMPORTANT: Return NULL if node is NULL. 
 */ 

NodePtr getNextNode (NodePtr node) {
	return node->next;
}

/** 
 * Return the previous node of the given node. 
 * IMPORTANT: Return NULL if node is NULL. 
 */ 

NodePtr getPrevNode (NodePtr node) {
	return node->prev;
}

OrderPtr getOrderData (NodePtr node) {
	return node->data;
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
	NodePtr currentNode = *head;
	while(currentNode != NULL) {
		if (currentNode == node) {
			node->prev->next = node->next;
			node->next->prev = node->prev;
			return;
		}
	}
}

/**
 * Remove the specified node from the list, and free all memory the node is using.
 * Note: if *head points to node, then it should be updated to point to 
 * the new list head (or NULL if the list is empty).
 * The node is guaranteed to be on the given linked list.
 */

void deleteNode (NodePtr* head, NodePtr node) {
	if (node == *head) {
		if ((*head)->next == NULL) {
			head = NULL;
			return;
		}
		head = &node->next;
	}
	evictNode(head, node);
	free(node->data);
	free(node);
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
	NodePtr currentNode = *head;
	while(currentNode != NULL) {
		currentNode = getPrevNode(currentNode);
		free(getPrevNode(currentNode)->data);
		free(getPrevNode(currentNode));
	}
	return;
}

/**
 * Print the list to out starting from the parameter node.
 * The format to print a single item is defined by printItem.
 */

void printList (NodePtr node, void (*printItem)(OrderPtr, FILE *), FILE *out) {
	while(node != NULL) {
		printItem(node->data, out);
		node = node->next;
	}
}

void printListStdOut(NodePtr *head) {
	NodePtr currentNode = *head;
	while(currentNode != NULL) {
		currentNode = currentNode->next;
	}
}

void clearStr(char *string) {
	int i = 0;
	for(i; i < strlen(string); i++) {
		string[i] = '\0';
	}
}
