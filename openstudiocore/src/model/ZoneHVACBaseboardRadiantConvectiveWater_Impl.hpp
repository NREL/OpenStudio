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

#ifndef MODEL_ZONEHVACBASEBOARDRADIANTCONVECTIVEWATER_IMPL_HPP
#define MODEL_ZONEHVACBASEBOARDRADIANTCONVECTIVEWATER_IMPL_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;
class Surface;
class ThermalZone;
class HVACComponent;

namespace detail {

  /** ZoneHVACBaseboardRadiantConvectiveWater_Impl is a ZoneHVACComponent_Impl that is the implementation class for ZoneHVACBaseboardRadiantConvectiveWater.*/
  class MODEL_API ZoneHVACBaseboardRadiantConvectiveWater_Impl : public ZoneHVACComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    ZoneHVACBaseboardRadiantConvectiveWater_Impl(const IdfObject& idfObject,
                                                 Model_Impl* model,
                                                 bool keepHandle);

    ZoneHVACBaseboardRadiantConvectiveWater_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                 Model_Impl* model,
                                                 bool keepHandle);

    ZoneHVACBaseboardRadiantConvectiveWater_Impl(const ZoneHVACBaseboardRadiantConvectiveWater_Impl& other,
                                                 Model_Impl* model,
                                                 bool keepHandle);

    virtual ~ZoneHVACBaseboardRadiantConvectiveWater_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    boost::optional<ThermalZone> thermalZone() const;

    bool addToThermalZone(ThermalZone & thermalZone) override;

    void removeFromThermalZone() override;

    virtual ModelObject clone(Model model) const override;

    virtual unsigned inletPort() const override;

    virtual unsigned outletPort() const override;

    virtual std::vector<ModelObject> children() const override;

    virtual std::vector<IdfObject> remove() override;

    //@}
    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    double fractionRadiant() const;

    double fractionofRadiantEnergyIncidentonPeople() const;

    HVACComponent heatingCoil() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setFractionRadiant(double fractionRadiant);

    bool setFractionofRadiantEnergyIncidentonPeople(double fractionofRadiantEnergyIncidentonPeople);

    bool setHeatingCoil( const HVACComponent& heatingCoil );

    //@}
    /** @name Other */
    //@{

    std::vector<Surface> surfaces() const;

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.ZoneHVACBaseboardRadiantConvectiveWater");

    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<Schedule> optionalAvailabilitySchedule() const;
    boost::optional<HVACComponent> optionalHeatingCoil() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_ZONEHVACBASEBOARDRADIANTCONVECTIVEWATER_IMPL_HPP

