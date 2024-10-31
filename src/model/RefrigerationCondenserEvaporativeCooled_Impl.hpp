/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_REFRIGERATIONCONDENSEREVAPORATIVECOOLED_IMPL_HPP
#define MODEL_REFRIGERATIONCONDENSEREVAPORATIVECOOLED_IMPL_HPP

#include "ModelAPI.hpp"
#include "ParentObject_Impl.hpp"

namespace openstudio {
namespace model {

  // WaterStorageTank object not used
  //class WaterStorageTank;
  class RefrigerationSystem;
  class Schedule;

  namespace detail {

    /** RefrigerationCondenserEvaporativeCooled_Impl is a ParentObject_Impl that is the implementation class for RefrigerationCondenserEvaporativeCooled.*/
    class MODEL_API RefrigerationCondenserEvaporativeCooled_Impl : public ParentObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      RefrigerationCondenserEvaporativeCooled_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      RefrigerationCondenserEvaporativeCooled_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      RefrigerationCondenserEvaporativeCooled_Impl(const RefrigerationCondenserEvaporativeCooled_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~RefrigerationCondenserEvaporativeCooled_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

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

      bool setFanSpeedControlType(const std::string& fanSpeedControlType);

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

      bool setMinimumCapacityFactor(double minimumCapacityFactor);

      void resetMinimumCapacityFactor();

      bool setMaximumCapacityFactor(double maximumCapacityFactor);

      void resetMaximumCapacityFactor();

      //bool setAirInletNode(const std::string& airInletNode);

      //void resetAirInletNode();

      bool setRatedAirFlowRate(boost::optional<double> ratedAirFlowRate);

      void resetRatedAirFlowRate();

      void autocalculateRatedAirFlowRate();

      bool setBasinHeaterCapacity(double basinHeaterCapacity);

      void resetBasinHeaterCapacity();

      bool setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature);

      void resetBasinHeaterSetpointTemperature();

      bool setRatedWaterPumpPower(boost::optional<double> ratedWaterPumpPower);

      void resetRatedWaterPumpPower();

      void autocalculateRatedWaterPumpPower();

      //bool setEvaporativeWaterSupplyTank(const boost::optional<WaterStorageTank>& waterStorageTank);

      //void resetEvaporativeWaterSupplyTank();

      bool setEvaporativeCondenserAvailabilitySchedule(Schedule& schedule);

      void resetEvaporativeCondenserAvailabilitySchedule();

      bool setEndUseSubcategory(const std::string& endUseSubcategory);

      void resetEndUseSubcategory();

      bool setCondenserRefrigerantOperatingChargeInventory(double condenserRefrigerantOperatingChargeInventory);

      void resetCondenserRefrigerantOperatingChargeInventory();

      bool setCondensateReceiverRefrigerantInventory(double condensateReceiverRefrigerantInventory);

      void resetCondensateReceiverRefrigerantInventory();

      bool setCondensatePipingRefrigerantInventory(double condensatePipingRefrigerantInventory);

      void resetCondensatePipingRefrigerantInventory();

      //@}
      /** @name Other */
      //@{

      boost::optional<RefrigerationSystem> system() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.RefrigerationCondenserEvaporativeCooled");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_REFRIGERATIONCONDENSEREVAPORATIVECOOLED_IMPL_HPP
