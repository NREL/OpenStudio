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
  : Splitter(std::move(p))
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

