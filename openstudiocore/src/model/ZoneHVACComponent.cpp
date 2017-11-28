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

#include "ZoneHVACComponent.hpp"
#include "ZoneHVACComponent_Impl.hpp"
#include "HVACComponent.hpp"
#include "HVACComponent_Impl.hpp"
#include "AirTerminalSingleDuctInletSideMixer.hpp"
#include "AirTerminalSingleDuctInletSideMixer_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include "PortList.hpp"
#include "PortList_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ZoneHVACComponent_Impl::ZoneHVACComponent_Impl(IddObjectType type, Model_Impl* model)
    : HVACComponent_Impl(type,model)
  {
  }

  ZoneHVACComponent_Impl::ZoneHVACComponent_Impl(const IdfObject& idfObject,
                                                 Model_Impl* model,
                                                 bool keepHandle)
    : HVACComponent_Impl(idfObject, model, keepHandle)
  {
  }

  ZoneHVACComponent_Impl::ZoneHVACComponent_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other,
      Model_Impl* model,
      bool keepHandle)
    : HVACComponent_Impl(other,model,keepHandle)
  {
  }

  ZoneHVACComponent_Impl::ZoneHVACComponent_Impl(const ZoneHVACComponent_Impl& other,
                                                 Model_Impl* model,
                                                 bool keepHandles)
    : HVACComponent_Impl(other,model,keepHandles)
  {
  }

  std::vector<ModelObject> ZoneHVACComponent_Impl::children() const
  {
    return std::vector<ModelObject>();
  }

  boost::optional<ParentObject> ZoneHVACComponent_Impl::parent() const
  {
    return boost::optional<ParentObject>();
  }

  const std::vector<std::string> & ZoneHVACComponent_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  ModelObject ZoneHVACComponent_Impl::clone(Model model) const
  {
    auto clone = ModelObject_Impl::clone(model).cast<ZoneHVACComponent>();
    if( clone.inletPort() != 0u ) {
      clone.setString(clone.inletPort(),"");
    }
    if( clone.outletPort() != 0u ) {
      clone.setString(clone.outletPort(),"");
    }

    return clone;
  }

  boost::optional<ThermalZone> ZoneHVACComponent_Impl::thermalZone()
  {
    auto thisObject = this->getObject<ModelObject>();
    for( const auto & thermalZone : model().getConcreteModelObjects<ThermalZone>() ) {
      std::vector<ModelObject> equipment = thermalZone.equipment();

      if( std::find(equipment.begin(),equipment.end(),thisObject) != equipment.end() ) {
        return thermalZone;
      }
    }
    return boost::none;
  }

  bool ZoneHVACComponent_Impl::addToThermalZone(ThermalZone & thermalZone)
  {
    Model m = this->model();

    if( thermalZone.model() != m ) {
      return false;
    }

    if( thermalZone.isPlenum() ) {
      return false;
    }

    removeFromThermalZone();

    thermalZone.setUseIdealAirLoads(false);

    // Connect nodes if this is an air based zone hvac component
    if( inletPort() != 0u && outletPort() != 0u ) {
      // Exhaust Node
      Node exhaustNode(m);
      PortList exhaustPortList = thermalZone.exhaustPortList();
      unsigned nextPort = exhaustPortList.nextPort();
      m.connect(exhaustPortList,nextPort,exhaustNode,exhaustNode.inletPort());
      ModelObject mo = this->getObject<ModelObject>();
      m.connect(exhaustNode,exhaustNode.outletPort(),mo,this->inletPort());

      // Air Inlet Node
      Node airInletNode(m);
      PortList inletPortList = thermalZone.inletPortList();
      unsigned nextInletPort = inletPortList.nextPort();
      m.connect(airInletNode,airInletNode.outletPort(),inletPortList,nextInletPort);
      m.connect(mo,this->outletPort(),airInletNode,airInletNode.inletPort());
    }

    thermalZone.addEquipment(this->getObject<ZoneHVACComponent>());

    return true;
  }

  void ZoneHVACComponent_Impl::removeFromThermalZone()
  {
    boost::optional<ThermalZone> thermalZone = this->thermalZone();
    boost::optional<AirLoopHVAC> airLoopHVAC = this->airLoopHVAC();
    Model m = this->model();

    if( airLoopHVAC ) {
      removeFromAirLoopHVAC();
    } else if( thermalZone ) {
      ZoneHVACComponent mo = getObject<ZoneHVACComponent>();

      if( auto t_inletNode = inletNode() ) {
        t_inletNode->disconnect();
        t_inletNode->remove();
      }

      if( auto t_outletNode = outletNode() )
      {
        t_outletNode->disconnect();
        t_outletNode->remove();
      }
    }

    auto thisObject = this->getObject<ModelObject>();
    for( auto & thermalZone : m.getConcreteModelObjects<ThermalZone>() ) {
      std::vector<ModelObject> equipment = thermalZone.equipment();

      if( std::find(equipment.begin(),equipment.end(),thisObject) != equipment.end() ) {
        thermalZone.removeEquipment(thisObject);
        break;
      }
    }
  }

  boost::optional<Node> ZoneHVACComponent_Impl::inletNode() const
  {
    boost::optional<ModelObject> mo = connectedObject(inletPort());
    boost::optional<Node> result;

    if( mo )
    {
      if( boost::optional<Node> node = mo->optionalCast<Node>() )
      {
        result = node;
      }
    }

    return result;
  }

  boost::optional<Node> ZoneHVACComponent_Impl::outletNode() const
  {
    boost::optional<ModelObject> mo = connectedObject(outletPort());
    boost::optional<Node> result;

    if( mo )
    {
      if( boost::optional<Node> node = mo->optionalCast<Node>() )
      {
        result = node;
      }
    }

    return result;
  }

  std::vector<openstudio::IdfObject> ZoneHVACComponent_Impl::remove()
  {
    removeFromThermalZone();

    return HVACComponent_Impl::remove();
  }

  bool ZoneHVACComponent_Impl::addToNode(Node & node)
  {
    bool result = false;

    boost::optional<ThermalZone> thermalZone;
    boost::optional<AirTerminalSingleDuctInletSideMixer> terminal;

    if( boost::optional<ModelObject> outlet = node.outletModelObject() ) {
      if( boost::optional<PortList> pl = outlet->optionalCast<PortList>() ) {
        thermalZone = pl->thermalZone();
      }
    }

    if( boost::optional<ModelObject> inlet = node.inletModelObject() ) {
      terminal = inlet->optionalCast<AirTerminalSingleDuctInletSideMixer>();
    }

    if( thermalZone && terminal ) {
      if( this->thermalZone() ) {
        removeFromThermalZone();
      }
      thermalZone->setUseIdealAirLoads(false);
      ZoneHVACComponent thisObject = getObject<ZoneHVACComponent>();
      thermalZone->addEquipment(thisObject);
      thermalZone->setCoolingPriority(thisObject,1);
      thermalZone->setHeatingPriority(thisObject,1);
      Model t_model = model();
      ModelObject thisModelObject = getObject<model::ZoneHVACComponent>();
      unsigned targetPort = node.connectedObjectPort( node.outletPort() ).get();
      ModelObject targetModelObject = node.connectedObject( node.outletPort() ).get();
      Node newNode( t_model );
      t_model.connect( node, node.outletPort(),
                       thisModelObject, inletPort() );
      t_model.connect( thisModelObject, outletPort(),
                       newNode, newNode.inletPort() );
      t_model.connect( newNode, newNode.outletPort(),
                       targetModelObject, targetPort );

      Node exhaustNode(t_model);
      PortList exhaustPortList = thermalZone->exhaustPortList();
      unsigned nextPort = exhaustPortList.nextPort();
      t_model.connect(exhaustPortList,nextPort,exhaustNode,exhaustNode.inletPort());
      t_model.connect(exhaustNode,exhaustNode.outletPort(),terminal.get(),terminal->secondaryAirInletPort());

      result = true;
    }

    return result;
  }

  void ZoneHVACComponent_Impl::removeFromAirLoopHVAC()
  {
    if( boost::optional<AirLoopHVAC> t_airLoopHVAC = airLoopHVAC() ) {
      boost::optional<Node> t_inletNode = inletNode();
      OS_ASSERT(t_inletNode);
      boost::optional<Node> t_outletNode = outletNode();
      OS_ASSERT(t_outletNode);

      unsigned targetPort = t_outletNode->connectedObjectPort(t_outletNode->outletPort()).get();
      ModelObject targetModelObject = t_outletNode->connectedObject(t_outletNode->outletPort()).get();
      t_outletNode->disconnect();
      t_outletNode->remove();

      Model t_model = model();
      t_model.connect( t_inletNode.get(), t_inletNode->outletPort(),
                       targetModelObject, targetPort );

      std::vector<AirTerminalSingleDuctInletSideMixer> terminalMixers =
        subsetCastVector<AirTerminalSingleDuctInletSideMixer>(t_airLoopHVAC->demandComponents(t_airLoopHVAC->demandInletNode(),t_inletNode.get()));
      if( ! terminalMixers.empty() ) {
        if( boost::optional<Node> secondaryNode = terminalMixers.front().secondaryAirInletNode() ) {
          secondaryNode->disconnect();
          secondaryNode->remove();
        }
      }

    }
  }

  boost::optional<AirLoopHVAC> ZoneHVACComponent_Impl::airLoopHVAC() const
  {
    if( boost::optional<Node> node = inletNode() ) {
      return node->airLoopHVAC();
    }

    return boost::none;
  }

  std::vector<HVACComponent> ZoneHVACComponent_Impl::edges(const boost::optional<HVACComponent> & prev)
  {
    std::vector<HVACComponent> edges;
    if( boost::optional<Node> t_node = this->outletNode() ) {
      edges.push_back(t_node->cast<HVACComponent>());
    }
    return edges;
  }

} // detail

