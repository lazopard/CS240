struct lnode;

//List management
struct lnode* newNode     (char* word, int count, int line);
void          pushNode    (struct lnode** head, struct lnode* node);
struct lnode* getNode     (struct lnode* head, char* word);
void          deleteNode  (struct lnode** head, struct lnode* node);
void          deleteList  (struct lnode** head);

/**
 * Insert the given node after the prevNode. If the  prevNode is NULL,
 * then the given node is inserted at the head of the list.
 */

void          insertNode  (struct lnode** head, struct lnode* prevNode,
                           struct lnode* insertingNode);
void          evictNode   (struct lnode** head, struct lnode* node);
void          printList (struct lnode *head);

//Content Management
int           nodeGetCount  (struct lnode*);
int           nodeGetLine   (struct lnode*);
char*         nodeGetWord   (struct lnode*);
struct lnode* nodeGetNext   (struct lnode* node);
struct lnode* nodeGetPrev   (struct lnode* node);
