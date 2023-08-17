/********************************************************************
* FILE: main.cxx 
* DESCRIPTION:
* AUTHOR: yyx
* LASTREVISED: 2021.8.14
********************************************************************/
#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include "GlobalVar.h"
#include "LexicalAnalyzer.h"
#include "SyntaxAnalyzer.h"
#include "IRGenerator.h"
#include "tree.hh"



//global variables
char token_text[1000][20]; // store the token text such as identifier, literal, etc.
char t_kinds[1000][20]; // store the token kind such as ID, INT_LITERAL, etc.
int line_info[1000]; // store the line number of each token
size_t line_counter = 1; // count the line number
size_t token_counter = 0; // count the token number in LexicalAnalyzer.c
size_t token_counter1 = 0; // count the token number in SyntaxAnalyzer.c
char Keywords[14][10]={"int","float","char","void","if","else","while","return","for","do","break","continue","define","include"};//store the main keywords
char Capital_Keywords[14][10]={"INT","FLOAT","CHAR","VOID","IF","ELSE","WHILE","RETURN","FOR","DO","BREAK","CONTINUE","DEFINE","INCLUDE"};//store the main keywords
char w[20]; //store the token kind of contomparary token in SyntaxAnalyzer.c
bool error_flag; // once error_flag is 1, release the memory of AST .

//the variables of tree.cxx and tree.h
tree<std::string> program_tree;
tree<std::string>::iterator program_root;
tree<std::string> external_defination_list_tree;
tree<std::string>::iterator external_defination_list_root;



