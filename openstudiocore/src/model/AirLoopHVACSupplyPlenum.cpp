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

#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include "AirLoopHVACZoneSplitter.hpp"
#include "AirLoopHVACZoneSplitter_Impl.hpp"
#include "AirLoopHVACSupplyPlenum.hpp"
#include "AirLoopHVACSupplyPlenum_Impl.hpp"
#include "AirLoopHVACReturnPlenum.hpp"
#include "AirLoopHVACReturnPlenum_Impl.hpp"
#include "AirLoopHVACZoneMixer.hpp"
#include "AirLoopHVACZoneMixer_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "AirTerminalSingleDuctUncontrolled.hpp"
#include "AirTerminalSingleDuctUncontrolled_Impl.hpp"
#include <utilities/idd/OS_AirLoopHVAC_SupplyPlenum_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>


namespace openstudio {
namespace model {

namespace detail {

  AirLoopHVACSupplyPlenum_Impl::AirLoopHVACSupplyPlenum_Impl(const IdfObject& idfObject,
                                                             Model_Impl* model,
                                                             bool keepHandle)
    : Splitter_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == AirLoopHVACSupplyPlenum::iddObjectType());
  }

  AirLoopHVACSupplyPlenum_Impl::AirLoopHVACSupplyPlenum_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                             Model_Impl* model,
                                                             bool keepHandle)
    : Splitter_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == AirLoopHVACSupplyPlenum::iddObjectType());
  }

  AirLoopHVACSupplyPlenum_Impl::AirLoopHVACSupplyPlenum_Impl(const AirLoopHVACSupplyPlenum_Impl& other,
                                                             Model_Impl* model,
                                                             bool keepHandle)
    : Splitter_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& AirLoopHVACSupplyPlenum_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType AirLoopHVACSupplyPlenum_Impl::iddObjectType() const {
    return AirLoopHVACSupplyPlenum::iddObjectType();
  }

  boost::optional<ThermalZone> AirLoopHVACSupplyPlenum_Impl::thermalZone() const {
    return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_AirLoopHVAC_SupplyPlenumFields::ThermalZone);
  }

  bool AirLoopHVACSupplyPlenum_Impl::setThermalZone(const boost::optional<ThermalZone>& thermalZone) {
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
      result = setPointer(OS_AirLoopHVAC_SupplyPlenumFields::ThermalZone, thermalZone.get().handle());
    }

    return result;
  }

  void AirLoopHVACSupplyPlenum_Impl::resetThermalZone() {
    bool result = setString(OS_AirLoopHVAC_SupplyPlenumFields::ThermalZone, "");
    OS_ASSERT(result);
  }

  unsigned AirLoopHVACSupplyPlenum_Impl::inletPort()
  {
    return OS_AirLoopHVAC_SupplyPlenumFields::InletNode;
  }

  unsigned AirLoopHVACSupplyPlenum_Impl::outletPort(unsigned branchIndex)
  {
    unsigned result;
    result = numNonextensibleFields();
    result = result + branchIndex;
    return result;
  }

  unsigned AirLoopHVACSupplyPlenum_Impl::nextOutletPort()
  {
    return outletPort( this->nextBranchIndex() );
  }

  bool AirLoopHVACSupplyPlenum_Impl::addToNode(Node & node)
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

    boost::optional<ModelObject> inletObj = node.inletModelObject();
    boost::optional<ModelObject> outletObj = node.outletModelObject();
    boost::optional<AirTerminalSingleDuctUncontrolled> directAirModelObject;
    boost::optional<ModelObject> directAirInletModelObject;

    if( inletObj && inletObj->optionalCast<AirTerminalSingleDuctUncontrolled>() )
    {
      directAirModelObject = inletObj->cast<AirTerminalSingleDuctUncontrolled>();
      directAirInletModelObject = directAirModelObject->inletModelObject();
    }

    // Is the immediate upstream object to the node a splitter
    // or a direct air terminal (special case since this guy oddly does not have an inlet node)
    boost::optional<AirLoopHVACZoneSplitter> splitter;

    if( result )
    {
      splitter = nodeAirLoop->zoneSplitter();

      if( directAirInletModelObject  )
      {
        if( ! (splitter && ( directAirInletModelObject.get() == splitter.get() )) )
        {
          result = false;
        }
      }
      else if( ! (inletObj && splitter && ( inletObj.get() == splitter.get() )) )
      {
        result = false;
      }
    }

    // Is there a zone on this branch
    if( result )
    {
      Mixer mixer = nodeAirLoop->zoneMixer();
      if( nodeAirLoop->demandComponents(node,mixer,ThermalZone::iddObjectType()).empty() )
      {
        result = false;
      }
    }

    unsigned oldOutletObjectPort;
    unsigned oldInletObjectPort;
    boost::optional<ModelObject> oldInletModelObject;
    boost::optional<ModelObject> oldOutletModelObject;

    // Record the old port connections
    if( result )
    {
      oldInletModelObject = splitter;

      if( directAirModelObject )
      {
        oldOutletModelObject = directAirModelObject;
        oldOutletObjectPort = directAirModelObject->inletPort();
        oldInletObjectPort = directAirModelObject->connectedObjectPort(directAirModelObject->inletPort()).get();
      } 
      else
      {
        oldOutletModelObject = node;
        oldOutletObjectPort = node.inletPort();
        oldInletObjectPort = node.connectedObjectPort(node.inletPort()).get();
      }
    } 

    // Create a new node and connect the plenum
    if( result )
    {
      AirLoopHVACSupplyPlenum thisObject = getObject<AirLoopHVACSupplyPlenum>();

      if( currentAirLoopHVAC )
      {
        splitter->removePortForBranch(splitter->branchIndexForOutletModelObject(oldOutletModelObject.get()));
        _model.connect(thisObject,thisObject.nextOutletPort(),oldOutletModelObject.get(),oldOutletObjectPort);
      }
      else
      {
        Node plenumInletNode(_model);
        plenumInletNode.createName();

        _model.connect(oldInletModelObject.get(),oldInletObjectPort,plenumInletNode,plenumInletNode.inletPort());
        _model.connect(plenumInletNode,plenumInletNode.outletPort(),thisObject,thisObject.inletPort());
        _model.connect(thisObject,thisObject.nextOutletPort(),oldOutletModelObject.get(),oldOutletObjectPort);
      }
    }

    return result;
  }

  bool AirLoopHVACSupplyPlenum_Impl::addBranchForZone(openstudio::model::ThermalZone & thermalZone, HVACComponent & terminal)
  {
    boost::optional<HVACComponent> t_terminal = terminal;

    return AirLoopHVACSupplyPlenum_Impl::addBranchForZoneImpl(thermalZone,t_terminal);
  }

  bool AirLoopHVACSupplyPlenum_Impl::addBranchForZone(openstudio::model::ThermalZone & thermalZone)
  {
    boost::optional<HVACComponent> t_terminal;

    return AirLoopHVACSupplyPlenum_Impl::addBranchForZoneImpl(thermalZone,t_terminal);
  }

  bool AirLoopHVACSupplyPlenum_Impl::addBranchForZoneImpl(openstudio::model::ThermalZone & thermalZone, boost::optional<HVACComponent> & terminal)
  {
    boost::optional<Splitter> splitter = getObject<AirLoopHVACSupplyPlenum>();
    boost::optional<Mixer> mixer;

    boost::optional<AirLoopHVAC> t_airLoopHVAC = airLoopHVAC();

    if( ! t_airLoopHVAC )
    {
      return false;
    }

    std::vector<ModelObject> returnPlenums;
    returnPlenums = t_airLoopHVAC->demandComponents(splitter.get(),
                                                    t_airLoopHVAC->demandOutletNode(),
                                                    AirLoopHVACReturnPlenum::iddObjectType());

    if( returnPlenums.size() == 1u )
    {
      mixer = returnPlenums.front().cast<Mixer>();
    }
    else
    {
      mixer = t_airLoopHVAC->zoneMixer();
    }

    OS_ASSERT(splitter);
    OS_ASSERT(mixer);

    return AirLoopHVAC_Impl::addBranchForZoneImpl(thermalZone,t_airLoopHVAC.get(),splitter.get(),mixer.get(),terminal);
  }

  std::vector<IdfObject> AirLoopHVACSupplyPlenum_Impl::remove()
  {
    Model t_model = model();

    if( boost::optional<AirLoopHVAC> t_airLoopHVAC = airLoopHVAC() )
    {
      AirLoopHVACZoneSplitter zoneSplitter= t_airLoopHVAC->zoneSplitter();
      std::vector<ModelObject> t_outletModelObjects = outletModelObjects();

      for( auto it = t_outletModelObjects.rbegin();
           it != t_outletModelObjects.rend();
           ++it )
      {
        unsigned branchIndex = branchIndexForOutletModelObject(*it); 
        unsigned t_outletPort = outletPort(branchIndex);
        unsigned connectedObjectInletPort = connectedObjectPort(t_outletPort).get();

        t_model.connect(zoneSplitter,zoneSplitter.nextOutletPort(),*it,connectedObjectInletPort);
      }

      boost::optional<ModelObject> mo = inletModelObject();
      OS_ASSERT(mo);
      boost::optional<Node> node = mo->optionalCast<Node>();
      OS_ASSERT(node);
      zoneSplitter.removePortForBranch(zoneSplitter.branchIndexForOutletModelObject(node.get()));
      node->remove();
    }

    return Splitter_Impl::remove();
  }

} // detail

