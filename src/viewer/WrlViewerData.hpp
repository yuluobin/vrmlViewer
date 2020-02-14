//------------------------------------------------------------------------
//  Copyright (C) Gabriel Taubin
//  Time-stamp: <2019-11-03 14:59:24 taubin>
//------------------------------------------------------------------------
//
// WrlViewerData.hpp
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

#ifndef _WRL_VIEWER_DATA_HPP_
#define _WRL_VIEWER_DATA_HPP_

#include "wrl/SceneGraph.hpp"

class WrlViewerData {
public:
  WrlViewerData();
  ~WrlViewerData();

  // void setInitialized(bool value) {
  //   _initialized = value;  // have OpenGL been initialized?
  // }

  void setSceneGraph(SceneGraph* pWrl);


  // bool        getInitialized() { return _initialized; }
  SceneGraph* getSceneGraph()  { return        _pWrl; }

  Vec4f&      getPlane();
  void        setPlane(Vec4f& f);

  int         getBBoxDepth();
  void        setBBoxDepth(int d);
  bool        getBBoxCube();
  void        setBBoxCube(bool value);
  bool        getBBoxOccupied();
  void        setBBoxOccupied(bool value);
  float       getBBoxScale();
  void        setBBoxScale(float scale);

private:

  // bool  _initialized;  // have OpenGL been initialized?
  SceneGraph*  _pWrl;
  Vec4f        _plane;
  int          _bboxDepth;
  bool         _bboxCube;
  bool         _bboxOccupied;
  float        _bboxScale;

};

#endif // _WRL_VIEWER_DATA_HPP_
