/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILHEATINGLOWTEMPRADIANTVARFLOW_HPP
#define MODEL_COILHEATINGLOWTEMPRADIANTVARFLOW_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"
#include "ZoneHVACLowTempRadiantVarFlow.hpp"
#include "ZoneHVACLowTempRadiantVarFlow_Impl.hpp"

namespace openstudio {

namespace model {

  class Schedule;

  namespace detail {

    class CoilHeatingLowTempRadiantVarFlow_Impl;

  }  // namespace detail

  /** CoilHeatingLowTempRadiantVarFlow is a StraightComponent that wraps the OpenStudio IDD object 'OS:Coil:Heating:LowTemperatureRadiant:VariableFlow'. */
  class MODEL_API CoilHeatingLowTempRadiantVarFlow : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilHeatingLowTempRadiantVarFlow(const Model& model, Schedule& heatingControlTemperature);

    virtual ~CoilHeatingLowTempRadiantVarFlow() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilHeatingLowTempRadiantVarFlow(const CoilHeatingLowTempRadiantVarFlow& other) = default;
    CoilHeatingLowTempRadiantVarFlow(CoilHeatingLowTempRadiantVarFlow&& other) = default;
    CoilHeatingLowTempRadiantVarFlow& operator=(const CoilHeatingLowTempRadiantVarFlow&) = default;
    CoilHeatingLowTempRadiantVarFlow& operator=(CoilHeatingLowTempRadiantVarFlow&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> heatingDesignCapacityMethodValues();

    /** @name Getters */
    //@{

    boost::optional<double> maximumHotWaterFlow() const;

    bool isMaximumHotWaterFlowDefaulted() const;

    bool isMaximumHotWaterFlowAutosized() const;

    double heatingControlThrottlingRange() const;

    bool isHeatingControlThrottlingRangeDefaulted() const;

    boost::optional<Schedule> heatingControlTemperatureSchedule() const;

    std::string heatingDesignCapacityMethod() const;

    boost::optional<double> heatingDesignCapacity() const;
    bool isHeatingDesignCapacityAutosized() const;

    double heatingDesignCapacityPerFloorArea() const;

    double fractionofAutosizedHeatingDesignCapacity() const;

    //@}
    /** @name Setters */
    //@{

    bool setMaximumHotWaterFlow(double maximumHotWaterFlow);

    void resetMaximumHotWaterFlow();

    void autosizeMaximumHotWaterFlow();

    bool setHeatingControlThrottlingRange(double heatingControlThrottlingRange);

    void resetHeatingControlThrottlingRange();

    bool setHeatingControlTemperatureSchedule(Schedule& schedule);

    void resetHeatingControlTemperatureSchedule();

    bool setHeatingDesignCapacityMethod(const std::string& heatingDesignCapacityMethod);

    bool setHeatingDesignCapacity(double heatingDesignCapacity);
    void autosizeHeatingDesignCapacity();

    bool setHeatingDesignCapacityPerFloorArea(double heatingDesignCapacityPerFloorArea);

    bool setFractionofAutosizedHeatingDesignCapacity(double fractionofAutosizedHeatingDesignCapacity);

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedMaximumHotWaterFlow() const;

    boost::optional<double> autosizedHeatingDesignCapacity() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilHeatingLowTempRadiantVarFlow_Impl;

    explicit CoilHeatingLowTempRadiantVarFlow(std::shared_ptr<detail::CoilHeatingLowTempRadiantVarFlow_Impl> impl);

    friend class detail::CoilHeatingLowTempRadiantVarFlow_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilHeatingLowTempRadiantVarFlow");

    // Used to find the ZoneHVAC that contains this coil
    boost::optional<ZoneHVACLowTempRadiantVarFlow> parentZoneHVAC() const;
  };

  /** \relates CoilHeatingLowTempRadiantVarFlow*/
  using OptionalCoilHeatingLowTempRadiantVarFlow = boost::optional<CoilHeatingLowTempRadiantVarFlow>;

  /** \relates CoilHeatingLowTempRadiantVarFlow*/
  using CoilHeatingLowTempRadiantVarFlowVector = std::vector<CoilHeatingLowTempRadiantVarFlow>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGLOWTEMPRADIANTVARFLOW_HPP
