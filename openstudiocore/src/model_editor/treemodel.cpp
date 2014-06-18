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

/*
  treemodel.cpp

  Provides a simple tree model to show how to create and use hierarchical
  models.
*/

#include <QMimeData>
#include <QModelIndex>

#include "../model/Lights.hpp"
#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/OutputVariable.hpp"
#include "../model/ParentObject_Impl.hpp"
#include "../model/People.hpp"
#include "../model/SimulationControl.hpp"
#include "../model/SimulationControl_Impl.hpp"
#include "../model/Surface.hpp"
#include "../model/SubSurface.hpp"
#include "../model/Space.hpp"
#include "../model/test/ModelFixture.hpp"
#include <model_editor/treeitem.h>
#include "TreeView.hpp"
#include "TreeViewWidget.hpp"
#include "../utilities/idf/IdfFile.hpp"
#include "../utilities/idf/WorkspaceObjectOrder.hpp"

#include <model_editor/treemodel.h>

#include "../utilities/core/Assert.hpp"

#define guidOpenCurlyBrace '{'
#define guidCloseCurlyBrace '}'

using namespace openstudio::model;
using namespace openstudio;

namespace modeleditor
{

TreeModel::TreeModel(openstudio::model::Model& model, TreeViewWidget * parent)
  : QAbstractItemModel(parent),
  mTreeViewWidget(parent),
  rootItem(nullptr),
  mMaskGUIDs(false)
{
  QList<QVariant> rootData;
  rootData << "Model Objects";
  //model.addObject(openstudio::IdfObject(openstudio::IddObjectType::Version));
  openstudio::model::SimulationControl simulation = model.getUniqueModelObject<openstudio::model::SimulationControl>();
  // rootItem deleted in destructor
  rootItem = new TreeItem(simulation,rootData);

  for (ModelObject object : model.modelObjects()){
    if (!object.parent()){
      setupModelData(object, rootItem);
    }
  }
}

TreeModel::TreeModel(TreeViewWidget *parent)
  : QAbstractItemModel(parent),
  mTreeViewWidget(parent),
  rootItem(nullptr),
  mMaskGUIDs(false)
{
}

TreeModel::~TreeModel()
{
  delete rootItem;
}

void TreeModel::loadModel(openstudio::model::Model& model)
{
  beginResetModel();

  ///! If we already have a tree and are loading a new OpenStudio model,
  ///! clean out the old tree first
  if(rootItem){
    delete rootItem;
  }
  QList<QVariant> rootData;
  rootData << "Model Objects";
  //model.addObject(openstudio::IdfObject(openstudio::IddObjectType::Version));
  openstudio::model::SimulationControl simulation = model.getUniqueModelObject<openstudio::model::SimulationControl>();
  // rootItem deleted in destructor
  rootItem = new TreeItem(simulation, rootData);

  for (ModelObject object : model.modelObjects()){
    if (!object.parent()){
      setupModelData(object, rootItem);
    }
  }

  endResetModel();
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
  if (parent.isValid())
    return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
  else
    return rootItem->columnCount();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();

  if (role != Qt::DisplayRole && role != Qt::EditRole)
    return QVariant();

  TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

  return item->data(index.column());
}

openstudio::model::OptionalModelObject TreeModel::modelAtIndex(const QModelIndex &index) const
{

  OS_ASSERT(index.isValid());
  TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
  return item->modelObject();
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
  if (index.isValid())
    return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
  else
    return nullptr;
}

TreeItem *TreeModel::getItem(const QModelIndex &index) const
{
  if(index.isValid()){
    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    if(item){
      return item;
    }
  }
  return rootItem;
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    return rootItem->data(section);

  return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
  if (!hasIndex(row, column, parent))
    return QModelIndex();

  TreeItem *parentItem;

  if (!parent.isValid())
    parentItem = rootItem;
  else
    parentItem = static_cast<TreeItem*>(parent.internalPointer());

  TreeItem *childItem = parentItem->child(row);
  if (childItem)
    return createIndex(row, column, childItem);
  else
    return QModelIndex();
}

bool TreeModel::insertRows(const QModelIndex row, const QModelIndexList rowList, openstudio::IddObjectType type)
{
  bool success = false;

  if(type == openstudio::IddObjectType::UserCustom){
    return success;
  }

  int position = rowList.at(0).row();
  int rows = rowList.size();

  QModelIndex parent = row;
  TreeItem * parentItem = getItem(row);
  OS_ASSERT(parentItem);
  //OptionalWorkspaceObject workspaceObject = parentItem->data().workspaceObject();
  //OS_ASSERT(workspaceObject);
  //QString iddObjectName(workspaceObject->iddObject().name().c_str());

  ///! For now, we should only try objects without children as the children wouldn't be
  ///! handled in the Qt model with this current arrangement.

  ///! OpenStudio model update...
  ///! Currently unable to safely try a list.   Need to cache what was successfully
  ///! pasted and then use that vector to populate the Qt model.

  openstudio::OptionalWorkspaceObject optWsObj;
  openstudio::model::OptionalModelObject optModelObj;
  std::vector<openstudio::model::ModelObject> parentObjects;

  for(int i=0; i<rows; ++i)
  {
    optWsObj = parentItem->modelObject().workspace().addObject(openstudio::IdfObject(type));
    if(!optWsObj)
    {
      return success;
    }
    else
    {
      optModelObj = parentItem->modelObject().model().getModelObject<ModelObject>(optWsObj->handle());
      if(!optModelObj)
      {
        return success;
      }
      else
      {
        ModelObject temp(parentItem->modelObject());
        OptionalParentObject tempParent = temp.optionalCast<ParentObject>();
        success = optModelObj->setParent(*tempParent);
        if(success)
        {
          mTreeViewWidget->loadModel();
          parentObjects.push_back(*optModelObj);
        } 
      }
    }
  }
  
  return success; // not updating Qt model
  
  if(!success) return success; // Nothing to add to the Qt model;
  
  ///! Qt model update...
  beginInsertRows(parent, position+1, position+rows);
  // get the idd object type
  QList<QVariant> objectNames;
  for(unsigned i=0; i<parentObjects.size(); i++)
  {
    getObjectNames(parentObjects.at(i), objectNames);
    success = parentItem->insertChildren(parentObjects.at(i), objectNames, position+1, rows, rootItem->columnCount());
    if(!success)
    {
      break;
    }
  }
  endInsertRows();

  return success;
}

bool TreeModel::addRows(const QModelIndexList rowList, openstudio::IddObjectType type)
{
  if(type == openstudio::IddObjectType::UserCustom){
    return false;
  }

  const QModelIndex row = rowList.at(0);

  return insertRows(row, rowList, type);
}

bool TreeModel::pasteRows(const QModelIndex& parentRow, std::vector<openstudio::model::ModelObject>& modelObjectsToPaste)
{
  bool success = false;

  TreeItem * parentItem = getItem(parentRow);
  OS_ASSERT(parentItem);

  openstudio::OptionalWorkspaceObject optionalWorkspaceObject;
  openstudio::OptionalWorkspaceObject optionalWorkspaceObjectToPaste;
  openstudio::IddObject iddObjectToPaste;
  openstudio::IddObjectType iddObjectToPasteType;
  openstudio::model::OptionalModelObject optionalModelObject;
  openstudio::model::OptionalParentObject optionalParentObject;

  for(const auto & elem : modelObjectsToPaste)
  {
   
    iddObjectToPaste = elem.iddObject();
    iddObjectToPasteType = iddObjectToPaste.type();
    if(iddObjectToPasteType == openstudio::IddObjectType::UserCustom) return success;
    optionalWorkspaceObject = parentItem->modelObject().model().addObject(openstudio::IdfObject(iddObjectToPasteType));
    if(!optionalWorkspaceObject) return success;
    optionalModelObject = parentItem->modelObject().model().getModelObject<ModelObject>(optionalWorkspaceObject->handle());
    if(!optionalModelObject) return success;
    ModelObject temp(parentItem->modelObject());
    if(OptionalParentObject tempParent = temp.optionalCast<ParentObject>())
    {
      success = optionalModelObject->setParent(*tempParent);
    }
  }
  mTreeViewWidget->loadModel(); // TODO this is also called in treeViewWidget::pasteObjects()

  return success; // not updating Qt model
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
  if (!index.isValid())
    return QModelIndex();

  TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
  TreeItem *parentItem = childItem->parent();

  if (parentItem == rootItem)
    return QModelIndex();

  ///! If we get here, parentItem cannot be NULL
  OS_ASSERT(parentItem);

  return createIndex(parentItem->row(), 0, parentItem);
}

bool TreeModel::removeRows(const QModelIndexList rowList, std::vector<openstudio::Handle>& handles)
{
  bool success = false;

  int position = rowList.at(0).row();
  int rows = rowList.size();
  QModelIndex parent = rowList.at(0).parent();
  TreeItem * parentItem = getItem(parent);
  std::vector<openstudio::IdfObject> objects;

  ///! OpenStudio model update...
  for(int i=0; i<rows; i++){
    TreeItem * treeItem = static_cast<TreeItem*>(rowList.at(i).internalPointer());
    openstudio::model::ModelObject object = treeItem->modelObject();
    objects = object.remove();
    if(!objects.empty()){
      success = true;
      for (const openstudio::IdfObject& object : objects)
      {
        handles.push_back(object.handle());
      }
    }
    // this list could mix parents and children, therefore
    // a parent may first be deleted and then we may
    // try to delete its child and fail since it has just
    // previously been deleted (i.e. don't break on a false)
  }

  return success; // not updating Qt model

  ///! Qt model update...
  beginRemoveRows(parent, position, position+rows-1);
  success = parentItem->removeChildren(position, rows);
  endRemoveRows();

  return success;
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
  TreeItem *parentItem;
  if (parent.column() > 0)
    return 0;

  if (!parent.isValid())
    parentItem = rootItem;
  else
    parentItem = static_cast<TreeItem*>(parent.internalPointer());

  return parentItem->childCount();
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  return false;

  if (role != Qt::EditRole)
    return false;

  TreeItem *item = getItem(index);
  bool result = item->setData(index.column(), value);

  if (result)
    emit dataChanged(index, index);

  return result;
}

bool TreeModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
  return false;

