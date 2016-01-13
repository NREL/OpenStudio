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

#ifndef MODEL_ZONEHVACHIGHTEMPERATURERADIANT_IMPL_HPP
#define MODEL_ZONEHVACHIGHTEMPERATURERADIANT_IMPL_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;

namespace detail {

  /** ZoneHVACHighTemperatureRadiant_Impl is a ZoneHVACComponent_Impl that is the implementation class for ZoneHVACHighTemperatureRadiant.*/
  class MODEL_API ZoneHVACHighTemperatureRadiant_Impl : public ZoneHVACComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    ZoneHVACHighTemperatureRadiant_Impl(const IdfObject& idfObject,
                                        Model_Impl* model,
                                        bool keepHandle);

    ZoneHVACHighTemperatureRadiant_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                        Model_Impl* model,
                                        bool keepHandle);

    ZoneHVACHighTemperatureRadiant_Impl(const ZoneHVACHighTemperatureRadiant_Impl& other,
                                        Model_Impl* model,
                                        bool keepHandle);

    virtual ~ZoneHVACHighTemperatureRadiant_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    //@}
    /** @name Getters */
    //@{

    std::vector<Surface> surfaces() const;

    boost::optional<Schedule> availabilitySchedule() const;

    boost::optional<double> maximumPowerInput() const;

    bool isMaximumPowerInputAutosized() const;

    std::string fuelType() const;

    double combustionEfficiency() const;

    double fractionofInputConvertedtoRadiantEnergy() const;

    double fractionofInputConvertedtoLatentEnergy() const;

    double fractionofInputthatIsLost() const;

    std::string temperatureControlType() const;

    double heatingThrottlingRange() const;

    boost::optional<Schedule> heatingSetpointTemperatureSchedule() const;

    double fractionofRadiantEnergyIncidentonPeople() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setMaximumPowerInput(boost::optional<double> maximumPowerInput);

    void autosizeMaximumPowerInput();

    bool setFuelType(std::string fuelType);

    bool setCombustionEfficiency(double combustionEfficiency);

    bool setFractionofInputConvertedtoRadiantEnergy(double fractionofInputConvertedtoRadiantEnergy);

    bool setFractionofInputConvertedtoLatentEnergy(double fractionofInputConvertedtoLatentEnergy);

    bool setFractionofInputthatIsLost(double fractionofInputthatIsLost);

    bool setTemperatureControlType(std::string temperatureControlType);

    bool setHeatingThrottlingRange(double heatingThrottlingRange);

    bool setHeatingSetpointTemperatureSchedule(Schedule& schedule);

    void resetHeatingSetpointTemperatureSchedule();

    bool setFractionofRadiantEnergyIncidentonPeople(double fractionofRadiantEnergyIncidentonPeople);

    //@}
    /** @name Other */
    //@{

    boost::optional<ThermalZone> thermalZone() const;

    bool addToThermalZone(ThermalZone & thermalZone) override;

    void removeFromThermalZone() override;

    virtual unsigned inletPort() const override;

    virtual unsigned outletPort() const override;
    //@}

   protected:

   private:
    REGISTER_LOGGER("openstudio.model.ZoneHVACHighTemperatureRadiant");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_ZONEHVACHIGHTEMPERATURERADIANT_IMPL_HPP

