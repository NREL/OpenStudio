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

#include "../model/ParentObject_Impl.hpp"
#include "InspectorGadget.hpp"
#include "ModelExplorer.hpp"
#include <model_editor/modeltest.h>
#include <model_editor/treemodel.h>
#include "TreeView.hpp"

#include "TreeViewWidget.hpp"

#include "../utilities/core/Assert.hpp"
#include <utilities/idd/IddEnums.hxx>

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
  connect(mTreeView, &TreeView::eventUpDnKeyRelease, this, static_cast<void (TreeViewWidget::*)()>(&TreeViewWidget::viewSelection));

  connect(mTreeView, &TreeView::clicked, this, static_cast<void (TreeViewWidget::*)()>(&TreeViewWidget::viewSelection));

  connect(mTreeView, &TreeView::eventEnter, this, &TreeViewWidget::eventEnter);

  connect(mTreeView, &TreeView::eventLeave, this, &TreeViewWidget::eventLeave);

  connect(this, &TreeViewWidget::expandAll, mTreeView, &TreeView::expandAll);

  connect(this, &TreeViewWidget::collapseAll, mTreeView, &TreeView::collapseAll);
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
