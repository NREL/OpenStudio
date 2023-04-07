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

#ifndef MODEL_EVAPORATIVECOOLERDIRECTRESEARCHSPECIAL_HPP
#define MODEL_EVAPORATIVECOOLERDIRECTRESEARCHSPECIAL_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "StraightComponent.hpp"
#include "Connection.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class Curve;

  namespace detail {
    class EvaporativeCoolerDirectResearchSpecial_Impl;
  }  // namespace detail

  /** EvaporativeCoolerDirectResearchSpecial is a StraightComponent that wraps the
 *  IDD object named "OS:EvaporativeCooler:Direct:ResearchSpecial". */
  class MODEL_API EvaporativeCoolerDirectResearchSpecial : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Constructs a new EvaporativeCoolerDirectResearchSpecial object and places it inside the
   *  model.  The object is fully initialized with all companion objects.
   */
    explicit EvaporativeCoolerDirectResearchSpecial(const Model& model, Schedule& schedule);

    virtual ~EvaporativeCoolerDirectResearchSpecial() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    EvaporativeCoolerDirectResearchSpecial(const EvaporativeCoolerDirectResearchSpecial& other) = default;
    EvaporativeCoolerDirectResearchSpecial(EvaporativeCoolerDirectResearchSpecial&& other) = default;
    EvaporativeCoolerDirectResearchSpecial& operator=(const EvaporativeCoolerDirectResearchSpecial&) = default;
    EvaporativeCoolerDirectResearchSpecial& operator=(EvaporativeCoolerDirectResearchSpecial&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters and Setters */
    //@{

    /** Returns the Schedule referred to by the Availability Schedule field. **/
    Schedule availabilitySchedule() const;

    /** \deprecated */
    Schedule availableSchedule() const;

    /** Sets the Schedule referred to by the AvailabilitySchedule field. **/
    bool setAvailabilitySchedule(Schedule& schedule);

    /** \deprecated */
    bool setAvailableSchedule(Schedule& schedule);

    /** Returns the value of the CoolerDesignEffectiveness field. **/
    double coolerDesignEffectiveness() const;

    /** Deprecated: In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Cooler Design Effectiveness" **/
    double coolerEffectiveness() const;

    /** Sets the value of the CoolerDesignEffectiveness field. **/
    bool setCoolerDesignEffectiveness(double value);

    /** Deprecated: In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Cooler Design Effectiveness" **/
    bool setCoolerEffectiveness(double value);

    /** Returns the value of the RecirculatingWaterPumpPowerConsumption field. **/
    /** In EnergyPlus 8.3.0 and above this property maps to the EnergyPlus field "Recirculating Water Pump Design Power" **/
    boost::optional<double> recirculatingWaterPumpPowerConsumption() const;

    /** Sets the value of the RecirculatingWaterPumpPowerConsumption field. **/
    bool setRecirculatingWaterPumpPowerConsumption(double value);

    void autosizeRecirculatingWaterPumpPowerConsumption();

    bool isRecirculatingWaterPumpPowerConsumptionAutosized() const;

    /** Returns the value of the PrimaryAirDesignFlowRate field. **/
    boost::optional<double> primaryAirDesignFlowRate() const;

    /** Sets the value of the PrimaryAirDesignFlowRate field. **/
    bool setPrimaryAirDesignFlowRate(double value);

    void autosizePrimaryAirDesignFlowRate();

    bool isPrimaryAirDesignFlowRateAutosized() const;

    /** Returns the Node referred to by the SensorNodeName field. **/
    boost::optional<Node> sensorNode() const;

    /** Sets the Node referred to by the SensorNodeName field. **/
    bool setSensorNode(const Node& node);

    /** Returns the value of the DriftLossFraction field. **/
    double driftLossFraction() const;

    /** Sets the value of the DriftLossFraction field. **/
    bool setDriftLossFraction(double value);

    /** Returns the value of the BlowdownConcentrationRatio field. **/
    double blowdownConcentrationRatio() const;

    /** Sets the value of the BlowdownConcentrationRatio field. **/
    bool setBlowdownConcentrationRatio(double value);

    boost::optional<Curve> effectivenessFlowRatioModifierCurve() const;

    bool setEffectivenessFlowRatioModifierCurve(const Curve& curve);

    void resetEffectivenessFlowRatioModifierCurve();

    double waterPumpPowerSizingFactor() const;

    bool setWaterPumpPowerSizingFactor(double waterPumpPowerSizingFactor);

    boost::optional<Curve> waterPumpPowerModifierCurve() const;

    bool setWaterPumpPowerModifierCurve(const Curve& curve);

    void resetWaterPumpPowerModifierCurve();

    /** Sets the minimum and maximum operation temperatures **/

    /** Returns the value of the EvaporativeOperationMinimumDrybulbTemperature field. **/
    double evaporativeOperationMinimumDrybulbTemperature() const;
    /** Sets the value of the EvaporativeOperationMinimumDrybulbTemperature field. **/
    bool setEvaporativeOperationMinimumDrybulbTemperature(double evaporativeOperationMinimumDrybulbTemperature);

    /** Returns the value of the EvaporativeOperationMaximumLimitWetbulbTemperature field. **/
    double evaporativeOperationMaximumLimitWetbulbTemperature() const;
    /** Sets the value of the EvaporativeOperationMaximumLimitWetbulbTemperature field. **/
    bool setEvaporativeOperationMaximumLimitWetbulbTemperature(double evaporativeOperationMaximumLimitWetbulbTemperature);

    /** Returns the value of the EvaporativeOperationMaximumLimitDrybulbTemperature field. **/
    double evaporativeOperationMaximumLimitDrybulbTemperature() const;
    /** Sets the value of the EvaporativeOperationMaximumLimitDrybulbTemperature field. **/
    bool setEvaporativeOperationMaximumLimitDrybulbTemperature(double evaporativeOperationMaximumLimitDrybulbTemperature);

    boost::optional<double> autosizedRecirculatingWaterPumpPowerConsumption() const;

    boost::optional<double> autosizedPrimaryAirDesignFlowRate() const;

    //@}
   protected:
    friend class Model;
    friend class openstudio::IdfObject;

    /// @cond
    using ImplType = detail::EvaporativeCoolerDirectResearchSpecial_Impl;

    explicit EvaporativeCoolerDirectResearchSpecial(std::shared_ptr<detail::EvaporativeCoolerDirectResearchSpecial_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.EvaporativeCoolerDirectResearchSpecial");
    /// @endcond
  };

  /** \relates EvaporativeCoolerDirectResearchSpecial */
  using OptionalEvaporativeCoolerDirectResearchSpecial = boost::optional<EvaporativeCoolerDirectResearchSpecial>;

  /** \relates EvaporativeCoolerDirectResearchSpecial */
  using EvaporativeCoolerDirectResearchSpecialVector = std::vector<EvaporativeCoolerDirectResearchSpecial>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EVAPORATIVECOOLERDIRECTRESEARCHSPECIAL_HPP
