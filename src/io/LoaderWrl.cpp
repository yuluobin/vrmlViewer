//------------------------------------------------------------------------
//  Copyright (C) Gabriel Taubin
//  Time-stamp: <2019-11-03 14:59:11 taubin>
//------------------------------------------------------------------------
//
// LoaderWrl.cpp
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

#include <stdio.h>
#include "Tokenizer.hpp"
#include "LoaderWrl.hpp"
#include "StrException.hpp"

#define VRML_HEADER "#VRML V2.0 utf8"

const char* LoaderWrl::_ext = "wrl";

//////////////////////////////////////////////////////////////////////
bool LoaderWrl::loadSceneGraph(Tokenizer& tokenizer, SceneGraph& wrl) {

	bool   success = false;

	string name;
	while (tokenizer.get()) {
		if (tokenizer == "DEF") {
			tokenizer.get();
			name = tokenizer;
			continue;
			//if (tokenizer == "POINTS" || tokenizer == "BOUNDING-BOX" || tokenizer == "SURFACE")
			//    continue;
		}
		if (tokenizer == "Group") {
			Group* g = new Group();
			if (name != "") g->setName(name);
			name = "";
			wrl.addChild(g);
			loadGroup(tokenizer, *g);
		} else if (tokenizer == "Transform") {
			Transform* t = new Transform();
			if (name != "") t->setName(name);
			name = "";
			wrl.addChild(t);
			loadTransform(tokenizer, *t);
		} else if (tokenizer == "Shape") {
			Shape* s = new Shape();
			if (name != "") s->setName(name);
			name = "";
			wrl.addChild(s);
			loadShape(tokenizer, *s);
		} else if (tokenizer == "") {
			throw "loadSceneGraph ERROR";
			break;
		} else {
			// fprintf(stderr,"tkn=\"%s\"\n",tkn.c_str());
			throw new StrException("unexpected token while parsing Group");
		}
	}
	success = true;

	// only tokens to find at this level
	return success;
}

//////////////////////////////////////////////////////////////////////
bool LoaderWrl::loadGroup(Tokenizer& tokenizer, Group& group) {

	// Group {
	//   MFNode children    []
	//   SFVec3f bboxCenter  0 0 0
	//   SFVec3f bboxSize   -1 -1 -1
	// }

	bool success = false;
	string name;

	if (!tokenizer.expecting("{"))
		throw new StrException("expecting \"{\"");
	while (success == false && tokenizer.get()) {
		if (tokenizer == "children") {	//	THIS can be RECURSIVE!!!!!!!!!!!!!!!!!!!!!!
			//	In THIS children
			if (!tokenizer.expecting("[")) {
				throw new StrException("\'[\' not found after \'children\'");
			}
			while (tokenizer.get()) {
				if (tokenizer == "DEF") {
					tokenizer.get();
					name = tokenizer;
					continue;
				}
				if (tokenizer == "Shape") {
					Shape* shape = new Shape();
					if (name != "") shape->setName(name);
					name = "";
					loadShape(tokenizer, *shape);
					group.addChild(shape);
				} else if (tokenizer == "Transform") {
					Transform* trans = new Transform();
					if (name != "") trans->setName(name);
					name = "";
					loadTransform(tokenizer, *trans);
					group.addChild(trans);
				} else if (tokenizer == "Group") {
					Group* g = new Group();
					if (name != "") g->setName(name);
					name = "";
					group.addChild(g);
					loadGroup(tokenizer, *g);
				} else if (tokenizer == "]") break;
			}
		} else if (tokenizer == "}") {
			//	TODO
			//	No need to update bbox????????????????????????????

			group.updateBBox();

			success = true;
		} else {
			throw new StrException("unexpected token while parsing Group");
		}
	}
	return success;
}

