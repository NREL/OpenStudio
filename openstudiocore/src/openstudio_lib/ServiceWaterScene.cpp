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

#include "ServiceWaterScene.hpp"
#include "GridItem.hpp"
#include "ServiceWaterGridItems.hpp"
#include "OSAppBase.hpp"
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
  //m_model.getImpl<model::detail::Model_Impl>().get()->addWorkspaceObjectPtr.connect<ServiceWaterScene, &ServiceWaterScene::onAddedWorkspaceObject>(this);
  connect(OSAppBase::instance(), &OSAppBase::workspaceObjectAddedPtr, this, &ServiceWaterScene::onAddedWorkspaceObject, Qt::QueuedConnection);
  
  m_model.getImpl<model::detail::Model_Impl>().get()->removeWorkspaceObjectPtr.connect<ServiceWaterScene, &ServiceWaterScene::onRemovedWorkspaceObject>(this);

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

  auto backgroundItem = new ServiceWaterItem(this);

  Q_UNUSED(backgroundItem);
}

model::Model ServiceWaterScene::model() const
{
  return m_model;
}

void ServiceWaterScene::onAddedWorkspaceObject(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> wPtr, const openstudio::IddObjectType& type, const openstudio::UUID& uuid)
{
  model::detail::WaterUseConnections_Impl* hvac_impl = dynamic_cast<model::detail::WaterUseConnections_Impl*>(wPtr.get());
  if(hvac_impl)
  {
    m_dirty = true;

    QTimer::singleShot(0,this,SLOT(layout()));
  }
}

void ServiceWaterScene::onRemovedWorkspaceObject(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> wPtr, const openstudio::IddObjectType& type, const openstudio::UUID& uuid)
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

  //model.getImpl<model::detail::Model_Impl>().get()->addWorkspaceObjectPtr.connect<WaterUseConnectionsDetailScene, &WaterUseConnectionsDetailScene::onAddedWorkspaceObject>(this);
  connect(OSAppBase::instance(), &OSAppBase::workspaceObjectAddedPtr, this, &WaterUseConnectionsDetailScene::onAddedWorkspaceObject, Qt::QueuedConnection);


  model.getImpl<model::detail::Model_Impl>().get()->removeWorkspaceObjectPtr.connect<WaterUseConnectionsDetailScene, &WaterUseConnectionsDetailScene::onRemovedWorkspaceObject>(this);

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

  auto backgroundItem = new WaterUseConnectionsDetailItem(this);

  Q_UNUSED(backgroundItem);
}

void WaterUseConnectionsDetailScene::onAddedWorkspaceObject(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> wPtr, const openstudio::IddObjectType& type, const openstudio::UUID& uuid)
{
  model::detail::WaterUseEquipment_Impl * hvac_impl = dynamic_cast<model::detail::WaterUseEquipment_Impl*>(wPtr.get());
  if(hvac_impl)
  {
    m_dirty = true;

    QTimer::singleShot(0,this,SLOT(layout()));
  }
}

void WaterUseConnectionsDetailScene::onRemovedWorkspaceObject(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> wPtr, const openstudio::IddObjectType& type, const openstudio::UUID& uuid)
{
  model::detail::WaterUseEquipment_Impl * hvac_impl = dynamic_cast<model::detail::WaterUseEquipment_Impl*>(wPtr.get());
  if(hvac_impl)
  {
    m_dirty = true;

    QTimer::singleShot(0,this,SLOT(layout()));
  }
}

} // openstudio

