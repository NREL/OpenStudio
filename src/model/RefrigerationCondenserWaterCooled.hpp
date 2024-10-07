/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_REFRIGERATIONCONDENSERWATERCOOLED_HPP
#define MODEL_REFRIGERATIONCONDENSERWATERCOOLED_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class RefrigerationSystem;

  namespace detail {

    class RefrigerationCondenserWaterCooled_Impl;

  }  // namespace detail

  /** RefrigerationCondenserWaterCooled is a StraightComponent that wraps the OpenStudio IDD object 'OS:Refrigeration:Condenser:WaterCooled'. */
  class MODEL_API RefrigerationCondenserWaterCooled : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit RefrigerationCondenserWaterCooled(const Model& model);

    virtual ~RefrigerationCondenserWaterCooled() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    RefrigerationCondenserWaterCooled(const RefrigerationCondenserWaterCooled& other) = default;
    RefrigerationCondenserWaterCooled(RefrigerationCondenserWaterCooled&& other) = default;
    RefrigerationCondenserWaterCooled& operator=(const RefrigerationCondenserWaterCooled&) = default;
    RefrigerationCondenserWaterCooled& operator=(RefrigerationCondenserWaterCooled&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> waterCooledLoopFlowTypeValues();

    /** @name Getters */
    //@{

    boost::optional<double> ratedEffectiveTotalHeatRejectionRate() const;

    double ratedCondensingTemperature() const;

    double ratedSubcoolingTemperatureDifference() const;

    bool isRatedSubcoolingTemperatureDifferenceDefaulted() const;

    double ratedWaterInletTemperature() const;

    std::string waterCooledLoopFlowType() const;

    bool isWaterCooledLoopFlowTypeDefaulted() const;

    boost::optional<Schedule> waterOutletTemperatureSchedule() const;

    boost::optional<double> waterDesignFlowRate() const;

    boost::optional<double> waterMaximumFlowRate() const;

    double waterMaximumWaterOutletTemperature() const;

    bool isWaterMaximumWaterOutletTemperatureDefaulted() const;

    double waterMinimumWaterInletTemperature() const;

    bool isWaterMinimumWaterInletTemperatureDefaulted() const;

    std::string endUseSubcategory() const;

    bool isEndUseSubcategoryDefaulted() const;

    boost::optional<double> condenserRefrigerantOperatingChargeInventory() const;

    boost::optional<double> condensateReceiverRefrigerantInventory() const;

    boost::optional<double> condensatePipingRefrigerantInventory() const;

    //@}
    /** @name Setters */
    //@{

    bool setRatedEffectiveTotalHeatRejectionRate(double ratedEffectiveTotalHeatRejectionRate);

    void resetRatedEffectiveTotalHeatRejectionRate();

    bool setRatedCondensingTemperature(double ratedCondensingTemperature);

    bool setRatedSubcoolingTemperatureDifference(double ratedSubcoolingTemperatureDifference);

    void resetRatedSubcoolingTemperatureDifference();

    bool setRatedWaterInletTemperature(double ratedWaterInletTemperature);

    bool setWaterCooledLoopFlowType(const std::string& waterCooledLoopFlowType);

    void resetWaterCooledLoopFlowType();

    bool setWaterOutletTemperatureSchedule(Schedule& schedule);

    void resetWaterOutletTemperatureSchedule();

    bool setWaterDesignFlowRate(double waterDesignFlowRate);

    void resetWaterDesignFlowRate();

    bool setWaterMaximumFlowRate(double waterMaximumFlowRate);

    void resetWaterMaximumFlowRate();

    bool setWaterMaximumWaterOutletTemperature(double waterMaximumWaterOutletTemperature);

    void resetWaterMaximumWaterOutletTemperature();

    bool setWaterMinimumWaterInletTemperature(double waterMinimumWaterInletTemperature);

    void resetWaterMinimumWaterInletTemperature();

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
    using ImplType = detail::RefrigerationCondenserWaterCooled_Impl;

    explicit RefrigerationCondenserWaterCooled(std::shared_ptr<detail::RefrigerationCondenserWaterCooled_Impl> impl);

    friend class detail::RefrigerationCondenserWaterCooled_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.RefrigerationCondenserWaterCooled");
  };

  /** \relates RefrigerationCondenserWaterCooled*/
  using OptionalRefrigerationCondenserWaterCooled = boost::optional<RefrigerationCondenserWaterCooled>;

  /** \relates RefrigerationCondenserWaterCooled*/
  using RefrigerationCondenserWaterCooledVector = std::vector<RefrigerationCondenserWaterCooled>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_REFRIGERATIONCONDENSERWATERCOOLED_HPP
