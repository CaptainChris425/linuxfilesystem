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
	if (strcmp(parent->name,name) == 0){ return parent;};
	if (parent->child != NULL) { parent = parent->child;}
	else {return 0;} //first step is to go to the child, if the child doesnt exist
	//				then the parent dir is empty
	if (strcmp(parent->name,name) == 0){ return parent;}
	while (parent->sibling){
		parent = parent->sibling;
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
	//pathname[0] == '/' ? start=root : start = cwd;
	n = tokenize(pathname);
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
			//TODO: if it is not a D it still needs to continue and make a 
				// D type node here
			printf("Dir already exists");
			return -1; 
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




int cd(char *pathname){
	NODE *p = path2node(pathname);
	if (p == NULL){
		printf("Directory was not found\n");	
	}
	else if (p->type == 'F'){
		//if this happens we need a function to continue checking from down the list	
		return 0;
	}else{
		cwd = p;
	}
}

int ls(char *pathname){
//	if (pathname==null){
//	}
	NODE *p;
	if (pathname == NULL){ 
		p = cwd;
	}else{
		p = path2node(pathname);
	}
	if (p == NULL){
		printf("Directory was not found\n");	
	}
	else if (p->type == 'F'){
		//if this happens we need a function to continue checking from down the list	
		return 0;
	}else{
		if (p->child){
			p = p->child;
			printf("%c --- %s\n", p->type,p->name);
			while(p->sibling){
				p = p->sibling;
				printf("%c --- %s\n", p->type,p->name);
			}
		}
	}
	return 0;
}	


int pwdhelper(NODE* start){
	if (start->parent){
		pwdhelper(start->parent);
	}else{
		printf("/");
		return 0;
	}
	printf("%s/",start->name);
	return 0;
}

int pwd(){
	NODE* p = cwd;		
	pwdhelper(p);	
	return 0;
}

int rmdir(char *pathname){
	NODE *p = path2node(pathname);
	if (p){
		if (p->type == 'F') { printf("rmdir failed - dir specified is a file"); return 0;}
		if (p->child){ printf("rmdir failed - dir is not empty\n"); return 0;}
		else{
			NODE *q = p->parent;
			if (q->child = p) {q->child = p->sibling; printf("%s removed\n",p->name);}
			else{
				q = q->child;
				while(q->sibling){
					if (q->sibling == p){ q->sibling = p->sibling;}
					else{ q = q->sibling; }
				}
			}
		}
	}	
	return 0;
}

int creat(char *pathname){
	return 0;
}

int creattest(){
	printf("Creat test\n");
	strcpy(pathname, "/a/x");
	creat(pathname);
	strcpy(pathname, "/y");
	creat(pathname);
	strcpy(pathname, "/z");
	creat(pathname);
	strcpy(pathname, "/a/f/l");
	creat(pathname);
	strcpy(pathname, "/a/x");
	printf("%s", path2node(pathname)->name);
	strcpy(pathname, "/y");
	printf("%s", path2node(pathname)->name);
	strcpy(pathname, "/z");
	printf("%s", path2node(pathname)->name);
	strcpy(pathname, "/a/f/l");
	printf("%s", path2node(pathname)->name);
	printf("\nExpected Output: (already)foa(null)r\n");

}

int rmdirtest (){
	printf("Rmdir test\n");
	strcpy(pathname,"/a/f/test");
	mkdir(pathname);
	strcpy(pathname,"/a/f/test2");
	mkdir(pathname);
	strcpy(pathname,"/a/f");
	ls(pathname);
	strcpy(pathname,"/a/f/r");
	rmdir(pathname);
	strcpy(pathname,"/a/f");
	ls(pathname);
	strcpy(pathname,"/a/f");
	rmdir(pathname);
	printf("\nExpected output: removed r & cannot remove f\n");
}

int pwdtest (){
	printf("Pwd test\n");
	strcpy(pathname,"/a/f");
	cd(pathname);
	printf("%s\n",cwd->name);
	pwd();
	printf("\n");
	strcpy(pathname,"/b");
	cd(pathname);
	printf("%s\n",cwd->name);
	pwd();
	printf("\nExpected Output: /a/f /b\n");
	
	
}

int lstest (){
	printf("Ls test\n");
	strcpy(pathname,"/a");
	printf("ls /a\n");
	ls(pathname);
	strcpy(pathname,"/");
	printf("ls / \n");
	ls(pathname);
	strcpy(pathname,"/b");
	printf("ls /b \n");
	ls(pathname);
	strcpy(pathname,"/");
	printf("cd -> / && ls null\n");
	cd(pathname);
	ls(NULL);	
	strcpy(pathname,"/a/f");
	printf("ls /a/f \n");
	ls(pathname);

	printf("\nExpected Output: f abocabor\n");
}
int cdtest (){
	printf("Cd test\n");
	strcpy(pathname,"/o");
	cd(pathname);
	printf("%s", cwd->name);
	strcpy(pathname,"/a/f");
	cd(pathname);
	printf("%s", cwd->name);
	strcpy(pathname,"/a");
	cd(pathname);
	printf("%s", cwd->name);
	strcpy(pathname,"f");
	cd(pathname);
	printf("%s", cwd->name);
	strcpy(pathname,"x");
	cd(pathname);
	printf("%s", cwd->name);
	printf("\nExpected Output: ofaf(notfound)f\n");
	
	
}

int mkdirtest (){
	printf("Mkdir test\n");
	strcpy(pathname, "/a/f");
	mkdir(pathname);
	strcpy(pathname, "/o");
	mkdir(pathname);
	strcpy(pathname, "/a");
	mkdir(pathname);
	strcpy(pathname, "/a/f/r");
	mkdir(pathname);
	strcpy(pathname, "/a/f");
	printf("%s", path2node(pathname)->name);
	strcpy(pathname, "/o");
	printf("%s", path2node(pathname)->name);
	strcpy(pathname, "/a");
	printf("%s", path2node(pathname)->name);
	strcpy(pathname, "/a/l");
	printf("%s", path2node(pathname)->name);
	strcpy(pathname, "/a/f/r");
	printf("%s", path2node(pathname)->name);
	printf("\nExpected Output: (already)foa(null)r\n");
}

int search_childtest(){
	printf("Search Child Test\n");
	root -> child = initializeNode(root,"a",'D');
	root -> child -> sibling = initializeNode(root,"b",'D');
	root -> child -> sibling -> child = initializeNode(root->child->sibling,"c",'D');
	printf("%s", search_child(root,"b")->name);
	printf("%s", search_child(root->child->sibling,"c")->name);
	printf("%s", search_child(root,"c")->name);
	printf("\nExpected Output: bc(null)\n");

}
int path2nodetest(){
	printf("Path2Node test\n");
	strcpy(pathname, "/a");
	NODE *p = path2node(pathname);
	printf("%s", p->name);
	fflush(stdout);
	strcpy(pathname,"/b");
	p = path2node(pathname);
	printf("%s", p->name);
	fflush(stdout);
	strcpy(pathname,"/c");
	p = path2node(pathname);
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
	path2nodetest();
	mkdirtest();
	cdtest();
	lstest();
	pwdtest();
	rmdirtest();
	return 0;
}

int main(){
	printf("MAIN Starting\n");
	testsuite();	
	printf("MAIN Success\n");
	return 0;
}
