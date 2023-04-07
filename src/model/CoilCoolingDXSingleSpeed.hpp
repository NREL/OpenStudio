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

#ifndef MODEL_COILCOOLINGDXSINGLESPEED_HPP
#define MODEL_COILCOOLINGDXSINGLESPEED_HPP

#include "ModelAPI.hpp"

#include "ModelObject.hpp"
#include "StraightComponent.hpp"
#include "Connection.hpp"
#include "../utilities/core/Deprecated.hpp"

namespace openstudio {
namespace model {

  class Curve;
  class Schedule;

  namespace detail {
    class CoilCoolingDXSingleSpeed_Impl;
  }  // namespace detail

  /** CoilCoolingDXSingleSpeed is an interface to the IDD object named
 *  "OS:Coil:Cooling:DX:SingleSpeed"
 *
 *  The purpose of this class is to simplify the construction and manipulation
 *  OS:Coil:Cooling:DX:SingleSpeed objects in energyplus.
 */
  class MODEL_API CoilCoolingDXSingleSpeed : public StraightComponent
  {
   public:
    /** @name Constructor and Destructors */
    //@{

    /** Constructs a new CoilCoolingDXSingleSpeed object and places it inside the
   *  model.  The object is fully initialized with all companion objects.
   *
   * \param model the Model that this coil will live in
   * \param availabilitySchedule the schedule that goes into field:Availability Schedule Name
   * \param coolingCurveFofTemp curve for field: Total Cooling Capacity Function of Temperature Curve Name
   * \param coolingCurveFofFlow curve for field: Total Cooling Capacity Function of Flow Fraction Curve Name
   * \param energyInputRatioFofTemp curve for field: Energy Input Ratio Function of Temperature Curve Name
   * \param energyInputRatioFofFlow curve for field: Energy Input Ratio Function of Flow Fraction Curve Name
   * \param partLoadFraction curve for field: Part Load Fraction Correlation Curve Name
   */
    explicit CoilCoolingDXSingleSpeed(const Model& model, Schedule& availabilitySchedule, const Curve& coolingCurveFofTemp,
                                      const Curve& coolingCurveFofFlow, const Curve& energyInputRatioFofTemp, const Curve& energyInputRatioFofFlow,
                                      const Curve& partLoadFraction);

    explicit CoilCoolingDXSingleSpeed(const Model& model);

    virtual ~CoilCoolingDXSingleSpeed() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilCoolingDXSingleSpeed(const CoilCoolingDXSingleSpeed& other) = default;
    CoilCoolingDXSingleSpeed(CoilCoolingDXSingleSpeed&& other) = default;
    CoilCoolingDXSingleSpeed& operator=(const CoilCoolingDXSingleSpeed&) = default;
    CoilCoolingDXSingleSpeed& operator=(CoilCoolingDXSingleSpeed&&) = default;

    //@}

    static IddObjectType iddObjectType();

    std::vector<std::string> condenserTypeValues() const;

    /** @name Getters */
    //@{

    /** Returns the Schedule referred to by the AvailabilitySchedule field. **/
    Schedule availabilitySchedule() const;

    /** Returns the value of the RatedCOP field. **/
    double ratedCOP() const;

    /** Returns the value of the RatedEvaporatorFanPowerPerVolumeFlowRate field. **/
    OS_DEPRECATED double ratedEvaporatorFanPowerPerVolumeFlowRate() const;

    /** Returns the value of the RatedEvaporatorFanPowerPerVolumeFlowRate2017 field. **/
    double ratedEvaporatorFanPowerPerVolumeFlowRate2017() const;

    /** Returns the value of the RatedEvaporatorFanPowerPerVolumeFlowRate2023 field. **/
    double ratedEvaporatorFanPowerPerVolumeFlowRate2023() const;

    /** Returns the CurveBiquadratic referred to by the TotalCoolingCapacityFunctionOfTemperatureCurveName field. **/
    Curve totalCoolingCapacityFunctionOfTemperatureCurve() const;

    /** Returns the CurveQuadratic referred to by the TotalCoolingCapacityFunctionOfFlowFractionCurveName field. **/
    Curve totalCoolingCapacityFunctionOfFlowFractionCurve() const;

    /** Returns the CurveBiquadratic referred to by the EnergyInputRatioFunctionOfTemperatureCurveName field. **/
    Curve energyInputRatioFunctionOfTemperatureCurve() const;

    /** Returns the CurveQuadratic referred to by the EnergyInputRatioFunctionOfFlowFractionCurveName field. **/
    Curve energyInputRatioFunctionOfFlowFractionCurve() const;

    /** Returns the CurveQuadratic referred to by the PartLoadFractionCorrelationCurveName field. **/
    Curve partLoadFractionCorrelationCurve() const;

    /** Returns the value of the NominalTimeForCondensateRemovalToBegin field. **/
    double nominalTimeForCondensateRemovalToBegin() const;

