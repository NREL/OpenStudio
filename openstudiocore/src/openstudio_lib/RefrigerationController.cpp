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

QSharedPointer<OSListItem> RefrigerationSystemListController::itemAt(int i)
{
  return QSharedPointer<RefrigerationSystemListItem>(new RefrigerationSystemListItem(this));
}

int RefrigerationSystemListController::count()
{
  return 6;
}

RefrigerationSystemListItem::RefrigerationSystemListItem(OSListController * listController)
  : OSListItem(listController)
{
}

} // openstudio

