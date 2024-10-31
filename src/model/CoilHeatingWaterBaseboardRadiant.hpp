/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILHEATINGWATERBASEBOARDRADIANT_HPP
#define MODEL_COILHEATINGWATERBASEBOARDRADIANT_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class CoilHeatingWaterBaseboardRadiant_Impl;

  }  // namespace detail

  /** CoilHeatingWaterBaseboardRadiant is a StraightComponent that wraps the OpenStudio IDD object 'OS:Coil:Heating:Water:Baseboard:Radiant'. */
  class MODEL_API CoilHeatingWaterBaseboardRadiant : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilHeatingWaterBaseboardRadiant(const Model& model);

    virtual ~CoilHeatingWaterBaseboardRadiant() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilHeatingWaterBaseboardRadiant(const CoilHeatingWaterBaseboardRadiant& other) = default;
    CoilHeatingWaterBaseboardRadiant(CoilHeatingWaterBaseboardRadiant&& other) = default;
    CoilHeatingWaterBaseboardRadiant& operator=(const CoilHeatingWaterBaseboardRadiant&) = default;
    CoilHeatingWaterBaseboardRadiant& operator=(CoilHeatingWaterBaseboardRadiant&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> heatingDesignCapacityMethodValues();

    /** @name Getters */
    //@{

    double ratedAverageWaterTemperature() const;

    double ratedWaterMassFlowRate() const;

    std::string heatingDesignCapacityMethod() const;

    boost::optional<double> heatingDesignCapacity() const;

    bool isHeatingDesignCapacityAutosized() const;

    double heatingDesignCapacityPerFloorArea() const;

    double fractionofAutosizedHeatingDesignCapacity() const;

    boost::optional<double> maximumWaterFlowRate() const;

    bool isMaximumWaterFlowRateAutosized() const;

    double convergenceTolerance() const;

    //@}
    /** @name Setters */
    //@{

    bool setRatedAverageWaterTemperature(double ratedAverageWaterTemperature);

    bool setRatedWaterMassFlowRate(double ratedWaterMassFlowRate);

    bool setHeatingDesignCapacityMethod(const std::string& heatingDesignCapacityMethod);

    bool setHeatingDesignCapacity(double heatingDesignCapacity);

    void autosizeHeatingDesignCapacity();

    bool setHeatingDesignCapacityPerFloorArea(double heatingDesignCapacityPerFloorArea);

    bool setFractionofAutosizedHeatingDesignCapacity(double fractionofAutosizedHeatingDesignCapacity);

    bool setMaximumWaterFlowRate(double maximumWaterFlowRate);

    void autosizeMaximumWaterFlowRate();

    bool setConvergenceTolerance(double convergenceTolerance);

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedHeatingDesignCapacity() const;

    boost::optional<double> autosizedMaximumWaterFlowRate() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilHeatingWaterBaseboardRadiant_Impl;

    explicit CoilHeatingWaterBaseboardRadiant(std::shared_ptr<detail::CoilHeatingWaterBaseboardRadiant_Impl> impl);

    friend class detail::CoilHeatingWaterBaseboardRadiant_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilHeatingWaterBaseboardRadiant");
  };

  /** \relates CoilHeatingWaterBaseboardRadiant*/
  using OptionalCoilHeatingWaterBaseboardRadiant = boost::optional<CoilHeatingWaterBaseboardRadiant>;

  /** \relates CoilHeatingWaterBaseboardRadiant*/
  using CoilHeatingWaterBaseboardRadiantVector = std::vector<CoilHeatingWaterBaseboardRadiant>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGWATERBASEBOARDRADIANT_HPP
