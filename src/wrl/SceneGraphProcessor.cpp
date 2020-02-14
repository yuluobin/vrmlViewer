//------------------------------------------------------------------------
//  Copyright (C) Gabriel Taubin
//  Time-stamp: <2019-11-03 14:59:31 taubin>
//------------------------------------------------------------------------
//
// SceneGraphProcessor.cpp
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
#include "SceneGraphProcessor.hpp"
#include "SceneGraphTraversal.hpp"
#include "Shape.hpp"
#include "IndexedFaceSet.hpp"
#include "IndexedLineSet.hpp"
#include "Appearance.hpp"
#include "Material.hpp"
#include "IsoSurf.hpp"

using namespace Eigen;

SceneGraphProcessor::SceneGraphProcessor(SceneGraph& wrl) :
    _wrl(wrl),
    _nGrid(0),
    _nPoints(0),
    _next((int*)0),
    _first((int*)0) {
}

SceneGraphProcessor::~SceneGraphProcessor() {
    _deletePartition();
}

int SceneGraphProcessor::_createPartition
(Vec3f& min, Vec3f& max, int depth, vector<float>& coord) {
    int nPointsInPartition = 0;
    _deletePartition();
    float dx = max.x - min.x;
    float dy = max.y - min.y;
    float dz = max.z - min.z;
    if (dx > 0.0f && dy > 0.0f && dz > 0.0f) {
        _nPoints = (int)(coord.size() / 3);
        if (depth < 0) depth = 0;
        _nGrid = 1 << depth;
        int nCells = _nCells(); // N * N * N
        _next = new int[_nPoints];
        _first = new int[nCells + _nGrid];
        int iCell, iPoint, ix, iy, iz;
        float x, y, z, nG = (float)_nGrid;
        for (iCell = 0; iCell < nCells; iCell++)
            _first[iCell] = -1;
        for (iPoint = 0; iPoint < _nPoints; iPoint++) {
            if ((x = coord[3 * iPoint]) < min.x || x > max.x) continue;
            if ((y = coord[3 * iPoint + 1]) < min.y || y > max.y) continue;
            if ((z = coord[3 * iPoint + 2]) < min.z || z > max.z) continue;
            ix = (int)((nG * (x - min.x)) / dx);
            iy = (int)((nG * (y - min.y)) / dy);
            iz = (int)((nG * (z - min.z)) / dz);
            iCell = ix + _nGrid * (iy + _nGrid * iz);
            _next[iPoint] = _first[iCell];
            _first[iCell] = iPoint;
            nPointsInPartition++;
        }
    }
    return nPointsInPartition;
}

void SceneGraphProcessor::_deletePartition() {
    if (_first != (int*)0) delete[] _first;
    if (_next != (int*)0) delete[] _next;
    _nGrid = 0;
    _nPoints = 0;
    _next = (int*)0;
    _first = (int*)0;
}

void SceneGraphProcessor::normalClear() {
    _applyToIndexedFaceSet(_normalClear);
}

void SceneGraphProcessor::normalInvert() {
    _applyToIndexedFaceSet(_normalInvert);
}

void SceneGraphProcessor::computeNormalPerFace() {
    _applyToIndexedFaceSet(_computeNormalPerFace);
}

void SceneGraphProcessor::computeNormalPerVertex() {
    _applyToIndexedFaceSet(_computeNormalPerVertex);
}

void SceneGraphProcessor::computeNormalPerCorner() {
    _applyToIndexedFaceSet(_computeNormalPerCorner);
}

void SceneGraphProcessor::_applyToIndexedFaceSet(IndexedFaceSet::Operator o) {
    SceneGraphTraversal traversal(_wrl);
    traversal.start();
    Node* node;
    while ((node = traversal.next()) != (Node*)0) {
        if (node->isShape()) {
            Shape* shape = (Shape*)node;
            node = shape->getGeometry();
            if (node != (Node*)0 && node->isIndexedFaceSet()) {
                IndexedFaceSet& ifs = *((IndexedFaceSet*)node);
                o(ifs);
            }
        }
    }
}

// Remove normal vectors from all the IndexedFaceSet nodes contained in the SceneGraph
void SceneGraphProcessor::_normalClear(IndexedFaceSet& ifs) {
    vector<float>& normal = ifs.getNormal();
    vector<int>& normalIndex = ifs.getNormalIndex();
    ifs.setNormalPerVertex(true);
    normal.clear();
    normalIndex.clear();
}

// Change the sign of all the normal vector coordinates stored in the `normal` array
void SceneGraphProcessor::_normalInvert(IndexedFaceSet& ifs) {
    vector<float>& normal = ifs.getNormal();
    for (int i = 0; i < (int)normal.size(); i++)
        normal[i] = -normal[i];
}

void SceneGraphProcessor::_computeFaceNormal
(vector<float>& coord, vector<int>& coordIndex,
    int i0, int i1, Vector3d& n, bool normalize) {

    // TODO

    // compute normal vector to face defined by the vertex indices
    // {coordIndex[i0],...,coordIndex[i1-1]}
    vector<Eigen::Vector3d> points;
    int count = 0;
    for (int i = i0; i < i1; i++, count++) {
        int iV = coordIndex[i];
        Eigen::Vector3d temp;
        points.push_back(temp);
        points[count](0) = coord[3 * iV];
        points[count](1) = coord[3 * iV + 1];
        points[count](2) = coord[3 * iV + 2];
    }
    Eigen::Vector3d sum(0, 0, 0);
    for (int i = 0; i < points.size(); i++) {
        sum += points[i];
    }
    //  find two vector not collinear
    sum = sum / points.size();
    int size = points.size();
    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            Eigen::Vector3d vec1, vec2;
            vec1 = points[i] - sum;
            vec2 = points[j] - sum;
            if ((vec1.cross(vec2)).norm() != 0) {
                // store the result in the 3D vector n
                // normalize to unit length or not, depending on the 'normalize' variable 
                n = normalize == true ? (vec1.cross(vec2)).normalized() : vec1.cross(vec2);
                return;
            } else {
                throw new StrException("There exits collinear");
            }
        }
    }
    throw new StrException("There exits collinear");
}

void SceneGraphProcessor::_computeNormalPerFace(IndexedFaceSet& ifs) {
    if (ifs.getNormalBinding() == IndexedFaceSet::PB_PER_FACE) return;

    // TODO
    vector<float>& coord = ifs.getCoord();
    vector<int>& coordIndex = ifs.getCoordIndex();
    vector<float>& normal = ifs.getNormal();
    vector<int>& normalIndex = ifs.getNormalIndex();
    normal.clear();
    normalIndex.clear();
    ifs.setNormalPerVertex(false);

    /*
    iF:         index of face
    iV:         index of vertice
    x0, x1, x2: coords of a point
    */
    int iF, iV, i, i0, i1;
    // float n0, n1, n2, x0, x1, x2;
    Eigen::Vector3d n;
    for (iF = i0 = i1 = 0; i1 < coordIndex.size(); i1++) {
        if (coordIndex[i1] < 0) {   // the 4-th parameter is -1
            SceneGraphProcessor::_computeFaceNormal(coord, coordIndex,
                i0, i1, n, true);
            for (i = 0; i < 3; i++) {
                normal.push_back(n(i));
            }

            i0 = i1 + 1; iF++;
        }
    }
}

void SceneGraphProcessor::_computeNormalPerVertex(IndexedFaceSet& ifs) {
    if (ifs.getNormalBinding() == IndexedFaceSet::PB_PER_VERTEX) return;

    // TODO
    vector<float>& coord = ifs.getCoord();
    vector<int>& coordIndex = ifs.getCoordIndex();
    vector<float>& normal = ifs.getNormal();
    vector<int>& normalIndex = ifs.getNormalIndex();

    int nV = coord.size() / 3;  // nV = number of vertices
    normal.clear();
    normalIndex.clear(); // will not be used
    ifs.setNormalPerVertex(false);
    // compute face normals without normalization
    int iF, iV, i, i0, i1;
    Eigen::Vector3d n;
    for (iF = i0 = i1 = 0; i1 < coordIndex.size(); i1++) {
        if (coordIndex[i1] < 0) {   // the 4-th parameter is -1
            SceneGraphProcessor::_computeFaceNormal(coord, coordIndex,
                i0, i1, n, false);
            for (i = 0; i < 3; i++) {
                normal.push_back(n(i));
            }
            i0 = i1 + 1; iF++;
        }
    }

    // save the computed face normal and clear the ifs normals
    vector<float> faceNormal;
    faceNormal.swap(normal);
    // at this point normal.size()==0
    // insert coord.size() zeros onto the normal array
    normal.resize(coord.size());
    std::fill(normal.begin(), normal.begin() + coord.size(), 0); // coord.size()==normal.size();
    ifs.setNormalPerVertex(true);

    for (iF = i0 = i1 = 0; i1 < coordIndex.size(); i1++) {
        if (coordIndex[i1] < 0) {
            // retrieve this face normal coordinates from the faceNormal array
            for (i = i0; i < i1; i++) {
                iV = coordIndex[i];
                // add face normal coordinates to this vertex normal accumulator
                normal[3 * iV] = faceNormal[3 * iV];
                normal[3 * iV + 1] = normal[3 * iV] + faceNormal[3 * iV + 1];
                normal[3 * iV + 2] = normal[3 * iV + 1] + faceNormal[3 * iV + 2];
            }
            i0 = i1 + 1; iF++;
        }
    }
    Eigen::Vector3d vec;
    for (iV = 0; iV < nV; iV++) {
        // normalize vertex normal to unit length
        vec(0) = normal[3 * iV];
        vec(1) = normal[3 * iV + 1];
        vec(2) = normal[3 * iV + 2];
        vec.normalize();
        normal[3 * iV] = vec(0);
        normal[3 * iV + 1] = vec(1);
        normal[3 * iV + 2] = vec(2);
    }
}

