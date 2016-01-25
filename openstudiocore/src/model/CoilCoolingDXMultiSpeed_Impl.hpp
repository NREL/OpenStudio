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

#ifndef MODEL_COILCOOLINGDXMULTISPEED_IMPL_HPP
#define MODEL_COILCOOLINGDXMULTISPEED_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;
class CoilCoolingDXMultiSpeedStageData;

namespace detail {

  /** CoilCoolingDXMultiSpeed_Impl is a StraightComponent_Impl that is the implementation class for CoilCoolingDXMultiSpeed.*/
  class MODEL_API CoilCoolingDXMultiSpeed_Impl : public StraightComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    CoilCoolingDXMultiSpeed_Impl(const IdfObject& idfObject,
                                 Model_Impl* model,
                                 bool keepHandle);

    CoilCoolingDXMultiSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                 Model_Impl* model,
                                 bool keepHandle);

    CoilCoolingDXMultiSpeed_Impl(const CoilCoolingDXMultiSpeed_Impl& other,
                                 Model_Impl* model,
                                 bool keepHandle);

    virtual ~CoilCoolingDXMultiSpeed_Impl() {}

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

    std::string condenserType() const;

    bool applyPartLoadFractiontoSpeedsGreaterthan1() const;

    bool applyLatentDegradationtoSpeedsGreaterthan1() const;

    bool isApplyLatentDegradationtoSpeedsGreaterthan1Defaulted() const;

    double crankcaseHeaterCapacity() const;

    double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const;

    double basinHeaterCapacity() const;

    double basinHeaterSetpointTemperature() const;

    boost::optional<Schedule> basinHeaterOperatingSchedule() const;

    std::string fuelType() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setCondenserType(std::string condenserType);

    void setApplyPartLoadFractiontoSpeedsGreaterthan1(bool applyPartLoadFractiontoSpeedsGreaterthan1);

    void setApplyLatentDegradationtoSpeedsGreaterthan1(bool applyLatentDegradationtoSpeedsGreaterthan1);

    void resetApplyLatentDegradationtoSpeedsGreaterthan1();

    bool setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity);

    bool setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);

    bool setBasinHeaterCapacity(double basinHeaterCapacity);

    bool setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature);

    bool setBasinHeaterOperatingSchedule(Schedule& schedule);

    void resetBasinHeaterOperatingSchedule();

    bool setFuelType(std::string fuelType);

    //@}
    /** @name Other */
    //@{
    
    unsigned inletPort() override;

    unsigned outletPort() override;

    ModelObject clone(Model model) const override;

    std::vector<ModelObject> children() const override;

    std::vector<CoilCoolingDXMultiSpeedStageData> stages() const;

    void addStage(CoilCoolingDXMultiSpeedStageData& stage);

    boost::optional<HVACComponent> containingHVACComponent() const override;

    bool addToNode(Node & node) override;

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.CoilCoolingDXMultiSpeed");

    boost::optional<Connection> optionalAirInletNode() const;
    boost::optional<Connection> optionalAirOutletNode() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_COILCOOLINGDXMULTISPEED_IMPL_HPP

