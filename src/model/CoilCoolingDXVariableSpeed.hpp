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

#ifndef MODEL_COILCOOLINGDXVARIABLESPEED_HPP
#define MODEL_COILCOOLINGDXVARIABLESPEED_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Curve;
  // class WaterStorageTank;
  class Schedule;
  class CoilCoolingDXVariableSpeedSpeedData;

  namespace detail {

    class CoilCoolingDXVariableSpeed_Impl;

  }  // namespace detail

  /** CoilCoolingDXVariableSpeed is a StraightComponent that wraps the OpenStudio IDD object 'OS:Coil:Cooling:DX:VariableSpeed'. */
  class MODEL_API CoilCoolingDXVariableSpeed : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilCoolingDXVariableSpeed(const Model& model);

    explicit CoilCoolingDXVariableSpeed(const Model& model, const Curve& partLoadFraction);

    virtual ~CoilCoolingDXVariableSpeed() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilCoolingDXVariableSpeed(const CoilCoolingDXVariableSpeed& other) = default;
    CoilCoolingDXVariableSpeed(CoilCoolingDXVariableSpeed&& other) = default;
    CoilCoolingDXVariableSpeed& operator=(const CoilCoolingDXVariableSpeed&) = default;
    CoilCoolingDXVariableSpeed& operator=(CoilCoolingDXVariableSpeed&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> condenserTypeValues();

    /** @name Getters */
    //@{

    int nominalSpeedLevel() const;

    boost::optional<double> grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel() const;

    bool isGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevelAutosized() const;

    boost::optional<double> ratedAirFlowRateAtSelectedNominalSpeedLevel() const;

    bool isRatedAirFlowRateAtSelectedNominalSpeedLevelAutosized() const;

    double nominalTimeforCondensatetoBeginLeavingtheCoil() const;

    double initialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity() const;

    Curve energyPartLoadFractionCurve() const;

    // boost::optional<std::string> condenserAirInletNodeName() const;

    std::string condenserType() const;

    boost::optional<double> evaporativeCondenserPumpRatedPowerConsumption() const;

    bool isEvaporativeCondenserPumpRatedPowerConsumptionAutosized() const;

    double crankcaseHeaterCapacity() const;

    double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const;

    // boost::optional<WaterStorageTank> supplyWaterStorageTank() const;

    // boost::optional<WaterStorageTank> condensateCollectionWaterStorageTank() const;

    double basinHeaterCapacity() const;

    double basinHeaterSetpointTemperature() const;

    boost::optional<Schedule> basinHeaterOperatingSchedule() const;

    double minimumOutdoorDryBulbTemperatureforCompressorOperation() const;

    //@}
    /** @name Setters */
    //@{

    bool setNominalSpeedLevel(int nominalSpeedLevel);

    bool setGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel(double grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel);

    void autosizeGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel();

    bool setRatedAirFlowRateAtSelectedNominalSpeedLevel(double ratedAirFlowRateAtSelectedNominalSpeedLevel);

    void autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel();

    bool setNominalTimeforCondensatetoBeginLeavingtheCoil(double nominalTimeforCondensatetoBeginLeavingtheCoil);

    bool setInitialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity(
      double initialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity);

    bool setEnergyPartLoadFractionCurve(const Curve& curve);

    // bool setCondenserAirInletNodeName(const std::string& condenserAirInletNodeName);

    // void resetCondenserAirInletNodeName();

    bool setCondenserType(const std::string& condenserType);

    bool setEvaporativeCondenserPumpRatedPowerConsumption(double evaporativeCondenserPumpRatedPowerConsumption);

    void autosizeEvaporativeCondenserPumpRatedPowerConsumption();

    bool setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity);

    bool setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);

    // bool setSupplyWaterStorageTank(const WaterStorageTank& waterStorageTank);

    // void resetSupplyWaterStorageTank();

    // bool setCondensateCollectionWaterStorageTank(const WaterStorageTank& waterStorageTank);

    // void resetCondensateCollectionWaterStorageTank();

    bool setBasinHeaterCapacity(double basinHeaterCapacity);

    bool setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature);

    bool setBasinHeaterOperatingSchedule(Schedule& schedule);

    void resetBasinHeaterOperatingSchedule();

    bool setMinimumOutdoorDryBulbTemperatureforCompressorOperation(double minimumOutdoorDryBulbTemperatureforCompressorOperation);

    //@}
    /** @name Other */
    //@{

    std::vector<CoilCoolingDXVariableSpeedSpeedData> speeds() const;

    bool addSpeed(const CoilCoolingDXVariableSpeedSpeedData& speed);

    void removeSpeed(const CoilCoolingDXVariableSpeedSpeedData& speed);

    void removeAllSpeeds();

    boost::optional<double> autosizedGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel() const;

    boost::optional<double> autosizedRatedAirFlowRateAtSelectedNominalSpeedLevel() const;

    boost::optional<double> autosizedEvaporativeCondenserPumpRatedPowerConsumption() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilCoolingDXVariableSpeed_Impl;

    explicit CoilCoolingDXVariableSpeed(std::shared_ptr<detail::CoilCoolingDXVariableSpeed_Impl> impl);

    friend class detail::CoilCoolingDXVariableSpeed_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilCoolingDXVariableSpeed");
  };

  /** \relates CoilCoolingDXVariableSpeed*/
  using OptionalCoilCoolingDXVariableSpeed = boost::optional<CoilCoolingDXVariableSpeed>;

  /** \relates CoilCoolingDXVariableSpeed*/
  using CoilCoolingDXVariableSpeedVector = std::vector<CoilCoolingDXVariableSpeed>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGDXVARIABLESPEED_HPP
