/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILCOOLINGDXMULTISPEED_HPP
#define MODEL_COILCOOLINGDXMULTISPEED_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class AirflowNetworkEquivalentDuct;
  class CoilCoolingDXMultiSpeedStageData;
  class Curve;
  class Schedule;

  namespace detail {

    class CoilCoolingDXMultiSpeed_Impl;

  }  // namespace detail

  /** CoilCoolingDXMultiSpeed is a StraightComponent that wraps the OpenStudio IDD object 'OS:Coil:Cooling:DX:MultiSpeed'.
 * Performance data for each stage is contained in a separate ModelObject called CoilCoolingDXMultiSpeedStageData **/
  class MODEL_API CoilCoolingDXMultiSpeed : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilCoolingDXMultiSpeed(const Model& model);

    virtual ~CoilCoolingDXMultiSpeed() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilCoolingDXMultiSpeed(const CoilCoolingDXMultiSpeed& other) = default;
    CoilCoolingDXMultiSpeed(CoilCoolingDXMultiSpeed&& other) = default;
    CoilCoolingDXMultiSpeed& operator=(const CoilCoolingDXMultiSpeed&) = default;
    CoilCoolingDXMultiSpeed& operator=(CoilCoolingDXMultiSpeed&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> condenserTypeValues();

    static std::vector<std::string> fuelTypeValues();

    /** @name Getters */
    //@{

    boost::optional<Schedule> availabilitySchedule() const;

    std::string condenserType() const;

    bool applyPartLoadFractiontoSpeedsGreaterthan1() const;

    bool applyLatentDegradationtoSpeedsGreaterthan1() const;

    bool isApplyLatentDegradationtoSpeedsGreaterthan1Defaulted() const;

    double crankcaseHeaterCapacity() const;

    boost::optional<Curve> crankcaseHeaterCapacityFunctionofTemperatureCurve() const;

    double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const;

    double basinHeaterCapacity() const;

    double basinHeaterSetpointTemperature() const;

    boost::optional<Schedule> basinHeaterOperatingSchedule() const;

    std::string fuelType() const;

    double minimumOutdoorDryBulbTemperatureforCompressorOperation() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setCondenserType(const std::string& condenserType);

    bool setApplyPartLoadFractiontoSpeedsGreaterthan1(bool applyPartLoadFractiontoSpeedsGreaterthan1);

    bool setApplyLatentDegradationtoSpeedsGreaterthan1(bool applyLatentDegradationtoSpeedsGreaterthan1);

    void resetApplyLatentDegradationtoSpeedsGreaterthan1();

    bool setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity);

    bool setCrankcaseHeaterCapacityFunctionofTemperatureCurve(const Curve& curve);
    void resetCrankcaseHeaterCapacityFunctionofTemperatureCurve();

    bool setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);

    bool setBasinHeaterCapacity(double basinHeaterCapacity);

    bool setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature);

    bool setBasinHeaterOperatingSchedule(Schedule& schedule);

    void resetBasinHeaterOperatingSchedule();

    bool setFuelType(const std::string& fuelType);

    bool setMinimumOutdoorDryBulbTemperatureforCompressorOperation(double minimumOutdoorDryBulbTemperatureforCompressorOperation);

    //@}
    /** @name Other */
    //@{

    /** Return the performance data for each stage. **/
    std::vector<CoilCoolingDXMultiSpeedStageData> stages() const;

    unsigned numberOfStages() const;

    /*
   * Get the index of a given CoilCoolingDXMultiSpeedStageData (1-indexed)
   */
    boost::optional<unsigned> stageIndex(const CoilCoolingDXMultiSpeedStageData& stage) const;

    /*
   * Add a new stage after all of the existing stages.
   */
    bool addStage(const CoilCoolingDXMultiSpeedStageData& stage);

    /*
   * Add a new CoilCoolingDXMultiSpeedStageData to the list which a given index (1 to x).
   * Internally calls addStage then setStageIndex, see remarks there
   */
    bool addStage(const CoilCoolingDXMultiSpeedStageData& stage, unsigned index);

    /*
   * You can shuffle the priority of a given CoilCoolingDXMultiSpeedStageData after having added it
   * If index is below 1, it's reset to 1.
   * If index is greater than the number of stages, will reset to last
   */
    bool setStageIndex(const CoilCoolingDXMultiSpeedStageData& stage, unsigned index);

    /*
   * Set all stages using a list of CoilCoolingDXMultiSpeedStageDatas
   * Internally calls addStage, and will return the global status, but will continue trying if there are problems
   * (eg: if you make a vector larger than the number of accepted stages, or a vector that has a stage from another model, the valid stages will be
   * added indeed, but it'll eventually return false)
   */
    bool setStages(const std::vector<CoilCoolingDXMultiSpeedStageData>& stages);

    /*
   * Removes all CoilCoolingDXMultiSpeedStageDatas in this object
   */
    void removeAllStages();

    /*
   * Remove the given CoilCoolingDXMultiSpeedStageData from this object's stages
   */
    bool removeStage(const CoilCoolingDXMultiSpeedStageData& stage);

    /*
   * Remove the CoilCoolingDXMultiSpeedStageData at the given index (1-indexed)
   */
    bool removeStage(unsigned index);

    /** Creates a new equivalent duct object if an object is not already attached. */
    AirflowNetworkEquivalentDuct getAirflowNetworkEquivalentDuct(double length, double diameter);

    /** Returns the attached equivalent duct object, if any. */
    boost::optional<AirflowNetworkEquivalentDuct> airflowNetworkEquivalentDuct() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilCoolingDXMultiSpeed_Impl;

    explicit CoilCoolingDXMultiSpeed(std::shared_ptr<detail::CoilCoolingDXMultiSpeed_Impl> impl);

    friend class detail::CoilCoolingDXMultiSpeed_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilCoolingDXMultiSpeed");
  };

  /** \relates CoilCoolingDXMultiSpeed*/
  using OptionalCoilCoolingDXMultiSpeed = boost::optional<CoilCoolingDXMultiSpeed>;

  /** \relates CoilCoolingDXMultiSpeed*/
  using CoilCoolingDXMultiSpeedVector = std::vector<CoilCoolingDXMultiSpeed>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGDXMULTISPEED_HPP
