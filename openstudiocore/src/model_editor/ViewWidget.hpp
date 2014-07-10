/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
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
  virtual void addObjects(openstudio::IddObjectType type = openstudio::IddObjectType::UserCustom) = 0;
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
