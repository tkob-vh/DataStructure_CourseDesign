/********************************************************************
* FILE: LexicalAnalyzer.h
* DESCRIPTION:
* AUTHOR: yyx
* LASTREVISED: 2021.8.14
********************************************************************/




#pragma once


int LookupKeyword(int token_num);
int gettoken(FILE * source_file,char c);
int scanner(FILE * source_file);