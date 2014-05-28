/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#ifndef MODEL_COILCOOLINGDXSINGLESPEED_HPP
#define MODEL_COILCOOLINGDXSINGLESPEED_HPP

#include "ModelAPI.hpp"

#include "ModelObject.hpp"
#include "StraightComponent.hpp"
#include "Connection.hpp"

namespace openstudio {
namespace model {

class Curve;
class Schedule;

namespace detail {
class CoilCoolingDXSingleSpeed_Impl;
} // detail

/** CoilCoolingDXSingleSpeed is an interface to the IDD object named
 *  "OS:Coil:Cooling:DX:SingleSpeed"
 *
 *  The purpose of this class is to simplify the construction and manipulation
 *  OS:Coil:Cooling:DX:SingleSpeed objects in energyplus.
 */
class MODEL_API CoilCoolingDXSingleSpeed : public StraightComponent {
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
  explicit CoilCoolingDXSingleSpeed(const Model& model,
                                    Schedule& availabilitySchedule,
                                    const Curve& coolingCurveFofTemp,
                                    const Curve& coolingCurveFofFlow,
                                    const Curve& energyInputRatioFofTemp,
                                    const Curve& energyInputRatioFofFlow,
                                    const Curve& partLoadFraction);

  virtual ~CoilCoolingDXSingleSpeed() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  /** Returns the Schedule referred to by the AvailabilitySchedule field. **/
  Schedule availabilitySchedule() const;

  /** Returns the value of the RatedCOP field. **/
  boost::optional<double> ratedCOP() const;

  /** Returns the value of the RatedEvaporatorFanPowerPerVolumeFlowRate field. **/
  boost::optional<double> ratedEvaporatorFanPowerPerVolumeFlowRate() const;

  /** Returns the CurveBiquadratic referred to by the TotalCoolingCapacityFunctionOfTemperatureCurveName field. **/
  Curve totalCoolingCapacityFunctionOfTemperatureCurve()const;

  /** Returns the CurveQuadratic referred to by the TotalCoolingCapacityFunctionOfFlowFractionCurveName field. **/
  Curve totalCoolingCapacityFunctionOfFlowFractionCurve()const;

  /** Returns the CurveBiquadratic referred to by the EnergyInputRatioFunctionOfTemperatureCurveName field. **/
  Curve energyInputRatioFunctionOfTemperatureCurve()const;

  /** Returns the CurveQuadratic referred to by the EnergyInputRatioFunctionOfFlowFractionCurveName field. **/
  Curve energyInputRatioFunctionOfFlowFractionCurve()const;

  /** Returns the CurveQuadratic referred to by the PartLoadFractionCorrelationCurveName field. **/
  Curve partLoadFractionCorrelationCurve()const;

  /** Returns the value of the NominalTimeForCondensateRemovalToBegin field. **/
  boost::optional<double> nominalTimeForCondensateRemovalToBegin() const;

  /** Returns the value of the RatioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity field. **/
  boost::optional<double> ratioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity()const;

  /** Returns the value of the MaximumCyclingRate field. **/
  boost::optional<double> maximumCyclingRate();

  /** Returns the value of the latentCapacityTimeConstant field. **/
  boost::optional<double> latentCapacityTimeConstant()const;

  OptionalString condenserAirInletNodeName()const;

  /** Returns the value of the CondenserType field. **/
  std::string condenserType()const;

  /** Returns the value of the EvaporativeCondenserEffectiveness field. **/
  boost::optional<double> evaporativeCondenserEffectiveness()const;

  /** Returns the value of the EvaporativeCondenserAirFlowRate field.
      optional == false means "autosize"
  **/
  boost::optional<double> evaporativeCondenserAirFlowRate()const;

  /** Returns the value of the EvaporativeCondenserPumpRatedPowerConsumption field.
      optional == false means "autosize"
  **/
  boost::optional<double> evaporativeCondenserPumpRatedPowerConsumption()const;

  /** Returns the value of the crankcaseHeaterCapacity field. **/
  boost::optional<double> crankcaseHeaterCapacity()const;

  /** Returns the value of the MaximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation field. **/
  boost::optional<double> maximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation()const;

  /** Returns the value of the BasinHeaterCapacity field. **/
  boost::optional<double> basinHeaterCapacity()const;

