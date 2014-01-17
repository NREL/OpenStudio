/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

#include "OSGridController.hpp"

#include <model/Model_impl.hpp>
#include <model/ModelObject_impl.hpp>

#include <utilities/core/Assert.hpp>

#include <algorithm>

#include <QWidget>

namespace openstudio {

OSGridController::OSGridController()
  : QObject()
{
}

OSGridController::OSGridController(IddObjectType iddObjectType, model::Model model)
  : QObject(),
  m_iddObjectType(iddObjectType),
  m_model(model)
{
}

OSGridController::~OSGridController()
{
}

QSharedPointer<OSListItem> OSGridController::itemAt(int i, int j)
{
  QSharedPointer<OSListItem> result;

  //std::vector<model::ModelObject> modelObjects = m_model.getModelObjectsByType(m_iddObjectType).sortSomewayortheother(); TODO
  //std::vector<model::ModelObject> modelObjects = m_model.getModelObjects<m_iddObjectType>();

  //model::ModelObject mo = modelObjects[i];

  //if( m_columnType[j] == COMBO )
  //{
  //  result = QSharedPointer<OSListComboItem>(new OSListComboItem(mo,.....));
  //}
  //else if( m_columnType[j] == DOUBLE )
  //{
  //}

  return result;
}

int OSGridController::rowCount() const
{
  return 0; // TODO
}
   
int OSGridController::columnCount() const
{
  return 0; // TODO
}



QWidget * OSGridItemDelegate::view(QSharedPointer<OSListItem> dataSource)
{
  QWidget * result = NULL;

  //if(QSharedPointer<OSListComboItem> comboItem = dataSource.objectCast<OSListComboItem>())
  //{
  //  OSComboBox2 * comboBox = new OSComboBox2();

    //comboBox.bind(comboItem->modelObject(),
    //              comboItem->choices(),
    //              comboItem->getter(),
    //              comboItem->setter());

    //result = comboBox;

    // void bind(model::ModelObject& modelObject,
    //           ChoicesGetter choices,
    //           StringGetter get,
    //           boost::optional<StringSetter> set=boost::none,
    //           boost::optional<NoFailAction> reset=boost::none,
    //           boost::optional<BasicQuery> isDefaulted=boost::none);

    // some bind/connection between combobox and comboItem
 // }
  //else if(QSharedPointer<OSDoubleItem> doubleItem  .... )
  //{
  //}

  return result;
}

} // openstudio

