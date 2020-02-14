//------------------------------------------------------------------------
//  Copyright (C) Gabriel Taubin
//  Time-stamp: <2019-11-19 14:13:18 taubin>
//------------------------------------------------------------------------
//
// WrlToolsWidget.hpp
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

#ifndef _WRL_TOOLSWIDGET_HPP_
#define _WRL_TOOLSWIDGET_HPP_

#include "ui_WrlToolsWidget.h"

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QCheckBox>

class WrlMainWindow;

class WrlToolsWidget : public QWidget, public Ui_WrlToolsWidget {

  Q_OBJECT

public:

  WrlToolsWidget(QWidget* parent=0);
  ~WrlToolsWidget();
                   
  void setMainWindow(WrlMainWindow *mw);
  void updateState();

private slots:

  // bounding box
  void bboxCube();
  void on_spinBoxBBoxDepth_valueChanged(int depth);
  void bboxDepthUp();
  void bboxDepthDown();
  void on_pushButtonBBoxAdd_clicked();
  void on_pushButtonBBoxRemove_clicked();
  void on_editBBoxScale_returnPressed();
  void on_checkBoxBBoxCube_stateChanged(int satate);

  // scene graph
  void on_pushButtonSceneGraphNormalNone_clicked();
  void on_pushButtonSceneGraphNormalPerVertex_clicked();
  void on_pushButtonSceneGraphNormalInvert_clicked();
  void on_pushButtonSceneGraphNormalPerFace_clicked();
  void on_pushButtonSceneGraphNormalPerCorner_clicked();
  void on_pushButtonSceneGraphEdgesAdd_clicked();
  void on_pushButtonSceneGraphEdgesRemove_clicked();
  void on_pushButtonSceneGraphIndexedFaceSetsShow_clicked();
  void on_pushButtonSceneGraphIndexedFaceSetsHide_clicked();
  void on_pushButtonSceneGraphIndexedLineSetsShow_clicked();
  void on_pushButtonSceneGraphIndexedLineSetsHide_clicked();

  // points
  void on_pushButtonPointsRemove_clicked();
  void on_pushButtonPointsShow_clicked();
  void on_pushButtonPointsHide_clicked();

  // surface reconstruction
  void on_pushButtonSurfaceSinglePlaneFit_clicked();
  void on_pushButtonSurfaceMultiplePlanesFit_clicked();
  void on_pushButtonSurfaceContinuousFit_clicked();
  void on_pushButtonSurfaceRemove_clicked();
  void on_pushButtonSurfaceShow_clicked();
  void on_pushButtonSurfaceHide_clicked();

private:

  WrlMainWindow* _mainWindow;

};

#endif // _WRL_TOOLSWIDGET_HPP_
