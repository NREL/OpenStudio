/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILHEATINGSTEAMBASEBOARDRADIANT_HPP
#define MODEL_COILHEATINGSTEAMBASEBOARDRADIANT_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class CoilHeatingSteamBaseboardRadiant_Impl;

  }  // namespace detail

  /** CoilHeatingSteamBaseboardRadiant is a StraightComponent that wraps the OpenStudio IDD object 'OS:Coil:Heating:Steam:Baseboard:Radiant'. */
  class MODEL_API CoilHeatingSteamBaseboardRadiant : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilHeatingSteamBaseboardRadiant(const Model& model);

    virtual ~CoilHeatingSteamBaseboardRadiant() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilHeatingSteamBaseboardRadiant(const CoilHeatingSteamBaseboardRadiant& other) = default;
    CoilHeatingSteamBaseboardRadiant(CoilHeatingSteamBaseboardRadiant&& other) = default;
    CoilHeatingSteamBaseboardRadiant& operator=(const CoilHeatingSteamBaseboardRadiant&) = default;
    CoilHeatingSteamBaseboardRadiant& operator=(CoilHeatingSteamBaseboardRadiant&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> heatingDesignCapacityMethodValues();

    /** @name Getters */
    //@{

    std::string heatingDesignCapacityMethod() const;

    boost::optional<double> heatingDesignCapacity() const;

    bool isHeatingDesignCapacityAutosized() const;

    double heatingDesignCapacityPerFloorArea() const;

    double fractionofAutosizedHeatingDesignCapacity() const;

    boost::optional<double> maximumSteamFlowRate() const;

    bool isMaximumSteamFlowRateAutosized() const;

    double degreeofSubCooling() const;

    double convergenceTolerance() const;

    //@}
    /** @name Setters */
    //@{

    bool setHeatingDesignCapacityMethod(const std::string& heatingDesignCapacityMethod);

    bool setHeatingDesignCapacity(double heatingDesignCapacity);

    void autosizeHeatingDesignCapacity();

    bool setHeatingDesignCapacityPerFloorArea(double heatingDesignCapacityPerFloorArea);

    bool setFractionofAutosizedHeatingDesignCapacity(double fractionofAutosizedHeatingDesignCapacity);

    bool setMaximumSteamFlowRate(double maximumSteamFlowRate);

    void autosizeMaximumSteamFlowRate();

    bool setDegreeofSubCooling(double degreeofSubCooling);

    bool setConvergenceTolerance(double convergenceTolerance);

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedHeatingDesignCapacity() const;

    boost::optional<double> autosizedMaximumSteamFlowRate() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilHeatingSteamBaseboardRadiant_Impl;

    explicit CoilHeatingSteamBaseboardRadiant(std::shared_ptr<detail::CoilHeatingSteamBaseboardRadiant_Impl> impl);

    friend class detail::CoilHeatingSteamBaseboardRadiant_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilHeatingSteamBaseboardRadiant");
  };

  /** \relates CoilHeatingSteamBaseboardRadiant*/
  using OptionalCoilHeatingSteamBaseboardRadiant = boost::optional<CoilHeatingSteamBaseboardRadiant>;

  /** \relates CoilHeatingSteamBaseboardRadiant*/
  using CoilHeatingSteamBaseboardRadiantVector = std::vector<CoilHeatingSteamBaseboardRadiant>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGSTEAMBASEBOARDRADIANT_HPP