  if (role != Qt::EditRole || orientation != Qt::Horizontal)
    return false;

  bool result = rootItem->setData(section, value);

  if (result)
    emit headerDataChanged(orientation, section, section);

  return result;
}

void TreeModel::getObjectNames(const openstudio::model::ModelObject& object, QList<QVariant>& objectNames)
{
 
  
  IddObject iddObject = object.iddObject();
  QString objectName(iddObject.name().c_str());
  // object might have a name
  OptionalString optionalString = object.name();
  if(optionalString){
    QString name = optionalString.get().c_str();
    int firstIdx = name.indexOf(guidOpenCurlyBrace);
    int secondIdx = name.indexOf(guidCloseCurlyBrace);
    if(mMaskGUIDs && (firstIdx != -1) && (secondIdx != -1)){
      ///! QString::trimmed() won't work due to characters after the last curly brace
      unsigned offset = 0;
      if(firstIdx>0 && firstIdx<name.size() && name.at(firstIdx-1).isSpace()){
        offset++;
      }
      name.remove(firstIdx-offset,secondIdx-firstIdx+1+offset);
    }
    objectName += " (";
    objectName += name.toStdString().c_str();
    objectName += ")";
  }
  objectNames.push_back(objectName);
}

void TreeModel::setupModelData(const openstudio::model::ModelObject& object, TreeItem *treeItemParent)
{
  // get the idd object type
  QList<QVariant> objectNames;
  getObjectNames(object, objectNames);

  auto newTreeItem = new TreeItem(object, objectNames, treeItemParent);
  treeItemParent->appendChild(newTreeItem);

  // loop through each child
  // and parent it with the newly created TreeItem
  if( OptionalParentObject p = object.optionalCast<ParentObject>())
  {
    for (const ModelObject& child : p->children())
    {
      setupModelData(child, newTreeItem);
    }
  }
}

