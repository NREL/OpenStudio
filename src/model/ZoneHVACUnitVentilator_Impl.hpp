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

#ifndef MODEL_ZONEHVACUNITVENTILATOR_IMPL_HPP
#define MODEL_ZONEHVACUNITVENTILATOR_IMPL_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class HVACComponent;

  namespace detail {

    /** ZoneHVACUnitVentilator_Impl is a ZoneHVACComponent_Impl that is the implementation class for ZoneHVACUnitVentilator.*/
    class MODEL_API ZoneHVACUnitVentilator_Impl : public ZoneHVACComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ZoneHVACUnitVentilator_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ZoneHVACUnitVentilator_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ZoneHVACUnitVentilator_Impl(const ZoneHVACUnitVentilator_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ZoneHVACUnitVentilator_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<IdfObject> remove() override;

      virtual std::vector<ModelObject> children() const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      Schedule availabilitySchedule() const;

      boost::optional<double> maximumSupplyAirFlowRate() const;

      bool isMaximumSupplyAirFlowRateAutosized() const;

      std::string outdoorAirControlType() const;

      boost::optional<double> minimumOutdoorAirFlowRate() const;

      bool isMinimumOutdoorAirFlowRateAutosized() const;

      Schedule minimumOutdoorAirSchedule() const;

      boost::optional<double> maximumOutdoorAirFlowRate() const;

      bool isMaximumOutdoorAirFlowRateAutosized() const;

      Schedule maximumOutdoorAirFractionorTemperatureSchedule() const;

      HVACComponent supplyAirFan() const;

      boost::optional<Schedule> supplyAirFanOperatingModeSchedule() const;

      boost::optional<HVACComponent> heatingCoil() const;

      double heatingConvergenceTolerance() const;

      boost::optional<HVACComponent> coolingCoil() const;

      double coolingConvergenceTolerance() const;

      // boost::optional<ModelObject> availabilityManagerList() const;

      // boost::optional<ModelObject> designSpecificationZoneHVACSizingObject() const;

      boost::optional<double> autosizedMaximumSupplyAirFlowRate() const;

      boost::optional<double> autosizedMinimumOutdoorAirFlowRate() const;

      boost::optional<double> autosizedMaximumOutdoorAirFlowRate() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      bool setMaximumSupplyAirFlowRate(boost::optional<double> maximumSupplyAirFlowRate);

      void autosizeMaximumSupplyAirFlowRate();

      bool setOutdoorAirControlType(const std::string& outdoorAirControlType);

      bool setMinimumOutdoorAirFlowRate(boost::optional<double> minimumOutdoorAirFlowRate);

      void autosizeMinimumOutdoorAirFlowRate();

      bool setMinimumOutdoorAirSchedule(Schedule& schedule);

      bool setMaximumOutdoorAirFlowRate(boost::optional<double> maximumOutdoorAirFlowRate);

      void autosizeMaximumOutdoorAirFlowRate();

      bool setMaximumOutdoorAirFractionorTemperatureSchedule(Schedule& schedule);

      bool setSupplyAirFan(const HVACComponent& supplyAirFan);

      bool setSupplyAirFanOperatingModeSchedule(Schedule& schedule);

      void resetSupplyAirFanOperatingModeSchedule();

      bool setHeatingCoil(const boost::optional<HVACComponent>& heatingCoil);

      void resetHeatingCoil();

      bool setHeatingConvergenceTolerance(double heatingConvergenceTolerance);

      bool setCoolingCoil(const boost::optional<HVACComponent>& coolingCoil);

      void resetCoolingCoil();

      bool setCoolingConvergenceTolerance(double coolingConvergenceTolerance);

      // bool setAvailabilityManagerList(const boost::optional<ModelObject>& systemAvailabilityManagerLists);

      // void resetAvailabilityManagerList();

      // bool setDesignSpecificationZoneHVACSizingObject(const boost::optional<ModelObject>& designSpecificationZoneHVACSizingName);

      // void resetDesignSpecificationZoneHVACSizingObject();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ZoneHVACUnitVentilator");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<Schedule> optionalAvailabilitySchedule() const;
      boost::optional<Schedule> optionalMinimumOutdoorAirSchedule() const;
      boost::optional<Schedule> optionalMaximumOutdoorAirFractionorTemperatureSchedule() const;
      boost::optional<HVACComponent> optionalSupplyAirFan() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEHVACUNITVENTILATOR_IMPL_HPP
