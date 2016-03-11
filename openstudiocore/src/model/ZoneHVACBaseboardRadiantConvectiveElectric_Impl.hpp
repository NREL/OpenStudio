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

#ifndef MODEL_ZONEHVACBASEBOARDRADIANTCONVECTIVEELECTRIC_IMPL_HPP
#define MODEL_ZONEHVACBASEBOARDRADIANTCONVECTIVEELECTRIC_IMPL_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;
class Surface;
class ThermalZone;

namespace detail {

  /** ZoneHVACBaseboardRadiantConvectiveElectric_Impl is a ZoneHVACComponent_Impl that is the implementation class for ZoneHVACBaseboardRadiantConvectiveElectric.*/
  class MODEL_API ZoneHVACBaseboardRadiantConvectiveElectric_Impl : public ZoneHVACComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    ZoneHVACBaseboardRadiantConvectiveElectric_Impl(const IdfObject& idfObject,
                                                    Model_Impl* model,
                                                    bool keepHandle);

    ZoneHVACBaseboardRadiantConvectiveElectric_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                    Model_Impl* model,
                                                    bool keepHandle);

    ZoneHVACBaseboardRadiantConvectiveElectric_Impl(const ZoneHVACBaseboardRadiantConvectiveElectric_Impl& other,
                                                    Model_Impl* model,
                                                    bool keepHandle);

    virtual ~ZoneHVACBaseboardRadiantConvectiveElectric_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    virtual unsigned inletPort() const override;

    virtual unsigned outletPort() const override;

    boost::optional<ThermalZone> thermalZone() const;

    bool addToThermalZone(ThermalZone & thermalZone) override;

    void removeFromThermalZone() override;

    //@}
    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    std::string heatingDesignCapacityMethod() const;

    boost::optional<double> heatingDesignCapacity() const;

    bool isHeatingDesignCapacityAutosized() const;

    double heatingDesignCapacityPerFloorArea() const;

    double fractionofAutosizedHeatingDesignCapacity() const;

    double efficiency() const;

    double fractionRadiant() const;

    double fractionofRadiantEnergyIncidentonPeople() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setHeatingDesignCapacityMethod(std::string heatingDesignCapacityMethod);

    bool setHeatingDesignCapacity(boost::optional<double> heatingDesignCapacity);

    void autosizeHeatingDesignCapacity();

    bool setHeatingDesignCapacityPerFloorArea(double heatingDesignCapacityPerFloorArea);

    bool setFractionofAutosizedHeatingDesignCapacity(double fractionofAutosizedHeatingDesignCapacity);

    bool setEfficiency(double efficiency);

    bool setFractionRadiant(double fractionRadiant);

    bool setFractionofRadiantEnergyIncidentonPeople(double fractionofRadiantEnergyIncidentonPeople);

    //@}
    /** @name Other */
    //@{

    std::vector<Surface> surfaces() const;

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.ZoneHVACBaseboardRadiantConvectiveElectric");

    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<Schedule> optionalAvailabilitySchedule() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_ZONEHVACBASEBOARDRADIANTCONVECTIVEELECTRIC_IMPL_HPP

