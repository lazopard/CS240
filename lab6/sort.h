int wordCmp   (struct lnode* n1, struct lnode* n2);
int lineCmp   (struct lnode* n1, struct lnode* n2);
int countCmp  (struct lnode* n1, struct lnode* n2);

void swap     (struct lnode** head, struct lnode* n1, struct lnode* n2);

void sortByWord     (struct lnode** head);
void sortByCount    (struct lnode** head);
void sortByLine     (struct lnode** head);


void sort     (struct lnode** head,
				void (*swapPtr)    (struct lnode**, struct lnode*, struct lnode*),
				int  (*comparePtr) (void*, void*));
