struct node {
	struct node *next;
	int val;
}

void deleteList(struct node *head) {
	struct node *current = head;
	struct node *next;
	while ( current != NULL) {
		next = current->next;
		free(current);
		current = next;
	}
	head = NULL;
}

struct node {
	struct node *next;
	struct node *prev;
	int val;
}

void remove(struct node **head, struct node *n1);

void insert(struct node **head, struct node *n1, struct node *n2);

void swap(struct node **head, struct node *n1, struct node *n2) {
	if (n1 == n2) return;
	if ( n1->next == n2) {
		remove(head,n2);
		insert(head,n1,n2);
	}
	if ( n2->next == n2) {
		remove(head,n2);
		insert(head,n2,n2);
	}
	else {
		remove(head, n1);
		remove(head, n2);
		insert(head, n1->prev, n2);
		insert(head, n2->prev, n1);
	}
}

