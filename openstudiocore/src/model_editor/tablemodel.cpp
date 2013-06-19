/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QMimeData>

#include <vector>
#include <algorithm>

#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include <model_editor/ClassViewWidget.hpp>
#include <model_editor/TableView.hpp>
#include <model/Model.hpp>
#include <utilities/idf/IdfFile.hpp>
#include <utilities/idf/WorkspaceObjectOrder.hpp>

#include <model_editor/tablemodel.h>

#define guidOpenCurlyBrace '{'
#define guidCloseCurlyBrace '}'

using namespace openstudio::model;
using namespace openstudio;

namespace modeleditor
{

//! [0]
TableModel::TableModel(openstudio::WorkspaceObjectVector& objects, ClassViewWidget * parent)
  : QAbstractTableModel(parent),
  mSortColumn(2),
  mSortOrder(Qt::AscendingOrder),
  mObjects(objects),
  mClassViewWidget(parent),
  mMaskGUIDs(false)
{
 
}
//! [0]

TableModel::TableModel(ClassViewWidget * parent)
  : QAbstractTableModel(parent),
    mClassViewWidget(parent),
    mMaskGUIDs(false)
{
  
}

TableModel::~TableModel()
{
}

void TableModel::loadObjects(openstudio::WorkspaceObjectVector& objects)
{
  beginResetModel();
  mObjects = objects;
  sort(mObjects, mSortColumn, mSortOrder);
  endResetModel();
}

void TableModel::setModel( openstudio::model::Model& model )
{
  mModel = model;
}

void TableModel::sort(int column, Qt::SortOrder sortOrder)
{
  mSortColumn = column;
  mSortOrder = sortOrder;

  emit layoutAboutToBeChanged();
  sort(mObjects,column,sortOrder);
  emit layoutChanged();
}

void TableModel::sort(openstudio::WorkspaceObjectVector& objects, int column, Qt::SortOrder sortOrder){

  switch(column)
  {
    case 0:
      if(sortOrder == Qt::AscendingOrder){
        std::sort(objects.begin(), objects.end(), boost::bind(&modeleditor::TableModel::cmpAscendIddName,this,_1,_2));
      }
      else if(sortOrder == Qt::DescendingOrder){
        std::sort(objects.begin(), objects.end(), boost::bind(&modeleditor::TableModel::cmpDescendIddName,this,_1,_2));
      }
      break;
    case 1:
      if(sortOrder == Qt::AscendingOrder){
        std::sort(objects.begin(), objects.end(), boost::bind(&modeleditor::TableModel::cmpAscendIdfName,this,_1,_2));
      }
      else if(sortOrder == Qt::DescendingOrder){
        std::sort(objects.begin(), objects.end(), boost::bind(&modeleditor::TableModel::cmpDescendIdfName,this,_1,_2));
      }
      break;
    case 2:
      mModel = mClassViewWidget->getModel();
      if(sortOrder == Qt::AscendingOrder){
        std::sort(objects.begin(), objects.end(), boost::bind(&modeleditor::TableModel::cmpAscendIndex,this,_1,_2));
      }
      else if(sortOrder == Qt::DescendingOrder){
        std::sort(objects.begin(), objects.end(), boost::bind(&modeleditor::TableModel::cmpDescendIndex,this,_1,_2));
      }
      break;
  }
}

bool TableModel::cmpAscendIddName(const openstudio::WorkspaceObject& object1, const openstudio::WorkspaceObject& object2)
{
  return (object1.iddObject().name() > object2.iddObject().name());
}

bool TableModel::cmpDescendIddName(const openstudio::WorkspaceObject& object1, const openstudio::WorkspaceObject& object2)
{
  return (object1.iddObject().name() < object2.iddObject().name());
}

bool TableModel::cmpAscendIdfName(const openstudio::WorkspaceObject& object1, const openstudio::WorkspaceObject& object2)
{
  OptionalString idfName1 = object1.idfObject().name();
  OptionalString idfName2 = object2.idfObject().name();

  if(!idfName1 && !idfName2){
    return false;
  }
  else if(!idfName1 && idfName2){
    return false;
  }
  else if(idfName1 && !idfName2){
    return true;
  }
  else{
    return (idfName1.get() > idfName2.get());
  }
}

bool TableModel::cmpDescendIdfName(const openstudio::WorkspaceObject& object1, const openstudio::WorkspaceObject& object2)
{
  OptionalString idfName1 = object1.idfObject().name();
  OptionalString idfName2 = object2.idfObject().name();

  if(!idfName1 && !idfName2){
    return false;
  }
  else if(!idfName1 && idfName2){
    return false;
  }
  else if(idfName1 && !idfName2){
    return true;
  }
  else{
    return (idfName1.get() < idfName2.get());
  }
}

bool TableModel::cmpAscendIndex(const openstudio::WorkspaceObject& object1, const openstudio::WorkspaceObject& object2)
{
  openstudio::WorkspaceObjectOrder order = mModel.order();
  openstudio::Handle handle1 = object1.handle();
  openstudio::Handle handle2 = object2.handle();
  Q_ASSERT(order.inOrder(handle1));
  Q_ASSERT(order.inOrder(handle2));
  openstudio::OptionalUnsigned indexInOrder1 = order.indexInOrder(handle1);
  openstudio::OptionalUnsigned indexInOrder2 = order.indexInOrder(handle2);
  Q_ASSERT(indexInOrder1);
  Q_ASSERT(indexInOrder2);

  return (indexInOrder1.get() > indexInOrder2.get());
}

bool TableModel::cmpDescendIndex(const openstudio::WorkspaceObject& object1, const openstudio::WorkspaceObject& object2)
{
  openstudio::WorkspaceObjectOrder order = mModel.order();
  openstudio::Handle handle1 = object1.handle();
  openstudio::Handle handle2 = object2.handle();
  Q_ASSERT(order.inOrder(handle1));
  Q_ASSERT(order.inOrder(handle2));
  openstudio::OptionalUnsigned indexInOrder1 = order.indexInOrder(handle1);
  openstudio::OptionalUnsigned indexInOrder2 = order.indexInOrder(handle2);
  Q_ASSERT(indexInOrder1);
  Q_ASSERT(indexInOrder2);

  return (indexInOrder1.get() < indexInOrder2.get());
}

//! [1]
int TableModel::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return mObjects.size();
}

