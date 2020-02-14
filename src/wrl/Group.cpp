//------------------------------------------------------------------------
//  Copyright (C) Gabriel Taubin
//  Time-stamp: <2019-11-03 14:59:25 taubin>
//------------------------------------------------------------------------
//
// Group.cpp
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
#include <math.h>
#include <algorithm>
#include "Transform.hpp"
#include "Shape.hpp"
#include "IndexedFaceSet.hpp"
#include "IndexedLineSet.hpp"
#include <SceneGraphTraversal.cpp>

Group::Group() :
	_bboxCenter(0.0f, 0.0f, 0.0f),
	_bboxSize(-1.0f, -1.0f, -1.0f) {
}

Group::~Group() {

	// TODO
	// delete all children here
	_children.clear();
	_bboxCenter = Vec3f(0.0f, 0.0f, 0.0f);
	_bboxSize = Vec3f(-1.0f, -1.0f, -1.0f);
}

vector<pNode>& Group::getChildren() {
	return _children;
}

Node* Group::getChild(const string& name) const {
	Node* node = (Node*)0;

	// TODO
	// find child matching the given name
	// do not traverse any deeper
	int n = _children.size();
	for (int i = 0; i < n; i++) {
		if (_children[i]->getName() == "name") {
			node = _children[i];
			break;
		}
	}

	return node;
}

int Group::getNumberOfChildren() const {

	// TODO
	return _children.size();
}

pNode Group::operator[](const int i) {
	pNode node = (pNode)0;

	// TODO
	// return the i-th node, if 0<=i<getNumberOfChildren()
	if (i >= 0 && i < this->getNumberOfChildren()) {
		node = _children[i];
	}

	return node;
}

Vec3f& Group::getBBoxCenter() {

	// TODO
	return _bboxCenter;
}

Vec3f& Group::getBBoxSize() {

	// TODO
	return _bboxSize;
}

float Group::getBBoxDiameter() {
	float diam2 = 0.0f;

	// TODO
	// compute the square of the sdiameter of the sphere circumscribing
	// the bounding box
	diam2 = _bboxSize.x * _bboxSize.x + _bboxSize.y * _bboxSize.y +
		_bboxSize.z * _bboxSize.z;
	return (diam2 > 0.0f) ? (float)sqrt(diam2) : 0.0f;
}

void Group::addChild(const pNode child) {

	// TODO
	// - add to the list of children, if not already there
	// - remember to set the parent of the child
	int n = this->getNumberOfChildren();
	for (int i = 0; i < n; i++) {
		if (this->getChildren()[i]->getName() == child->getName()) {
			return;
		}
	}
	_children.push_back(child);
	child->setParent(this);
}

void Group::removeChild(const pNode child) {

	// TODO
	// look for child in the list of children
	// if found, remove from the list and delete the child
	int n = this->getNumberOfChildren();
	for (int i = 0; i < n; i++) {
		if (this->getChildren()[i] == child) {
			delete this->getChildren()[i];
			this->getChildren().erase(this->getChildren().begin() + i);
		}
	}
}

void Group::setBBoxCenter(Vec3f& value) {

	// TODO
	_bboxCenter = value;
}
void Group::setBBoxSize(Vec3f& value) {

	// TODO
	_bboxSize = value;
}

void Group::clearBBox() {

	// TODO
	_bboxCenter = Vec3f(0.0f, 0.0f, 0.0f);
	_bboxSize = Vec3f(-1.0f, -1.0f, -1.0f);
}
bool Group::hasEmptyBBox() const {

	// TODO
	if (_bboxSize.x < 0 || _bboxSize.y < 0 || _bboxSize.z < 0)
		return true;
	return false;
}

void Group::appendBBoxCoord(vector<float>& coord) {

	// TODO

	// if the bbox is not empty, compute the coordinates of the eight
	// corners of the bounding box, and append them to the coord array
	// WHY GIVE TO THE COORD ARRAY???????????????????????????????????????????
	if (!this->hasEmptyBBox()) {
		float xMax = 0, yMax = 0, zMax = 0,	xMin = 0, yMin = 0, zMin = 0, i = 0;
		while (i < coord.size()) {
			xMax = coord[i] > xMax ? coord[i] : xMax;
			xMin = coord[i] < xMin ? coord[i] : xMin;
			i++;
			yMax = coord[i] > yMax ? coord[i] : yMax;
			yMin = coord[i] < yMin ? coord[i] : yMin;
			i++;
			zMax = coord[i] > zMax ? coord[i] : zMax;
			zMin = coord[i] < zMin ? coord[i] : zMin;
			i++;
		}
		coord.push_back(xMax);
		coord.push_back(yMax);
		coord.push_back(zMax);
		coord.push_back(xMax);
		coord.push_back(yMax);
		coord.push_back(zMin);
		coord.push_back(xMax);
		coord.push_back(yMin);
		coord.push_back(zMax);
		coord.push_back(xMax);
		coord.push_back(yMin);
		coord.push_back(zMin);

		coord.push_back(xMin);
		coord.push_back(yMax);
		coord.push_back(zMax);
		coord.push_back(xMin);
		coord.push_back(yMax);
		coord.push_back(zMin);
		coord.push_back(xMin);
		coord.push_back(yMin);
		coord.push_back(zMax);
		coord.push_back(xMin);
		coord.push_back(yMin);
		coord.push_back(zMin);
	}
}

