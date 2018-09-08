/*
Christopher Young
CptS360
Linux File System
*/
//TODO : Abstract tree and node functions away from main file
// 		do not do until functional. too hard to start that way



#include <stdio.h>
#include <stdlib.h>

typedef struct node{
	char name[64];
	char type;
	struct node *child, *sibling, *parent;
}NODE;


int main(){
	NODE *root;
	printf("Success\n");
	return 0;
}
