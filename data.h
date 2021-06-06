/*
 * @Descriptions: 
 * @Author: Junhui Luo
 * @Blog: https://luojunhui1.github.io/
 * @Date: 2021-05-24 02:15:04
 * @LastEditTime: 2021-06-07 01:23:30
 */
#ifndef extern_
 #define extern_ extern
#endif

extern_ int    Line;           //current processing line
extern_ int    Putback;        //putback character
extern_ FILE   *inFile;        //input file
extern_ FILE   *outFile;  //output file
extern_ struct token	Token; //token variable
extern_ char Text[TEXTLEN + 1];		// Last identifier scanned
extern_ struct symtable Gsym[NSYMBOLS];	// Global symbol table