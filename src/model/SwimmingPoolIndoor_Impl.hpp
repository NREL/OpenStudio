/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef MODEL_SWIMMINGPOOLINDOOR_IMPL_HPP
#define MODEL_SWIMMINGPOOLINDOOR_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

// TODO: Check the following class names against object getters and setters.
class FloorSurface;
class Schedule;
class Schedule;
class Schedule;
class Connection;
class Connection;
class Schedule;
class Schedule;
class Schedule;

namespace detail {

  /** SwimmingPoolIndoor_Impl is a StraightComponent_Impl that is the implementation class for SwimmingPoolIndoor.*/
  class MODEL_API SwimmingPoolIndoor_Impl : public StraightComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    SwimmingPoolIndoor_Impl(const IdfObject& idfObject,
                            Model_Impl* model,
                            bool keepHandle);

    SwimmingPoolIndoor_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                            Model_Impl* model,
                            bool keepHandle);

    SwimmingPoolIndoor_Impl(const SwimmingPoolIndoor_Impl& other,
                            Model_Impl* model,
                            bool keepHandle);

    virtual ~SwimmingPoolIndoor_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    //@}
    /** @name Getters */
    //@{

    // TODO: Check return type. From object lists, some candidates are: FloorSurface.
    FloorSurface surface() const;

    double averageDepth() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    Schedule activityFactorSchedule() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    Schedule makeupWaterSupplySchedule() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    Schedule coverSchedule() const;

    double coverEvaporationFactor() const;

    double coverConvectionFactor() const;

    double coverShortWavelengthRadiationFactor() const;

    double coverLongWavelengthRadiationFactor() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    Connection poolWaterInletNode() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    Connection poolWaterOutletNode() const;

    double poolHeatingSystemMaximumWaterFlowRate() const;

    double poolMiscellaneousEquipmentPower() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    Schedule setpointTemperatureSchedule() const;

    double maximumNumberofPeople() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    Schedule peopleSchedule() const;

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    Schedule peopleHeatGainSchedule() const;

    //@}
    /** @name Setters */
    //@{

    // TODO: Check argument type. From object lists, some candidates are: FloorSurface.
    bool setSurface(const FloorSurface& floorSurface);

    bool setAverageDepth(double averageDepth);

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
  // Note Schedules are passed by reference, not const reference.
    bool setActivityFactorSchedule(Schedule& schedule);

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
  // Note Schedules are passed by reference, not const reference.
    bool setMakeupWaterSupplySchedule(Schedule& schedule);

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
  // Note Schedules are passed by reference, not const reference.
    bool setCoverSchedule(Schedule& schedule);

    bool setCoverEvaporationFactor(double coverEvaporationFactor);

    bool setCoverConvectionFactor(double coverConvectionFactor);

    bool setCoverShortWavelengthRadiationFactor(double coverShortWavelengthRadiationFactor);

    bool setCoverLongWavelengthRadiationFactor(double coverLongWavelengthRadiationFactor);

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setPoolWaterInletNode(const Connection& connection);

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setPoolWaterOutletNode(const Connection& connection);

    bool setPoolHeatingSystemMaximumWaterFlowRate(double poolHeatingSystemMaximumWaterFlowRate);

    bool setPoolMiscellaneousEquipmentPower(double poolMiscellaneousEquipmentPower);

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
  // Note Schedules are passed by reference, not const reference.
    bool setSetpointTemperatureSchedule(Schedule& schedule);

    bool setMaximumNumberofPeople(double maximumNumberofPeople);

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
  // Note Schedules are passed by reference, not const reference.
    bool setPeopleSchedule(Schedule& schedule);

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
  // Note Schedules are passed by reference, not const reference.
    bool setPeopleHeatGainSchedule(Schedule& schedule);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.SwimmingPoolIndoor");

    // TODO: Check the return types of these methods.
    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<FloorSurface> optionalSurface() const;
    boost::optional<Schedule> optionalActivityFactorSchedule() const;
    boost::optional<Schedule> optionalMakeupWaterSupplySchedule() const;
    boost::optional<Schedule> optionalCoverSchedule() const;
    boost::optional<Connection> optionalPoolWaterInletNode() const;
    boost::optional<Connection> optionalPoolWaterOutletNode() const;
    boost::optional<Schedule> optionalSetpointTemperatureSchedule() const;
    boost::optional<Schedule> optionalPeopleSchedule() const;
    boost::optional<Schedule> optionalPeopleHeatGainSchedule() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_SWIMMINGPOOLINDOOR_IMPL_HPP

