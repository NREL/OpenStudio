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
  : StraightComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

