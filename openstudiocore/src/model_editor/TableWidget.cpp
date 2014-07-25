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

#include "TableWidget.hpp"

using namespace openstudio::model;
using namespace openstudio;

namespace modeleditor
{

TableWidget::TableWidget(QWidget *parent)
  : QTableWidget(parent)
{
}

TableWidget::~TableWidget()
{
}

void TableWidget::populateTable(const openstudio::WorkspaceObject& obj)
{
  clear();
  setCurrentCell(0,0);
  setRowCount(1);

  openstudio::IddObject iddObj = obj.iddObject();
  for(unsigned i=0; i<obj.numFields(); ++i){
    bool defaultText = false;
    OptionalString val = obj.getString(i);
    if(!val){
      defaultText = true;
      val = obj.getString(i, true);
      // if still no value it is just blank
      if (!val){
        val = "";
      }
    }
    // setItem causes QTableWIdget to take ownership of newItem
    QTableWidgetItem * newItem = new QTableWidgetItem((*val).c_str());
    if(defaultText){
      newItem->setTextColor(Qt::gray);
    }
    setItem(rowCount()-1, 1, newItem);

    openstudio::OptionalIddField iddField = iddObj.getField(i);
    if(!iddField){
      // log error
    }
    else{
      *val = iddField->name();
    }
    // setItem causes QTableWIdget to take ownership of newItem
    newItem = new QTableWidgetItem((*val).c_str());
    setItem(rowCount()-1, 0, newItem);

    setRowCount(rowCount() + 1);
  }

  resizeColumnsToContents();
  resizeRowsToContents();
}

} // namespace modeleditor