AirLoopHVACSupplyPlenum::AirLoopHVACSupplyPlenum(const Model& model)
  : Splitter(AirLoopHVACSupplyPlenum::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::AirLoopHVACSupplyPlenum_Impl>());
}

IddObjectType AirLoopHVACSupplyPlenum::iddObjectType() {
  return IddObjectType(IddObjectType::OS_AirLoopHVAC_SupplyPlenum);
}

boost::optional<ThermalZone> AirLoopHVACSupplyPlenum::thermalZone() const {
  return getImpl<detail::AirLoopHVACSupplyPlenum_Impl>()->thermalZone();
}

bool AirLoopHVACSupplyPlenum::setThermalZone(const ThermalZone& thermalZone) {
  return getImpl<detail::AirLoopHVACSupplyPlenum_Impl>()->setThermalZone(thermalZone);
}

void AirLoopHVACSupplyPlenum::resetThermalZone() {
  getImpl<detail::AirLoopHVACSupplyPlenum_Impl>()->resetThermalZone();
}

unsigned AirLoopHVACSupplyPlenum::inletPort()
{
  return getImpl<detail::AirLoopHVACSupplyPlenum_Impl>()->inletPort();
}

unsigned AirLoopHVACSupplyPlenum::outletPort(unsigned branchIndex)
{
  return getImpl<detail::AirLoopHVACSupplyPlenum_Impl>()->outletPort(branchIndex);
}

unsigned AirLoopHVACSupplyPlenum::nextOutletPort()
{
  return getImpl<detail::AirLoopHVACSupplyPlenum_Impl>()->nextOutletPort();
}

bool AirLoopHVACSupplyPlenum::addToNode(Node & node)
{
  return getImpl<detail::AirLoopHVACSupplyPlenum_Impl>()->addToNode(node);
}

bool AirLoopHVACSupplyPlenum::addBranchForZone(openstudio::model::ThermalZone & thermalZone)
{
  return getImpl<detail::AirLoopHVACSupplyPlenum_Impl>()->addBranchForZone(thermalZone);
}

bool AirLoopHVACSupplyPlenum::addBranchForZone(openstudio::model::ThermalZone & thermalZone, HVACComponent & terminal)
{
  return getImpl<detail::AirLoopHVACSupplyPlenum_Impl>()->addBranchForZone(thermalZone,terminal);
}


/// @cond
AirLoopHVACSupplyPlenum::AirLoopHVACSupplyPlenum(std::shared_ptr<detail::AirLoopHVACSupplyPlenum_Impl> impl)
  : Splitter(impl)
{}
/// @endcond

} // model
} // openstudio

