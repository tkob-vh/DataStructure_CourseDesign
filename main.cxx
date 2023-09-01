/********************************************************************
* FILE: main.cxx 
* DESCRIPTION: 
* This file is used to test the lexical analysis, syntax analysis and 
* intermediate code generation.
* AUTHOR: yyx
* ID: U202215545
********************************************************************/
#include<stdio.h>
#include<ctype.h>
#include<string.h>

#include "var.hh"
#include "scanner.hh"
#include "parser.hh"
#include "code_gen.hh"
#include "tree.hh"


//global variables
Token_kind token_kind; // store the token kind
char token_text[1000][20]; // store the token text such as identifier, literal, etc.
char t_kinds[1000][20]; // store the token kind such as ID, INT_LITERAL, etc.
int line_info[1000]; // store the line number of each token
size_t line_counter = 1; // count the line number
size_t token_counter = 0; // count the token number in Scanner.c
size_t token_counter1 = 0; // count the token number in Parser.c
char Keywords[16][10]={"int","float","char","void","if","else","while","return","for","do","break","continue","define","include","long","double"};//store the main keywords
char Capital_Keywords[16][10]={"INT","FLOAT","CHAR","VOID","IF","ELSE","WHILE","RETURN","FOR","DO","BREAK","CONTINUE","DEFINE","INCLUDE","LONG","DOUBLE"};//store the main keywords
char w[20]; //store the token kind of contomparary token in Parser.c
bool error_flag; // once error_flag is 1, release the memory of AST .
int errors=0;
char temp_kind[20];
char temp_text[20];
char TK[51][15]={"ERROR_TOKEN","ID","INT_LITERAL","FLOAT_LITERAL","CHAR_LITERAL","INT","FLOAT","CHAR","VOID","IF","ELSE","WHILE","RETURN","FOR","DO","BREAK","CONTINUE","DEFINE","INCLUDE","LONG","DOUBLE","SHARP","LINECOMMENT","BLOCKCOMMENT","PLUS","MINUS","MULTIPLY","DIVIDE","MOD","ASSIGN","EQ","NEQ","GREATER","LESS","GREATER_EQ","LESS_EQ","LBRACE","RBRACE","LBRACKET","RBRACKET","LP","RP","AND","OR","SEMICOLON","SINGLE_QUOTE","DOUBLE_QUOTE","COMMA","END_OF_FILE","LONG_LITERAL","DOUBLE_LITERAL"};
char function_list[100][20];//store the function name
int function_counter;
int brace_counter;
char op_table[17][17]={
            //+     -     *     /     %     =    ==    !=     >     <    >=    <=    &&    ||     (     )     #
/* + */     {'>',  '>',  '<',  '<',  '<',  '<',  '<',  '<',  '>',  '>',  '>',  '>',  '<',  '<',  '<',  '>',  '>'},
/* - */     {'>',  '>',  '<',  '<',  '<',  '<',  '<',  '<',  '>',  '>',  '>',  '>',  '<',  '<',  '<',  '>',  '>'},
/* * */     {'>',  '>',  '>',  '>',  '>',  '<',  '<',  '<',  '>',  '>',  '>',  '>',  '<',  '<',  '<',  '>',  '>'},
/* / */     {'>',  '>',  '>',  '>',  '>',  '<',  '<',  '<',  '>',  '>',  '>',  '>',  '<',  '<',  '<',  '>',  '>'},
/* % */     {'>',  '>',  '>',  '>',  '>',  '<',  '<',  '<',  '>',  '>',  '>',  '>',  '<',  '<',  '<',  '>',  '>'},
/* = */     {'<',  '<',  '<',  '<',  '<',  '=',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  ' ',  '>'},
/* == */    {'<',  '<',  '<',  '<',  '<',  '<',  '=',  '=',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '>',  '>'},
/* != */    {'<',  '<',  '<',  '<',  '<',  '<',  '=',  '=',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '>',  '>'},
/* > */     {'>',  '>',  '>',  '>',  '>',  '<',  '<',  '<',  '>',  '>',  '>',  '>',  '<',  '<',  '<',  '>',  '>'},
/* < */     {'>',  '>',  '>',  '>',  '>',  '<',  '<',  '<',  '>',  '>',  '>',  '>',  '<',  '<',  '<',  '>',  '>'},
/* >= */    {'>',  '>',  '>',  '>',  '>',  '<',  '<',  '<',  '>',  '>',  '>',  '>',  '<',  '<',  '<',  '>',  '>'},
/* <= */    {'>',  '>',  '>',  '>',  '>',  '<',  '<',  '<',  '>',  '>',  '>',  '>',  '<',  '<',  '<',  '>',  '>'},
/* && */    {'>',  '>',  '>',  '>',  '>',  '<',  '<',  '<',  '>',  '>',  '>',  '>',  '>',  '>',  '<',  '>',  '>'},
/* || */    {'>',  '>',  '>',  '>',  '>',  '<',  '<',  '<',  '>',  '>',  '>',  '>',  '>',  '>',  '<',  '>',  '>'},
/* ( */     {'<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '=',  ' '},
/* ) */     {'>',  '>',  '>',  '>',  '>',  ' ',  ' ',  ' ',  '>',  '>',  '>',  '>',  ' ',  ' ',  ' ',  '>',  '>'},
/* # */     {'<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  ' ',  '='}
};

char table_arr[17][15]={"PLUS","MINUS","MULTIPLY","DIVIDE","MOD","ASSIGN","EQ","NEQ","GREATER","LESS","GREATER_EQ","LESS_EQ","AND","OR","LP","RP","SHARP"};

tree<std::string> program_tree;
tree<std::string>::iterator program_root;
tree<std::string> preprocessing_list_tree;
tree<std::string> external_defination_list_tree;
tree<std::string>::iterator it;
tree<std::string> empty_tree;


int main(){
    
    //open the file which is going to be scanned
    char filename[100];
    char input[50];
    strcpy(filename,"./test/TestRoutine.cxx");
    FILE *fp;
    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Error opening file in scanner.\n");
        return false;
    }

    //lexical analysis
    scanner(fp,filename);
 
    //syntax analysis
    program_root=program();
    printTree();

    //intermediate code generation
    program_code();

    fclose(fp);
    
    return 0;
}

