#pragma once
#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_
#include <fstream>
#include <string>
#include <iostream>
using namespace std;

class Tokenizer : public string {
protected:
	std::ifstream& _ifstr;
public:
	Tokenizer(std::ifstream& ifstr) :_ifstr(ifstr) { }
	bool get();
	bool expecting(const string& str);
};

#endif // !_TOKENIZER_H_