//------------------------------------------------------------------------
//  Copyright (C) Gabriel Taubin
//  Time-stamp: <2019-11-03 14:59:12 taubin>
//------------------------------------------------------------------------
//
// LoaderWrl.hpp
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

#ifndef _LOADER_WRL_HPP_
#define _LOADER_WRL_HPP_

#include "Loader.hpp"
#include "Tokenizer.hpp"
#include <wrl/Transform.hpp>
#include <wrl/Shape.hpp>
#include <wrl/Appearance.hpp>
#include <wrl/Material.hpp>
#include <wrl/ImageTexture.hpp>
#include <wrl/IndexedFaceSet.hpp>
#include <wrl/IndexedLineSet.hpp>

class LoaderWrl : public Loader {

private:

  const static char* _ext;

public:

  LoaderWrl()  {};
  ~LoaderWrl() {};

  bool  load(const char* filename, SceneGraph& wrl);
  const char* ext() const { return _ext; }

private:

  // TODO

  // one possible design; feel free to implement it in a different way

  bool loadSceneGraph(Tokenizer& tkn, SceneGraph& wrl);
  bool loadGroup(Tokenizer& tkn, Group& group);
  bool loadTransform(Tokenizer& tkn, Transform& transform);
  bool loadChildren(Tokenizer& tkn, Group& group);
  bool loadShape(Tokenizer& tkn, Shape& transform);
  bool loadAppearance(Tokenizer& tkn, Appearance& appearance);
  bool loadMaterial(Tokenizer& tkn, Material& material);
  bool loadImageTexture(Tokenizer& tkn, ImageTexture& imageTexture);
  bool loadIndexedFaceSet(Tokenizer& tkn, IndexedFaceSet& ifs);
  bool loadIndexedLineSet(Tokenizer& tkn, IndexedLineSet& ifs);
  bool loadVecFloat(Tokenizer& tkn,vector<float>& vec);
  bool loadVecInt(Tokenizer &tkn,vector<int>& vec);
  bool loadVecString(Tokenizer &tkn,vector<string>& vec);

};

#endif // _LOADER_WRL_HPP_
