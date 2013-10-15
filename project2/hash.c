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

/**
 * TODO: In this file, implement all the functions listed in hash.h.
 * 
 * Note: Do not modify the includes.
 */

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
	return NULL;
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

}

/**
 *  * Cancel an order from the hash.
 *   * Remove the order whose id is data->id from the list, and free the memory.
 *    */

void cancelOrder(struct hashStorage* hash, struct order* data) {

}

/**
 * * Reduce the quantity of the order whose id is data->id by the amount stored in data->quantity.
 * * If the remaining quantity is 0 then the node should be removed from
 * * the hash and its memory allocation should be destroyed.
 * */

void reduceOrderQty(struct hashStorage* hash, struct order* data) {

}

/**
 * * Change the symbol, quantity, and price of the order whose id is data->id to
 * * be the values of data->symbol, data->quantity, and data->price.
 * * If the remaining quantity is 0 then the node should be removed from
 * * the hash and its memory allocation should be destroyed.
 * **/

void changeOrder(struct hashStorage* hash, struct order* data) {

}

/**
 *  * Given the hash and the output stream, print the order book. The function should
 *   * print each item using the function hash->printItem and the output stream required 
 *    * for the print.
 *     */

void printOrderBook (struct hashStorage* hash, FILE *out) {

}

/**
 ** Destroy the order book and free all the memory allocated for that hash.
 **/

void freeOrderBook  (struct hashStorage** hash) {

}
