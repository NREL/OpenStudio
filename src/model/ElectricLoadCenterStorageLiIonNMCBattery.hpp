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

#ifndef MODEL_ELECTRICLOADCENTERSTORAGELIIONNMCBATTERY_HPP
#define MODEL_ELECTRICLOADCENTERSTORAGELIIONNMCBATTERY_HPP

#include "ModelAPI.hpp"
#include "ElectricalStorage.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class ThermalZone;

  namespace detail {

    class ElectricLoadCenterStorageLiIonNMCBattery_Impl;

  }  // namespace detail

  /** ElectricLoadCenterStorageLiIonNMCBattery is a ElectricalStorage that wraps the OpenStudio IDD object 'OS:ElectricLoadCenter:Storage:LiIonNMCBattery'. */
  class MODEL_API ElectricLoadCenterStorageLiIonNMCBattery : public ElectricalStorage
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ElectricLoadCenterStorageLiIonNMCBattery(const Model& model);

    explicit ElectricLoadCenterStorageLiIonNMCBattery(const Model& model, const int numberofCellsinSeries, const int numberofStringsinParallel,
                                                      const double batteryMass, const double batterySurfaceArea);

    virtual ~ElectricLoadCenterStorageLiIonNMCBattery() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ElectricLoadCenterStorageLiIonNMCBattery(const ElectricLoadCenterStorageLiIonNMCBattery& other) = default;
    ElectricLoadCenterStorageLiIonNMCBattery(ElectricLoadCenterStorageLiIonNMCBattery&& other) = default;
    ElectricLoadCenterStorageLiIonNMCBattery& operator=(const ElectricLoadCenterStorageLiIonNMCBattery&) = default;
    ElectricLoadCenterStorageLiIonNMCBattery& operator=(ElectricLoadCenterStorageLiIonNMCBattery&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> lifetimeModelValues();

    /** @name Getters */
    //@{

    // Convenience method to return the electricalLoadCenter on which it's assigned (optional)
    // In parent
    // boost::optional<ElectricLoadCenterDistribution> electricLoadCenterDistribution() const;

    Schedule availabilitySchedule() const;

    // Included in parent class, shouldn't need to define it here...
    //virtual boost::optional<ThermalZone> thermalZone() const override;

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

    // Included in parent class, shouldn't need to define it...
    //virtual bool setThermalZone(ThermalZone& zone) override;
    //virtual void resetThermalZone() override;

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
    /// @cond
    using ImplType = detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl;

    explicit ElectricLoadCenterStorageLiIonNMCBattery(std::shared_ptr<detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl> impl);

    friend class detail::ElectricLoadCenterStorageLiIonNMCBattery_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ElectricLoadCenterStorageLiIonNMCBattery");
  };

  /** \relates ElectricLoadCenterStorageLiIonNMCBattery*/
  using OptionalElectricLoadCenterStorageLiIonNMCBattery = boost::optional<ElectricLoadCenterStorageLiIonNMCBattery>;

  /** \relates ElectricLoadCenterStorageLiIonNMCBattery*/
  using ElectricLoadCenterStorageLiIonNMCBatteryVector = std::vector<ElectricLoadCenterStorageLiIonNMCBattery>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ELECTRICLOADCENTERSTORAGELIIONNMCBATTERY_HPP
