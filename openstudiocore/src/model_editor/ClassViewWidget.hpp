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

#ifndef MODELEDITOR_CLASSVIEWWIDGET_HPP
#define MODELEDITOR_CLASSVIEWWIDGET_HPP

#include <QModelIndex>

#include "ViewWidget.hpp"

#include "../model/Building.hpp"
#include "ModelEditorAPI.hpp"

namespace modeleditor
{

class TableModel;
class TableView;

class ClassViewWidget : public ViewWidget
{
  Q_OBJECT

public:
  ClassViewWidget(openstudio::model::Model& model, QWidget *parent = nullptr);
  ClassViewWidget(QWidget *parent = nullptr);
  virtual ~ClassViewWidget();
  void loadWorkspace(const openstudio::Workspace& workspace);
  virtual void addObjects(openstudio::IddObjectType type = openstudio::IddObjectType::UserCustom);
  virtual void loadModel();
  virtual void removeObjects();
  virtual void copyObjects();
  virtual void pasteObjects();
  virtual bool hasSelectedRows();
  virtual bool hasRowsToPaste();
  TableView * getTableView();
  TableModel * getTableModel();
  virtual void toggleGUIDs();

public slots:
  virtual void viewSelection(const QModelIndex& modelIndex);
  virtual void on_nameChanged(QString);
  virtual void viewSelection();

signals:

protected:
  TableView * mTableView;
  TableModel * mTableModel;

private:
  virtual void createWidgets();
  virtual void connectSignalsAndSlots();
  virtual void createLayout();
  virtual void loadData();
  virtual void loadModel(openstudio::model::Model& model);
  void insertObjects(const QModelIndexList& rowList,
    const std::vector<openstudio::IdfObject>& idfObjects,
    std::vector<openstudio::WorkspaceObject>& wsObjects);

  std::vector<openstudio::IdfObject> mIdfObjectsToPaste;
};

} // namespace modeleditor

#endif // MODELEDITOR_CLASSVIEWWIDGET_HPP
