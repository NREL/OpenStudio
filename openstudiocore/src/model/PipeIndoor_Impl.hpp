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

#ifndef MODEL_PIPEINDOOR_IMPL_HPP
#define MODEL_PIPEINDOOR_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

// TODO: Check the following class names against object getters and setters.
class Construction;
class Connection;
class Connection;
class ThermalZone;
class Schedule;
class Schedule;

namespace detail {

  /** PipeIndoor_Impl is a  StraightComponent_Impl that is the implementation class for PipeIndoor.*/
  class MODEL_API PipeIndoor_Impl : public  StraightComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    PipeIndoor_Impl(const IdfObject& idfObject,
                    Model_Impl* model,
                    bool keepHandle);

    PipeIndoor_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                    Model_Impl* model,
                    bool keepHandle);

    PipeIndoor_Impl(const PipeIndoor_Impl& other,
                    Model_Impl* model,
                    bool keepHandle);

    virtual ~PipeIndoor_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    //@}
    /** @name Getters */
    //@{

    virtual unsigned inletPort() override;

    virtual unsigned outletPort() override;

    // TODO: Check return type. From object lists, some candidates are: Construction.
    boost::optional<Construction> construction() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    boost::optional<Connection> fluidInletNode() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    boost::optional<Connection> fluidOutletNode() const;

    std::string environmentType() const;

    // TODO: Check return type. From object lists, some candidates are: ThermalZone.
    boost::optional<ThermalZone> ambientTemperatureZone() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> ambientTemperatureSchedule() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    boost::optional<Schedule> ambientAirVelocitySchedule() const;

    double pipeInsideDiameter() const;

    double pipeLength() const;

    //@}
    /** @name Setters */
    //@{

    // TODO: Check argument type. From object lists, some candidates are: Construction.
    bool setConstruction(const boost::optional<Construction>& construction);

    void resetConstruction();

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setFluidInletNode(const boost::optional<Connection>& connection);

    void resetFluidInletNode();

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setFluidOutletNode(const boost::optional<Connection>& connection);

    void resetFluidOutletNode();

    bool setEnvironmentType(std::string environmentType);

    // TODO: Check argument type. From object lists, some candidates are: ThermalZone.
    bool setAmbientTemperatureZone(const boost::optional<ThermalZone>& thermalZone);

    void resetAmbientTemperatureZone();

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    bool setAmbientTemperatureSchedule(Schedule& schedule);

    void resetAmbientTemperatureSchedule();

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    bool setAmbientAirVelocitySchedule(Schedule& schedule);

    void resetAmbientAirVelocitySchedule();

    bool setPipeInsideDiameter(double pipeInsideDiameter);

    bool setPipeLength(double pipeLength);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.PipeIndoor");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_PIPEINDOOR_IMPL_HPP

