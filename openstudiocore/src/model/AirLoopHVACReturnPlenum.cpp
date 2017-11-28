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

#include "AirLoopHVACReturnPlenum.hpp"
#include "AirLoopHVACReturnPlenum_Impl.hpp"
#include "AirLoopHVACZoneMixer.hpp"
#include "AirLoopHVACZoneMixer_Impl.hpp"
#include "AirLoopHVACSupplyPlenum.hpp"
#include "AirLoopHVACSupplyPlenum_Impl.hpp"
#include "AirLoopHVACZoneSplitter.hpp"
#include "AirLoopHVACZoneSplitter_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "PortList.hpp"
#include "PortList_Impl.hpp"
#include <utilities/idd/OS_AirLoopHVAC_ReturnPlenum_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>


namespace openstudio {
namespace model {

namespace detail {

  AirLoopHVACReturnPlenum_Impl::AirLoopHVACReturnPlenum_Impl(const IdfObject& idfObject,
                                                             Model_Impl* model,
                                                             bool keepHandle)
    : Mixer_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == AirLoopHVACReturnPlenum::iddObjectType());
  }

  AirLoopHVACReturnPlenum_Impl::AirLoopHVACReturnPlenum_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                             Model_Impl* model,
                                                             bool keepHandle)
    : Mixer_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == AirLoopHVACReturnPlenum::iddObjectType());
  }

  AirLoopHVACReturnPlenum_Impl::AirLoopHVACReturnPlenum_Impl(const AirLoopHVACReturnPlenum_Impl& other,
                                                             Model_Impl* model,
                                                             bool keepHandle)
    : Mixer_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& AirLoopHVACReturnPlenum_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType AirLoopHVACReturnPlenum_Impl::iddObjectType() const {
    return AirLoopHVACReturnPlenum::iddObjectType();
  }

  boost::optional<ThermalZone> AirLoopHVACReturnPlenum_Impl::thermalZone() const {
    return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_AirLoopHVAC_ReturnPlenumFields::ThermalZone);
  }

  bool AirLoopHVACReturnPlenum_Impl::setThermalZone(const boost::optional<ThermalZone>& thermalZone) {
    bool result(false);

    if( ! thermalZone )
    {
      resetThermalZone();
      result = true;
    }
    else if( (! thermalZone->getImpl<ThermalZone_Impl>()->airLoopHVACSupplyPlenum()) &&
             (! thermalZone->getImpl<ThermalZone_Impl>()->airLoopHVACReturnPlenum()) &&
             (thermalZone->equipment().size() == 0) &&
             (! thermalZone->useIdealAirLoads()) )
    {
      result = setPointer(OS_AirLoopHVAC_ReturnPlenumFields::ThermalZone, thermalZone.get().handle());
    }

    return result;
  }

  void AirLoopHVACReturnPlenum_Impl::resetThermalZone() {
    bool result = setString(OS_AirLoopHVAC_ReturnPlenumFields::ThermalZone, "");
    OS_ASSERT(result);
  }

  unsigned AirLoopHVACReturnPlenum_Impl::outletPort() const
  {
    return OS_AirLoopHVAC_ReturnPlenumFields::OutletNode;
  }

  PortList AirLoopHVACReturnPlenum_Impl::inducedAirOutletPortList()
  {
    model::AirLoopHVACReturnPlenum plenum = getObject<model::AirLoopHVACReturnPlenum>();
    boost::optional<PortList> portList = plenum.getModelObjectTarget<PortList>(OS_AirLoopHVAC_ReturnPlenumFields::InducedAirOutletPortList);
    if( ! portList )
    {
      portList = PortList(plenum);
      setPointer(OS_AirLoopHVAC_ReturnPlenumFields::InducedAirOutletPortList,portList->handle());
    }
    return portList.get();
  }

  unsigned AirLoopHVACReturnPlenum_Impl::inletPort(unsigned branchIndex) const
  {
    unsigned result;
    result = numNonextensibleFields();
    result = result + branchIndex;
    return result;
  }

  unsigned AirLoopHVACReturnPlenum_Impl::nextInletPort() const
  {
    return inletPort( this->nextBranchIndex() );
  }

  bool AirLoopHVACReturnPlenum_Impl::addToNode(Node & node)
  {
    bool result = true;

    Model _model = model();

    // Is the node in this model
    if( node.model() != _model )
    {
      result = false;
    }

    // Is the node part of an air loop
    boost::optional<AirLoopHVAC> nodeAirLoop = node.airLoopHVAC();

    if( ! nodeAirLoop )
    {
      result = false;
    }

    // Is this plenum already connected to a different air loop
    boost::optional<AirLoopHVAC> currentAirLoopHVAC = airLoopHVAC();
    if( currentAirLoopHVAC && (currentAirLoopHVAC.get() != nodeAirLoop) )
    {
      result = false;
    }

    boost::optional<ModelObject> outletObj = node.outletModelObject();
    boost::optional<ModelObject> inletObj = node.inletModelObject();
    boost::optional<AirLoopHVACZoneMixer> mixer;

    // Is the immediate downstream object to the node a mixer
    if( result )
    {
      mixer = nodeAirLoop->zoneMixer();

      if( ! (outletObj && mixer && (outletObj.get() == mixer.get()) ) )
      {
        result = false;
      }
    }

    // Make sure there is not already a return plenum
    if( result )
    {
      if(  inletObj && inletObj->optionalCast<AirLoopHVACReturnPlenum>() )
      {
        result = false;
      }
    }

    // Is there a zone on this branch
    if( result )
    {
      Splitter splitter = nodeAirLoop->zoneSplitter();
      if( nodeAirLoop->demandComponents(splitter,node,ThermalZone::iddObjectType()).empty() )
      {
        result = false;
      }
    }

    if( result )
    {
      unsigned inletObjectPort;
      unsigned outletObjectPort;
      boost::optional<ModelObject> inletModelObject;
      boost::optional<ModelObject> outletModelObject;

      inletModelObject = node;
      inletObjectPort = node.outletPort();
      outletModelObject = outletObj;
      outletObjectPort = node.connectedObjectPort(node.outletPort()).get();

      AirLoopHVACReturnPlenum thisObject = getObject<AirLoopHVACReturnPlenum>();

      if( currentAirLoopHVAC )
      {
        mixer->removePortForBranch(mixer->branchIndexForInletModelObject(inletModelObject.get()));
        _model.connect(inletModelObject.get(),inletObjectPort,thisObject,thisObject.nextInletPort());
      }
      else
      {
        Node plenumOutletNode(_model);
        plenumOutletNode.createName();

        _model.connect(inletModelObject.get(),inletObjectPort,thisObject,thisObject.nextInletPort());
        _model.connect(thisObject,thisObject.outletPort(),plenumOutletNode,plenumOutletNode.inletPort());
        _model.connect(plenumOutletNode,plenumOutletNode.outletPort(),outletModelObject.get(),outletObjectPort);
      }
    }

    return result;
  }

  bool AirLoopHVACReturnPlenum_Impl::addBranchForZone(openstudio::model::ThermalZone & thermalZone)
  {
    boost::optional<HVACComponent> t_terminal;

    return addBranchForZoneImpl(thermalZone,t_terminal);
  }

  bool AirLoopHVACReturnPlenum_Impl::addBranchForZone(openstudio::model::ThermalZone & thermalZone, HVACComponent & terminal)
  {
    boost::optional<HVACComponent> t_terminal = terminal;

    return addBranchForZoneImpl(thermalZone,t_terminal);
  }

  bool AirLoopHVACReturnPlenum_Impl::addBranchForZoneImpl(openstudio::model::ThermalZone & thermalZone, boost::optional<HVACComponent> & terminal)
  {
    boost::optional<Splitter> splitter;
    boost::optional<Mixer> mixer = getObject<AirLoopHVACReturnPlenum>();

    boost::optional<AirLoopHVAC> t_airLoopHVAC = airLoopHVAC();

    if( ! t_airLoopHVAC )
    {
      return false;
    }

    std::vector<ModelObject> supplyPlenums;
    supplyPlenums = t_airLoopHVAC->demandComponents(t_airLoopHVAC->demandInletNode(),
                                                    mixer.get(),
                                                    AirLoopHVACSupplyPlenum::iddObjectType());

    if( supplyPlenums.size() == 1u )
    {
      splitter = supplyPlenums.front().cast<Splitter>();
    }
    else
    {
      splitter = t_airLoopHVAC->zoneSplitter();
    }

    OS_ASSERT(splitter);
    OS_ASSERT(mixer);

    return AirLoopHVAC_Impl::addBranchForZoneImpl(thermalZone,t_airLoopHVAC.get(),splitter.get(),mixer.get(),terminal);
  }

  std::vector<IdfObject> AirLoopHVACReturnPlenum_Impl::remove()
  {
    Model t_model = model();

    if( boost::optional<AirLoopHVAC> t_airLoopHVAC = airLoopHVAC() )
    {
      AirLoopHVACZoneMixer zoneMixer = t_airLoopHVAC->zoneMixer();
      std::vector<ModelObject> t_inletModelObjects = inletModelObjects();

      for( auto it = t_inletModelObjects.rbegin();
           it != t_inletModelObjects.rend();
           ++it )
      {
        unsigned branchIndex = branchIndexForInletModelObject(*it);
        unsigned t_inletPort = inletPort(branchIndex);
        unsigned connectedObjectOutletPort = connectedObjectPort(t_inletPort).get();

        t_model.connect(*it,connectedObjectOutletPort,zoneMixer,zoneMixer.nextInletPort());
      }

      boost::optional<ModelObject> mo = outletModelObject();
      OS_ASSERT(mo);
      boost::optional<Node> node = mo->optionalCast<Node>();
      OS_ASSERT(node);
      zoneMixer.removePortForBranch(zoneMixer.branchIndexForInletModelObject(node.get()));
      node->remove();
    }

    return Mixer_Impl::remove();
  }

} // detail

