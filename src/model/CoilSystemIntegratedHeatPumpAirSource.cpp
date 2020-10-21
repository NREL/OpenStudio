/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "CoilSystemIntegratedHeatPumpAirSource.hpp"
#include "CoilSystemIntegratedHeatPumpAirSource_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"

#include "AirLoopHVACUnitarySystem.hpp"
#include "AirLoopHVACUnitarySystem_Impl.hpp"

#include <utilities/idd/OS_CoilSystem_IntegratedHeatPump_AirSource_FieldEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CoilSystemIntegratedHeatPumpAirSource_Impl::CoilSystemIntegratedHeatPumpAirSource_Impl(const IdfObject& idfObject,
                                                                                         Model_Impl* model,
                                                                                         bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilSystemIntegratedHeatPumpAirSource::iddObjectType());
  }

  CoilSystemIntegratedHeatPumpAirSource_Impl::CoilSystemIntegratedHeatPumpAirSource_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                         Model_Impl* model,
                                                                                         bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilSystemIntegratedHeatPumpAirSource::iddObjectType());
  }

  CoilSystemIntegratedHeatPumpAirSource_Impl::CoilSystemIntegratedHeatPumpAirSource_Impl(const CoilSystemIntegratedHeatPumpAirSource_Impl& other,
                                                                                         Model_Impl* model,
                                                                                         bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CoilSystemIntegratedHeatPumpAirSource_Impl::outputVariableNames() const
  {
    static const std::vector<std::string> result;

    return result;
  }

  IddObjectType CoilSystemIntegratedHeatPumpAirSource_Impl::iddObjectType() const {
    return CoilSystemIntegratedHeatPumpAirSource::iddObjectType();
  }


  std::vector<ModelObject> CoilSystemIntegratedHeatPumpAirSource_Impl::children() const
  {
    std::vector<ModelObject> result;

    // TODO

    return result;
  }

  ModelObject CoilSystemIntegratedHeatPumpAirSource_Impl::clone(Model model) const
  {
    auto newCoilSystem = StraightComponent_Impl::clone(model).cast<CoilSystemIntegratedHeatPumpAirSource>();

    // TODO

    return newCoilSystem;
  }

  boost::optional<HVACComponent> CoilSystemIntegratedHeatPumpAirSource_Impl::containingHVACComponent() const
  {
     // AirLoopHVACUnitarySystem
    std::vector<AirLoopHVACUnitarySystem> airLoopHVACUnitarySystems = this->model().getConcreteModelObjects<AirLoopHVACUnitarySystem>();

    for( const auto & airLoopHVACUnitarySystem : airLoopHVACUnitarySystems )
    {
      if( boost::optional<HVACComponent> coolingCoil = airLoopHVACUnitarySystem.coolingCoil() )
      {
        if( coolingCoil->handle() == this->handle() )
        {
          return airLoopHVACUnitarySystem;
        }
      }
    }

    return boost::none;;
  }

  boost::optional<ZoneHVACComponent> CoilSystemIntegratedHeatPumpAirSource_Impl::containingZoneHVACComponent() const
  {

    return boost::none;
  }







  unsigned CoilSystemIntegratedHeatPumpAirSource_Impl::inletPort() const {
    return OS_CoilSystem_IntegratedHeatPump_AirSourceFields::AirInletNodeName;
  }

  unsigned CoilSystemIntegratedHeatPumpAirSource_Impl::outletPort() const {
    return OS_CoilSystem_IntegratedHeatPump_AirSourceFields::AirOutletNodeName;
  }

  bool CoilSystemIntegratedHeatPumpAirSource_Impl::addToNode(Node & node)
  {

    return false;
  }

} // detail

CoilSystemIntegratedHeatPumpAirSource::CoilSystemIntegratedHeatPumpAirSource(const Model& model)
  : StraightComponent(CoilSystemIntegratedHeatPumpAirSource::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilSystemIntegratedHeatPumpAirSource_Impl>());


}

IddObjectType CoilSystemIntegratedHeatPumpAirSource::iddObjectType() {
  return IddObjectType(IddObjectType::OS_CoilSystem_IntegratedHeatPump_AirSource);
}



/// @cond
CoilSystemIntegratedHeatPumpAirSource::CoilSystemIntegratedHeatPumpAirSource(std::shared_ptr<detail::CoilSystemIntegratedHeatPumpAirSource_Impl> impl)
  : StraightComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