ZoneHVACComponent::ZoneHVACComponent(std::shared_ptr<detail::ZoneHVACComponent_Impl> p)
  : HVACComponent(std::move(p))
{}

ZoneHVACComponent::ZoneHVACComponent(IddObjectType type,const Model& model)
  : HVACComponent(type,model)
{
  OS_ASSERT(getImpl<detail::ZoneHVACComponent_Impl>());
}

std::vector<ModelObject> ZoneHVACComponent::children() const
{
  return getImpl<detail::ZoneHVACComponent_Impl>()->children();
}

unsigned ZoneHVACComponent::inletPort() const
{
  return getImpl<detail::ZoneHVACComponent_Impl>()->inletPort();
}

unsigned ZoneHVACComponent::outletPort() const
{
  return getImpl<detail::ZoneHVACComponent_Impl>()->outletPort();
}

boost::optional<ThermalZone> ZoneHVACComponent::thermalZone()
{
  return getImpl<detail::ZoneHVACComponent_Impl>()->thermalZone();
}

bool ZoneHVACComponent::addToThermalZone(ThermalZone & thermalZone)
{
  return getImpl<detail::ZoneHVACComponent_Impl>()->addToThermalZone(thermalZone);
}

void ZoneHVACComponent::removeFromThermalZone()
{
  return getImpl<detail::ZoneHVACComponent_Impl>()->removeFromThermalZone();
}

boost::optional<Node> ZoneHVACComponent::inletNode() const
{
  return getImpl<detail::ZoneHVACComponent_Impl>()->inletNode();
}

boost::optional<Node> ZoneHVACComponent::outletNode() const
{
  return getImpl<detail::ZoneHVACComponent_Impl>()->outletNode();
}

bool ZoneHVACComponent::addToNode(Node & node)
{
  return getImpl<detail::ZoneHVACComponent_Impl>()->addToNode(node);
}

boost::optional<AirLoopHVAC> ZoneHVACComponent::airLoopHVAC() const
{
  return getImpl<detail::ZoneHVACComponent_Impl>()->airLoopHVAC();
}

} // model

} // openstudio

