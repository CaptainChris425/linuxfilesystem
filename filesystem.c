/*
Christopher Young
CptS360
Linux File System
*/
//TODO : Abstract tree and node functions away from main file
// 		do not do until functional. too hard to start that way



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct node{
	char name[64];
	char type;
	struct node *child, *sibling, *parent;
}NODE;

NODE* initializeNode(char name[],char type){
	NODE * n;
	n = (NODE *)malloc(sizeof(NODE));
	strcpy(n->name, name);
	n->type = type;
	n->child = NULL;
	n->sibling = NULL;
	n->parent = n;
	return n;
}

NODE *root, *cwd, *start;
char line[128];
char command[64], pathname[64];
char dname[64], bname[64];
char *name[100];
int n;
FILE *fp;

int tokenize(char *pathname){
	n = 0;
	char *s;
	s = strtok(pathname,"/");
	while(s){
		name[n] = s;
		n++;
		s = strtok(0,"/");
	}		
	return n;
		
}

NODE *search_child(NODE* parent, char *name){
	//TODO fix this so it works:
		//doesnt work when the node doesnt exist
	if (strcmp(parent->name,name) == 0){ return parent;};
	if (parent->child != NULL) { parent = parent->child;}
	else {return 0;} //first step is to go to the child, if the child doesnt exist
	//				then the parent dir is empty
	if (strcmp(parent->name,name) == 0){ return parent;}
	while (parent->sibling){
		if (strcmp(parent->name,name) == 0){ return parent;}
		else { parent = parent->sibling;};
		if (strcmp(parent->name,name) == 0){ return parent;}
	}
	return 0;
}

int search_childtest(){
	printf("Search Child Test\n");
	root -> child = initializeNode("a",'D');
	root -> child -> sibling = initializeNode("b",'D');
	printf("%s", search_child(root,"b")->name);
	printf("\nExpected Output: b\n");

}


NODE *path2node(char *pathname){
	if (pathname[0] == '/') { start = root; }
	else {start = cwd;};
	n = tokenize(pathname);
	NODE *p = start;
	
	for (int i=0;i<n;i++){
		p = search_child(p,name[i]);
		if (p==0) {return 0;};	
	}
	return p;
}


int path2nodetest(){
	printf("Path2Node test\n");
	NODE *p = path2node("/a");
	printf("%s", p->name);
	p = path2node("/b");
	printf("%s", p->name);
	//p = path2node("/b/c");
	//printf("%d", p);
	printf("\nExpected Output: ab0\n");

}
int initializefilesystem(){
	printf("Starting File System Loading: ...");
	root = initializeNode( "/" , 'D');
	cwd = root;
	printf("Done Loading\n");
	return 0;
}
		
int initializefilesystemtest(){
	printf("Initialize file system test\n");
	initializefilesystem();
	printf("%d",strcmp(root->name,"/"));
	printf("%d",root->type =='D');
	printf("\nExpected Output:  01\n");
}

int tokenizetest(){
	strcpy(pathname, "/a/b/c");
	tokenize(pathname);
	printf("Tokenize Test\n");
	printf("%d",*name[0]=='a');
	printf("%d",*name[1]=='b');
	printf("%d",*name[2]=='c');
	strcpy(pathname, "/b/c/a");
	printf("%d",*name[0]=='a');
	printf("%d",*name[1]=='b');
	printf("%d",*name[2]=='c');
	printf("\nExptected Output : 111000 \n");	
}

int testsuite(){
	tokenizetest();
	initializefilesystemtest();
	search_childtest();
	path2nodetest();
	return 0;
}

int mkdir(char *pathname){
	
	return 0;
}
int main(){
	printf("MAIN Starting");
	testsuite();	
	printf("MAIN Success\n");
	return 0;
}
