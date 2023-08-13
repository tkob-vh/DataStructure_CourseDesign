/********************************************************************
* FILE: LexicalAnalyzer.c
* DESCRIPTION:
* This file is used to analyze the lexical of the source file using FSM.
* AUTHOR: yyx
* LASTREVISED: 2021.8.13
********************************************************************/

#include<stdio.h>
#include<ctype.h>
#include<string.h>


// The token kind
enum token_kind
{ERROR_TOKEN,
ID,
INT_LITERAL,
FLOAT_LITERAL,
CHAR_LITERAL,

INT,
FLOAT,
CHAR,
VOID,
IF,
ELSE,
WHILE,
RETURN,
FOR,
DO,

PLUS,
MINUS,
MULTIPLY,
DIVIDE,
MOD,
ASSIGN,
EQ,
NEQ,
LBRACE,
RBRACE,
LBRACKET,
RBRACKET,
LP,
RP,
COMMA,
SEMICOLON,
SINGLE_QUOTE,
DOUBLE_QUOTE,
END_OF_FILE,
};


//global variables
char token_text[500][10]; // store the token text such as identifier, literal, etc.
size_t line_counter = 1; // count the line number
size_t token_counter = 0; // count the token number
char Keywords[10][10]={"int","float","char","void","if","else","while","return","for","do"};//store the main keywords




//Lookup the keyword and return its token kind, else return ID
int LookupKeyword(int token_num){
    int i;
    for(i=0;i<10;i++){
        if(strcmp(token_text[token_num],Keywords[i])==0){
            return i+5;
        }
    }
    return ID;
}

//get the token from the source file  and return the token kind
int gettoken(FILE * source_file,char c){
    //skip the space, tab, newline and carriage return
    while((c=fgetc(source_file))==' '||c=='\t'||c=='\n'||c=='\r'){
        if(c=='\n'){
            line_counter++;
        }
    }
    
    //need modified!!!
    if(c==EOF){
        return END_OF_FILE;
    }
    
    //If c is a letter or underscore, it is an identifier or keyword
    if(isalpha(c)||c=='_'){
        int i=0;
        do{
            token_text[token_counter][i++]=c;
            c=fgetc(source_file);
        }while(isalpha(c)||isdigit(c)||c=='_');
        ungetc(c,source_file);
        token_text[token_counter][i]='\0';
        token_counter++;
        return LookupKeyword(token_counter-1);
    }

    //If c is a digit, it is an integer or float literal
    else if(isdigit(c)){
        int i=0;
        
        do{
            token_text[token_counter][i++]=c;
            c=fgetc(source_file);
        }while(isdigit(c)||c=='.'); //without considering 0x form such as 0x123,and suffix form such as 123L
        ungetc(c,source_file);
        token_text[token_counter][i]='\0';
        token_counter++;
        if(strchr(token_text[token_counter-1],'.')!=NULL){
            return FLOAT_LITERAL;
        }
        else{
            return INT_LITERAL;
        }
    }
    
    
    //If c is a character, it is a char literal
    else if(c=='\''){
        int i=0;
        do{
            token_text[token_counter][i++]=c;
            c=fgetc(source_file);
        }while(c!='\'');
        token_text[token_counter][i++]=c;
        token_text[token_counter][i]='\0';
        if(i>3){
            printf("Error: line %d: The char literal is too long.\n",line_counter);
            return ERROR_TOKEN;
        }
        token_counter++;
        return CHAR_LITERAL;
    }
 
    //If c is a operator
    switch(c){
        case '+':
            token_text[token_counter][0]=c;
            token_text[token_counter][1]='\0';
            token_counter++;
            return PLUS;
    }
}

//The function used to scan the source file and return the message.
int scanner(FILE * source_file){
    source_file = fopen("TestRoutine.c", "r");
    if (source_file == NULL)
    {
        printf("Error opening file\n");
        return 0;
    };
    char ch;
    gettoken(source_file,ch);

}