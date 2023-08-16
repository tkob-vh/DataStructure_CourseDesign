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
#include "tree.h"



//global variables
char token_text[1000][20]; // store the token text such as identifier, literal, etc.
char t_kinds[1000][20]; // store the token kind such as ID, INT_LITERAL, etc.
size_t line_counter = 1; // count the line number
size_t token_counter = 0; // count the token number in LexicalAnalyzer.c
size_t token_counter1 = 0; // count the token number in SyntaxAnalyzer.c
char Keywords[14][10]={"int","float","char","void","if","else","while","return","for","do","break","continue","define","include"};//store the main keywords
char w[20]; //store the token kind of contomparary token in SyntaxAnalyzer.c
bool error_flag; // once error_flag is 1, release the memory of AST .

//the variables of tree.cxx and tree.h
FILE* fp;
Trees Set[10] = { '\0',NULL };
int* p1, * p2, * p3;
BiTree* p4;
int* p5;