void SceneGraphProcessor::_computeNormalPerCorner(IndexedFaceSet& ifs) {
    if (ifs.getNormalBinding() == IndexedFaceSet::PB_PER_CORNER) return;

    // TODO
    vector<float>& coord = ifs.getCoord();
    vector<int>& coordIndex = ifs.getCoordIndex();
    vector<float>& normal = ifs.getNormal();
    vector<int>& normalIndex = ifs.getNormalIndex();
    normal.clear();
    normalIndex.clear();
    ifs.setNormalPerVertex(true);

    int iF, iV0, iV1, iV2, i, i0, i1, iN;
    for (iF = i0 = i1 = 0; i1 < coordIndex.size(); i1++) {
        if (coordIndex[i1] < 0) { // end of face marker found
            for (i = i0; i < i1; i++) {
                //  iV0, iV1, iV2 are like a circle
                iV0 = i == i0 ? coordIndex[i1 - 1] : coordIndex[i - 1]; // [i1-1] if i==0
                iV1 = coordIndex[i];
                iV2 = i == i1 - 1 ? coordIndex[i0] : coordIndex[i + 1]; // [i0] if i==i0-1
                Eigen::Vector3d temp0(coord[iV0 * 3], coord[iV0 * 3 + 1], coord[iV0 * 3 + 2]);
                Eigen::Vector3d temp1(coord[iV1 * 3], coord[iV1 * 3 + 1], coord[iV1 * 3 + 2]);
                Eigen::Vector3d temp2(coord[iV2 * 3], coord[iV2 * 3 + 1], coord[iV2 * 3 + 2]);
                Eigen::Vector3d vec1, vec2;
                vec1 = temp0 - temp1;
                vec2 = temp2 - temp1;
                if ((vec1.cross(vec2)).norm() == 0) {
                    throw new StrException("There exits collinear");
                }
                Eigen::Vector3d vec = (vec1.cross(vec2)).normalized();

                iN = normal.size() / 3;
                for (int k = 0; k < 3; k++) {
                    normal.push_back(vec(k));
                }
                normalIndex.push_back(iN);
            }
            normalIndex.push_back(-1);
            i0 = i1 + 1; iF++;
        }
    }
}

void SceneGraphProcessor::bboxAdd(int depth, float scale, bool cube) {

    // 1) look for a Shape node named "BOUNDING-BOX" as a child of the
    // SceneGraph root node; if not found create a new one with that
    // name, add an Appearance node to the Shape node, and a Material
    // node to the Appearance node; assign a proper value to the
    // diffuseColor of the Material node; add the Shape node as a child
    // of the SceneGraph

    Shape* s = new Shape();
    int n = _wrl.getNumberOfChildren();
    for (int i = 0; i < n; i++) {
        if (_wrl[i]->getName() == "BOUNDING-BOX") {
            s = (Shape*)_wrl.getChildren()[i];
            break;
        }
    }
    if (!s->hasAppearanceMaterial()) {
        s->setName("BOUNDING-BOX");
        Appearance* appearance = new Appearance();
        Material* material = new Material();
        material->setDiffuseColor(Color(0.5, 0.3, 0.0));
        material->setTransparency(0.0);

        appearance->setMaterial(material);
        s->setAppearance(appearance);
        _wrl.addChild(s);
    }

    // 2) look for an IndexedFaceSet node in the geometry fields of the
    // Shape node; if an IndexedFaceSet is found, delete the
    // IndexedFaceSet and set the geometry field to (Node*)0; if the
    // geometry field is equal to (Node*)0, create a new IndexedLineSet;

    // HOW TO DELETE INDEXEDFACESET????????????????????????????????????????????
    if (s->hasGeometryIndexedFaceSet()) {
        delete s->getGeometry();
        s->setGeometry((Node*)0);
    }
    IndexedLineSet* ils = new IndexedLineSet();
    if (s->getGeometry() == (Node*)0 || !s->hasGeometryIndexedLineSet()) {
        ils = new IndexedLineSet();
        s->setGeometry(ils);
    } else {
        // 3) clear the IndexedLineSet
        ils = (IndexedLineSet*)s->getGeometry();
        ils->clear();
    }

    // 4) update the SceneGraph bounding box

    _wrl.updateBBox();

    // 5) create the vertices of the regular grid of side N=2^depth;
    // such grid has (N+1)*(N+1)*(N+1) vertices; store the coordinates
    // in the coord field of the IndexedLineSet

    vector<float>& _coord = ils->getCoord();
    int N = pow(2, depth);
    float x0, x1, y0, y1, z0, z1;
    if (cube) {
        float maxTemp = max(max(_wrl.getBBoxSize().x, _wrl.getBBoxSize().y), _wrl.getBBoxSize().z);
        x0 = (_wrl.getBBoxCenter()).x - (maxTemp / 2) * scale;
        x1 = (_wrl.getBBoxCenter()).x + (maxTemp / 2) * scale;
        y0 = (_wrl.getBBoxCenter()).y - (maxTemp / 2) * scale;
        y1 = (_wrl.getBBoxCenter()).y + (maxTemp / 2) * scale;
        z0 = (_wrl.getBBoxCenter()).z - (maxTemp / 2) * scale;
        z1 = (_wrl.getBBoxCenter()).z + (maxTemp / 2) * scale;
    } else {
        x0 = (_wrl.getBBoxCenter()).x - (_wrl.getBBoxSize().x / 2) * scale;
        x1 = (_wrl.getBBoxCenter()).x + (_wrl.getBBoxSize().x / 2) * scale;
        y0 = (_wrl.getBBoxCenter()).y - (_wrl.getBBoxSize().y / 2) * scale;
        y1 = (_wrl.getBBoxCenter()).y + (_wrl.getBBoxSize().y / 2) * scale;
        z0 = (_wrl.getBBoxCenter()).z - (_wrl.getBBoxSize().z / 2) * scale;
        z1 = (_wrl.getBBoxCenter()).z + (_wrl.getBBoxSize().z / 2) * scale;
    }

    for (int i = 0; i < N + 1; i++) {
        for (int j = 0; j < N + 1; j++) {
            for (int k = 0; k < N + 1; k++) {
                float x = ((N - i) * x0 + i * x1) / N;
                float y = ((N - j) * y0 + j * y1) / N;
                float z = ((N - k) * z0 + k * z1) / N;

                _coord.push_back(x);
                _coord.push_back(y);
                _coord.push_back(z);
            }
        }
    }

    // 6) create the edges of the regular grid; such grid has
    // 3*(N+1)*(N+1)*N edges

    set<pairs> set;
    vector<int>& _coordIndex = ils->getCoordIndex();
    for (int i = 0; i < (N + 1) * (N + 1) * N; i += (N + 1) * (N + 1)) {
        for (int j = 0; j < (N + 1) * N; j += N + 1) {
            for (int k = 0; k < N; k++) {
                int min = i + j + k;
                pairs pair = make_pair(min, min + 1);
                if (set.find(pair) == set.end()) {
                    set.insert(pair);
                    _coordIndex.push_back(min);
                    _coordIndex.push_back(min + 1);
                    _coordIndex.push_back(-1);
                }
                pair = make_pair(min, min + N + 1);
                if (set.find(pair) == set.end()) {
                    set.insert(pair);
                    _coordIndex.push_back(min);
                    _coordIndex.push_back(min + N + 1);
                    _coordIndex.push_back(-1);
                }
                pair = make_pair(min, min + (N + 1) * (N + 1));
                if (set.find(pair) == set.end()) {
                    set.insert(pair);
                    _coordIndex.push_back(min);
                    _coordIndex.push_back(min + (N + 1) * (N + 1));
                    _coordIndex.push_back(-1);
                }
                pair = make_pair(min + 1, min + 1 + N + 1);
                if (set.find(pair) == set.end()) {
                    set.insert(pair);
                    _coordIndex.push_back(min + 1);
                    _coordIndex.push_back(min + 1 + N + 1);
                    _coordIndex.push_back(-1);
                }
                pair = make_pair(min + 1, min + 1 + (N + 1) * (N + 1));
                if (set.find(pair) == set.end()) {
                    set.insert(pair);
                    _coordIndex.push_back(min + 1);
                    _coordIndex.push_back(min + 1 + (N + 1) * (N + 1));
                    _coordIndex.push_back(-1);
                }
                pair = make_pair(min + N + 1, min + N + 2);
                if (set.find(pair) == set.end()) {
                    set.insert(pair);
                    _coordIndex.push_back(min + N + 1);
                    _coordIndex.push_back(min + N + 2);
                    _coordIndex.push_back(-1);
                }
                pair = make_pair(min + N + 1, min + N + 1 + (N + 1) * (N + 1));
                if (set.find(pair) == set.end()) {
                    set.insert(pair);
                    _coordIndex.push_back(min + N + 1);
                    _coordIndex.push_back(min + N + 1 + (N + 1) * (N + 1));
                    _coordIndex.push_back(-1);
                }
                pair = make_pair(min + N + 2, min + N + 2 + (N + 1) * (N + 1));
                if (set.find(pair) == set.end()) {
                    set.insert(pair);
                    _coordIndex.push_back(min + N + 2);
                    _coordIndex.push_back(min + N + 2 + (N + 1) * (N + 1));
                    _coordIndex.push_back(-1);
                }
                pair = make_pair(min + (N + 1) * (N + 1), min + (N + 1) * (N + 1) + 1);
                if (set.find(pair) == set.end()) {
                    set.insert(pair);
                    _coordIndex.push_back(min + (N + 1) * (N + 1));
                    _coordIndex.push_back(min + (N + 1) * (N + 1) + 1);
                    _coordIndex.push_back(-1);
                }
                pair = make_pair(min + (N + 1) * (N + 1), min + (N + 1) * (N + 1) + N + 1);
                if (set.find(pair) == set.end()) {
                    set.insert(pair);
                    _coordIndex.push_back(min + (N + 1) * (N + 1));
                    _coordIndex.push_back(min + (N + 1) * (N + 1) + N + 1);
                    _coordIndex.push_back(-1);
                }
                pair = make_pair(min + (N + 1) * (N + 1) + 1, min + (N + 1) * (N + 1) + 1 + N + 1);
                if (set.find(pair) == set.end()) {
                    set.insert(pair);
                    _coordIndex.push_back(min + (N + 1) * (N + 1) + 1);
                    _coordIndex.push_back(min + (N + 1) * (N + 1) + 1 + N + 1);
                    _coordIndex.push_back(-1);
                }
                pair = make_pair(min + (N + 1) * (N + 1) + N + 1, min + (N + 1) * (N + 1) + N + 1 + 1);
                if (set.find(pair) == set.end()) {
                    set.insert(pair);
                    _coordIndex.push_back(min + (N + 1) * (N + 1) + N + 1);
                    _coordIndex.push_back(min + (N + 1) * (N + 1) + N + 1 + 1);
                    _coordIndex.push_back(-1);
                }
            }
        }
    }
}

