//------------------------------------------------------------------------
//  Copyright (C) Gabriel Taubin
//  Time-stamp: <2019-11-19 14:13:18 taubin>
//------------------------------------------------------------------------
//
// WrlToolsWidget.cpp
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

#include "WrlToolsWidget.hpp"
#include "WrlMainWindow.hpp"

#include "wrl/SceneGraphProcessor.hpp"

//////////////////////////////////////////////////////////////////////
WrlToolsWidget::WrlToolsWidget(QWidget* parent):
  QWidget(),
  _mainWindow((WrlMainWindow*)0) {
    (void) parent;

  setupUi(this);
}

//////////////////////////////////////////////////////////////////////
WrlToolsWidget::~WrlToolsWidget() {
}

//////////////////////////////////////////////////////////////////////
void WrlToolsWidget::setMainWindow(WrlMainWindow *mw) {
  _mainWindow = mw;
}

//////////////////////////////////////////////////////////////////////
void WrlToolsWidget::updateState() {
  edit3DCanvasWidth->setText
    ("  "+QString::number(_mainWindow->getGLWidgetWidth()));
  edit3DCanvasHeight->setText
    ("  "+QString::number(_mainWindow->getGLWidgetHeight()));

  WrlViewerData& data  = _mainWindow->getData();

  int   bboxDepth = data.getBBoxDepth();
  bool  bboxCube  = data.getBBoxCube();
  float bboxScale = data.getBBoxScale();

  spinBoxBBoxDepth->setValue(bboxDepth);
  checkBoxBBoxCube->setChecked(bboxCube);
  editBBoxScale->setText("  "+QString::number(bboxScale,'f',2));

  Vec4f& plane = data.getPlane();
  editSurfaceSinglePlaneCoefficient0->setText("  "+QString::number(plane.x));
  editSurfaceSinglePlaneCoefficient1->setText("  "+QString::number(plane.y));
  editSurfaceSinglePlaneCoefficient2->setText("  "+QString::number(plane.z));
  editSurfaceSinglePlaneCoefficient3->setText("  "+QString::number(plane.w));
  
  SceneGraph* wrl = data.getSceneGraph();
  if(wrl==(SceneGraph*)0) {

    pushButtonBBoxAdd->setEnabled(false);
    pushButtonBBoxRemove->setEnabled(false);
    pushButtonSceneGraphNormalNone->setEnabled(false);
    pushButtonSceneGraphNormalPerVertex->setEnabled(false);
    pushButtonSceneGraphNormalPerFace->setEnabled(false);
    pushButtonSceneGraphNormalPerCorner->setEnabled(false);
    pushButtonSceneGraphNormalInvert->setEnabled(false);
    pushButtonSceneGraphEdgesAdd->setEnabled(false);
    pushButtonSceneGraphEdgesRemove->setEnabled(false);
    pushButtonSceneGraphIndexedFaceSetsShow->setEnabled(false);
    pushButtonSceneGraphIndexedFaceSetsHide->setEnabled(false);
    pushButtonSceneGraphIndexedLineSetsShow->setEnabled(false);
    pushButtonSceneGraphIndexedLineSetsHide->setEnabled(false);
    pushButtonPointsRemove->setEnabled(false);
    pushButtonPointsShow->setEnabled(false);
    pushButtonPointsHide->setEnabled(false);
    pushButtonSurfaceSinglePlaneFit->setEnabled(false);
    pushButtonSurfaceMultiplePlanesFit->setEnabled(false);
    pushButtonSurfaceContinuousFit->setEnabled(false);
    pushButtonSurfaceRemove->setEnabled(false);
    pushButtonSurfaceShow->setEnabled(false);
    pushButtonSurfaceHide->setEnabled(false);

  } else /* if(wrl!=(SceneGraph*)0) */ {

    SceneGraphProcessor processor(*wrl);

    bool hasBBox   = processor.hasBBox();
    pushButtonBBoxAdd->setEnabled(!hasBBox);
    pushButtonBBoxRemove->setEnabled(hasBBox);

    bool hasEdges  = processor.hasEdges();
    pushButtonSceneGraphEdgesAdd->setEnabled(!hasEdges);
    pushButtonSceneGraphEdgesRemove->setEnabled(hasEdges);

    bool hasNormal = false;
    bool hasFaces  = processor.hasIndexedFaceSetFaces();
    bool value     = processor.hasIndexedFaceSetNormalNone();
    pushButtonSceneGraphNormalNone->setEnabled(!value);
    value = processor.hasIndexedFaceSetNormalPerVertex();
    hasNormal |= value;
    pushButtonSceneGraphNormalPerVertex->setEnabled(hasFaces && !value);
    value = processor.hasIndexedFaceSetNormalPerFace();
    hasNormal |= value;
    pushButtonSceneGraphNormalPerFace->setEnabled(hasFaces && !value);
    value = processor.hasIndexedFaceSetNormalPerCorner();
    hasNormal |= value;
    pushButtonSceneGraphNormalPerCorner->setEnabled(hasFaces && !value);
    pushButtonSceneGraphNormalInvert->setEnabled(hasNormal);

    value = processor.hasIndexedFaceSetShown();
    pushButtonSceneGraphIndexedFaceSetsHide->setEnabled(value);
    value = processor.hasIndexedFaceSetHidden();
    pushButtonSceneGraphIndexedFaceSetsShow->setEnabled(value);

    value = processor.hasIndexedLineSetShown();
    pushButtonSceneGraphIndexedLineSetsHide->setEnabled(value);
    value = processor.hasIndexedLineSetHidden();
    pushButtonSceneGraphIndexedLineSetsShow->setEnabled(value);

    Node* points = wrl->find("POINTS"); // should be a Shape node
    bool  hasPoints = (points!=(Node*)0 && points->isShape());
    if(hasPoints) {
      pushButtonPointsRemove->setEnabled(true);
      bool show = points->getShow();
      pushButtonPointsShow->setEnabled(!show);
      pushButtonPointsHide->setEnabled(show);

      Shape* shape = (Shape*)points;
      bool hasPointNormals = false;
      bool hasPointColors  = false;
      Node* node = shape->getGeometry();
      if(node!=(Node*)0 && node->isIndexedFaceSet()) {
        IndexedFaceSet* ifs = (IndexedFaceSet*)node;
        hasPointNormals = (ifs->getNormalBinding()==IndexedFaceSet::PB_PER_VERTEX);
        hasPointColors  = (ifs->getColorBinding() ==IndexedFaceSet::PB_PER_VERTEX);
      }
      checkBoxPointsHasNormals->setChecked(hasPointNormals);
      checkBoxPointsHasColors->setChecked(hasPointColors);
    } else {
      pushButtonPointsRemove->setEnabled(false);
      pushButtonPointsShow->setEnabled(false);
      pushButtonPointsHide->setEnabled(false);
    }
    pushButtonSurfaceSinglePlaneFit->setEnabled(hasPoints);
    pushButtonSurfaceMultiplePlanesFit->setEnabled(hasPoints);
    pushButtonSurfaceContinuousFit->setEnabled(hasPoints);

    Node* surface    = wrl->find("SURFACE"); // should be a Shape node
    bool  hasSurface = (surface!=(Node*)0 && surface->isShape());
    if(hasSurface) {
      pushButtonSurfaceRemove->setEnabled(true);
      bool show = surface->getShow();
      pushButtonSurfaceShow->setEnabled(!show);;
      pushButtonSurfaceHide->setEnabled(show);;
    } else {
      pushButtonSurfaceRemove->setEnabled(false);
      pushButtonSurfaceShow->setEnabled(false);;
      pushButtonSurfaceHide->setEnabled(false);;
    }
    
    //     // get number of points
    int nPts = 0;
    if(hasPoints) {
      Shape* shape = (Shape*)points;
      Node* node = shape->getGeometry();
      if(node!=0 && node->isIndexedFaceSet()) {
        IndexedFaceSet* ifs = (IndexedFaceSet*)node;
        nPts = ifs->getNumberOfCoord();
      }
    }
    editPointsNumber->setText("  "+QString::number(nPts));

    // get number of surface vertices and faces
    int nVertices = 0;
    int nFaces = 0;
    if(hasSurface) {
      Shape* shape = (Shape*)surface;
      Node* node = shape->getGeometry();
      if(node!=0 && node->isIndexedFaceSet()) {
        IndexedFaceSet* ifs = (IndexedFaceSet*)node;
        nVertices = ifs->getNumberOfCoord();
        nFaces = ifs->getNumberOfFaces();
      }
    }
    editSurfaceVertices->setText("  "+QString::number(nVertices));
    editSurfaceFaces->setText("  "+QString::number(nFaces));

  }
}

