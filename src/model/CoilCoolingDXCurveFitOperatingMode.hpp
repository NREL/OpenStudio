/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILCOOLINGDXCURVEFITOPERATINGMODE_HPP
#define MODEL_COILCOOLINGDXCURVEFITOPERATINGMODE_HPP

#include <model/ModelAPI.hpp>
#include "ResourceObject.hpp"

namespace openstudio {

namespace model {

  class CoilCoolingDXCurveFitPerformance;
  class CoilCoolingDXCurveFitSpeed;

  namespace detail {

    class CoilCoolingDXCurveFitOperatingMode_Impl;

  }  // namespace detail

  /** CoilCoolingDXCurveFitOperatingMode is a ResourceObject that wraps the OpenStudio IDD object 'OS:Coil:Cooling:DX:CurveFit:OperatingMode'. */
  class MODEL_API CoilCoolingDXCurveFitOperatingMode : public ResourceObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilCoolingDXCurveFitOperatingMode(const Model& model);

    virtual ~CoilCoolingDXCurveFitOperatingMode() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilCoolingDXCurveFitOperatingMode(const CoilCoolingDXCurveFitOperatingMode& other) = default;
    CoilCoolingDXCurveFitOperatingMode(CoilCoolingDXCurveFitOperatingMode&& other) = default;
    CoilCoolingDXCurveFitOperatingMode& operator=(const CoilCoolingDXCurveFitOperatingMode&) = default;
    CoilCoolingDXCurveFitOperatingMode& operator=(CoilCoolingDXCurveFitOperatingMode&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> condenserTypeValues();

    /** @name Getters */
    //@{

    boost::optional<double> ratedGrossTotalCoolingCapacity() const;

    bool isRatedGrossTotalCoolingCapacityAutosized() const;

    boost::optional<double> ratedEvaporatorAirFlowRate() const;

    bool isRatedEvaporatorAirFlowRateAutosized() const;

    boost::optional<double> ratedCondenserAirFlowRate() const;

    bool isRatedCondenserAirFlowRateAutosized() const;

    double maximumCyclingRate() const;

    double ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity() const;

    double latentCapacityTimeConstant() const;

    double nominalTimeforCondensateRemovaltoBegin() const;

    bool applyLatentDegradationtoSpeedsGreaterthan1() const;

    std::string condenserType() const;

    boost::optional<double> nominalEvaporativeCondenserPumpPower() const;

    bool isNominalEvaporativeCondenserPumpPowerAutosized() const;

    // Returns the nominal speed number if set, or the number of speeds if not
    unsigned nominalSpeedNumber() const;
    bool isNominalSpeedNumberDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setRatedGrossTotalCoolingCapacity(double ratedGrossTotalCoolingCapacity);
    void autosizeRatedGrossTotalCoolingCapacity();

    bool setRatedEvaporatorAirFlowRate(double ratedEvaporatorAirFlowRate);
    void autosizeRatedEvaporatorAirFlowRate();

    bool setRatedCondenserAirFlowRate(double ratedCondenserAirFlowRate);
    void autosizeRatedCondenserAirFlowRate();

    bool setMaximumCyclingRate(double maximumCyclingRate);

    bool
      setRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity(double ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity);

    bool setLatentCapacityTimeConstant(double latentCapacityTimeConstant);

    bool setNominalTimeforCondensateRemovaltoBegin(double nominalTimeforCondensateRemovaltoBegin);

    bool setApplyLatentDegradationtoSpeedsGreaterthan1(bool applyLatentDegradationtoSpeedsGreaterthan1);

    bool setCondenserType(const std::string& condenserType);

    bool setNominalEvaporativeCondenserPumpPower(double nominalEvaporativeCondenserPumpPower);
    void autosizeNominalEvaporativeCondenserPumpPower();

    bool setNominalSpeedNumber(unsigned nominalSpeedNumber);
    void resetNominalSpeedNumber();

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedRatedGrossTotalCoolingCapacity();

    boost::optional<double> autosizedRatedEvaporatorAirFlowRate();

    boost::optional<double> autosizedRatedCondenserAirFlowRate();

    boost::optional<double> autosizedNominalEvaporativeCondenserPumpPower();

    void autosize();

    void applySizingValues();

    std::vector<CoilCoolingDXCurveFitPerformance> coilCoolingDXCurveFitPerformances() const;

    std::vector<CoilCoolingDXCurveFitSpeed> speeds() const;

    unsigned numberOfSpeeds() const;

    /*
   * Get the index of a given CoilCoolingDXCurveFitSpeed (1-indexed)
   */
    boost::optional<unsigned> speedIndex(const CoilCoolingDXCurveFitSpeed& speed) const;

    /*
   * Add a new CoilCoolingDXCurveFitSpeed at the end of the list (priority = last).
   */
    bool addSpeed(const CoilCoolingDXCurveFitSpeed& speed);

    /*
   * Add a new CoilCoolingDXCurveFitSpeed to the list which a given index (1 to x).
   * Internally calls addSpeed then setSpeedIndex, see remarks there
   */
    bool addSpeed(const CoilCoolingDXCurveFitSpeed& speed, unsigned index);

    /*
   * You can shuffle the priority of a given CoilCoolingDXCurveFitSpeed after having added it
   * If index is below 1, it's reset to 1.
   * If index is greater than the number of speeds, will reset to last
   */
    bool setSpeedIndex(const CoilCoolingDXCurveFitSpeed& speed, unsigned index);

    /*
   * Set all speeds using a list of CoilCoolingDXCurveFitSpeeds
   * Internally calls addSpeed, and will return the global status, but will continue trying if there are problems
   * (eg: if you make a vector larger than the number of accepted speeds, or a vector that has a speed from another model, the valid speeds will be
   * added indeed, but it'll eventually return false)
   */
    bool setSpeeds(const std::vector<CoilCoolingDXCurveFitSpeed>& speeds);

    /*
   * Removes all CoilCoolingDXCurveFitSpeeds in this object
   */
    void removeAllSpeeds();

    /*
   * Remove the given CoilCoolingDXCurveFitSpeed from this object's speeds
   */
    bool removeSpeed(const CoilCoolingDXCurveFitSpeed& speed);

    /*
   * Remove the CoilCoolingDXCurveFitSpeed at the given index (1-indexed)
   */
    bool removeSpeed(unsigned index);

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilCoolingDXCurveFitOperatingMode_Impl;

    explicit CoilCoolingDXCurveFitOperatingMode(std::shared_ptr<detail::CoilCoolingDXCurveFitOperatingMode_Impl> impl);

    friend class detail::CoilCoolingDXCurveFitOperatingMode_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilCoolingDXCurveFitOperatingMode");
  };

  /** \relates CoilCoolingDXCurveFitOperatingMode*/
  using OptionalCoilCoolingDXCurveFitOperatingMode = boost::optional<CoilCoolingDXCurveFitOperatingMode>;

  /** \relates CoilCoolingDXCurveFitOperatingMode*/
  using CoilCoolingDXCurveFitOperatingModeVector = std::vector<CoilCoolingDXCurveFitOperatingMode>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGDXCURVEFITOPERATINGMODE_HPP