void SceneGraphProcessor::bboxRemove() {

    // TODO

    // 1) look for a child of the SceneGraph named "BOUNDING-BOX"
    // 2) if found, remove from the children array and delete
    int n = _wrl.getNumberOfChildren();
    for (int i = 0; i < n; i++) {
        if (_wrl[i]->getName() == "BOUNDING-BOX")
            _wrl.getChildren().erase(_wrl.getChildren().begin() + i);
    }
}

void SceneGraphProcessor::edgesAdd() {

    // TODO

    // 1) using a SceneGraphTraversal, look for Shape nodes

    SceneGraphTraversal traversal(_wrl);
    traversal.start();
    Node* node;
    while ((node = traversal.next()) != (Node*)0) {
        if (node->isShape()) {
            // 2) for each Shape node containing an IndexedFaceSet as geometry
            Shape* shape = (Shape*)node;
            if (shape->hasGeometryIndexedFaceSet() && shape->getName() != "EDGES") {
                Group* parent = (Group*)shape->getParent();
                Shape* edge = new Shape();
                edge->setName("EDGES");
                // 3) look for another Shape node, sibiling of the previous Shape
                // node (i.e. child of the same Group node), named "EDGES"; if not
                // found create a new one, and insert it as a new child of the Group
                // node which is parent of the Shape node; if the "EDGES" Shape node
                // does not contain an IndexedLineSet as geometry, delete the
                // geometry node, and set to (Node*)0; 
                // if the geometry node is equal to (Node*)0, create a new IndexedLineSet
                // and save it in the geometry field of the first Shape node
                int n = parent->getNumberOfChildren();
                bool find = false;
                for (int i = 0; i < n; i++) {
                    if (parent->getChildren()[i]->getName() == "EDGES") {
                        find = true;
                        edge = (Shape*)parent->getChildren()[i];
                    }
                }
                if (find == false) {
                    parent->addChild(edge);
                }
                if (!edge->hasGeometryIndexedLineSet()) {
                    delete edge->getGeometry();
                    edge->setGeometry((Node*)0);
                }
                // ?????????????????????????????????????????????????????????
                IndexedLineSet* ils = new IndexedLineSet();
                if (edge->getGeometry() == (Node*)0) {
                    edge->setGeometry(ils);
                } else {
                    ils = (IndexedLineSet*)edge->getGeometry();
                }
                // 4) make sure that the appearance field of the EDGES Shape node
                // contains and Appearance node, and the material field of the
                // Appearance node contains a Material node; if node create them;
                // set the diffuseColor field of the Material node to an appropriate
                // value
                Appearance* appearance = new Appearance();
                Material* material = new Material();
                material->setDiffuseColor(Color(0.1, 0.5, 0.7));
                material->setTransparency(0.0);
                appearance->setMaterial(material);
                edge->setAppearance(appearance);
                // 5) clear the IndexedLineSet node
                ils->clear();
                // 6) copy the coord field from the IndexedFaceSet node to the
                // IndexedLineSet node
                vector<float>& _coordTemp = ((IndexedFaceSet*)shape->getGeometry())->getCoord();
                vector<float>& _coord = ils->getCoord();
                int size = _coordTemp.size();
                for (int i = 0; i < size; i++) {
                    _coord.push_back(_coordTemp[i]);
                }
                // 7) for each face of the IndexedFaceSet node, determine the
                // corresponding boundary edges and store them in the coordIndex
                // array of the IndexedLineSet
                set<pairs> set;
                vector<int>& _coordIndexTemp = ((IndexedFaceSet*)shape->getGeometry())->getCoordIndex();
                vector<int>& _coordIndex = ils->getCoordIndex();
                int start = 0;
                for (int i = 0; i < _coordIndexTemp.size(); i++) {
                    if (_coordIndexTemp[i] == -1) {
                        for (int j = start; j < i - 1; j++) {
                            pairs pair1 = make_pair(_coordIndexTemp[j], _coordIndexTemp[j + 1]);
                            pairs pair2 = make_pair(_coordIndexTemp[j + 1], _coordIndexTemp[j]);
                            if (set.find(pair1) == set.end() && set.find(pair2) == set.end()) {
                                set.insert(pair1);
                                _coordIndex.push_back(_coordIndexTemp[j]);
                                _coordIndex.push_back(_coordIndexTemp[j + 1]);
                                _coordIndex.push_back(-1);
                            }
                        }
                        start = i + 1;
                    }
                }
                n = _coordIndex.size();
            }
        }
    }
}

void SceneGraphProcessor::edgesRemove() {

    // TODO

    // using a SceneGraphTraversal, look for all the shape nodes
    // named "EDGES", remove them from the scene graph and delete them
    SceneGraphTraversal traversal(_wrl);
    traversal.start();
    Node* node;
    while ((node = traversal.next()) != (Node*)0) {
        if (node->isShape() && node->getName() == "EDGES") {
            node = (Node*)0;
            int n = _wrl.getNumberOfChildren();
            for (int i = 0; i < n; i++) {
                if (_wrl.getChildren()[i]->getName() == "EDGES") {
                    _wrl.getChildren().erase(_wrl.getChildren().begin() + i);
                    break;
                }
            }
        }
    }
}

void SceneGraphProcessor::shapeIndexedFaceSetShow() {

    // TODO

    // using a SceneGraphTraversal look for all Shape nodes containing
    // an IndexedFaceSet as geometry, and set the show variable to true

    SceneGraphTraversal traversal(_wrl);
    traversal.start();
    Node* node;
    while ((node = traversal.next()) != (Node*)0) {
        if (node->isShape()) {
            Shape* shape = (Shape*)node;
            if (shape->hasGeometryIndexedFaceSet())
                shape->setShow(true);
        }
    }
}

void SceneGraphProcessor::shapeIndexedFaceSetHide() {

    // TODO

    // using a SceneGraphTraversal look for all Shape nodes containing
    // an IndexedFaceSet as geometry, and set the show variable to false

    SceneGraphTraversal traversal(_wrl);
    traversal.start();
    Node* node;
    while ((node = traversal.next()) != (Node*)0) {
        if (node->isShape()) {
            Shape* shape = (Shape*)node;
            if (shape->hasGeometryIndexedFaceSet())
                shape->setShow(false);
        }
    }

}

void SceneGraphProcessor::shapeIndexedLineSetShow() {

    // TODO

    // using a SceneGraphTraversal look for all Shape nodes containing
    // an IndexedLineSet as geometry, and set the show variable to true

    SceneGraphTraversal traversal(_wrl);
    traversal.start();
    Node* node;
    while ((node = traversal.next()) != (Node*)0) {
        if (node->isShape()) {
            Shape* shape = (Shape*)node;
            if (shape->hasGeometryIndexedLineSet())
                shape->setShow(true);
        }
    }

}

void SceneGraphProcessor::shapeIndexedLineSetHide() {

    // TODO

    // using a SceneGraphTraversal look for all Shape nodes containing
    // an IndexedLineSet as geometry, and set the show variable to true

    SceneGraphTraversal traversal(_wrl);
    traversal.start();
    Node* node;
    while ((node = traversal.next()) != (Node*)0) {
        if (node->isShape()) {
            Shape* shape = (Shape*)node;
            if (shape->hasGeometryIndexedLineSet())
                shape->setShow(false);
        }
    }

}

bool SceneGraphProcessor::hasBBox() {
    return _wrl.getChild("BOUNDING-BOX") != (Node*)0;
}

// a Property is define in Node.h as a function with a single Node&
// argument which returns a boolean value

bool SceneGraphProcessor::_hasShapeProperty(Shape::Property p) {
    bool value = false;

    // using a SceneGraphTraversal look for Shape nodes; return true if
    // the Property p is true for at least one Shape node, and false
    // otherwise
    SceneGraphTraversal traversal(_wrl);
    traversal.start();
    Node* node;
    while ((node = traversal.next()) != (Node*)0) {
        if (node->isShape()) {
            Shape& shape = *((Shape*)node);
            if (p(shape)) value = true;    //  ?????????
        }
    }

    return value;
}

