//------------------------------------------------------------------------
//  Copyright (C) Gabriel Taubin
//  Time-stamp: <2019-11-03 14:59:33 taubin>
//------------------------------------------------------------------------
//
// Shape.cpp
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
#include "Shape.hpp"
#include "Appearance.hpp"

Shape::Shape() :
	_appearance((Node*)0),
	_geometry((Node*)0) {
}

Shape::~Shape() {
	_appearance = NULL;
	_geometry = NULL;
}

Node* Shape::getAppearance() {
	return _appearance;
}

Node* Shape::getGeometry() {
	return _geometry;
}

bool Shape::hasAppearanceNone() {

	// TODO
	Node* node = this->getAppearance();
	if (node == (Node*)0) return true;
	return false;
}

bool Shape::hasAppearanceMaterial() {

	// TODO
	if (this->getAppearance() == (Node*)0) return false;
	Appearance* appearance = (Appearance*)this->getAppearance();
	Node* node = appearance->getMaterial();
	if (node != (Node*)0) return true;
	return false;
}

bool Shape::hasAppearanceImageTexture() {

	// TODO
	if (this->getAppearance() == (Node*)0) return false;
	Appearance* appearance = (Appearance*)this->getAppearance();
	Node* node = appearance->getTexture();
	if (node != (Node*)0 && node->isImageTexture()) return true;
	return false;
}

//bool Shape::hasAppearanceUnsupported() {
//
//	// TODO
//
//}

bool Shape::hasGeometryNull() {

	// TODO
	Node* node = this->getGeometry();
	if (node == (Node*)0) return true;
	return false;
}

bool Shape::hasGeometryIndexedFaceSet() {

	// TODO
	Node* node = this->getGeometry();
	if (node != (Node*)0 && node->isIndexedFaceSet())
		return true;
	return false;
}

bool Shape::hasGeometryIndexedLineSet() {

	// TODO
	Node* node = this->getGeometry();
	if (node != (Node*)0 && node->isIndexedLineSet())
		return true;
	return false;
}

//bool Shape::hasGeometryUnsupported() {
//
//	// TODO
//
//}

void Shape::setAppearance(Node* node) {
	_appearance = node;
}

void Shape::setGeometry(Node* node) {
	_geometry = node;
}

void Shape::printInfo(string indent) {
	std::cout << indent;
	if (_name != "") std::cout << "DEF " << _name << " ";
	std::cout << "Shape {\n";
	if (Node* appearance = getAppearance()) {
		std::cout << indent << "  appearance\n";
		appearance->printInfo(indent + "    ");
	}
	if (Node* geometry = getGeometry()) {
		std::cout << indent << "  geometry\n";
		geometry->printInfo(indent + "    ");
	}
	std::cout << indent << "}\n";
}
