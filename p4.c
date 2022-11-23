https://powcoder.com
代写代考加微信 powcoder
Assignment Project Exam Help
Add WeChat powcoder
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <string.h>
#include <inttypes.h>

/* Kinds of tokens */
enum Kind {
    ELSE,    // else
    END,     // <end of string>
    EQ,      // =
    EQEQ,    // ==
    ID,      // <identifier>
    IF,      // if
    INT,     // <integer value >
    LBRACE,  // {
    LEFT,    // (
    MUL,     // *
    NONE,    // <no valid token>
    PLUS,    // +
    PRINT,   // print
    RBRACE,  // }
    RIGHT,   // )
    SEMI,    // ;
    WHILE,   // while
    FUNC,    // function
    CALL     // call statement
};

FILE *f;

/* information about a token */
struct Token {
    enum Kind kind;
    uint64_t value;
    char *ptr;
    char *start;
    char *id;
};


struct node{
    uint64_t value;
    char *key;
    struct node *next;
};

struct Node{
    char *key;
    struct Node *prev;
    struct Node *next;
};

struct node *first = NULL;
struct node *this = NULL;

struct Node *code = NULL;
struct Node *data = NULL;

void insert(char *key, uint64_t value){
   struct node *link = (struct node*) malloc(sizeof(struct node));
   
   link->key = key;
   link->value = value;

   link->next = first;
   
   first = link;

}

void print_list(){
   struct node *pt = first;
   printf("The Identifier List: [ ");
   while(pt != NULL){
	printf("(%s, %lu), ", pt->key, pt->value);
	pt = pt->next;
   }
   printf("] \n");
}


int isEmpty(){
   return first == NULL;
}

int get_length(){
   int length = 0;
   struct node *cur;
		
   for(cur = first; cur != NULL; cur = cur->next) {
       length++;
   }
	   	
   return length;
}

struct node* find(char* key){
    //print_list();
	struct node* cur = first;

    if(first == NULL) {
        return NULL;
    }

    while(strcmp(cur->key, key) != 0) {
	if(cur->next == NULL) {
		return NULL;
	} 
	else{
		cur = cur->next;
	}      
    }  	
    return cur;
}

/* The symbol table */

uint64_t get(char *id) {
    struct node *target = find(id);
    if(target == NULL){
	return 0;
    }
    return target->value;
}

void set(char *id, uint64_t value) {
    struct node *temp = find(id);
    if(temp == NULL){
        insert(id, value);
    }
    else{
	temp->value = value;
    }
}

int getNumLength(uint64_t val){
    int count = 0;
    while(val != 0){
        val /= 10;
        count++;
    }

    return count; 
}

void putCode(char *key){

    struct Node *cur = (struct Node*) malloc(sizeof(struct Node));
    struct Node *last = code;

    cur = (struct Node*) malloc(sizeof(struct Node));
    cur->next = NULL;
    cur->key = key;
    
    if(code == NULL){
        cur->prev = NULL;
        code = cur;
        return;
    }

    while(last->next != NULL){
        last = last->next;
    }
    last->next = cur;
    cur->prev = last;

}


void putData(char *key){

    struct Node *cur = (struct Node*) malloc(sizeof(struct Node));
    struct Node *last = data;

    cur = (struct Node*) malloc(sizeof(struct Node));
    cur->next = NULL;
    cur->key = key;
    
    if(data == NULL){
        cur->prev = NULL;
        data = cur;
        return;
    }

    while(last->next != NULL){
        last = last->next;
    }
    last->next = cur;
    cur->prev = last;

}