bool LoaderWrl::loadTransform(Tokenizer& tokenizer, Transform& transform) {

	// Transform {
	//   MFNode     children          []
	//   SFVec3f    bboxCenter        0 0 0
	//   SFVec3f    bboxSize          -1 -1 -1
	//   SFVec3f    center            0 0 0
	//   SFRotation rotation          0 0 1 0
	//   SFVec3f    scale             1 1 1
	//   SFRotation scaleOrientation  0 0 1 0
	//   SFVec3f    translation       0 0 0
	// }

	bool success = false;
	string name;

	if (!tokenizer.expecting("{"))
		throw new StrException("expecting \"{\"");
	while (success == false && tokenizer.get()) {
		if (tokenizer == "children") {

			// TODO ...
			//	In THIS children
			if (!tokenizer.expecting("[")) {
				throw new StrException("\'[\' not found after \'children\'");
			}
			while (tokenizer.get()) {
				if (tokenizer == "DEF") {
					tokenizer.get();
					name = tokenizer;
					continue;
				}
				if (tokenizer == "Shape") {
					Shape* shape = new Shape();
					if (name != "") shape->setName(name);
					name = "";
					loadShape(tokenizer, *shape);
					transform.addChild(shape);
				}
				if (tokenizer == "Transform") {
					Transform* trans = new Transform();
					if (name != "") trans->setName(name);
					name = "";
					loadTransform(tokenizer, *trans);
					transform.addChild(trans);
				}
				if (tokenizer == "]") break;
			}
		} else if (tokenizer == "center") {
			float arr[3] = { 0 };
			int i = 0;
			while (i < 3 && tokenizer.get()) {
				arr[i++] = (stof(tokenizer));
			}
			transform.setCenter(Vec3f(arr[0], arr[1], arr[2]));

		} else if (tokenizer == "rotation") {
			float arr[4] = { 0 };
			int i = 0;
			while (i < 4 && tokenizer.get()) {
				arr[i++] = (stof(tokenizer));
			}
			transform.setRotation(Rotation(Vec3f(arr[0], arr[1], arr[2]), arr[3]));

		} else if (tokenizer == "scale") {
			float arr[3] = { 0 };
			int i = 0;
			while (i < 3 && tokenizer.get()) {
				arr[i++] = (stof(tokenizer));
			}
			transform.setScale(Vec3f(arr[0], arr[1], arr[2]));

		} else if (tokenizer == "scaleOrientation") {
			float arr[4] = { 0 };
			int i = 0;
			while (i < 4 && tokenizer.get()) {
				arr[i++] = (stof(tokenizer));
			}
			transform.setScaleOrientation(Rotation(Vec3f(arr[0], arr[1], arr[2]), arr[3]));

		} else if (tokenizer == "translation") {
			float arr[3] = { 0 };
			int i = 0;
			while (i < 3 && tokenizer.get()) {
				arr[i++] = (stof(tokenizer));
			}
			transform.setTranslation(Vec3f(arr[0], arr[1], arr[2]));

		} else if (tokenizer == "}") {
			success = true;
		} else {
			throw new StrException("unexpected token while parsing Group");
		}
	}
	return success;
}

//////////////////////////////////////////////////////////////////////
bool LoaderWrl::loadChildren(Tokenizer& tkn, Group& group) {

	bool   success = false;

	// TODO

	return success;
}

//////////////////////////////////////////////////////////////////////
bool LoaderWrl::loadShape(Tokenizer& tokenizer, Shape& shape) {

	bool success = false;
	string name;

	// DEF NAME Shape {
	//   SFNode appearance NULL
	//   SFNode geometry   NULL
	// }

	// or

	// Shape {
	//   SFNode appearance NULL
	//   SFNode geometry   NULL
	// }
	if (!tokenizer.expecting("{")) {
		throw new StrException("\'{\' not found after \'Shape\'");
	}
	while (success == false && tokenizer.get()) {
		if (tokenizer == "DEF") {
			tokenizer.get();
			name = tokenizer;
			continue;
		}
		if (tokenizer == "geometry") {
			//	is there always geometry before IndexedFaceSet or Line
			//	Maybe not
			tokenizer.get();
			if (tokenizer == "IndexedFaceSet") {
				IndexedFaceSet* ifs = new IndexedFaceSet();
				if (name != "") ifs->setName(name);
				name = "";
				loadIndexedFaceSet(tokenizer, *ifs);
				shape.setGeometry(ifs);
			} else if (tokenizer == "IndexedLineSet") {
				IndexedLineSet* ils = new IndexedLineSet();
				if (name != "") ils->setName(name);
				name = "";
				loadIndexedLineSet(tokenizer, *ils);
				shape.setGeometry(ils);
			} else {
				throw new StrException("Something wrong after \"geometry\"");
			}
		} else if (tokenizer == "appearance" && tokenizer.expecting("Appearance")) {
			Appearance* apprearance = new Appearance();
			loadAppearance(tokenizer, *apprearance);
			shape.setAppearance(apprearance);
		} else if (tokenizer == "}") {
			success = true;
		}
	}
	return success;
}

