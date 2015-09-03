/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#ifndef MODEL_ELECTRICLOADCENTERINVERTERSIMPLE_IMPL_HPP
#define MODEL_ELECTRICLOADCENTERINVERTERSIMPLE_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "Inverter_Impl.hpp"

namespace openstudio {
namespace model {

// TODO: Check the following class names against object getters and setters.
class Schedule;
class Zone;

namespace detail {

  /** ElectricLoadCenterInverterSimple_Impl is a Inverter_Impl that is the implementation class for ElectricLoadCenterInverterSimple.*/
  class MODEL_API ElectricLoadCenterInverterSimple_Impl : public Inverter_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    ElectricLoadCenterInverterSimple_Impl(const IdfObject& idfObject,
                                          Model_Impl* model,
                                          bool keepHandle);

    ElectricLoadCenterInverterSimple_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                          Model_Impl* model,
                                          bool keepHandle);

    ElectricLoadCenterInverterSimple_Impl(const ElectricLoadCenterInverterSimple_Impl& other,
                                          Model_Impl* model,
                                          bool keepHandle);

    virtual ~ElectricLoadCenterInverterSimple_Impl() {}

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

    boost::optional<double> radiativeFraction() const;

    boost::optional<double> inverterEfficiency() const;

    //@}
    /** @name Setters */
    //@{

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    // TODO: Check argument type. From object lists, some candidates are: Zone.
    bool setZone(const boost::optional<Zone>& zone);

    void resetZone();

    bool setRadiativeFraction(boost::optional<double> radiativeFraction);

    void resetRadiativeFraction();

    bool setInverterEfficiency(boost::optional<double> inverterEfficiency);

    void resetInverterEfficiency();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.ElectricLoadCenterInverterSimple");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_ELECTRICLOADCENTERINVERTERSIMPLE_IMPL_HPP

