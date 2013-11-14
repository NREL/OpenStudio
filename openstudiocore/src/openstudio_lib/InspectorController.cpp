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

#include <openstudio_lib/InspectorController.hpp>
#include <openstudio_lib/InspectorView.hpp>
#include <openstudio_lib/OSAppBase.hpp>
#include <openstudio_lib/OSDocument.hpp>
#include <openstudio_lib/MainWindow.hpp>
#include <model/AirLoopHVACZoneSplitter.hpp>
#include <model/AirLoopHVACZoneSplitter_Impl.hpp>
#include <model/AirLoopHVACZoneMixer.hpp>
#include <model/AirLoopHVACZoneMixer_Impl.hpp>
#include <model/AirLoopHVAC.hpp>
#include <model/AirLoopHVAC_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/PlantLoop.hpp>
#include <model/PlantLoop_Impl.hpp>
#include <model/HVACComponent.hpp>
#include <model/HVACComponent_Impl.hpp>
#include <model/AirTerminalSingleDuctConstantVolumeCooledBeam.hpp>
#include <model/AirTerminalSingleDuctConstantVolumeCooledBeam_Impl.hpp>
#include <model/AirTerminalSingleDuctUncontrolled.hpp>
#include <model/AirTerminalSingleDuctUncontrolled_Impl.hpp>
#include <model/AirTerminalSingleDuctVAVReheat.hpp>
#include <model/AirTerminalSingleDuctVAVReheat_Impl.hpp>
#include <model/ZoneHVACFourPipeFanCoil.hpp>
#include <model/ZoneHVACFourPipeFanCoil_Impl.hpp>
#include <model/ZoneHVACPackagedTerminalAirConditioner.hpp>
#include <model/ZoneHVACPackagedTerminalAirConditioner_Impl.hpp>
#include <model/ZoneHVACUnitHeater.hpp>
#include <model/ZoneHVACUnitHeater_Impl.hpp>
#include <model/ScheduleCompact.hpp>
#include <model/ScheduleCompact_Impl.hpp>
#include <model/WaterToAirComponent.hpp>
#include <model/WaterToAirComponent_Impl.hpp>

#include <utilities/core/Assert.hpp>

namespace openstudio {

InspectorController::InspectorController()
  : QObject()
{
  m_inspectorView = new InspectorView();

  connect( m_inspectorView,SIGNAL(addZoneClicked(model::ThermalZone &)),
           this,SLOT(addBranchForZone(model::ThermalZone &)));

  bool isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                             m_inspectorView, SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  connect( m_inspectorView,SIGNAL(removeZoneClicked(model::ThermalZone &)),
           this,SLOT(removeBranchForZone(model::ThermalZone &)));

  connect( m_inspectorView,SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)),
           this,SLOT(addToLoop(model::Loop &, boost::optional<model::HVACComponent> &)));

  connect( m_inspectorView,SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)),
           this,SLOT(removeFromLoop(model::Loop &, boost::optional<model::HVACComponent> &)));

  connect(m_inspectorView,SIGNAL(destroyed(QObject *)),this,SLOT(onViewDestroyed(QObject *)));
}

InspectorController::~InspectorController()
{
  if( m_inspectorView != 0 )
  {
    delete m_inspectorView;

    m_inspectorView = 0;
  }
}

InspectorView * InspectorController::inspectorView()
{
  return m_inspectorView;
}

void InspectorController::layoutModelObject( model::OptionalModelObject & modelObject, bool readOnly )
{
  bool displayIP = OSAppBase::instance()->currentDocument()->mainWindow()->displayIP();

  m_inspectorView->layoutModelObject( modelObject, readOnly, displayIP );

  m_modelObject = modelObject;
}