    /** Returns the value of the RatioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity field. **/
    double ratioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity() const;

    /** Returns the value of the MaximumCyclingRate field. **/
    double maximumCyclingRate();

    /** Returns the value of the latentCapacityTimeConstant field. **/
    double latentCapacityTimeConstant() const;

    boost::optional<std::string> condenserAirInletNodeName() const;

    /** Returns the value of the CondenserType field. **/
    std::string condenserType() const;

    /** Returns the value of the EvaporativeCondenserEffectiveness field. **/
    double evaporativeCondenserEffectiveness() const;

    /** Returns the value of the EvaporativeCondenserAirFlowRate field.
      optional == false means "autosize"
  **/
    boost::optional<double> evaporativeCondenserAirFlowRate() const;

    /** Returns the value of the EvaporativeCondenserPumpRatedPowerConsumption field.
      optional == false means "autosize"
  **/
    boost::optional<double> evaporativeCondenserPumpRatedPowerConsumption() const;

    /** Returns the value of the crankcaseHeaterCapacity field. **/
    double crankcaseHeaterCapacity() const;

    /** Returns the value of the MaximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation field. **/
    double maximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation() const;

    /** Returns the value of the BasinHeaterCapacity field. **/
    double basinHeaterCapacity() const;

    /** Returns the value of the BasinHeaterSetpointTemperature field. **/
    double basinHeaterSetpointTemperature() const;

    /** Returns the Schedule referred to by the BasinHeaterOperatingSchedule field. **/
    boost::optional<Schedule> basinHeaterOperatingSchedule() const;

    double minimumOutdoorDryBulbTemperatureforCompressorOperation() const;

    //@}
    /** @name Setters */
    //@{

    /** Sets the Schedule referred to by the AvailabilitySchedule field. **/
    bool setAvailabilitySchedule(Schedule& schedule);

    /** Sets the value of the RatedCOP field. **/
    bool setRatedCOP(boost::optional<double> value);

    bool setRatedCOP(double value);

    /** Sets the value of the RatedEvaporatorFanPowerPerVolumeFlowRate field. **/
    OS_DEPRECATED bool setRatedEvaporatorFanPowerPerVolumeFlowRate(boost::optional<double> value);

    /** Sets the value of the RatedEvaporatorFanPowerPerVolumeFlowRate2017 field. **/
    bool setRatedEvaporatorFanPowerPerVolumeFlowRate2017(boost::optional<double> value);

    /** Sets the value of the RatedEvaporatorFanPowerPerVolumeFlowRate2023 field. **/
    bool setRatedEvaporatorFanPowerPerVolumeFlowRate2023(boost::optional<double> value);

    OS_DEPRECATED bool setRatedEvaporatorFanPowerPerVolumeFlowRate(double value);

    bool setRatedEvaporatorFanPowerPerVolumeFlowRate2017(double value);

    bool setRatedEvaporatorFanPowerPerVolumeFlowRate2023(double value);

    /** Sets the CurveBiquadratic referred to by the TotalCoolingCapacityFunctionOfTemperatureCurveName field. **/
    bool setTotalCoolingCapacityFunctionOfTemperatureCurve(const Curve& curve);

    /** Sets the CurveQuadratic referred to by the TotalCoolingCapacityFunctionOfFlowFractionCurnveName field. **/
    bool setTotalCoolingCapacityFunctionOfFlowFractionCurve(const Curve& curve);

    /** Sets the CurveBiquadratic referred to by the EnergyInputRatioFunctionOfTemperatureCurveName field. **/
    bool setEnergyInputRatioFunctionOfTemperatureCurve(const Curve& curve);

    /** Sets the CurveQuadratic referred to by the EnergyInputRatioFunctionOfFlowFractionCurveName field. **/
    bool setEnergyInputRatioFunctionOfFlowFractionCurve(const Curve& curve);

    /** Sets the CurveQuadratic referred to by the PartLoadFractionCorrelationCurveName field. **/
    bool setPartLoadFractionCorrelationCurve(const Curve& curve);

    /** Sets the value of the NominalTimeForCondensateRemovalToBegin field. **/
    bool setNominalTimeForCondensateRemovalToBegin(boost::optional<double> value);

    bool setNominalTimeForCondensateRemovalToBegin(double value);

    /** Sets the value of the RatioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity field. **/
    bool setRatioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity(boost::optional<double> value);

    bool setRatioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity(double value);

    /** Sets the value of the MaximumCyclingRate field. **/
    bool setMaximumCyclingRate(boost::optional<double> value);

    bool setMaximumCyclingRate(double value);

    /** Sets the value of the latentCapacityTimeConstant field. **/
    bool setLatentCapacityTimeConstant(boost::optional<double> value);

    bool setLatentCapacityTimeConstant(double value);

    bool setCondenserAirInletNodeName(const boost::optional<std::string>&);