//////////////////////////////////////////////////////////////////////
bool LoaderWrl::loadAppearance(Tokenizer& tokenizer, Appearance& appearance) {

	// DEF NAME Appearance {
	//   SFNode material NULL
	//   SFNode texture NULL
	//   // SFNode textureTransform NULL
	// }

	// or

	// Appearance {
	//   SFNode material NULL
	//   SFNode texture NULL
	//   // SFNode textureTransform NULL
	// }

	bool   success = false;
	string name;

	if (!tokenizer.expecting("{")) {
		throw new StrException("NO \'{\' after \"Appearancel\"");
	}
	while (success == false && tokenizer.get()) {
		if (tokenizer == "DEF") {
			tokenizer.get();
			name = tokenizer;
			continue;
		}
		if (tokenizer == "material" && tokenizer.expecting("Material")) {
			Material* material = new Material();
			if (name != "") material->setName(name);
			name = "";
			loadMaterial(tokenizer, *material);
			appearance.setMaterial(material);
		} else if (tokenizer == "texture" && tokenizer.expecting("ImageTexture")) {
			ImageTexture* texture = new ImageTexture();
			if (name != "") texture->setName(name);
			name = "";
			loadImageTexture(tokenizer, *texture);
			appearance.setTexture(texture);
		} else if (tokenizer == "}") {
			success = true;
		}
	}
	return success;
}

//////////////////////////////////////////////////////////////////////
bool LoaderWrl::loadMaterial(Tokenizer& tokenizer, Material& material) {

	// DEF NAME Material {
	//   SFFloat ambientIntensity 0.2
	//   SFColor diffuseColor     0.8 0.8 0.8
	//   SFColor emissiveColor    0 0 0
	//   SFFloat shininess        0.2
	//   SFColor specularColor    0 0 0
	//   SFFloat transparency     0
	// }

	// or

	// Material {
	//   SFFloat ambientIntensity 0.2
	//   SFColor diffuseColor     0.8 0.8 0.8
	//   SFColor emissiveColor    0 0 0
	//   SFFloat shininess        0.2
	//   SFColor specularColor    0 0 0
	//   SFFloat transparency     0
	// }

	bool success = false;

	if (!tokenizer.expecting("{")) {
		throw new StrException("NO \'{\' after \"Material\"");
	}
	while (success == false && tokenizer.get()) {
		if (tokenizer == "ambientIntensity") {
			if (tokenizer.get()) {			//	if NOT a NUMBER???????????????
				material.setAmbientIntensity(stof(tokenizer));
			}
		} else if (tokenizer == "diffuseColor") {
			float arr[3] = { 0 };
			int i = 0;
			while (i < 3 && tokenizer.get()) {
				arr[i++] = (stof(tokenizer));
			}
			material.setDiffuseColor(Color::Color(arr[0], arr[1], arr[2]));
		} else if (tokenizer == "emissiveColor") {
			float arr[3] = { 0 };
			int i = 0;
			while (i < 3 && tokenizer.get()) {
				arr[i++] = (stof(tokenizer));
			}
			material.setEmissiveColor(Color::Color(arr[0], arr[1], arr[2]));
		} else if (tokenizer == "shininess") {
			if (tokenizer.get()) {			//	if NOT a NUMBER???????????????
				material.setShininess(stof(tokenizer));
			}
		} else if (tokenizer == "specularColor") {
			float arr[3] = { 0 };
			int i = 0;
			while (i < 3 && tokenizer.get()) {
				arr[i++] = (stof(tokenizer));
			}
			material.setSpecularColor(Color::Color(arr[0], arr[1], arr[2]));
		} else if (tokenizer == "transparency") {
			if (tokenizer.get()) {			//	if NOT a NUMBER???????????????
				material.setTransparency(stof(tokenizer));
			}
		} else if (tokenizer == "}") {
			success = true;
		}
	}

	return success;

}

