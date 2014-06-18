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

#ifndef MODEL_ZONECONTROLHUMIDISTAT_IMPL_HPP
#define MODEL_ZONECONTROLHUMIDISTAT_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;

namespace detail {

  /** ZoneControlHumidistat_Impl is a ModelObject_Impl that is the implementation class for ZoneControlHumidistat.*/
  class MODEL_API ZoneControlHumidistat_Impl : public ModelObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    ZoneControlHumidistat_Impl(const IdfObject& idfObject,
                               Model_Impl* model,
                               bool keepHandle);

    ZoneControlHumidistat_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                               Model_Impl* model,
                               bool keepHandle);

    ZoneControlHumidistat_Impl(const ZoneControlHumidistat_Impl& other,
                               Model_Impl* model,
                               bool keepHandle);

    virtual ~ZoneControlHumidistat_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    //@}
    /** @name Getters */
    //@{

    boost::optional<Schedule> humidifyingRelativeHumiditySetpointSchedule() const;

    boost::optional<Schedule> dehumidifyingRelativeHumiditySetpointSchedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setHumidifyingRelativeHumiditySetpointSchedule(Schedule& schedule);

    bool setDehumidifyingRelativeHumiditySetpointSchedule(Schedule& schedule);

    void resetHumidifyingRelativeHumiditySetpointSchedule();

    void resetDehumidifyingRelativeHumiditySetpointSchedule();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.ZoneControlHumidistat");

  };

} // detail

} // model
} // openstudio

#endif // MODEL_ZONECONTROLHUMIDISTAT_IMPL_HPP

