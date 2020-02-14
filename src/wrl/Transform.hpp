//------------------------------------------------------------------------
//  Copyright (C) Gabriel Taubin
//  Time-stamp: <2019-11-03 15:06:39 taubin>
//------------------------------------------------------------------------
//
// Transform.hpp
//
// Software developed for the Fal9 2019 Brown University course
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

#ifndef _TRANSFORM_HPP_
#define _TRANSFORM_HPP_

// Transform {
//   eventIn MFNode addChildren
//   eventIn MFNode removeChildren
//   exposedField SFVec3f center 0 0 0
//   exposedField MFNode children []
//   exposedField SFRotation rotation 0 0 1 0
//   exposedField SFVec3f scale 1 1 1
//   exposedField SFRotation scaleOrientation 0 0 1 0
//   exposedField SFVec3f translation 0 0 0
//   field SFVec3f bboxCenter 0 0 0
//   field SFVec3f bboxSize -1 -1 -1
// }

#include "Group.hpp"
#include "Rotation.hpp"

using namespace std;

class Transform : public Group {

private:

  Vec3f         _center;           // 0 0 0
  Rotation      _rotation;         // 0 0 1 0
  Vec3f         _scale;            // 1 1 1
  Rotation      _scaleOrientation; // 0 0 1 0
  Vec3f         _translation;      // 0 0 0

  // inherited from Group
  // vector<Node*> _children;
  // Vec3f         _bboxCenter;
  // Vec3f         _bboxSize;

public:
  
  Transform();
  virtual ~Transform();

  Vec3f&    getCenter();
  Rotation& getRotation();
  Vec3f&    getScale();
  Rotation& getScaleOrientation();
  Vec3f&    getTranslation();

  void      setCenter(Vec3f& value);
  void      setRotation(Rotation& value);
  void      setRotation(Vec4f& value);
  void      setScale(Vec3f& value);
  void      setScaleOrientation(Rotation& value);
  void      setScaleOrientation(Vec4f& value);
  void      setTranslation(Vec3f& value);

  void      getMatrix(Eigen::Matrix4f T /*[16]*/);

  virtual bool    isTransform() const { return        true; }
  virtual string  getType()     const { return "Transform"; }

  typedef bool    (*Property)(Transform& transform);
  typedef void    (*Operator)(Transform& transform);

  // can be used for debugging
  virtual void    printInfo(string indent);

private:

  static void _makeRotation(Rotation& r, float* R /*[9]*/);

};

#endif // _TRANSFORM_HPP_
