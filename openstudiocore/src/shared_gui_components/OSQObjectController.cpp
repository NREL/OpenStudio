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

#include "OSQObjectController.hpp"

#include "../utilities/core/Assert.hpp"

#include <algorithm>

namespace openstudio {
  

OSQObjectController::OSQObjectController()
  : QObject()
{
}

OSQObjectController::~OSQObjectController()
{
  auto it = m_objects.begin();
  auto itend = m_objects.end();
  for( ; it != itend; ++it)
  {
    if( *it )
    {
      delete *it;
    }
  }  
}

void OSQObjectController::addQObject(QObject * object)
{
  m_objects.push_back(object);

  bool isConnected = connect(object,SIGNAL(destroyed(QObject *)),this,SLOT(onObjectDestroyed(QObject *)));
  OS_ASSERT(isConnected);
}

void OSQObjectController::onObjectDestroyed(QObject * object)
{
  auto it = find(m_objects.begin(),m_objects.end(),object);

  if( it != m_objects.end() )
  {
    *it = nullptr;
  }
}


} // openstudio