  /** Returns the value of the BasinHeaterSetpointTemperature field. **/
  boost::optional<double> basinHeaterSetpointTemperature() const;

  /** Returns the Schedule referred to by the BasinHeaterOperatingSchedule field. **/
  boost::optional<Schedule> basinHeaterOperatingSchedule() const;

  /** \deprecated */
  Schedule getAvailabilitySchedule()const;

  /** \deprecated */
  boost::optional<double> getRatedCOP() const;

  /** \deprecated */
  boost::optional<double> getRatedEvaporatorFanPowerPerVolumeFlowRate()const ;

  /** \deprecated */
  Curve getTotalCoolingCapacityFunctionOfTemperatureCurve()const;

  /** \deprecated */
  Curve getTotalCoolingCapacityFunctionOfFlowFractionCurve()const;

  /** \deprecated */
  Curve getEnergyInputRatioFunctionOfTemperatureCurve() const;

  /** \deprecated */
  Curve getEnergyInputRatioFunctionOfFlowFractionCurve() const;

  /** \deprecated */
  Curve getPartLoadFractionCorrelationCurve() const;

  /** \deprecated */
  boost::optional<double> getNominalTimeForCondensateRemovalToBegin() const;

  /** \deprecated */
  boost::optional<double> getRatioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity() const;

  /** \deprecated */
  boost::optional<double> getMaximumCyclingRate();

  /** \deprecated */
  boost::optional<double> getLatentCapacityTimeConstant()const;

  /** \deprecated */
  OptionalString getCondenserAirInletNodeName()const;

  /** \deprecated */
  std::string getCondenserType()const;

  /** \deprecated */
  boost::optional<double> getEvaporativeCondenserEffectiveness()const;

  /** \deprecated */
  boost::optional<double> getEvaporativeCondenserAirFlowRate()const;

  /** \deprecated */
  boost::optional<double> getEvaporativeCondenserPumpRatedPowerConsumption()const;

  /** \deprecated */
  boost::optional<double> getCrankcaseHeaterCapacity() const;

  /** \deprecated */
  boost::optional<double> getMaximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation()const;

  /** \deprecated */
  boost::optional<double> getBasinHeaterCapacity() const;

  /** \deprecated */
  boost::optional<double> getBasinHeaterSetpointTemperature() const;

  /** \deprecated */
  boost::optional<Schedule> getBasinHeaterOperatingSchedule() const;

  //@}
  /** @name Setters */
  //@{

  /** Sets the Schedule referred to by the AvailabilitySchedule field. **/
  bool setAvailabilitySchedule(Schedule& schedule );

  /** Sets the value of the RatedCOP field. **/
  void setRatedCOP( boost::optional<double> value );

  /** Sets the value of the RatedEvaporatorFanPowerPerVolumeFlowRate field. **/
  void setRatedEvaporatorFanPowerPerVolumeFlowRate( boost::optional<double> value );

  /** Sets the CurveBiquadratic referred to by the TotalCoolingCapacityFunctionOfTemperatureCurveName field. **/
  void setTotalCoolingCapacityFunctionOfTemperatureCurve( const Curve& curve );

  /** Sets the CurveQuadratic referred to by the TotalCoolingCapacityFunctionOfFlowFractionCurnveName field. **/
  void setTotalCoolingCapacityFunctionOfFlowFractionCurve( const Curve& curve );

  /** Sets the CurveBiquadratic referred to by the EnergyInputRatioFunctionOfTemperatureCurveName field. **/
  void setEnergyInputRatioFunctionOfTemperatureCurve( const Curve& curve );

  /** Sets the CurveQuadratic referred to by the EnergyInputRatioFunctionOfFlowFractionCurveName field. **/
  void setEnergyInputRatioFunctionOfFlowFractionCurve( const Curve& curve );  

  /** Sets the CurveQuadratic referred to by the PartLoadFractionCorrelationCurveName field. **/
  void setPartLoadFractionCorrelationCurve( const Curve & curve );

  /** Sets the value of the NominalTimeForCondensateRemovalToBegin field. **/
  void setNominalTimeForCondensateRemovalToBegin( boost::optional<double> value );

  /** Sets the value of the RatioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity field. **/
  void setRatioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity( boost::optional<double> value );

