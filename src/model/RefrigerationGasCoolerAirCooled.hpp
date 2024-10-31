/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_REFRIGERATIONGASCOOLERAIRCOOLED_HPP
#define MODEL_REFRIGERATIONGASCOOLERAIRCOOLED_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {

namespace model {

  class CurveLinear;
  // class ThermalZone;

  namespace detail {

    class RefrigerationGasCoolerAirCooled_Impl;

  }  // namespace detail

  /** RefrigerationGasCoolerAirCooled is a ParentObject that wraps the OpenStudio IDD object 'OS:Refrigeration:GasCooler:AirCooled'. */
  class MODEL_API RefrigerationGasCoolerAirCooled : public ParentObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit RefrigerationGasCoolerAirCooled(const Model& model);

    virtual ~RefrigerationGasCoolerAirCooled() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    RefrigerationGasCoolerAirCooled(const RefrigerationGasCoolerAirCooled& other) = default;
    RefrigerationGasCoolerAirCooled(RefrigerationGasCoolerAirCooled&& other) = default;
    RefrigerationGasCoolerAirCooled& operator=(const RefrigerationGasCoolerAirCooled&) = default;
    RefrigerationGasCoolerAirCooled& operator=(RefrigerationGasCoolerAirCooled&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> gasCoolerFanSpeedControlTypeValues();

    /** @name Getters */
    //@{

    boost::optional<CurveLinear> ratedTotalHeatRejectionRateCurve() const;

    std::string gasCoolerFanSpeedControlType() const;

    bool isGasCoolerFanSpeedControlTypeDefaulted() const;

    double ratedFanPower() const;

    bool isRatedFanPowerDefaulted() const;

    double minimumFanAirFlowRatio() const;

    bool isMinimumFanAirFlowRatioDefaulted() const;

    double transitionTemperature() const;

    bool isTransitionTemperatureDefaulted() const;

    double transcriticalApproachTemperature() const;

    bool isTranscriticalApproachTemperatureDefaulted() const;

    double subcriticalTemperatureDifference() const;

    bool isSubcriticalTemperatureDifferenceDefaulted() const;

    double minimumCondensingTemperature() const;

    bool isMinimumCondensingTemperatureDefaulted() const;

    // boost::optional<ThermalZone> airInletNode() const;

    std::string endUseSubcategory() const;

    bool isEndUseSubcategoryDefaulted() const;

    double gasCoolerRefrigerantOperatingChargeInventory() const;

    bool isGasCoolerRefrigerantOperatingChargeInventoryDefaulted() const;

    double gasCoolerReceiverRefrigerantInventory() const;

    bool isGasCoolerReceiverRefrigerantInventoryDefaulted() const;

    double gasCoolerOutletPipingRefrigerantInventory() const;

    bool isGasCoolerOutletPipingRefrigerantInventoryDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setRatedTotalHeatRejectionRateCurve(const CurveLinear& curveLinear);

    void resetRatedTotalHeatRejectionRateCurve();

    bool setGasCoolerFanSpeedControlType(const std::string& gasCoolerFanSpeedControlType);

    void resetGasCoolerFanSpeedControlType();

    bool setRatedFanPower(double ratedFanPower);

    void resetRatedFanPower();

    bool setMinimumFanAirFlowRatio(double minimumFanAirFlowRatio);

    void resetMinimumFanAirFlowRatio();

    bool setTransitionTemperature(double transitionTemperature);

    void resetTransitionTemperature();

    bool setTranscriticalApproachTemperature(double transcriticalApproachTemperature);

    void resetTranscriticalApproachTemperature();

    bool setSubcriticalTemperatureDifference(double subcriticalTemperatureDifference);

    void resetSubcriticalTemperatureDifference();

    bool setMinimumCondensingTemperature(double minimumCondensingTemperature);

    void resetMinimumCondensingTemperature();

    // bool setAirInletNode(const ThermalZone& thermalZone);

    // void resetAirInletNode();

    bool setEndUseSubcategory(const std::string& endUseSubcategory);

    void resetEndUseSubcategory();

    bool setGasCoolerRefrigerantOperatingChargeInventory(double gasCoolerRefrigerantOperatingChargeInventory);

    void resetGasCoolerRefrigerantOperatingChargeInventory();

    bool setGasCoolerReceiverRefrigerantInventory(double gasCoolerReceiverRefrigerantInventory);

    void resetGasCoolerReceiverRefrigerantInventory();

    bool setGasCoolerOutletPipingRefrigerantInventory(double gasCoolerOutletPipingRefrigerantInventory);

    void resetGasCoolerOutletPipingRefrigerantInventory();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::RefrigerationGasCoolerAirCooled_Impl;

    explicit RefrigerationGasCoolerAirCooled(std::shared_ptr<detail::RefrigerationGasCoolerAirCooled_Impl> impl);

    friend class detail::RefrigerationGasCoolerAirCooled_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.RefrigerationGasCoolerAirCooled");
  };

  /** \relates RefrigerationGasCoolerAirCooled*/
  using OptionalRefrigerationGasCoolerAirCooled = boost::optional<RefrigerationGasCoolerAirCooled>;

  /** \relates RefrigerationGasCoolerAirCooled*/
  using RefrigerationGasCoolerAirCooledVector = std::vector<RefrigerationGasCoolerAirCooled>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_REFRIGERATIONGASCOOLERAIRCOOLED_HPP
