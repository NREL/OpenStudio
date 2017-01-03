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
