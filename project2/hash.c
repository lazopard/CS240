/**
 * hash.c
 *
 * Define the functions used for a hash.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "list.h"
#include "hash.h"

/*struct hashStorage {
	int (*funcHash) (int);
	void (*printItem) (struct order *, FILE *);
	NodePtr* table;
	int size;
};

*/

/**
 *  * Create a new instance of struct hashStorage, initialize it with the given parameters, and return it.
 *  * Notes:
 *  *  size is the length of the hash array.
 *  *  printOrder specifies the format how an order is printed to an output stream.
 *  *  myHash is the hash function. If myHash is NULL then the hash should be a linked list and the size should be 1.
 *  *  The struct member table is a pointer to an array, with length size, of linked lists.
 *  */

struct hashStorage* createHash(int size, int (*myHash)(int), void (*printOrder)(struct order*, FILE*)) {
	struct hashStorage *newHashStorage;
	if (myHash == NULL) {
		newHashStorage->size = 1;
		newHashStorage->printItem = printOrder;
		//deal with hash
		//deal with table
		return newHashStorage;
	}
	newHashStorage = malloc(sizeof(struct hashStorage));
	newHashStorage->size = size;
	newHashStorage->printItem = printOrder;
	newHashStorage->funcHash = myHash;
	NodePtr *newTable = malloc(sizeof(NodePtr)*size);;
	int i = 0;
	for(i; i < size; i++) {
		NodePtr temp = newNode(NULL);
		*(newTable + i) = temp;
	}
	newHashStorage->table = newTable;
}

/**
 *  * Return the size of the hash table
 *   */

int getHashSize (struct hashStorage* hash) {
	return hash->size;
}

/**
 *  * Return the table of the hash.
 *   */

struct onode** getHashTable (struct hashStorage* hash) {
	return hash->table;
}

/**
 *  * Add an order to the hash structure, and return the pointer to the onode.
 *   * You should add a copy of data to the hash instead of adding data directly.
 *    * Hint: create a new onode and push it to the correct linked list in the hash table.
 *     */

struct onode* addOrder(struct hashStorage* hash, struct order* data) {
	NodePtr newOrder = newNode(data);
	int index = hash->funcHash(newOrder->data->id);
	pushNode(hash->table + index, newOrder);
}

/**
 *  * Cancel an order from the hash.
 *   * Remove the order whose id is data->id from the list, and free the memory.
 *    */

void cancelOrder(struct hashStorage* hash, struct order* data) {
	int idIndex = hash->funcHash(data->id);

}

/**
 * * Reduce the quantity of the order whose id is data->id by the amount stored in data->quantity.
 * * If the remaining quantity is 0 then the node should be removed from
 * * the hash and its memory allocation should be destroyed.
 * */

void reduceOrderQty(struct hashStorage* hash, struct order* data) {
	int index = hash->funcHash(data->id)
	setOrderQty((hash->table + index)->data, (hash->table + index)->data->quantity - data->quantity);
	if ((hash->table + index)->data->quantity <= 0) {
		cancelOrder(hash, (hash->table + index)->data);
	}
	return;
}

/**
 * * Change the symbol, quantity, and price of the order whose id is data->id to
 * * be the values of data->symbol, data->quantity, and data->price.
 * * If the remaining quantity is 0 then the node should be removed from
 * * the hash and its memory allocation should be destroyed.
 * **/

void changeOrder(struct hashStorage* hash, struct order* data) {
	int index = hash->funcHash(data->id);
	setOrderPrice((hash->table + index)->data, data->price)
	setOrderSymbol((hash->table + index)->data, data->symbol)
	setOrderQty((hash->table + index)->data, data->quantity)
	if ((hash->table + index)->data->quatity <= 0) {
		cancelOrder(hash, (hash->table + index)->data);
	}
	return;
}

/**
 *  * Given the hash and the output stream, print the order book. The function should
 *   * print each item using the function hash->printItem and the output stream required 
 *    * for the print.
 *     */

void printOrderBook (struct hashStorage* hash, FILE *out) {
	int i;
	for(i = 0; i < hash->size; i++) {
		hash->printItem((hash->table + i)->data, out); 
	}
	return;
}

/**
 ** Destroy the order book and free all the memory allocated for that hash.
 **/

void freeOrderBook  (struct hashStorage** hash) {
}
