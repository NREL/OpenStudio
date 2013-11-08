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

#include "RefrigerationController.hpp"
#include "RefrigerationScene.hpp"
#include "RefrigerationGraphicsItems.hpp"
#include "OSAppBase.hpp"
#include "../model/Model.hpp"
#include "../model/RefrigerationSystem.hpp"
#include "../model/RefrigerationSystem_Impl.hpp"
#include "../utilities/core/Compare.hpp"

namespace openstudio {

RefrigerationController::RefrigerationController()
  : QObject()
{
  m_refrigerationScene = QSharedPointer<RefrigerationScene>(new RefrigerationScene());

  m_refrigerationSystemGridItem = QSharedPointer<RefrigerationSystemGridItem>(new RefrigerationSystemGridItem());

  m_refrigerationSystemListController = QSharedPointer<RefrigerationSystemListController>(new RefrigerationSystemListController());

  m_refrigerationSystemGridItem->setListController(m_refrigerationSystemListController);

  m_refrigerationSystemGridItem->setDelegate(QSharedPointer<RefrigerationSystemItemDelegate>(new RefrigerationSystemItemDelegate()));

  m_refrigerationScene->addItem(m_refrigerationSystemGridItem.data());
}

QSharedPointer<RefrigerationScene> RefrigerationController::refrigerationScene() const
{
  return m_refrigerationScene;
}

QSharedPointer<RefrigerationSystemListController> RefrigerationController::refrigerationSystemListController() const
{
  return m_refrigerationSystemListController;
}

QSharedPointer<OSListItem> RefrigerationSystemListController::itemAt(int i)
{
  return QSharedPointer<RefrigerationSystemListItem>(new RefrigerationSystemListItem(this));
}

int RefrigerationSystemListController::count()
{
  return systems().size();
}

RefrigerationSystemListItem::RefrigerationSystemListItem(OSListController * listController)
  : OSListItem(listController)
{
}

void RefrigerationSystemListController::createNewSystem()
{
  if( boost::optional<model::Model> model = OSAppBase::instance()->currentModel() )
  {
    model::RefrigerationSystem system(model.get());

    std::vector<model::RefrigerationSystem> _systems = systems();

    int i = 0;

    for( std::vector<model::RefrigerationSystem>::const_iterator it = _systems.begin();
         it != _systems.end();
         it++ )
    {
      if( *it == system )
      {
        break;
      }

      i++;
    }

    emit itemInserted(i);
  }
}

std::vector<model::RefrigerationSystem> RefrigerationSystemListController::systems() const
{
  std::vector<model::RefrigerationSystem> result;

  if( boost::optional<model::Model> model = OSAppBase::instance()->currentModel() )
  {
    result = model->getModelObjects<model::RefrigerationSystem>();
  }

  std::sort(result.begin(), result.end(), WorkspaceObjectNameLess());

  return result;
}

} // openstudio

