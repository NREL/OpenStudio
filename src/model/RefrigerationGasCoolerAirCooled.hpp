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

    virtual ~RefrigerationGasCoolerAirCooled() = default;
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
