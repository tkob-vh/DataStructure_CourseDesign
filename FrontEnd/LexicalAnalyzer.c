/********************************************************************
* FILE: LexicalAnalyzer.c
* DESCRIPTION:
* This file is used to analyze the lexical of the source file using FSM.
* 
* AUTHOR: yyx
* LASTREVISED: 2021.8.14
********************************************************************/

#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include "GlobalVar.h"


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
        //+ - * / % // /* */
        {
        case '+':
            token_text[token_counter][0]=c;
            token_text[token_counter][1]='\0';
            token_counter++;
            return PLUS;
        case '-':
            token_text[token_counter][0]=c;
            token_text[token_counter][1]='\0';
            token_counter++;
            return MINUS;
        case '*':
            token_text[token_counter][0]=c;
            token_text[token_counter][1]='\0';
            token_counter++;
            return MULTIPLY;
        case '/': //need modified!!!
            token_text[token_counter][0]=c;
            token_text[token_counter][1]='\0';
            token_counter++;
            return DIVIDE;
        case '%':
            token_text[token_counter][0]=c;
            token_text[token_counter][1]='\0';
            token_counter++;
            return MOD;
        }
        // = == != > >= < <=
        {
        case '=':
            token_text[token_counter][0]=c;
            c=fgetc(source_file);
            if(c=='='){
                token_text[token_counter][1]=c;
                token_text[token_counter][2]='\0';
                token_counter++;
                return EQ;
            }
            else{
                token_text[token_counter][1]='\0';
                ungetc(c,source_file);
                token_counter++;
                return ASSIGN;
            }
        case '!':
            token_text[token_counter][0]=c;
            c=fgetc(source_file);
            if(c=='='){
                token_text[token_counter][1]=c;
                token_text[token_counter][2]='\0';
                token_counter++;
                return NEQ;
            }
            else{
                token_text[token_counter][1]='\0';
                ungetc(c,source_file);
                token_counter++;
                return ERROR_TOKEN;
            }
        case '>':
            token_text[token_counter][0]=c;
            c=fgetc(source_file);
            if(c=='='){
                token_text[token_counter][1]=c;
                token_text[token_counter][2]='\0';
                token_counter++;
                return GREATER_EQ;
            }
            else{
                token_text[token_counter][1]='\0';
                ungetc(c,source_file);
                token_counter++;
                return GREATER;
            }
        case '<':
            token_text[token_counter][0]=c;
            c=fgetc(source_file);
            if(c=='='){
                token_text[token_counter][1]=c;
                token_text[token_counter][2]='\0';
                token_counter++;
                return LESS_EQ;
            }
            else{
                token_text[token_counter][1]='\0';
                ungetc(c,source_file);
                token_counter++;
                return LESS;
            }
        }
        // { } [ ] ( ) ; #
        {
        case '{':
            token_text[token_counter][0]=c;
            token_text[token_counter][1]='\0';
            token_counter++;
            return LBRACE;
        case '}':
            token_text[token_counter][0]=c;
            token_text[token_counter][1]='\0';
            token_counter++;
            return RBRACE;
        case '[':
            token_text[token_counter][0]=c;
            token_text[token_counter][1]='\0';
            token_counter++;
            return LBRACKET;
        case ']':
            token_text[token_counter][0]=c;
            token_text[token_counter][1]='\0';
            token_counter++;
            return RBRACKET;
        case '(':
            token_text[token_counter][0]=c;
            token_text[token_counter][1]='\0';
            token_counter++;
            return LP;
        case ')':
            token_text[token_counter][0]=c;
            token_text[token_counter][1]='\0';
            token_counter++;
            return RP;
        case ';':
            token_text[token_counter][0]=c;
            token_text[token_counter][1]='\0';
            token_counter++;
            return SEMICOLON;
        case '#':
            token_text[token_counter][0]=c;
            token_text[token_counter][1]='\0';
            token_counter++;
            return SHARP;
    }
        // && ||
        {
        case '&':
            token_text[token_counter][0]=c;
            c=fgetc(source_file);
            if(c=='&'){
                token_text[token_counter][1]=c;
                token_text[token_counter][2]='\0';
                token_counter++;
                return AND;
            }
            else{
                token_text[token_counter][1]='\0';
                ungetc(c,source_file);
                token_counter++;
                return ERROR_TOKEN;
            }
        case '|':
            token_text[token_counter][0]=c;
            c=fgetc(source_file);
            if(c=='|'){
                token_text[token_counter][1]=c;
                token_text[token_counter][2]='\0';
                token_counter++;
                return OR;
            }
            else{
                token_text[token_counter][1]='\0';
                ungetc(c,source_file);
                token_counter++;
                return ERROR_TOKEN;
            }
        }

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
    int t_kind;
    printf("Lexeme Kind\t\tLexeme Value\n");
    do{
        t_kind=gettoken(source_file,ch);
        if(t_kind==ERROR_TOKEN){
            printf("Error: line %d: The token is not recognized.\n",line_counter);
            return 0;
        }
        else if(t_kind==LINECOMMENT){
            printf("Line comment.\n");
        }
        else if(t_kind==BLOCKCOMMENT){
            printf("Block comment.\n");
        }
        else{
            printf("%d\t\t%s\n",t_kind,token_text[token_counter-1]);
        }

    }while(t_kind!=END_OF_FILE);
    
    fclose(source_file);
    return 1;

}

