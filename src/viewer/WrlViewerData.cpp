//------------------------------------------------------------------------
//  Copyright (C) Gabriel Taubin
//  Time-stamp: <2019-11-03 14:59:24 taubin>
//------------------------------------------------------------------------
//
// WrlViewerData.cpp
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

#include "WrlViewerData.hpp"

WrlViewerData::WrlViewerData():
  // _initialized(false),
  _pWrl((SceneGraph*)0),
  _bboxDepth(0),
  _bboxCube(true),
  _bboxOccupied(false),
  _bboxScale(1.05f) {
  _plane.x = _plane.y = _plane.z = _plane.w = 0.0f;
}

WrlViewerData::~WrlViewerData() {
  if(_pWrl!=(SceneGraph*)0) delete _pWrl;
}

void WrlViewerData::setSceneGraph(SceneGraph* pWrl) {
  if(pWrl!=_pWrl) {
    if(_pWrl!=(SceneGraph*)0) delete _pWrl;
    _pWrl = pWrl;
  }
}

Vec4f& WrlViewerData::getPlane() {
  return _plane;
}

void WrlViewerData::setPlane(Vec4f& f) {
  _plane = f;
}

int WrlViewerData::getBBoxDepth() {
  return _bboxDepth;
}

void WrlViewerData::setBBoxDepth(int d) {
  if(d<0) d = 0; else if(d>10) d = 10;
  _bboxDepth = d;
}

bool WrlViewerData::getBBoxCube() {
  return _bboxCube;
}

void WrlViewerData::setBBoxCube(bool value) {
  _bboxCube = value;
}

bool WrlViewerData::getBBoxOccupied() {
  return _bboxOccupied;
}

void WrlViewerData::setBBoxOccupied(bool value) {
  _bboxOccupied = value;
}

float WrlViewerData::getBBoxScale() {
  return   _bboxScale;
}

void WrlViewerData::setBBoxScale(float scale) {
  _bboxScale = (scale<0.0f)?0.0f:scale;
}
