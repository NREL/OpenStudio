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

#include "LoopScene.hpp"
#include "OSAppBase.hpp"
#include "GridItem.hpp"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QApplication>
#include <QDrag>
#include <QGraphicsItem>
#include <QMimeData>
#include <cmath>
#include "../model/Loop.hpp"
#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/Splitter.hpp"
#include "../model/Mixer.hpp"
#include "../model/ModelObject.hpp"
#include "../model/HVACComponent.hpp"
#include "../model/HVACComponent_Impl.hpp"
#include "../model/StraightComponent.hpp"
#include "../model/StraightComponent_Impl.hpp"
#include "../model/AirLoopHVACOutdoorAirSystem.hpp"
#include "../model/AirLoopHVACOutdoorAirSystem_Impl.hpp"
#include "../model/ThermalZone.hpp"
#include "../model/ThermalZone_Impl.hpp"
#include "../model/Node.hpp"
#include "../model/Node_Impl.hpp"
#include <QTimer>

using namespace openstudio::model;

namespace openstudio {

LoopScene::LoopScene( model::Loop loop,
                      QObject * parent )
  : GridScene(parent),
    m_loop(loop),
    m_dirty(true)
{
  // loop.model().getImpl<model::detail::Model_Impl>().get()->addWorkspaceObjectPtr.connect<LoopScene, &LoopScene::addedWorkspaceObject>(this);
  connect(OSAppBase::instance(), &OSAppBase::workspaceObjectAddedPtr, this, &LoopScene::addedWorkspaceObject, Qt::QueuedConnection);

  loop.model().getImpl<model::detail::Model_Impl>().get()->removeWorkspaceObjectPtr.connect<LoopScene, &LoopScene::removedWorkspaceObject>(this);

  layout();
}

void LoopScene::initDefault()
{

}

void LoopScene::layout()
{
  if( m_dirty && !m_loop.handle().isNull() )
  {
    QList<QGraphicsItem *> itemList = items();
    for( QList<QGraphicsItem *>::iterator it = itemList.begin();
         it < itemList.end();
         ++it )
    {
      removeItem(*it);
      delete *it;
    }

    SystemItem * systemItem = new SystemItem(m_loop,this);

    systemItem->setPos(50,50);

    this->setSceneRect(0,0,(systemItem->getHGridLength() * 100) + 100, ((systemItem->getVGridLength()) * 100) + 100);

    update();

    m_dirty = false;
  }
}

DemandSideItem * LoopScene::createDemandSide()
{
  auto demandInletNodes = m_loop.demandInletNodes();
  auto demandOutletNode = m_loop.demandOutletNode();

  DemandSideItem * demandSideItem = new DemandSideItem( nullptr,
                                                        demandInletNodes,
                                                        demandOutletNode );

  return demandSideItem;
}

SupplySideItem * LoopScene::createSupplySide()
{
  auto supplyInletNode = m_loop.supplyInletNode();
  auto supplyOutletNodes = m_loop.supplyOutletNodes();

  SupplySideItem * supplySideItem = new SupplySideItem( nullptr,
                                                        supplyInletNode,
                                                        supplyOutletNodes);

  return supplySideItem;
}

model::Loop LoopScene::loop()
{
  return m_loop;
}

void LoopScene::addedWorkspaceObject(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> wPtr, const openstudio::IddObjectType& type, const openstudio::UUID& uuid )
{
  model::detail::HVACComponent_Impl* hvac_impl = dynamic_cast<model::detail::HVACComponent_Impl*>(wPtr.get());
  if(hvac_impl)
  {
    m_dirty = true;

    QTimer::singleShot(0,this,SLOT(layout()));
  }
}

void LoopScene::removedWorkspaceObject(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> wPtr, const openstudio::IddObjectType& type, const openstudio::UUID& uuid )
{
  m_dirty = true;

  QTimer::singleShot(0,this,SLOT(layout()));
}

} // openstudio

