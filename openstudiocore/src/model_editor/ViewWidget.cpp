/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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