bool SceneGraphProcessor::_hasIndexedFaceSetProperty(IndexedFaceSet::Property p) {
    bool value = false;

    // using a SceneGraphTraversal look for Shape nodes containing and
    // IndexedFaceSet nodes as geometry; return true if the Property p
    // is true for at least one of these IndexedFaceSets node, and false
    // otherwise

    SceneGraphTraversal traversal(_wrl);
    traversal.start();
    Node* node;
    while ((node = traversal.next()) != (Node*)0) {
        if (node->isShape()) {
            Shape* shape = (Shape*)node;
            node = shape->getGeometry();
            if (node != (Node*)0 && node->isIndexedFaceSet()) {
                IndexedFaceSet& ifs = *((IndexedFaceSet*)node);
                if (p(ifs)) value = true;
            }
        }
    }

    return value;
}

bool SceneGraphProcessor::_hasIndexedLineSetProperty(IndexedLineSet::Property p) {
    bool value = false;

    // using a SceneGraphTraversal look for Shape nodes containing and
    // IndexedFaceSet nodes as geometry; return true if the Property p
    // is true for at least one of these IndexedFaceSets node, and false
    // otherwise

    SceneGraphTraversal traversal(_wrl);
    traversal.start();
    Node* node;
    while ((node = traversal.next()) != (Node*)0) {
        if (node->isShape()) {
            Shape* shape = (Shape*)node;
            node = shape->getGeometry();
            if (node != (Node*)0 && node->isIndexedFaceSet()) {
                IndexedLineSet& ifs = *((IndexedLineSet*)node);
                if (p(ifs)) value = true;
            }
        }
    }

    return value;
}

// this is a Property
bool SceneGraphProcessor::_hasFaces(IndexedFaceSet& ifs) {
    bool value = false;

    // TODO
    if (ifs.getCoordIndex().size() != 0) value = true;

    return value;
}

// this is a Property
bool SceneGraphProcessor::_hasNormalNone(IndexedFaceSet& ifs) {
    bool value = false;

    // TODO
    if (ifs.getNormal().size() == 0) value = true;

    return value;
}

// this is a Property
bool SceneGraphProcessor::_hasNormalPerFace(IndexedFaceSet& ifs) {
    bool value = false;

    // TODO
    if (ifs.getNormalPerVertex() == false) value = true;

    return value;
}

// this is a Property
bool SceneGraphProcessor::_hasNormalPerVertex(IndexedFaceSet& ifs) {
    bool value = false;

    // TODO
    if (ifs.getNormalPerVertex()) value = true;

    return value;
}

// this is a IndexedFaceSet::Property
bool SceneGraphProcessor::_hasNormalPerCorner(IndexedFaceSet& ifs) {
    bool value = false;

    return value;
}

bool SceneGraphProcessor::hasIndexedFaceSetFaces() {
    return _hasIndexedFaceSetProperty(_hasFaces);
}

bool SceneGraphProcessor::hasIndexedFaceSetNormalNone() {
    return _hasIndexedFaceSetProperty(_hasNormalNone);
}

bool SceneGraphProcessor::hasIndexedFaceSetNormalPerFace() {
    return _hasIndexedFaceSetProperty(_hasNormalPerFace);
}

bool SceneGraphProcessor::hasIndexedFaceSetNormalPerVertex() {
    return _hasIndexedFaceSetProperty(_hasNormalPerVertex);
}

bool SceneGraphProcessor::hasIndexedFaceSetNormalPerCorner() {
    return _hasIndexedFaceSetProperty(_hasNormalPerCorner);
}

// VRML'97
//
// If the color field is not NULL, it shall contain a Color node, and
// the colours are applied to the line(s) as follows:
//
// If colorPerVertex is FALSE:
//
// If the colorIndex field is not empty, then one colour is used for
// each polyline of the IndexedLineSet. There must be at least as many
// indices in the colorIndex field as there are polylines in the
// IndexedLineSet. If the greatest index in the colorIndex field is N,
// then there must be N+1 colours in the Color node. The colorIndex
// field must not contain any negative entries.
//
// If the colorIndex field is empty, then the colours from the Color
// node are applied to each polyline of the IndexedLineSet in
// order. There must be at least as many colours in the Color node as
// there are polylines.
//
// If colorPerVertex is TRUE:
//
// If the colorIndex field is not empty, then colours are applied to
// each vertex of the IndexedLineSet in exactly the same manner that
// the coordIndex field is used to supply coordinates for each vertex
// from the Coordinate node. The colorIndex field must contain at
// least as many indices as the coordIndex field and must contain
// end-of-polyline markers (-1) in exactly the same places as the
// coordIndex field. If the greatest index in the colorIndex field is
// N, then there must be N+1 colours in the Color node.
//
// If the colorIndex field is empty, then the coordIndex field is used
// to choose colours from the Color node. If the greatest index in the
// coordIndex field is N, then there must be N+1 colours in the Color
// node.
//
// If the color field is NULL and there is a Material defined for the
// Appearance affecting this IndexedLineSet, the emissiveColor of the
// Material shall be used to draw the lines.

// this is a Property
bool SceneGraphProcessor::_hasColorNone(IndexedLineSet& ils) {
    bool value = false;

    // TODO 
    if (ils.getNumberOfColor() == 0) value = true;

    return value;
}

// this is a Property
bool SceneGraphProcessor::_hasColorPerVertex(IndexedLineSet& ils) {
    bool value = false;

    // TODO 
    value = ils.getColorPerVertex();

    return value;
}

// this is a Property
bool SceneGraphProcessor::_hasColorPerPolyline(IndexedLineSet& ils) {
    bool value = false;

    // TODO 
    if (ils.getNumberOfColor() > 0 && ils.getNumberOfPolylines() > 0)
        value = true;

    return value;
}

bool SceneGraphProcessor::hasIndexedLineSetColorNone() {
    return _hasIndexedLineSetProperty(_hasColorNone);
}

bool SceneGraphProcessor::hasIndexedLineSetColorPerVertex() {
    return _hasIndexedLineSetProperty(_hasColorPerVertex);
}

bool SceneGraphProcessor::hasIndexedLineSetColorPerPolyline() {
    return _hasIndexedLineSetProperty(_hasColorPerPolyline);
}

// this is a Property
bool SceneGraphProcessor::_hasEdges(Shape& shape) {
    bool value = false;

    // TODO 
    if (shape.getName() == "EDGES")
        value = true;

    return value;
}

// this is a Property
bool SceneGraphProcessor::_hasIndexedFaceSetShown(Shape& shape) {
    bool value = false;

    // TODO 
    value = shape.getShow();

    return value;
}

// this is a Property
bool SceneGraphProcessor::_hasIndexedFaceSetHidden(Shape& shape) {
    bool value = false;

    // TODO 
    value = !shape.getShow();

    return !value;
}

// this is a Property
bool SceneGraphProcessor::_hasIndexedLineSetShown(Shape& shape) {
    bool value = false;

    // TODO 
    value = shape.getShow();

    return value;
}

// this is a Property
bool SceneGraphProcessor::_hasIndexedLineSetHidden(Shape& shape) {
    bool value = false;

    // TODO 
    value = !shape.getShow();

    return value;
}

bool SceneGraphProcessor::hasEdges() {
    return _hasShapeProperty(_hasEdges);
}

bool SceneGraphProcessor::hasIndexedFaceSetShown() {
    return _hasShapeProperty(_hasIndexedFaceSetShown);
}

bool SceneGraphProcessor::hasIndexedFaceSetHidden() {
    return _hasShapeProperty(_hasIndexedFaceSetHidden);
}

bool SceneGraphProcessor::hasIndexedLineSetShown() {
    return _hasShapeProperty(_hasIndexedLineSetShown);
}

bool SceneGraphProcessor::hasIndexedLineSetHidden() {
    return _hasShapeProperty(_hasIndexedLineSetHidden);
}

void SceneGraphProcessor::removeSceneGraphChild(const string& name) {

    // TODO
    int n = _wrl.getNumberOfChildren();
    for (int i = 0; i < n; i++) {
        if (_wrl.getChildren()[i]->getName() == name) {
            _wrl.getChildren().erase(_wrl.getChildren().begin() + i);
        }
    }
}

void SceneGraphProcessor::pointsRemove() {
    removeSceneGraphChild("POINTS");
}

void SceneGraphProcessor::surfaceRemove() {
    removeSceneGraphChild("SURFACE");
}

IndexedFaceSet* SceneGraphProcessor::_getNamedShapeIFS
(const string& name, bool create) {

    IndexedFaceSet* ifs = (IndexedFaceSet*)0;

    // TODO

    // 1) look for a child of the SceneGraph node with the given name
    int n = _wrl.getNumberOfChildren();
    for (int i = 0; i < n; i++) {
        // 2) if found, and the geometry field contains an IndexedFaceSet
        // node, return the pointer to the IndexedFaceSet node.
        // ??????????????????????????????????????????????????????????????????
        if (_wrl.getChildren()[i]->getName() == name) {
            Shape* shape = (Shape*)_wrl.getChildren()[i];
            if (shape->hasGeometryIndexedFaceSet()) {
                IndexedFaceSet* ifs = (IndexedFaceSet*)shape->getGeometry();
                return ifs;
            }
        }
        // 3) otherwise, if create==true, create a new Shape node, make it a
        // child of the SceneGraph, add an Appearance node as appearance,
        // add an IndexedFaceSet as geometry, and return the pointer to the
        // IndexedFaceSet node.

        // 4) make sure tha the Appearance node has a Material node in the
        // material field with a properly defined diffuseColor field, before
        // you return
        if (create) {
            Shape* shape = new Shape();
            _wrl.addChild(shape);
            Appearance* appearance = new Appearance();
            IndexedFaceSet* ifs = new IndexedFaceSet();
            Material* material = new Material();
            material->setDiffuseColor(Color(0.1, 0.1, 0.1));
            material->setTransparency(0.0);
            appearance->setMaterial(material);
            shape->setGeometry(ifs);
            shape->setAppearance(appearance);
            return ifs;
        }
    }

    return ifs;
}

