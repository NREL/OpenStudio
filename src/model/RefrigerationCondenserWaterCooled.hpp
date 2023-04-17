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

    virtual ~RefrigerationCondenserWaterCooled() = default;
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
