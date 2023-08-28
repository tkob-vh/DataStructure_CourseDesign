/********************************************************************
* FILE: main.cxx 
* DESCRIPTION:
* AUTHOR: yyx
********************************************************************/
#include<stdio.h>
#include<ctype.h>
#include<string.h>

#include "var.hh"
#include "Scanner.hh"
#include "Parser.hh"
#include "IRGen.hh"
#include "tree.hh"

Token_kind token_kind; // store the token kind
//global variables
char token_text[1000][20]; // store the token text such as identifier, literal, etc.
char t_kinds[1000][20]; // store the token kind such as ID, INT_LITERAL, etc.
int line_info[1000]; // store the line number of each token
size_t line_counter = 1; // count the line number
size_t token_counter = 0; // count the token number in Scanner.c
size_t token_counter1 = 0; // count the token number in Parser.c
char Keywords[14][10]={"int","float","char","void","if","else","while","return","for","do","break","continue","define","include"};//store the main keywords
char Capital_Keywords[14][10]={"INT","FLOAT","CHAR","VOID","IF","ELSE","WHILE","RETURN","FOR","DO","BREAK","CONTINUE","DEFINE","INCLUDE"};//store the main keywords
char w[20]; //store the token kind of contomparary token in Parser.c
bool error_flag; // once error_flag is 1, release the memory of AST .
int errors=0;
char temp_kind[20];
char temp_text[20];
char TK[47][15]={"ERROR_TOKEN","ID","INT_LITERAL","FLOAT_LITERAL","CHAR_LITERAL","INT","FLOAT","CHAR","VOID","IF","ELSE","WHILE","RETURN","FOR","DO","BREAK","CONTINUE","DEFINE","INCLUDE","SHARP","LINECOMMENT","BLOCKCOMMENT","PLUS","MINUS","MULTIPLY","DIVIDE","MOD","ASSIGN","EQ","NEQ","GREATER","LESS","GREATER_EQ","LESS_EQ","LBRACE","RBRACE","LBRACKET","RBRACKET","LP","RP","AND","OR","SEMICOLON","SINGLE_QUOTE","DOUBLE_QUOTE","COMMA","END_OF_FILE"};


char op_table[17][17]={
            //+     -     *     /     %     =    ==    !=     >     <    >=    <=    &&    ||     (     )     #
/* + */     {'>',  '>',  '<',  '<',  '<',  '<',  '<',  '<',  '>',  '>',  '>',  '>',  '<',  '<',  '<',  '>',  '>'},
/* - */     {'>',  '>',  '<',  '<',  '<',  '<',  '<',  '<',  '>',  '>',  '>',  '>',  '<',  '<',  '<',  '>',  '>'},
/* * */     {'>',  '>',  '>',  '>',  '>',  '<',  '<',  '<',  '>',  '>',  '>',  '>',  '<',  '<',  '<',  '>',  '>'},
/* / */     {'>',  '>',  '>',  '>',  '>',  '<',  '<',  '<',  '>',  '>',  '>',  '>',  '<',  '<',  '<',  '>',  '>'},
/* % */     {'>',  '>',  '>',  '>',  '>',  '<',  '<',  '<',  '>',  '>',  '>',  '>',  '<',  '<',  '<',  '>',  '>'},
/* = */     {'<',  '<',  '<',  '<',  '<',  '=',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '<'},
/* == */    {'<',  '<',  '<',  '<',  '<',  '<',  '=',  '=',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '<'},
/* != */    {'<',  '<',  '<',  '<',  '<',  '<',  '=',  '=',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '<',  '<'},
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
tree<std::string>::iterator preprocessing_list_root;

tree<std::string> external_defination_list_tree;
tree<std::string>::iterator external_defination_list_root;

tree<std::string> external_variable_defination_tree;
tree<std::string>::iterator external_variable_defination_root;

tree<std::string> function_defination_tree;
tree<std::string>::iterator function_defination_root;

tree<std::string> formal_parameter_list_tree;
tree<std::string>::iterator formal_parameter_list_root;

tree<std::string> compound_statement_tree;
tree<std::string>::iterator compound_statement_root;

tree<std::string> local_variable_defination_list_tree;
tree<std::string>::iterator local_variable_defination_list_root;

tree<std::string> statement_list_tree;
tree<std::string>::iterator statement_list_root;

tree<std::string> statement_tree;
tree<std::string>::iterator statement_root;

tree<std::string> expression_tree;
tree<std::string>::iterator expression_root;




int main(){
    char filename[100];
    strcpy(filename,"/home/yyx/DataStructureExperiment/test/basic.cxx");


    FILE *fp;
    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Error opening file in scanner.\n");
        return false;
    };

    
    //lexical analysis
    scanner(fp,filename);

    //syntax analysis
    program_root=program();
    treePrint();
    //intermediate code generation
    //...

    fclose(fp);
    return 0;
}

