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

#include <QMimeData>

#include "ListWidget.hpp"

namespace modeleditor
{
  QString modeleditor::ListWidget::mMimeType = "ListWidget data";

ListWidget::ListWidget(QWidget * parent)
  : QListWidget(parent)
{
}

ListWidget::~ListWidget()
{
}

QMimeData * ListWidget::mimeData(const QList<QListWidgetItem *> items) const
{
  QMimeData * mimeData = QListWidget::mimeData(items);
  mimeData->setData(getMimeType(),QByteArray());

  QString string;
  for(int i=0; i<items.size(); i++){
    QListWidgetItem * item = items.at(i);
    string += item->text();
    if(i < items.size() -1){
      string += ",";
    }
  }
  mimeData->setText(string);

  return mimeData;
}

///! Slots

} // namespace modeleditor