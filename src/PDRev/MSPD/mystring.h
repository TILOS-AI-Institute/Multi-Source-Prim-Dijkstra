

#ifndef MYSTRING_H
#define MYSTRING_H

#include <string>
#include <vector>

using namespace std;

// functions
char *string_to_char(string str);
vector<string> split(string input, string delim);
void print_vector_string(vector<string> input);
bool is_string_number(string input);
bool string_is_integer_greater_than_zero(string input);
int string_to_int(string input);
bool string_is_floating_not_negative(string input);
float string_to_float(string input);

#endif
