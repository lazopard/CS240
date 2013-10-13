/**
 * list.c
 *
 * Define the functions used for a linked list.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "list.h"

/**
 * TODO: In this file, implement all the functions listed in list.h.
 * 
 * Note: Do not modify the includes.
 */
 
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
