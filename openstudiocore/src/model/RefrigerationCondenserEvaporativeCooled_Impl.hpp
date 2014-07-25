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

#ifndef MODEL_REFRIGERATIONCONDENSEREVAPORATIVECOOLED_IMPL_HPP
#define MODEL_REFRIGERATIONCONDENSEREVAPORATIVECOOLED_IMPL_HPP

#include "ModelAPI.hpp"
#include "ParentObject_Impl.hpp"

namespace openstudio {
namespace model {

// WaterStorageTank object not used
//class WaterStorageTank;
class Schedule;

namespace detail {

  /** RefrigerationCondenserEvaporativeCooled_Impl is a ParentObject_Impl that is the implementation class for RefrigerationCondenserEvaporativeCooled.*/
  class MODEL_API RefrigerationCondenserEvaporativeCooled_Impl : public ParentObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    RefrigerationCondenserEvaporativeCooled_Impl(const IdfObject& idfObject,
                                                 Model_Impl* model,
                                                 bool keepHandle);

    RefrigerationCondenserEvaporativeCooled_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                 Model_Impl* model,
                                                 bool keepHandle);

    RefrigerationCondenserEvaporativeCooled_Impl(const RefrigerationCondenserEvaporativeCooled_Impl& other,
                                                 Model_Impl* model,
                                                 bool keepHandle);

    virtual ~RefrigerationCondenserEvaporativeCooled_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual ModelObject clone(Model model) const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    //@}
    /** @name Getters */
    //@{

    double ratedEffectiveTotalHeatRejectionRate() const;

    double ratedSubcoolingTemperatureDifference() const;

    bool isRatedSubcoolingTemperatureDifferenceDefaulted() const;

    std::string fanSpeedControlType() const;

    bool isFanSpeedControlTypeDefaulted() const;

    double ratedFanPower() const;

    double minimumFanAirFlowRatio() const;

    bool isMinimumFanAirFlowRatioDefaulted() const;

    double approachTemperatureConstantTerm() const;

    bool isApproachTemperatureConstantTermDefaulted() const;

    double approachTemperatureCoefficient2() const;

    bool isApproachTemperatureCoefficient2Defaulted() const;

    double approachTemperatureCoefficient3() const;

    bool isApproachTemperatureCoefficient3Defaulted() const;

    double approachTemperatureCoefficient4() const;

    bool isApproachTemperatureCoefficient4Defaulted() const;

    double minimumCapacityFactor() const;

    bool isMinimumCapacityFactorDefaulted() const;

    double maximumCapacityFactor() const;

    bool isMaximumCapacityFactorDefaulted() const;

    //std::string airInletNode() const;

    boost::optional<double> ratedAirFlowRate() const;

    bool isRatedAirFlowRateDefaulted() const;

    bool isRatedAirFlowRateAutocalculated() const;

    double basinHeaterCapacity() const;

    bool isBasinHeaterCapacityDefaulted() const;

    double basinHeaterSetpointTemperature() const;

    bool isBasinHeaterSetpointTemperatureDefaulted() const;

    boost::optional<double> ratedWaterPumpPower() const;

    bool isRatedWaterPumpPowerDefaulted() const;

    bool isRatedWaterPumpPowerAutocalculated() const;

    //boost::optional<WaterStorageTank> evaporativeWaterSupplyTank() const;

    boost::optional<Schedule> evaporativeCondenserAvailabilitySchedule() const;

    std::string endUseSubcategory() const;

    bool isEndUseSubcategoryDefaulted() const;

    double condenserRefrigerantOperatingChargeInventory() const;

    bool isCondenserRefrigerantOperatingChargeInventoryDefaulted() const;

    double condensateReceiverRefrigerantInventory() const;

    bool isCondensateReceiverRefrigerantInventoryDefaulted() const;

    double condensatePipingRefrigerantInventory() const;

    bool isCondensatePipingRefrigerantInventoryDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setRatedEffectiveTotalHeatRejectionRate(double ratedEffectiveTotalHeatRejectionRate);

    bool setRatedSubcoolingTemperatureDifference(double ratedSubcoolingTemperatureDifference);

    void resetRatedSubcoolingTemperatureDifference();

    bool setFanSpeedControlType(std::string fanSpeedControlType);

    void resetFanSpeedControlType();

    bool setRatedFanPower(double ratedFanPower);

    bool setMinimumFanAirFlowRatio(double minimumFanAirFlowRatio);

    void resetMinimumFanAirFlowRatio();

    bool setApproachTemperatureConstantTerm(double approachTemperatureConstantTerm);

    void resetApproachTemperatureConstantTerm();

    bool setApproachTemperatureCoefficient2(double approachTemperatureCoefficient2);

    void resetApproachTemperatureCoefficient2();

    bool setApproachTemperatureCoefficient3(double approachTemperatureCoefficient3);

    void resetApproachTemperatureCoefficient3();

    bool setApproachTemperatureCoefficient4(double approachTemperatureCoefficient4);

    void resetApproachTemperatureCoefficient4();

    void setMinimumCapacityFactor(double minimumCapacityFactor);

    void resetMinimumCapacityFactor();

    void setMaximumCapacityFactor(double maximumCapacityFactor);

    void resetMaximumCapacityFactor();

    //bool setAirInletNode(std::string airInletNode);

    //void resetAirInletNode();

    void setRatedAirFlowRate(boost::optional<double> ratedAirFlowRate);

    void resetRatedAirFlowRate();

    void autocalculateRatedAirFlowRate();

    bool setBasinHeaterCapacity(double basinHeaterCapacity);

    void resetBasinHeaterCapacity();

    bool setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature);

    void resetBasinHeaterSetpointTemperature();

    void setRatedWaterPumpPower(boost::optional<double> ratedWaterPumpPower);

    void resetRatedWaterPumpPower();

    void autocalculateRatedWaterPumpPower();

    //bool setEvaporativeWaterSupplyTank(const boost::optional<WaterStorageTank>& waterStorageTank);

    //void resetEvaporativeWaterSupplyTank();

    bool setEvaporativeCondenserAvailabilitySchedule(Schedule& schedule);

    void resetEvaporativeCondenserAvailabilitySchedule();

    void setEndUseSubcategory(std::string endUseSubcategory);

    void resetEndUseSubcategory();

    void setCondenserRefrigerantOperatingChargeInventory(double condenserRefrigerantOperatingChargeInventory);

    void resetCondenserRefrigerantOperatingChargeInventory();

    void setCondensateReceiverRefrigerantInventory(double condensateReceiverRefrigerantInventory);

    void resetCondensateReceiverRefrigerantInventory();

    void setCondensatePipingRefrigerantInventory(double condensatePipingRefrigerantInventory);

    void resetCondensatePipingRefrigerantInventory();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.RefrigerationCondenserEvaporativeCooled");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_REFRIGERATIONCONDENSEREVAPORATIVECOOLED_IMPL_HPP

