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
