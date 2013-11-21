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

#ifndef MODEL_ZONEHVACREFRIGERATIONCHILLERSET_IMPL_HPP
#define MODEL_ZONEHVACREFRIGERATIONCHILLERSET_IMPL_HPP

#include <model/ModelAPI.hpp>
#include <model/ZoneHVACComponent_Impl.hpp>

namespace openstudio {
namespace model {

// TODO: Check the following class names against object getters and setters.
class Schedule;
// class RefrigerationAirChiller;
// class ThermalZone;

namespace detail {

  /** ZoneHVACRefrigerationChillerSet_Impl is a ZoneHVACComponent_Impl that is the implementation class for ZoneHVACRefrigerationChillerSet.*/
  class MODEL_API ZoneHVACRefrigerationChillerSet_Impl : public ZoneHVACComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    ZoneHVACRefrigerationChillerSet_Impl(const IdfObject& idfObject,
                                         Model_Impl* model,
                                         bool keepHandle);

    ZoneHVACRefrigerationChillerSet_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                         Model_Impl* model,
                                         bool keepHandle);

    ZoneHVACRefrigerationChillerSet_Impl(const ZoneHVACRefrigerationChillerSet_Impl& other,
                                         Model_Impl* model,
                                         bool keepHandle);

    virtual ~ZoneHVACRefrigerationChillerSet_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    // virtual ModelObject clone(Model model) const;

    // virtual std::vector<IdfObject> remove();

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    // virtual std::vector<ModelObject> children() const;

    virtual unsigned inletPort();

    virtual unsigned outletPort();

    //@}
    /** @name Getters */
    //@{

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> availabilitySchedule() const;

    // TODO: Check return type. From object lists, some candidates are: ThermalZone.
    // boost::optional<ThermalZone> zone() const;

    // boost::optional<std::string> airInletNodeName() const;

    // boost::optional<std::string> airOutletNodeName() const;

    // TODO: Handle this object's extensible fields.

    // std::vector<RefrigerationAirChiller> airChillers() const;

    //@}
    /** @name Setters */
    //@{

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    // TODO: Check argument type. From object lists, some candidates are: ThermalZone.
    // bool setZone(const boost::optional<ThermalZone>& thermalZone);

    // void resetZone();

    // void setAirInletNodeName(boost::optional<std::string> airInletNodeName);

    // void resetAirInletNodeName();

    // void setAirOutletNodeName(boost::optional<std::string> airOutletNodeName);

    // void resetAirOutletNodeName();

    // TODO: Handle this object's extensible fields.

    // bool addAirChiller( const RefrigerationAirChiller & refrigerationAirChiller);

    // bool addAirChiller( const RefrigerationAirChiller& refrigerationAirChiller, unsigned groupIndex );

    // void removeAirChiller( const RefrigerationAirChiller & refrigerationAirChiller);

    // void removeAirChiller( unsigned groupIndex );

    // void removeAllAirChillers();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.ZoneHVACRefrigerationChillerSet");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_ZONEHVACREFRIGERATIONCHILLERSET_IMPL_HPP

