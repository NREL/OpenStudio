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

#ifndef MODEL_ZONEHVACEVAPORATIVECOOLERUNIT_IMPL_HPP
#define MODEL_ZONEHVACEVAPORATIVECOOLERUNIT_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "ZoneHVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class HVACComponent;

  namespace detail {

    /** ZoneHVACEvaporativeCoolerUnit_Impl is a ZoneHVACComponent_Impl that is the implementation class for ZoneHVACEvaporativeCoolerUnit.*/
    class MODEL_API ZoneHVACEvaporativeCoolerUnit_Impl : public ZoneHVACComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ZoneHVACEvaporativeCoolerUnit_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ZoneHVACEvaporativeCoolerUnit_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ZoneHVACEvaporativeCoolerUnit_Impl(const ZoneHVACEvaporativeCoolerUnit_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ZoneHVACEvaporativeCoolerUnit_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual ModelObject clone(Model model) const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual std::vector<ModelObject> children() const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      Schedule availabilitySchedule() const;

      HVACComponent supplyAirFan() const;

      boost::optional<double> designSupplyAirFlowRate() const;

      bool isDesignSupplyAirFlowRateAutosized() const;

      std::string fanPlacement() const;

      std::string coolerUnitControlMethod() const;

      double throttlingRangeTemperatureDifference() const;

      double coolingLoadControlThresholdHeatTransferRate() const;

      HVACComponent firstEvaporativeCooler() const;

      boost::optional<HVACComponent> secondEvaporativeCooler() const;

      double shutOffRelativeHumidity() const;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      bool setSupplyAirFan(const HVACComponent& hvacComponent);

      bool setDesignSupplyAirFlowRate(double designSupplyAirFlowRate);

      void autosizeDesignSupplyAirFlowRate();

      bool setFanPlacement(const std::string& fanPlacement);

      bool setCoolerUnitControlMethod(const std::string& coolerUnitControlMethod);

      bool setThrottlingRangeTemperatureDifference(double throttlingRangeTemperatureDifference);

      bool setCoolingLoadControlThresholdHeatTransferRate(double coolingLoadControlThresholdHeatTransferRate);

      bool setFirstEvaporativeCooler(const HVACComponent& hvacComponent);

      bool setSecondEvaporativeCooler(const HVACComponent& hvacComponent);

      void resetSecondEvaporativeCooler();

      bool setShutOffRelativeHumidity(double shutOffRelativeHumidity);

      //@}
      /** @name Other */
      //@{

      boost::optional<double> autosizedDesignSupplyAirFlowRate();

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ZoneHVACEvaporativeCoolerUnit");

      boost::optional<Schedule> optionalAvailabilitySchedule() const;
      boost::optional<HVACComponent> optionalSupplyAirFan() const;
      boost::optional<HVACComponent> optionalFirstEvaporativeCooler() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEHVACEVAPORATIVECOOLERUNIT_IMPL_HPP
