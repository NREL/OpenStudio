/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef MODEL_AIRLOOPHVACRETURNPLENUM_IMPL_HPP
#define MODEL_AIRLOOPHVACRETURNPLENUM_IMPL_HPP

#include "ModelAPI.hpp"
#include "Mixer_Impl.hpp"

namespace openstudio {
namespace model {

  class ThermalZone;
  class PortList;
  class ZoneHVACIdealLoadsAirSystem;

  namespace detail {

    /** AirLoopHVACReturnPlenum_Impl is a Mixer_Impl that is the implementation class for AirLoopHVACReturnPlenum.*/
    class MODEL_API AirLoopHVACReturnPlenum_Impl : public Mixer_Impl
    {

     public:
      AirLoopHVACReturnPlenum_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirLoopHVACReturnPlenum_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirLoopHVACReturnPlenum_Impl(const AirLoopHVACReturnPlenum_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AirLoopHVACReturnPlenum_Impl() = default;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      boost::optional<ThermalZone> thermalZone() const;

      bool setThermalZone(const boost::optional<ThermalZone>& thermalZone);

      void resetThermalZone();

      unsigned outletPort() const override;

      PortList inducedAirOutletPortList();

      unsigned inletPort(unsigned branchIndex) const override;

      unsigned nextInletPort() const override;

      bool addToNode(Node& node) override;

      bool addBranchForZone(openstudio::model::ThermalZone& thermalZone);

      bool addBranchForZone(openstudio::model::ThermalZone& thermalZone, HVACComponent& terminal);

      bool addBranchForZoneImpl(openstudio::model::ThermalZone& thermalZone, boost::optional<HVACComponent>& terminal);

      std::vector<IdfObject> remove() override;

      std::vector<model::ZoneHVACIdealLoadsAirSystem> zoneHVACIdealLoadsAirSystems() const;

     private:
      REGISTER_LOGGER("openstudio.model.AirLoopHVACReturnPlenum");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRLOOPHVACRETURNPLENUM_IMPL_HPP
