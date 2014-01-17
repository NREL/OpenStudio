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

//#include <algorithm>

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

QSharedPointer<QWidget> OSGridController::widgetAt(int i, int j)
{
  QSharedPointer<QWidget> result;

  result = QSharedPointer<QWidget>(new QLabel());
  
  //std::vector<model::ModelObject> modelObjects = m_model.getModelObjectsByType(m_iddObjectType).sortSomewayortheother(); TODO
  //std::vector<model::ModelObject> modelObjects = m_model.getModelObjects<m_iddObjectType>();

  //model::ModelObject mo = modelObjects[i];

  if( m_columnTypes.at(j) == OSGridController::COMBOBOX )
  {
  }

  return result;
}

int OSGridController::rowCount() const
{
  return 5; // TODO
}
   
int OSGridController::columnCount() const
{
  return 5; // TODO
}

} // openstudio

