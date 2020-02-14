//------------------------------------------------------------------------
//  Copyright (C) Gabriel Taubin
//  Time-stamp: <2019-11-19 14:16:26 taubin>
//------------------------------------------------------------------------
//
// Tokenizer.cpp
//
// Software developed for the Fall 2019 Brown University course
// ENGN 2912B Scientific Computing in C++
// Copyright (c) 2019, Gabriel Taubin
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the Brown University nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL GABRIEL TAUBIN BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "Tokenizer.hpp"

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
		while (this->_ifstr.peek() != EOF) {
			char tempCurr = this->_ifstr.ifstream::get();
			char tempNext = this->_ifstr.ifstream::peek();
			if (tempNext == '#') {
				while (this->_ifstr.peek() != EOF && tempNext != '\n') {
					tempCurr = this->_ifstr.ifstream::get();
					tempNext = this->_ifstr.ifstream::peek();
				}
				break;
			} else if (isspace(tempCurr)) {
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