#include <stdio.h>
#include <stdlib.h>


struct Node {
    int data; // sthread
    struct Node *next; // pointer to the next sthread
};

struct sthread_sem_struct {
    int count;
    int sthread_count;
    struct Node *head;
    struct Node *start;
};


void insertNode(struct sthread_sem_struct *sem, int data) {
    
          if (sem->sthread_count == 0) {

	       sem->start = sem->head ;
               sem->head->data = data;
               sem->head->next = NULL;
               sem->sthread_count++;

                printf("data:%d node:%p next:%p\n", sem->head->data, sem->head, sem->head->next);
           } 
           else 
	   {
		struct Node *temp = (struct Node *) malloc((size_t)sizeof(struct Node));
                sem->head->next = temp;
		temp->data = data;
                temp->next = NULL;
                sem->head = temp;
                sem->sthread_count++;
                printf("2.data:%d node:%p next:%p\n", sem->head->data, sem->head, sem->head->next);
           }
}

void removeNode(struct sthread_sem_struct *sem) {
    struct Node *temp;
    temp = sem->start;
    sem->start = sem->start->next;
    free(temp);
                printf("3.data:%d node:%p next:%p\n", sem->head->data, sem->head, sem->head->next);
   
    // wake up node
    // potentially free it
}

void traverse(struct sthread_sem_struct *sem) {
    struct Node *current;
    current = sem->start;
    while(current != NULL) {
        printf("value:%d\n", current->data);
        current = current->next;
    }
}

int main() {

struct sthread_sem_struct mylist;
mylist.head = (struct Node*) malloc((size_t)sizeof(struct Node));
mylist.sthread_count = 0;

//initialize
insertNode(&mylist, 24);
insertNode(&mylist, 9943);
insertNode(&mylist, 724);
insertNode(&mylist, 9);
insertNode(&mylist, 3);
insertNode(&mylist, 3333);
insertNode(&mylist, 1222223214);

traverse(&mylist);

removeNode(&mylist);
removeNode(&mylist);
removeNode(&mylist);

traverse(&mylist);


return 0;	
}
