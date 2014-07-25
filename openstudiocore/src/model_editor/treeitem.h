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

#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>
#include <QVariant>

#include <model/Building.hpp>
#include "ModelEditorAPI.hpp"

namespace modeleditor
{

class TreeItem
{
public:
  TreeItem(const openstudio::model::ModelObject& object, const QList<QVariant> &data, TreeItem *parent = nullptr);
  ~TreeItem();

  void appendChild(TreeItem *child);
  bool insertChildren(openstudio::model::ModelObject& object, const QList<QVariant> &data, int position, int count, int columns);
  bool removeChildren(int position, int count);
  bool moveChildren(int position, int count);
  TreeItem *child(int row);
  int childCount() const;
  int columnCount() const;
  QVariant data(int column) const;
  openstudio::model::ModelObject modelObject() const;
  int row() const;
  bool setData(int column, const QVariant &value);
  bool setData(openstudio::model::ModelObject &data);
  TreeItem * parent();

private:
  QList<TreeItem*> mChildItems;
  QList<QVariant> mItemData;
  TreeItem *mParentItem;
  openstudio::model::ModelObject mObject;

};

} // namespace modeleditor

#endif // TREEITEM_H
