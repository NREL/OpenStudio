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

#ifndef MODEL_ZONEHVACLOWTEMPERATURERADIANTELECTRIC_IMPL_HPP
#define MODEL_ZONEHVACLOWTEMPERATURERADIANTELECTRIC_IMPL_HPP

#include "ModelAPI.hpp"
#include "Surface.hpp"
#include "Surface_Impl.hpp"
#include "ZoneHVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class HVACComponent;

  namespace detail {

    /** ZoneHVACLowTemperatureRadiantElectric_Impl is a ZoneHVAC_Impl that is the implementation class for ZoneHVACLowTemperatureRadiantElectric.*/
    class MODEL_API ZoneHVACLowTemperatureRadiantElectric_Impl : public ZoneHVACComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */

      ZoneHVACLowTemperatureRadiantElectric_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ZoneHVACLowTemperatureRadiantElectric_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ZoneHVACLowTemperatureRadiantElectric_Impl(const ZoneHVACLowTemperatureRadiantElectric_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ZoneHVACLowTemperatureRadiantElectric_Impl() = default;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      /** @name Virtual Methods */

      virtual ModelObject clone(Model model) const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      /** @name Getters */

      Schedule availabilitySchedule() const;

      Schedule heatingSetpointTemperatureSchedule() const;

      boost::optional<std::string> radiantSurfaceType() const;

      std::vector<Surface> surfaces() const;

      boost::optional<double> maximumElectricalPowertoPanel() const;

      bool isMaximumElectricalPowertoPanelDefaulted() const;

      bool isMaximumElectricalPowertoPanelAutosized() const;

      std::string temperatureControlType() const;

      bool isTemperatureControlTypeDefaulted() const;

      std::string setpointControlType() const;

      bool isSetpointControlTypeDefaulted() const;

      double heatingThrottlingRange() const;

      bool isHeatingThrottlingRangeDefaulted() const;

      boost::optional<double> autosizedMaximumElectricalPowertoPanel() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      /** @name Setters */

      bool setAvailabilitySchedule(Schedule& schedule);

      bool setHeatingSetpointTemperatureSchedule(Schedule& schedule);

      bool setRadiantSurfaceType(boost::optional<std::string> radiantSurfaceType);

      void resetRadiantSurfaceType();

      bool setMaximumElectricalPowertoPanel(boost::optional<double> maximumElectricalPowertoPanel);

      void resetMaximumElectricalPowertoPanel();

      void autosizeMaximumElectricalPowertoPanel();

      bool setTemperatureControlType(const std::string& temperatureControlType);

      void resetTemperatureControlType();

      bool setSetpointControlType(const std::string& setpointControlType);

      void resetSetpointControlType();

      bool setHeatingThrottlingRange(double heatingThrottlingRange);

      void resetHeatingThrottlingRange();

      boost::optional<ThermalZone> thermalZone() const override;

      bool addToThermalZone(ThermalZone& thermalZone) override;

      void removeFromThermalZone() override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ZoneHVACLowTemperatureRadiantElectric");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<Schedule> optionalAvailabilitySchedule() const;
      boost::optional<Schedule> optionalHeatingSetpointTemperatureSchedule() const;

      std::vector<std::string> radiantSurfaceTypeValues() const;
      std::vector<std::string> temperatureControlTypeValues() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEHVACLOWTEMPERATURERADIANTELECTRIC_IMPL_HPP
