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

#ifndef MODEL_ELECTRICLOADCENTERSTORAGELIIONNMCBATTERY_IMPL_HPP
#define MODEL_ELECTRICLOADCENTERSTORAGELIIONNMCBATTERY_IMPL_HPP

#include "ModelAPI.hpp"
#include "ElectricalStorage_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class ThermalZone;

  namespace detail {

    /** ElectricLoadCenterStorageLiIonNMCBattery_Impl is a ElectricalStorage_Impl that is the implementation class for ElectricLoadCenterStorageLiIonNMCBattery.*/
    class MODEL_API ElectricLoadCenterStorageLiIonNMCBattery_Impl : public ElectricalStorage_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ElectricLoadCenterStorageLiIonNMCBattery_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ElectricLoadCenterStorageLiIonNMCBattery_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ElectricLoadCenterStorageLiIonNMCBattery_Impl(const ElectricLoadCenterStorageLiIonNMCBattery_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ElectricLoadCenterStorageLiIonNMCBattery_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual boost::optional<ThermalZone> thermalZone() const override;

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

      // Override ElectricalStorage (Base class) methods
      virtual bool setThermalZone(const ThermalZone& thermalZone) override;
      virtual void resetThermalZone() override;

      //@}
      /** @name Getters */
      //@{

      // Convenience method to return the electricalLoadCenter on which it's assigned (optional)
      // Included in Base Class
      // boost::optional<ElectricLoadCenterDistribution> electricLoadCenterDistribution() const;

      Schedule availabilitySchedule() const;

      double radiativeFraction() const;

      std::string lifetimeModel() const;

      int numberofCellsinSeries() const;

      int numberofStringsinParallel() const;

      double initialFractionalStateofCharge() const;

      double dctoDCChargingEfficiency() const;

      double batteryMass() const;

      double batterySurfaceArea() const;

      double batterySpecificHeatCapacity() const;

      double heatTransferCoefficientBetweenBatteryandAmbient() const;

      double fullyChargedCellVoltage() const;

      double cellVoltageatEndofExponentialZone() const;

      double cellVoltageatEndofNominalZone() const;

      double defaultNominalCellVoltage() const;

      double fullyChargedCellCapacity() const;

      double fractionofCellCapacityRemovedattheEndofExponentialZone() const;

      double fractionofCellCapacityRemovedattheEndofNominalZone() const;

      double chargeRateatWhichVoltagevsCapacityCurveWasGenerated() const;

      double batteryCellInternalElectricalResistance() const;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      bool setRadiativeFraction(double radiativeFraction);

      bool setLifetimeModel(const std::string& lifetimeModel);

      bool setNumberofCellsinSeries(int numberofCellsinSeries);

      bool setNumberofStringsinParallel(int numberofStringsinParallel);

      bool setInitialFractionalStateofCharge(double initialFractionalStateofCharge);

      bool setDCtoDCChargingEfficiency(double dctoDCChargingEfficiency);

      bool setBatteryMass(double batteryMass);

      bool setBatterySurfaceArea(double batterySurfaceArea);

      bool setBatterySpecificHeatCapacity(double batterySpecificHeatCapacity);

      bool setHeatTransferCoefficientBetweenBatteryandAmbient(double heatTransferCoefficientBetweenBatteryandAmbient);

      bool setFullyChargedCellVoltage(double fullyChargedCellCapacity);

      bool setCellVoltageatEndofExponentialZone(double cellVoltageatEndofExponentialZone);

      bool setCellVoltageatEndofNominalZone(double cellVoltageatEndofNominalZone);

      bool setDefaultNominalCellVoltage(double defaultNominalCellVoltage);

      bool setFullyChargedCellCapacity(double fullyChargedCellCapacity);

      bool setFractionofCellCapacityRemovedattheEndofExponentialZone(double fractionofCellCapacityRemovedattheEndofExponentialZone);

      bool setFractionofCellCapacityRemovedattheEndofNominalZone(double fractionofCellCapacityRemovedattheEndofNominalZone);

      bool setChargeRateatWhichVoltagevsCapacityCurveWasGenerated(double chargeRateatWhichVoltagevsCapacityCurveWasGenerated);

      bool setBatteryCellInternalElectricalResistance(double batteryCellInternalElectricalResistance);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ElectricLoadCenterStorageLiIonNMCBattery");
      boost::optional<Schedule> optionalAvailabilitySchedule() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ELECTRICLOADCENTERSTORAGELIIONNMCBATTERY_IMPL_HPP
