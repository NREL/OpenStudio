/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILHEATINGDXMULTISPEED_HPP
#define MODEL_COILHEATINGDXMULTISPEED_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class AirflowNetworkEquivalentDuct;
  class CoilHeatingDXMultiSpeedStageData;
  class Curve;
  class Schedule;

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

    virtual ~CoilHeatingDXMultiSpeed() override = default;
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

    boost::optional<Curve> crankcaseHeaterCapacityFunctionofTemperatureCurve() const;

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

    bool setCrankcaseHeaterCapacityFunctionofTemperatureCurve(const Curve& curve);
    void resetCrankcaseHeaterCapacityFunctionofTemperatureCurve();

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
