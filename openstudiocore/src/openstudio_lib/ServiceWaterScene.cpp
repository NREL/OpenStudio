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

#include "ServiceWaterScene.hpp"
#include "GridItem.hpp"
#include "ServiceWaterGridItems.hpp"
#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/ModelObject.hpp"
#include "../model/ModelObject_Impl.hpp"
#include "../model/HVACComponent.hpp"
#include "../model/HVACComponent_Impl.hpp"
#include "../model/WaterUseConnections.hpp"
#include "../model/WaterUseConnections_Impl.hpp"
#include "../model/WaterUseEquipment.hpp"
#include "../model/WaterUseEquipment_Impl.hpp"
#include <QTimer>

namespace openstudio {

ServiceWaterScene::ServiceWaterScene(const model::Model & model)
  : GridScene(),
    m_dirty(true),
    m_model(model)
{
  connect( m_model.getImpl<openstudio::model::detail::Model_Impl>().get(),
           SIGNAL(addWorkspaceObject(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl>, const openstudio::IddObjectType&, const openstudio::UUID&)),
           this,
           SLOT(onAddedWorkspaceObject(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl>)) );

  connect( m_model.getImpl<openstudio::model::detail::Model_Impl>().get(),
           SIGNAL(removeWorkspaceObject(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl>, const openstudio::IddObjectType&, const openstudio::UUID&)),
           this,
           SLOT(onRemovedWorkspaceObject(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl>)) );

  layout();
}

void ServiceWaterScene::layout()
{
  QList<QGraphicsItem *> itemList = items();
  for( QList<QGraphicsItem *>::iterator it = itemList.begin(); 
       it < itemList.end(); 
       ++it )
  {
    removeItem(*it);
    delete *it;
  }

  ServiceWaterItem * backgroundItem = new ServiceWaterItem(this);

  Q_UNUSED(backgroundItem);
}

model::Model ServiceWaterScene::model() const
{
  return m_model;
}

void ServiceWaterScene::onAddedWorkspaceObject(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> wPtr )
{
  model::detail::WaterUseConnections_Impl* hvac_impl = dynamic_cast<model::detail::WaterUseConnections_Impl*>(wPtr.get());
  if(hvac_impl)
  {
    m_dirty = true;

    QTimer::singleShot(0,this,SLOT(layout()));
  }
}

void ServiceWaterScene::onRemovedWorkspaceObject(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> wPtr)
{
  model::detail::WaterUseConnections_Impl* hvac_impl = dynamic_cast<model::detail::WaterUseConnections_Impl*>(wPtr.get());
  if(hvac_impl)
  {
    m_dirty = true;

    QTimer::singleShot(0,this,SLOT(layout()));
  }
}

WaterUseConnectionsDetailScene::WaterUseConnectionsDetailScene(const model::WaterUseConnections & waterUseConnections)
  : GridScene(),
    m_dirty(true),
    m_waterUseConnections(waterUseConnections)
{
  model::Model model = m_waterUseConnections.model();

  connect( model.getImpl<openstudio::model::detail::Model_Impl>().get(),
           SIGNAL(addWorkspaceObject(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl>, const openstudio::IddObjectType&, const openstudio::UUID&)),
           this,
           SLOT(onAddedWorkspaceObject(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl>)) );

  connect( model.getImpl<openstudio::model::detail::Model_Impl>().get(),
           SIGNAL(removeWorkspaceObject(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl>, const openstudio::IddObjectType&, const openstudio::UUID&)),
           this,
           SLOT(onRemovedWorkspaceObject(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl>)) );

  layout();
}

model::WaterUseConnections WaterUseConnectionsDetailScene::waterUseConnections() const
{
  return m_waterUseConnections;
}

void WaterUseConnectionsDetailScene::layout()
{
  QList<QGraphicsItem *> itemList = items();
  for( QList<QGraphicsItem *>::iterator it = itemList.begin(); 
       it < itemList.end(); 
       ++it )
  {
    removeItem(*it);
    delete *it;
  }

  WaterUseConnectionsDetailItem * backgroundItem = new WaterUseConnectionsDetailItem(this);

  Q_UNUSED(backgroundItem);
}

void WaterUseConnectionsDetailScene::onAddedWorkspaceObject(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> wPtr )
{
  model::detail::WaterUseEquipment_Impl * hvac_impl = dynamic_cast<model::detail::WaterUseEquipment_Impl*>(wPtr.get());
  if(hvac_impl)
  {
    m_dirty = true;

    QTimer::singleShot(0,this,SLOT(layout()));
  }
}

void WaterUseConnectionsDetailScene::onRemovedWorkspaceObject(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> wPtr)
{
  model::detail::WaterUseEquipment_Impl * hvac_impl = dynamic_cast<model::detail::WaterUseEquipment_Impl*>(wPtr.get());
  if(hvac_impl)
  {
    m_dirty = true;

    QTimer::singleShot(0,this,SLOT(layout()));
  }
}

} // openstudio

