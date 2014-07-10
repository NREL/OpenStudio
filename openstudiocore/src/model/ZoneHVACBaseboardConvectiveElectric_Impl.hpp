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

#ifndef MODEL_ZONEHVACBASEBOARDCONVECTIVEELECTRIC_IMPL_HPP
#define MODEL_ZONEHVACBASEBOARDCONVECTIVEELECTRIC_IMPL_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent_Impl.hpp"
#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"

namespace openstudio {
namespace model {

class Schedule;
class ThermalZone;

namespace detail {

  /** ZoneHVACBaseboardConvectiveElectric_Impl is a ZoneHVACComponent_Impl that is the implementation class for ZoneHVACBaseboardConvectiveElectric.*/
  class MODEL_API ZoneHVACBaseboardConvectiveElectric_Impl : public ZoneHVACComponent_Impl {
    Q_OBJECT;

    Q_PROPERTY(boost::optional<double> nominalCapacity READ nominalCapacity WRITE setNominalCapacity);
    Q_PROPERTY(openstudio::OSOptionalQuantity nominalCapacity_SI READ nominalCapacity_SI WRITE setNominalCapacity);
    Q_PROPERTY(openstudio::OSOptionalQuantity nominalCapacity_IP READ nominalCapacity_IP WRITE setNominalCapacity);
    Q_PROPERTY(bool isNominalCapacityAutosized READ isNominalCapacityAutosized);

    Q_PROPERTY(double efficiency READ efficiency WRITE setEfficiency RESET resetEfficiency);
    Q_PROPERTY(openstudio::Quantity efficiency_SI READ efficiency_SI WRITE setEfficiency RESET resetEfficiency);
    Q_PROPERTY(openstudio::Quantity efficiency_IP READ efficiency_IP WRITE setEfficiency RESET resetEfficiency);
    Q_PROPERTY(bool isEfficiencyDefaulted READ isEfficiencyDefaulted);

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> availabilitySchedule READ availabilityScheduleAsModelObject WRITE setAvailabilityScheduleAsModelObject);
   public:
    /** @name Constructors and Destructors */
    //@{

    ZoneHVACBaseboardConvectiveElectric_Impl(const IdfObject& idfObject,
                                             Model_Impl* model,
                                             bool keepHandle);

    ZoneHVACBaseboardConvectiveElectric_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle);

    ZoneHVACBaseboardConvectiveElectric_Impl(const ZoneHVACBaseboardConvectiveElectric_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle);

    virtual ~ZoneHVACBaseboardConvectiveElectric_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    //@}
    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    boost::optional<double> nominalCapacity() const;

    OSOptionalQuantity getNominalCapacity(bool returnIP=false) const;

    bool isNominalCapacityAutosized() const;

    double efficiency() const;

    Quantity getEfficiency(bool returnIP=false) const;

    bool isEfficiencyDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    void setNominalCapacity(boost::optional<double> nominalCapacity);

    bool setNominalCapacity(const OSOptionalQuantity& nominalCapacity);

    void autosizeNominalCapacity();

    bool setEfficiency(double efficiency);

    bool setEfficiency(const Quantity& efficiency);

    void resetEfficiency();

    //@}
    /** @name Other */
    //@{

    unsigned inletPort();

    unsigned outletPort();
  
    boost::optional<ThermalZone> thermalZone();

    bool addToThermalZone(ThermalZone & thermalZone);

    void removeFromThermalZone();

    //@}
   private:
    REGISTER_LOGGER("openstudio.model.ZoneHVACBaseboardConvectiveElectric");

    // TODO: Check the return types of these methods.
    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<Schedule> optionalAvailabilitySchedule() const;

    openstudio::OSOptionalQuantity nominalCapacity_SI() const;
    openstudio::OSOptionalQuantity nominalCapacity_IP() const;
    openstudio::Quantity efficiency_SI() const;
    openstudio::Quantity efficiency_IP() const;

    boost::optional<ModelObject> availabilityScheduleAsModelObject() const;

    bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_ZONEHVACBASEBOARDCONVECTIVEELECTRIC_IMPL_HPP

