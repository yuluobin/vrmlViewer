//------------------------------------------------------------------------
//  Copyright (C) Gabriel Taubin
//  Time-stamp: <2019-11-03 14:59:32 taubin>
//------------------------------------------------------------------------
//
// SceneGraphTraversal.hpp
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

#include <iostream>
#include "SceneGraphTraversal.hpp"

// Use as follows
//
// SceneGraph* wrl=null;
// // load wrl from file or create
// SceneGraphTraversal t = new SceneGraphTraversal(*wrl);
//
// Node* child;
// t.start();
// while((child=t.next())!=null) {
//   // do something with the node
// }

// NOTE
// that this implemenation is different from the one we provided for
// the previous assignment; this one does not have the advance()
// method; the pointer to the next node is incremented internally when
// the next() method is called; also, the start() method must be
// called before each traversal, including the first one after the
// constructor;

SceneGraphTraversal::SceneGraphTraversal(SceneGraph& wrl) :
	_wrl(wrl) {
}

void SceneGraphTraversal::start() {
	_node.clear();
	int n = _wrl.getNumberOfChildren();
	while ((--n) >= 0)
		_node.push_back(_wrl[n]);
}

Node* SceneGraphTraversal::next() {
	Node* next = (_node.size() > 0) ? _node.back() : (Node*)0;
	// advance for next call
	if (_node.size() > 0) {
		Node* node = _node.back(); _node.pop_back();
		if (node->isGroup()) {
			Group* group = (Group*)node;
			int n = group->getNumberOfChildren();
			while ((--n) >= 0)
				_node.push_back((*group)[n]);
		}
	}
	return next;
}

int SceneGraphTraversal::depth() {
	int d = 0;
	if (_node.size() > 0)
		d = (_node.back())->getDepth();
	return d;
}

