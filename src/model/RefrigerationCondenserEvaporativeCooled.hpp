/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_REFRIGERATIONCONDENSEREVAPORATIVECOOLED_HPP
#define MODEL_REFRIGERATIONCONDENSEREVAPORATIVECOOLED_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {

namespace model {

  // WaterStorageTank object not used
  //class WaterStorageTank;
  class Schedule;
  class RefrigerationSystem;

  namespace detail {

    class RefrigerationCondenserEvaporativeCooled_Impl;

  }  // namespace detail

  /** RefrigerationCondenserEvaporativeCooled is a ParentObject that wraps the OpenStudio IDD object 'OS:Refrigeration:Condenser:EvaporativeCooled'. */
  class MODEL_API RefrigerationCondenserEvaporativeCooled : public ParentObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit RefrigerationCondenserEvaporativeCooled(const Model& model);

    virtual ~RefrigerationCondenserEvaporativeCooled() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    RefrigerationCondenserEvaporativeCooled(const RefrigerationCondenserEvaporativeCooled& other) = default;
    RefrigerationCondenserEvaporativeCooled(RefrigerationCondenserEvaporativeCooled&& other) = default;
    RefrigerationCondenserEvaporativeCooled& operator=(const RefrigerationCondenserEvaporativeCooled&) = default;
    RefrigerationCondenserEvaporativeCooled& operator=(RefrigerationCondenserEvaporativeCooled&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> fanSpeedControlTypeValues();

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

    bool setRatedAirFlowRate(double ratedAirFlowRate);

    void resetRatedAirFlowRate();

    void autocalculateRatedAirFlowRate();

    bool setBasinHeaterCapacity(double basinHeaterCapacity);

    void resetBasinHeaterCapacity();

    bool setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature);

    void resetBasinHeaterSetpointTemperature();

    bool setRatedWaterPumpPower(double ratedWaterPumpPower);

    void resetRatedWaterPumpPower();

    void autocalculateRatedWaterPumpPower();

    //bool setEvaporativeWaterSupplyTank(const WaterStorageTank& waterStorageTank);

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

    // The parent RefrigerationSystem, which rejects heat to this condenser
    // This is a convenience method to find any RefrigerationSystem that uses this condenser
    boost::optional<RefrigerationSystem> system() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::RefrigerationCondenserEvaporativeCooled_Impl;

    explicit RefrigerationCondenserEvaporativeCooled(std::shared_ptr<detail::RefrigerationCondenserEvaporativeCooled_Impl> impl);

    friend class detail::RefrigerationCondenserEvaporativeCooled_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.RefrigerationCondenserEvaporativeCooled");
  };

  /** \relates RefrigerationCondenserEvaporativeCooled*/
  using OptionalRefrigerationCondenserEvaporativeCooled = boost::optional<RefrigerationCondenserEvaporativeCooled>;

  /** \relates RefrigerationCondenserEvaporativeCooled*/
  using RefrigerationCondenserEvaporativeCooledVector = std::vector<RefrigerationCondenserEvaporativeCooled>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_REFRIGERATIONCONDENSEREVAPORATIVECOOLED_HPP
