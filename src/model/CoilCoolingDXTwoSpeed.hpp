/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef MODEL_COILCOOLINGDXTWOSPEED_HPP
#define MODEL_COILCOOLINGDXTWOSPEED_HPP

#include "ModelAPI.hpp"

#include "ModelObject.hpp"
#include "StraightComponent.hpp"
#include "Connection.hpp"

namespace openstudio {
namespace model {

class Curve;
class Schedule;

namespace detail {
class CoilCoolingDXTwoSpeed_Impl;
} // detail

/** CoilCoolingDXTwoSpeed is an interface to the IDD object named
 *  "OS:Coil:Cooling:DX:SingleSpeed"
 *
 *  The purpose of this class is to simplify the construction and manipulation
 *  OS:Coil:Cooling:DX:SingleSpeed objects in energyplus.
 */
class MODEL_API CoilCoolingDXTwoSpeed : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Constructs a new CoilCoolingDXTwoSpeed object and places it inside the
   *  model.  The object is fully initialized with all companion objects.
   *
   * \param model the Model that this coil will live in
   * \param availabilitySchedule the schedule that goes into field:Availability Schedule Name
   * \param coolingCurveFofTemp curve for field: Total Cooling Capacity Function of Temperature Curve Name
   * \param coolingCurveFofFlow curve for field: Total Cooling Capacity Function of Flow Fraction Curve Name
   * \param energyInputRatioFofTemp curve for field: Energy Input Ratio Function of Temperature Curve Name
   * \param energyInputRatioFofFlow curve for field: Energy Input Ratio Function of Flow Fraction Curve Name
   * \param partLoadFraction curve for field: Part Load Fraction Correlation Curve Name
   * \param lowSpeedCoolingCurveFofTemp curve for field: Low Speed Total Cooling Capacity Function of Temperature Curve Name
   * \param lowSpeedEnergyInputRatioFofTemp curve for field: Low Speed Energy Input Ratio Function of Temperature Curve Name
   */
  CoilCoolingDXTwoSpeed(const Model& model,
                        Schedule& availabilitySchedule,
                        const Curve& coolingCurveFofTemp,
                        const Curve& coolingCurveFofFlow,
                        const Curve& energyInputRatioFofTemp,
                        const Curve& energyInputRatioFofFlow,
                        const Curve& partLoadFraction,
                        const Curve& lowSpeedCoolingCurveFofTemp,
                        const Curve& lowSpeedEnergyInputRatioFofTemp);

  CoilCoolingDXTwoSpeed(const Model& model);

  virtual ~CoilCoolingDXTwoSpeed() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  /** Returns the Schedule referred to by the AvailabilitySchedule field. **/
  Schedule availabilitySchedule() const;

  /** \deprecated */
  Schedule getAvailabilitySchedule() const;

  /** Returns the value of the RatedHighSpeedTotalCoolingCapacity field.
   *  Optional == false means "Autocalculate" */
  boost::optional<double> ratedHighSpeedTotalCoolingCapacity() const;

  /** \deprecated */
  boost::optional<double> getRatedHighSpeedTotalCoolingCapacity() const;

  /** Returns the value of the RatedHighSpeedSensibleHeatRatio field.
   *   Optional == false means "Autocalculate" */
  boost::optional<double> ratedHighSpeedSensibleHeatRatio() const;

  /** \deprecated */
  boost::optional<double> getRatedHighSpeedSensibleHeatRatio() const;

  /** Returns the value of the RatedHighSpeedCOP field. **/
  boost::optional<double> ratedHighSpeedCOP() const;

  /** \deprecated **/
  boost::optional<double> getRatedHighSpeedCOP() const;

  /** Returns the value of the RatedHighSpeedAirFlowRate field.
   *  Optional == false means "Autocalculate" */
  boost::optional<double> ratedHighSpeedAirFlowRate() const;

  /** \deprecated */
  boost::optional<double> getRatedHighSpeedAirFlowRate() const;

  /** Returns the Curve referred to by the
   *  TotalCoolingCapacityFunctionOfTemperatureCurveName field. */
  Curve totalCoolingCapacityFunctionOfTemperatureCurve()const;

  /** \deprecated */
  Curve getTotalCoolingCapacityFunctionOfTemperatureCurve()const;

  /** Returns the Curve referred to by the
   *  TotalCoolingCapacityFunctionOfFlowFractionCurveName field. */
  Curve totalCoolingCapacityFunctionOfFlowFractionCurve()const;

  /** \deprecated */
  Curve getTotalCoolingCapacityFunctionOfFlowFractionCurve() const;

