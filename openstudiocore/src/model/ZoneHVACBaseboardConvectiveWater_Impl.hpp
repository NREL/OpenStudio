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

#ifndef MODEL_ZONEHVACBASEBOARDCONVECTIVEWATER_IMPL_HPP
#define MODEL_ZONEHVACBASEBOARDCONVECTIVEWATER_IMPL_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent_Impl.hpp"
#include "ThermalZone_Impl.hpp"
#include "Model_Impl.hpp"


namespace openstudio {
namespace model {

class Schedule;
class StraightComponent;

namespace detail {

  /** ZoneHVACBaseboardConvectiveWater_Impl is a ZoneHVACComponent_Impl that is the implementation class for ZoneHVACBaseboardConvectiveWater.*/
  class MODEL_API ZoneHVACBaseboardConvectiveWater_Impl : public ZoneHVACComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    ZoneHVACBaseboardConvectiveWater_Impl(const IdfObject& idfObject,
                                          Model_Impl* model,
                                          bool keepHandle);

    ZoneHVACBaseboardConvectiveWater_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                          Model_Impl* model,
                                          bool keepHandle);

    ZoneHVACBaseboardConvectiveWater_Impl(const ZoneHVACBaseboardConvectiveWater_Impl& other,
                                          Model_Impl* model,
                                          bool keepHandle);

    virtual ~ZoneHVACBaseboardConvectiveWater_Impl() {}

    //@}

    /** @name Virtual Methods */
    //@{

    virtual ModelObject clone(Model model) const;

    virtual std::vector<IdfObject> remove();

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    virtual std::vector<ModelObject> children() const;

    //@}
    /** @name Getters */
    //@{


    unsigned inletPort();
    unsigned outletPort();

    Schedule availabilitySchedule() const;

    StraightComponent heatingCoil() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setHeatingCoil(const StraightComponent& baseboardHeatingCoil);

    boost::optional<ThermalZone> thermalZone();

    bool addToThermalZone(ThermalZone & thermalZone);

    void removeFromThermalZone();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.ZoneHVACBaseboardConvectiveWater");

    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<Schedule> optionalAvailabilitySchedule() const;
    boost::optional<StraightComponent> optionalHeatingCoil() const;

    boost::optional<ModelObject> availabilityScheduleAsModelObject() const;
    boost::optional<ModelObject> heatingCoilAsModelObject() const;

    bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setHeatingCoilAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_ZONEHVACBASEBOARDCONVECTIVEWATER_IMPL_HPP

