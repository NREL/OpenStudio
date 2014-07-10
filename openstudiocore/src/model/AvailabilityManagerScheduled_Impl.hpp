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

#ifndef MODEL_AVAILABILITYMANAGERSCHEDULED_IMPL_HPP
#define MODEL_AVAILABILITYMANAGERSCHEDULED_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

// TODO: Check the following class names against object getters and setters.
class Schedule;

namespace detail {

  /** AvailabilityManagerScheduled_Impl is a ModelObject_Impl that is the implementation class for AvailabilityManagerScheduled.*/
  class MODEL_API AvailabilityManagerScheduled_Impl : public ModelObject_Impl {
    Q_OBJECT;

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> schedule READ scheduleAsModelObject WRITE setScheduleAsModelObject);

   public:

    AvailabilityManagerScheduled_Impl(const IdfObject& idfObject,
                                      Model_Impl* model,
                                      bool keepHandle);

    AvailabilityManagerScheduled_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                      Model_Impl* model,
                                      bool keepHandle);

    AvailabilityManagerScheduled_Impl(const AvailabilityManagerScheduled_Impl& other,
                                      Model_Impl* model,
                                      bool keepHandle);

    virtual ~AvailabilityManagerScheduled_Impl() {}

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    Schedule schedule() const;

    bool setSchedule(Schedule& schedule);

    ModelObject clone(Model model) const;

   private:

    REGISTER_LOGGER("openstudio.model.AvailabilityManagerScheduled");

    boost::optional<Schedule> optionalSchedule() const;

    boost::optional<ModelObject> scheduleAsModelObject() const;

    bool setScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_AVAILABILITYMANAGERSCHEDULED_IMPL_HPP

