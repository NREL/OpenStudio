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

#ifndef MODEL_CHILLERELECTRICASHRAE205_HPP
#define MODEL_CHILLERELECTRICASHRAE205_HPP

#include <model/ModelAPI.hpp>
#include "WaterToWaterComponent.hpp"

namespace openstudio {

namespace model {

  class ExternalFile;
  class Node;
  class ThermalZone;
  class Schedule;

  namespace detail {

    class ChillerElectricASHRAE205_Impl;

  }  // namespace detail

  /** ChillerElectricASHRAE205 is a WaterToWaterComponent that wraps the OpenStudio IDD object 'OS:Chiller:Electric:ASHRAE205'. */
  class MODEL_API ChillerElectricASHRAE205 : public WaterToWaterComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ChillerElectricASHRAE205(const ExternalFile& representationFile);

    virtual ~ChillerElectricASHRAE205() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ChillerElectricASHRAE205(const ChillerElectricASHRAE205& other) = default;
    ChillerElectricASHRAE205(ChillerElectricASHRAE205&& other) = default;
    ChillerElectricASHRAE205& operator=(const ChillerElectricASHRAE205&) = default;
    ChillerElectricASHRAE205& operator=(ChillerElectricASHRAE205&&) = default;

    // TODO: once supported by E+, re-enable
    static constexpr bool isHeatRecoverySupportedByEnergyplus = false;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> performanceInterpolationMethodValues();

    static std::vector<std::string> ambientTemperatureIndicatorValues();

    static std::vector<std::string> chillerFlowModeValues();

    /** @name Getters */
    //@{

    ExternalFile representationFile() const;

    std::string performanceInterpolationMethod() const;

    boost::optional<double> ratedCapacity() const;
    bool isRatedCapacityAutosized() const;

    double sizingFactor() const;

    std::string ambientTemperatureIndicator() const;

    boost::optional<Schedule> ambientTemperatureSchedule() const;

    boost::optional<ThermalZone> ambientTemperatureZone() const;

    boost::optional<std::string> ambientTemperatureOutdoorAirNodeName() const;

    boost::optional<double> chilledWaterMaximumRequestedFlowRate() const;
    bool isChilledWaterMaximumRequestedFlowRateAutosized() const;

    boost::optional<double> condenserMaximumRequestedFlowRate() const;
    bool isCondenserMaximumRequestedFlowRateAutosized() const;

    std::string chillerFlowMode() const;

    boost::optional<double> oilCoolerDesignFlowRate() const;

    boost::optional<double> auxiliaryCoolingDesignFlowRate() const;

    std::string endUseSubcategory() const;
    bool isEndUseSubcategoryDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setRepresentationFile(const ExternalFile& externalFile);

    bool setPerformanceInterpolationMethod(const std::string& performanceInterpolationMethod);

    bool setRatedCapacity(double ratedCapacity);
    void autosizeRatedCapacity();

    bool setSizingFactor(double sizingFactor);

    // This is done via the setAmbientXXX methods
    // bool setAmbientTemperatureIndicator(const std::string& ambientTemperatureIndicator);

    /** This will make the Ambient Temperature Indicator = 'Schedule' */
    bool setAmbientTemperatureSchedule(Schedule& schedule);
    /** This will reset the Ambient Temperature Indicator = 'Outdoors' */
    void resetAmbientTemperatureSchedule();

    /** This will make the Ambient Temperature Indicator = 'Zone' */
    bool setAmbientTemperatureZone(const ThermalZone& thermalZone);
    /** This will reset the Ambient Temperature Indicator = 'Outdoors' */
    void resetAmbientTemperatureZone();

    /** This will make the Ambient Temperature Indicator = 'Outdoors' */
    bool setAmbientTemperatureOutdoorAirNodeName(const std::string& ambientTemperatureOutdoorAirNodeName);
    /** This will reset the Ambient Temperature Indicator = 'Outdoors'. A default Outdoor Air Node Name will be used in the ForwardTranslator */
    void resetAmbientTemperatureOutdoorAirNodeName();