//////////////////////////////////////////////////////////////////////
void eigenFit(const vector<float>& coordPoints,
    const Vec3f& min, const Vec3f& max, Vec4f& f) {

    char str[256];
    cerr << "eigenFit() {" << endl;

    int nPoints = (int)((coordPoints.size()) / 3);
    float x0 = min.x, x1 = max.x, dx = x1 - x0;
    float y0 = min.y, y1 = max.y, dy = y1 - y0;
    float z0 = min.z, z1 = max.z, dz = z1 - z0;
    float dMax = dx; if (dy > dMax) dMax = dy; if (dz > dMax) dMax = dz;

    // compute the mean of the points contained in the 
    double x, y, z;
    double xMean = 0.0f;
    double yMean = 0.0f;
    double zMean = 0.0f;
    int   nMean = 0;
    for (int iP = 0; iP < nPoints; iP++) {
        x = (double)(coordPoints[3 * iP]);
        y = (double)(coordPoints[3 * iP + 1]);
        z = (double)(coordPoints[3 * iP + 2]);
        if (x0 <= x && x <= x1 && y0 <= y && y <= y1 && z0 <= z && z <= z1) {
            xMean += x;
            yMean += y;
            zMean += z;
            nMean++;
        }
    }

    cerr << "  nMean = " << nMean << endl;
    if (nMean == 0) {
        // throw exception ??
        return;
    }

    xMean /= ((double)nMean);
    yMean /= ((double)nMean);
    zMean /= ((double)nMean);

    cerr << "  pMean = [" << endl;
    sprintf(str, "    %12.6f", xMean); cerr << str << endl;
    sprintf(str, "    %12.6f", yMean); cerr << str << endl;
    sprintf(str, "    %12.6f", zMean); cerr << str << endl;
    cerr << "  ]" << endl;

    // compute the scatter matrix
    double dxp, dyp, dzp;
    double M00 = 0.0, M01 = 0.0, M02 = 0.0, M11 = 0.0, M12 = 0.0, M22 = 0.0;
    for (int iP = 0; iP < nPoints; iP++) {
        x = (double)(coordPoints[3 * iP]);
        y = (double)(coordPoints[3 * iP + 1]);
        z = (double)(coordPoints[3 * iP + 2]);
        if (x0 <= x && x <= x1 && y0 <= y && y <= y1 && z0 <= z && z <= z1) {
            dxp = (x - xMean) / dMax;
            dyp = (y - yMean) / dMax;
            dzp = (z - zMean) / dMax;
            M00 += dxp * dxp; M01 += dxp * dyp; M02 += dxp * dzp;
            M11 += dyp * dyp; M12 += dyp * dzp;
            M22 += dzp * dzp;
        }
    }

    double dMean = (double)nMean;
    M00 /= dMean; M01 /= dMean; M02 /= dMean;
    M11 /= dMean; M12 /= dMean;
    M22 /= dMean;
    Matrix3d M;
    M << M00, M01, M02, M01, M11, M12, M02, M12, M22;

    // double dMean = (double)nMean;
    // M(0,0) /= dMean; M(0,1) /= dMean; M(0,2) /= dMean;
    // M(1,0) /= dMean; M(1,1) /= dMean; M(1,2) /= dMean;
    // M(2,0) /= dMean; M(2,1) /= dMean; M(2,2) /= dMean;

    cerr << "  M = [" << endl;
    sprintf(str, "    %12.6f %12.6f %12.6f", M(0, 0), M(0, 1), M(0, 2)); cerr << str << endl;
    sprintf(str, "    %12.6f %12.6f %12.6f", M(1, 0), M(1, 1), M(1, 2)); cerr << str << endl;
    sprintf(str, "    %12.6f %12.6f %12.6f", M(2, 0), M(2, 1), M(2, 2)); cerr << str << endl;
    cerr << "  ]" << endl;

    SelfAdjointEigenSolver<Matrix3d> eigensolver(M);

    if (eigensolver.info() != Success) {
        // trow exception ?
        // abort();
        return;
    }

    Vector3d L(eigensolver.eigenvalues());
    Matrix3d E(eigensolver.eigenvectors());

    cerr << "  eigenvalues(M)" << endl;
    cerr << "  L = [" << endl;
    sprintf(str, "    %12.6f", L(0)); cerr << str << endl;
    sprintf(str, "    %12.6f", L(1)); cerr << str << endl;
    sprintf(str, "    %12.6f", L(2)); cerr << str << endl;
    cerr << "  ]" << endl;

    cerr << "  eigenvectors(M)" << endl;
    cerr << "  E = [" << endl;
    sprintf(str, "    %12.6f %12.6f %12.6f", E(0, 0), E(0, 1), E(0, 2)); cerr << str << endl;
    sprintf(str, "    %12.6f %12.6f %12.6f", E(1, 0), E(1, 1), E(1, 2)); cerr << str << endl;
    sprintf(str, "    %12.6f %12.6f %12.6f", E(2, 0), E(2, 1), E(2, 2)); cerr << str << endl;
    cerr << "  ]" << endl;

    // L(0)                   minimum eigenvalue
    // (E(0,0),E(1,0),E(2,0)) minimum eigenvector

    f.x = (float)(E(0, 0));
    f.y = (float)(E(1, 0));
    f.z = (float)(E(2, 0));
    f.w = -(float)(E(0, 0) * xMean + E(1, 0) * yMean + E(2, 0) * zMean);

    cerr << "  f = [" << endl;
    sprintf(str, "    %12.6f", f.x);  cerr << str << endl;
    sprintf(str, "    %12.6f", f.y);  cerr << str << endl;
    sprintf(str, "    %12.6f", f.z);  cerr << str << endl;
    sprintf(str, "    %12.6f", f.w);  cerr << str << endl;
    cerr << "  ]" << endl;

    cerr << "}" << endl;
}

//////////////////////////////////////////////////////////////////////
void meanFit(const vector<float>& coordPoints,
    const vector<float>& normalPoints,
    const Vec3f& min, const Vec3f& max, Vec4f& f) {

    int nPoints = (int)((coordPoints.size()) / 3);
    float x0 = min.x, x1 = max.x, dx = x1 - x0;
    float y0 = min.y, y1 = max.y, dy = y1 - y0;
    float z0 = min.z, z1 = max.z, dz = z1 - z0;
    float dMax = dx; if (dy > dMax) dMax = dy; if (dz > dMax) dMax = dz;

    // compute the mean of the points contained in the 
    double x, y, z, nn;
    double xMean = 0.0f;
    double yMean = 0.0f;
    double zMean = 0.0f;
    double nxMean = 0.0f;
    double nyMean = 0.0f;
    double nzMean = 0.0f;
    int    nMean = 0;
    for (int iP = 0; iP < nPoints; iP++) {
        x = (double)(coordPoints[3 * iP]);
        y = (double)(coordPoints[3 * iP + 1]);
        z = (double)(coordPoints[3 * iP + 2]);
        if (x0 <= x && x <= x1 && y0 <= y && y <= y1 && z0 <= z && z <= z1) {
            xMean += x;
            yMean += y;
            zMean += z;
            x = (double)(normalPoints[3 * iP]);
            y = (double)(normalPoints[3 * iP + 1]);
            z = (double)(normalPoints[3 * iP + 2]);
            nxMean += x;
            nyMean += y;
            nzMean += z;
            nMean++;
        }
    }

    if (nMean == 0) {
        // throw exception ??
        return;
    }

    // normalize the point mean
    xMean /= ((double)nMean);
    yMean /= ((double)nMean);
    zMean /= ((double)nMean);
    // normalize the normal mean to unit length
    nn = nxMean * nxMean + nyMean * nyMean + nzMean * nzMean;
    if (nn > 0.0) {
        nn = sqrt(nn); nxMean /= nn; nyMean /= nn; nzMean /= nn;
    }
    // set the linear function coefficients
    f.x = (float)(nxMean);
    f.y = (float)(nyMean);
    f.z = (float)(nzMean);
    f.w = -(float)(nxMean * xMean + nyMean * yMean + nzMean * zMean);
}

