//------------------------------------------------------------------------
//  Copyright (C) Gabriel Taubin
//  Time-stamp: <2019-11-03 14:59:31 taubin>
//------------------------------------------------------------------------
//
// SceneGraph.cpp
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
#include "SceneGraph.hpp"
#include "SceneGraphTraversal.hpp"
#include "Shape.hpp"
#include "Appearance.hpp"

SceneGraph::SceneGraph() {
	_parent = this; // SceneGraph is always a tree root
}

SceneGraph::~SceneGraph() {
}

void SceneGraph::clear() {

	// TODO
	// delete all children and clear the children array
	//delete &_children;
	_children.clear();
}

string& SceneGraph::getUrl() {
	return _url;
}

void SceneGraph::setUrl(const string& url) {
	_url = url;
}


// NOTES

// 1) a more efficient way to implement this function is to use a
// map<string,Node*> which we would fill using the Node::setName()
// method; but for small SceneGraphs, such as the ones we are
// operating on, doing a search each time we need to find a node by
// name is acceptable.

// 2) we have decided not to implementing the USE keyword yet; we
// would need smart pointers to do so, in such a way that the node
// would be deleted only when the number of references to it go back
// to zero.

Node* SceneGraph::find(const string& name) {
	Node* node = (Node*)0;

	// - using a SceneGraphTraversal, look for a node with the given
	// - name

	// - remember that the SceneGraphTraversal does not visit the nodes
	//   pointed to inside the Shape nodes

	// - for each Shape node, you need to check the names of
	//   1) the Appearance node (if any) pointed to by the appearance
	//   field of the Shape node
	//   2) the Material node (if any) pointed to by the material field
	//   of the Appearnce node
	//   3) the PixelTexture or ImageTexture node (if any) pointed to by
	//   the texture field of the Appearnce node
	//   4) the IndexedFaceSet or IndexedLineSet nodes (if any) pointed
	//   to by the geometry field of the Shape node

	SceneGraphTraversal traversal(*this);
	traversal.start();
	while ((node = traversal.next()) != (Node*)0) {
		if (node->isShape()) {
			Shape* shape = (Shape*)node;
			if (shape->getName() == name) return node;
			//	4)
			node = shape->getGeometry();
			if (node != (Node*)0 && (node->isIndexedFaceSet() || node->isIndexedLineSet())) {
				if (node->getName() == name)
					return node;
			}
			//	1)
			node = shape->getAppearance();
			if (node != (Node*)0 && node->getName() == name) return node;
			//	2)	well... may be some errors
			node = ((Appearance*)(shape->getAppearance()))->getMaterial();
			if (node != (Node*)0 && node->getName() == name) {
				return node;
			}
			//	3)	well... may be some errors
			node = ((Appearance*)(shape->getAppearance()))->getTexture();
			if (node != (Node*)0 && (node->isImageTexture() || node->isPixelTexture()))
				if (node->getName() == name)
					return node;

		} else if (node->isGroup()) {
			Group* group = (Group*)node;
			if (group->getName() == name) return node;
		} else if (node->isTransform()) {
			Transform* transform = (Transform*)node;
			if (transform->getName() == name) return node;
		}
	}

	return node;
}

void SceneGraph::printInfo(string indent) {
	std::cout << indent;
	if (_name != "") std::cout << "DEF " << _name << " ";
	std::cout << "SceneGraph {\n";
	std::cout << indent << "  " << "_url = \"" << _url << "\"\n";
	std::cout << indent << "  " << "children [\n";
	int nChildren = getNumberOfChildren();
	for (int i = 0; i < nChildren; i++) {
		Node* node = (*this)[i];
		node->printInfo(indent + "    ");
	}
	std::cout << indent << "  " << "]\n";
	std::cout << indent << "}\n";
}
