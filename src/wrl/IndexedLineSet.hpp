//------------------------------------------------------------------------
//  Copyright (C) Gabriel Taubin
//  Time-stamp: <2019-11-03 14:59:27 taubin>
//------------------------------------------------------------------------
//
// IndexedLineSet.hpp
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

#ifndef _INDEXED_LINE_SET_HPP_
#define _INDEXED_LINE_SET_HPP_

// VRML'97
//
// IndexedLineSet {
//   eventIn       MFInt32 set_colorIndex
//   eventIn       MFInt32 set_coordIndex
//   exposedField  SFNode  coord             NULL
//   field         MFInt32 coordIndex        []        # [-1,)
//   exposedField  SFNode  color             NULL
//   field         MFInt32 colorIndex        []        # [-1,)
//   field         SFBool  colorPerVertex    TRUE
// }

#include "Node.hpp"
#include <vector>

using namespace std;

class IndexedLineSet : public Node {

private:

  vector<float> _coord;
  vector<int>   _coordIndex;
  vector<float> _color;
  vector<int>   _colorIndex;
  bool          _colorPerVertex;

public:
  
  IndexedLineSet();

  void           clear();
  bool&          getColorPerVertex();
  vector<float>& getCoord();
  vector<int>&   getCoordIndex();
  vector<float>& getColor();
  vector<int>&   getColorIndex();

  int            getNumberOfPolylines();

  int            getNumberOfCoord();
  int            getNumberOfColor();

  void           setColorPerVertex(bool value);

  virtual bool    isIndexedLineSet() const { return             true; }
  virtual string  getType()          const { return "IndexedLineSet"; }

  typedef bool    (*Property)(IndexedLineSet& ifs);
  typedef void    (*Operator)(IndexedLineSet& ifs);

  // can be used for debugging
  virtual void    printInfo(string indent);
};

#endif // _INDEXED_LINE_SET_HPP_