  /** Sets the value of the MaximumCyclingRate field. **/
  void setMaximumCyclingRate( boost::optional<double> value );  

  /** Sets the value of the latentCapacityTimeConstant field. **/
  void setLatentCapacityTimeConstant( boost::optional<double> value );

  void setCondenserAirInletNodeName(const boost::optional<std::string>&);

  /** Sets the value of the CondenserType field.
   *  Options are AirCooled and EvaporativelyCooled.
   */
  void setCondenserType( const std::string& value );

  /** Sets the value of the EvaporativeCondenserEffectiveness field. **/
  void setEvaporativeCondenserEffectiveness( boost::optional<double> value );

  /** Sets the value of the EvaporativeCondenserAirFlowRate field.
      if value==false then set field to "autosize"
  **/
  void setEvaporativeCondenserAirFlowRate( boost::optional<double> value );

  /** Sets the value of the EvaporativeCondenserPumpRatedPowerConsumption field.
      if value==false then set field to "autosize"
  **/
  void setEvaporativeCondenserPumpRatedPowerConsumption( boost::optional<double> value );

  /** Sets the value of the crankcaseHeaterCapacity field. **/
  void setCrankcaseHeaterCapacity( boost::optional<double> value );  

  /** Sets the value of the MaximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation field. **/
  void setMaximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation( boost::optional<double> value );

  //getSupplyWaterStorageTankName
  //setSupplyWaterStorageTankName

  //getCondensateCollectionWaterStorageTankName
  //setCondensateCollectionWaterStorageTankName

  /** Sets the value of the BasinHeaterCapacity field. **/
  void setBasinHeaterCapacity( boost::optional<double> value );

  /** Sets the value of the BasinHeaterSetpointTemperature field. **/
  void setBasinHeaterSetpointTemperature( boost::optional<double> value );

  /** Sets the Schedule referred to by the BasinHeaterOperatingSchedule field. **/
  bool setBasinHeaterOperatingSchedule(Schedule& schedule );

  void resetBasinHeaterOperatingSchedule();


  boost::optional<double> ratedTotalCoolingCapacity() const;

  OSOptionalQuantity getRatedTotalCoolingCapacity(bool returnIP=false) const;

  bool isRatedTotalCoolingCapacityAutosized() const;

  bool setRatedTotalCoolingCapacity(double ratedTotalCoolingCapacity);

  bool setRatedTotalCoolingCapacity(const Quantity& ratedTotalCoolingCapacity);

  void autosizeRatedTotalCoolingCapacity();


  boost::optional<double> ratedSensibleHeatRatio() const;

  OSOptionalQuantity getRatedSensibleHeatRatio(bool returnIP=false) const;

  bool isRatedSensibleHeatRatioAutosized() const;

  bool setRatedSensibleHeatRatio(double ratedSensibleHeatRatio);

  bool setRatedSensibleHeatRatio(const Quantity& ratedSensibleHeatRatio);

  void autosizeRatedSensibleHeatRatio();


  boost::optional<double> ratedAirFlowRate() const;

  OSOptionalQuantity getRatedAirFlowRate(bool returnIP=false) const;

  bool isRatedAirFlowRateAutosized() const;

  bool setRatedAirFlowRate(double ratedAirFlowRate);

  bool setRatedAirFlowRate(const Quantity& ratedAirFlowRate);

  void autosizeRatedAirFlowRate();

  //@}
 protected:

  friend class Model;

  friend class openstudio::IdfObject;

  /// @cond

  typedef detail::CoilCoolingDXSingleSpeed_Impl ImplType;

  explicit CoilCoolingDXSingleSpeed(std::shared_ptr<detail::CoilCoolingDXSingleSpeed_Impl> impl);

  private:

  REGISTER_LOGGER("openstudio.model.CoilCoolingDXSingleSpeed");

  /// @endcond

};

/** \relates CoilCoolingDXSingleSpeed */
typedef boost::optional<CoilCoolingDXSingleSpeed> OptionalCoilCoolingDXSingleSpeed;

/** \relates CoilCoolingDXSingleSpeed */
typedef std::vector<CoilCoolingDXSingleSpeed> CoilCoolingDXSingleSpeedVector;

} // model
} // openstudio

#endif // MODEL_COILCOOLINGDXSINGLESPEED_HPP
