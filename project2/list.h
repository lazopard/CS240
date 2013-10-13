/**
 * list.h
 *
 * Define the data type onode to store a node in a linked list, and
 * declare the functions needed to manipulate a linked list.
 */

/**
 * TODO: In file called list.c, implement all the functions listed in this header,
 * except for printOrderData, which is already done.
 */

#ifndef LIST_H_

	#define LIST_H_
	
	#define MAX_SYMBOL_LENGTH 7		//including the terminator
	
	/**
	 * Define the struct to store the properties of one order.
	 */
	struct order {
		int id;
		char symbol[MAX_SYMBOL_LENGTH];
		char side;
		int quantity;
		double price;
	};
	
	typedef struct order* OrderPtr;
	typedef struct onode* NodePtr;
	
	/**
	 * Define the structure for a node.
	 * Double link implementation.
	 */
	struct onode {
		OrderPtr data;
		NodePtr next;
		NodePtr prev;
	};
	
	/**
	 * Return the id stored in an order struct.
	 */
	int getOrderId (OrderPtr orderData);

	/**
	 * Return the side character stored in an order struct.
	 */
	char getOrderSide (OrderPtr orderData);
	
	/**
	 * Return the quantity stored in an order struct.
	 */
	int getOrderQty (OrderPtr orderData);
	
	/**
	 * Return the price stored in an order struct.
	 */
	double getOrderPrice (OrderPtr orderData);
	
	/**
	 * Return the stock symbol stored in an order struct.
	 */
	char* getOrderSymbol (OrderPtr orderData);
	
	/**
	 * Set the id in an order struct to be the value of id.
	 */
	void setOrderId (OrderPtr orderData, int id);

	/**
	 * Set the side symbol in an order struct to be the value of side.
	 */
	void setOrderSide (OrderPtr orderData, char side);
	
	/**
	 * Set the quantity in an order struct to be the value of newQty.
	 */
	void setOrderQty (OrderPtr orderData, int newQty);
	
	/**
	 * Set the price in an order struct to be the value of newPrice.
	 */
	void setOrderPrice (OrderPtr orderData, double newPrice);
	
	/**
	 * Copy the string in newSymbol to the member symbol in an order struct.
	 * The maximum length of the symbol is MAX_SYMBOL_LENGTH.
	 */
	void setOrderSymbol (OrderPtr orderData, char* newSymbol);
	
	/**
	 * Print the properties of a single order.
	 * Already given in the file list.c.
	 */
	void printOrderData(OrderPtr orderData, FILE *out);

	/**
	 * Create and return a new linked list node filled in with the given order.
	 * Note: the values in data should be duplicated rather than being linked to the node directly.
	 */
	NodePtr newNode (OrderPtr data);

	/**
	 * In a linked list with *head as the head pointer, add the given node to the front of the list.
	 */
	void pushNode (NodePtr* head, NodePtr node) ;

	/**
	 * In a linked list with *head as the head pointer, return the onode with the given order id.
	 */
	NodePtr getOrderNode (NodePtr head, int id);

	/**
	 * Return the next node of the given node.
	 * IMPORTANT: Return NULL if node is NULL.
	 */
	NodePtr getNextNode (NodePtr node);

	/**
	 * Return the previous node of the given node.
	 * IMPORTANT: Return NULL if node is NULL.
	 */
	NodePtr getPrevNode (NodePtr node);
	
	/**
	 * Return the data of the given node.
	 */
	OrderPtr getOrderData (NodePtr node);
	
	/**
	 * Insert the given node (insertingNode) after the node prevNode into the list with
	 * head pointer head. If the prevNode is NULL, then the node insertingNode is inserted
	 * at the front of the list.
	 */
	void insertNode (NodePtr* head, NodePtr prevNode, NodePtr insertingNode);
	
	/**
	 * Evict the specified node from the list, but do not free its memory.
	 * The node is guaranteed to be on the given linked list.
	 */
	void evictNode (NodePtr* head, NodePtr node);
	
	/**
	 * Remove the specified node from the list, and free all memory the node is using.
	 * Note: if *head points to node, then it should be updated to point to 
	 * the new list head (or NULL if the list is empty).
	 * The node is guaranteed to be on the given linked list.
	 */
	void deleteNode (NodePtr* head, NodePtr node);
	
	/**
	 * Swap nodes n1 and n2 in the linked list pointed to by *head.
	 * Eg., if the list looks like: -- n1 -- n2 -- , then after the swapping, 
	 * it should look like: -- n2 -- n1 --.
	 * The nodes are guaranteed to be on the given linked list.
	 */
	void swapNodes (NodePtr* head, NodePtr n1, NodePtr n2);
	
	/**
	 * Delete every node in the list pointed to by the *head pointer. After calling this
	 * function, all memory used by the list should be freed, and *head should become NULL.
	 */
	void deleteList (NodePtr* head);
		
	
	/**
	 * Print the list to out starting from the parameter node.
	 * The format to print a single item is defined by printItem.
	 */
	void printList (NodePtr node, void (*printItem)(OrderPtr, FILE *), FILE *out);

#endif //LIST_H_
