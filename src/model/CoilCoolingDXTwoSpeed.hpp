/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
  }  // namespace detail

  /** CoilCoolingDXTwoSpeed is an interface to the IDD object named
 *  "OS:Coil:Cooling:DX:SingleSpeed"
 *
 *  The purpose of this class is to simplify the construction and manipulation
 *  OS:Coil:Cooling:DX:SingleSpeed objects in energyplus.
 */
  class MODEL_API CoilCoolingDXTwoSpeed : public StraightComponent
  {
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
    CoilCoolingDXTwoSpeed(const Model& model, Schedule& availabilitySchedule, const Curve& coolingCurveFofTemp, const Curve& coolingCurveFofFlow,
                          const Curve& energyInputRatioFofTemp, const Curve& energyInputRatioFofFlow, const Curve& partLoadFraction,
                          const Curve& lowSpeedCoolingCurveFofTemp, const Curve& lowSpeedEnergyInputRatioFofTemp);

    CoilCoolingDXTwoSpeed(const Model& model);

    virtual ~CoilCoolingDXTwoSpeed() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilCoolingDXTwoSpeed(const CoilCoolingDXTwoSpeed& other) = default;
    CoilCoolingDXTwoSpeed(CoilCoolingDXTwoSpeed&& other) = default;
    CoilCoolingDXTwoSpeed& operator=(const CoilCoolingDXTwoSpeed&) = default;
    CoilCoolingDXTwoSpeed& operator=(CoilCoolingDXTwoSpeed&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    /** Returns the Schedule referred to by the AvailabilitySchedule field. **/
    Schedule availabilitySchedule() const;

    /** Returns the value of the RatedHighSpeedTotalCoolingCapacity field.
   *  Optional == false means "Autocalculate" */
    boost::optional<double> ratedHighSpeedTotalCoolingCapacity() const;

    /** Returns the value of the RatedHighSpeedSensibleHeatRatio field.
   *   Optional == false means "Autocalculate" */
    boost::optional<double> ratedHighSpeedSensibleHeatRatio() const;

    /** Returns the value of the RatedHighSpeedCOP field. **/
    double ratedHighSpeedCOP() const;

    /** Returns the value of the RatedHighSpeedAirFlowRate field.
   *  Optional == false means "Autocalculate" */
    boost::optional<double> ratedHighSpeedAirFlowRate() const;

    double ratedHighSpeedEvaporatorFanPowerPerVolumeFlowRate2017() const;

    double ratedHighSpeedEvaporatorFanPowerPerVolumeFlowRate2023() const;

    /** Returns the Curve referred to by the
   *  TotalCoolingCapacityFunctionOfTemperatureCurveName field. */
    Curve totalCoolingCapacityFunctionOfTemperatureCurve() const;

    /** Returns the Curve referred to by the
   *  TotalCoolingCapacityFunctionOfFlowFractionCurveName field. */
    Curve totalCoolingCapacityFunctionOfFlowFractionCurve() const;

    /** Returns the Curve referred to by the
   *  EnergyInputRatioFunctionOfTemperatureCurveName field. */
    Curve energyInputRatioFunctionOfTemperatureCurve() const;

    /** Returns the Curve referred to by the
   *  EnergyInputRatioFunctionOfFlowFractionCurveName field. */
    Curve energyInputRatioFunctionOfFlowFractionCurve() const;

    /** Returns the Curve referred to by the PartLoadFractionCorrelationCurveName
   *  field. **/
    Curve partLoadFractionCorrelationCurve() const;

    /** Returns the value of the RatedLowSpeedTotalCoolingCapacity field.
   *  Optional == false means "Autocalculate" */
    boost::optional<double> ratedLowSpeedTotalCoolingCapacity() const;

    /** Returns the value of the RatedLowSpeedSensibleHeatRatio field.
   *  Optional == false means "Autocalculate" */
    boost::optional<double> ratedLowSpeedSensibleHeatRatio() const;

    /** Returns the value of the RatedLowSpeedCOP field. */
    double ratedLowSpeedCOP() const;

    /** Returns the value of the RatedLowSpeedAirFlowRate field.
   *  Optional == false means "Autocalculate" */
    boost::optional<double> ratedLowSpeedAirFlowRate() const;

    double ratedLowSpeedEvaporatorFanPowerPerVolumeFlowRate2017() const;

    double ratedLowSpeedEvaporatorFanPowerPerVolumeFlowRate2023() const;

    /** Returns the Curve referred to by the
   *  TotalCoolingCapacityFunctionOfTemperatureCurveName field. */
    Curve lowSpeedTotalCoolingCapacityFunctionOfTemperatureCurve() const;

    /** Returns the Curve referred to by the
   *  LowSpeedEnergyInputRatioFunctionOfTemperatureCurveName field. */
    Curve lowSpeedEnergyInputRatioFunctionOfTemperatureCurve() const;

    boost::optional<std::string> condenserAirInletNodeName() const;

    /** Returns the value of the CondenserType field. */
    std::string condenserType() const;

    /** Returns the value of the HighSpeedEvaporativeCondenserEffectiveness field. */
    double highSpeedEvaporativeCondenserEffectiveness() const;

    /** Returns the value of the HighSpeedEvaporativeCondenserAirFlowRate field.
   *  optional == flase means "autosize" */
    boost::optional<double> highSpeedEvaporativeCondenserAirFlowRate() const;

    /** Returns the value of the HighSpeedEvaporativeCondenserPumpRatedPowerConsumption field.
   *  optional == flase means "autosize" */
    boost::optional<double> highSpeedEvaporativeCondenserPumpRatedPowerConsumption() const;

    /** Returns the value of the LowSpeedEvaporativeCondenserEffectiveness field. */
    double lowSpeedEvaporativeCondenserEffectiveness() const;

    /** Returns the value of the LowSpeedEvaporativeCondenserAirFlowRate field.
   *  optional == flase means "autosize" */
    boost::optional<double> lowSpeedEvaporativeCondenserAirFlowRate() const;

    /** Returns the value of the LowSpeedEvaporativeCondenserPumpRatedPowerConsumption field.
   *  optional == flase means "autosize" */
    boost::optional<double> lowSpeedEvaporativeCondenserPumpRatedPowerConsumption() const;

    /** Returns the value of the BasinHeaterCapacity field. **/
    double basinHeaterCapacity() const;

    /** Returns the value of the BasinHeaterSetpointTemperature field. **/
    double basinHeaterSetpointTemperature() const;

    /** Returns the Schedule referred to by the BasinHeaterOperatingSchedule field. **/
    boost::optional<Schedule> basinHeaterOperatingSchedule() const;

    double minimumOutdoorDryBulbTemperatureforCompressorOperation() const;

    double unitInternalStaticAirPressure() const;

    //@}
    /** @name Setters */
    //@{

    /** Sets the Schedule referred to by the AvailabilitySchedule field. **/
    bool setAvailabilitySchedule(Schedule& schedule);

    /** Sets the value of the RatedHighSpeedTotalCoolingCapacity field.
      If optional is false, field is set to autocalculate
  **/
    bool setRatedHighSpeedTotalCoolingCapacity(boost::optional<double> value);

    bool setRatedHighSpeedTotalCoolingCapacity(double value);

    /** Sets the value of the RatedHighSpeedSensibleHeatRatio field.
   *   If optional is false, field is set to autocalculate */
    bool setRatedHighSpeedSensibleHeatRatio(boost::optional<double> value);

    bool setRatedHighSpeedSensibleHeatRatio(double value);

    /** Sets the value of the RatedHighSpeedCOP field. **/
    bool setRatedHighSpeedCOP(double value);

    /** Sets the value of the RatedHighSpeedAirFlowRate field.
   *  If optional is false, field is set to autocalculate */
    bool setRatedHighSpeedAirFlowRate(boost::optional<double> value);

    bool setRatedHighSpeedAirFlowRate(double value);

    bool setRatedHighSpeedEvaporatorFanPowerPerVolumeFlowRate2017(double ratedHighSpeedEvaporatorFanPowerPerVolumeFlowRate2017);

    bool setRatedHighSpeedEvaporatorFanPowerPerVolumeFlowRate2023(double ratedHighSpeedEvaporatorFanPowerPerVolumeFlowRate2023);

    /** Sets the Curve referred to by the
   *  TotalCoolingCapacityFunctionOfTemperatureCurveName field. **/
    bool setTotalCoolingCapacityFunctionOfTemperatureCurve(const Curve& curve);

    /** Sets the Curve referred to by the TotalCoolingCapacityFunctionOfFlowFractionCurnveName field. **/
    bool setTotalCoolingCapacityFunctionOfFlowFractionCurve(const Curve& curve);

    /** Sets the Curve referred to by the EnergyInputRatioFunctionOfTemperatureCurveName field. **/
    bool setEnergyInputRatioFunctionOfTemperatureCurve(const Curve& curve);

    /** Sets the Curve referred to by the EnergyInputRatioFunctionOfFlowFractionCurveName field. **/
    bool setEnergyInputRatioFunctionOfFlowFractionCurve(const Curve& curve);

    /** Sets the Curve referred to by the PartLoadFractionCorrelationCurveName field. **/
    bool setPartLoadFractionCorrelationCurve(const Curve& curve);

    /** Sets the value of the RatedLowSpeedTotalCoolingCapacity field.
   *  If optional is false, field is set to autocalculate */
    bool setRatedLowSpeedTotalCoolingCapacity(boost::optional<double> value);

    bool setRatedLowSpeedTotalCoolingCapacity(double value);

    /** Sets the value of the RatedLowSpeedSensibleHeatRatio field.
      If optional is false, field is set to autocalculate
  **/
    bool setRatedLowSpeedSensibleHeatRatio(boost::optional<double> value);

    bool setRatedLowSpeedSensibleHeatRatio(double value);

    /** Sets the value of the RatedLowSpeedCOP field. **/
    bool setRatedLowSpeedCOP(double value);

    /** Sets the value of the RatedLowSpeedAirFlowRate field.
   *  If optional is false, field is set to autocalculate */
    bool setRatedLowSpeedAirFlowRate(boost::optional<double> value);

    bool setRatedLowSpeedAirFlowRate(double value);

    bool setRatedLowSpeedEvaporatorFanPowerPerVolumeFlowRate2017(double ratedLowSpeedEvaporatorFanPowerPerVolumeFlowRate2017);

    bool setRatedLowSpeedEvaporatorFanPowerPerVolumeFlowRate2023(double ratedLowSpeedEvaporatorFanPowerPerVolumeFlowRate2023);

    /** Sets the Curve referred to by the LowSpeedTotalCoolingCapacityFunctionOfTemperatureCurveName field. */
    bool setLowSpeedTotalCoolingCapacityFunctionOfTemperatureCurve(const Curve& curve);

    /** Sets the Curve referred to by the LowSpeedEnergyInputRatioFunctionOfTemperatureCurveName field. */
    bool setLowSpeedEnergyInputRatioFunctionOfTemperatureCurve(const Curve& curve);

    bool setCondenserAirInletNodeName(const std::string&);

    /** Sets the value of the CondenserType field.
   *  Options are AirCooled and EvaporativelyCooled. */
    bool setCondenserType(const std::string& value);

    /** Sets the value of the HighSpeedEvaporativeCondenserEffectiveness field. */
    bool setHighSpeedEvaporativeCondenserEffectiveness(double value);

    /** Sets the value of the HighSpeedEvaporativeCondenserAirFlowRate field.
   *  if value==false then set field to "autosize" */
    bool setHighSpeedEvaporativeCondenserAirFlowRate(boost::optional<double> value);

    bool setHighSpeedEvaporativeCondenserAirFlowRate(double value);

    /** Sets the value of the HighSpeedEvaporativeCondenserPumpRatedPowerConsumption field.
   *  if value==false then set field to "autosize" */
    bool setHighSpeedEvaporativeCondenserPumpRatedPowerConsumption(boost::optional<double> value);

    bool setHighSpeedEvaporativeCondenserPumpRatedPowerConsumption(double value);

    /** Sets the value of the LowSpeedEvaporativeCondenserEffectiveness field. */
    bool setLowSpeedEvaporativeCondenserEffectiveness(double value);

    /** Sets the value of the LowSpeedEvaporativeCondenserAirFlowRate field.
   *  if value==false then set field to "autosize" */
    bool setLowSpeedEvaporativeCondenserAirFlowRate(boost::optional<double> value);

    bool setLowSpeedEvaporativeCondenserAirFlowRate(double value);

    /** Sets the value of the LowSpeedEvaporativeCondenserPumpRatedPowerConsumption field.
   *  if value==false then set field to "autosize" */
    bool setLowSpeedEvaporativeCondenserPumpRatedPowerConsumption(boost::optional<double> value);

    bool setLowSpeedEvaporativeCondenserPumpRatedPowerConsumption(double value);

    //supplyWaterStorageTankName
    //setSupplyWaterStorageTankName

    //condensateCollectionWaterStorageTankName
    //setCondensateCollectionWaterStorageTankName

    /** Sets the value of the BasinHeaterCapacity field. **/
    bool setBasinHeaterCapacity(double value);

    /** Sets the value of the BasinHeaterSetpointTemperature field. **/
    bool setBasinHeaterSetpointTemperature(double value);

    /** Sets the Schedule referred to by the BasinHeaterOperatingSchedule field. **/
    bool setBasinHeaterOperatingSchedule(Schedule& schedule);

    void resetBasinHeaterOperatingSchedule();

    bool setMinimumOutdoorDryBulbTemperatureforCompressorOperation(double minimumOutdoorDryBulbTemperatureforCompressorOperation);

    bool setUnitInternalStaticAirPressure(double unitInternalStaticAirPressure);

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedRatedHighSpeedTotalCoolingCapacity() const;

    boost::optional<double> autosizedRatedHighSpeedSensibleHeatRatio() const;

    boost::optional<double> autosizedRatedHighSpeedAirFlowRate() const;

    boost::optional<double> autosizedRatedLowSpeedTotalCoolingCapacity() const;

    boost::optional<double> autosizedRatedLowSpeedSensibleHeatRatio() const;

    boost::optional<double> autosizedRatedLowSpeedAirFlowRate() const;

    boost::optional<double> autosizedHighSpeedEvaporativeCondenserAirFlowRate() const;

    boost::optional<double> autosizedHighSpeedEvaporativeCondenserPumpRatedPowerConsumption() const;

    boost::optional<double> autosizedLowSpeedEvaporativeCondenserAirFlowRate() const;

    boost::optional<double> autosizedLowSpeedEvaporativeCondenserPumpRatedPowerConsumption() const;

    //@}
   protected:
    friend class Model;
    friend class openstudio::IdfObject;

    /// @cond
    using ImplType = detail::CoilCoolingDXTwoSpeed_Impl;

    explicit CoilCoolingDXTwoSpeed(std::shared_ptr<detail::CoilCoolingDXTwoSpeed_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.CoilCoolingDXTwoSpeed");
    /// @endcond
  };

  /** \relates CoilCoolingDXTwoSpeed */
  using OptionalCoilCoolingDXTwoSpeed = boost::optional<CoilCoolingDXTwoSpeed>;

  /** \relates CoilCoolingDXTwoSpeed */
  using CoilCoolingDXTwoSpeedVector = std::vector<CoilCoolingDXTwoSpeed>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGDXTWOSPEED_HPP
