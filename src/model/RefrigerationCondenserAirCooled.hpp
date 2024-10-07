/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_REFRIGERATIONCONDENSERAIRCOOLED_HPP
#define MODEL_REFRIGERATIONCONDENSERAIRCOOLED_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {

namespace model {

  class CurveLinear;
  class ThermalZone;
  class RefrigerationSystem;

  namespace detail {

    class RefrigerationCondenserAirCooled_Impl;

  }  // namespace detail

  /** RefrigerationCondenserAirCooled is a ParentObject that wraps the OpenStudio IDD object 'OS:Refrigeration:Condenser:AirCooled'. */
  class MODEL_API RefrigerationCondenserAirCooled : public ParentObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit RefrigerationCondenserAirCooled(const Model& model);

    virtual ~RefrigerationCondenserAirCooled() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    RefrigerationCondenserAirCooled(const RefrigerationCondenserAirCooled& other) = default;
    RefrigerationCondenserAirCooled(RefrigerationCondenserAirCooled&& other) = default;
    RefrigerationCondenserAirCooled& operator=(const RefrigerationCondenserAirCooled&) = default;
    RefrigerationCondenserAirCooled& operator=(RefrigerationCondenserAirCooled&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> condenserFanSpeedControlTypeValues();

    /** @name Getters */
    //@{

    boost::optional<CurveLinear> ratedEffectiveTotalHeatRejectionRateCurve() const;

    double ratedSubcoolingTemperatureDifference() const;

    bool isRatedSubcoolingTemperatureDifferenceDefaulted() const;

    std::string condenserFanSpeedControlType() const;

    bool isCondenserFanSpeedControlTypeDefaulted() const;

    double ratedFanPower() const;

    bool isRatedFanPowerDefaulted() const;

    double minimumFanAirFlowRatio() const;

    bool isMinimumFanAirFlowRatioDefaulted() const;

    boost::optional<ThermalZone> airInletZone() const;

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

    bool setRatedEffectiveTotalHeatRejectionRateCurve(const CurveLinear& curveLinear);

    void resetRatedEffectiveTotalHeatRejectionRateCurve();

    bool setRatedSubcoolingTemperatureDifference(double ratedSubcoolingTemperatureDifference);

    void resetRatedSubcoolingTemperatureDifference();

    bool setCondenserFanSpeedControlType(const std::string& condenserFanSpeedControlType);

    void resetCondenserFanSpeedControlType();

    bool setRatedFanPower(double ratedFanPower);

    void resetRatedFanPower();

    bool setMinimumFanAirFlowRatio(double minimumFanAirFlowRatio);

    void resetMinimumFanAirFlowRatio();

    bool setAirInletZone(const ThermalZone& thermalZone);

    void resetAirInletZone();

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
    using ImplType = detail::RefrigerationCondenserAirCooled_Impl;

    explicit RefrigerationCondenserAirCooled(std::shared_ptr<detail::RefrigerationCondenserAirCooled_Impl> impl);

    friend class detail::RefrigerationCondenserAirCooled_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.RefrigerationCondenserAirCooled");
  };

  /** \relates RefrigerationCondenserAirCooled*/
  using OptionalRefrigerationCondenserAirCooled = boost::optional<RefrigerationCondenserAirCooled>;

  /** \relates RefrigerationCondenserAirCooled*/
  using RefrigerationCondenserAirCooledVector = std::vector<RefrigerationCondenserAirCooled>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_REFRIGERATIONCONDENSERAIRCOOLED_HPP
