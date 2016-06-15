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

#ifndef MODEL_ELECTRICLOADCENTERSTORAGESIMPLE_IMPL_HPP
#define MODEL_ELECTRICLOADCENTERSTORAGESIMPLE_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "ParentObject_Impl.hpp"

namespace openstudio {
namespace model {

// TODO: Check the following class names against object getters and setters.
class Schedule;
class Zone;

namespace detail {

  /** ElectricLoadCenterStorageSimple_Impl is a ParentObject_Impl that is the implementation class for ElectricLoadCenterStorageSimple.*/
  class MODEL_API ElectricLoadCenterStorageSimple_Impl : public ParentObject_Impl {
    Q_OBJECT;
   public:
    /** @name Constructors and Destructors */
    //@{

    ElectricLoadCenterStorageSimple_Impl(const IdfObject& idfObject,
                                         Model_Impl* model,
                                         bool keepHandle);

    ElectricLoadCenterStorageSimple_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                         Model_Impl* model,
                                         bool keepHandle);

    ElectricLoadCenterStorageSimple_Impl(const ElectricLoadCenterStorageSimple_Impl& other,
                                         Model_Impl* model,
                                         bool keepHandle);

    virtual ~ElectricLoadCenterStorageSimple_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    //@}
    /** @name Getters */
    //@{

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> availabilitySchedule() const;

    // TODO: Check return type. From object lists, some candidates are: Zone.
    boost::optional<Zone> zone() const;

    double radiativeFractionforZoneHeatGains() const;

    bool isRadiativeFractionforZoneHeatGainsDefaulted() const;

    double nominalEnergeticEfficiencyforCharging() const;

    bool isNominalEnergeticEfficiencyforChargingDefaulted() const;

    double nominalDischargingEnergeticEfficiency() const;

    bool isNominalDischargingEnergeticEfficiencyDefaulted() const;

    double maximumStorageCapacity() const;

    double maximumPowerforDischarging() const;

    double maximumPowerforCharging() const;

    boost::optional<double> initialStateofCharge() const;

    //@}
    /** @name Setters */
    //@{

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    // TODO: Check argument type. From object lists, some candidates are: Zone.
    bool setZone(const boost::optional<Zone>& zone);

    void resetZone();

    bool setRadiativeFractionforZoneHeatGains(double radiativeFractionforZoneHeatGains);

    void resetRadiativeFractionforZoneHeatGains();

    bool setNominalEnergeticEfficiencyforCharging(double nominalEnergeticEfficiencyforCharging);

    void resetNominalEnergeticEfficiencyforCharging();

    bool setNominalDischargingEnergeticEfficiency(double nominalDischargingEnergeticEfficiency);

    void resetNominalDischargingEnergeticEfficiency();

    void setMaximumStorageCapacity(double maximumStorageCapacity);

    bool setMaximumPowerforDischarging(double maximumPowerforDischarging);

    bool setMaximumPowerforCharging(double maximumPowerforCharging);

    void setInitialStateofCharge(boost::optional<double> initialStateofCharge);

    void resetInitialStateofCharge();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.ElectricLoadCenterStorageSimple");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_ELECTRICLOADCENTERSTORAGESIMPLE_IMPL_HPP