void WrlToolsWidget::on_spinBoxBBoxDepth_valueChanged(int depth) {
  WrlViewerData& data = _mainWindow->getData();
  int newDepth = (depth<0)?0:(depth>10)?10:depth;
  int oldDepth = data.getBBoxDepth();
  if(newDepth!=oldDepth) {
    data.setBBoxDepth(depth);
    // is scene graph has a BOUNDING-BOX node => rebuild it
    SceneGraph* pWrl = data.getSceneGraph();
    SceneGraphProcessor processor(*pWrl);
    if(processor.hasBBox()) {
      float scale = data.getBBoxScale();
      bool  cube  = data.getBBoxCube();
      processor.bboxAdd(newDepth,scale,cube);
      _mainWindow->setSceneGraph(pWrl,false);
      _mainWindow->refresh();
      updateState();
    }
  }
}

void WrlToolsWidget::bboxDepthUp() {
  WrlViewerData& data = _mainWindow->getData();
  on_spinBoxBBoxDepth_valueChanged(data.getBBoxDepth()+1);
}

void WrlToolsWidget::bboxDepthDown() {
  WrlViewerData& data = _mainWindow->getData();
  on_spinBoxBBoxDepth_valueChanged(data.getBBoxDepth()-1);
}

void WrlToolsWidget::on_pushButtonBBoxAdd_clicked() {
  WrlViewerData& data = _mainWindow->getData();
  int depth = data.getBBoxDepth();
  SceneGraph* pWrl = data.getSceneGraph();
  SceneGraphProcessor processor(*pWrl);
  data.setBBoxDepth(depth);
  float scale = data.getBBoxScale();
  bool  cube  = data.getBBoxCube();
  processor.bboxAdd(depth,scale,cube);
  _mainWindow->setSceneGraph(pWrl,false);
  _mainWindow->refresh();
  updateState();
}

