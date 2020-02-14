//------------------------------------------------------------------------
//  Copyright (C) Gabriel Taubin
//  Time-stamp: <2019-11-19 14:13:18 taubin>
//------------------------------------------------------------------------
//
// WrlMainWindow.cpp
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

#include "WrlMainWindow.hpp"
#include "WrlGLWidget.hpp"
#include "WrlAboutDialog.hpp"

#include <QApplication>
#include <QMenuBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QDesktopWidget>

#include "io/LoaderWrl.hpp"
#include "io/SaverWrl.hpp"

#include <iostream>

using namespace std;

int WrlMainWindow::_timerInterval = 20;

//////////////////////////////////////////////////////////////////////
WrlMainWindow::WrlMainWindow(QWidget* parent, Qt::WindowFlags flags):
  QMainWindow(parent,flags) {
  setupUi(this);
  setWindowIcon(QIcon("qt.icns"));
  setWindowTitle(WINDOW_TITLE);

  LoaderWrl* wrlLoader = new LoaderWrl();
  _loader.registerLoader(wrlLoader);

  SaverWrl* wrlSaver = new SaverWrl();
  _saver.registerSaver(wrlSaver);

  // for animation
  _timer = new QTimer(this);
  _timer->setInterval(_timerInterval);
  connect(_timer, SIGNAL(timeout()), glWidget, SLOT(update()));

  // glWidget->setBackgroundColor(qRgb(75, 100, 150));
  glWidget->setBackgroundColor(qRgb(200,200,200));
  glWidget->setMaterialColor(qRgb(225, 150, 75));
  glWidget->setMainWindow(this);
  glWidget->show();

  // QDesktopWidget dw;
  // const QRect dw_ag = dw.availableGeometry(this);
  // resize(dw_ag.width()/2,3*dw_ag.height()/4);

  toolsWidget->setMainWindow(this);
  // toolsWidget->hide();
  toolsWidget->show();

  showStatusBarMessage("");
}

//////////////////////////////////////////////////////////////////////
WrlMainWindow::~WrlMainWindow() {
}

//////////////////////////////////////////////////////////////////////
void WrlMainWindow::showStatusBarMessage(const QString & message) {
  statusBar()->showMessage(message);
}

//////////////////////////////////////////////////////////////////////
void WrlMainWindow::on_fileLoadAction_triggered() {

  std::string filename;

  // stop animation
  _timer->stop();

  QFileDialog fileDialog(this);
  fileDialog.setFileMode(QFileDialog::ExistingFile); // allowed to select only one 
  fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
  fileDialog.setNameFilter(tr("VRML Files (*.wrl)"));
  QStringList fileNames;
  if(fileDialog.exec()) {
    fileNames = fileDialog.selectedFiles();
    if(fileNames.size()>0)
      filename = fileNames.at(0).toStdString();
  }

  if (filename.empty()) {
    showStatusBarMessage("load filename is empty");
  } else {

    static char str[1024];

    sprintf(str,"Loading \"%s\" ...",filename.c_str());
    showStatusBarMessage(QString(str));

    SceneGraph* pWrl = new SceneGraph();
    if(_loader.load(filename.c_str(),*pWrl)) { // if success
      sprintf(str,"Loaded \"%s\"",filename.c_str());
      pWrl->updateBBox();
      glWidget->setSceneGraph(pWrl,true);
      toolsWidget->updateState();
    } else {
      delete pWrl;
      sprintf(str,"Unable to load \"%s\"",filename.c_str());
    }

    showStatusBarMessage(QString(str));
  } 

  // restart animation
  _timer->start(_timerInterval);
}