AirLoopHVACReturnPlenum::AirLoopHVACReturnPlenum(const Model& model)
  : Mixer(AirLoopHVACReturnPlenum::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::AirLoopHVACReturnPlenum_Impl>());
}

IddObjectType AirLoopHVACReturnPlenum::iddObjectType() {
  return IddObjectType(IddObjectType::OS_AirLoopHVAC_ReturnPlenum);
}

boost::optional<ThermalZone> AirLoopHVACReturnPlenum::thermalZone() const {
  return getImpl<detail::AirLoopHVACReturnPlenum_Impl>()->thermalZone();
}

bool AirLoopHVACReturnPlenum::setThermalZone(const ThermalZone& thermalZone) {
  return getImpl<detail::AirLoopHVACReturnPlenum_Impl>()->setThermalZone(thermalZone);
}

void AirLoopHVACReturnPlenum::resetThermalZone() {
  getImpl<detail::AirLoopHVACReturnPlenum_Impl>()->resetThermalZone();
}

unsigned AirLoopHVACReturnPlenum::outletPort() const
{
  return getImpl<detail::AirLoopHVACReturnPlenum_Impl>()->outletPort();
}

unsigned AirLoopHVACReturnPlenum::inletPort(unsigned branchIndex) const
{
  return getImpl<detail::AirLoopHVACReturnPlenum_Impl>()->inletPort(branchIndex);
}

unsigned AirLoopHVACReturnPlenum::nextInletPort() const
{
  return getImpl<detail::AirLoopHVACReturnPlenum_Impl>()->nextInletPort();
}

bool AirLoopHVACReturnPlenum::addToNode(Node & node)
{
  return getImpl<detail::AirLoopHVACReturnPlenum_Impl>()->addToNode(node);
}

bool AirLoopHVACReturnPlenum::addBranchForZone(openstudio::model::ThermalZone & thermalZone)
{
  return getImpl<detail::AirLoopHVACReturnPlenum_Impl>()->addBranchForZone(thermalZone);
}

bool AirLoopHVACReturnPlenum::addBranchForZone(openstudio::model::ThermalZone & thermalZone, HVACComponent & terminal)
{
  return getImpl<detail::AirLoopHVACReturnPlenum_Impl>()->addBranchForZone(thermalZone,terminal);
}

/// @cond
AirLoopHVACReturnPlenum::AirLoopHVACReturnPlenum(std::shared_ptr<detail::AirLoopHVACReturnPlenum_Impl> impl)
  : Mixer(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