void storeInt(char* key, uint64_t val){
    int len = getNumLength(val);
    char *value=(char*) malloc(len+1);
    sprintf(value, "%lu", val);
    char mov[] = "    mov $";
    char *buffer1 = (char*) malloc(1024);
    snprintf(buffer1, (sizeof(mov)+len+1), "%s%s", mov, value);
    char reg[] = ",%rax\n";
    char *buffer2 = (char*) malloc(1024);
    snprintf(buffer2, (strlen(buffer1)+strlen(reg)+1), "%s%s", buffer1, reg);
    putCode(buffer2);

    char v[] = "    mov %rax,v_";
    char rip[] = "(%rip)\n";
    char *var=(char*) malloc(1024);
    char *varrip=(char*) malloc(1024);
    snprintf(var, (strlen(v)+strlen(key)+1), "%s%s", v, key);
    snprintf(varrip, (strlen(var)+strlen(rip)+1), "%s%s", var, rip);
    putCode(varrip);
}

void printVal(uint64_t val){
    
    int len = getNumLength(val);
    char value[len+1];
    sprintf(value, "%lu", val);
    char mov[] = "    mov $";
    char *buffer1 = (char*) malloc(1024);
    snprintf(buffer1, (strlen(mov)+len+1), "%s%s", mov, value);
    char reg[] = ",%rsi\n";
    char *buffer2 = (char*) malloc(1024);
    snprintf(buffer2, (strlen(buffer1)+strlen(reg)+1), "%s%s", buffer1, reg);
    
    putCode(buffer2);
    putCode("    lea format(%rip),%rdi\n");
    putCode("    mov %rax,%rsi\n");
    putCode("    xor %rax,%rax\n");
    putCode("    .extern printf\n");
    putCode("    call printf\n");
    putCode("    xor %rax,%rax\n");

}


void printKey(char* key){

    char v[] = "v_";
    char mov[] = "    mov ";
    char rax[] = ",%rax\n";
    char rip[] = "(%rip)";
    char *var = (char*) malloc(1024);
    char *movvar= (char*) malloc(1024);
    char *movvarrip= (char*) malloc(1024);
    char *movvarriprax= (char*) malloc(1024);
    snprintf(var, (strlen(v)+strlen(key)+1), "%s%s", v, key);
    snprintf(movvar, (strlen(mov)+strlen(var)+1), "%s%s", mov, var);
    snprintf(movvarrip, (strlen(movvar)+strlen(rip)+1), "%s%s", movvar, rip);
    snprintf(movvarriprax, (strlen(movvarrip)+strlen(rax)+1), "%s%s", movvarrip, rax);

    putCode(movvarriprax);
    putCode("    lea format(%rip),%rdi\n");
    putCode("    mov %rax,%rsi\n");
    putCode("    xor %rax,%rax\n");
    putCode("    .extern printf\n");
    putCode("    call printf\n");
    putCode("    xor %rax,%rax\n");

}


void insertData(char*key){

    char v[] = "v_";
    char quad[] = ":.quad 0\n";
    char *var= (char*) malloc(1024);
    char *data= (char*) malloc(1024);
    snprintf(var, (strlen(v)+strlen(key)+1), "%s%s", v, key);
    snprintf(data, (strlen(var)+strlen(quad)+1), "%s%s", var, quad);
    if(find(key)== NULL)
        putData(data);
    //printf("break point\n");
}

void writeCode(){

    struct Node *cur2 = data;
    while(cur2!=NULL){
        fputs(cur2->key, f);
        cur2 = cur2->next;
    }
    
    struct Node *cur = code;
    while(cur!=NULL){
        fputs(cur->key, f);
        cur = cur->next;
    }


}

/* The current token */
static struct Token current = { NONE, 0, NULL, NULL };

static jmp_buf escape;

enum Kind peek();

static char *remaining() {
    return current.ptr;
}

static void error() {
    printf("error at '%s'\n", remaining());
    longjmp(escape, 1);
}


enum Kind peek(){
    char* while_key = "while";
    char* if_key= "if";
    char* else_key = "else";
    char* print_key = "print";
    char* fun_key = "fun";

    char* pt = current.ptr;
    current.kind = NONE;

