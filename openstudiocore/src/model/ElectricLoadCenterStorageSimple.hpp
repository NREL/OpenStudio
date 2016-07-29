/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

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

} // detail

/** ElectricLoadCenterStorageSimple is a ElectricalStorage that wraps the OpenStudio IDD object 'OS:ElectricLoadCenter:Storage:Simple'. */
class MODEL_API ElectricLoadCenterStorageSimple : public ElectricalStorage {
  public:
    /** @name Constructors and Destructors */
    //@{

    explicit ElectricLoadCenterStorageSimple(const Model& model);

    virtual ~ElectricLoadCenterStorageSimple() {}

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
    typedef detail::ElectricLoadCenterStorageSimple_Impl ImplType;

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
typedef boost::optional<ElectricLoadCenterStorageSimple> OptionalElectricLoadCenterStorageSimple;

/** \relates ElectricLoadCenterStorageSimple*/
typedef std::vector<ElectricLoadCenterStorageSimple> ElectricLoadCenterStorageSimpleVector;

} // model
} // openstudio

#endif // MODEL_ELECTRICLOADCENTERSTORAGESIMPLE_HPP

