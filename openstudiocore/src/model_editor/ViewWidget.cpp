/**********************************************************************
*  Copyright (c) 2008-2015, Alliance for Sustainable Energy.  
*  All rights reserved.
*  
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*  
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*  
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#include <QApplication>
#include <QBoxLayout>
#include <QSettings>
#include <QSplitter>

#include "IGPrecisionDialog.hpp"
#include "InspectorGadget.hpp"
#include "ModelExplorer.hpp"

#include "ViewWidget.hpp"

#include "../utilities/idd/IddFile.hpp"

#include "../utilities/core/Assert.hpp"

namespace modeleditor
{

ViewWidget::ViewWidget(QWidget *parent)
  : QWidget(parent),
  mSplitter(nullptr),
  mIG(nullptr),
  mIGPrecisionDlg(nullptr),
  mModelExplorer(nullptr),
  mModelDirty(false)
{
  mModelExplorer = qobject_cast<ModelExplorer *>(this->parent());
  OS_ASSERT(mModelExplorer);

  createWidgets();
  createLayout();
  connectSignalsAndSlots();
}

ViewWidget::ViewWidget(openstudio::model::Model model, QWidget *parent)
  : QWidget(parent),
  mSplitter(nullptr),
  mIG(nullptr),
  mIGPrecisionDlg(nullptr),
  mModelExplorer(nullptr),
  mModel(model),
  mModelDirty(false)
{
  mModelExplorer = qobject_cast<ModelExplorer *>(this->parent());
  OS_ASSERT(mModelExplorer);

  createWidgets();
  createLayout();
  connectSignalsAndSlots();
}

ViewWidget::~ViewWidget()
{
  saveState();
}

void ViewWidget::saveState()
{
  if(mSplitterSetting.length() == 0) return;
  QString organizationName = QCoreApplication::organizationName();
  QString applicationName = QCoreApplication::applicationName();
  QSettings settings(organizationName, applicationName);
  settings.setValue(mSplitterSetting, mSplitter->saveState());
}

void ViewWidget::restoreState()
{
  if(mSplitterSetting.length() == 0) return;
  QString organizationName = QCoreApplication::organizationName();
  QString applicationName = QCoreApplication::applicationName();
  QSettings settings(organizationName, applicationName);
  mSplitter->restoreState(settings.value(mSplitterSetting).toByteArray());
}

openstudio::model::Model& ViewWidget::getModel()
{
  return mModel;
}

const openstudio::IddFile& ViewWidget::getIddFile()
{
  return mModelExplorer->getIddFile();
}

void ViewWidget::createWidgets()
{
  mSplitter = new QSplitter(this);
  QSplitter splitter;
  mSplitter->setHandleWidth(2*splitter.handleWidth());

  mIG = new InspectorGadget(this);
  mIG->setMinimumSize(300,400);

  mIGPrecisionDlg = new IGPrecisionDialog(this);
}

void ViewWidget::connectSignalsAndSlots()
{
  connect(mIG, &InspectorGadget::nameChanged, this, &ViewWidget::on_nameChanged);

  connect(this, &ViewWidget::commentsShow, mIG, &InspectorGadget::commentConfig);

  connect(this, &ViewWidget::precisionDlgShow, mIGPrecisionDlg, &IGPrecisionDialog::show);

  connect(this, &ViewWidget::precisionDlgHide, mIGPrecisionDlg, &IGPrecisionDialog::hide);

  connect(mIGPrecisionDlg, &IGPrecisionDialog::accepted, mIG, &InspectorGadget::setPrec);

  connect(mIGPrecisionDlg, &IGPrecisionDialog::accepted, this, &ViewWidget::precisionDlgFinished);

  connect(mIGPrecisionDlg, &IGPrecisionDialog::rejected, this, &ViewWidget::precisionDlgFinished);

  connect(mIGPrecisionDlg, &IGPrecisionDialog::finished, this, &ViewWidget::precisionDlgFinished);

  connect(mIG, &InspectorGadget::dirty, this, &ViewWidget::modelDirty);

 
}

void ViewWidget::createLayout()
{
  auto hLayout = new QHBoxLayout();
  hLayout->addWidget(mSplitter);

  setLayout(hLayout);
}

bool ViewWidget::getModelDirty()const
{
  return mModelDirty;
}

void ViewWidget::setModelDirty(const bool modelDirty)
{
  mModelDirty = modelDirty;
}

void ViewWidget::emitModelDirty()
{
  emit modelDirty();
}

void ViewWidget::showComments(const bool showComments)
{
  emit commentsShow(showComments);
}


void ViewWidget::showPrecisionDlg(const bool showPrecisionDlg)
{
  if(showPrecisionDlg){
    emit precisionDlgShow();
  }
  else{
    emit precisionDlgHide();
  }
}

  void ViewWidget::showAllFields(bool state)
  {
    mIG->showAllFields(state);
  }

  void ViewWidget::createAllFields()
  {
    mIG->createAllFields();
  }
  
  void ViewWidget::setRecursive(bool state)
  {
    mIG->setRecursive(state);
  }                                  

///! Slots

void ViewWidget::on_modelDirty()
{
  setModelDirty(true);
}

void ViewWidget::on_nameChanged(QString)
{
}

} // namespace modeleditor