    int is_while = 1;
    int is_print = 1;
    int is_if = 1;
    int is_else = 1;
    int is_fun = 1;
    if(*pt == '\0'){
	current.kind = END;
    }
    else if(*pt == '='){
	if(*(pt+1) == '='){
		current.kind = EQEQ;
    	}
	else{
		current.kind = EQ;
	}
    }
    else if(*pt == '('){
	pt++;
	while(*pt == ' ' || *pt == '\t' || *pt == '\n'){
		pt++;
	}
	if(*pt == ')') current.kind = CALL;
	else{
		current.kind = LEFT;
	}
    }
    else if(*pt == ')'){
	current.kind = RIGHT;
    }
    else if(*pt == '{'){
	current.kind = LBRACE;
    }    
    else if(*pt == '}'){
	current.kind = RBRACE;
    }
    else if(*pt == '+'){
	current.kind = PLUS;
    }
    else if(*pt == '*'){
	current.kind = MUL;
    }
    else if(*pt == ';'){
	current.kind = SEMI;
    }
    else if(*pt >= 'a' && *pt <= 'z'){
	for(int i = 0; i < 5; i++){
	    	if(pt[i] == '\0' || pt[i] != while_key[i]){
			//printf("1\n");
			current.kind = ID;
			is_while = 0;
	    	}	    
	}
	if(is_while && (pt[5] == ' ' || pt[5] == '(')){
	    	current.kind = WHILE;
		//printf("=========================");
	}
	else{	
	    	current.kind = ID;
	}
	if(current.kind != WHILE){
	    	for(int i = 0; i < 2; i++){
			if(pt[i] == '\0' || pt[i] != if_key[i]){
		    		current.kind = ID;
				is_if = 0;
			}		
	    	}
		if(!('a' <= pt[2] && pt[2] <= 'z') && !('0' <= pt[2] && pt[2] <= '9')&& is_if){
			current.kind = IF;
	    	}
	   	else{
	    		current.kind = ID;
			//printf("4\n");
	    	}
	}
	if(current.kind != WHILE && current.kind != IF){
	    	for(int i = 0; i < 4; i++){
			if(pt[i] == '\0' || pt[i] != else_key[i]){
		    		current.kind = ID;
				is_else = 0;
			}
	    	}		
			if(!('a' <= pt[4] && pt[4] <= 'z') && !('0' <= pt[4] && pt[4] <= '9')&& is_else){
				current.kind = ELSE;
	    		}	
	    		else{
				current.kind = ID;
			//	printf("6\n");
	    		}	
		}
	if(current.kind != WHILE && current.kind != IF && current.kind != ELSE){
	    	for(int i = 0; i < 5; i++){
			//printf("%c\n", last_ptr[i]);
			if(pt[i] == '\0' || pt[i] != print_key[i]){
		    		current.kind = ID;
				is_print = 0;
			}
	    	}	
	   		if(!('a' <= pt[5] && pt[5] <= 'z') && !('0' <= pt[5] && pt[5] <= '9')&& is_print){
				current.kind = PRINT; 
	    		}
	    		else{
				current.kind = ID;
	    		}	
		}
	if(current.kind != WHILE && current.kind != IF && current.kind != ELSE && current.kind != PRINT){
	    	for(int i = 0; i < 3; i++){
			//printf("%c\n", last_ptr[i]);
			if(pt[i] == '\0' || pt[i] != fun_key[i]){
		    		current.kind = ID;
				is_fun = 0;
			}
	    	}	
	   		if(!('a' <= pt[3] && pt[3] <= 'z') && !('0' <= pt[3] && pt[3] <= '9')&& is_fun){

				current.kind = FUNC; 
	    		}
	    		else{
				current.kind = ID;
	    		}	
		}

	
    }	
    else if(*pt >= '0' && *pt <= '9'){
        while((*pt >= '0' && *pt <= '9') || *pt == '_'){
            pt++;
        }
        current.kind = INT;
    }
    
    return current.kind;
    
}

