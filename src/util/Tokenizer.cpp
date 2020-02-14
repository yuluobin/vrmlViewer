//------------------------------------------------------------------------
// Tokenizer is used like a auto-moving charactor stack to store parenthesis
// or other similar code symbols

#include "Tokenizer.h"

bool Tokenizer::expecting(const string& str) {
	return Tokenizer::get() && (str == *this);
}

bool Tokenizer::get() {
	this->clear();
	bool success = false;

	//	Examine pre-requested condition
	if (!this->_ifstr.is_open()) {
		cout << "Current file is not opened. `get` function failed...";
		success = false;
		return success;
	} else {
		while (this->_ifstr.peek() != EOF ) {
			char tempCurr = this->_ifstr.ifstream::get();
			char tempNext = this->_ifstr.ifstream::peek();
			if (isspace(tempCurr)) {
				//	If there is already something in `this`
				if (!this->empty()) {
					break;
				} else continue;
			} else if (tempNext == '{' || tempNext == '}' || tempNext == ','
				|| tempNext == '[' || tempNext == ']') {
				this->push_back(tempCurr);
				break;
			} else {
				this->push_back(tempCurr);
				continue;
			}
		}
		//	If the token is not EMPTY
		if (!this->empty()) {
			success = true;
		}
	}

	return success;
}