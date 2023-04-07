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

#ifndef MODEL_AIRLOOPHVACUNITARYHEATPUMPAIRTOAIR_IMPL_HPP
#define MODEL_AIRLOOPHVACUNITARYHEATPUMPAIRTOAIR_IMPL_HPP

#include "StraightComponent_Impl.hpp"

namespace openstudio {

namespace model {

  class Schedule;

  class HVACComponent;

  class ThermalZone;

  class Node;

  namespace detail {

    /** AirLoopHVACUnitaryHeatPumpAirToAir_Impl is a StraightComponent_Impl that is the implementation class for AirLoopHVACUnitaryHeatPumpAirToAir.*/
    class MODEL_API AirLoopHVACUnitaryHeatPumpAirToAir_Impl : public StraightComponent_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      AirLoopHVACUnitaryHeatPumpAirToAir_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirLoopHVACUnitaryHeatPumpAirToAir_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirLoopHVACUnitaryHeatPumpAirToAir_Impl(const AirLoopHVACUnitaryHeatPumpAirToAir_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AirLoopHVACUnitaryHeatPumpAirToAir_Impl() = default;

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

      virtual bool addToNode(Node& node) override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      Schedule availabilitySchedule() const;

      boost::optional<double> supplyAirFlowRateDuringCoolingOperation() const;

      bool isSupplyAirFlowRateDuringCoolingOperationAutosized() const;

      boost::optional<double> supplyAirFlowRateDuringHeatingOperation() const;

      bool isSupplyAirFlowRateDuringHeatingOperationAutosized() const;

      boost::optional<double> supplyAirFlowRateWhenNoCoolingorHeatingisNeeded() const;

      bool isSupplyAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const;

      boost::optional<ThermalZone> controllingZone() const;

      HVACComponent supplyAirFan() const;

      HVACComponent heatingCoil() const;

      HVACComponent coolingCoil() const;

      HVACComponent supplementalHeatingCoil() const;

      boost::optional<double> maximumSupplyAirTemperaturefromSupplementalHeater() const;

      bool isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized() const;

      double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation() const;

      bool isMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperationDefaulted() const;

      std::string fanPlacement() const;

      bool isFanPlacementDefaulted() const;

      boost::optional<Schedule> supplyAirFanOperatingModeSchedule() const;

      std::string dehumidificationControlType() const;

      bool isDehumidificationControlTypeDefaulted() const;

      boost::optional<double> autosizedSupplyAirFlowRateDuringCoolingOperation() const;

      boost::optional<double> autosizedSupplyAirFlowRateDuringHeatingOperation() const;

      boost::optional<double> autosizedSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded() const;

      boost::optional<double> autosizedMaximumSupplyAirTemperaturefromSupplementalHeater() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      bool setSupplyAirFlowRateDuringCoolingOperation(boost::optional<double> supplyAirFlowRateDuringCoolingOperation);

      void autosizeSupplyAirFlowRateDuringCoolingOperation();

      bool setSupplyAirFlowRateDuringHeatingOperation(boost::optional<double> n2);

      void autosizeSupplyAirFlowRateDuringHeatingOperation();

      bool setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(boost::optional<double> supplyAirFlowRateWhenNoCoolingorHeatingisNeeded);

      void resetSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();

      void autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();

      bool setControllingZone(ThermalZone& zone);

      void resetControllingZone();

      bool setSupplyAirFan(HVACComponent& hvacComponent);

      bool setHeatingCoil(HVACComponent& hvacComponent);

      bool setCoolingCoil(HVACComponent& hvacComponent);

      bool setSupplementalHeatingCoil(HVACComponent& hvacComponent);

      bool setMaximumSupplyAirTemperaturefromSupplementalHeater(boost::optional<double> maximumSupplyAirTemperaturefromSupplementalHeater);

      void autosizeMaximumSupplyAirTemperaturefromSupplementalHeater();

      bool setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation);

      void resetMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation();

      bool setFanPlacement(const std::string& fanPlacement);

      void resetFanPlacement();

      bool setSupplyAirFanOperatingModeSchedule(Schedule& schedule);

      void resetSupplyAirFanOperatingModeSchedule();

      bool setDehumidificationControlType(const std::string& dehumidificationControlType);

      void resetDehumidificationControlType();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AirLoopHVACUnitaryHeatPumpAirToAir");

      boost::optional<ModelObject> availabilityScheduleAsModelObject() const;
      boost::optional<ModelObject> controllingZoneAsModelObject() const;
      boost::optional<ModelObject> supplyAirFanAsModelObject() const;
      boost::optional<ModelObject> heatingCoilAsModelObject() const;
      boost::optional<ModelObject> coolingCoilAsModelObject() const;
      boost::optional<ModelObject> supplementalHeatingCoilAsModelObject() const;
      boost::optional<ModelObject> supplyAirFanOperatingModeScheduleAsModelObject() const;

      bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setControllingZoneAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setSupplyAirFanAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setHeatingCoilAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setCoolingCoilAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setSupplementalHeatingCoilAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setSupplyAirFanOperatingModeScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRLOOPHVACUNITARYHEATPUMPAIRTOAIR_IMPL_HPP
