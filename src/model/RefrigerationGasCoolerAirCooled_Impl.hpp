/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_REFRIGERATIONGASCOOLERAIRCOOLED_IMPL_HPP
#define MODEL_REFRIGERATIONGASCOOLERAIRCOOLED_IMPL_HPP

#include "ModelAPI.hpp"
#include "ParentObject_Impl.hpp"

namespace openstudio {
namespace model {

  class CurveLinear;
  // class ThermalZone;

  namespace detail {

    /** RefrigerationGasCoolerAirCooled_Impl is a ParentObject_Impl that is the implementation class for RefrigerationGasCoolerAirCooled.*/
    class MODEL_API RefrigerationGasCoolerAirCooled_Impl : public ParentObject_Impl
    {
      //
     public:
      /** @name Constructors and Destructors */
      //@{

      RefrigerationGasCoolerAirCooled_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      RefrigerationGasCoolerAirCooled_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      RefrigerationGasCoolerAirCooled_Impl(const RefrigerationGasCoolerAirCooled_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~RefrigerationGasCoolerAirCooled_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<IddObjectType> allowableChildTypes() const override;

      virtual std::vector<ModelObject> children() const override;

      //@}
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

      bool setRatedTotalHeatRejectionRateCurve(const boost::optional<CurveLinear>& curveLinear);

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

      // bool setAirInletNode(const boost::optional<ThermalZone>& thermalZone);

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
     private:
      REGISTER_LOGGER("openstudio.model.RefrigerationGasCoolerAirCooled");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_REFRIGERATIONGASCOOLERAIRCOOLED_IMPL_HPP
