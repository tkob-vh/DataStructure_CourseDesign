/********************************************************************
* FILE: LexicalAnalyzer.h
* DESCRIPTION:
* AUTHOR: yyx
* LASTREVISED: 2021.8.14
********************************************************************/

extern int w; // store the token kind of contomparary token in SyntaxAnalyzer.c
extern bool error_flag; // once error_flag is 1, release the memory of AST .
extern char token_text[1000][20]; // store the token text such as identifier, literal, etc.
extern char t_kinds[1000][20]; // store the token kind such as ID, INT_LITERAL, etc.
extern size_t line_counter; // count the line number
extern size_t token_counter; // count the token number
