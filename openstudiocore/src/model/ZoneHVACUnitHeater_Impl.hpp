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

#ifndef MODEL_ZONEHVACUNITHEATER_IMPL_HPP
#define MODEL_ZONEHVACUNITHEATER_IMPL_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent_Impl.hpp"

#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"

namespace openstudio {
namespace model {

class Schedule;
class HVACComponent;

namespace detail {

  /** ZoneHVACUnitHeater_Impl is a ZoneHVACComponent_Impl that is the implementation class for ZoneHVACUnitHeater.*/

  class MODEL_API ZoneHVACUnitHeater_Impl : public ZoneHVACComponent_Impl
  {
   public:
    /** @name Constructors and Destructors */

    ZoneHVACUnitHeater_Impl(const IdfObject& idfObject,
                            Model_Impl* model,
                            bool keepHandle);

    ZoneHVACUnitHeater_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                            Model_Impl* model,
                            bool keepHandle);

    ZoneHVACUnitHeater_Impl(const ZoneHVACUnitHeater_Impl& other,
                            Model_Impl* model,
                            bool keepHandle);

    virtual ~ZoneHVACUnitHeater_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual ModelObject clone(Model model) const;

    virtual std::vector<IdfObject> remove();

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    virtual std::vector<ModelObject> children() const;

    virtual unsigned inletPort();

    virtual unsigned outletPort();

    //@}
    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    HVACComponent supplyAirFan() const;

    boost::optional<double> maximumSupplyAirFlowRate() const;

    bool isMaximumSupplyAirFlowRateAutosized() const;

    std::string fanControlType() const;

    HVACComponent heatingCoil() const;

    boost::optional<double> maximumHotWaterFlowRate() const;

    bool isMaximumHotWaterFlowRateAutosized() const;

    double minimumHotWaterFlowRate() const;

    bool isMinimumHotWaterFlowRateDefaulted() const;

    double heatingConvergenceTolerance() const;

    bool isHeatingConvergenceToleranceDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setSupplyAirFan(const HVACComponent & hvacComponent );

    bool setMaximumSupplyAirFlowRate(boost::optional<double> maximumSupplyAirFlowRate);

    void autosizeMaximumSupplyAirFlowRate();

    bool setFanControlType(std::string fanControlType);

    bool setHeatingCoil(const HVACComponent & hvacComponent );

    bool setMaximumHotWaterFlowRate(boost::optional<double> maximumHotWaterFlowRate);

    void resetMaximumHotWaterFlowRate();

    void autosizeMaximumHotWaterFlowRate();

    bool setMinimumHotWaterFlowRate(double minimumHotWaterFlowRate);

    void resetMinimumHotWaterFlowRate();

    bool setHeatingConvergenceTolerance(double heatingConvergenceTolerance);

    void resetHeatingConvergenceTolerance();

    //@}
    /** @name Other */
    //@{

   protected:
   private:

    REGISTER_LOGGER("openstudio.model.ZoneHVACUnitHeater");

    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    boost::optional<Schedule> optionalAvailabilitySchedule() const;
    boost::optional<HVACComponent> optionalSupplyAirFan() const;
    boost::optional<HVACComponent> optionalHeatingCoil() const;

    std::vector<std::string> fanControlTypeValues() const;

    boost::optional<ModelObject> availabilityScheduleAsModelObject() const;
    boost::optional<ModelObject> supplyAirFanAsModelObject() const;
    boost::optional<ModelObject> heatingCoilAsModelObject() const;

    bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setSupplyAirFanAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setHeatingCoilAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_ZONEHVACUNITHEATER_IMPL_HPP

