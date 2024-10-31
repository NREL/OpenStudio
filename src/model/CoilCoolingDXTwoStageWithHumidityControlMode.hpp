/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILCOOLINGDXTWOSTAGEWITHHUMIDITYCONTROLMODE_HPP
#define MODEL_COILCOOLINGDXTWOSTAGEWITHHUMIDITYCONTROLMODE_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class AirflowNetworkEquivalentDuct;
  class CoilPerformanceDXCooling;
  class Curve;
  class Schedule;

  namespace detail {

    class CoilCoolingDXTwoStageWithHumidityControlMode_Impl;

  }  // namespace detail

  /** CoilCoolingDXTwoStageWithHumidityControlMode is a StraightComponent that wraps the OpenStudio IDD object 'OS:Coil:Cooling:DX:TwoStageWithHumidityControlMode'. */
  class MODEL_API CoilCoolingDXTwoStageWithHumidityControlMode : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilCoolingDXTwoStageWithHumidityControlMode(const Model& model);

    virtual ~CoilCoolingDXTwoStageWithHumidityControlMode() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilCoolingDXTwoStageWithHumidityControlMode(const CoilCoolingDXTwoStageWithHumidityControlMode& other) = default;
    CoilCoolingDXTwoStageWithHumidityControlMode(CoilCoolingDXTwoStageWithHumidityControlMode&& other) = default;
    CoilCoolingDXTwoStageWithHumidityControlMode& operator=(const CoilCoolingDXTwoStageWithHumidityControlMode&) = default;
    CoilCoolingDXTwoStageWithHumidityControlMode& operator=(CoilCoolingDXTwoStageWithHumidityControlMode&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    boost::optional<Schedule> availabilitySchedule() const;

    double crankcaseHeaterCapacity() const;

    boost::optional<Curve> crankcaseHeaterCapacityFunctionofTemperatureCurve() const;

    double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const;

    int numberofCapacityStages() const;

    int numberofEnhancedDehumidificationModes() const;

    boost::optional<CoilPerformanceDXCooling> normalModeStage1CoilPerformance() const;

    boost::optional<CoilPerformanceDXCooling> normalModeStage1Plus2CoilPerformance() const;

    boost::optional<CoilPerformanceDXCooling> dehumidificationMode1Stage1CoilPerformance() const;

    boost::optional<CoilPerformanceDXCooling> dehumidificationMode1Stage1Plus2CoilPerformance() const;

    double basinHeaterCapacity() const;

    double basinHeaterSetpointTemperature() const;

    boost::optional<Schedule> basinHeaterOperatingSchedule() const;

    double minimumOutdoorDryBulbTemperatureforCompressorOperation() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity);

    bool setCrankcaseHeaterCapacityFunctionofTemperatureCurve(const Curve& curve);
    void resetCrankcaseHeaterCapacityFunctionofTemperatureCurve();

    bool setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);

    bool setNumberofCapacityStages(int numberofCapacityStages);

    bool setNumberofEnhancedDehumidificationModes(int numberofEnhancedDehumidificationModes);

    bool setNormalModeStage1CoilPerformance(const CoilPerformanceDXCooling& coilPerformanceDXCooling);

    void resetNormalModeStage1CoilPerformance();

    bool setNormalModeStage1Plus2CoilPerformance(const CoilPerformanceDXCooling& coilPerformanceDXCooling);

    void resetNormalModeStage1Plus2CoilPerformance();

    bool setDehumidificationMode1Stage1CoilPerformance(const CoilPerformanceDXCooling& coilPerformanceDXCooling);

    void resetDehumidificationMode1Stage1CoilPerformance();

    bool setDehumidificationMode1Stage1Plus2CoilPerformance(const CoilPerformanceDXCooling& coilPerformanceDXCooling);

    void resetDehumidificationMode1Stage1Plus2CoilPerformance();

    bool setBasinHeaterCapacity(double basinHeaterCapacity);

    bool setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature);

    bool setBasinHeaterOperatingSchedule(Schedule& schedule);

    void resetBasinHeaterOperatingSchedule();

    bool setMinimumOutdoorDryBulbTemperatureforCompressorOperation(double minimumOutdoorDryBulbTemperatureforCompressorOperation);

    //@}
    /** @name Other */
    //@{

    /** Creates a new equivalent duct object if an object is not already attached. */
    AirflowNetworkEquivalentDuct getAirflowNetworkEquivalentDuct(double length, double diameter);

    /** Returns the attached equivalent duct object, if any. */
    boost::optional<AirflowNetworkEquivalentDuct> airflowNetworkEquivalentDuct() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilCoolingDXTwoStageWithHumidityControlMode_Impl;

    explicit CoilCoolingDXTwoStageWithHumidityControlMode(std::shared_ptr<detail::CoilCoolingDXTwoStageWithHumidityControlMode_Impl> impl);

    friend class detail::CoilCoolingDXTwoStageWithHumidityControlMode_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilCoolingDXTwoStageWithHumidityControlMode");
  };

  /** \relates CoilCoolingDXTwoStageWithHumidityControlMode*/
  using OptionalCoilCoolingDXTwoStageWithHumidityControlMode = boost::optional<CoilCoolingDXTwoStageWithHumidityControlMode>;

  /** \relates CoilCoolingDXTwoStageWithHumidityControlMode*/
  using CoilCoolingDXTwoStageWithHumidityControlModeVector = std::vector<CoilCoolingDXTwoStageWithHumidityControlMode>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGDXTWOSTAGEWITHHUMIDITYCONTROLMODE_HPP
