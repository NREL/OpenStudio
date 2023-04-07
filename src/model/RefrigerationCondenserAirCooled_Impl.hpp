/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

      virtual ~RefrigerationCondenserAirCooled_Impl() = default;

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