void WrlToolsWidget::on_pushButtonBBoxRemove_clicked() {
  WrlViewerData& data = _mainWindow->getData();
  SceneGraph* pWrl = data.getSceneGraph();
  if(pWrl!=(SceneGraph*)0) {
    SceneGraphProcessor processor(*pWrl);
    processor.bboxRemove();
    _mainWindow->setSceneGraph(pWrl,false);
    _mainWindow->refresh();
    updateState();
  }
}

void WrlToolsWidget::on_editBBoxScale_returnPressed() {
  WrlViewerData& data = _mainWindow->getData();
  float scale = data.getBBoxScale();
  QString str = editBBoxScale->text();
  float value = str.toFloat();
  if(value!=scale) {
    data.setBBoxScale(value);
    SceneGraphProcessor processor(*(data.getSceneGraph()));
    if(processor.hasBBox()) {
      int   depth = data.getBBoxDepth();
      float scale = data.getBBoxScale();
      bool  cube  = data.getBBoxCube();
      processor.bboxAdd(depth,scale,cube);
      _mainWindow->setSceneGraph(data.getSceneGraph(),false);
      _mainWindow->refresh();
      updateState();
    }
  }
}

void WrlToolsWidget::on_checkBoxBBoxCube_stateChanged(int state) {
  WrlViewerData& data = _mainWindow->getData();
  data.setBBoxCube((state!=0));
  SceneGraphProcessor processor(*(data.getSceneGraph()));
  if(processor.hasBBox()) {
    int   depth = data.getBBoxDepth();
    float scale = data.getBBoxScale();
    bool  cube  = data.getBBoxCube();
    processor.bboxAdd(depth,scale,cube);
    _mainWindow->setSceneGraph(data.getSceneGraph(),false);
    _mainWindow->refresh();
    updateState();
  }
}

