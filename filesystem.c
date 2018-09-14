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

NODE *search_child(NODE* parent, char *name,char type){
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
	if (strcmp(parent->name,name) == 0&& parent->type == type){ return parent;};
	if (parent->child != NULL) { parent = parent->child;}
	else {return 0;} //first step is to go to the child, if the child doesnt exist
	//				then the parent dir is empty
	if (strcmp(parent->name,name) == 0&& parent->type == type){ return parent;}
	while (parent->sibling){
		parent = parent->sibling;
		if (strcmp(parent->name,name) == 0 && parent->type == type){ return parent;}
	}
	return 0;
}

NODE *path2node(char *pathname, char type){
	/* Searches for the node in the file system
		that has the same location as specified
		in the pathname
	returns null if node does not exist */
	if (pathname[0] == '/') { start = root; }
	else {start = cwd;};
	//pathname[0] == '/' ? start=root : start = cwd;
	n = tokenize(pathname);
	NODE *p = start;
	if (type == 'D'){
		for (int i=0;i<n;i++){
			p = search_child(p,name[i],type);
			if (p==NULL) {return 0;};	
		}
	}else{
		for (int i=0;i<n-1;i++){
			p = search_child(p,name[i],'D');
			if (p==NULL) {return 0;};	
		}
		p = search_child(p,name[n-1],type);
		if (p==NULL){return 0;}
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
	NODE *p = path2node(pathname,'D');
	NODE *q = start;
	if (p!= NULL){
		printf("Dir already exists\n");
		return -1;
	}
	for(int i=0; i<n;i++){
		p = search_child(q,name[i],'D');
		if (p==NULL){
			addChild(q,initializeNode(q,name[i],'D'));
			p = search_child(q,name[i],'D');
		}
		q = p;
	}
	return 0;
}

int creat(char *pathname){
	NODE *p = path2node(pathname,'F');
	NODE *q = start;
	if (p!= NULL){
		printf("File already exists\n");
		return -1;
	}		
	for (int i=0;i<n-1;i++){
		p = search_child(q,name[i],'D');
		if (p==NULL){
			addChild(q,initializeNode(q,name[i],'D'));
			p = search_child(q,name[i],'D');
		}
		q = p;
	}
	p = search_child(q,name[n-1],'F');
	if (p==NULL){
		addChild(q,initializeNode(q,name[n-1],'F'));
	}else{printf("File already exists\n");}
	return 0;
}

int cd(char *pathname){
	NODE *p = path2node(pathname,'D');
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
	if (pathname == NULL || pathname == "\n"){ 
		p = cwd;
	}else{
		p = path2node(pathname, 'D');
	}
	if (p == NULL){
		printf("Directory was not found\n");	
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
	printf("\n");
	return 0;
}

int rmdir(char *pathname){
	NODE *p = path2node(pathname,'D');
	if (p){
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
	}else{
		printf("Dir was not found to remove\n");	
	}
	return 0;
}

int rm(char *pathname){
	NODE *p = path2node(pathname,'F');
	if (p){
		NODE *q = p->parent;
		if (q->child == p) {q->child = p->sibling; printf("%s removed\n",p->name);}
		else{
			q = q->child;
			while(q->sibling){
				if (q->sibling == p){
					q->sibling = p->sibling;
					printf("%s removed\n",p->name);
					return 0;}
				else{ q = q->sibling; }
			}
		}
	}else{
		printf("File was not found to remove\n");
	}	

}

int menu(){
	printf("=====================================================\n" \
		"mkdir pathname : Makes a new directory at specified pathname\n" \
		"rmdir pathname : Removes the directory, if it is empty\n" \
		"cd [pathname] : Changes cwd to pathname or '/' if not specified\n" \
		"ls [pathname] : Lists the content of the cwd or pathname\n" \
		"pwd		: Prints the absolute pathname\n" \
		"creat [pathname] : Creates a FILE at specified pathname\n" \
		"rm [pathname] : Removes the file at pathname\n" \
		"save [filename] : Saves the file tree to filename\n" \
		"load [filename] : Loads the file tree from filename\n" \
		"=====================================================\n");
	return 0;
}

int quit(char *pathname){
	exit(0);}

int menutest(){
	menu();
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
	strcpy(pathname, "/a/f/l/r");
	creat(pathname);
	fflush(stdout);
	NODE* p;
	strcpy(pathname, "/a/f/l");
	ls(pathname);
	strcpy(pathname, "/a/x");
	p = path2node(pathname,'F');
	printf("%c --- %s\n", p->type,p->name);

	strcpy(pathname, "/y");
	p = path2node(pathname,'F');
	printf("%c --- %s\n", p->type,p->name);

	strcpy(pathname, "/z");
	p = path2node(pathname,'F');
	printf("%c --- %s\n", p->type,p->name);

	strcpy(pathname, "/a/f/l/r");
	p = path2node(pathname,'F');
	printf("%c --- %s\n", p->type,p->name);

	printf("\nExpected Output: xyzl\n");

}

int rmtest(){
	printf("Rm test\n");
	strcpy(pathname,"/a/f");
	ls(pathname);
	printf("Before adding files^^\n");

	strcpy(pathname,"/a/f/test");
	creat(pathname);
	strcpy(pathname,"/a/f/test2");
	creat(pathname);
	strcpy(pathname,"/a/f");
	printf("\n");
	ls(pathname);
	printf("\n");
	strcpy(pathname,"/a/f/test");
	rm(pathname);
	strcpy(pathname,"/a/f");
	printf("\n");
	ls(pathname);
	printf("\n");
	strcpy(pathname,"/a/f/test2");
	rm(pathname);
	strcpy(pathname,"/a/f/test2");
	rm(pathname);
	strcpy(pathname,"/a/f");
	printf("\n");
	ls(pathname);
	printf("\n");
	printf("\nExpected output: removed test&test2 (cannot find)\n");

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
	printf("%s", path2node(pathname,'D')->name);
	strcpy(pathname, "/o");
	printf("%s", path2node(pathname,'D')->name);
	strcpy(pathname, "/a");
	printf("%s", path2node(pathname,'D')->name);
	strcpy(pathname, "/a/l");
	printf("%s", path2node(pathname,'D')->name);
	strcpy(pathname, "/a/f/r");
	printf("%s", path2node(pathname,'D')->name);
	printf("\nExpected Output: (already)foa(null)r\n");
}

int search_childtest(){
	printf("Search Child Test\n");
	root -> child = initializeNode(root,"a",'D');
	root -> child -> sibling = initializeNode(root,"b",'D');
	root -> child -> sibling -> child = initializeNode(root->child->sibling,"c",'D');
	printf("%s", search_child(root,"b",'D')->name);
	printf("%s", search_child(root->child->sibling,"c",'D')->name);
	printf("%s", search_child(root,"c",'D')->name);
	printf("\nExpected Output: bc(null)\n");

}

int path2nodetest(){
	printf("Path2Node test\n");
	strcpy(pathname, "/a");
	NODE *p = path2node(pathname,'D');
	printf("%s", p->name);
	fflush(stdout);
	strcpy(pathname,"/b");
	p = path2node(pathname,'D');
	printf("%s", p->name);
	fflush(stdout);
	strcpy(pathname,"/c");
	p = path2node(pathname,'D');
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
	menutest();
	tokenizetest();
	initializefilesystemtest();
	search_childtest();
	path2nodetest();
	mkdirtest();
	cdtest();
	lstest();
	pwdtest();
	rmdirtest();
	creattest();
	rmtest();
	return 0;
}

char *cmd[] = {"mkdir","rmdir","ls","cd","pwd","creat","rm","quit","menu","reload","save",NULL};
int findCmd(char *command){
	int i = 0;
	while(cmd[i]){
		if (!strcmp(command,cmd[i])){
			return i; }
		i++;
	}
	return -1;
}

int (*fptr[])(char *) = { (int (*) ())mkdir,rmdir,ls,cd,pwd,creat,rm,quit,menu};

int main(){
/*
	printf("MAIN Starting\n");
	testsuite();	
	printf("MAIN Success\n");
*/
	int index;
	initializefilesystem();
	while(1){
		fgets(line,128,stdin);
		line[strlen(line)-1] = 0;
		pathname[0] = 0;
		sscanf(line, "%s %s", command, pathname);
		printf("pathname = %c",pathname[0]);
		index = findCmd(command);
		int r = fptr[index](pathname);
	}


			
	return 0;
}