//////////////////////////////////////////////////////////////////////
void WrlMainWindow::on_fileSaveAction_triggered() {

  std::string filename;

  // stop animation
  _timer->stop();

  QFileDialog fileDialog(this);
  fileDialog.setFileMode(QFileDialog::AnyFile); // allowed to select only one 
  fileDialog.setAcceptMode(QFileDialog::AcceptSave);
  fileDialog.setNameFilter(tr("VRML Files (*.wrl)"));
  QStringList fileNames;
  if(fileDialog.exec()) {
    fileNames = fileDialog.selectedFiles();
    if(fileNames.size()>0)
      filename = fileNames.at(0).toStdString();
  }

  // restart animation
  _timer->start(_timerInterval);

  if (filename.empty()) {
    showStatusBarMessage("save filename is empty");
  } else {

    static char str[1024];

    sprintf(str,"Saving \"%s\" ...",filename.c_str());
    showStatusBarMessage(QString(str));

    SceneGraph* pWrl = glWidget->getSceneGraph();

    if(_saver.save(filename.c_str(),*pWrl)) {
      sprintf(str,"Saved \"%s\"", filename.c_str());
    } else {
      sprintf(str,"Unable to save \"%s\"",filename.c_str());
    }

    showStatusBarMessage(QString(str));
  }
}

//////////////////////////////////////////////////////////////////////
void WrlMainWindow::on_fileExitAction_triggered() {
  close();
}

//////////////////////////////////////////////////////////////////////
void WrlMainWindow::on_toolsShowAction_triggered() {
  // int c3dWidth  = getGLWidgetWidth();
  // int c3dHeight = getGLWidgetHeight();
  toolsWidget->show();
  // setGLWidgetSize(c3dWidth,c3dHeight);
}

//////////////////////////////////////////////////////////////////////
void WrlMainWindow::on_toolsHideAction_triggered() {
  // int c3dWidth  = getGLWidgetWidth();
  // int c3dHeight = getGLWidgetHeight();
  toolsWidget->hide();
  // setGLWidgetSize(c3dWidth,c3dHeight);
}

//////////////////////////////////////////////////////////////////////
void WrlMainWindow::on_helpAboutAction_triggered() {
  WrlAboutDialog dialog(this);
  dialog.exec();
}

//////////////////////////////////////////////////////////////////////
int WrlMainWindow::getGLWidgetWidth() {
  return glWidget->size().width();
}

//////////////////////////////////////////////////////////////////////
int WrlMainWindow::getGLWidgetHeight() {
  return glWidget->size().height();
}

//////////////////////////////////////////////////////////////////////
void WrlMainWindow::resizeEvent(QResizeEvent* event) {
  cout << "WrlMainWindow::resizeEvent() {" << endl;
  auto gls = glWidget->size();
  cout << "  glWidget.size() = (" << gls.width() << "," << gls.height() << ")" << endl;
  auto tls = toolsWidget->size();
  cout << "  toolsWidget.size() = (" << tls.width() << "," << tls.height() << ")" << endl;
  (void) event;
  toolsWidget->updateState();
  cout << "}" << endl;
}

//////////////////////////////////////////////////////////////////////
// void WrlMainWindow::setGLWidgetSize(int c3dWidth, int c3dHeight) {
//   int width  = _bdryLeft + c3dWidth  +  _bdryRight;
//   if(_toolsWidget->isVisible()) width  += _bdryCenter + _toolsWidth;
//   QMenuBar   *menuBar   = this->menuBar();
//   QStatusBar *statusBar = this->statusBar();
//   int mbH = (menuBar  )?menuBar->height()  :0;
//   int sbH = (statusBar)?statusBar->height():0;
//   int height = _bdryTop  + c3dHeight + _bdryBottom + sbH+mbH;
//   this->resize(width,height);
//   // _resize(width,height);
// }

WrlViewerData& WrlMainWindow::getData() const {
  return glWidget->getData();
}

SceneGraph* WrlMainWindow::getSceneGraph() {
  return glWidget->getSceneGraph();
}

void WrlMainWindow::setSceneGraph(SceneGraph* pWrl, bool resetHomeView) {
  glWidget->setSceneGraph(pWrl,resetHomeView);
}

void WrlMainWindow::updateState() {
  toolsWidget->updateState();
}

void WrlMainWindow::refresh() {
  glWidget->update();
}
