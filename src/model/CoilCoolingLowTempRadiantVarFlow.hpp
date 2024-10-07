/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILCOOLINGLOWTEMPRADIANTVARFLOW_HPP
#define MODEL_COILCOOLINGLOWTEMPRADIANTVARFLOW_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"
#include "ZoneHVACLowTempRadiantVarFlow.hpp"
#include "ZoneHVACLowTempRadiantVarFlow_Impl.hpp"

namespace openstudio {

namespace model {

  class Schedule;

  namespace detail {

    class CoilCoolingLowTempRadiantVarFlow_Impl;

  }  // namespace detail

  /** CoilCoolingLowTempRadiantVarFlow is a StraightComponent that wraps the OpenStudio IDD object 'OS:Coil:Cooling:LowTemperatureRadiant:VariableFlow'. */
  class MODEL_API CoilCoolingLowTempRadiantVarFlow : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilCoolingLowTempRadiantVarFlow(const Model& model, Schedule& coolingControlTemperatureSchedule);

    virtual ~CoilCoolingLowTempRadiantVarFlow() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilCoolingLowTempRadiantVarFlow(const CoilCoolingLowTempRadiantVarFlow& other) = default;
    CoilCoolingLowTempRadiantVarFlow(CoilCoolingLowTempRadiantVarFlow&& other) = default;
    CoilCoolingLowTempRadiantVarFlow& operator=(const CoilCoolingLowTempRadiantVarFlow&) = default;
    CoilCoolingLowTempRadiantVarFlow& operator=(CoilCoolingLowTempRadiantVarFlow&&) = default;

    //@}

    static IddObjectType iddObjectType();

    //unsigned inletPort() const;

    //unsigned outletPort() const;

    static std::vector<std::string> coolingDesignCapacityMethodValues();

    static std::vector<std::string> condensationControlTypeValues();

    /** @name Getters */
    //@{

    boost::optional<double> maximumColdWaterFlow() const;

    bool isMaximumColdWaterFlowDefaulted() const;

    bool isMaximumColdWaterFlowAutosized() const;

    double coolingControlThrottlingRange() const;

    bool isCoolingControlThrottlingRangeDefaulted() const;

    boost::optional<Schedule> coolingControlTemperatureSchedule() const;

    std::string condensationControlType() const;

    bool isCondensationControlTypeDefaulted() const;

    double condensationControlDewpointOffset() const;

    bool isCondensationControlDewpointOffsetDefaulted() const;

    std::string coolingDesignCapacityMethod() const;

    boost::optional<double> coolingDesignCapacity() const;
    bool isCoolingDesignCapacityAutosized() const;

    double coolingDesignCapacityPerFloorArea() const;

    double fractionofAutosizedCoolingDesignCapacity() const;

    //@}
    /** @name Setters */
    //@{

    bool setMaximumColdWaterFlow(double maximumColdWaterFlow);

    void resetMaximumColdWaterFlow();

    void autosizeMaximumColdWaterFlow();

    bool setCoolingControlThrottlingRange(double coolingControlThrottlingRange);

    void resetCoolingControlThrottlingRange();

    bool setCoolingControlTemperatureSchedule(Schedule& schedule);

    void resetCoolingControlTemperatureSchedule();

    bool setCondensationControlType(const std::string& condensationControlType);

    void resetCondensationControlType();

    bool setCondensationControlDewpointOffset(double condensationControlDewpointOffset);

    void resetCondensationControlDewpointOffset();

    bool setCoolingDesignCapacityMethod(const std::string& coolingDesignCapacityMethod);

    bool setCoolingDesignCapacity(double coolingDesignCapacity);
    void autosizeCoolingDesignCapacity();

    bool setCoolingDesignCapacityPerFloorArea(double coolingDesignCapacityPerFloorArea);

    bool setFractionofAutosizedCoolingDesignCapacity(double fractionofAutosizedCoolingDesignCapacity);

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedMaximumColdWaterFlow() const;

    boost::optional<double> autosizedCoolingDesignCapacity() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilCoolingLowTempRadiantVarFlow_Impl;

    explicit CoilCoolingLowTempRadiantVarFlow(std::shared_ptr<detail::CoilCoolingLowTempRadiantVarFlow_Impl> impl);

    friend class detail::CoilCoolingLowTempRadiantVarFlow_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilCoolingLowTempRadiantVarFlow");

    // Used to find the ZoneHVAC that contains this coil
    boost::optional<ZoneHVACLowTempRadiantVarFlow> parentZoneHVAC() const;
  };

  /** \relates CoilCoolingLowTempRadiantVarFlow*/
  using OptionalCoilCoolingLowTempRadiantVarFlow = boost::optional<CoilCoolingLowTempRadiantVarFlow>;

  /** \relates CoilCoolingLowTempRadiantVarFlow*/
  using CoilCoolingLowTempRadiantVarFlowVector = std::vector<CoilCoolingLowTempRadiantVarFlow>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGLOWTEMPRADIANTVARFLOW_HPP
