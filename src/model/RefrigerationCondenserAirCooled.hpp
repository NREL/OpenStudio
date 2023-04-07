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

    virtual ~RefrigerationCondenserAirCooled() = default;
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
