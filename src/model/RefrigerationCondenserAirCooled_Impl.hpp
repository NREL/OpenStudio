/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_REFRIGERATIONCONDENSERAIRCOOLED_IMPL_HPP
#define MODEL_REFRIGERATIONCONDENSERAIRCOOLED_IMPL_HPP

#include "ModelAPI.hpp"
#include "ParentObject_Impl.hpp"

namespace openstudio {
namespace model {

  class RefrigerationSystem;
  class CurveLinear;
  class ThermalZone;

  namespace detail {

    /** RefrigerationCondenserAirCooled_Impl is a ParentObject that is the implementation class for RefrigerationCondenserAirCooled.*/
    class MODEL_API RefrigerationCondenserAirCooled_Impl : public ParentObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      RefrigerationCondenserAirCooled_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      RefrigerationCondenserAirCooled_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      RefrigerationCondenserAirCooled_Impl(const RefrigerationCondenserAirCooled_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~RefrigerationCondenserAirCooled_Impl() override = default;

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

      bool setRatedEffectiveTotalHeatRejectionRateCurve(const boost::optional<CurveLinear>& curveLinear);

      void resetRatedEffectiveTotalHeatRejectionRateCurve();

      bool setRatedSubcoolingTemperatureDifference(double ratedSubcoolingTemperatureDifference);

      void resetRatedSubcoolingTemperatureDifference();

      bool setCondenserFanSpeedControlType(const std::string& condenserFanSpeedControlType);

      void resetCondenserFanSpeedControlType();

      bool setRatedFanPower(double ratedFanPower);

      void resetRatedFanPower();

      bool setMinimumFanAirFlowRatio(double minimumFanAirFlowRatio);

      void resetMinimumFanAirFlowRatio();

      bool setAirInletZone(const boost::optional<ThermalZone>& thermalZone);

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

      boost::optional<RefrigerationSystem> system() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.RefrigerationCondenserAirCooled");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_REFRIGERATIONCONDENSERAIRCOOLED_IMPL_HPP