//////////////////////////////////////////////////////////////////////
bool LoaderWrl::loadImageTexture(Tokenizer& tokenizer, ImageTexture& imageTexture) {

	// DEF NAME ImageTexture {
	//   MFString url []
	//   SFBool repeatS TRUE
	//   SFBool repeatT TRUE
	// }

	// or

	// ImageTexture {
	//   MFString url []
	//   SFBool repeatS TRUE
	//   SFBool repeatT TRUE
	// }

	bool success = false;

	if (!tokenizer.expecting("{")) {
		throw new StrException("NO \'{\' after \"ImageTexture\"");
	}
	while (success == false && tokenizer.get()) {
		// ---------- deal with URL -------------
		if (tokenizer == "url") {
			tokenizer.get();
			while (tokenizer.get() && tokenizer.at(0) == '\"') {
				imageTexture.adToUrl(tokenizer);
			}
			continue;
		}
		// --------------------------------------
		else if (tokenizer == "repeatS") {
			if (tokenizer.get()) {			//	if NOT a NUMBER???????????????
				if (tokenizer == "TRUE")
					imageTexture.setRepeatS(true);
				else imageTexture.setRepeatS(false);
			}
		} else if (tokenizer == "repeatT") {
			if (tokenizer.get()) {			//	if NOT a NUMBER???????????????
				if (tokenizer == "TRUE")
					imageTexture.setRepeatT(true);
				else imageTexture.setRepeatT(false);
			}
		} else if (tokenizer == "}") {
			success = true;
		}
	}

	return success;
}

