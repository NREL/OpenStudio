/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#ifndef MODEL_REFRIGERATIONWALKIN_IMPL_HPP
#define MODEL_REFRIGERATIONWALKIN_IMPL_HPP

#include <model/ModelAPI.hpp>
#include <model/ModelObject_Impl.hpp>

namespace openstudio {
namespace model {

// TODO: Check the following class names against object getters and setters.
class Schedule;
class RefrigerationWalkInZoneBoundary;

namespace detail {

  /** RefrigerationWalkIn_Impl is a ModelObject_Impl that is the implementation class for RefrigerationWalkIn.*/
  class MODEL_API RefrigerationWalkIn_Impl : public ModelObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    RefrigerationWalkIn_Impl(const IdfObject& idfObject,
                             Model_Impl* model,
                             bool keepHandle);

    RefrigerationWalkIn_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                             Model_Impl* model,
                             bool keepHandle);

    RefrigerationWalkIn_Impl(const RefrigerationWalkIn_Impl& other,
                             Model_Impl* model,
                             bool keepHandle);

    virtual ~RefrigerationWalkIn_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    virtual ModelObject clone(Model model) const;

    virtual std::vector<IdfObject> remove();

    bool addZoneBoundary(const RefrigerationWalkInZoneBoundary& refrigerationWalkInZoneBoundary);

    void removeZoneBoundary(unsigned groupIndex);

    void removeAllZoneBoundaries();

    std::vector<RefrigerationWalkInZoneBoundary> zoneBoundaries() const;

    //@}
    /** @name Getters */
    //@{

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> availabilitySchedule() const;

    double ratedCoilCoolingCapacity() const;

    double operatingTemperature() const;

    double ratedCoolingSourceTemperature() const;

    double ratedTotalHeatingPower() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> heatingPowerSchedule() const;

    double ratedCoolingCoilFanPower() const;

    bool isRatedCoolingCoilFanPowerDefaulted() const;

    double ratedCirculationFanPower() const;

    bool isRatedCirculationFanPowerDefaulted() const;

    double ratedTotalLightingPower() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> lightingSchedule() const;

    std::string defrostType() const;

    bool isDefrostTypeDefaulted() const;

    std::string defrostControlType() const;

    bool isDefrostControlTypeDefaulted() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    Schedule defrostSchedule() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> defrostDripDownSchedule() const;

    boost::optional<double> defrostPower() const;

    boost::optional<double> temperatureTerminationDefrostFractiontoIce() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> restockingSchedule() const;

    double averageRefrigerantChargeInventory() const;

    bool isAverageRefrigerantChargeInventoryDefaulted() const;

    double insulatedFloorSurfaceArea() const;

    double insulatedFloorUValue() const;

    bool isInsulatedFloorUValueDefaulted() const;

    // TODO: Handle this object's extensible fields.

    //@}
    /** @name Setters */
    //@{

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    void setRatedCoilCoolingCapacity(double ratedCoilCoolingCapacity);

    bool setOperatingTemperature(double operatingTemperature);

    bool setRatedCoolingSourceTemperature(double ratedCoolingSourceTemperature);

    void setRatedTotalHeatingPower(double ratedTotalHeatingPower);

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    bool setHeatingPowerSchedule(Schedule& schedule);

    void resetHeatingPowerSchedule();

    bool setRatedCoolingCoilFanPower(double ratedCoolingCoilFanPower);

    void resetRatedCoolingCoilFanPower();

    bool setRatedCirculationFanPower(double ratedCirculationFanPower);

    void resetRatedCirculationFanPower();

    void setRatedTotalLightingPower(double ratedTotalLightingPower);

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    bool setLightingSchedule(Schedule& schedule);

    void resetLightingSchedule();

    bool setDefrostType(std::string defrostType);

    void resetDefrostType();

    bool setDefrostControlType(std::string defrostControlType);

    void resetDefrostControlType();

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    bool setDefrostSchedule(Schedule& schedule);

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    bool setDefrostDripDownSchedule(Schedule& schedule);

    void resetDefrostDripDownSchedule();

    bool setDefrostPower(boost::optional<double> defrostPower);

    void resetDefrostPower();

    bool setTemperatureTerminationDefrostFractiontoIce(boost::optional<double> temperatureTerminationDefrostFractiontoIce);

    void resetTemperatureTerminationDefrostFractiontoIce();

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    bool setRestockingSchedule(Schedule& schedule);

    void resetRestockingSchedule();

    void setAverageRefrigerantChargeInventory(double averageRefrigerantChargeInventory);

    void resetAverageRefrigerantChargeInventory();

    bool setInsulatedFloorSurfaceArea(double insulatedFloorSurfaceArea);

    bool setInsulatedFloorUValue(double insulatedFloorUValue);

    void resetInsulatedFloorUValue();

    // TODO: Handle this object's extensible fields.

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.RefrigerationWalkIn");

    // TODO: Check the return types of these methods.
    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<Schedule> optionalDefrostSchedule() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_REFRIGERATIONWALKIN_IMPL_HPP