//////////////////////////////////////////////////////////////////////
void SceneGraphProcessor::fitSinglePlane
(const Vec3f& center, const Vec3f& size,
    const float scale, const bool cube, Vec4f& f) {

    cerr << "SceneGraphProcessor::fitSinglePlane() {" << endl;

    //#if IMPLEMENTED /* define or remove */

      // 1) make sure that the bounding box is not empty; if empty throw a
      // StrExeption with a proper message

    if (_wrl.hasEmptyBBox()) {
        throw new StrException("BBOX is EMPTY!");
        //return;
    }

    // 2) find or create the input POINTS set in the scene graph; input
    // points must have coordinates and normal vector; otherwise throw
    // an StrException

    int n = _wrl.getNumberOfChildren();
    bool find = false;
    Shape* points = new Shape();
    for (int i = 0; i < n; i++) {
        if (_wrl.getChildren()[i]->getName() == "POINTS") {
            points = (Shape*)_wrl.getChildren()[i];
            find = true;
            if ((!points->hasGeometryIndexedFaceSet()) ||
                ((IndexedFaceSet*)points->getGeometry())->getNumberOfNormal() == 0) {
                throw new StrException("POINTS has NO coordinates or normal");
            }
        }
    }
    if (find == false) {
        points->setName("POINTS");
        _wrl.addChild(points);
        IndexedFaceSet* ifs = new IndexedFaceSet();
        points->setGeometry(ifs);
    }

    // get from the POINTS IndexedFaceSet
    vector<float>& coordPoints = ((IndexedFaceSet*)points->getGeometry())->getCoord();
    vector<float>& normalPoints = ((IndexedFaceSet*)points->getGeometry())->getNormal();

    // 3) compute the coordinates of the SceneGraph bounding box corners

    float x0, x1, y0, y1, z0, z1;
    if (cube) {
        float maxTemp = max(max(size.x, size.y), size.z);
        x0 = (center).x - (maxTemp / 2) * scale;
        x1 = (center).x + (maxTemp / 2) * scale;
        y0 = (center).y - (maxTemp / 2) * scale;
        y1 = (center).y + (maxTemp / 2) * scale;
        z0 = (center).z - (maxTemp / 2) * scale;
        z1 = (center).z + (maxTemp / 2) * scale;
    } else {
        x0 = center.x - (size.x / 2) * scale;
        x1 = center.x + (size.x / 2) * scale;
        y0 = center.y - (size.y / 2) * scale;
        y1 = center.y + (size.y / 2) * scale;
        z0 = center.z - (size.z / 2) * scale;
        z1 = center.z + (size.z / 2) * scale;
    }
    Vec3f min(x0, y0, z0);
    Vec3f max(x1, y1, z1);
    // compute the eight bbox corner coordinates
    Vec3f v[8];
    v[0].x = x0; v[1].x = x0; v[2].x = x0; v[3].x = x0;
    v[4].x = x1; v[5].x = x1; v[6].x = x1; v[7].x = x1;
    v[0].y = y0; v[1].y = y0; v[2].y = y1; v[3].y = y1;
    v[4].y = y0; v[5].y = y0; v[6].y = y1; v[7].y = y1;
    v[0].z = z0; v[1].z = z1; v[2].z = z0; v[3].z = z1;
    v[4].z = z0; v[5].z = z1; v[6].z = z0; v[7].z = z1;

    // 4) fit a plane to the points contained in the bounding box
    eigenFit(coordPoints, min, max, f);
    meanFit(coordPoints, normalPoints, min, max, f);

    // 5) find or create the output SURFACE in the scene graph 
    n = _wrl.getNumberOfChildren();
    Shape* surface = new Shape();
    find = false;
    for (int i = 0; i < n; i++) {
        if (_wrl.getChildren()[i]->getName() == "SURFACE") {
            surface = (Shape*)_wrl.getChildren()[i];
            find = true;
            break;
        }
    }
    if (find == false) {
        surface->setName("SURFACE");
        _wrl.addChild(surface);
    }

    IndexedFaceSet* plane = new IndexedFaceSet();
    surface->setGeometry(plane);
    plane->clear();

    // get from the SURFACE IndexedFaceSet
    vector<float>& coordIfs = plane->getCoord();
    vector<int>& coordIndexIfs = plane->getCoordIndex();

    // 6) evaluate the linear function at bounding box corners
    float F[8]; // function values at bbox corners
    bool  b[8]; // function is positive or negative ?
    for (int i = 0; i < 8; i++) {
        // set (x,y,z) the the i=th corner coordinates computed in step 3) above
        // b[i] = (F[i] = x*f.x+y*f.y+z*f.z+f.w)<0.0f;
        float x = v[i].x; float y = v[i].y; float z = v[i].z;
        F[i] = x * f.x + y * f.y + z * f.z + f.w;
        b[i] = F[i] < 0.0f;
    }

    cerr << "//                6 ----- 7 = (x1,y1,z1)" << endl;
    cerr << "//               /|      /|             " << endl;
    cerr << "//              4 ----- 5 |             " << endl;
    cerr << "//              | |     | |             " << endl;
    cerr << "//              | 2 ----| 3             " << endl;
    cerr << "//              |/      |/              " << endl;
    cerr << "// (x0,y0,z0) = 0 ----- 1               " << endl;

    cerr << "  b = [" << endl;
    cerr << "    " << b[0] << endl;
    cerr << "    " << b[1] << endl;
    cerr << "    " << b[2] << endl;
    cerr << "    " << b[3] << endl;
    cerr << "    " << b[4] << endl;
    cerr << "    " << b[5] << endl;
    cerr << "    " << b[6] << endl;
    cerr << "    " << b[7] << endl;
    cerr << "  ]" << endl;

    //////////////////////////////////////////////////////////////////////
    //
    //    vertices      //    edges                 //    faces
    //      6-----7     //        [6]---11---[7]    //        1
    //     /|    /|     //        /|         /|     //        | 3
    //    4-----5 |     //       6 2        7 3     //        |/
    //    | 2---|-3     //      /  |       /  |     //    4---+---5
    //    |/    |/      //    [4]---10---[5]  |     //       /|
    //    0-----1       //     |   |      |   |     //      2 |
    //                  //     |  [2]--9--|--[3]    //        0
    //                  //     0  /       1  /      //
    //                  //     | 4        | 5       //
    //                  //     |/         |/        //
    //                  //    [0]---8----[1]        //
    //

    const int(*edge)[2] = IsoSurf::getEdgeTable();

    // 7) compute the isovertex coordinates;
    // can you explain what is going on here?

    float tj, tk;
    int   iE[12], iV, i, j, k;
    for (i = 0; i < 12; i++) {
        // set the isovertex index to 'no vertex' (-1)
        iV = -1;
        j = edge[i][0];
        k = edge[i][1];
        if (b[j] != b[k]) {
            // isvertex index
            iV = (int)((coordIfs.size() / 3));
            // isovertex coordinates
            tk = F[j] / (F[j] - F[k]);
            tj = F[k] / (F[k] - F[j]);
            float x = tj * v[j].x + tk * v[k].x;
            float y = tj * v[j].y + tk * v[k].y;
            float z = tj * v[j].z + tk * v[k].z;
            coordIfs.push_back(x);
            coordIfs.push_back(y);
            coordIfs.push_back(z);
        }
        iE[i] = iV;
    }
    cerr << "  edge to isovertex table" << endl;
    cerr << "  iE = [" << endl;
    //for (i = 0; i < 12; i++) {
    //	sprintf(str, "    %2d (%2d, %2d) -> %3d", i,
    //		edge[i][0], edge[i][1], iE[i]);
    //	cerr << str << endl;
    //}
    cerr << "  ]" << endl;

    // 8) create isosurface faces
    //vector<int>& coordIndex = plane->getCoordIndex(); // coordIndex.size()==0
    int nFaces = IsoSurf::makeCellFaces(b, iE, coordIndexIfs);
    cerr << "  nFaces = " << nFaces << endl;

    // 9) save face normal
    plane->setNormalPerVertex(false);
    vector<float>& normal = plane->getNormal();
    // normal.size()==0 here
    normal.push_back(f.x);
    normal.push_back(f.y);
    normal.push_back(f.z);

    //#endif // IMPLEMENTED

    cerr << "}" << endl;
}