//////////////////////////////////////////////////////////////////////
bool LoaderWrl::loadIndexedFaceSet(Tokenizer& tokenizer, IndexedFaceSet& ifs) {

	// DEF NAME IndexedFaceSet {
	//   SFNode  color             NULL
	//   SFNode  coord             NULL
	//   SFNode  normal            NULL
	//   SFNode  texCoord          NULL
	//   SFBool  ccw               TRUE
	//   MFInt32 colorIndex        []        # [-1,)
	//   SFBool  colorPerVertex    TRUE
	//   SFBool  convex            TRUE
	//   MFInt32 coordIndex        []        # [-1,)
	//   SFFloat creaseAngle       0         # [ 0,)
	//   MFInt32 normalIndex       []        # [-1,)
	//   SFBool  normalPerVertex   TRUE
	//   SFBool  solid             TRUE
	//   MFInt32 texCoordIndex     []        # [-1,)
	// }

	// or

	// IndexedFaceSet {
	//   SFNode  color             NULL
	//   SFNode  coord             NULL
	//   SFNode  normal            NULL
	//   SFNode  texCoord          NULL
	//   SFBool  ccw               TRUE
	//   MFInt32 colorIndex        []        # [-1,)
	//   SFBool  colorPerVertex    TRUE
	//   SFBool  convex            TRUE
	//   MFInt32 coordIndex        []        # [-1,)
	//   SFFloat creaseAngle       0         # [ 0,)
	//   MFInt32 normalIndex       []        # [-1,)
	//   SFBool  normalPerVertex   TRUE
	//   SFBool  solid             TRUE
	//   MFInt32 texCoordIndex     []        # [-1,)
	// }

	bool success = false;

	ifs.clear();

	bool& _ccw = ifs.getCcw();
	bool& _convex = ifs.getConvex();
	float& _creaseAngle = ifs.getCreaseangle();
	bool& _solid = ifs.getSolid();
	bool& _normalPerVertex = ifs.getNormalPerVertex();;
	bool& _colorPerVertex = ifs.getColorPerVertex();
	vector<float>& _coord = ifs.getCoord();
	vector<int>& _coordIndex = ifs.getCoordIndex();
	vector<float>& _normal = ifs.getNormal();
	vector<int>& _normalIndex = ifs.getNormalIndex();
	vector<float>& _color = ifs.getColor();
	vector<int>& _colorIndex = ifs.getColorIndex();
	vector<float>& _texCoord = ifs.getTexCoord();
	vector<int>& _texCoordIndex = ifs.getTexCoordIndex();

	if (!tokenizer.expecting("{")) {
		throw new StrException("NO \'{\' after \"IndexedFaceSet\"");
	}
	while (tokenizer.get()) {
		if (tokenizer == "coord") {
			if (!tokenizer.expecting("Coordinate")) {
				throw new StrException("NO \'Coordinate\' after \"IndexedFaceSet\"");
			}
			if (!tokenizer.expecting("{")) {
				throw new StrException("Coordinate \'{\' NOT found");
			}
			if (!tokenizer.expecting("point")) {
				throw new StrException("\'point\' NOT found");
			}
			if (!tokenizer.expecting("[")) {
				throw new StrException("point \'[\' NOT found");
			}
			while (tokenizer.get() && tokenizer != "]") {
				_coord.push_back(stof(tokenizer));
			}
			if (!tokenizer.expecting("}")) {
				throw new StrException("Coordinate \'}\' NOT found");
			}
		} else if (tokenizer == "coordIndex") {
			if (!tokenizer.expecting("[")) {
				throw new StrException("coordIndex \'[\' NOT found");
			}
			while (tokenizer.get() && tokenizer != "]") {
				_coordIndex.push_back(stoi(tokenizer));
			}
		} else if (tokenizer == "normal") {
			if (!tokenizer.expecting("Normal")) {
				throw new StrException("normal \'Normal\' NOT found");
			}
			if (!tokenizer.expecting("{")) {
				throw new StrException("Normal \'{\' NOT found");
			}
			if (!tokenizer.expecting("vector")) {
				throw new StrException("\'vector\' NOT found");
			}
			if (!tokenizer.expecting("[")) {
				throw new StrException("vector \'[\' NOT found");
			}
			while (tokenizer.get() && tokenizer != "]") {
				_normal.push_back(stof(tokenizer));
			}
			if (!tokenizer.expecting("}")) {
				throw new StrException("Normal \'}\' NOT found");
			}
		} else if (tokenizer == "normalIndex") {
			if (!tokenizer.expecting("[")) {
				throw new StrException("normalIndex \'[\' NOT found");
			}
			while (tokenizer.get() && tokenizer != "]") {
				_normalIndex.push_back(stoi(tokenizer));
			}
		} else if (tokenizer == "color") {
			if (!tokenizer.expecting("Color")) {
				throw new StrException("color \'Color\' NOT found");
			}
			if (!tokenizer.expecting("{")) {
				throw new StrException("Color \'{\' NOT found");
			}
			if (!tokenizer.expecting("color")) {
				throw new StrException("\'color\' NOT found");
			}
			if (!tokenizer.expecting("[")) {
				throw new StrException("color \'[\' NOT found");
			}
			while (tokenizer.get() && tokenizer != "]") {
				_color.push_back(stof(tokenizer));
			}
			if (!tokenizer.expecting("}")) {
				throw new StrException("Color \'}\' NOT found");
			}
		} else if (tokenizer == "colorIndex") {
			if (!tokenizer.expecting("[")) {
				throw new StrException("colorIndex \'[\' NOT found");
			}
			while (tokenizer.get() && tokenizer != "]") {
				_colorIndex.push_back(stoi(tokenizer));
			}
		} else if (tokenizer == "texCoord") {
			if (!tokenizer.expecting("TextureCoordinate")) {
				throw new StrException("texCoord \'TextureCoordinate\' NOT found");
			}
			if (!tokenizer.expecting("{")) {
				throw new StrException("TextureCoordinate \'{\' NOT found");
			}
			if (!tokenizer.expecting("point")) {
				throw new StrException("\'point\' NOT found");
			}
			if (!tokenizer.expecting("[")) {
				throw new StrException("point \'[\' NOT found");
			}
			while (tokenizer.get() && tokenizer != "]") {
				_texCoord.push_back(stof(tokenizer));
			}
			if (!tokenizer.expecting("}")) {
				throw new StrException("TextureCoordinate \'}\' NOT found");
			}
		} else if (tokenizer == "texCoordIndex") {
			if (!tokenizer.expecting("[")) {
				throw new StrException("texCoordIndex \'[\' NOT found");
			}
			while (tokenizer.get() && tokenizer != "]") {
				_texCoordIndex.push_back(stoi(tokenizer));
			}
		} else if (tokenizer == "ccw") {
			if (tokenizer.get() && tokenizer == "TRUE") {
				_ccw = true;
			} else if (tokenizer == "FALSE") {
				_ccw = false;
			} else {
				throw new StrException("ccw \'TRUE\' | \'FALSE\' NOT found");
			}
		} else if (tokenizer == "convex") {
			if (tokenizer.get() && tokenizer == "TRUE") {
				_convex = true;
			} else if (tokenizer == "FALSE") {
				_convex = false;
			} else {
				throw new StrException("convex \'TRUE\' | \'FALSE\' NOT found");
			}
		} else if (tokenizer == "solid") {
			if (tokenizer.get() && tokenizer == "TRUE") {
				_solid = true;
			} else if (tokenizer == "FALSE") {
				_solid = false;
			} else {
				throw new StrException("solid \'TRUE\' | \'FALSE\' NOT found");
			}
		} else if (tokenizer == "normalPerVertex") {
			if (tokenizer.get() && tokenizer == "TRUE") {
				_normalPerVertex = true;
			} else if (tokenizer == "FALSE") {
				_normalPerVertex = false;
			} else {
				throw new StrException("normPerVertex \'TRUE\' | \'FALSE\' NOT found");
			}
		} else if (tokenizer == "colorPerVertex") {
			if (tokenizer.get() && tokenizer == "TRUE") {
				_colorPerVertex = true;
			} else if (tokenizer == "FALSE") {
				_colorPerVertex = false;
			} else {
				throw new StrException("colorPerVertex \'TRUE\' | \'FALSE\' NOT found");
			}
		} else if (tokenizer == "creaseAngle") {
			if (tokenizer.get()) {
				_creaseAngle = stof(tokenizer);
			} else {
				throw new StrException("creasePerVertex \'number\' NOT found");
			}
		} else if (tokenizer == "}") {
			success = true;
			break;
		} else {
			throw new StrException("IndexedFaceSet \'}\' NOT found");
		}
	}

	return success;
}

