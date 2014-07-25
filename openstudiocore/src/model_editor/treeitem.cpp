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

/*
  treeitem.cpp

  A container for items of data supplied by the simple tree model.
*/

#include <QStringList>

#include <model_editor/treeitem.h>

namespace modeleditor
{

TreeItem::TreeItem(const openstudio::model::ModelObject& object, const QList<QVariant> &data, TreeItem *parent)
: mObject(object)
{
  mParentItem = parent;
  mItemData = data;
}

TreeItem::~TreeItem()
{
  qDeleteAll(mChildItems);
}

void TreeItem::appendChild(TreeItem *item)
{
  mChildItems.append(item);
}

TreeItem *TreeItem::child(int row)
{
  return mChildItems.value(row);
}

int TreeItem::childCount() const
{
  return mChildItems.count();
}

int TreeItem::columnCount() const
{
  return mItemData.count();
}

QVariant TreeItem::data(int column) const
{
  return mItemData.value(column);
}

openstudio::model::ModelObject TreeItem::modelObject() const
{
  return mObject;
}

TreeItem *TreeItem::parent()
{
  return mParentItem;
}

int TreeItem::row() const
{
  if (mParentItem)
    return mParentItem->mChildItems.indexOf(const_cast<TreeItem*>(this));

  return 0;
}

bool TreeItem::setData(int column, const QVariant &value)
{
  if (column < 0 || column >= mItemData.size())
    return false;

  mItemData[column] = value;
    return true;
}

bool TreeItem::setData(openstudio::model::ModelObject &data)
{
  mObject = data;
  return true;
}

bool TreeItem::insertChildren(openstudio::model::ModelObject& object, const QList<QVariant> &data, int position, int count, int columns)
{
  if (position < 0 || position > mChildItems.size())
    return false;

  for (int row = 0; row < count; ++row) {
    auto item = new TreeItem(object, data, this);
    mChildItems.insert(position, item);
  }

  return true;
}

bool TreeItem::removeChildren(int position, int count)
{
  if (position < 0 || position + count > mChildItems.size())
    return false;

  for (int row = 0; row < count; ++row)
    delete mChildItems.takeAt(position);

  return true;
}

bool TreeItem::moveChildren(int position, int count)
{
  if (position < 0 || position + count > mChildItems.size())
    return false;

  for (int row = 0; row < count; ++row)
    delete mChildItems.takeAt(position);

  return true;
}

} // namespace modeleditor