int TableModel::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return 3;
}
//! [1]

//! [2]
QVariant TableModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();

  if (index.row() >= static_cast<int>(mObjects.size()) || index.row() < 0)
    return QVariant();

  if (role == Qt::DisplayRole) {
    openstudio::WorkspaceObject object = mObjects.at(index.row());
    if (index.column() == 0){
      IddObject iddObject = object.iddObject();
      QString iddName(iddObject.name().c_str());
      return iddName;
    }
    else if (index.column() == 1){
      // object might have a name  
      OptionalString optionalString = object.name();
      QString idfName;
      if(optionalString){
        idfName = optionalString.get().c_str();
        int firstIdx = idfName.indexOf(guidOpenCurlyBrace);
        int secondIdx = idfName.indexOf(guidCloseCurlyBrace);
        if(mMaskGUIDs &&(firstIdx != -1) && (secondIdx != -1)){
          ///! QString::trimmed() won't work due to characters after the last curly brace
          unsigned offset = 0;
          if(firstIdx>0 && firstIdx<idfName.size() && idfName.at(firstIdx-1).isSpace()){
            offset++;
          }
          idfName.remove(firstIdx-offset,secondIdx-firstIdx+1+offset);
        }
      }
      return idfName; 
    }
    else if (index.column() == 2){
      openstudio::Handle handle = object.handle();
      openstudio::WorkspaceObjectOrder order = mClassViewWidget->getModel().order();
      Q_ASSERT(order.inOrder(handle));
      openstudio::OptionalUnsigned indexInOrder = order.indexInOrder(handle);
      Q_ASSERT(indexInOrder);
      if(indexInOrder){
        return indexInOrder.get();
      }
      else{
        return -1;
      }
    }
  }

  return QVariant();
}
//! [2]

openstudio::OptionalWorkspaceObject TableModel::objectAtIndex(const QModelIndex &index) const
{
  Q_ASSERT(index.isValid());
  Q_ASSERT(index.row() >= 0 && index.row() < static_cast<int>(mObjects.size()));

  return mObjects.at(index.row());
}

//! [3]
QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role != Qt::DisplayRole)
    return QVariant();

  if (orientation == Qt::Horizontal) {
    switch (section) {
      case 0:
        return tr("Object");

      case 1:
        return tr("Name");

      case 2:
        return tr("Index");

      default:
        return QVariant();
    }
  }
  return QVariant();
}
//! [3]

bool TableModel::insertRows(std::vector<openstudio::WorkspaceObject> wsObjects, const QModelIndexList& rowList)
{
  int position = rowList.at(0).row();
  int rows = rowList.size();

  return insertRows(wsObjects, position, rows);
}

bool TableModel::insertRows(std::vector<openstudio::WorkspaceObject> wsObjects, const QModelIndex& row, const QModelIndexList& rowList)
{
  int position = rowList.at(0).row();
  int rows = rowList.size();

  return insertRows(wsObjects, position, rows, row);
}

//! [4]
bool TableModel::insertRows(std::vector<openstudio::WorkspaceObject> wsObjects, int position, int rows, const QModelIndex &index)
{
  beginInsertRows(QModelIndex(), position+1, position+rows);

  std::vector<openstudio::WorkspaceObject>::iterator it = mObjects.begin()+position+1;
  mObjects.insert(it,wsObjects.begin(),wsObjects.end());

  endInsertRows();
  return true;
}
//! [4]

