#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include<ctype.h>
#define SIZE 300000

struct text {
    char *line;
  struct  text *next;
};

typedef struct text* header;

void insert_header(header* linepointer , char* toadd) {
    struct text* temp=malloc(sizeof(struct text));
        (temp)->next=NULL;
        (temp)->line=strdup(toadd);
        *linepointer=temp;
}

void modify(int position, header* linepointer , char* toadd) { // linepointer -> current line in the text | toadd->the new line we want to be added into the text. OVERWRITE || NEWLINE

    if(position==1){
        if((*linepointer)==NULL){
            insert_header(linepointer,toadd);
        }
        else{
            if((*linepointer)->line!=NULL)
            free((*linepointer)->line);
            (*linepointer)->line=strdup(toadd);
        }
        return;
    }
    header current = *linepointer;

    while(position!=2){
        position--;
        current = current->next;
    }

    if(current->next==NULL) {
        insert_header(&(current->next), toadd);
    }
    else {
        if(current->next->line!=NULL)
        free(current->next->line);
        current->next->line=strdup(toadd);
    }
}

void printlistaFileStrategy2(header linepointer,int start,int end){
  if(start==0){
      start++;
    fputs(".\n", stdout); }

    while(linepointer!=NULL&&end>=1){
      if(start==1){
fputs((linepointer)->line,stdout);

                linepointer = (linepointer)->next;
                end--;
    }else{
        start--;
        linepointer = (linepointer)->next;
        end--;
    }

    }
    while(start!=1){
        start--;
        end--;
    }
    if(end>0){
        int i;
        for( i=0;i<end;i++)
        fputs(".\n", stdout);
    }

}

void printlista(header linepointer){
    while(linepointer!=NULL){
fputs((linepointer)->line,stdout);

                linepointer = (linepointer)->next;
    }
}
void delete(int start, int end, header* linepointer)
{
        header to_delete;
    if(start==1) {
        to_delete=*linepointer;
        int i;

        for( i=1;i<=end;i++) {
            if(to_delete==NULL) {
             break;
         }
            *linepointer=(*linepointer)->next;
            if(to_delete!=NULL&&to_delete->line!=NULL)
            free(to_delete->line);
            if(to_delete!=NULL)
            free(to_delete);

            to_delete=*linepointer;

        }

        return;
    }
    if( *linepointer==NULL){

        return;
    }
    header prev = NULL;
    header curr = *linepointer;
    while(curr!=NULL && start!=1)
    {

        prev=curr;
        curr=curr->next;
        start--;
        end--;
    }
    to_delete = curr;
    int i;
    for( i=start;i<=end;i++)
     {
         if(to_delete==NULL) {
             break;
         }
         curr=curr->next;
         if(to_delete!=NULL&&to_delete->line!=NULL)
        free(to_delete->line);
        if(to_delete!=NULL)
        free(to_delete);
         to_delete=curr;
    }

if(prev!=NULL){
    prev->next=to_delete;}

}

void deleteAll(header* lineheader) {
    header to_delete;
    while(*lineheader!=NULL) {
         to_delete=*lineheader;
        *lineheader=(*lineheader)->next;
        if(to_delete!=NULL&&to_delete->line!=NULL)
            free(to_delete->line);
            if(to_delete!=NULL)
            free(to_delete);

    }
    *lineheader=NULL;
}

void duplicate(header* original, header* duplicated) {
    if(*original!=NULL) {
        insert_header(duplicated,(*original)->line);
        duplicate(&((*original)->next),&((*duplicated)->next));
    }
}


struct state {
    header text_list;
  struct  state *next;
  struct state *prev;
};

typedef struct state* current;
/*
void insert_header_current(current* linepointer , int toadd) {
    struct state* temp=malloc(sizeof(struct state));

        (temp)->next=NULL;
        (temp)->prev=*linepointer;
        *linepointer=temp;
}*/
void deleteAllNextStates(current* currentState) {
    current to_delete;
    current curr;
        if(*currentState==NULL){
            return;
        }
        to_delete=(*currentState)->next;
        (*currentState)->next=NULL;
    while(to_delete!=NULL) {
        curr=to_delete->next;
        deleteAll(&(to_delete->text_list));
        free(to_delete);
        to_delete=curr;
    }
}

void insert_state(current* currentState) {
    deleteAllNextStates(currentState);
    struct state* newState=malloc(sizeof(struct state ));
    newState->text_list=NULL;

    duplicate(&((*currentState)->text_list),&(newState->text_list));
    newState->next=NULL;
    newState->prev=*currentState;
    (*currentState)->next=newState;
    *currentState=newState;
}

void modifyState(current* currentState, int pos,char* newString) {
    modify(pos,&((*currentState)->text_list),newString);
}

void printListOfStates(current startingState) {
    current previous=startingState->prev;
    while (previous!=NULL) {
        printlista(previous->text_list);
        previous=previous->prev;
    }

    while (startingState!=NULL) {


        printlista(startingState->text_list);
    startingState=startingState->next;
    }

}


void undoStates(current* currentState, int n) {
    int i;
    for( i=0;i<n&& (*currentState)->prev!=NULL;i++,*currentState=(*currentState)->prev);
}

void redoStates(current* currentState, int n) {
        int i;
        for( i=0;i<n&& (*currentState)->next!=NULL;i++,*currentState=(*currentState)->next);

}


