/****************************************************************************
 **
 ** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef TREEMODEL_H
#define TREEMODEL_H
 
#include <QAbstractItemModel>
#include <QVariant>
 
#include <model/ParentObject.hpp>
#include <model/Building.hpp>
#include "ModelEditorAPI.hpp"

class QModelIndex;

namespace modeleditor
{

class TreeViewWidget;
class TreeItem;

class TreeModel : public QAbstractItemModel
{
  Q_OBJECT

public:
  TreeModel(openstudio::model::Model& model, TreeViewWidget * parent);
  TreeModel(TreeViewWidget * parent);
  ~TreeModel();

  void loadModel(openstudio::model::Model& model);
  QVariant data(const QModelIndex &index, int role) const;
  openstudio::model::OptionalModelObject modelAtIndex(const QModelIndex &index) const;
  Qt::ItemFlags flags(const QModelIndex &index) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
  QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
  QModelIndex parent(const QModelIndex &index) const;
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
  bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole);
  bool addRows(const QModelIndexList rowList, openstudio::IddObjectType type = openstudio::IddObjectType::UserCustom);
  bool removeRows(const QModelIndexList rowList, std::vector<openstudio::Handle>& handles);
  bool pasteRows(const QModelIndex& parentRow,   std::vector<openstudio::model::ModelObject>& modelObjectsToPaste);
  Qt::DropActions supportedDropActions() const;
  QModelIndexList getPersistentIndexList() const;
  void toggleGUIDs();

public slots:

signals:

protected:

private:
  void setupModelData(const openstudio::model::ModelObject& object, TreeItem *parent);
  TreeItem *getItem(const QModelIndex &index) const;
  bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
  bool insertRows(const QModelIndex row, const QModelIndexList rowList, openstudio::IddObjectType type);
  void getObjectNames(const openstudio::model::ModelObject& object, QList<QVariant>& objectNames);

  ///! QAbstractTableModel::parent() returns a QModelItemIndex, not the parent widget* as QObject::parent() does,
  ///! hence the parent* data member below
  TreeViewWidget * mTreeViewWidget;
  TreeItem *rootItem;
  bool mMaskGUIDs;
};

} // namespace modeleditor

#endif
