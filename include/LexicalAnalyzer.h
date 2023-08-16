/********************************************************************
* FILE: LexicalAnalyzer.h
* DESCRIPTION:
* AUTHOR: yyx
* LASTREVISED: 2021.8.14
********************************************************************/
#pragma once

//#define enumToStr(WEEK)  "\""#WEEK"\""


extern char token_text[1000][20]; // store the token text such as identifier, literal, etc.
extern char t_kinds[1000][20]; // store the token kind such as ID, INT_LITERAL, etc.
extern int line_info[1000]; // store the line number of each token
extern size_t line_counter; // count the line number
extern size_t token_counter; // count the token number
extern char Keywords[14][10];//store the main keywords
extern int w; //store the token kind of contomparary token in SyntaxAnalyzer.c
extern Token_kind token_kind;


int LookupKeyword(int token_num);
char* EnumtoStr(int enub);
int gettoken(FILE * source_file,char c);
bool scanner(FILE * source_file);