void handleCStrategy2(current* currentState,int start,int end,FILE* input){
  char line_toAdd[1024];
  insert_state(currentState);
  int i;
  for( i=start;i<=end;i++){
    fgets(line_toAdd,1024,input);
    modify(i,&((*currentState)->text_list),line_toAdd);
  }
  fgets(line_toAdd,1024,input);

}

void handleD(current* currentState,int start,int end){
  insert_state(currentState);

  delete(start,end,&((*currentState)->text_list));
}

void handleR(current* currentState,int n){
 redoStates(currentState,n);
}

void handleU(current* currentState,int n){
    undoStates(currentState,n);
}

void handlePStrategy2(current currentState,int start,int end){
   printlistaFileStrategy2(currentState->text_list,start,end);
}



struct textMat {
    char *line[SIZE];
  int numline;
};

typedef struct textMat* headerMat;

headerMat globalText;


void modifyWrapper(int start,int end,headerMat* linepointer , FILE* toread){
char line[1024];
int originalLines=(*linepointer)->numline;
if((*linepointer)->numline<end){
	(*linepointer)->numline=end;
}
	int i;
   for(i=start;i<=end; i++){
		fgets(line,1024,toread);
		if(i<=originalLines)
			free((*linepointer)->line[i]);
		(*linepointer)->line[i]=strdup(line);
	}

}
void printlistaFile(headerMat linepointer,int start,int end,FILE* fp){
    if(start==0){
    	fputs(".\n",fp);
        start++;
        }
    int i;
    for(i=start;i<=(linepointer)->numline&&i<=end; i++){
    	fputs((linepointer)->line[i],fp);
    }
    for(i=(linepointer)->numline+1;i<=end; i++){
        	fputs(".\n",fp);
        }

}


void handleP(headerMat text,int start,int end,FILE * output){
   printlistaFile(text,start,end,output);
}


void deleteAllMatrix(char** lineheader,int end) {
    int i;
	for( i=1;i<=end;i++)
        	free(lineheader[i]);
}
char *trim(char *s) {
    char *ptr;
    if (!s)
        return NULL;   // handle NULL string
    if (!*s)
        return s;      // handle empty string
    for (ptr = s + strlen(s) - 1; (ptr >= s) && isspace(*ptr); --ptr);
    ptr[1] = '\0';
    return s;
}

void handleC(headerMat* currentState,int start,int end,FILE* input){
  char line_toAdd[1024];
  modifyWrapper(start,end,currentState,input);
  fgets(line_toAdd,sizeof(line_toAdd),input);
}
void handleCMatrix(headerMat* linepointer,int start,int end,char** input){
	int originalLines=(*linepointer)->numline;
	if((*linepointer)->numline<end){
		(*linepointer)->numline=end;
	}
		int i;
	   for(i=start;i<=end; i++){
	    if(i<=originalLines)
	    	free((*linepointer)->line[i]);
	    (*linepointer)->line[i]=strdup(input[i-start+1]);
	}


}

int getStrategy(FILE* input){
	char line[1024];
	 int start,end;
	while (fgets(line,99,input) !=NULL) {
	    char command=trim(line)[strlen(trim(line))-1];


	    switch(command){
	                  case 'c':{
	                	 sscanf(line,"%d,%d%c",&start,&end,&command);
	                	 int i;
	                	 for(i=start;i<=end; i++)
	                	 		fgets(line,1024,input);
	                	 fgets(line,sizeof(line),input);
	                	 break;

	                    }
	                  case 'q':{
	                	  rewind(input);
	                   return 1;
	                  	                    break;
	                  	                  }
	                  case'p':
	                	  break;

	                    default :{
	                    	rewind(input);
	                    	return 0;
	                    }

	                  }



	            }
	rewind(input);
		       return 1;
}

int main() {
	current currentTestState =malloc(sizeof(struct state));
	currentTestState->prev = NULL;
	currentTestState->next = NULL;
	currentTestState->text_list=NULL;
	globalText=malloc(sizeof(struct textMat));
	globalText->numline=0;
	FILE *input;
	input=stdin;
	input=fopen("Write_Only_1_input.txt","r");
	FILE *output;
	output=stdout;
	output=fopen("output.txt","w");
	char line[100];
	int start,end;
	int strategy=getStrategy(input);
	    while (fgets(line,99,input) !=NULL) {
                    char command=trim(line)[strlen(trim(line))-1];

        switch(command){
              case 'c':{

                sscanf(line,"%d,%d%c",&start,&end,&command);
                if(strategy==1)
                handleC(&globalText,start,end,input);
                else
                handleCStrategy2(&currentTestState,start,end,stdin);
                            break;
                            }

                            case 'd':{
                              int start,end;
                            sscanf(line,"%d,%d%c",&start,&end,&command);
                              handleD(&currentTestState,start,end);
                                break;
                            }


                          case 'u':{
                             int start;
                            sscanf(line,"%d%c",&start,&command);
                            handleU(&currentTestState,start);
                            break;
                          }

                          case 'r':{
                          int start;
                            sscanf(line,"%d%c",&start,&command);
                            handleR(&currentTestState,start);
                          break;
                          }
                          case 'p':{
                            int start,end;
                            sscanf(line,"%d,%d%c",&start,&end,&command);
                            if(strategy==1)
                            	handleP(globalText,start,end,output);
                            else
                            	handlePStrategy2(currentTestState, start, end);
                            break;
                          }

                          case 'q':{
                            return 0;
                          }


        }
        }

	    //printf("printing undo\n");
	    //printAllCommands(commandUndo);
       // printf("%d %d",commandStart,commandLast);
    return 0;
}