    bool setChilledWaterMaximumRequestedFlowRate(double chilledWaterMaximumRequestedFlowRate);
    void autosizeChilledWaterMaximumRequestedFlowRate();

    bool setCondenserMaximumRequestedFlowRate(double condenserMaximumRequestedFlowRate);
    void autosizeCondenserMaximumRequestedFlowRate();

    bool setChillerFlowMode(const std::string& chillerFlowMode);

    bool setOilCoolerDesignFlowRate(double oilCoolerDesignFlowRate);
    void resetOilCoolerDesignFlowRate();

    bool setAuxiliaryCoolingDesignFlowRate(double auxiliaryCoolingDesignFlowRate);
    void resetAuxiliaryCoolingDesignFlowRate();

    bool setEndUseSubcategory(const std::string& endUseSubcategory);
    void resetEndUseSubcategory();

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedRatedCapacity();

    boost::optional<double> autosizedChilledWaterMaximumRequestedFlowRate();

    boost::optional<double> autosizedCondenserMaximumRequestedFlowRate();

    /** Convenience Function to return the Chilled Water Loop (chiller on supply) **/
    boost::optional<PlantLoop> chilledWaterLoop() const;
    // Same as supplyInletModelObject, but cast to a Node
    boost::optional<Node> chilledWaterInletNode() const;
    boost::optional<Node> chilledWaterOutletNode() const;

    /** Convenience Function to return the Condenser Water Loop (chiller on demand side) **/
    boost::optional<PlantLoop> condenserWaterLoop() const;
    boost::optional<Node> condenserInletNode() const;
    boost::optional<Node> condenserOutletNode() const;

    // Not implemented in E+ 22.2.0, but reserved as a Tertiary Loop for now
    boost::optional<PlantLoop> heatRecoveryLoop() const;
    boost::optional<Node> heatRecoveryInletNode() const;
    boost::optional<Node> heatRecoveryOutletNode() const;

    unsigned oilCoolerInletPort() const;
    boost::optional<ModelObject> oilCoolerInletModelObject() const;
    boost::optional<Node> oilCoolerInletNode() const;
    unsigned oilCoolerOutletPort() const;
    boost::optional<ModelObject> oilCoolerOutletModelObject() const;
    boost::optional<Node> oilCoolerOutletNode() const;
    boost::optional<PlantLoop> oilCoolerLoop() const;
    bool addDemandBranchOnOilCoolerLoop(PlantLoop& plantLoop);
    bool addToOilCoolerLoopNode(Node& node);
    bool removeFromOilCoolerLoop();

    unsigned auxiliaryInletPort() const;
    boost::optional<ModelObject> auxiliaryInletModelObject() const;
    boost::optional<Node> auxiliaryInletNode() const;
    unsigned auxiliaryOutletPort() const;
    boost::optional<ModelObject> auxiliaryOutletModelObject() const;
    boost::optional<Node> auxiliaryOutletNode() const;
    bool addDemandBranchOnAuxiliaryLoop(PlantLoop& plantLoop);
    bool addToAuxiliaryLoopNode(Node& node);
    bool removeFromAuxiliaryLoop();
    boost::optional<PlantLoop> auxiliaryLoop() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::ChillerElectricASHRAE205_Impl;

    explicit ChillerElectricASHRAE205(std::shared_ptr<detail::ChillerElectricASHRAE205_Impl> impl);

    friend class detail::ChillerElectricASHRAE205_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ChillerElectricASHRAE205");
  };

  /** \relates ChillerElectricASHRAE205*/
  using OptionalChillerElectricASHRAE205 = boost::optional<ChillerElectricASHRAE205>;

  /** \relates ChillerElectricASHRAE205*/
  using ChillerElectricASHRAE205Vector = std::vector<ChillerElectricASHRAE205>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CHILLERELECTRICASHRAE205_HPP
