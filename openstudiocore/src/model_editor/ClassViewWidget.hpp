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
  virtual void addObjects(openstudio::IddObjectType type = openstudio::IddObjectType("UserCustom")) override;
  virtual void loadModel() override;
  virtual void removeObjects() override;
  virtual void copyObjects() override;
  virtual void pasteObjects() override;
  virtual bool hasSelectedRows() override;
  virtual bool hasRowsToPaste() override;
  TableView * getTableView();
  TableModel * getTableModel();
  virtual void toggleGUIDs() override;

public slots:
  virtual void viewSelection(const QModelIndex& modelIndex) override;
  virtual void on_nameChanged(QString) override;
  virtual void viewSelection();

signals:

protected:
  TableView * mTableView;
  TableModel * mTableModel;

private:
  virtual void createWidgets() override;
  virtual void connectSignalsAndSlots() override;
  virtual void createLayout() override;
  virtual void loadData() override;
  virtual void loadModel(openstudio::model::Model& model) override;
  void insertObjects(const QModelIndexList& rowList,
    const std::vector<openstudio::IdfObject>& idfObjects,
    std::vector<openstudio::WorkspaceObject>& wsObjects);

  std::vector<openstudio::IdfObject> mIdfObjectsToPaste;
};

} // namespace modeleditor

#endif // MODELEDITOR_CLASSVIEWWIDGET_HPP