//////////////////////////////////////////////////////////////////////
bool LoaderWrl::loadIndexedLineSet(Tokenizer& tokenizer, IndexedLineSet& ils) {

	// DEF NAME IndexedLineSet {
	//   SFNode  coord             NULL
	//   MFInt32 coordIndex        []        # [-1,)
	//   SFNode  color             NULL
	//   MFInt32 colorIndex        []        # [-1,)
	//   SFBool  colorPerVertex    TRUE
	// }

	// or

	// IndexedLineSet {
	//   SFNode  coord             NULL
	//   MFInt32 coordIndex        []        # [-1,)
	//   SFNode  color             NULL
	//   MFInt32 colorIndex        []        # [-1,)
	//   SFBool  colorPerVertex    TRUE
	// }

	bool success = false;

	// TODO
	ils.clear();

	bool& _colorPerVertex = ils.getColorPerVertex();
	vector<float>& _coord = ils.getCoord();
	vector<int>& _coordIndex = ils.getCoordIndex();
	vector<float>& _color = ils.getColor();
	vector<int>& _colorIndex = ils.getColorIndex();

	if (!tokenizer.expecting("{")) {
		throw new StrException("NO \'{\' after \"IndexedFaceSet\"");
	}
	while (tokenizer.get()) {
		if (tokenizer == "coord") {
			if (!tokenizer.expecting("Coordinate")) {
				throw new StrException("NO \'Coordinate\' after \"IndexedFaceSet\"");
			}
			if (!tokenizer.expecting("{")) {
				throw new StrException("Coordinate \'{\' NOT found");
			}
			if (!tokenizer.expecting("point")) {
				throw new StrException("\'point\' NOT found");
			}
			if (!tokenizer.expecting("[")) {
				throw new StrException("point \'[\' NOT found");
			}
			while (tokenizer.get() && tokenizer != "]") {
				_coord.push_back(stof(tokenizer));
			}
			if (!tokenizer.expecting("}")) {
				throw new StrException("Coordinate \'}\' NOT found");
			}
		} else if (tokenizer == "coordIndex") {
			if (!tokenizer.expecting("[")) {
				throw new StrException("coordIndex \'[\' NOT found");
			}
			while (tokenizer.get() && tokenizer != "]") {
				_coordIndex.push_back(stoi(tokenizer));
			}
		} else if (tokenizer == "color") {
			if (!tokenizer.expecting("Color")) {
				throw new StrException("color \'Color\' NOT found");
			}
			if (!tokenizer.expecting("{")) {
				throw new StrException("Color \'{\' NOT found");
			}
			if (!tokenizer.expecting("color")) {
				throw new StrException("\'color\' NOT found");
			}
			if (!tokenizer.expecting("[")) {
				throw new StrException("color \'[\' NOT found");
			}
			while (tokenizer.get() && tokenizer != "]") {
				_color.push_back(stof(tokenizer));
			}
			if (!tokenizer.expecting("}")) {
				throw new StrException("Color \'}\' NOT found");
			}
		} else if (tokenizer == "colorIndex") {
			if (!tokenizer.expecting("[")) {
				throw new StrException("colorIndex \'[\' NOT found");
			}
			while (tokenizer.get() && tokenizer != "]") {
				_colorIndex.push_back(stoi(tokenizer));
			}
		} else if (tokenizer == "colorPerVertex") {
			if (tokenizer.get() && tokenizer == "TRUE") {
				_colorPerVertex = true;
			} else if (tokenizer == "FALSE") {
				_colorPerVertex = false;
			} else {
				throw new StrException("colorPerVertex \'TRUE\' | \'FALSE\' NOT found");
			}
		} else if (tokenizer == "}") {
			success = true;
			break;
		} else {
			throw new StrException("IndexedLineSet \'}\' NOT found");
		}
	}

	return success;
}

