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

#ifndef MODEL_ZONEHVACENERGYRECOVERYVENTILATOR_IMPL_HPP
#define MODEL_ZONEHVACENERGYRECOVERYVENTILATOR_IMPL_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;
class HVACComponent;
class ZoneHVACEnergyRecoveryVentilatorController;

namespace detail {

  /** ZoneHVACEnergyRecoveryVentilator_Impl is a ZoneHVACComponent_Impl that is the implementation class for ZoneHVACEnergyRecoveryVentilator.*/
  class MODEL_API ZoneHVACEnergyRecoveryVentilator_Impl : public ZoneHVACComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    ZoneHVACEnergyRecoveryVentilator_Impl(const IdfObject& idfObject,
                                          Model_Impl* model,
                                          bool keepHandle);

    ZoneHVACEnergyRecoveryVentilator_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                          Model_Impl* model,
                                          bool keepHandle);

    ZoneHVACEnergyRecoveryVentilator_Impl(const ZoneHVACEnergyRecoveryVentilator_Impl& other,
                                          Model_Impl* model,
                                          bool keepHandle);

    virtual ~ZoneHVACEnergyRecoveryVentilator_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    virtual ModelObject clone(Model model) const override;

    virtual std::vector<ModelObject> children() const override;

    virtual unsigned inletPort() const override;

    virtual unsigned outletPort() const override;

    //@}
    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    HVACComponent heatExchanger() const;

    boost::optional<double> supplyAirFlowRate() const;

    bool isSupplyAirFlowRateAutosized() const;

    boost::optional<double> exhaustAirFlowRate() const;

    bool isExhaustAirFlowRateAutosized() const;

    HVACComponent supplyAirFan() const;

    HVACComponent exhaustAirFan() const;

    boost::optional<ZoneHVACEnergyRecoveryVentilatorController> controller() const;

    double ventilationRateperUnitFloorArea() const;

    double ventilationRateperOccupant() const;

    // boost::optional<ModelObject> availabilityManagerList() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setHeatExchanger(const HVACComponent& hxAirToAirSensibleAndLatent);

    bool setSupplyAirFlowRate(boost::optional<double> supplyAirFlowRate);

    void autosizeSupplyAirFlowRate();

    bool setExhaustAirFlowRate(boost::optional<double> exhaustAirFlowRate);

    void autosizeExhaustAirFlowRate();

    bool setSupplyAirFan(const HVACComponent& supplyAirFan);

    bool setExhaustAirFan(const HVACComponent& exhaustAirFan);

    bool setController(const boost::optional<ZoneHVACEnergyRecoveryVentilatorController>& zoneHVACEnergyRecoveryVentilatorController);

    void resetController();

    bool setVentilationRateperUnitFloorArea(double ventilationRateperUnitFloorArea);

    bool setVentilationRateperOccupant(double ventilationRateperOccupant);

    // bool setAvailabilityManagerList(const boost::optional<ModelObject>& systemAvailabilityManagerLists);

    // void resetAvailabilityManagerList();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.ZoneHVACEnergyRecoveryVentilator");

    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<Schedule> optionalAvailabilitySchedule() const;
    boost::optional<HVACComponent> optionalHeatExchanger() const;
    boost::optional<HVACComponent> optionalSupplyAirFan() const;
    boost::optional<HVACComponent> optionalExhaustAirFan() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_ZONEHVACENERGYRECOVERYVENTILATOR_IMPL_HPP