void consume() {
    current.kind = peek();
    if(current.kind == EQ || current.kind == LEFT || current.kind == RIGHT || current.kind == LBRACE || current.kind == RBRACE
		    || current.kind == PLUS || current.kind == MUL || current.kind == SEMI){
			current.ptr++;
    }
    else if(current.kind == EQEQ){
	current.ptr +=2;
    }
    else if(current.kind == WHILE || current.kind == PRINT){
		current.ptr += 5;
    }
    else if(current.kind == IF){
		current.ptr += 2;
    }
    else if(current.kind == FUNC){
		current.ptr += 3;
    }
    else if(current.kind == ELSE){
		current.ptr += 4;
    }
    else if(current.kind == CALL){
	while(*current.ptr != ')'){
		current.ptr++;
	}
	current.ptr++;
    }
    else if(current.kind == INT){
	current.value = 0;
	while((*current.ptr >= '0' && *current.ptr <= '9') || *current.ptr == '_'){
		if(*current.ptr != '_'){
			current.value = current.value * 10 + (*current.ptr++ - '0');
		}
		else{
			current.ptr++;
		}
	}
    }
    else if(current.kind == ID){
	current.start = current.ptr;
	int count = 0;
	while(*current.ptr != '\0' 
			&& ((('a' <= *current.ptr) && (*current.ptr <= 'z'))
				||(('0' <= *current.ptr) && (*current.ptr <= '9')))){
		count++;
		current.ptr++;
	}
	current.id = (char*)malloc(count + 1);
	
	for(int i = 0; i < count; i++){
		current.id[i] = current.start[i];
	}
	current.id[count] = '\0';
    }
    while((*current.ptr == ' ' || *current.ptr == '\n' || *current.ptr == '\t') && *current.ptr != '\0'){
	    current.ptr++;
    }
    current.start = current.ptr;
}

char *getId(void) {
    char *temp_id = current.id;
    return temp_id;
}

uint64_t getInt(void) {
    if(current.kind == ID) return get(current.id);
    return current.value;
}

uint64_t expression(void);
void seq(int doit);

/* handle id, literals, and (...) */
uint64_t e1(void) {
    if (peek() == LEFT) {
        consume();
        uint64_t v = expression();
        if (peek() != RIGHT) {
	    printf("error break 3: \n");
            error();
        }
        consume();
        return v;
    } else if (peek() == CALL){
	consume();
   	return getInt();
    } else if (peek() == INT) {
	    consume();
        uint64_t v = getInt();
        return v;
    } else if (peek() == ID) {
	    consume();
        char *id = getId();
        insertData(id);
        return get(id);
    } else {
	printf("error break 2:%i \n", peek());
        error();
        return 0;
    }
}

/* handle '*' */
uint64_t e2(void) {
    uint64_t value = e1();
    while (peek() == MUL) {
        consume();
        value = value * e1();
    }
    return value;
}

/* handle '+' */
uint64_t e3(void) {
    uint64_t value = e2();
    while (peek() == PLUS) {
        consume();
        value = value + e2();
    }
    return value;
}

/* handle '==' */
uint64_t e4(void) {
    uint64_t value = e3();
    while (peek() == EQEQ) {
        consume();
        value = value == e3();
    }
    return value;
}

uint64_t expression(void) {
    return e4();
}

uint64_t statement(int doit) {
    switch(peek()) {
    case ID: {
        consume();
        char *id = getId();
        insertData(id);
        if(peek() == CALL){
            consume();
            if(doit){
            char* temp = current.ptr;
            current.ptr = (char*)get(current.id);
            statement(doit);
            current.ptr = temp;
            }
	    }   
        else{
            consume();  
        if(peek() == FUNC){
            consume();
            if(doit){
            uint64_t v = (uintptr_t)current.ptr;
            set(id, v);
            }
            statement(0);
        }
        else{
                uint64_t v = expression();
                if (doit){
                    
                    storeInt(id, v);
                    set(id, v);
                }	
                if (peek() == SEMI) {
                    consume();
                }
            }
        }
        return 1;
    }
    case LBRACE:
        consume();
        seq(doit);
        if (peek() != RBRACE){
           	printf("error break 1 \n");
	       	error();
		
	}
        consume();
        return 1;
    case IF: {
       	consume();
	if(expression()){
		statement(doit);
		if(peek() == ELSE){
			consume();
			statement(0);
		}
	}
	else{
		statement(0);
		if(peek() == ELSE){
			consume();
			statement(doit);
		}
	}
        return 1;
    }
    case WHILE: {
        consume();
	char*temp = current.ptr;
	if(!doit){
	    expression();
	}
	else{
		while(expression()){
	    		statement(doit);
	    		current.ptr = temp;
		}
	}
	statement(0);
        return 1;
    }
    case PRINT:{
        consume();
        if(doit){
            
            uint64_t val = expression();
            printf("================%lu",current.value);
            if(current.id == NULL){
                printVal(current.value);
            }
            else
		        printKey(current.id);
	    }
	    else {
		    expression();
	    }
        return 1;
	}
    case SEMI:
        consume();
        return 1;
    default:
        return 0;
    }
}