void InspectorController::addBranchForZone(model::ThermalZone & zone)
{
  model::OptionalHVACComponent hvacComponent = m_modelObject->optionalCast<model::HVACComponent>();

  model::OptionalAirLoopHVAC airLoop;

  if( hvacComponent )
  {
    airLoop = hvacComponent->airLoopHVAC();
  }

  if( airLoop )
  {
    model::Model model = zone.model();

    airLoop->addBranchForZone(zone,boost::none);
  }
}

void InspectorController::removeBranchForZone(model::ThermalZone & zone)
{
  model::OptionalAirLoopHVAC airLoop = zone.airLoopHVAC();

  if( airLoop )
  {
    airLoop->removeBranchForZone(zone);
  }
}

void InspectorController::onViewDestroyed(QObject * object)
{
  m_inspectorView = 0;
}

void InspectorController::addToLoop(model::Loop & loop, boost::optional<model::HVACComponent> & hvacComponent)
{
  if( hvacComponent )
  {
    if( boost::optional<model::PlantLoop> plantLoop = loop.optionalCast<model::PlantLoop>() )
    {
      plantLoop->addDemandBranchForComponent(hvacComponent.get());
    }
  }

  //if( m_modelObject )
  //{
  //  if( boost::optional<model::PlantLoop> plantLoop = loop.optionalCast<model::PlantLoop>() )
  //  {
  //    if( boost::optional<model::AirTerminalSingleDuctVAVReheat> comp =
  //          m_modelObject->optionalCast<model::AirTerminalSingleDuctVAVReheat>() )
  //    {
  //      if( boost::optional<model::HVACComponent> coil = comp->reheatCoil() )
  //      {
  //        if( boost::optional<model::WaterToAirComponent> waterCoil = coil->optionalCast<model::WaterToAirComponent>() )
  //        {
  //          plantLoop->addDemandBranchForComponent( waterCoil.get() );
  //        }
  //      }
  //    }
  //    else if( boost::optional<model::ZoneHVACPackagedTerminalAirConditioner> comp =
  //          m_modelObject->optionalCast<model::ZoneHVACPackagedTerminalAirConditioner>() )
  //    {
  //      if( boost::optional<model::HVACComponent> coil = comp->heatingCoil() )
  //      {
  //        if( boost::optional<model::WaterToAirComponent> waterCoil = coil->optionalCast<model::WaterToAirComponent>() )
  //        {
  //          plantLoop->addDemandBranchForComponent( waterCoil.get() );
  //        }
  //      }
  //    }
  //    else if( boost::optional<model::ZoneHVACFourPipeFanCoil> comp =
  //          m_modelObject->optionalCast<model::ZoneHVACFourPipeFanCoil>() )
  //    {
  //      if( waterToAirComponent )
  //      {
  //        if( boost::optional<model::HVACComponent> coil = comp->heatingCoil() )
  //        {
  //          if( waterToAirComponent->handle() == coil->handle() )
  //          {
  //            plantLoop->addDemandBranchForComponent( waterToAirComponent.get() );
  //          }
  //        }
  //        if( boost::optional<model::HVACComponent> coil = comp->coolingCoil() )
  //        {
  //          if( waterToAirComponent->handle() == coil->handle() )
  //          {
  //            plantLoop->addDemandBranchForComponent( waterToAirComponent.get() );
  //          }
  //        }
  //      }
  //    }
  //    else if( boost::optional<model::HVACComponent> comp = m_modelObject->optionalCast<model::HVACComponent>() )
  //    {
  //      plantLoop->addDemandBranchForComponent( comp.get() );
  //    }
  //  }
  //}
}

void InspectorController::removeFromLoop(model::Loop & loop, boost::optional<model::HVACComponent> & hvacComponent)
{
  if( hvacComponent )
  {
    if( boost::optional<model::PlantLoop> plantLoop = loop.optionalCast<model::PlantLoop>() )
    {
      plantLoop->removeDemandBranchWithComponent(hvacComponent.get());
    }
  }
}

void InspectorController:: toggleUnits(bool displayIP)
{
}

} // openstudio

