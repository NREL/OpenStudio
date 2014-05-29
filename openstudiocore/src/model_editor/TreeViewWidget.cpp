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

#include <QBoxLayout>
#include <QItemSelection>
#include <QSplitter>
#include <QStringList>

#include "../model/ParentObject_Impl.hpp"
#include "InspectorGadget.hpp"
#include "ModelExplorer.hpp"
#include <model_editor/modeltest.h>
#include <model_editor/treemodel.h>
#include "TreeView.hpp"

#include "TreeViewWidget.hpp"

#include "../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace modeleditor
{

TreeViewWidget::TreeViewWidget(openstudio::model::Model& model, QWidget *parent)
  : ViewWidget(parent),
  mTreeView(nullptr),
  mTreeModel(nullptr)
{
  mSplitterSetting = "TreeViewWidgetSplitterSizes";
  createWidgets();

  mTreeModel = new TreeModel(this);
  loadModel(model);

  new ModelTest(mTreeModel, this);
  mTreeView->setModel(mTreeModel);

  connectSignalsAndSlots();
  createLayout();
  restoreState();
  loadData();
}

TreeViewWidget::TreeViewWidget(QWidget *parent)
  : ViewWidget(parent),
  mTreeView(nullptr),
  mTreeModel(nullptr)
{
  mSplitterSetting = "TreeViewWidgetSplitterSizes";
  createWidgets();

  mTreeModel = new TreeModel(this);

  connectSignalsAndSlots();
  createLayout();
  restoreState();
  loadData();
}

TreeViewWidget::~TreeViewWidget()
{
}

TreeView* TreeViewWidget::getTreeView()
{
  return mTreeView;
}

TreeModel* TreeViewWidget::getTreeModel()
{
  return mTreeModel;
}

void TreeViewWidget::loadData()
{
  loadModel();
  QModelIndex index = mTreeModel->index(0, 0);
  mTreeView->setCurrentIndex(index);
  QModelIndexList rowList;
  if(mTreeView->getSelectedRows(rowList)){
    viewSelection(rowList.at(0));
  }
}

void TreeViewWidget::loadModel(openstudio::model::Model& model)
{
  ///! Creating and loading the simulation adds objects
  ///! to the model which is shared by the app's views
  //int size1 = mModel.workspace().objects().size();
  //DLM@20101107: this is bad because it makes a SimulationControl if there was none, not good for component
  //editing
  //  openstudio::model::SimulationControl simulation = model.getUniqueModelObject<openstudio::model::SimulationControl>();
  //  loadSimulation(simulation);
  //int size2 = mModel.workspace().objects().size();

  mTreeModel->loadModel(model);
  new ModelTest(mTreeModel, this);
  mTreeView->setModel(mTreeModel);
}

void TreeViewWidget::loadModel()
{
  if(mModelExplorer){
    setExpandedIndexHandles();
    mModel = mModelExplorer->getModel();
    loadModel(mModel);
    setModelDirty(false);
    expandAppropriateTreeNodes();
  }
}

void TreeViewWidget::createWidgets()
{
  mTreeView = new TreeView(this);
  mTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  mTreeView->setSelectionBehavior(QAbstractItemView::SelectRows);
  mTreeView->setSelectionMode(QAbstractItemView::SingleSelection);
  mTreeView->setDragEnabled(true);
  mTreeView->viewport()->setAcceptDrops(true);
  mTreeView->setDropIndicatorShown(true);
}

void TreeViewWidget::connectSignalsAndSlots()
{
  bool connected;
  connected = connect(mTreeView,SIGNAL(eventUpDnKeyRelease()), 
    this,SLOT(viewSelection()));
  OS_ASSERT(connected);

  connected = connect(mTreeView,SIGNAL(clicked(const QModelIndex&)), 
    this,SLOT(viewSelection(const QModelIndex&)));
  OS_ASSERT(connected);

  connected = connect(mTreeView,SIGNAL(eventEnter()),
    this,SIGNAL(eventEnter()));
  OS_ASSERT(connected);

  connected = connect(mTreeView,SIGNAL(eventLeave()),
    this,SIGNAL(eventLeave()));
  OS_ASSERT(connected);

  connected = connect(this,SIGNAL(expandAll()),
    mTreeView,SLOT(expandAll()));
  OS_ASSERT(connected);

  connected = connect(this,SIGNAL(collapseAll()),
    mTreeView,SLOT(collapseAll()));
  OS_ASSERT(connected);
}

void TreeViewWidget::expandAllNodes()
{
  emit expandAll();
}

void TreeViewWidget::collapseAllNodes()
{
  emit collapseAll();
}

void TreeViewWidget::createLayout()
{
  mSplitter->addWidget(mTreeView);
  mSplitter->addWidget(mIG);
}

void TreeViewWidget::addObjects(openstudio::IddObjectType type)
{
  if(type == openstudio::IddObjectType::UserCustom){
    return;
  }

  QModelIndexList rowList;
  if(mTreeView->getSelectedRows(rowList)){
    mTreeView->expand(rowList.at(0));
    openstudio::Handle handle = mTreeModel->modelAtIndex(rowList.at(0))->handle();
    TreeModel * model = static_cast<TreeModel *>(mTreeView->model());
    model->addRows(rowList,type);
    loadModel();

    ///! TODO this is a hack to get all indexes on all tree nodes
    expandAllNodes();
    QModelIndex index;
    QModelIndexList persistentIndexList = mTreeModel->getPersistentIndexList();

    for(int i=0; i<persistentIndexList.size(); i++){
      index = persistentIndexList.at(i);
      if(index.isValid()){
        if(mTreeModel->modelAtIndex(index)->handle() == handle){
          mTreeView->setCurrentIndex(index);
          break;
        }
      }
    }

    emit modelDirty();
  }
}

void TreeViewWidget::removeObjects()
{
  QModelIndexList rowList;
  if(mTreeView->getSelectedRows(rowList)){
    std::vector<openstudio::Handle> handles;
    TreeModel * model = static_cast<TreeModel *>(mTreeView->model());
    model->removeRows(rowList,handles);
    if(!handles.empty()){
      loadModel();
      emit modelDirty();
    }
  }
}

void TreeViewWidget::copyObjects()
{
  mModelObjectsToPaste.clear();
  QModelIndexList rowList;
  if(mTreeView->getSelectedRows(rowList)){
    TreeModel * model = static_cast<TreeModel *>(mTreeView->model());
    for (const QModelIndex& row : rowList){
      mModelObjectsToPaste.push_back(*(model->modelAtIndex(row)));
    }
  }
}

void TreeViewWidget::pasteObjects()
{
  QModelIndexList rowList;
  if(mTreeView->getSelectedRows(rowList)){
    mTreeView->expand(rowList.at(0));
    openstudio::Handle handle = mModelObjectsToPaste.at(0).handle();

    TreeModel * model = static_cast<TreeModel *>(mTreeView->model());
    model->pasteRows(rowList.at(0),mModelObjectsToPaste);
    loadModel();

    ///! TODO this is a hack to get all indexes on all tree nodes
    expandAllNodes();
    QModelIndex index;
    QModelIndexList persistentIndexList = mTreeModel->getPersistentIndexList();

    for(int i=0; i<persistentIndexList.size(); i++){
      index = persistentIndexList.at(i);
      if(index.isValid()){
        if(mTreeModel->modelAtIndex(index)->handle() == handle){
          mTreeView->setCurrentIndex(index);
          break;
        }
      }
    }

    emit modelDirty();
  }
}

std::vector<openstudio::IddObjectType> TreeViewWidget::getAllowableChildTypes()
{
  QModelIndexList rowList;
  std::vector<openstudio::IddObjectType> allowableChildTypes;
  if(mTreeView->getSelectedRows(rowList)){
    TreeModel * model = static_cast<TreeModel *>(mTreeView->model());
    OptionalModelObject temp = model->modelAtIndex(rowList.at(0));
    OptionalParentObject tempParent;
    if( temp && 
        ( tempParent = temp->optionalCast<ParentObject>() ) )
    {
      allowableChildTypes = tempParent->allowableChildTypes();
    }
  }
  return allowableChildTypes;
}

bool TreeViewWidget::hasSelectedRows()
{
  return mTreeView->hasSelectedRows();
}

bool TreeViewWidget::hasRowsToPaste()
{
  return mModelObjectsToPaste.size();
}

void TreeViewWidget::setExpandedIndexHandles()
{
  mExpandedIndexHandles.clear();

  std::pair<QModelIndex, openstudio::Handle> expandedIndexHandle;
  
  QModelIndex index;
  QModelIndexList persistentIndexList = mTreeModel->getPersistentIndexList();

  for(int i=0; i<persistentIndexList.size(); i++){
    index = persistentIndexList.at(i);
    if(index.isValid() && mTreeView->isExpanded(index)){
      expandedIndexHandle.first = index;
      expandedIndexHandle.second = mTreeModel->modelAtIndex(index)->handle();
      mExpandedIndexHandles.push_back(expandedIndexHandle);
    }
  }
}

void TreeViewWidget::expandAppropriateTreeNodes()
{
  ///! TODO this is a hack to get all indexes on all tree nodes
  expandAllNodes();
  QModelIndex index;
  QModelIndexList persistentIndexList = mTreeModel->getPersistentIndexList();
  collapseAllNodes();

  for(int i=0; i<persistentIndexList.size(); i++){
    index = persistentIndexList.at(i);
    if(index.isValid()){
      for(unsigned j=0; j<mExpandedIndexHandles.size(); j++){
        if(mTreeModel->modelAtIndex(index)->handle() == mExpandedIndexHandles.at(j).second){
          mTreeView->expand(index);
          break;
        }
      }
    }
  }
}

void TreeViewWidget::toggleGUIDs()
{
  mTreeModel->toggleGUIDs();
}

///! Slots
void TreeViewWidget::viewSelection()
{
  QItemSelectionModel * itemSelectionModel = mTreeView->selectionModel();
  viewSelection(itemSelectionModel->currentIndex());
}

void TreeViewWidget::viewSelection(const QModelIndex& modelIndex)
{
  // get the available object
  openstudio::model::OptionalModelObject modelObject = mTreeModel->modelAtIndex(modelIndex);
  mIG->layoutModelObj(*modelObject);
}

void TreeViewWidget::on_nameChanged(QString)
{
  if(isVisible()){
    loadModel(); // TODO costly and clunky
    update();
    return;
    /*QModelIndexList rowList;
    if(mTreeView->getSelectedRows(rowList)){
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
