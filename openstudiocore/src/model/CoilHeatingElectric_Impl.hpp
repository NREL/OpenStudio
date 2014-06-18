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

#ifndef MODEL_COILHEATINGELECTRIC_IMPL_HPP
#define MODEL_COILHEATINGELECTRIC_IMPL_HPP

#include "StraightComponent_Impl.hpp"

namespace openstudio {

namespace model {

class Schedule;

class Node;

namespace detail {

  /** CoilHeatingElectric_Impl is a StraightComponent_Impl that is the
   *  implementation class for CoilHeatingElectric.*/
  class MODEL_API CoilHeatingElectric_Impl : public StraightComponent_Impl {
    Q_OBJECT;

    Q_PROPERTY(double efficiency READ efficiency WRITE setEfficiency RESET resetEfficiency);
    Q_PROPERTY(bool isEfficiencyDefaulted READ isEfficiencyDefaulted);
    Q_PROPERTY(boost::optional<double> nominalCapacity READ nominalCapacity WRITE setNominalCapacity RESET resetNominalCapacity);
    Q_PROPERTY(bool isNominalCapacityDefaulted READ isNominalCapacityDefaulted);
    Q_PROPERTY(bool isNominalCapacityAutosized READ isNominalCapacityAutosized);

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> availabilitySchedule READ availabilityScheduleAsModelObject WRITE setAvailabilityScheduleAsModelObject);
  public:
    /** @name Constructors and Destructors */
    //@{

    CoilHeatingElectric_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    CoilHeatingElectric_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                             Model_Impl* model,
                             bool keepHandle);

    CoilHeatingElectric_Impl(const CoilHeatingElectric_Impl& other,
                             Model_Impl* model,
                             bool keepHandle);

    virtual ~CoilHeatingElectric_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    virtual unsigned inletPort();

    virtual unsigned outletPort();

    virtual bool addToNode(Node & node);

    virtual boost::optional<HVACComponent> containingHVACComponent() const;

    virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const;    

    //@}
    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    double efficiency() const;

    bool isEfficiencyDefaulted() const;

    boost::optional<double> nominalCapacity() const;

    bool isNominalCapacityDefaulted() const;

    bool isNominalCapacityAutosized() const;

    boost::optional<Node> temperatureSetpointNode() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule & schedule );

    bool setEfficiency(double efficiency);

    void resetEfficiency();

    void setNominalCapacity(boost::optional<double> nominalCapacity);

    void resetNominalCapacity();

    void autosizeNominalCapacity();

    void setTemperatureSetpointNode(Node & temperatureSetpointNode);

    void resetTemperatureSetpointNode();

    //@}
  protected:
  private:
    REGISTER_LOGGER("openstudio.model.CoilHeatingElectric");

    boost::optional<ModelObject> availabilityScheduleAsModelObject() const;

    bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_COILHEATINGELECTRIC_IMPL_HPP