Qt::DropActions TreeModel::supportedDropActions() const
{
  return Qt::CopyAction | Qt::MoveAction;
}

QModelIndexList TreeModel::getPersistentIndexList() const
{
  return persistentIndexList();
}

bool TreeModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
  bool success = false;

  if(action != Qt::CopyAction) return success;

 TreeItem * parentItem = getItem(parent);
 OS_ASSERT(parentItem);

 if(data->hasText()){
    if(!data->hasFormat("ListWidget data")) return success;
    QString string = data->text();
    QStringList strings = string.split(",");
    openstudio::IddFile iddFile = mTreeViewWidget->getIddFile();
    boost::optional<openstudio::IddObject> optionalIddObject;
    
    openstudio::OptionalWorkspaceObject optionalWorkspaceObject;
    openstudio::model::OptionalModelObject optionalModelObject;
    openstudio::model::OptionalParentObject optionalParentObject;
    for(int i=0; i<strings.size(); i++)
    {
      optionalIddObject = iddFile.getObject(strings.at(i).toStdString());
      if(optionalIddObject)
      {
        openstudio::IdfObject idfObject(optionalIddObject->type());
        optionalWorkspaceObject = mTreeViewWidget->getModel().addObject(idfObject);
        if(optionalWorkspaceObject)
        {
          optionalModelObject = parentItem->modelObject().model().getModelObject<ModelObject>(optionalWorkspaceObject->handle());
          if(optionalModelObject)
          {
            ///! OpenStudio model update...
            ModelObject temp(parentItem->modelObject());
            if(OptionalParentObject tempParent = temp.optionalCast<ParentObject>())
            {
              success = optionalModelObject->setParent(*tempParent);
            }
            if(success)
            {
              success = mTreeViewWidget->getModel().order().insert(optionalWorkspaceObject->handle(), this->modelAtIndex(parent)->handle());
            } 
          }
        }
      }
    }
  }
  else{
    OS_ASSERT(mTreeViewWidget && mTreeViewWidget->getTreeView());
    QModelIndexList rowList;
    if(!mTreeViewWidget->getTreeView()->getSelectedRows(rowList)){
      return success;
    }

    //FIX THIS why is position unused? LER
    //int position = rowList.at(0).row();
    int rows = rowList.size();
    QModelIndex sourceParent = rowList.at(0).parent();

    TreeItem * parentItem = getItem(parent);

    ///! OpenStudio model update...
    for(int i=0; i<rows; i++){
      ModelObject temp(parentItem->modelObject());
      if(OptionalParentObject tempParent = temp.optionalCast<ParentObject>())
      {
        success = this->modelAtIndex(rowList.at(i))->setParent(*tempParent);
      }
    }
  }

  mTreeViewWidget->loadModel();
  mTreeViewWidget->emitModelDirty();

  return success; // not updating Qt model
}
void TreeModel::toggleGUIDs()
{
  mMaskGUIDs = !mMaskGUIDs;
}

} // namespace modeleditor