void WrlToolsWidget::bboxCube() {
  on_checkBoxBBoxCube_stateChanged((checkBoxBBoxCube->isChecked())?2:0);
}

void WrlToolsWidget::on_pushButtonSceneGraphEdgesAdd_clicked() {
  WrlViewerData& data = _mainWindow->getData();
  SceneGraph*    pWrl = data.getSceneGraph();
  if(pWrl!=(SceneGraph*)0) {
    SceneGraphProcessor processor(*pWrl);
    processor.edgesAdd();
    _mainWindow->setSceneGraph(pWrl,false);
    _mainWindow->refresh();
    updateState();
  }
}

void WrlToolsWidget::on_pushButtonSceneGraphEdgesRemove_clicked() {
  WrlViewerData& data = _mainWindow->getData();
  SceneGraph*    pWrl = data.getSceneGraph();
  if(pWrl!=(SceneGraph*)0) {
    SceneGraphProcessor processor(*pWrl);
    processor.edgesRemove();
    _mainWindow->setSceneGraph(pWrl,false);
    _mainWindow->refresh();
    updateState();
  }
}

void WrlToolsWidget::on_pushButtonSceneGraphNormalInvert_clicked() {
  WrlViewerData& data = _mainWindow->getData();
  SceneGraph*    pWrl = data.getSceneGraph();
  if(pWrl!=(SceneGraph*)0) {
    SceneGraphProcessor processor(*pWrl);
    processor.normalInvert();
    _mainWindow->setSceneGraph(pWrl,false);
    _mainWindow->refresh();
    updateState();
  }
}

void WrlToolsWidget::on_pushButtonSceneGraphNormalNone_clicked() {
  WrlViewerData& data = _mainWindow->getData();
  SceneGraph*    pWrl = data.getSceneGraph();
  if(pWrl!=(SceneGraph*)0) {
    SceneGraphProcessor processor(*pWrl);
    processor.normalClear();
    _mainWindow->setSceneGraph(pWrl,false);
    _mainWindow->refresh();
    updateState();
  }
}

void WrlToolsWidget::on_pushButtonSceneGraphNormalPerVertex_clicked() {
  WrlViewerData& data = _mainWindow->getData();
  SceneGraph*    pWrl = data.getSceneGraph();
  if(pWrl!=(SceneGraph*)0) {
    SceneGraphProcessor processor(*pWrl);
    processor.computeNormalPerVertex();
    _mainWindow->setSceneGraph(pWrl,false);
    _mainWindow->refresh();
    updateState();
  }
}

void WrlToolsWidget::on_pushButtonSceneGraphNormalPerFace_clicked() {
  WrlViewerData& data = _mainWindow->getData();
  SceneGraph*    pWrl = data.getSceneGraph();
  if(pWrl!=(SceneGraph*)0) {
    SceneGraphProcessor processor(*pWrl);
    processor.computeNormalPerFace();
    _mainWindow->setSceneGraph(pWrl,false);
    _mainWindow->refresh();
    updateState();
  }
}

