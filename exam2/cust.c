#include <stdio.h>

void menu();
int id,first,last;
struct CustomerInfo
{
	char FirstName[15];        /* These are the varibles for the customer infomation */
	char LastName[20];
	int ID;
};

int main()
{                               /* program starts */

	int selection = 0;

	void menu()
	{                    /* Menu loop              function */

		do
		{                                                                   /* menu start */
			printf("\n\n - What would you like to do?");
			printf("\n1  - Store a customer record");
			printf("\n2  - View customer Records");
			printf("\n3  - Quit program");
			scanf("%i", &selection);

		}   while (selection > 3);

		printf("You have entered an incorrect value");  /* If selection is greater than 3 then end program */
		return 0;
	}

	switch(selection)
	{
		/* switch statement starts */
		case 1:
			struct CustomerInfo s;
			printf("Please enter the customers details including First name, Lastname and ID.\n\n");
			printf("Enter First name: ");
			scanf("%s", s.FirstName);                                           /* Option 1: Asks to enter the customers details to store then loops back to program */
			printf("Enter Last name:  ");
			scanf("%s", s.LastName);
			printf("Enter Customer ID: ");
			scanf("%s", s.ID);
			void menu();
			break;

		case  2:

			printf("\nDisplaying Infomation\n");
			printf("First name: %s\n",s.Firstname);                             /* Option 2: Prints the customer details as listed in option 1 */
			printf("Last name: %s\n",s.Lastname);
			printf("Customer ID: %s\n",s.ID);
			void menu();
			break;

		case  3:                /* Option 3:     Program ends if option 3 is chosen. */
			break;
	}