    /** Sets the value of the CondenserType field.
   *  Options are AirCooled and EvaporativelyCooled.
   */
    bool setCondenserType(const std::string& value);

    /** Sets the value of the EvaporativeCondenserEffectiveness field. **/
    bool setEvaporativeCondenserEffectiveness(boost::optional<double> value);

    bool setEvaporativeCondenserEffectiveness(double value);

    bool isEvaporativeCondenserAirFlowRateAutosized() const;

    /** Sets the value of the EvaporativeCondenserAirFlowRate field.
      if value==false then set field to "autosize"
  **/
    bool setEvaporativeCondenserAirFlowRate(boost::optional<double> value);

    bool setEvaporativeCondenserAirFlowRate(double value);

    void autosizeEvaporativeCondenserAirFlowRate();

    bool isEvaporativeCondenserPumpRatedPowerConsumptionAutosized() const;

    /** Sets the value of the EvaporativeCondenserPumpRatedPowerConsumption field.
      if value==false then set field to "autosize"
  **/
    bool setEvaporativeCondenserPumpRatedPowerConsumption(boost::optional<double> value);

    bool setEvaporativeCondenserPumpRatedPowerConsumption(double value);

    void autosizeEvaporativeCondenserPumpRatedPowerConsumption();

    /** Sets the value of the crankcaseHeaterCapacity field. **/
    bool setCrankcaseHeaterCapacity(boost::optional<double> value);

    bool setCrankcaseHeaterCapacity(double value);

    /** Sets the value of the MaximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation field. **/
    bool setMaximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation(boost::optional<double> value);

    bool setMaximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation(double value);

    //getSupplyWaterStorageTankName
    //setSupplyWaterStorageTankName

    //getCondensateCollectionWaterStorageTankName
    //setCondensateCollectionWaterStorageTankName

    /** Sets the value of the BasinHeaterCapacity field. **/
    bool setBasinHeaterCapacity(boost::optional<double> value);

    bool setBasinHeaterCapacity(double value);

    /** Sets the value of the BasinHeaterSetpointTemperature field. **/
    bool setBasinHeaterSetpointTemperature(boost::optional<double> value);

    bool setBasinHeaterSetpointTemperature(double value);

    /** Sets the Schedule referred to by the BasinHeaterOperatingSchedule field. **/
    bool setBasinHeaterOperatingSchedule(Schedule& schedule);

    void resetBasinHeaterOperatingSchedule();

    bool setMinimumOutdoorDryBulbTemperatureforCompressorOperation(double minimumOutdoorDryBulbTemperatureforCompressorOperation);

    boost::optional<double> ratedTotalCoolingCapacity() const;

    bool isRatedTotalCoolingCapacityAutosized() const;

    bool setRatedTotalCoolingCapacity(double ratedTotalCoolingCapacity);

    void autosizeRatedTotalCoolingCapacity();

    boost::optional<double> ratedSensibleHeatRatio() const;

    bool isRatedSensibleHeatRatioAutosized() const;

    bool setRatedSensibleHeatRatio(double ratedSensibleHeatRatio);

    void autosizeRatedSensibleHeatRatio();

    boost::optional<double> ratedAirFlowRate() const;

    bool isRatedAirFlowRateAutosized() const;

    bool setRatedAirFlowRate(double ratedAirFlowRate);

    void autosizeRatedAirFlowRate();

    /** Returns an equivalent duct object, creating a new one if an object is not already attached. */
    AirflowNetworkEquivalentDuct getAirflowNetworkEquivalentDuct(double length, double diameter);
    /** Returns the attached equivalent duct object if there is one. */
    boost::optional<AirflowNetworkEquivalentDuct> airflowNetworkEquivalentDuct() const;

    // Autosizing methods

    boost::optional<double> autosizedRatedAirFlowRate() const;

    boost::optional<double> autosizedRatedTotalCoolingCapacity() const;

    boost::optional<double> autosizedRatedSensibleHeatRatio() const;

    boost::optional<double> autosizedEvaporativeCondenserAirFlowRate() const;

    boost::optional<double> autosizedEvaporativeCondenserPumpRatedPowerConsumption() const;

    //@}
   protected:
    friend class Model;

    friend class openstudio::IdfObject;

    /// @cond

    using ImplType = detail::CoilCoolingDXSingleSpeed_Impl;

    explicit CoilCoolingDXSingleSpeed(std::shared_ptr<detail::CoilCoolingDXSingleSpeed_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.CoilCoolingDXSingleSpeed");

    /// @endcond
  };

  /** \relates CoilCoolingDXSingleSpeed */
  using OptionalCoilCoolingDXSingleSpeed = boost::optional<CoilCoolingDXSingleSpeed>;

  /** \relates CoilCoolingDXSingleSpeed */
  using CoilCoolingDXSingleSpeedVector = std::vector<CoilCoolingDXSingleSpeed>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGDXSINGLESPEED_HPP