void Group::updateBBox(vector<float>& coord) {

	// TODO

	// - update the bbox dimensions to also enclose the points contained
	//   in the coord array

	// - if the coord array does not contain any points, return without
	//   doing anything

	// - if the bounding box is empty, compute the bounding box of the
	//    points contained in the coord array

}

void Group::updateBBox() {

	// TODO

	// compute the smallest bounding box containing
	// 1) all the coord of of geometry contained in Shape node children
	// 2) the 8 corners of the bounding boxes of the Group node children
	// 3) the 8 corners of the bounding boxes of the Transform node
	// children, after they are transformed to this Group coordinate system
	SceneGraph wrl;
	wrl.addChild(this);
	//Initialize global transformation matrix
	Eigen::Matrix4f T = Eigen::Matrix4f::Identity(4, 4);
	stack<Eigen::Matrix4f> stack;
	Node* node = &wrl;
	stack.push(T);	//	!!!!!!!!!!!!
	Group::dfs(stack, node, T);
}

void			Group::dfs(stack<Eigen::Matrix4f> stack, Node* node, Eigen::Matrix4f T) {
	if (node == (Node*)0) return;

	if (node->isShape()) {
		Shape* shape = (Shape*)node;
		//IndexedFaceSet* geometry = new IndexedFaceSet();
		//if (node->getType() == "IndexedLineSet") {
		//	geometry = (IndexedLineSet*)shape->getGeometry();
		//} else {
		//	geometry = (IndexedFaceSet*)shape->getGeometry();
		//}
		if (shape->getName() == "BOUNDING-BOX") {
			return;
		}
		IndexedFaceSet* geometry = (IndexedFaceSet*)shape->getGeometry();
		
		vector<float>& coord = geometry->getCoord();
		Vec3f center = _bboxCenter;
		Vec3f size = _bboxSize;
		float xMax = center.x + size.x / 2, yMax = center.y + size.y / 2, zMax = center.z + size.z / 2,
			xMin = center.x - size.x / 2, yMin = center.y - size.y / 2, zMin = center.z - size.z / 2;
		for (int i = 0; i < geometry->getNumberOfCoord(); i++) {
			Eigen::Vector4f currCoord;
			int count = 0;
			for (int j = i * 3; j < i * 3 + 3; j++) {
				currCoord(count++) = coord[j];
			}
			currCoord(3) = 1.0f;
			Eigen::Vector4f tempCoord = T * currCoord;
			xMax = tempCoord(0) > xMax ? tempCoord(0) : xMax;
			yMax = tempCoord(1) > yMax ? tempCoord(1) : yMax;
			zMax = tempCoord(2) > zMax ? tempCoord(2) : zMax;
			xMin = tempCoord(0) < xMin ? tempCoord(0) : xMin;
			yMin = tempCoord(1) < yMin ? tempCoord(1) : yMin;
			zMin = tempCoord(2) < zMin ? tempCoord(2) : zMin;
		}
		_bboxSize = Vec3f(xMax - xMin, yMax - yMin, zMax - zMin);
		_bboxCenter = Vec3f((xMax + xMin) / 2, (yMax + yMin) / 2, (zMax + zMin) / 2);

	} else if (node->isTransform()) {
		stack.push(T);
		Transform* transform = (Transform*)node;
		Eigen::Matrix4f currT = Eigen::Matrix4f::Identity(4, 4);
		transform->getMatrix(currT);
		T = currT * T;

		for (int i = 0; i < transform->getNumberOfChildren(); i++) {
			dfs(stack, transform->getChildren()[i], T);
		}
		T = stack.top();
		stack.pop();
	} else if (node->isGroup()) {
		stack.push(T);
		Group* group = (Group*)node;
		for (int i = 0; i < group->getNumberOfChildren(); i++) {
			dfs(stack, group->getChildren()[i], T);
		}
		T = stack.top();
		stack.pop();
	}
}

// TODO
// can be used for debugging
// use as template to implement the method for other Node subclasses

void Group::printInfo(string indent) {
	std::cout << indent;
	if (_name != "") std::cout << "DEF " << _name << " ";
	std::cout << "Group {\n";
	std::cout << indent << "  " << "children [\n";
	int nChildren = getNumberOfChildren();
	for (int i = 0; i < nChildren; i++) {
		Node* node = (*this)[i];
		node->printInfo(indent + "    ");
	}
	std::cout << indent << "  " << "]\n";
	std::cout << indent << "}\n";
}
