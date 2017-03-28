#ifndef M_BASICS_H
#define M_BASICS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// len: returns the length of the string 's'
int len(char *s);

// concat: concatenates 'a' and 'b', returns a new string
char *concat(char *a, char *b);

// readn: reads up to 'n' chars into the string 'arr'
// returns the amount of chars read
int readn(char *arr, int n);

// read_line: reads in the string 'arr' until '\n' or 'EOF' is found
// or 'size'-1 chars are read
// returns the amount of chars read
int read_line(char *arr, int size);

// strcmpr_first: compares the first 'n' chars of the strings 'a' and 'b';
// if a > b,  returns > 0
// if b > a,  returns < 0
// if a == b, returns 0
int strcmpr_first(char *a, char *b, int n);

// read_char: returns the pushed char or a char read with getchar()
int read_char();

// push_char: pushes a char
// returns the pushed char
int push_char(int c);

// discard_line: discards a line
// returns the amount of chars discarded
int discard_line();

// discard_until: discards chars until 'goal' or EOF is found
// returns the amount of chars discarded
int discard_until(int goal);

// discard_while: discards chars while they are present in the string 'goals'
// returns the amount of chars discarded
int discard_while(char *goals);

// read_until: reads chars into 'str' until 'goal' or EOF is found, or 'size'-1 
// chars are read
// returns the amount of chars read
int read_until(int goal, char* str, int size);

bool char_in_string(char c, char *goals);

// split: splits 'str' into various strings with 'goal' being the separator
// strings are stored in the array 'arr' of size 'size'
// returns the resulting amount of strings
int split(char** arr, int size, char* str, char goal);

// is_prime: returns true if n is prime
bool is_prime(int n);

// next_prime: returns the next prime closest to n
int next_prime(int n);

#endif
