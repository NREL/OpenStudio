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

#include <QBoxLayout>
#include <QItemSelection>
#include <QSplitter>
#include <QStringList>

#include "InspectorGadget.hpp"
#include "ModelExplorer.hpp"
#include <model_editor/tablemodel.h>
#include "TableView.hpp"
#include "ClassViewWidget.hpp"

#include "../utilities/idf/WorkspaceObjectOrder.hpp"

#include "../utilities/core/Assert.hpp"
#include <utilities/idd/IddEnums.hxx>

namespace modeleditor
{

ClassViewWidget::ClassViewWidget(openstudio::model::Model& model, QWidget *parent)
  : ViewWidget(parent),
  mTableView(nullptr),
  mTableModel(nullptr)
{
  mSplitterSetting = "ClassViewWidgetSplitterSizes";
  createWidgets();

  mTableModel = new TableModel(this);
  mTableModel->setModel(model);
  loadModel(model);

  connectSignalsAndSlots();
  createLayout();
  restoreState();
  loadData();
}

ClassViewWidget::ClassViewWidget(QWidget *parent)
  : ViewWidget(parent),
  mTableView(nullptr),
  mTableModel(nullptr)
{
  mSplitterSetting = "ClassViewWidgetSplitterSizes";
  createWidgets();

  mTableModel = new TableModel(this);

  connectSignalsAndSlots();
  createLayout();
  restoreState();
  loadData();
}

ClassViewWidget::~ClassViewWidget()
{
}

TableView* ClassViewWidget::getTableView()
{
  return mTableView;
}

TableModel* ClassViewWidget::getTableModel()
{
  return mTableModel;
}

void ClassViewWidget::loadData()
{
  loadModel();
  mTableView->selectRow(0);
  QModelIndexList rowList;
  if(mTableView->getSelectedRows(rowList)){
    viewSelection(rowList.at(0));
  }
}

void ClassViewWidget::loadModel(openstudio::model::Model& model)
{
  loadWorkspace(model);
}

void ClassViewWidget::loadModel()
{
  if(mModelExplorer){
    mModel = mModelExplorer->getModel();
    loadModel(mModel);
    //mTableView->resizeColumnsToContents();
    //mTableView->resizeRowsToContents();
    setModelDirty(false);
    if(isVisible()){
      emit modelUpdated(static_cast<QAbstractItemModel *>(mTableModel));
    }
  }
}

void ClassViewWidget::loadWorkspace(const openstudio::Workspace& workspace)
{
  openstudio::WorkspaceObjectVector v = mModel.objects();
  mTableModel->loadObjects(v);

  mTableView->setModel(mTableModel);
  mTableView->setSortingEnabled(true);
}

void ClassViewWidget::createWidgets()
{
  mTableView = new TableView(this);
  mTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  mTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
  mTableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
  mTableView->setDragEnabled(true);
  mTableView->viewport()->setAcceptDrops(true);
  mTableView->setDropIndicatorShown(true);
}

void ClassViewWidget::connectSignalsAndSlots()
{
  connect(mTableView, &TableView::eventUpDnKeyRelease, this, static_cast<void (ClassViewWidget::*)()>(&ClassViewWidget::viewSelection));

  connect(mTableView, &TableView::clicked, this, static_cast<void (ClassViewWidget::*)()>(&ClassViewWidget::viewSelection));

  connect(mTableView, &TableView::eventEnter, this, &ClassViewWidget::eventEnter);

  connect(mTableView, &TableView::eventLeave, this, &ClassViewWidget::eventLeave);
}

void ClassViewWidget::createLayout()
{
  mSplitter->addWidget(mTableView);
  mSplitter->addWidget(mIG);

  mTableView->resizeColumnsToContents();
  mTableView->resizeRowsToContents();
}

void ClassViewWidget::insertObjects(const QModelIndexList& rowList,
                                    const std::vector<openstudio::IdfObject>& idfObjects,
                                    std::vector<openstudio::WorkspaceObject>& wsObjects)
{
  if(!rowList.size()){
    return;
  }

  QModelIndex selectedRow = rowList.at(0);
  TableModel * model = static_cast<TableModel *>(mTableView->model());
  openstudio::OptionalWorkspaceObject selectedObj = model->objectAtIndex(selectedRow);

  openstudio::OptionalWorkspaceObject optWsObj;

  ///! OpenStudio model update...
  for(unsigned i=0; i<idfObjects.size(); i++){
    optWsObj = mModel.addObject(idfObjects.at(i));
    if(optWsObj){
      wsObjects.push_back(*optWsObj);
      mModel.order().insert(optWsObj->handle(),selectedObj->handle());
    }
  }
}

void ClassViewWidget::addObjects(openstudio::IddObjectType type)
{
  if(type == openstudio::IddObjectType::UserCustom){
    return;
  }

  QModelIndexList rowList;
  if(!mTableView->getSelectedRows(rowList)){
    mTableView->selectRow(0);
    mTableView->getSelectedRows(rowList);
  }

  std::vector<openstudio::IdfObject> idfObjects;
  for(int i=0; i<rowList.size(); i++){
    idfObjects.push_back(openstudio::IdfObject(type));
  }

  ///! OpenStudio model update...
  std::vector<openstudio::WorkspaceObject> wsObjects;
  insertObjects(rowList,idfObjects,wsObjects);

  ///! Qt model update...
  //mTableModel->insertRows(wsObjects,rowList);

  loadModel();
  emit modelDirty();
}

void ClassViewWidget::pasteObjects()
{
  QModelIndexList rowList;
  if(mTableView->getSelectedRows(rowList)){

    QModelIndex selectedRow = rowList.at(0);

    ///! OpenStudio model update...
    std::vector<openstudio::WorkspaceObject> wsObjects;
    insertObjects(rowList,mIdfObjectsToPaste,wsObjects);

    ///! Qt model update...
    //mTableModel->insertRows(wsObjects,selectedRow,mPastedRows);

    loadModel();
    ///! select the first new row
    mTableView->selectRow(selectedRow.row()+1);
    
    emit modelDirty();
  }
}

void ClassViewWidget::removeObjects()
{
  QModelIndexList rowList;
  if(mTableView->getSelectedRows(rowList)){
    openstudio::Handle handle;
    std::vector<openstudio::Handle> handles;
    ///! OpenStudio model update...
    for(int i=0; i<rowList.size(); i++){
      handle = mTableModel->objectAtIndex(rowList.at(i))->handle();
      handles.push_back(handle);
      mModel.removeObject(mTableModel->objectAtIndex(rowList.at(i))->handle());
    }

    if(!handles.empty()){
      ///! Qt model update...
      //mTableModel->removeRows(rowList);
      loadModel();
      emit modelDirty();
    }

  }
}

void ClassViewWidget::copyObjects()
{
  mIdfObjectsToPaste.clear();
  QModelIndexList rowList;
  if(mTableView->getSelectedRows(rowList)){
    for (const QModelIndex& row : rowList){
      mIdfObjectsToPaste.push_back(*(mTableModel->objectAtIndex(row)));
    }
  }
}

bool ClassViewWidget::hasSelectedRows()
{
  return mTableView->hasSelectedRows();
}

bool ClassViewWidget::hasRowsToPaste()
{
  return mIdfObjectsToPaste.size();
}

void ClassViewWidget::toggleGUIDs()
{
  mTableModel->toggleGUIDs();
}

///! Slots
void ClassViewWidget::viewSelection()
{
  QItemSelectionModel * itemSelectionModel = mTableView->selectionModel();
  viewSelection(itemSelectionModel->currentIndex());
}

void ClassViewWidget::viewSelection(const QModelIndex& modelIndex)
{
  // get the available object
  openstudio::OptionalWorkspaceObject obj = mTableModel->objectAtIndex(modelIndex);
  mIG->layoutModelObj(*obj);
}

void ClassViewWidget::on_nameChanged(QString)
{
  if(isVisible()){
    loadModel(); // TODO costly and clunky
    update();
    return;
    /*QModelIndexList rowList;
    if(mTableView->getSelectedRows(rowList)){
      ///! we are only coded to handle a single row
      OS_ASSERT(rowList.size() == 1);
      //rowList.at(0).
    }*/
  }
  else{
    mModelDirty = true;
  }
}

} // namespace modeleditor
