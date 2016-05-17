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

#ifndef MODEL_ZONECONTROLCONTAMINANTCONTROLLER_IMPL_HPP
#define MODEL_ZONECONTROLCONTAMINANTCONTROLLER_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

class ThermalZone;
class Schedule;

namespace detail {

  /** ZoneControlContaminantController_Impl is a ModelObject_Impl that is the implementation class for ZoneControlContaminantController.*/
  class MODEL_API ZoneControlContaminantController_Impl : public ModelObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    ZoneControlContaminantController_Impl(const IdfObject& idfObject,
                                          Model_Impl* model,
                                          bool keepHandle);

    ZoneControlContaminantController_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                          Model_Impl* model,
                                          bool keepHandle);

    ZoneControlContaminantController_Impl(const ZoneControlContaminantController_Impl& other,
                                          Model_Impl* model,
                                          bool keepHandle);

    virtual ~ZoneControlContaminantController_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    //@}
    /** @name Getters */
    //@{

    boost::optional<Schedule> carbonDioxideControlAvailabilitySchedule() const;

    boost::optional<Schedule> carbonDioxideSetpointSchedule() const;

    boost::optional<Schedule> minimumCarbonDioxideConcentrationSchedule() const;

    boost::optional<Schedule> genericContaminantControlAvailabilitySchedule() const;

    boost::optional<Schedule> genericContaminantSetpointSchedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setCarbonDioxideControlAvailabilitySchedule(Schedule& schedule);

    void resetCarbonDioxideControlAvailabilitySchedule();

    bool setCarbonDioxideSetpointSchedule(Schedule& schedule);

    void resetCarbonDioxideSetpointSchedule();

    bool setMinimumCarbonDioxideConcentrationSchedule(Schedule& schedule);

    void resetMinimumCarbonDioxideConcentrationSchedule();

    bool setGenericContaminantControlAvailabilitySchedule(Schedule& schedule);

    void resetGenericContaminantControlAvailabilitySchedule();

    bool setGenericContaminantSetpointSchedule(Schedule& schedule);

    void resetGenericContaminantSetpointSchedule();

    //@}
    /** @name Other */
    //@{

    boost::optional<ThermalZone> controlledZone() const;

    bool setControlledZone(ThermalZone& zone);

    void resetControlledZone();

    ModelObject clone(Model model) const;

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.ZoneControlContaminantController");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_ZONECONTROLCONTAMINANTCONTROLLER_IMPL_HPP

