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

#ifndef MODEL_COILWATERHEATINGAIRTOWATERHEATPUMPVARIABLESPEED_HPP
#define MODEL_COILWATERHEATINGAIRTOWATERHEATPUMPVARIABLESPEED_HPP

#include "ModelAPI.hpp"
#include "HVACComponent.hpp"

namespace openstudio {

namespace model {

  class Curve;
  class CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData;

  namespace detail {

    class CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl;

  }  // namespace detail

  /** CoilWaterHeatingAirToWaterHeatPumpVariableSpeed is a ModelObject that wraps the OpenStudio IDD object 'OS:Coil:WaterHeating:AirToWaterHeatPump:VariableSpeed'. */
  class MODEL_API CoilWaterHeatingAirToWaterHeatPumpVariableSpeed : public HVACComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilWaterHeatingAirToWaterHeatPumpVariableSpeed(const Model& model);

    virtual ~CoilWaterHeatingAirToWaterHeatPumpVariableSpeed() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilWaterHeatingAirToWaterHeatPumpVariableSpeed(const CoilWaterHeatingAirToWaterHeatPumpVariableSpeed& other) = default;
    CoilWaterHeatingAirToWaterHeatPumpVariableSpeed(CoilWaterHeatingAirToWaterHeatPumpVariableSpeed&& other) = default;
    CoilWaterHeatingAirToWaterHeatPumpVariableSpeed& operator=(const CoilWaterHeatingAirToWaterHeatPumpVariableSpeed&) = default;
    CoilWaterHeatingAirToWaterHeatPumpVariableSpeed& operator=(CoilWaterHeatingAirToWaterHeatPumpVariableSpeed&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    int nominalSpeedLevel() const;

    double ratedWaterHeatingCapacity() const;

    double ratedEvaporatorInletAirDryBulbTemperature() const;

    double ratedEvaporatorInletAirWetBulbTemperature() const;

    double ratedCondenserInletWaterTemperature() const;

    boost::optional<double> ratedEvaporatorAirFlowRate() const;

    bool isRatedEvaporatorAirFlowRateAutocalculated() const;

    boost::optional<double> ratedCondenserWaterFlowRate() const;

    bool isRatedCondenserWaterFlowRateAutocalculated() const;

    std::string evaporatorFanPowerIncludedinRatedCOP() const;

    std::string condenserPumpPowerIncludedinRatedCOP() const;

    std::string condenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP() const;

    double fractionofCondenserPumpHeattoWater() const;

    double crankcaseHeaterCapacity() const;

    double maximumAmbientTemperatureforCrankcaseHeaterOperation() const;

    std::string evaporatorAirTemperatureTypeforCurveObjects() const;

    Curve partLoadFractionCorrelationCurve() const;

    //@}
    /** @name Setters */
    //@{

    bool setNominalSpeedLevel(int nominalSpeedLevel);

    bool setRatedWaterHeatingCapacity(double ratedWaterHeatingCapacity);

    bool setRatedEvaporatorInletAirDryBulbTemperature(double ratedEvaporatorInletAirDryBulbTemperature);

    bool setRatedEvaporatorInletAirWetBulbTemperature(double ratedEvaporatorInletWetDryBulbTemperature);

    bool setRatedCondenserInletWaterTemperature(double ratedCondenserInletWaterTemperature);

    bool setRatedEvaporatorAirFlowRate(double ratedEvaporatorAirFlowRate);

    void autocalculateRatedEvaporatorAirFlowRate();

    bool setRatedCondenserWaterFlowRate(double ratedCondenserWaterFlowRate);

    void autocalculateRatedCondenserWaterFlowRate();

    bool setEvaporatorFanPowerIncludedinRatedCOP(const std::string& evaporatorFanPowerIncludedinRatedCOP);

    bool setCondenserPumpPowerIncludedinRatedCOP(const std::string& condenserPumpPowerIncludedinRatedCOP);

    bool setCondenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP(const std::string& condenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP);

    bool setFractionofCondenserPumpHeattoWater(double fractionofCondenserPumpHeattoWater);

    bool setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity);

    bool setMaximumAmbientTemperatureforCrankcaseHeaterOperation(double maximumAmbientTemperatureforCrankcaseHeaterOperation);

    bool setEvaporatorAirTemperatureTypeforCurveObjects(const std::string& evaporatorAirTemperatureTypeforCurveObjects);

    bool setPartLoadFractionCorrelationCurve(const Curve& partLoadFractionCorrelationCurve);

    //@}
    /** @name Other */
    //@{

    std::vector<CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData> speeds() const;

    bool addSpeed(const CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData& speed);

    void removeSpeed(const CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData& speed);

    void removeAllSpeeds();

    boost::optional<double> autocalculatedRatedEvaporatorAirFlowRate() const;

    boost::optional<double> autocalculatedRatedCondenserWaterFlowRate() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl;

    explicit CoilWaterHeatingAirToWaterHeatPumpVariableSpeed(std::shared_ptr<detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl> impl);

    friend class detail::CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilWaterHeatingAirToWaterHeatPumpVariableSpeed");
  };

  /** \relates CoilWaterHeatingAirToWaterHeatPumpVariableSpeed*/
  using OptionalCoilWaterHeatingAirToWaterHeatPumpVariableSpeed = boost::optional<CoilWaterHeatingAirToWaterHeatPumpVariableSpeed>;

  /** \relates CoilWaterHeatingAirToWaterHeatPumpVariableSpeed*/
  using CoilWaterHeatingAirToWaterHeatPumpVariableSpeedVector = std::vector<CoilWaterHeatingAirToWaterHeatPumpVariableSpeed>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILWATERHEATINGAIRTOWATERHEATPUMPVARIABLESPEED_HPP
