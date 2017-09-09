#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<limits.h>

// idea is to maintain a linked list of size Capacity. Each node has a Age.
// The most aged node will be modified if the list is full and there is new 
// entry to be made into the cache.

typedef struct linked_list {
	int page;	// key to look into Cache
	int val;	// val hold by the page.
	int age;	// newest entry will have 0.
	struct linked_list *next;
} l_node;

l_node *head, *tail; // globals

// initialize and return the linked list node.
l_node *getNode() {
	l_node *temp = (l_node *)malloc(sizeof(l_node));
	temp->next = NULL;
	temp->age = 0;
	temp->page = -1;
	temp->val = INT_MIN;
	return temp;
}

// on a successful get(key) or set(set, key) call the age of each entry in cache
// will be updated by 1. Except for the recent node whose age will be zero.
void incrementAge(l_node *entry) {
	l_node *walk = head;
	entry->age = 0;
	while(walk) {
		if (walk != entry)
			walk->age += 1;
		walk = walk->next;
	}
}

// return the node with Max Age, as Cache is Full, the LRU entry in the cache
// will be modified.
l_node *getMaxAgeNode() {
	l_node *temp = head, *return_node = NULL;
	int max_age = INT_MIN;
	while(temp) {
		if (temp->age > max_age){
			return_node = temp;
			max_age = temp->age;
		}
		temp = temp->next;
	}
	return return_node;
}

int lruCacheGet(int page) {
	l_node *temp = head;
	while ( temp && temp->page != page) {
		temp = temp->next;
	}
	if (!temp)
		return (-1);
	incrementAge(temp);
	return temp->val;
}


int pageNotInCache(int page) {
	l_node *temp = head;
	while(temp){
		if (temp->page == page)
			return (0);
		temp = temp->next;
	}
	return (1);
}


void lruCacheSet(int page, int val){

	// if cache is full and the page is new.
	if (tail->page != -1 && pageNotInCache(page)) {
		l_node *max_age = getMaxAgeNode();
		max_age->page = page;
		max_age->val = val;
		incrementAge(max_age);
		return;
	}
	l_node *temp = head;

	while(temp && temp->page != page){
		temp = temp->next;
	}
	if (temp && temp->page == page){
		temp->val = val;
		incrementAge(temp);
		return;
	}
	temp = head;

	while(temp && temp->page != -1){
		temp = temp->next;
	}
	temp->page = page;
	temp->val = val;
	incrementAge(temp);
}

// build a linked list with Capacity number of nodes. Each Node is a cache line.
void lruCacheInit(int capacity) {
	int i;
	l_node *temp, *a_temp;
	head  = getNode();
	a_temp = head;
	for (i = 1; i < capacity; i++) {
		temp  = getNode();
		a_temp->next = temp;
		a_temp = temp;
	}
	tail = a_temp;
}

// assuming this will called at the end.
void lruCacheFree() {
	l_node *temp;
	temp = head;
	while(temp != tail) {
		head = head->next;
		free(temp);
		temp = head;
	}
}

void print_list() {
	l_node *temp = head;
	while(temp) {
		printf("List : %d ",temp->page);
		printf("%d ",temp->val);
		printf("%d \n",temp->age);
		temp = temp->next;
	}
}

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Please Provide Cache capacity\n");
		return (0);
	}
	int capacity = atoi(argv[1]);

	// Init Cache with Capacity.
	lruCacheInit(capacity);

	lruCacheSet(1,10);
	lruCacheSet(2,20);
	lruCacheSet(3,30);
	lruCacheSet(4,40);
	lruCacheSet(5,50);

	int val = 0;
	val = lruCacheGet(1);
	printf("%d\n", val);
	val = lruCacheGet(5);
	printf("%d\n", val);
	val = lruCacheGet(3);
	printf("%d\n", val);
	val = lruCacheGet(4);
	printf("%d\n", val);
	lruCacheFree();

	return(0);
}
