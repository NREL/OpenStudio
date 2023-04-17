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

#ifndef MODEL_COILHEATINGDXMULTISPEED_HPP
#define MODEL_COILHEATINGDXMULTISPEED_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class Curve;
  class CoilHeatingDXMultiSpeedStageData;
  class AirflowNetworkEquivalentDuct;

  namespace detail {

    class CoilHeatingDXMultiSpeed_Impl;

  }  // namespace detail

  /** CoilHeatingDXMultiSpeed is a StraightComponent that wraps the OpenStudio IDD object 'OS:Coil:Heating:DX:MultiSpeed'. */
  class MODEL_API CoilHeatingDXMultiSpeed : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilHeatingDXMultiSpeed(const Model& model);

    virtual ~CoilHeatingDXMultiSpeed() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilHeatingDXMultiSpeed(const CoilHeatingDXMultiSpeed& other) = default;
    CoilHeatingDXMultiSpeed(CoilHeatingDXMultiSpeed&& other) = default;
    CoilHeatingDXMultiSpeed& operator=(const CoilHeatingDXMultiSpeed&) = default;
    CoilHeatingDXMultiSpeed& operator=(CoilHeatingDXMultiSpeed&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> defrostStrategyValues();

    static std::vector<std::string> defrostControlValues();

    static std::vector<std::string> fuelTypeValues();

    /** @name Getters */
    //@{

    boost::optional<Schedule> availabilitySchedule() const;

    double minimumOutdoorDryBulbTemperatureforCompressorOperation() const;

    boost::optional<double> outdoorDryBulbTemperaturetoTurnOnCompressor() const;

    double crankcaseHeaterCapacity() const;

    double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const;

    boost::optional<Curve> defrostEnergyInputRatioFunctionofTemperatureCurve() const;

    double maximumOutdoorDryBulbTemperatureforDefrostOperation() const;

    std::string defrostStrategy() const;

    std::string defrostControl() const;

    double defrostTimePeriodFraction() const;

    boost::optional<double> resistiveDefrostHeaterCapacity() const;

    bool isResistiveDefrostHeaterCapacityAutosized() const;

    bool applyPartLoadFractiontoSpeedsGreaterthan1() const;

    std::string fuelType() const;

    int regionnumberforCalculatingHSPF() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setMinimumOutdoorDryBulbTemperatureforCompressorOperation(double minimumOutdoorDryBulbTemperatureforCompressorOperation);

    bool setOutdoorDryBulbTemperaturetoTurnOnCompressor(double outdoorDryBulbTemperaturetoTurnOnCompressor);

    void resetOutdoorDryBulbTemperaturetoTurnOnCompressor();

    bool setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity);

    bool setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);

    bool setDefrostEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve);

    void resetDefrostEnergyInputRatioFunctionofTemperatureCurve();

    bool setMaximumOutdoorDryBulbTemperatureforDefrostOperation(double maximumOutdoorDryBulbTemperatureforDefrostOperation);

    bool setDefrostStrategy(const std::string& defrostStrategy);

    bool setDefrostControl(const std::string& defrostControl);

    bool setDefrostTimePeriodFraction(double defrostTimePeriodFraction);

    bool setResistiveDefrostHeaterCapacity(double resistiveDefrostHeaterCapacity);

    void autosizeResistiveDefrostHeaterCapacity();

    bool setApplyPartLoadFractiontoSpeedsGreaterthan1(bool applyPartLoadFractiontoSpeedsGreaterthan1);

    bool setFuelType(const std::string& fuelType);

    bool setRegionnumberforCalculatingHSPF(int regionnumberforCalculatingHSPF);

    //@}
    /** @name Other */
    //@{

    /** Return the performance data for each stage. **/
    std::vector<CoilHeatingDXMultiSpeedStageData> stages() const;

    unsigned numberOfStages() const;

    /*
   * Get the index of a given CoilHeatingDXMultiSpeedStageData (1-indexed)
   */
    boost::optional<unsigned> stageIndex(const CoilHeatingDXMultiSpeedStageData& stage) const;

    /*
   * Add a new stage after all of the existing stages.
   */
    bool addStage(const CoilHeatingDXMultiSpeedStageData& stage);

    /*
   * Add a new CoilHeatingDXMultiSpeedStageData to the list which a given index (1 to x).
   * Internally calls addStage then setStageIndex, see remarks there
   */
    bool addStage(const CoilHeatingDXMultiSpeedStageData& stage, unsigned index);

    /*
   * You can shuffle the priority of a given CoilHeatingDXMultiSpeedStageData after having added it
   * If index is below 1, it's reset to 1.
   * If index is greater than the number of stages, will reset to last
   */
    bool setStageIndex(const CoilHeatingDXMultiSpeedStageData& stage, unsigned index);

    /*
   * Set all stages using a list of CoilHeatingDXMultiSpeedStageDatas
   * Internally calls addStage, and will return the global status, but will continue trying if there are problems
   * (eg: if you make a vector larger than the number of accepted stages, or a vector that has a stage from another model, the valid stages will be
   * added indeed, but it'll eventually return false)
   */
    bool setStages(const std::vector<CoilHeatingDXMultiSpeedStageData>& stages);

    /*
   * Removes all CoilHeatingDXMultiSpeedStageDatas in this object
   */
    void removeAllStages();

    /*
   * Remove the given CoilHeatingDXMultiSpeedStageData from this object's stages
   */
    bool removeStage(const CoilHeatingDXMultiSpeedStageData& stage);

    /*
   * Remove the CoilHeatingDXMultiSpeedStageData at the given index (1-indexed)
   */
    bool removeStage(unsigned index);

    /** Creates a new equivalent duct object if an object is not already attached. */
    AirflowNetworkEquivalentDuct getAirflowNetworkEquivalentDuct(double length, double diameter);

    /** Returns the attached equivalent duct object, if any. */
    boost::optional<AirflowNetworkEquivalentDuct> airflowNetworkEquivalentDuct() const;

    boost::optional<double> autosizedResistiveDefrostHeaterCapacity() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilHeatingDXMultiSpeed_Impl;

    explicit CoilHeatingDXMultiSpeed(std::shared_ptr<detail::CoilHeatingDXMultiSpeed_Impl> impl);

    friend class detail::CoilHeatingDXMultiSpeed_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilHeatingDXMultiSpeed");
  };

  /** \relates CoilHeatingDXMultiSpeed*/
  using OptionalCoilHeatingDXMultiSpeed = boost::optional<CoilHeatingDXMultiSpeed>;

  /** \relates CoilHeatingDXMultiSpeed*/
  using CoilHeatingDXMultiSpeedVector = std::vector<CoilHeatingDXMultiSpeed>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGDXMULTISPEED_HPP
