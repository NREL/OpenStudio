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

#ifndef MODEL_ELECTRICLOADCENTERSTORAGESIMPLE_HPP
#define MODEL_ELECTRICLOADCENTERSTORAGESIMPLE_HPP

#include "ModelAPI.hpp"
#include "ElectricalStorage.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class ThermalZone;

  namespace detail {

    class ElectricLoadCenterStorageSimple_Impl;

  }  // namespace detail

  /** ElectricLoadCenterStorageSimple is a ElectricalStorage that wraps the OpenStudio IDD object 'OS:ElectricLoadCenter:Storage:Simple'. */
  class MODEL_API ElectricLoadCenterStorageSimple : public ElectricalStorage
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ElectricLoadCenterStorageSimple(const Model& model);

    virtual ~ElectricLoadCenterStorageSimple() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ElectricLoadCenterStorageSimple(const ElectricLoadCenterStorageSimple& other) = default;
    ElectricLoadCenterStorageSimple(ElectricLoadCenterStorageSimple&& other) = default;
    ElectricLoadCenterStorageSimple& operator=(const ElectricLoadCenterStorageSimple&) = default;
    ElectricLoadCenterStorageSimple& operator=(ElectricLoadCenterStorageSimple&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    // Convenience method to return the electricalLoadCenter on which it's assigned (optional)
    // In parent
    // boost::optional<ElectricLoadCenterDistribution> electricLoadCenterDistribution() const;

    Schedule availabilitySchedule() const;
    bool isAvailabilityScheduleDefaulted() const;

    // TODO: Included in parent class, shouldn't need to define it here...
    //virtual boost::optional<ThermalZone> thermalZone() const override;

    double radiativeFractionforZoneHeatGains() const;
    bool isRadiativeFractionforZoneHeatGainsDefaulted() const;

    double nominalEnergeticEfficiencyforCharging() const;
    bool isNominalEnergeticEfficiencyforChargingDefaulted() const;

    double nominalDischargingEnergeticEfficiency() const;
    bool isNominalDischargingEnergeticEfficiencyDefaulted() const;

    double maximumStorageCapacity() const;

    double maximumPowerforDischarging() const;

    double maximumPowerforCharging() const;

    double initialStateofCharge() const;
    bool isInitialStateofChargeDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);
    void resetAvailabilitySchedule();

    // TODO: Included in parent class, shouldn't need to define it...
    //virtual bool setThermalZone(ThermalZone& zone) override;
    //virtual void resetThermalZone() override;

    bool setRadiativeFractionforZoneHeatGains(double radiativeFractionforZoneHeatGains);
    void resetRadiativeFractionforZoneHeatGains();

    bool setNominalEnergeticEfficiencyforCharging(double nominalEnergeticEfficiencyforCharging);
    void resetNominalEnergeticEfficiencyforCharging();

    // TODO: I've requested an .IDD change in EnergyPlus, to make this "Nominal Energetic Efficiency for Discharging"
    // TODO: https://github.com/NREL/EnergyPlus/issues/5730
    bool setNominalDischargingEnergeticEfficiency(double nominalDischargingEnergeticEfficiency);
    void resetNominalDischargingEnergeticEfficiency();

    bool setMaximumStorageCapacity(double maximumStorageCapacity);

    bool setMaximumPowerforDischarging(double maximumPowerforDischarging);

    bool setMaximumPowerforCharging(double maximumPowerforCharging);

    bool setInitialStateofCharge(double initialStateofCharge);
    void resetInitialStateofCharge();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::ElectricLoadCenterStorageSimple_Impl;

    explicit ElectricLoadCenterStorageSimple(std::shared_ptr<detail::ElectricLoadCenterStorageSimple_Impl> impl);

    friend class detail::ElectricLoadCenterStorageSimple_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ElectricLoadCenterStorageSimple");
  };

  /** \relates ElectricLoadCenterStorageSimple*/
  using OptionalElectricLoadCenterStorageSimple = boost::optional<ElectricLoadCenterStorageSimple>;

  /** \relates ElectricLoadCenterStorageSimple*/
  using ElectricLoadCenterStorageSimpleVector = std::vector<ElectricLoadCenterStorageSimple>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ELECTRICLOADCENTERSTORAGESIMPLE_HPP