  /** Returns the Curve referred to by the
   *  EnergyInputRatioFunctionOfTemperatureCurveName field. */
  Curve energyInputRatioFunctionOfTemperatureCurve() const;

  /** \deprecated */
  Curve getEnergyInputRatioFunctionOfTemperatureCurve() const;

  /** Returns the Curve referred to by the
   *  EnergyInputRatioFunctionOfFlowFractionCurveName field. */
  Curve energyInputRatioFunctionOfFlowFractionCurve() const;

  /** \deprecated */
  Curve getEnergyInputRatioFunctionOfFlowFractionCurve() const;

  /** Returns the Curve referred to by the PartLoadFractionCorrelationCurveName
   *  field. **/
  Curve partLoadFractionCorrelationCurve() const;

  /** \deprecated */
  Curve getPartLoadFractionCorrelationCurve() const;

  /** Returns the value of the RatedLowSpeedTotalCoolingCapacity field.
   *  Optional == false means "Autocalculate" */
  boost::optional<double> ratedLowSpeedTotalCoolingCapacity() const;

  /** \deprecated */
  boost::optional<double> getRatedLowSpeedTotalCoolingCapacity() const;

  /** Returns the value of the RatedLowSpeedSensibleHeatRatio field.
   *  Optional == false means "Autocalculate" */
  boost::optional<double> ratedLowSpeedSensibleHeatRatio() const;

  /** \deprecated */
  boost::optional<double> getRatedLowSpeedSensibleHeatRatio() const;

  /** Returns the value of the RatedLowSpeedCOP field. */
  boost::optional<double> ratedLowSpeedCOP() const;

  /** \deprecated */
  boost::optional<double> getRatedLowSpeedCOP() const;

  /** Returns the value of the RatedLowSpeedAirFlowRate field.
   *  Optional == false means "Autocalculate" */
  boost::optional<double> ratedLowSpeedAirFlowRate() const;

  /** \deprecated */
  boost::optional<double> getRatedLowSpeedAirFlowRate() const;

  /** Returns the Curve referred to by the
   *  TotalCoolingCapacityFunctionOfTemperatureCurveName field. */
  Curve lowSpeedTotalCoolingCapacityFunctionOfTemperatureCurve()const;

  /** \deprecated */
  Curve getLowSpeedTotalCoolingCapacityFunctionOfTemperatureCurve()const;

  /** Returns the Curve referred to by the
   *  LowSpeedEnergyInputRatioFunctionOfTemperatureCurveName field. */
  Curve lowSpeedEnergyInputRatioFunctionOfTemperatureCurve() const;

  /** \deprecated */
  Curve getLowSpeedEnergyInputRatioFunctionOfTemperatureCurve() const;

  boost::optional<std::string> condenserAirInletNodeName() const;

  /** \deprecated */
  boost::optional<std::string> getCondenserAirInletNodeName() const;

  /** Returns the value of the CondenserType field. */
  std::string condenserType()const;

  /** \deprecated */
  std::string getCondenserType()const;

  /** Returns the value of the HighSpeedEvaporativeCondenserEffectiveness field. */
  boost::optional<double> highSpeedEvaporativeCondenserEffectiveness() const;

  /** \deprecated */
  boost::optional<double> getHighSpeedEvaporativeCondenserEffectiveness() const;

  /** Returns the value of the HighSpeedEvaporativeCondenserAirFlowRate field.
   *  optional == flase means "autosize" */
  boost::optional<double> highSpeedEvaporativeCondenserAirFlowRate() const;

  /** \deprecated */
  boost::optional<double> getHighSpeedEvaporativeCondenserAirFlowRate() const;

  /** Returns the value of the HighSpeedEvaporativeCondenserPumpRatedPowerConsumption field.
   *  optional == flase means "autosize" */
  boost::optional<double> highSpeedEvaporativeCondenserPumpRatedPowerConsumption()const;

  /** \deprecated */
  boost::optional<double> getHighSpeedEvaporativeCondenserPumpRatedPowerConsumption()const;

  /** Returns the value of the LowSpeedEvaporativeCondenserEffectiveness field. */
  boost::optional<double> lowSpeedEvaporativeCondenserEffectiveness() const;

  /** \deprecated */
  boost::optional<double> getLowSpeedEvaporativeCondenserEffectiveness() const;

  /** Returns the value of the LowSpeedEvaporativeCondenserAirFlowRate field.
   *  optional == flase means "autosize" */
  boost::optional<double> lowSpeedEvaporativeCondenserAirFlowRate() const;

  /** \deprecated */
  boost::optional<double> getLowSpeedEvaporativeCondenserAirFlowRate() const;

