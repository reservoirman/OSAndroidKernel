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

                
           } 
           else 
	   {
		struct Node *temp = (struct Node *) malloc((size_t)sizeof(struct Node));
                sem->head->next = temp;
		temp->data = data;
                temp->next = NULL;
                sem->head = temp;
                sem->sthread_count++;
                
           }
printf("Insert data: data:%d node:%p next:%p cnt:%d\n", sem->head->data, sem->head, sem->head->next, sem->sthread_count);
}

void removeNode(struct sthread_sem_struct *sem) {
    struct Node *temp;
    temp = sem->start;
    sem->start = sem->start->next;
    free(temp);
                printf("RemoveNode:data:%d node:%p next:%p cnt:%d\n", sem->head->data, sem->head, sem->head->next, sem->sthread_count);
   
    // wake up node
    // potentially free it
}

void traverse(struct sthread_sem_struct *sem) {
    struct Node *current;
    current = sem->start;
    while(current != NULL) {
        printf("current data: %d current node: %p next node: %p\n", current->data, current, current->next);
        current = current->next;
    }
}

int main() {

struct sthread_sem_struct mylist;
mylist.head = (struct Node*) malloc((size_t)sizeof(struct Node));
mylist.sthread_count = 0;

//initialize
insertNode(&mylist, 1);
insertNode(&mylist, 2);
insertNode(&mylist, 3);
insertNode(&mylist, 4);
insertNode(&mylist, 5);
insertNode(&mylist, 6);
insertNode(&mylist, 7);

traverse(&mylist);

removeNode(&mylist);
removeNode(&mylist);
removeNode(&mylist);

traverse(&mylist);


return 0;	
}