void WrlToolsWidget::on_pushButtonSceneGraphNormalPerCorner_clicked() {
  WrlViewerData& data = _mainWindow->getData();
  SceneGraph*    pWrl = data.getSceneGraph();
  if(pWrl!=(SceneGraph*)0) {
    SceneGraphProcessor processor(*pWrl);
    processor.computeNormalPerCorner();
    _mainWindow->setSceneGraph(pWrl,false);
    _mainWindow->refresh();
    updateState();
  }
}

void WrlToolsWidget::on_pushButtonPointsRemove_clicked() {
  WrlViewerData& data = _mainWindow->getData();
  SceneGraph*    pWrl = data.getSceneGraph();
  if(pWrl!=(SceneGraph*)0) {
    SceneGraphProcessor processor(*pWrl);
    processor.pointsRemove();
    _mainWindow->setSceneGraph(pWrl,false);
    _mainWindow->refresh();
    updateState();
  }
}

void WrlToolsWidget::on_pushButtonPointsShow_clicked() {
  WrlViewerData& data = _mainWindow->getData();
  SceneGraph*    pWrl = data.getSceneGraph();
  if(pWrl==(SceneGraph*)0) return;
  Node* node = pWrl->find("POINTS");
  if(node==(Node*)0) return;
  node->setShow(true);
  _mainWindow->setSceneGraph(pWrl,false);
  _mainWindow->refresh();
  updateState();
}

void WrlToolsWidget::on_pushButtonPointsHide_clicked() {
  WrlViewerData& data = _mainWindow->getData();
  SceneGraph*    pWrl = data.getSceneGraph();
  if(pWrl==(SceneGraph*)0) return;
  Node* node = pWrl->find("POINTS");
  if(node==(Node*)0) return;
  node->setShow(false);
  _mainWindow->setSceneGraph(pWrl,false);
  _mainWindow->refresh();
  updateState();
}

void WrlToolsWidget::on_pushButtonSceneGraphIndexedFaceSetsShow_clicked() {
  WrlViewerData& data = _mainWindow->getData();
  SceneGraph*    pWrl = data.getSceneGraph();
  if(pWrl!=(SceneGraph*)0) {
    SceneGraphProcessor processor(*pWrl);
    processor.shapeIndexedFaceSetShow();
    _mainWindow->setSceneGraph(pWrl,false);
    _mainWindow->refresh();
    updateState();
  }
}

void WrlToolsWidget::on_pushButtonSceneGraphIndexedFaceSetsHide_clicked() {
  WrlViewerData& data = _mainWindow->getData();
  SceneGraph*    pWrl = data.getSceneGraph();
  if(pWrl!=(SceneGraph*)0) {
    SceneGraphProcessor processor(*pWrl);
    processor.shapeIndexedFaceSetHide();
    _mainWindow->setSceneGraph(pWrl,false);
    _mainWindow->refresh();
    updateState();
  }
}

void WrlToolsWidget::on_pushButtonSceneGraphIndexedLineSetsShow_clicked() {
  WrlViewerData& data = _mainWindow->getData();
  SceneGraph*    pWrl = data.getSceneGraph();
  if(pWrl!=(SceneGraph*)0) {
    SceneGraphProcessor processor(*pWrl);
    processor.shapeIndexedLineSetShow();
    _mainWindow->setSceneGraph(pWrl,false);
    _mainWindow->refresh();
    updateState();
  }
}

void WrlToolsWidget::on_pushButtonSceneGraphIndexedLineSetsHide_clicked() {
  WrlViewerData& data = _mainWindow->getData();
  SceneGraph*    pWrl = data.getSceneGraph();
  if(pWrl!=(SceneGraph*)0) {
    SceneGraphProcessor processor(*pWrl);
    processor.shapeIndexedLineSetHide();
    _mainWindow->setSceneGraph(pWrl,false);
    _mainWindow->refresh();
    updateState();
  }
}