//////////////////////////////////////////////////////////////////////
bool LoaderWrl::loadVecFloat(Tokenizer& tkn, vector<float>& vec) {

	bool success = false;

	// TODO

	return success;
}

//////////////////////////////////////////////////////////////////////
bool LoaderWrl::loadVecInt(Tokenizer& tkn, vector<int>& vec) {

	bool success = false;

	// TODO

	return success;
}

//////////////////////////////////////////////////////////////////////
bool LoaderWrl::loadVecString(Tokenizer& tkn, vector<string>& vec) {

	bool success = false;

	// TODO

	return success;
}

//////////////////////////////////////////////////////////////////////
bool LoaderWrl::load(const char* filename, SceneGraph& wrl) {

	bool success = false;

	try {

		// open the file
		std::ifstream ifs(filename);

		// clear the container
		wrl.clear();
		// save the filename
		wrl.setUrl(filename);

		// read and check header line
		char header[16] = "";
		// first line of file must be equal to VRML_HEADER
		char cstr[20];
		ifs.getline(cstr, 20);
		string str = cstr;
		if (str != VRML_HEADER) cerr << "ERROR: VRML HEADER MISSING" << endl;

		// create a Tokenizer and start parsing
		Tokenizer tkn(ifs);
		loadSceneGraph(tkn, wrl);

		wrl.updateBBox();

		// if we have reached this point we have succeeded
		// close file

		success = true;

	} catch (StrException * e) {

		// if we catch an exception
		// 1) close file if still open
		// 2) print an error message
		// 3) delete e;
		// 4) clear the wrl container
		// 5) clear the wrl container url field

		success = false;

	}

	return success;
}

