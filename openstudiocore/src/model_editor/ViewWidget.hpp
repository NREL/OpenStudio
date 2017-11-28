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

#ifndef MODELEDITOR_VIEWWIDGET_HPP
#define MODELEDITOR_VIEWWIDGET_HPP

#include <QWidget>

#include "../model/Model.hpp"
#include "../model/Building.hpp"
#include "ModelEditorAPI.hpp"

class IGPrecisionDialog;
class InspectorGadget;

class QAbstractItemModel;
class QModelIndex;
class QSplitter;

namespace openstudio {
  class IddFile;
}

namespace modeleditor
{

class ModelExplorer;

class ViewWidget : public QWidget
{
  Q_OBJECT

public:
  ViewWidget(QWidget *parent = nullptr);
  ViewWidget(openstudio::model::Model model, QWidget *parent = nullptr);
  virtual ~ViewWidget();
  virtual void addObjects(openstudio::IddObjectType type = openstudio::IddObjectType("UserCustom")) = 0;
  virtual void loadModel() = 0;
  virtual void removeObjects() = 0;
  virtual void copyObjects() = 0;
  virtual void pasteObjects() = 0;
  virtual bool hasSelectedRows() = 0;
  virtual bool hasRowsToPaste() = 0;
  virtual openstudio::model::Model& getModel();
  virtual const openstudio::IddFile& getIddFile();
  virtual bool getModelDirty()const;
  virtual void setModelDirty(const bool modelDirty);
  virtual void emitModelDirty();
  virtual void toggleGUIDs() = 0;
  virtual void restoreState();
  virtual void showComments(const bool showComments);
  virtual void showPrecisionDlg(const bool showPrecisionDlg = true);
  virtual void showAllFields(bool state);
  virtual void createAllFields();
  virtual void setRecursive(bool state);                                  
                                                                   

public slots:
  virtual void viewSelection(const QModelIndex& modelIndex) = 0;
  void on_modelDirty();
  virtual void on_nameChanged(QString);

signals:
  void eventEnter();
  void eventLeave();
  void modelDirty();
  void modelUpdated(QAbstractItemModel * model);
  void commentsShow(bool);
  void precisionDlgShow();
  void precisionDlgHide();
  void precisionDlgFinished();

protected:
  QSplitter * mSplitter;
  InspectorGadget * mIG;
  IGPrecisionDialog * mIGPrecisionDlg;
  ModelExplorer * mModelExplorer;
  openstudio::model::Model mModel;
  bool mModelDirty;
  QString mSplitterSetting;

private:
  virtual void connectSignalsAndSlots();
  virtual void createLayout();
  virtual void createWidgets();
  virtual void loadData() = 0;
  virtual void loadModel(openstudio::model::Model& model) = 0;
  virtual void saveState();
};

} // namespace modeleditor

#endif // MODELEDITOR_VIEWWIDGET_HPP