void WrlToolsWidget::on_pushButtonSurfaceRemove_clicked() {
  WrlViewerData& data = _mainWindow->getData();
  SceneGraph*    pWrl = data.getSceneGraph();
  if(pWrl!=(SceneGraph*)0) {
    SceneGraphProcessor processor(*pWrl);
    processor.surfaceRemove();
    _mainWindow->setSceneGraph(pWrl,false);
    _mainWindow->refresh();
    updateState();
  }
}

void WrlToolsWidget::on_pushButtonSurfaceShow_clicked() {
  WrlViewerData& data = _mainWindow->getData();
  SceneGraph*    pWrl = data.getSceneGraph();
  if(pWrl==(SceneGraph*)0) return;
  Node* node = pWrl->find("SURFACE");
  if(node==(Node*)0) return;
  node->setShow(true);
  _mainWindow->setSceneGraph(pWrl,false);
  _mainWindow->refresh();
  updateState();
}

void WrlToolsWidget::on_pushButtonSurfaceHide_clicked() {
  WrlViewerData& data = _mainWindow->getData();
  SceneGraph*    pWrl = data.getSceneGraph();
  if(pWrl==(SceneGraph*)0) return;
  Node* node = pWrl->find("SURFACE");
  if(node==(Node*)0) return;
  node->setShow(false);
  _mainWindow->setSceneGraph(pWrl,false);
  _mainWindow->refresh();
  updateState();
}

void WrlToolsWidget::on_pushButtonSurfaceSinglePlaneFit_clicked() {
  WrlViewerData& data = _mainWindow->getData();
  SceneGraph*    pWrl = data.getSceneGraph();
  if(pWrl!=(SceneGraph*)0) {
    SceneGraphProcessor processor(*pWrl);
    Vec3f& center = pWrl->getBBoxCenter();
    Vec3f& size   = pWrl->getBBoxSize();
    float  scale  = data.getBBoxScale();
    bool   cube   = data.getBBoxCube();
    // int    depth = data.getBBoxDepth();
    Vec4f  f;
    processor.fitSinglePlane(center,size,scale,cube,f);
    data.setPlane(f);
    _mainWindow->setSceneGraph(pWrl,false);
    _mainWindow->refresh();
    updateState();
  }
}

void WrlToolsWidget::on_pushButtonSurfaceMultiplePlanesFit_clicked() {
  WrlViewerData& data = _mainWindow->getData();
  Vec4f f; data.setPlane(f);
  //
  SceneGraph*    pWrl = data.getSceneGraph();
  if(pWrl!=(SceneGraph*)0) {
    SceneGraphProcessor processor(*pWrl);
    Vec3f& center = pWrl->getBBoxCenter();
    Vec3f& size   = pWrl->getBBoxSize();
    float  scale  = data.getBBoxScale();
    bool   cube   = data.getBBoxCube();
    int    depth = data.getBBoxDepth();
    vector<float> fVec;
    processor.fitMultiplePlanes(center,size,depth,scale,cube,fVec);
    _mainWindow->setSceneGraph(pWrl,false);
    _mainWindow->refresh();
    updateState();
  }
}

void WrlToolsWidget::on_pushButtonSurfaceContinuousFit_clicked() {
  WrlViewerData& data = _mainWindow->getData();
  Vec4f f; data.setPlane(f);
  //
  SceneGraph*    pWrl = data.getSceneGraph();
  if(pWrl!=(SceneGraph*)0) {
    SceneGraphProcessor processor(*pWrl);
    Vec3f& center = pWrl->getBBoxCenter();
    Vec3f& size   = pWrl->getBBoxSize();
    float  scale  = data.getBBoxScale();
    bool   cube   = data.getBBoxCube();
    int    depth = data.getBBoxDepth();
    processor.fitContinuous(center,size,depth,scale,cube);
    _mainWindow->setSceneGraph(pWrl,false);
    _mainWindow->refresh();
    updateState();
  }
}
