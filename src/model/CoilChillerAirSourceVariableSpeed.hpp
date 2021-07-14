/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef MODEL_COILCHILLERAIRSOURCEVARIABLESPEED_HPP
#define MODEL_COILCHILLERAIRSOURCEVARIABLESPEED_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {
namespace model {

  class Curve;
  class Schedule;
  class CoilChillerAirSourceVariableSpeedSpeedData;

  namespace detail {

    class CoilChillerAirSourceVariableSpeed_Impl;

  }  // namespace detail

  /** CoilChillerAirSourceVariableSpeed is a StraightComponent that wraps the OpenStudio IDD object 'OS:Coil:Chiller:AirSource:VariableSpeed'. */
  class MODEL_API CoilChillerAirSourceVariableSpeed : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilChillerAirSourceVariableSpeed(const Model& model);

    virtual ~CoilChillerAirSourceVariableSpeed() {}

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    int nominalSpeedLevel() const;

    boost::optional<double> ratedChilledWaterCapacity() const;

    bool isRatedChilledWaterCapacityAutosized() const;

    double ratedEvaporatorInletWaterTemperature() const;

    double ratedCondenserInletAirTemperature() const;

    boost::optional<double> ratedEvaporatorWaterFlowRate() const;

    bool isRatedEvaporatorWaterFlowRateAutocalculated() const;

    std::string evaporatorPumpPowerIncludedinRatedCOP() const;

    std::string evaporatorPumpHeatIncludedinRatedCoolingCapacityandRatedCOP() const;

    double fractionofEvaporatorPumpHeattoWater() const;

    double crankcaseHeaterCapacity() const;

    double maximumAmbientTemperatureforCrankcaseHeaterOperation() const;

    boost::optional<Curve> partLoadFractionCorrelationCurve() const;

    boost::optional<Schedule> gridSignalSchedule() const;

    double lowerBoundToApplyGridResponsiveControl() const;

    bool isLowerBoundToApplyGridResponsiveControlDefaulted() const;

    double upperBoundToApplyGridResponsiveControl() const;

    bool isUpperBoundToApplyGridResponsiveControlDefaulted() const;

    int maxSpeedLevelDuringGridResponsiveControl() const;

    bool isMaxSpeedLevelDuringGridResponsiveControlDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setNominalSpeedLevel(int nominalSpeedLevel);

    bool setRatedChilledWaterCapacity(double ratedChilledWaterCapacity);

    void autosizeRatedChilledWaterCapacity();

    bool setRatedEvaporatorInletWaterTemperature(double ratedEvaporatorInletWaterTemperature);

    bool setRatedCondenserInletAirTemperature(double ratedCondenserInletAirTemperature);

    bool setRatedEvaporatorWaterFlowRate(double ratedEvaporatorWaterFlowRate);

    void autocalculateRatedEvaporatorWaterFlowRate();

    bool setEvaporatorPumpPowerIncludedinRatedCOP(std::string evaporatorPumpPowerIncludedinRatedCOP);

    bool setEvaporatorPumpHeatIncludedinRatedCoolingCapacityandRatedCOP(std::string evaporatorPumpHeatIncludedinRatedCoolingCapacityandRatedCOP);

    bool setFractionofEvaporatorPumpHeattoWater(double fractionofEvaporatorPumpHeattoWater);

    bool setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity);

    bool setMaximumAmbientTemperatureforCrankcaseHeaterOperation(double maximumAmbientTemperatureforCrankcaseHeaterOperation);

    bool setPartLoadFractionCorrelationCurve(const Curve& partLoadFractionCorrelationCurve);

    void resetPartLoadFractionCorrelationCurve();

    bool setGridSignalSchedule(Schedule& schedule);

    void resetGridSignalSchedule();

    bool setLowerBoundToApplyGridResponsiveControl(double lowerBoundToApplyGridResponsiveControl);

    void resetLowerBoundToApplyGridResponsiveControl();

    bool setUpperBoundToApplyGridResponsiveControl(double upperBoundToApplyGridResponsiveControl);

    void resetUpperBoundToApplyGridResponsiveControl();

    bool setMaxSpeedLevelDuringGridResponsiveControl(int maxSpeedLevelDuringGridResponsiveControl);

    void resetMaxSpeedLevelDuringGridResponsiveControl();

    //@}
    /** @name Other */
    //@{

    std::vector<CoilChillerAirSourceVariableSpeedSpeedData> speeds() const;

    bool addSpeed(const CoilChillerAirSourceVariableSpeedSpeedData& speed);

    void removeSpeed(const CoilChillerAirSourceVariableSpeedSpeedData& speed);

    void removeAllSpeeds();

    boost::optional<double> autosizedRatedChilledWaterCapacity() const;

    boost::optional<double> autocalculatedRatedEvaporatorWaterFlowRate() const;

    //@}
   protected:
    /// @cond
    typedef detail::CoilChillerAirSourceVariableSpeed_Impl ImplType;

    explicit CoilChillerAirSourceVariableSpeed(std::shared_ptr<detail::CoilChillerAirSourceVariableSpeed_Impl> impl);

    friend class detail::CoilChillerAirSourceVariableSpeed_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilChillerAirSourceVariableSpeed");
  };

  /** \relates CoilChillerAirSourceVariableSpeed*/
  typedef boost::optional<CoilChillerAirSourceVariableSpeed> OptionalCoilChillerAirSourceVariableSpeed;

  /** \relates CoilChillerAirSourceVariableSpeed*/
  typedef std::vector<CoilChillerAirSourceVariableSpeed> CoilChillerAirSourceVariableSpeedVector;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCHILLERAIRSOURCEVARIABLESPEED_HPP