//////////////////////////////////////////////////////////////////////
void SceneGraphProcessor::fitMultiplePlanes
(const Vec3f& center, const Vec3f& size,
    const int depth, const float scale, const bool cube,
    vector<float>& f) {

    // TODO

    // 1) find or create the POINTS Shape node; extract coordinates and normal
    // vectors from the IndexedFaceSet stored in the geometry field
    //
    // vector<float>& coordPoints = points->getCoord();
    // vector<float>& normalPoints = points->getNormal();
    int n = _wrl.getNumberOfChildren();
    Shape* points = new Shape();
    bool find = false;
    for (int i = 0; i < n; i++) {
        if (_wrl.getChildren()[i]->getName() == "POINTS") {
            points = (Shape*)(_wrl.getChildren()[i]);
            find = true;
        }
    }
    if (find == false) {
        points->setName("POINTS");
        _wrl.addChild(points);
    }
    vector<float>& coordPoints = ((IndexedFaceSet*)points->getGeometry())->getCoord();
    vector<float>& normalPoints = ((IndexedFaceSet*)points->getGeometry())->getNormal();

    // 2) find or create the SURFACE Shape node, and clear the
    // IndexedFaceSet stored in the geometry field

    Shape* surface = new Shape();
    find = false;
    for (int i = 0; i < n; i++) {
        if (_wrl.getChildren()[i]->getName() == "SURFACE") {
            surface = (Shape*)(_wrl.getChildren()[i]);
            find = true;
        }
    }
    if (find == false) {
        surface->setName("SURFACE");
        _wrl.addChild(surface);
    }
    IndexedFaceSet* ifs = new IndexedFaceSet();
    surface->setGeometry(ifs);
    ifs->clear();

    // 3) the argument vector<float>&f will contain one scalar values;
    // clear it here
    f.clear();

    // 4) if the bounding box is empty (size.x==size.y==size.z==0) or scale<=0
    // throw an StrException 

    if (size.x <= 0 || size.y <= 0 ||
        size.z <= 0 || scale <= 0)
        throw new StrException("BOUNDING-BOX is EMPTY!");

    // 5) compute the coordinates of the 8 vertices of the scaled bounding box
    float x0, x1, y0, y1, z0, z1;
    if (cube) {
        float maxTemp = max(max(size.x, size.y), size.z);
        x0 = (center).x - (maxTemp / 2) * scale;
        x1 = (center).x + (maxTemp / 2) * scale;
        y0 = (center).y - (maxTemp / 2) * scale;
        y1 = (center).y + (maxTemp / 2) * scale;
        z0 = (center).z - (maxTemp / 2) * scale;
        z1 = (center).z + (maxTemp / 2) * scale;
    } else {
        x0 = center.x - (size.x / 2) * scale;
        x1 = center.x + (size.x / 2) * scale;
        y0 = center.y - (size.y / 2) * scale;
        y1 = center.y + (size.y / 2) * scale;
        z0 = center.z - (size.z / 2) * scale;
        z1 = center.z + (size.z / 2) * scale;
    }
    Vec3f min(x0, y0, z0);
    Vec3f max(x1, y1, z1);

    // 6) create a partition of the points as an array of linked lists

    _createPartition(min, max, depth, coordPoints);

    // 7) for each occupied cell, fit a plane, and compute the
    // isosurface polygon within the corresponding cube; accumulate
    // polygons as vertices faces, and face normals of the SURFACE
    // IndexedFaceSet

    Appearance* appearance = new Appearance();
    Material* material = new Material();
    material->setAmbientIntensity(0.0f);
    material->setDiffuseColor(Color(.2, .5, .2));
    appearance->setMaterial(material);
    surface->setAppearance(appearance);
    vector<float>& coordIfs = ifs->getCoord();
    vector<int>& coordIndexIfs = ifs->getCoordIndex();
    vector<float> coordTemp, normalTemp;

    for (int i = 0; i < _nGrid; i++) {
        for (int j = 0; j < _nGrid; j++) {
            for (int k = 0; k < _nGrid; k++) {
                int iCell = k + _nGrid * (j + _nGrid * i);
                cerr << "iCell = " << iCell << endl;
                coordTemp.clear();
                normalTemp.clear();
                if (_first[iCell] < 0) continue;

                Vec3f minCell, maxCell;
                minCell.x = (((float)_nGrid - k) * min.x + (k)*max.x) / (_nGrid);
                maxCell.x = (((float)_nGrid - k - 1) * min.x + (k + 1) * max.x) / (_nGrid);
                minCell.y = (((float)_nGrid - j) * min.y + (j)*max.y) / (_nGrid);
                maxCell.y = (((float)_nGrid - j - 1) * min.y + (j + 1) * max.y) / (_nGrid);
                minCell.z = (((float)_nGrid - i) * min.z + (i)*max.z) / (_nGrid);
                maxCell.z = (((float)_nGrid - i - 1) * min.z + (i + 1) * max.z) / (_nGrid);

                Vec3f v[8];
                v[0].x = minCell.x; v[1].x = minCell.x; v[2].x = minCell.x; v[3].x = minCell.x;
                v[4].x = maxCell.x; v[5].x = maxCell.x; v[6].x = maxCell.x; v[7].x = maxCell.x;
                v[0].y = minCell.y; v[1].y = minCell.y; v[2].y = maxCell.y; v[3].y = maxCell.y;
                v[4].y = minCell.y; v[5].y = minCell.y; v[6].y = maxCell.y; v[7].y = maxCell.y;
                v[0].z = minCell.z; v[1].z = maxCell.z; v[2].z = minCell.z; v[3].z = maxCell.z;
                v[4].z = minCell.z; v[5].z = maxCell.z; v[6].z = minCell.z; v[7].z = maxCell.z;

                // traverse of the cell
                for (int iP = _first[iCell]; iP >= 0; iP = _next[iP]) {
                    coordTemp.push_back(coordPoints[3 * iP]);
                    coordTemp.push_back(coordPoints[3 * iP + 1]);
                    coordTemp.push_back(coordPoints[3 * iP + 2]);
                    normalTemp.push_back(normalPoints[3 * iP]);
                    normalTemp.push_back(normalPoints[3 * iP + 1]);
                    normalTemp.push_back(normalPoints[3 * iP + 2]);
                }

                Vec4f fTemp;
                eigenFit(coordPoints, minCell, maxCell, fTemp);
                meanFit(coordPoints, normalPoints, minCell, maxCell, fTemp);
                // not sure what `f` does
                f.push_back(fTemp.x);
                f.push_back(fTemp.y);
                f.push_back(fTemp.z);
                f.push_back(fTemp.w);

                float F[8]; // function values at bbox corners
                bool  b[8]; // function is positive or negative ?
                for (int c = 0; c < 8; c++) {
                    // set (x,y,z) the the i=th corner coordinates computed in step 3) above
                    // b[i] = (F[i] = x*f.x+y*f.y+z*f.z+f.w)<0.0f;
                    float x = v[c].x; float y = v[c].y; float z = v[c].z;
                    F[c] = x * fTemp.x + y * fTemp.y + z * fTemp.z + fTemp.w;
                    b[c] = F[c] < 0.0f;
                }

                const int(*edge)[2] = IsoSurf::getEdgeTable();
                float tj, tk;
                int   iE[12], iV, q, w, e;
                for (q = 0; q < 12; q++) {
                    // set the isovertex index to 'no vertex' (-1)
                    iV = -1;
                    w = edge[q][0];
                    e = edge[q][1];
                    if (b[w] != b[e]) {
                        // isvertex index
                        iV = (int)((coordIfs.size() / 3));
                        // isovertex coordinates
                        tk = F[w] / (F[w] - F[e]);
                        tj = F[e] / (F[e] - F[w]);
                        float x = tj * v[w].x + tk * v[e].x;
                        float y = tj * v[w].y + tk * v[e].y;
                        float z = tj * v[w].z + tk * v[e].z;
                        coordIfs.push_back(x);
                        coordIfs.push_back(y);
                        coordIfs.push_back(z);
                    }
                    iE[q] = iV;
                }
                int nFaces = IsoSurf::makeCellFaces(b, iE, coordIndexIfs);
                ifs->setNormalPerVertex(false);
                vector<float>& normal = ifs->getNormal();
                normal.push_back(fTemp.x);
                normal.push_back(fTemp.y);
                normal.push_back(fTemp.z);
            }
        }
    }

    // 8) delete partition
    ifs->setNormalPerVertex(false);
    _deletePartition();
}

