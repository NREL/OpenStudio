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

#ifndef MODEL_COILWATERHEATINGDESUPERHEATER_IMPL_HPP
#define MODEL_COILWATERHEATINGDESUPERHEATER_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;
class CurveBiquadratic;
class HVACComponent;
class WaterHeaterMixed;

namespace detail {

  /** CoilWaterHeatingDesuperheater_Impl is a StraightComponent_Impl that is the implementation class for CoilWaterHeatingDesuperheater.*/
  class MODEL_API CoilWaterHeatingDesuperheater_Impl : public StraightComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    CoilWaterHeatingDesuperheater_Impl(const IdfObject& idfObject,
                                       Model_Impl* model,
                                       bool keepHandle);

    CoilWaterHeatingDesuperheater_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                       Model_Impl* model,
                                       bool keepHandle);

    CoilWaterHeatingDesuperheater_Impl(const CoilWaterHeatingDesuperheater_Impl& other,
                                       Model_Impl* model,
                                       bool keepHandle);

    virtual ~CoilWaterHeatingDesuperheater_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    virtual unsigned inletPort();

    virtual unsigned outletPort();

    virtual bool addToNode(Node & node);

    virtual ModelObject clone(Model model) const;

    virtual std::vector<IdfObject> remove();

    virtual std::vector<IddObjectType> allowableChildTypes() const;

    virtual std::vector<ModelObject> children() const;

    boost::optional<ModelObject> heatRejectionTarget() const;

    bool addToHeatRejectionTarget(const ModelObject& heatRejectionTarget);

    void removeFromHeatRejectionTarget();

    //@}
    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    Schedule setpointTemperatureSchedule() const;

    double deadBandTemperatureDifference() const;

    bool isDeadBandTemperatureDifferenceDefaulted() const;

    boost::optional<double> ratedHeatReclaimRecoveryEfficiency() const;

    double ratedInletWaterTemperature() const;

    double ratedOutdoorAirTemperature() const;

    double maximumInletWaterTemperatureforHeatReclaim() const;

    boost::optional<CurveBiquadratic> heatReclaimEfficiencyFunctionofTemperatureCurve() const;

    // boost::optional<HVACComponent> tank() const;

    boost::optional<ModelObject> heatingSource() const;

    double waterFlowRate() const;

    double waterPumpPower() const;

    bool isWaterPumpPowerDefaulted() const;

    double fractionofPumpHeattoWater() const;

    bool isFractionofPumpHeattoWaterDefaulted() const;

    double onCycleParasiticElectricLoad() const;

    bool isOnCycleParasiticElectricLoadDefaulted() const;

    double offCycleParasiticElectricLoad() const;

    bool isOffCycleParasiticElectricLoadDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setSetpointTemperatureSchedule(Schedule& schedule);

    bool setDeadBandTemperatureDifference(double deadBandTemperatureDifference);

    void resetDeadBandTemperatureDifference();

    bool setRatedHeatReclaimRecoveryEfficiency(boost::optional<double> ratedHeatReclaimRecoveryEfficiency);

    void resetRatedHeatReclaimRecoveryEfficiency();

    void setRatedInletWaterTemperature(double ratedInletWaterTemperature);

    void setRatedOutdoorAirTemperature(double ratedOutdoorAirTemperature);

    void setMaximumInletWaterTemperatureforHeatReclaim(double maximumInletWaterTemperatureforHeatReclaim);

    bool setHeatReclaimEfficiencyFunctionofTemperatureCurve(const boost::optional<CurveBiquadratic>& curveBiquadratic);

    void resetHeatReclaimEfficiencyFunctionofTemperatureCurve();

    // bool setTank(const HVACComponent& waterHeater);

    bool setHeatingSource(const boost::optional<ModelObject>& heatingSource);

    void resetHeatingSource();

    bool setWaterFlowRate(double waterFlowRate);

    bool setWaterPumpPower(double waterPumpPower);

    void resetWaterPumpPower();

    bool setFractionofPumpHeattoWater(double fractionofPumpHeattoWater);

    void resetFractionofPumpHeattoWater();

    bool setOnCycleParasiticElectricLoad(double onCycleParasiticElectricLoad);

    void resetOnCycleParasiticElectricLoad();

    bool setOffCycleParasiticElectricLoad(double offCycleParasiticElectricLoad);

    void resetOffCycleParasiticElectricLoad();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.CoilWaterHeatingDesuperheater");

    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<Schedule> optionalSetpointTemperatureSchedule() const;
    // boost::optional<HVACComponent> optionalTank() const;
    // boost::optional<ModelObject> optionalHeatingSource() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_COILWATERHEATINGDESUPERHEATER_IMPL_HPP

