/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILHEATINGWATERBASEBOARD_HPP
#define MODEL_COILHEATINGWATERBASEBOARD_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class CoilHeatingWaterBaseboard_Impl;

  }  // namespace detail

  /** CoilHeatingWaterBaseboard is a StraightComponent that wraps the OpenStudio IDD object 'OS:Coil:Heating:Water:Baseboard'. */
  class MODEL_API CoilHeatingWaterBaseboard : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    CoilHeatingWaterBaseboard(const Model& model);

    virtual ~CoilHeatingWaterBaseboard() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilHeatingWaterBaseboard(const CoilHeatingWaterBaseboard& other) = default;
    CoilHeatingWaterBaseboard(CoilHeatingWaterBaseboard&& other) = default;
    CoilHeatingWaterBaseboard& operator=(const CoilHeatingWaterBaseboard&) = default;
    CoilHeatingWaterBaseboard& operator=(CoilHeatingWaterBaseboard&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> heatingDesignCapacityMethodValues();

    /** @name Getters */
    //@{

    unsigned inletPort() const;

    unsigned outletPort() const;

    std::string heatingDesignCapacityMethod() const;

    boost::optional<double> heatingDesignCapacity() const;

    bool isHeatingDesignCapacityAutosized() const;

    double heatingDesignCapacityPerFloorArea() const;

    double fractionofAutosizedHeatingDesignCapacity() const;

    /** Returns the value of the UFactorTimesAreaValue field. **/
    boost::optional<double> uFactorTimesAreaValue() const;

    bool isUFactorTimesAreaValueDefaulted() const;

    /** Returns true if the UFactorTimesAreaValue field is set to autosize **/
    bool isUFactorTimesAreaValueAutosized() const;

    /** Returns the value of the MaximumWaterFlowRate field. **/
    boost::optional<double> maximumWaterFlowRate() const;

    bool isMaximumWaterFlowRateDefaulted() const;

    bool isMaximumWaterFlowRateAutosized() const;

    double convergenceTolerance() const;

    bool isConvergenceToleranceDefaulted() const;

    //@}
    /** @name Setters */
    //@{
    bool setHeatingDesignCapacityMethod(const std::string& heatingDesignCapacityMethod);

    bool setHeatingDesignCapacity(double heatingDesignCapacity);

    void autosizeHeatingDesignCapacity();

    bool setHeatingDesignCapacityPerFloorArea(double heatingDesignCapacityPerFloorArea);

    bool setFractionofAutosizedHeatingDesignCapacity(double fractionofAutosizedHeatingDesignCapacity);

    bool setUFactorTimesAreaValue(double uFactorTimesAreaValue);

    void resetUFactorTimesAreaValue();

    void autosizeUFactorTimesAreaValue();

    bool setMaximumWaterFlowRate(double maximumWaterFlowRate);

    void resetMaximumWaterFlowRate();

    void autosizeMaximumWaterFlowRate();

    bool setConvergenceTolerance(double convergenceTolerance);

    void resetConvergenceTolerance();

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedHeatingDesignCapacity() const;

    boost::optional<double> autosizedUFactorTimesAreaValue() const;

    boost::optional<double> autosizedMaximumWaterFlowRate() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilHeatingWaterBaseboard_Impl;

    explicit CoilHeatingWaterBaseboard(std::shared_ptr<detail::CoilHeatingWaterBaseboard_Impl> impl);

    friend class detail::CoilHeatingWaterBaseboard_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilHeatingWaterBaseboard");
  };

  /** \relates CoilHeatingWaterBaseboard*/
  using OptionalCoilHeatingWaterBaseboard = boost::optional<CoilHeatingWaterBaseboard>;

  /** \relates CoilHeatingWaterBaseboard*/
  using CoilHeatingWaterBaseboardVector = std::vector<CoilHeatingWaterBaseboard>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGWATERBASEBOARD_HPP
