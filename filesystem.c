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

NODE* initializeNode(NODE *parent,char name[],char type){
	NODE * n;
	n = (NODE *)malloc(sizeof(NODE));
	strcpy(n->name, name);
	n->type = type;
	n->child = NULL;
	n->sibling = NULL;
	n->parent = parent;
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
	printf("\nThe tokenizing string is %s\n", pathname);
	n = 0;
	char *s;
	printf("\nn = 0\n");	
	s = strtok(pathname,"/");
	printf("The first character = %s",s);
	while(s){
		printf("\nTokenize: %s", s);
		name[n] = s;
		n++;
		s = strtok(0,"/");
	}		
	return n;
		
}

NODE *search_child(NODE* parent, char *name){
	//TODO fix this so it works:
		//doesnt work when the node doesnt exist

/*What needs to happen here:
    Search the child of the node
	if it is null then return 0;
	if it is not null
	    move to it
	check the nodes name
	if it is it then return the node
	else while it has siblings
	    move to the sibling
	    check siblings name
	    if it is then return the node
	return 0
*/
	printf("Starting search_child\n");
	fflush(stdout);

	if (strcmp(parent->name,name) == 0){ return parent;};
	printf("1. %s\n", parent->name);
	fflush(stdout);

	if (parent->child != NULL) { parent = parent->child;
		printf("2. %s\n", parent->name);
		fflush(stdout);}
	else {return 0;} //first step is to go to the child, if the child doesnt exist
	//				then the parent dir is empty
	if (strcmp(parent->name,name) == 0){ return parent;}
	while (parent->sibling){
		parent = parent->sibling;
		printf("3. %s\n", parent->name);
		fflush(stdout);
		if (strcmp(parent->name,name) == 0){ return parent;}
	}
	return 0;
}

NODE *path2node(char *pathname){
	/* Searches for the node in the file system
		that has the same location as specified
		in the pathname
	returns null if node does not exist */
	if (pathname[0] == '/') { start = root; }
	else {start = cwd;};
	printf("Tokenize after this");
	fflush(stdout);
	n = tokenize(pathname);
	printf("Tokenized");
	fflush(stdout);

	NODE *p = start;
	
	for (int i=0;i<n;i++){
		p = search_child(p,name[i]);
		if (p==NULL) {return 0;};	
	}
	return p;
}

int initializefilesystem(){
	printf("Starting File System Loading: ...");
	root = initializeNode(NULL, "/" , 'D');
	cwd = root;
	printf("Done Loading\n");
	return 0;
}
int addChild(NODE *parent, NODE *child){
	if (parent->child == NULL){
		parent->child = child;
		return 0;
	}
	else{
		parent = parent->child;
		while (parent->sibling){
			parent = parent->sibling;
		}
		parent->sibling = child;
		return 0;
	}
}

int mkdir(char *pathname){
	/*Check if the pathname already exists. Then using the name
actually I should be able to start at start
	we are going to start at the cwd or at the root depending on pathname[0]
	then we should be able to call find_child on name[n] and name[n+1]
	nove to when find child returns null
		call add to parent		
*/
	NODE *p = path2node(pathname);
	NODE *q = start;
	if (p!= NULL){
		if(p->type == 'D'){
			return 0; 
		}
	}
	for(int i=0; i<n;i++){
		p = search_child(q,name[i]);
		if (p==NULL){
			addChild(q,initializeNode(q,name[i],'D'));
			p = search_child(q,name[i]);
		}
		q = p;
	}
	return 0;
}

int mkdirtest (){
	printf("Mkdir test\n");
	//TODO: STRCPY BEFORE CALLING FUNCTION
	mkdir("/a/f");
	mkdir("/o");
	mkdir("/a");
	printf("%s", path2node("/a/f")->name);
	printf("%s", path2node("/o")->name);
	printf("%s", path2node("/a")->name);


	

}
int search_childtest(){
	printf("Search Child Test\n");
	root -> child = initializeNode(root,"a",'D');
	root -> child -> sibling = initializeNode(root->child,"b",'D');
	root -> child -> sibling -> child = initializeNode(root->child->sibling,"c",'D');
	printf("%s", search_child(root,"b")->name);
	printf("%s", search_child(root->child->sibling,"c")->name);
	printf("%s", search_child(root,"c")->name);

	printf("\nExpected Output: bc(null)\n");

}
int path2nodetest(){
	printf("Path2Node test\n");
	NODE *p = path2node("/a/");
	printf("%s", p->name);
	fflush(stdout);
	p = path2node("/b");
	printf("%s", p->name);
	fflush(stdout);
	p = path2node("/c");
	printf("%s", p->name );
	fflush(stdout);
	printf("\nExpected Output: ab0\n");

}

		
int initializefilesystemtest(){
	printf("Initialize file system test\n");
	initializefilesystem();
	printf("%d",strcmp(root->name,"/"));
	printf("%d",strcmp(cwd->name,"/"));
	printf("%d",root->type =='D');
	printf("\nExpected Output:  001\n");
}

int tokenizetest(){
	strcpy(pathname, "/a/b/c");
	tokenize(pathname);
	printf("Tokenize Test\n");
	printf("%d",*name[0]=='a');
	printf("%d",*name[1]=='b');
	printf("%d",*name[2]=='c');
	strcpy(pathname, "/b/c");
	tokenize(pathname);
	printf("%d",*name[0]=='b');
	printf("%d",*name[1]=='c');
	//printf("%d",*name[2]=='a');
	printf("\nExptected Output : 111111 \n");	
}

int testsuite(){
	tokenizetest();
	initializefilesystemtest();
	search_childtest();
	//path2nodetest();
	mkdirtest();
	return 0;
}

int main(){
	printf("MAIN Starting");
	testsuite();	
	printf("MAIN Success\n");
	return 0;
}
