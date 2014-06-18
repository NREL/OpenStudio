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

#include <QKeyEvent>
#include <QTableView>

#include "TableView.hpp"

namespace modeleditor
{

TableView::TableView(QWidget *parent)
  : QTableView(parent)
{
}

TableView::~TableView()
{
}

void TableView::enterEvent(QEvent * event)
{
  emit eventEnter();
}

void TableView::leaveEvent(QEvent * event)
{
  emit eventLeave();
}

void TableView::keyReleaseEvent(QKeyEvent * event)
{
  if(event->key() == Qt::Key_Up || event->key() == Qt::Key_Down){
    emit eventUpDnKeyRelease();
  }
}

bool TableView::getSelectedRows(QModelIndexList& rowList)
{
 // bool success = false;
  QItemSelectionModel * selectionMod = nullptr;
  selectionMod = selectionModel();
  if(selectionMod){
    //success = true;
    rowList = selectionMod->selectedRows();
  }
  return !rowList.empty();
}

bool TableView::hasSelectedRows()
{
  QModelIndexList rowList;
  return getSelectedRows(rowList);
}

} // namespace modeleditor