  /** Returns the value of the LowSpeedEvaporativeCondenserPumpRatedPowerConsumption field.
   *  optional == flase means "autosize" */
  boost::optional<double> lowSpeedEvaporativeCondenserPumpRatedPowerConsumption()const;

  /** \deprecated */
  boost::optional<double> getLowSpeedEvaporativeCondenserPumpRatedPowerConsumption()const;

  /** Returns the value of the BasinHeaterCapacity field. **/
  boost::optional<double> basinHeaterCapacity() const;

  /** \deprecated **/
  boost::optional<double> getBasinHeaterCapacity() const;

  /** Returns the value of the BasinHeaterSetpointTemperature field. **/
  boost::optional<double> basinHeaterSetpointTemperature() const;

  /** \deprecated **/
  boost::optional<double> getBasinHeaterSetpointTemperature() const;

  /** Returns the Schedule referred to by the BasinHeaterOperatingSchedule field. **/
  boost::optional<Schedule> basinHeaterOperatingSchedule() const;

  /** \deprecated */
  boost::optional<Schedule> getBasinHeaterOperatingSchedule() const;

  //@}
  /** @name Setters */
  //@{

  /** Sets the Schedule referred to by the AvailabilitySchedule field. **/
  bool setAvailabilitySchedule(Schedule& schedule);

  /** Sets the value of the RatedHighSpeedTotalCoolingCapacity field.
      If optional is false, field is set to autocalculate
  **/
  bool setRatedHighSpeedTotalCoolingCapacity( boost::optional<double> value );

  bool setRatedHighSpeedTotalCoolingCapacity( double value );

  /** Sets the value of the RatedHighSpeedSensibleHeatRatio field.
   *   If optional is false, field is set to autocalculate */
  bool setRatedHighSpeedSensibleHeatRatio( boost::optional<double> value );

  bool setRatedHighSpeedSensibleHeatRatio( double value );

  /** Sets the value of the RatedHighSpeedCOP field. **/
  bool setRatedHighSpeedCOP( double value );

  /** Sets the value of the RatedHighSpeedAirFlowRate field.
   *  If optional is false, field is set to autocalculate */
  bool setRatedHighSpeedAirFlowRate( boost::optional<double> value );

  bool setRatedHighSpeedAirFlowRate( double value );

  /** Sets the Curve referred to by the
   *  TotalCoolingCapacityFunctionOfTemperatureCurveName field. **/
  bool setTotalCoolingCapacityFunctionOfTemperatureCurve( const Curve& curve );

  /** Sets the Curve referred to by the TotalCoolingCapacityFunctionOfFlowFractionCurnveName field. **/
  bool setTotalCoolingCapacityFunctionOfFlowFractionCurve( const Curve& curve );

  /** Sets the Curve referred to by the EnergyInputRatioFunctionOfTemperatureCurveName field. **/
  bool setEnergyInputRatioFunctionOfTemperatureCurve( const Curve& curve );

  /** Sets the Curve referred to by the EnergyInputRatioFunctionOfFlowFractionCurveName field. **/
  bool setEnergyInputRatioFunctionOfFlowFractionCurve( const Curve& curve );

  /** Sets the Curve referred to by the PartLoadFractionCorrelationCurveName field. **/
  bool setPartLoadFractionCorrelationCurve( const Curve & curve );

  /** Sets the value of the RatedLowSpeedTotalCoolingCapacity field.
   *  If optional is false, field is set to autocalculate */
  bool setRatedLowSpeedTotalCoolingCapacity( boost::optional<double> value );

  bool setRatedLowSpeedTotalCoolingCapacity( double value );

  /** Sets the value of the RatedLowSpeedSensibleHeatRatio field.
      If optional is false, field is set to autocalculate
  **/
  bool setRatedLowSpeedSensibleHeatRatio( boost::optional<double> value );

  bool setRatedLowSpeedSensibleHeatRatio( double value );

  /** Sets the value of the RatedLowSpeedCOP field. **/
  bool setRatedLowSpeedCOP( double value );

  /** Sets the value of the RatedLowSpeedAirFlowRate field.
   *  If optional is false, field is set to autocalculate */
  bool setRatedLowSpeedAirFlowRate( boost::optional<double> value );

  bool setRatedLowSpeedAirFlowRate( double value );

  /** Sets the Curve referred to by the LowSpeedTotalCoolingCapacityFunctionOfTemperatureCurveName field. */
  bool setLowSpeedTotalCoolingCapacityFunctionOfTemperatureCurve( const Curve& curve );

