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

#ifndef MODEL_COILCOOLINGDXTWOSTAGEWITHHUMIDITYCONTROLMODE_IMPL_HPP
#define MODEL_COILCOOLINGDXTWOSTAGEWITHHUMIDITYCONTROLMODE_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;
class CoilPerformanceDXCooling;

namespace detail {

  /** CoilCoolingDXTwoStageWithHumidityControlMode_Impl is a StraightComponent_Impl that is the implementation class for CoilCoolingDXTwoStageWithHumidityControlMode.*/
  class MODEL_API CoilCoolingDXTwoStageWithHumidityControlMode_Impl : public StraightComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    CoilCoolingDXTwoStageWithHumidityControlMode_Impl(const IdfObject& idfObject,
                                                      Model_Impl* model,
                                                      bool keepHandle);

    CoilCoolingDXTwoStageWithHumidityControlMode_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                      Model_Impl* model,
                                                      bool keepHandle);

    CoilCoolingDXTwoStageWithHumidityControlMode_Impl(const CoilCoolingDXTwoStageWithHumidityControlMode_Impl& other,
                                                      Model_Impl* model,
                                                      bool keepHandle);

    virtual ~CoilCoolingDXTwoStageWithHumidityControlMode_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    //@}
    /** @name Getters */
    //@{

    boost::optional<Schedule> availabilitySchedule() const;

    double crankcaseHeaterCapacity() const;

    double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const;

    int numberofCapacityStages() const;

    int numberofEnhancedDehumidificationModes() const;

    boost::optional<CoilPerformanceDXCooling> normalModeStage1CoilPerformance() const;

    boost::optional<CoilPerformanceDXCooling> normalModeStage1Plus2CoilPerformance() const;

    boost::optional<CoilPerformanceDXCooling> dehumidificationMode1Stage1CoilPerformance() const;

    boost::optional<CoilPerformanceDXCooling> dehumidificationMode1Stage1Plus2CoilPerformance() const;

    double basinHeaterCapacity() const;

    double basinHeaterSetpointTemperature() const;

    boost::optional<Schedule> basinHeaterOperatingSchedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity);

    bool setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);

    bool setNumberofCapacityStages(int numberofCapacityStages);

    bool setNumberofEnhancedDehumidificationModes(int numberofEnhancedDehumidificationModes);

    bool setNormalModeStage1CoilPerformance(const boost::optional<CoilPerformanceDXCooling>& coilPerformanceDX);

    void resetNormalModeStage1CoilPerformance();

    bool setNormalModeStage1Plus2CoilPerformance(const boost::optional<CoilPerformanceDXCooling>& coilPerformanceDX);

    void resetNormalModeStage1Plus2CoilPerformance();

    bool setDehumidificationMode1Stage1CoilPerformance(const boost::optional<CoilPerformanceDXCooling>& coilPerformanceDX);

    void resetDehumidificationMode1Stage1CoilPerformance();

    bool setDehumidificationMode1Stage1Plus2CoilPerformance(const boost::optional<CoilPerformanceDXCooling>& coilPerformanceDX);

    void resetDehumidificationMode1Stage1Plus2CoilPerformance();

    bool setBasinHeaterCapacity(double basinHeaterCapacity);

    bool setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature);

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    bool setBasinHeaterOperatingSchedule(Schedule& schedule);

    void resetBasinHeaterOperatingSchedule();

    //@}
    /** @name Other */
    //@{

    unsigned inletPort() override;
    unsigned outletPort() override;

    ModelObject clone(Model model) const override;
    std::vector<ModelObject> children() const override;
    boost::optional<HVACComponent> containingHVACComponent() const override;
    bool addToNode(Node & node) override;

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.CoilCoolingDXTwoStageWithHumidityControlMode");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_COILCOOLINGDXTWOSTAGEWITHHUMIDITYCONTROLMODE_IMPL_HPP