bool TableModel::moveRows(const QModelIndex& row, const QModelIndexList& rowList)
{
  bool success = false;

  int rows = rowList.size();
  int position = rowList.at(0).row();

  ///! Ensure that drag != drop
  for(int i=0; i<rows; i++){
    if(rowList.at(i).row() == row.row()){
      return success;
    }
  }

  openstudio::WorkspaceObjectVector temp;
  for(int i=0; i<rows; i++){
    temp.push_back(mObjects.at(rowList.at(i).row()));
  }

  beginMoveRows(QModelIndex(), position, position+rows-1, QModelIndex(), row.row());

  ///! std::list could use splice, but we are working with a vector
  std::vector<openstudio::WorkspaceObject>::iterator it = mObjects.begin()+row.row();
  mObjects.insert(it,temp.begin(),temp.end());

  if(position > row.row()){
    it = mObjects.begin()+position+rows;
  }
  else{
    it = mObjects.begin()+position;
  }
  mObjects.erase(it,it+rows);

  endMoveRows();

  return true;
}

bool TableModel::removeRows(const QModelIndexList& rowList)
{
  int position = rowList.at(0).row();
  int rows = rowList.size();

  return removeRows(position, rows);
}

//! [5]
bool TableModel::removeRows(int position, int rows, const QModelIndex &index)
{
  Q_UNUSED(index);    
  beginRemoveRows(QModelIndex(), position, position+rows-1);

  std::vector<openstudio::WorkspaceObject>::iterator it = mObjects.begin()+position;
  mObjects.erase(it,it+rows);

  endRemoveRows();
  return true;
}
//! [5]

//! [6]
bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  if(index.isValid() && role == Qt::EditRole){
    //int row = index.row();

    QPair<QString, QString> p; // = mObjects.value(row);
    if (index.column() == 0)
      p.first = value.toString();
    else if (index.column() == 1)
      p.second = value.toString();
    else
      return false;
    
    //mObjects.replace(row, p);
    emit(dataChanged(index, index));

    return true;
  }
  return false;
}
//! [6]

//! [7]
Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
  if (index.isValid())
    return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
  else
    return 0;
}
//! [7]

void TableModel::setupModelData(openstudio::WorkspaceObjectVector& objects)
{
  // loop through each object
  BOOST_FOREACH(const openstudio::WorkspaceObject& object, objects){
    // get the idd object type
    openstudio::IddObject iddObject = object.iddObject();
    QString iddName(iddObject.name().c_str());
    // object might have a name  
    OptionalString temp = object.name();
    QString idfName;
    if(temp){
      idfName = (*temp).c_str();
    }
    QModelIndex idx = this->index(0, 0, QModelIndex());
    setData(idx, iddName, Qt::EditRole);
    idx = this->index(0, 1, QModelIndex());
    setData(idx, idfName, Qt::EditRole);
  }
}

Qt::DropActions TableModel::supportedDropActions() const
{
  return Qt::CopyAction | Qt::MoveAction;
}

bool TableModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
  bool success = false;

  if(action != Qt::CopyAction) return success;

  if(data->hasText()){
    if(!data->hasFormat("ListWidget data")) return success;
    QString string = data->text();
    QStringList strings = string.split(",");
    for(int i=0; i<strings.size(); i++){
      openstudio::IddFile iddFile = mClassViewWidget->getIddFile();
      boost::optional<openstudio::IddObject> optionalIddObject = iddFile.getObject(strings.at(i).toStdString());
      if(optionalIddObject){
        openstudio::IdfObject idfObject = openstudio::IdfObject(optionalIddObject->type());
        openstudio::OptionalWorkspaceObject optionalWorkspaceObject = mClassViewWidget->getModel().addObject(idfObject);
        if(optionalWorkspaceObject){
          success = mClassViewWidget->getModel().order().insert(optionalWorkspaceObject->handle(), this->objectAtIndex(parent)->handle());
        }
      }
    }
    mClassViewWidget->loadModel();
  }
  else{
    Q_ASSERT(mClassViewWidget && mClassViewWidget->getTableView());
    QModelIndexList rowList;
    if(!mClassViewWidget->getTableView()->getSelectedRows(rowList)){
      return success;
    }

    int rows = rowList.size();

    openstudio::WorkspaceObjectOrder order = mClassViewWidget->getModel().order();

    ///! OpenStudio model update...
    for(int i=0; i<rows; i++){
      if(!parent.isValid()) continue;
      success = order.move(mObjects.at(rowList.at(i).row()).handle(),this->objectAtIndex(parent)->handle());
      if(!success) return success;
    }
  }

  ///! Qt model update...
  //success = moveRows(parent, rowList);

  ///! sort Qt view by workspace index
  sort(2, Qt::DescendingOrder);

  mClassViewWidget->emitModelDirty();

  return success;
}

void TableModel::toggleGUIDs()
{
  mMaskGUIDs = !mMaskGUIDs;
}

} // namespace modeleditor