  /** Sets the Curve referred to by the LowSpeedEnergyInputRatioFunctionOfTemperatureCurveName field. */
  bool setLowSpeedEnergyInputRatioFunctionOfTemperatureCurve( const Curve& curve );

  bool setCondenserAirInletNodeName(const std::string&);

  /** Sets the value of the CondenserType field.
   *  Options are AirCooled and EvaporativelyCooled. */
  bool setCondenserType( const std::string& value );

  /** Sets the value of the HighSpeedEvaporativeCondenserEffectiveness field. */
  bool setHighSpeedEvaporativeCondenserEffectiveness( double value );

  /** Sets the value of the HighSpeedEvaporativeCondenserAirFlowRate field.
   *  if value==false then set field to "autosize" */
  bool setHighSpeedEvaporativeCondenserAirFlowRate( boost::optional<double> value );

  bool setHighSpeedEvaporativeCondenserAirFlowRate( double value );

  /** Sets the value of the HighSpeedEvaporativeCondenserPumpRatedPowerConsumption field.
   *  if value==false then set field to "autosize" */
  bool setHighSpeedEvaporativeCondenserPumpRatedPowerConsumption( boost::optional<double> value );

  bool setHighSpeedEvaporativeCondenserPumpRatedPowerConsumption( double value );

  /** Sets the value of the LowSpeedEvaporativeCondenserEffectiveness field. */
  bool setLowSpeedEvaporativeCondenserEffectiveness( double value );

  /** Sets the value of the LowSpeedEvaporativeCondenserAirFlowRate field.
   *  if value==false then set field to "autosize" */
  bool setLowSpeedEvaporativeCondenserAirFlowRate( boost::optional<double> value );

  bool setLowSpeedEvaporativeCondenserAirFlowRate( double value );

  /** Sets the value of the LowSpeedEvaporativeCondenserPumpRatedPowerConsumption field.
   *  if value==false then set field to "autosize" */
  bool setLowSpeedEvaporativeCondenserPumpRatedPowerConsumption( boost::optional<double> value );

  bool setLowSpeedEvaporativeCondenserPumpRatedPowerConsumption( double value );

  //supplyWaterStorageTankName
  //setSupplyWaterStorageTankName

  //condensateCollectionWaterStorageTankName
  //setCondensateCollectionWaterStorageTankName

  /** Sets the value of the BasinHeaterCapacity field. **/
  bool setBasinHeaterCapacity( double value );

  /** Sets the value of the BasinHeaterSetpointTemperature field. **/
  bool setBasinHeaterSetpointTemperature( double value );

  /** Sets the Schedule referred to by the BasinHeaterOperatingSchedule field. **/
  bool setBasinHeaterOperatingSchedule(Schedule& schedule);

  void resetBasinHeaterOperatingSchedule();

  boost::optional<double> autosizedRatedHighSpeedTotalCoolingCapacity() const ;

  boost::optional<double> autosizedRatedHighSpeedSensibleHeatRatio() const ;

  boost::optional<double> autosizedRatedHighSpeedAirFlowRate() const ;

  boost::optional<double> autosizedRatedLowSpeedTotalCoolingCapacity() const ;

  boost::optional<double> autosizedRatedLowSpeedSensibleHeatRatio() const ;

  boost::optional<double> autosizedRatedLowSpeedAirFlowRate() const ;

  boost::optional<double> autosizedHighSpeedEvaporativeCondenserAirFlowRate() const ;

  boost::optional<double> autosizedHighSpeedEvaporativeCondenserPumpRatedPowerConsumption() const ;

  boost::optional<double> autosizedLowSpeedEvaporativeCondenserAirFlowRate() const ;

  boost::optional<double> autosizedLowSpeedEvaporativeCondenserPumpRatedPowerConsumption() const ;



  //@}
 protected:

  friend class Model;
  friend class openstudio::IdfObject;

  /// @cond
  typedef detail::CoilCoolingDXTwoSpeed_Impl ImplType;

  explicit CoilCoolingDXTwoSpeed(std::shared_ptr<detail::CoilCoolingDXTwoSpeed_Impl> impl);

 private:
  REGISTER_LOGGER("openstudio.model.CoilCoolingDXTwoSpeed");
  /// @endcond
};

/** \relates CoilCoolingDXTwoSpeed */
typedef boost::optional<CoilCoolingDXTwoSpeed> OptionalCoilCoolingDXTwoSpeed;

/** \relates CoilCoolingDXTwoSpeed */
typedef std::vector<CoilCoolingDXTwoSpeed> CoilCoolingDXTwoSpeedVector;

} // model
} // openstudio

#endif // MODEL_COILCOOLINGDXTWOSPEED_HPP
