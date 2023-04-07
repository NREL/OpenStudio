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

#ifndef MODEL_COILWATERHEATINGAIRTOWATERHEATPUMP_HPP
#define MODEL_COILWATERHEATINGAIRTOWATERHEATPUMP_HPP

#include "ModelAPI.hpp"
#include "HVACComponent.hpp"

namespace openstudio {

namespace model {

  class Curve;

  namespace detail {

    class CoilWaterHeatingAirToWaterHeatPump_Impl;

  }  // namespace detail

  /** CoilWaterHeatingAirToWaterHeatPump is a ModelObject that wraps the OpenStudio IDD object 'OS:Coil:WaterHeating:AirToWaterHeatPump'.
 *  As of EnergyPlus version 8.4.0 this object maps to Coil:WaterHeating:AirToWaterHeatPump:Pumped in idf format.
 */
  class MODEL_API CoilWaterHeatingAirToWaterHeatPump : public HVACComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilWaterHeatingAirToWaterHeatPump(const Model& model);

    explicit CoilWaterHeatingAirToWaterHeatPump(const Model& model, Curve& heatingCapacityFunctionofTemperatureCurve,
                                                Curve& heatingCapacityFunctionofAirFlowFractionCurve,
                                                Curve& heatingCapacityFunctionofWaterFlowFractionCurve, Curve& heatingCOPFunctionofTemperatureCurve,
                                                Curve& heatingCOPFunctionofAirFlowFractionCurve, Curve& heatingCOPFunctionofWaterFlowFractionCurve,
                                                Curve& partLoadFractionCorrelationCurve);

    virtual ~CoilWaterHeatingAirToWaterHeatPump() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilWaterHeatingAirToWaterHeatPump(const CoilWaterHeatingAirToWaterHeatPump& other) = default;
    CoilWaterHeatingAirToWaterHeatPump(CoilWaterHeatingAirToWaterHeatPump&& other) = default;
    CoilWaterHeatingAirToWaterHeatPump& operator=(const CoilWaterHeatingAirToWaterHeatPump&) = default;
    CoilWaterHeatingAirToWaterHeatPump& operator=(CoilWaterHeatingAirToWaterHeatPump&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> evaporatorAirTemperatureTypeforCurveObjectsValues();

    /** @name Getters */
    //@{

    double ratedHeatingCapacity() const;

    double ratedCOP() const;

    double ratedSensibleHeatRatio() const;

    double ratedEvaporatorInletAirDryBulbTemperature() const;

    double ratedEvaporatorInletAirWetBulbTemperature() const;

    double ratedCondenserInletWaterTemperature() const;

    boost::optional<double> ratedEvaporatorAirFlowRate() const;

    bool isRatedEvaporatorAirFlowRateAutosized() const;

    boost::optional<double> ratedCondenserWaterFlowRate() const;

    bool isRatedCondenserWaterFlowRateAutosized() const;

    bool evaporatorFanPowerIncludedinRatedCOP() const;

    bool condenserPumpPowerIncludedinRatedCOP() const;

    bool condenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP() const;

    double condenserWaterPumpPower() const;

    double fractionofCondenserPumpHeattoWater() const;

    double crankcaseHeaterCapacity() const;

    double maximumAmbientTemperatureforCrankcaseHeaterOperation() const;

    std::string evaporatorAirTemperatureTypeforCurveObjects() const;

    Curve heatingCapacityFunctionofTemperatureCurve() const;

    Curve heatingCapacityFunctionofAirFlowFractionCurve() const;

    Curve heatingCapacityFunctionofWaterFlowFractionCurve() const;

    Curve heatingCOPFunctionofTemperatureCurve() const;

    Curve heatingCOPFunctionofAirFlowFractionCurve() const;

    Curve heatingCOPFunctionofWaterFlowFractionCurve() const;

    Curve partLoadFractionCorrelationCurve() const;

    //@}
    /** @name Setters */
    //@{

    bool setRatedHeatingCapacity(double ratedHeatingCapacity);

    bool setRatedCOP(double ratedCOP);

    bool setRatedSensibleHeatRatio(double ratedSensibleHeatRatio);

    bool setRatedEvaporatorInletAirDryBulbTemperature(double ratedEvaporatorInletAirDryBulbTemperature);

    bool setRatedEvaporatorInletAirWetBulbTemperature(double ratedEvaporatorInletAirWetBulbTemperature);

    bool setRatedCondenserInletWaterTemperature(double ratedCondenserInletWaterTemperature);

    bool setRatedEvaporatorAirFlowRate(double ratedEvaporatorAirFlowRate);

    void autosizeRatedEvaporatorAirFlowRate();

    bool setRatedCondenserWaterFlowRate(double ratedCondenserWaterFlowRate);

    void autosizeRatedCondenserWaterFlowRate();

    bool setEvaporatorFanPowerIncludedinRatedCOP(bool evaporatorFanPowerIncludedinRatedCOP);

    bool setCondenserPumpPowerIncludedinRatedCOP(bool condenserPumpPowerIncludedinRatedCOP);

    bool setCondenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP(bool condenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP);

    bool setCondenserWaterPumpPower(double condenserWaterPumpPower);

    bool setFractionofCondenserPumpHeattoWater(double fractionofCondenserPumpHeattoWater);

    bool setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity);

    bool setMaximumAmbientTemperatureforCrankcaseHeaterOperation(double maximumAmbientTemperatureforCrankcaseHeaterOperation);

    bool setEvaporatorAirTemperatureTypeforCurveObjects(const std::string& evaporatorAirTemperatureTypeforCurveObjects);

    bool setHeatingCapacityFunctionofTemperatureCurve(const Curve& curve);

    bool setHeatingCapacityFunctionofAirFlowFractionCurve(const Curve& curve);

    bool setHeatingCapacityFunctionofWaterFlowFractionCurve(const Curve& curve);

    bool setHeatingCOPFunctionofTemperatureCurve(const Curve& curve);

    bool setHeatingCOPFunctionofAirFlowFractionCurve(const Curve& curve);

    bool setHeatingCOPFunctionofWaterFlowFractionCurve(const Curve& curve);

    bool setPartLoadFractionCorrelationCurve(const Curve& curve);

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedRatedEvaporatorAirFlowRate() const;

    boost::optional<double> autosizedRatedCondenserWaterFlowRate() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilWaterHeatingAirToWaterHeatPump_Impl;

    explicit CoilWaterHeatingAirToWaterHeatPump(std::shared_ptr<detail::CoilWaterHeatingAirToWaterHeatPump_Impl> impl);

    friend class detail::CoilWaterHeatingAirToWaterHeatPump_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilWaterHeatingAirToWaterHeatPump");
  };

  /** \relates CoilWaterHeatingAirToWaterHeatPump*/
  using OptionalCoilWaterHeatingAirToWaterHeatPump = boost::optional<CoilWaterHeatingAirToWaterHeatPump>;

  /** \relates CoilWaterHeatingAirToWaterHeatPump*/
  using CoilWaterHeatingAirToWaterHeatPumpVector = std::vector<CoilWaterHeatingAirToWaterHeatPump>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILWATERHEATINGAIRTOWATERHEATPUMP_HPP