//////////////////////////////////////////////////////////////////////
void SceneGraphProcessor::fitContinuous
(const Vec3f& center, const Vec3f& size,
    const int depth, const float scale, const bool cube) {

    int n = _wrl.getNumberOfChildren();
    Shape* points = new Shape();
    bool find = false;
    for (int i = 0; i < n; i++) {
        if (_wrl.getChildren()[i]->getName() == "POINTS") {
            points = (Shape*)(_wrl.getChildren()[i]);
            find = true;
        }
    }
    if (find == false) {
        points->setName("POINTS");
        _wrl.addChild(points);
    }
    vector<float>& coordPoints = ((IndexedFaceSet*)points->getGeometry())->getCoord();
    vector<float>& normalPoints = ((IndexedFaceSet*)points->getGeometry())->getNormal();

    Shape* surface = new Shape();
    find = false;
    for (int i = 0; i < n; i++) {
        if (_wrl.getChildren()[i]->getName() == "SURFACE") {
            surface = (Shape*)(_wrl.getChildren()[i]);
            find = true;
        }
    }
    if (find == false) {
        surface->setName("SURFACE");
        _wrl.addChild(surface);
    }
    IndexedFaceSet* ifs = new IndexedFaceSet();
    surface->setGeometry(ifs);
    ifs->clear();

    if (size.x <= 0 || size.y <= 0 ||
        size.z <= 0 || scale <= 0)
        throw new StrException("BOUNDING-BOX is EMPTY!");

    // 1) same as fitMultiplePlanes, but rather than computing the
    // isosurfaces within the hexahedral cells right away, first save
    // the linear function values at the corners of the cells

    float x0, x1, y0, y1, z0, z1;
    if (cube) {
        float maxTemp = max(max(size.x, size.y), size.z);
        x0 = (center).x - (maxTemp / 2) * scale;
        x1 = (center).x + (maxTemp / 2) * scale;
        y0 = (center).y - (maxTemp / 2) * scale;
        y1 = (center).y + (maxTemp / 2) * scale;
        z0 = (center).z - (maxTemp / 2) * scale;
        z1 = (center).z + (maxTemp / 2) * scale;
    } else {
        x0 = center.x - (size.x / 2) * scale;
        x1 = center.x + (size.x / 2) * scale;
        y0 = center.y - (size.y / 2) * scale;
        y1 = center.y + (size.y / 2) * scale;
        z0 = center.z - (size.z / 2) * scale;
        z1 = center.z + (size.z / 2) * scale;
    }
    Vec3f min(x0, y0, z0);
    Vec3f max(x1, y1, z1);

    _createPartition(min, max, depth, coordPoints);

    Appearance* appearance = new Appearance();
    Material* material = new Material();
    material->setAmbientIntensity(0.0f);
    material->setDiffuseColor(Color(.2, .5, .2));
    appearance->setMaterial(material);
    surface->setAppearance(appearance);
    vector<float>& coordIfs = ifs->getCoord();
    vector<int>& coordIndexIfs = ifs->getCoordIndex();
    vector<float>& normal = ifs->getNormal();

    _nGrid = 1 << depth;
    //int nGridVertices = (_nGrid + 1) * (_nGrid + 1) * (_nGrid + 1);
    float*** fGridVertex;
    fGridVertex = new float** [_nGrid + 1];
    for (int x = 0; x < _nGrid + 1; ++x) {
        fGridVertex[x] = new float* [_nGrid + 1];
        for (int y = 0; y < _nGrid + 1; ++y) {
            fGridVertex[x][y] = new float[_nGrid + 1];
            for (int z = 0; z < _nGrid + 1; ++z) {
                fGridVertex[x][y][z] = 0.0f;
            }
        }
    }
    float*** wGridVertex;
    wGridVertex = new float** [_nGrid + 1];
    for (int x = 0; x < _nGrid + 1; ++x) {
        wGridVertex[x] = new float* [_nGrid + 1];
        for (int y = 0; y < _nGrid + 1; ++y) {
            wGridVertex[x][y] = new float[_nGrid + 1];
            for (int z = 0; z < _nGrid + 1; ++z) {
                wGridVertex[x][y][z] = 0.0f;
            }
        }
    }
    vector<float> coordTemp, normalTemp;
    // for non empty each cell (i,j,k) {
    // fit linear function Vec4f f to the points contained in the cell
    // (make sure that linear function normal vector matches the average point normal)
    //    for each corner of the cell {
    //      determine the coordinates of the corner
    //      evaluate the linear function
    //      determine the grid vertex index iV of the corner
    //      add the evaluated value to fGridVertex[iV]
    //      increment wGridVertex[iV] by one
    //    }
    // }
    // 2) note that empty cells do not have function values associated
    // with
    for (int i = 0; i < _nGrid; i++) {
        for (int j = 0; j < _nGrid; j++) {
            for (int k = 0; k < _nGrid; k++) {
                int iCell = k + _nGrid * (j + _nGrid * i);
                //cerr << "iCell = " << iCell << endl;
                coordTemp.clear();
                normalTemp.clear();
                if (_first[iCell] < 0) continue;

                Vec4f f;
                Vec3f minCell, maxCell;
                minCell.x = ((_nGrid - k) * min.x + (k)*max.x) / (_nGrid);
                maxCell.x = ((_nGrid - k - 1) * min.x + (k + 1) * max.x) / (_nGrid);
                minCell.y = ((_nGrid - j) * min.y + (j)*max.y) / (_nGrid);
                maxCell.y = ((_nGrid - j - 1) * min.y + (j + 1) * max.y) / (_nGrid);
                minCell.z = ((_nGrid - i) * min.z + (i)*max.z) / (_nGrid);
                maxCell.z = ((_nGrid - i - 1) * min.z + (i + 1) * max.z) / (_nGrid);
                Vec3f v[8];
                v[0].x = minCell.x; v[1].x = minCell.x; v[2].x = minCell.x; v[3].x = minCell.x;
                v[4].x = maxCell.x; v[5].x = maxCell.x; v[6].x = maxCell.x; v[7].x = maxCell.x;
                v[0].y = minCell.y; v[1].y = minCell.y; v[2].y = maxCell.y; v[3].y = maxCell.y;
                v[4].y = minCell.y; v[5].y = minCell.y; v[6].y = maxCell.y; v[7].y = maxCell.y;
                v[0].z = minCell.z; v[1].z = maxCell.z; v[2].z = minCell.z; v[3].z = maxCell.z;
                v[4].z = minCell.z; v[5].z = maxCell.z; v[6].z = minCell.z; v[7].z = maxCell.z;

                // traverse of the cell
                for (int iP = _first[iCell]; iP >= 0; iP = _next[iP]) {
                    coordTemp.push_back(coordPoints[3 * iP]);
                    coordTemp.push_back(coordPoints[3 * iP + 1]);
                    coordTemp.push_back(coordPoints[3 * iP + 2]);
                    normalTemp.push_back(normalPoints[3 * iP]);
                    normalTemp.push_back(normalPoints[3 * iP + 1]);
                    normalTemp.push_back(normalPoints[3 * iP + 2]);
                }

                eigenFit(coordTemp, minCell, maxCell, f);
                meanFit(coordTemp, normalTemp, minCell, maxCell, f);
                normal.push_back(f.x);
                normal.push_back(f.y);
                normal.push_back(f.z);

                // evaluate the linear function at bounding box corners
                float F[8]; // function values at bbox corners
                //int map[8] = { 0, 4, 2, 6, 1, 5, 3, 7 };
                for (int c = 0; c < 8; c++) {
                    float x = v[c].x; float y = v[c].y; float z = v[c].z;
                    F[c] = x * f.x + y * f.y + z * f.z + f.w;
                    // determine the grid vertex index iV of the corner
                    // add the evaluated value to fGridVertex[iV]
                    // increment wGridVertex[iV] by one
                    //int iV = iCell + map[c];
                    //fGridVertex[iV] = F[c];
                    //wGridVertex[iV]++;
                }
                fGridVertex[i][j][k    ] += F[0];
                fGridVertex[i][j][k + 1] += F[4];
                fGridVertex[i][j + 1][k] += F[2];
                fGridVertex[i][j + 1][k + 1] += F[6];
                fGridVertex[i + 1][j][k    ] += F[1];
                fGridVertex[i + 1][j][k + 1] += F[5];
                fGridVertex[i + 1][j + 1][k] += F[3];
                fGridVertex[i + 1][j + 1][k + 1] += F[7];

                wGridVertex[i][j][k]++;
                wGridVertex[i][j][k + 1]++;
                wGridVertex[i][j + 1][k]++;
                wGridVertex[i][j + 1][k + 1]++;
                wGridVertex[i + 1][j][k]++;
                wGridVertex[i + 1][j][k + 1]++;
                wGridVertex[i + 1][j + 1][k]++;
                wGridVertex[i + 1][j + 1][k + 1]++;
            }
        }
    }

    // 3) for each grid vertex of an occupied incident cell, compute the
    // average of the values saved on the incident cell corners; and
    // then replace the incident cell corner values by the average.

    // normalize function values
    for (int i = 0; i <= _nGrid; i++) {
        for (int j = 0; j <= _nGrid; j++) {
            for (int k = 0; k <= _nGrid; k++) {
                if (wGridVertex[i][j][k] > 0.0f) {
                    fGridVertex[i][j][k] /= wGridVertex[i][j][k];
                }
            }
        }
    }

    // 4) now compute the isosurface polygon with the cells, as in the
    // fitMultiplePlanes method, but using the new values instead

    // 5) fill the SURFACE IndexedFaceSet exactly as in the
    // fitMultiplePlanes method

    for (int i = 0; i < _nGrid; i++) {
        for (int j = 0; j < _nGrid; j++) {
            for (int k = 0; k < _nGrid; k++) {
                int iCell = k + _nGrid * (j + _nGrid * i);
                //cerr << "iCell = " << iCell << endl;
                if (_first[iCell] < 0) continue;

                Vec3f minCell, maxCell;
                minCell.x = (((float)_nGrid - k) * min.x + (k)*max.x) / (float)(_nGrid);
                maxCell.x = (((float)_nGrid - k - 1) * min.x + (k + 1) * max.x) / (float)(_nGrid);
                minCell.y = (((float)_nGrid - j) * min.y + (j)*max.y) / (float)(_nGrid);
                maxCell.y = (((float)_nGrid - j - 1) * min.y + (j + 1) * max.y) / (float)(_nGrid);
                minCell.z = (((float)_nGrid - i) * min.z + (i)*max.z) / (float)(_nGrid);
                maxCell.z = (((float)_nGrid - i - 1) * min.z + (i + 1) * max.z) / (float)(_nGrid);

                Vec3f v[8];
                v[0].x = minCell.x; v[1].x = minCell.x; v[2].x = minCell.x; v[3].x = minCell.x;
                v[4].x = maxCell.x; v[5].x = maxCell.x; v[6].x = maxCell.x; v[7].x = maxCell.x;
                v[0].y = minCell.y; v[1].y = minCell.y; v[2].y = maxCell.y; v[3].y = maxCell.y;
                v[4].y = minCell.y; v[5].y = minCell.y; v[6].y = maxCell.y; v[7].y = maxCell.y;
                v[0].z = minCell.z; v[1].z = maxCell.z; v[2].z = minCell.z; v[3].z = maxCell.z;
                v[4].z = minCell.z; v[5].z = maxCell.z; v[6].z = minCell.z; v[7].z = maxCell.z;

                float F[8]; // function values at bbox corners
                bool  b[8]; // function is positive or negative ?
                //int map[8] = { 0, 4, 2, 6, 1, 5, 3, 7 };
                F[0] = fGridVertex[i][j][k];//retrieve F from fGridVertex
                F[4] = fGridVertex[i][j][k + 1];
                F[2] = fGridVertex[i][j + 1][k];
                F[6] = fGridVertex[i][j + 1][k + 1];
                F[1] = fGridVertex[i + 1][j][k];
                F[5] = fGridVertex[i + 1][j][k + 1];
                F[3] = fGridVertex[i + 1][j + 1][k];
                F[7] = fGridVertex[i + 1][j + 1][k + 1];
                for (int c = 0; c < 8; c++) {
                    //int iV = iCell + map[c];
                    //F[c] = fGridVertex[iV];
                    b[c] = F[c] < 0.0f;
                }

                const int(*edge)[2] = IsoSurf::getEdgeTable();
                float tj, tk;
                int   iE[12], iV, q, w, e;
                for (q = 0; q < 12; q++) {
                    // set the isovertex index to 'no vertex' (-1)
                    iV = -1;
                    w = edge[q][0];
                    e = edge[q][1];
                    if (b[w] != b[e]) {
                        // isvertex index
                        iV = (int)((coordIfs.size() / 3));
                        // isovertex coordinates
                        tk = F[w] / (F[w] - F[e]);
                        tj = F[e] / (F[e] - F[w]);
                        float x = tj * v[w].x + tk * v[e].x;
                        float y = tj * v[w].y + tk * v[e].y;
                        float z = tj * v[w].z + tk * v[e].z;
                        coordIfs.push_back(x);
                        coordIfs.push_back(y);
                        coordIfs.push_back(z);
                    }
                    iE[q] = iV;
                }
                int nFaces = IsoSurf::makeCellFaces(b, iE, coordIndexIfs);
                //vector<float>& normal = ifs->getNormal();
                //normal.push_back(f.x);
                //normal.push_back(f.y);
                //normal.push_back(f.z);
            }
        }
    }
    ifs->setNormalPerVertex(false);
    _deletePartition();
    delete[] fGridVertex;
    delete[] wGridVertex;
}
