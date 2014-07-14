/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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
  class MODEL_API ZoneHVACLowTemperatureRadiantElectric_Impl : public ZoneHVACComponent_Impl {   
   public:
    /** @name Constructors and Destructors */

    ZoneHVACLowTemperatureRadiantElectric_Impl(const IdfObject& idfObject,
                                               Model_Impl* model,
                                               bool keepHandle);

    ZoneHVACLowTemperatureRadiantElectric_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                               Model_Impl* model,
                                               bool keepHandle);

    ZoneHVACLowTemperatureRadiantElectric_Impl(const ZoneHVACLowTemperatureRadiantElectric_Impl& other,
                                               Model_Impl* model,
                                               bool keepHandle);

    virtual ~ZoneHVACLowTemperatureRadiantElectric_Impl() {}
    
    virtual unsigned inletPort();

    virtual unsigned outletPort();

    /** @name Virtual Methods */

    virtual ModelObject clone(Model model) const;
    
    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

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

    double heatingThrottlingRange() const;

    bool isHeatingThrottlingRangeDefaulted() const;

    /** @name Setters */

    bool setAvailabilitySchedule(Schedule& schedule);
    
    bool setHeatingSetpointTemperatureSchedule(Schedule& schedule);

    bool setRadiantSurfaceType(boost::optional<std::string> radiantSurfaceType);

    void resetRadiantSurfaceType();

    bool setMaximumElectricalPowertoPanel(boost::optional<double> maximumElectricalPowertoPanel);

    void resetMaximumElectricalPowertoPanel();

    void autosizeMaximumElectricalPowertoPanel();

    bool setTemperatureControlType(std::string temperatureControlType);

    void resetTemperatureControlType();

    bool setHeatingThrottlingRange(double heatingThrottlingRange);

    void resetHeatingThrottlingRange();
    
    boost::optional<ThermalZone> thermalZone() const;

    bool addToThermalZone(ThermalZone & thermalZone);

    void removeFromThermalZone();

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

} // detail

} // model
} // openstudio

#endif // MODEL_ZONEHVACLOWTEMPERATURERADIANTELECTRIC_IMPL_HPP

