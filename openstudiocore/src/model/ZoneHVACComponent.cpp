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

#include "ZoneHVACComponent.hpp"
#include "ZoneHVACComponent_Impl.hpp"
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
    : ParentObject_Impl(type,model)
  {
  }

  ZoneHVACComponent_Impl::ZoneHVACComponent_Impl(const IdfObject& idfObject,
                                                 Model_Impl* model,
                                                 bool keepHandle)
    : ParentObject_Impl(idfObject, model, keepHandle)
  {
  }

  ZoneHVACComponent_Impl::ZoneHVACComponent_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other,
      Model_Impl* model,
      bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {
  }

  ZoneHVACComponent_Impl::ZoneHVACComponent_Impl(const ZoneHVACComponent_Impl& other,
                                                 Model_Impl* model,
                                                 bool keepHandles)
    : ParentObject_Impl(other,model,keepHandles)
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
    return ModelObject_Impl::clone(model);
  }

  boost::optional<ThermalZone> ZoneHVACComponent_Impl::thermalZone()
  {
    boost::optional<ThermalZone> result;

    if( boost::optional<ModelObject> mo1 = connectedObject(outletPort()) )
    {
      if( boost::optional<Node> node = mo1->optionalCast<Node>() )
      {
        if( boost::optional<ModelObject> mo2 = node->outletModelObject() )
        {
          if( boost::optional<PortList> pl = mo2->optionalCast<PortList>() )
          {
            if( boost::optional<ThermalZone> tz = pl->thermalZone() )
            {
              result = tz;
            }
          }
        }
      }
    }

    return result;
  }

  bool ZoneHVACComponent_Impl::addToThermalZone(ThermalZone & thermalZone)
  {
    Model m = this->model();

    if( thermalZone.model() != m )
    {
      return false;
    }

    if( thermalZone.isPlenum() )
    {
      return false;
    }

    removeFromThermalZone();

    thermalZone.setUseIdealAirLoads(false);

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

    thermalZone.addEquipment(this->getObject<ZoneHVACComponent>());

    return true;
  }

  void ZoneHVACComponent_Impl::removeFromThermalZone()
  {
    boost::optional<ThermalZone> thermalZone = this->thermalZone();
    Model m = this->model();

    if( thermalZone )
    {
      ZoneHVACComponent mo = getObject<ZoneHVACComponent>();

      boost::optional<Node> inletNode = this->inletNode();

      if( inletNode )
      {
        inletNode->disconnect();

        inletNode->remove();
      }

      boost::optional<Node> outletNode = this->outletNode();

      if( outletNode )
      {
        outletNode->disconnect();

        outletNode->remove();
      }
    }

    ModelObject thisObject = this->getObject<ModelObject>();
    std::vector<ThermalZone> thermalZones = m.getConcreteModelObjects<ThermalZone>();
    for( auto & thermalZone : thermalZones )
    {
      std::vector<ModelObject> equipment = thermalZone.equipment();

      if( std::find(equipment.begin(),equipment.end(),thisObject) != equipment.end() )
      {
        thermalZone.removeEquipment(thisObject);

        break;
      }
    }
  }

  boost::optional<Node> ZoneHVACComponent_Impl::inletNode()
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

  boost::optional<Node> ZoneHVACComponent_Impl::outletNode()
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

    return ParentObject_Impl::remove();
  }

} // detail

ZoneHVACComponent::ZoneHVACComponent(std::shared_ptr<detail::ZoneHVACComponent_Impl> p)
  : ParentObject(p)
{}

ZoneHVACComponent::ZoneHVACComponent(IddObjectType type,const Model& model)
  : ParentObject(type,model)
{
  OS_ASSERT(getImpl<detail::ZoneHVACComponent_Impl>());
}     

std::vector<ModelObject> ZoneHVACComponent::children() const
{
  return getImpl<detail::ZoneHVACComponent_Impl>()->children();
}

unsigned ZoneHVACComponent::inletPort()
{
  return getImpl<detail::ZoneHVACComponent_Impl>()->inletPort();
}

unsigned ZoneHVACComponent::outletPort()
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

boost::optional<Node> ZoneHVACComponent::inletNode()
{
  return getImpl<detail::ZoneHVACComponent_Impl>()->inletNode();
}

boost::optional<Node> ZoneHVACComponent::outletNode()
{
  return getImpl<detail::ZoneHVACComponent_Impl>()->outletNode();
}

} // model

} // openstudio

