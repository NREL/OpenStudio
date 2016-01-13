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
#include "AirLoopHVACZoneSplitter.hpp"
#include "AirLoopHVACZoneSplitter_Impl.hpp"
#include "HVACComponent.hpp"
#include "HVACComponent_Impl.hpp"
#include "Node.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "AirTerminalSingleDuctUncontrolled.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include <utilities/idd/OS_AirLoopHVAC_ZoneSplitter_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Compare.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail{

  AirLoopHVACZoneSplitter_Impl::AirLoopHVACZoneSplitter_Impl(const IdfObject& idfObject,
                                                             Model_Impl* model,
                                                             bool keepHandle):
  Splitter_Impl(idfObject,model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == AirLoopHVACZoneSplitter::iddObjectType());
  }

  AirLoopHVACZoneSplitter_Impl::AirLoopHVACZoneSplitter_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
        : Splitter_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == AirLoopHVACZoneSplitter::iddObjectType());
  }

  AirLoopHVACZoneSplitter_Impl::AirLoopHVACZoneSplitter_Impl(
      const AirLoopHVACZoneSplitter_Impl& other,
      Model_Impl* model,
      bool keepHandle)
        : Splitter_Impl(other,model,keepHandle)
  {
  }

  AirLoopHVACZoneSplitter_Impl::~AirLoopHVACZoneSplitter_Impl()
  {
  }

  const std::vector<std::string>& AirLoopHVACZoneSplitter_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType AirLoopHVACZoneSplitter_Impl::iddObjectType() const {
    return AirLoopHVACZoneSplitter::iddObjectType();
  }

  std::vector<openstudio::IdfObject> AirLoopHVACZoneSplitter_Impl::remove()
  {
    if( this->airLoopHVAC() )
    {
      return std::vector<openstudio::IdfObject>();
    }
    else
    {
      OptionalAirLoopHVACZoneSplitter self = model().getModelObject<AirLoopHVACZoneSplitter>(handle());
      model().disconnect(*self,inletPort());
      for( int i = 0; i < int(nextBranchIndex()) - 1; i++ )
      {
        model().disconnect(*self,outletPort(i));
      }
      return HVACComponent_Impl::remove();
    }
  }

  void AirLoopHVACZoneSplitter_Impl::disconnect()
  {
    ModelObject mo = this->getObject<ModelObject>();
    model().disconnect(mo,inletPort());
    for( int i = 0; i < int(nextBranchIndex()); i++ )
    {
      model().disconnect(mo,outletPort(i));
    }
  }

  unsigned AirLoopHVACZoneSplitter_Impl::inletPort()
  {
    return OS_AirLoopHVAC_ZoneSplitterFields::InletNodeName;
  }

  unsigned AirLoopHVACZoneSplitter_Impl::outletPort(unsigned branchIndex)
  {
    unsigned result;
    result = numNonextensibleFields();
    result = result + branchIndex;
    return result;
  }

  unsigned AirLoopHVACZoneSplitter_Impl::nextOutletPort()
  {
    return outletPort( this->nextBranchIndex() );
  }

  std::vector<ThermalZone> AirLoopHVACZoneSplitter_Impl::thermalZones()
  {
    std::vector<ThermalZone> zones;
    std::vector<ModelObject> modelObjects;
    //std::vector<ModelObject> _outletModelObjects = outletModelObjects();

    OptionalAirLoopHVAC _airLoopHVAC = airLoopHVAC();
    OptionalNode demandOutletNode;
    OptionalNode demandInletNode;
    if( _airLoopHVAC )
    {
      demandOutletNode = _airLoopHVAC->demandOutletNode();
      demandInletNode = _airLoopHVAC->demandInletNode();
    }
    else
    {
      return zones;
    }

    modelObjects = _airLoopHVAC->demandComponents( demandInletNode.get(),
                                                   demandOutletNode.get(),
                                                   ThermalZone::iddObjectType() );

    for( const auto & modelObject : modelObjects )
    {
      OptionalThermalZone zone;
      zone = modelObject.optionalCast<ThermalZone>();
      if( zone )
      {
        zones.push_back(*zone);
      }
    }

    return zones;
  }

} // detail

AirLoopHVACZoneSplitter::AirLoopHVACZoneSplitter(const Model& model)
  : Splitter(AirLoopHVACZoneSplitter::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::AirLoopHVACZoneSplitter_Impl>());
}

AirLoopHVACZoneSplitter::AirLoopHVACZoneSplitter(std::shared_ptr<detail::AirLoopHVACZoneSplitter_Impl> p)
  : Splitter(p)
{}

std::vector<openstudio::IdfObject> AirLoopHVACZoneSplitter::remove()
{
  return getImpl<detail::AirLoopHVACZoneSplitter_Impl>()->remove();
}

unsigned AirLoopHVACZoneSplitter::inletPort()
{
  return getImpl<detail::AirLoopHVACZoneSplitter_Impl>()->inletPort();
}

unsigned AirLoopHVACZoneSplitter::outletPort(unsigned branchIndex)
{
  return getImpl<detail::AirLoopHVACZoneSplitter_Impl>()->outletPort(branchIndex);
}

unsigned AirLoopHVACZoneSplitter::nextOutletPort()
{
  return getImpl<detail::AirLoopHVACZoneSplitter_Impl>()->nextOutletPort();
}

IddObjectType AirLoopHVACZoneSplitter::iddObjectType() {
  IddObjectType result(IddObjectType::OS_AirLoopHVAC_ZoneSplitter);
  return result;
}

std::vector<ThermalZone> AirLoopHVACZoneSplitter::thermalZones()
{
  return getImpl<detail::AirLoopHVACZoneSplitter_Impl>()->thermalZones();
}

void AirLoopHVACZoneSplitter::disconnect()
{
  return getImpl<detail::AirLoopHVACZoneSplitter_Impl>()->disconnect();
}

} // model

} // openstudio

