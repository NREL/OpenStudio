/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILCOOLINGWATERPANELRADIANT_HPP
#define MODEL_COILCOOLINGWATERPANELRADIANT_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class CoilCoolingWaterPanelRadiant_Impl;

  }  // namespace detail

  /** CoilCoolingWaterPanelRadiant is a StraightComponent that wraps the OpenStudio IDD object 'OS:Coil:Cooling:Water:Panel:Radiant'. */
  class MODEL_API CoilCoolingWaterPanelRadiant : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilCoolingWaterPanelRadiant(const Model& model);

    virtual ~CoilCoolingWaterPanelRadiant() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilCoolingWaterPanelRadiant(const CoilCoolingWaterPanelRadiant& other) = default;
    CoilCoolingWaterPanelRadiant(CoilCoolingWaterPanelRadiant&& other) = default;
    CoilCoolingWaterPanelRadiant& operator=(const CoilCoolingWaterPanelRadiant&) = default;
    CoilCoolingWaterPanelRadiant& operator=(CoilCoolingWaterPanelRadiant&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> coolingDesignCapacityMethodValues();

    /** @name Getters */
    //@{

    double ratedInletWaterTemperature() const;

    double ratedInletSpaceTemperature() const;

    double ratedWaterMassFlowRate() const;

    std::string coolingDesignCapacityMethod() const;

    boost::optional<double> coolingDesignCapacity() const;

    bool isCoolingDesignCapacityAutosized() const;

    double coolingDesignCapacityPerFloorArea() const;

    double fractionofAutosizedCoolingDesignCapacity() const;

    boost::optional<double> maximumChilledWaterFlowRate() const;

    bool isMaximumChilledWaterFlowRateAutosized() const;

    std::string controlType() const;

    double coolingControlThrottlingRange() const;

    boost::optional<Schedule> coolingControlTemperatureSchedule() const;

    std::string condensationControlType() const;

    double condensationControlDewpointOffset() const;

    //@}
    /** @name Setters */
    //@{

    bool setRatedInletWaterTemperature(double ratedInletWaterTemperature);

    bool setRatedInletSpaceTemperature(double ratedInletSpaceTemperature);

    bool setRatedWaterMassFlowRate(double ratedWaterMassFlowRate);

    bool setCoolingDesignCapacityMethod(const std::string& coolingDesignCapacityMethod);

    bool setCoolingDesignCapacity(double coolingDesignCapacity);

    void autosizeCoolingDesignCapacity();

    bool setCoolingDesignCapacityPerFloorArea(double coolingDesignCapacityPerFloorArea);

    bool setFractionofAutosizedCoolingDesignCapacity(double fractionofAutosizedCoolingDesignCapacity);

    bool setMaximumChilledWaterFlowRate(double maximumChilledWaterFlowRate);

    void autosizeMaximumChilledWaterFlowRate();

    bool setControlType(const std::string& controlType);

    bool setCoolingControlThrottlingRange(double coolingControlThrottlingRange);

    bool setCoolingControlTemperatureSchedule(Schedule& coolingControlTemperatureSchedule);

    void resetCoolingControlTemperatureSchedule();

    bool setCondensationControlType(const std::string& condensationControlType);

    bool setCondensationControlDewpointOffset(double condensationControlDewpointOffset);

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedCoolingDesignCapacity() const;

    boost::optional<double> autosizedMaximumChilledWaterFlowRate() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilCoolingWaterPanelRadiant_Impl;

    explicit CoilCoolingWaterPanelRadiant(std::shared_ptr<detail::CoilCoolingWaterPanelRadiant_Impl> impl);

    friend class detail::CoilCoolingWaterPanelRadiant_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilCoolingWaterPanelRadiant");
  };

  /** \relates CoilCoolingWaterPanelRadiant*/
  using OptionalCoilCoolingWaterPanelRadiant = boost::optional<CoilCoolingWaterPanelRadiant>;

  /** \relates CoilCoolingWaterPanelRadiant*/
  using CoilCoolingWaterPanelRadiantVector = std::vector<CoilCoolingWaterPanelRadiant>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGWATERPANELRADIANT_HPP
