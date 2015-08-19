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

#ifndef MODEL_PLANTCOMPONENTTEMPERATURESOURCE_IMPL_HPP
#define MODEL_PLANTCOMPONENTTEMPERATURESOURCE_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

// TODO: Check the following class names against object getters and setters.
class Connection;
class Connection;
class Schedule;

namespace detail {

  /** PlantComponentTemperatureSource_Impl is a StraightComponent_Impl that is the implementation class for PlantComponentTemperatureSource.*/
  class MODEL_API PlantComponentTemperatureSource_Impl : public StraightComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    PlantComponentTemperatureSource_Impl(const IdfObject& idfObject,
                                         Model_Impl* model,
                                         bool keepHandle);

    PlantComponentTemperatureSource_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                         Model_Impl* model,
                                         bool keepHandle);

    PlantComponentTemperatureSource_Impl(const PlantComponentTemperatureSource_Impl& other,
                                         Model_Impl* model,
                                         bool keepHandle);

    virtual ~PlantComponentTemperatureSource_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    //@}
    /** @name Getters */
    //@{

    // TODO: Check return type. From object lists, some candidates are: Connection.
    Connection inletNode() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    Connection outletNode() const;

    boost::optional<double> designVolumeFlowRate() const;

    bool isDesignVolumeFlowRateAutosized() const;

    std::string temperatureSpecificationType() const;

    boost::optional<double> sourceTemperature() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> sourceTemperatureSchedule() const;

    //@}
    /** @name Setters */
    //@{

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setInletNode(const Connection& connection);

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setOutletNode(const Connection& connection);

    bool setDesignVolumeFlowRate(boost::optional<double> designVolumeFlowRate);

    void autosizeDesignVolumeFlowRate();

    bool setTemperatureSpecificationType(std::string temperatureSpecificationType);

    void setSourceTemperature(boost::optional<double> sourceTemperature);

    void resetSourceTemperature();

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    bool setSourceTemperatureSchedule(Schedule& schedule);

    void resetSourceTemperatureSchedule();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.PlantComponentTemperatureSource");

    // TODO: Check the return types of these methods.
    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<Connection> optionalInletNode() const;
    boost::optional<Connection> optionalOutletNode() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_PLANTCOMPONENTTEMPERATURESOURCE_IMPL_HPP

