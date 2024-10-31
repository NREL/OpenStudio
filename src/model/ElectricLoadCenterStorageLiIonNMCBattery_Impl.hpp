/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

      virtual ~ElectricLoadCenterStorageLiIonNMCBattery_Impl() override = default;

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