void seq(int doit) {
    while (statement(doit)) ;
}

void program(void) {
    seq(1);
    if (peek() != END){
        error();
    }
}

void interpret(char *prog) {
    current.kind = NONE;
    current.start = prog;
    current.ptr = prog;
    int x = setjmp(escape);
    if (x == 0) {
        program();
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr,"usage: %s <name>\n",argv[0]);
        exit(1);
    }

    char* name = argv[1];
    size_t len = strlen(name);

    size_t sLen = len+3;  // ".s" + 0
    char* sName = (char*) malloc(sLen);
    if (sName == 0) {
        perror("malloc");
        exit(1);
    }



    strncpy(sName,name,sLen);
    strncat(sName,".s",sLen);



    size_t fLen = len+5; // ".fun" + 0
    char* fName = (char*) malloc(fLen);
    if (sName == 0) {
        perror("malloc");
        exit(1);
    }
    strncpy(fName,name,fLen);
    strncat(fName,".fun",fLen);

    printf("compiling %s to produce %s\n",fName,sName);

    f = fopen(sName,"w");
    
    if (f == 0) {
        perror(sName);
        exit(1);
    }
    

    FILE* input = fopen(fName, "r");
    fseek(input, 0, SEEK_END);
    int length = ftell(input);

    fseek(input, 0, SEEK_SET);

    
    /*     fputs("    .data\n",f);
    fputs("format: .byte '%', 'd', 10, 0\n",f);
    fputs("    .text\n",f);
    fputs("    .global main\n",f);
    fputs("main:\n",f);
    fputs("    mov $0,%rax\n",f);
    fputs("    lea format(%rip),%rdi\n",f);
    fputs("    mov $42,%rsi\n",f);
    fputs("    .extern printf\n",f);
    fputs("    call printf\n",f);
    fputs("    mov $0,%rax\n",f);
    fputs("    ret\n",f); */
    
    putData("    .data\n");
    putData("format: .byte '%', 'd', 10, 0\n");
    putData("v_argc: .quad 0\n");
    putCode("    .text\n");
    putCode("    .global main\n");
    putCode("main:\n");
    putCode("    mov %rdi, v_argc(%rip)\n");

    char* begin = (char*)malloc(length + 1);
    int counter = 0;
    char curChar = fgetc(input);
    while(curChar != EOF){
	    begin[counter] = curChar;
	    curChar = fgetc(input);
	    counter++;
    }
    begin[counter] = '\0';
    while(*begin == ' '|| *begin == '\n' || *begin == '\t') begin++;
    interpret(begin);

    putCode("    mov $0,%rax\n");
    putCode("    ret\n");

    writeCode();

    fclose(f);

    size_t commandLen = len*2 + 1000;
    char* command = (char*) malloc(commandLen); 

    snprintf(command,commandLen,"gcc -static -o %s.exec %s",name,sName);
    //snprintf(command,commandLen,"gcc -o %s.exec %s",name,sName);
    printf("compiling %s to produce %s.exec\n",sName,name);
    printf("running %s\n",command);
    int rc = system(command);
    if (rc != 0) {
        perror(command);
        exit(1);
    }

    return 0;
}
