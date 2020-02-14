//------------------------------------------------------------------------
//  Copyright (C) Gabriel Taubin
//  Time-stamp: <2019-11-03 14:59:24 taubin>
//------------------------------------------------------------------------
//
// Appearance.cpp
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
#include "Appearance.hpp"

Appearance::Appearance():
  _material((Node*)0),
  _texture((Node*)0) /*,*/
  /* _textureTransform;((Node*)0) */ {
}

Appearance::~Appearance() {
}

Node* Appearance::getMaterial() {

  // TODO
    return _material;
}

Node* Appearance::getTexture() {

  // TODO

    return _texture;
}

// Node* Appearance::getTextureTransform() {
//   // TODO
// }

void Appearance::setMaterial(Node* material) {

  // TODO
    _material = material;

}

void Appearance::setTexture(Node* texture) {
  texture->setParent(this);
  _texture = texture;
}

// void Appearance::setTextureTransform(Node* textureTransform) {
//
//   // TODO
//
// }

// TODO
// can be used for debugging
// use as template to implement the method for other Node subclasses

void Appearance::printInfo(string indent) {
  std::cout << indent;
  if(_name!="") std::cout << "DEF " << _name << " ";
  std::cout << "Appearance {\n";
  if(_material!=(Node*)0) {
    std::cout << indent << "  material\n";
    _material->printInfo(indent+"    ");
  }
  if(_texture!=(Node*)0) {
    std::cout << indent << "  texture\n";
    _texture->printInfo(indent+"    ");
  }
  std::cout << indent << "}\n";
}
