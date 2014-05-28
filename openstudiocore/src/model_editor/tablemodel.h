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

#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>
#include <QPair>

#include <model/Model.hpp>
#include <model/Building.hpp>
#include "ModelEditorAPI.hpp"

namespace modeleditor
{
class ClassViewWidget;

//! [0]
class MODELEDITOR_API TableModel : public QAbstractTableModel
{
  Q_OBJECT

public:
  TableModel(openstudio::WorkspaceObjectVector& objects, ClassViewWidget * parent);
  TableModel(ClassViewWidget * parent);
  ~TableModel();
  void loadObjects(openstudio::WorkspaceObjectVector& objects);
  void setModel( openstudio::model::Model& model );
  int rowCount(const QModelIndex &parent) const;
  int columnCount(const QModelIndex &parent) const;
  QVariant data(const QModelIndex &index, int role) const;
  openstudio::OptionalWorkspaceObject objectAtIndex(const QModelIndex &index) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const;
  Qt::ItemFlags flags(const QModelIndex &index) const;
  bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole);
  bool insertRows(std::vector<openstudio::WorkspaceObject> wsObjects, const QModelIndexList& rowList);
  bool insertRows(std::vector<openstudio::WorkspaceObject> wsObjects, const QModelIndex& row, const QModelIndexList& rowList);
  bool insertRows(std::vector<openstudio::WorkspaceObject> wsObjects, int position, int rows, const QModelIndex &index=QModelIndex());
  bool removeRows(int position, int rows, const QModelIndex &index=QModelIndex());
  bool removeRows(const QModelIndexList& rowList);
  bool moveRows(const QModelIndex& row, const QModelIndexList& rowList);
  Qt::DropActions supportedDropActions() const;
  virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);
  void toggleGUIDs();

public slots:

signals:

protected:

private:
  void setupModelData(openstudio::WorkspaceObjectVector& objects);
  bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
  void sort(openstudio::WorkspaceObjectVector& objects, int column = 0, Qt::SortOrder order = Qt::AscendingOrder);
  ///! std::sort comparison functions
  bool cmpAscendIddName(const openstudio::WorkspaceObject& object1, const openstudio::WorkspaceObject& object2);
  bool cmpDescendIddName(const openstudio::WorkspaceObject& object1, const openstudio::WorkspaceObject& object2);
  bool cmpAscendIdfName(const openstudio::WorkspaceObject& object1, const openstudio::WorkspaceObject& object2);
  bool cmpDescendIdfName(const openstudio::WorkspaceObject& object1, const openstudio::WorkspaceObject& object2);
  bool cmpAscendIndex(const openstudio::WorkspaceObject& object1, const openstudio::WorkspaceObject& object2);
  bool cmpDescendIndex(const openstudio::WorkspaceObject& object1, const openstudio::WorkspaceObject& object2);

  int mSortColumn;
  Qt::SortOrder mSortOrder;
  openstudio::WorkspaceObjectVector mObjects;
  ///! QAbstractTableModel::parent() returns a QModelItemIndex, not the parent widget* as QObject::parent() does,
  ///! hence the parent* data member below
  ClassViewWidget * mClassViewWidget;
  openstudio::model::Model mModel;
  bool mMaskGUIDs;
};
//! [0]

} // namespace modeleditor

#endif
