/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#include "AirTerminalSingleDuctInletSideMixer.hpp"
#include "AirTerminalSingleDuctInletSideMixer_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "ZoneHVACComponent.hpp"
#include "ZoneHVACComponent_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Splitter.hpp"
#include "Splitter_Impl.hpp"
#include "Mixer.hpp"
#include "Mixer_Impl.hpp"
#include "PortList.hpp"
#include "PortList_Impl.hpp"
#include <utilities/idd/OS_AirTerminal_SingleDuct_InletSideMixer_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  AirTerminalSingleDuctInletSideMixer_Impl::AirTerminalSingleDuctInletSideMixer_Impl(const IdfObject& idfObject,
                                                                                     Model_Impl* model,
                                                                                     bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == AirTerminalSingleDuctInletSideMixer::iddObjectType());
  }

  AirTerminalSingleDuctInletSideMixer_Impl::AirTerminalSingleDuctInletSideMixer_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                     Model_Impl* model,
                                                                                     bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == AirTerminalSingleDuctInletSideMixer::iddObjectType());
  }

  AirTerminalSingleDuctInletSideMixer_Impl::AirTerminalSingleDuctInletSideMixer_Impl(const AirTerminalSingleDuctInletSideMixer_Impl& other,
                                                                                     Model_Impl* model,
                                                                                     bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& AirTerminalSingleDuctInletSideMixer_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType AirTerminalSingleDuctInletSideMixer_Impl::iddObjectType() const {
    return AirTerminalSingleDuctInletSideMixer::iddObjectType();
  }

  unsigned AirTerminalSingleDuctInletSideMixer_Impl::inletPort()
  {
    return OS_AirTerminal_SingleDuct_InletSideMixerFields::TerminalUnitPrimaryAirInlet;
  }

  unsigned AirTerminalSingleDuctInletSideMixer_Impl::outletPort()
  {
    return OS_AirTerminal_SingleDuct_InletSideMixerFields::TerminalUnitOutlet;
  }

  bool AirTerminalSingleDuctInletSideMixer_Impl::addToNode(Node & node)
  {
    Model _model = node.model();

    if( boost::optional<ModelObject> outlet = node.outletModelObject() )
    {
      boost::optional<ThermalZone> thermalZone;

      if( boost::optional<PortList> portList = outlet->optionalCast<PortList>()  )
      {
        thermalZone = portList->thermalZone();
      }

      if( thermalZone || (outlet->optionalCast<Mixer>() && node.airLoopHVAC()) )
      {
        if( boost::optional<ModelObject> inlet = node.inletModelObject() )
        {
          if( boost::optional<Splitter> splitter = inlet->optionalCast<Splitter>() )
          {
            boost::optional<ModelObject> sourceModelObject = inlet;
            boost::optional<unsigned> sourcePort = node.connectedObjectPort(node.inletPort());

            if( sourcePort && sourceModelObject )
            {
              Node inletNode(_model);

              _model.connect( sourceModelObject.get(),
                              sourcePort.get(),
                              inletNode,
                              inletNode.inletPort() );
              
              _model.connect( inletNode,
                              inletNode.outletPort(),
                              this->getObject<ModelObject>(),
                              this->inletPort() );

              _model.connect( this->getObject<ModelObject>(),
                              outletPort(),
                              node,
                              node.inletPort() );

              if( thermalZone )
              {
                AirTerminalSingleDuctInletSideMixer mo = this->getObject<AirTerminalSingleDuctInletSideMixer>();
                thermalZone->addEquipment(mo);
              }

              return true; 
            }
          }
        }
      }
    }

    return false;
  }

  std::vector<IdfObject> AirTerminalSingleDuctInletSideMixer_Impl::remove()
  {
    Model _model = this->model();
    AirTerminalSingleDuctInletSideMixer thisObject = this->getObject<AirTerminalSingleDuctInletSideMixer>();
    boost::optional<ThermalZone> t_thermalZone;
    boost::optional<ZoneHVACComponent> t_zoneComp;

    if( boost::optional<AirLoopHVAC> t_airLoopHVAC = airLoopHVAC() ) {
      std::vector<ZoneHVACComponent> zoneComps = 
        subsetCastVector<ZoneHVACComponent>(t_airLoopHVAC->demandComponents(thisObject,t_airLoopHVAC->demandOutletNode()));
      if( ! zoneComps.empty() ) {
        t_zoneComp = zoneComps.front();
        t_thermalZone = t_zoneComp->thermalZone();
        t_zoneComp->removeFromThermalZone();
      }
    }

    boost::optional<ModelObject> sourceModelObject = this->inletModelObject();
    boost::optional<unsigned> sourcePort = this->connectedObjectPort(this->inletPort());
    
    boost::optional<ModelObject> targetModelObject = this->outletModelObject();
    boost::optional<unsigned> targetPort = this->connectedObjectPort(this->outletPort());

    std::vector<ThermalZone> thermalZones = _model.getConcreteModelObjects<ThermalZone>();
    for( auto & thermalZone : thermalZones )
    {
      std::vector<ModelObject> equipment = thermalZone.equipment();

      if( std::find(equipment.begin(),equipment.end(),thisObject) != equipment.end() )
      {
        thermalZone.removeEquipment(thisObject);

        break;
      }
    }

    if( sourcePort && sourceModelObject
        && targetPort && targetModelObject )
    {
      if( boost::optional<Node> inletNode = sourceModelObject->optionalCast<Node>() )
      {
        if( boost::optional<ModelObject> source2ModelObject = inletNode->inletModelObject() )
        {
          if( boost::optional<unsigned> source2Port = inletNode->connectedObjectPort(inletNode->inletPort()) )
          {
            _model.connect( source2ModelObject.get(),
                            source2Port.get(),
                            targetModelObject.get(),
                            targetPort.get() );

            inletNode->disconnect();
            inletNode->remove();

            std::vector<IdfObject> result =  StraightComponent_Impl::remove();
            if( t_thermalZone ) {
              t_zoneComp->addToThermalZone(t_thermalZone.get());
            }
            return result;
          }
        }
      }
    }

    model().disconnect(getObject<ModelObject>(),inletPort());
    model().disconnect(getObject<ModelObject>(),outletPort());

    return StraightComponent_Impl::remove();
  }

  bool AirTerminalSingleDuctInletSideMixer_Impl::isRemovable() const
  {
    return true;
  }

  unsigned AirTerminalSingleDuctInletSideMixer_Impl::secondaryAirInletPort() const
  {
    return OS_AirTerminal_SingleDuct_InletSideMixerFields::TerminalUnitSecondaryAirInlet;
  }
  
  boost::optional<Node> AirTerminalSingleDuctInletSideMixer_Impl::secondaryAirInletNode() const
  {
    boost::optional<Node> result;
    if( boost::optional<ModelObject> mo = connectedObject(secondaryAirInletPort()) ) {
      result = mo->optionalCast<Node>();
    }
    return result;
  }

} // detail

AirTerminalSingleDuctInletSideMixer::AirTerminalSingleDuctInletSideMixer(const Model& model)
  : StraightComponent(AirTerminalSingleDuctInletSideMixer::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::AirTerminalSingleDuctInletSideMixer_Impl>());
}

IddObjectType AirTerminalSingleDuctInletSideMixer::iddObjectType() {
  return IddObjectType(IddObjectType::OS_AirTerminal_SingleDuct_InletSideMixer);
}

unsigned AirTerminalSingleDuctInletSideMixer::secondaryAirInletPort() const
{
  return getImpl<detail::AirTerminalSingleDuctInletSideMixer_Impl>()->secondaryAirInletPort();
}

boost::optional<Node> AirTerminalSingleDuctInletSideMixer::secondaryAirInletNode() const
{
  return getImpl<detail::AirTerminalSingleDuctInletSideMixer_Impl>()->secondaryAirInletNode();
}

/// @cond
AirTerminalSingleDuctInletSideMixer::AirTerminalSingleDuctInletSideMixer(std::shared_ptr<detail::AirTerminalSingleDuctInletSideMixer_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond

} // model
} // openstudio

