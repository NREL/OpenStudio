/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_GROUNDHEATEXCHANGERHORIZONTALTRENCH_HPP
#define MODEL_GROUNDHEATEXCHANGERHORIZONTALTRENCH_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"
#include "../utilities/core/Deprecated.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class GroundHeatExchangerHorizontalTrench_Impl;

  }  // namespace detail

  /** GroundHeatExchangerHorizontalTrench is a StraightComponent that wraps the OpenStudio IDD object 'OS:GroundHeatExchanger:HorizontalTrench'. */
  class MODEL_API GroundHeatExchangerHorizontalTrench : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit GroundHeatExchangerHorizontalTrench(const Model& model);

    explicit GroundHeatExchangerHorizontalTrench(const Model& model, const ModelObject& undisturbedGroundTemperatureModel);

    virtual ~GroundHeatExchangerHorizontalTrench() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    GroundHeatExchangerHorizontalTrench(const GroundHeatExchangerHorizontalTrench& other) = default;
    GroundHeatExchangerHorizontalTrench(GroundHeatExchangerHorizontalTrench&& other) = default;
    GroundHeatExchangerHorizontalTrench& operator=(const GroundHeatExchangerHorizontalTrench&) = default;
    GroundHeatExchangerHorizontalTrench& operator=(GroundHeatExchangerHorizontalTrench&&) = default;

    //@}

    static IddObjectType iddObjectType();

    OS_DEPRECATED(3, 6, 0) static std::vector<std::string> groundTemperatureModelValues();

    /** @name Getters */
    //@{

    double designFlowRate() const;

    double trenchLengthinPipeAxialDirection() const;

    int numberofTrenches() const;

    double horizontalSpacingBetweenPipes() const;

    double pipeInnerDiameter() const;

    double pipeOuterDiameter() const;

    double burialDepth() const;

    double soilThermalConductivity() const;

    double soilDensity() const;

    double soilSpecificHeat() const;

    double pipeThermalConductivity() const;

    double pipeDensity() const;

    double pipeSpecificHeat() const;

    double soilMoistureContentPercent() const;

    double soilMoistureContentPercentatSaturation() const;

    OS_DEPRECATED(3, 6, 0) std::string groundTemperatureModel() const;

    // cppcheck-suppress functionStatic
    OS_DEPRECATED(3, 6, 0) bool isGroundTemperatureModelDefaulted() const;

    OS_DEPRECATED(3, 6, 0) double kusudaAchenbachAverageSurfaceTemperature() const;

    OS_DEPRECATED(3, 6, 0) double kusudaAchenbachAverageAmplitudeofSurfaceTemperature() const;

    OS_DEPRECATED(3, 6, 0) double kusudaAchenbachPhaseShiftofMinimumSurfaceTemperature() const;

    double evapotranspirationGroundCoverParameter() const;

    ModelObject undisturbedGroundTemperatureModel() const;

    //@}
    /** @name Setters */
    //@{

    bool setDesignFlowRate(double designFlowRate);

    bool setTrenchLengthinPipeAxialDirection(double trenchLengthinPipeAxialDirection);

    bool setNumberofTrenches(int numberofTrenches);

    bool setHorizontalSpacingBetweenPipes(double horizontalSpacingBetweenPipes);

    bool setPipeInnerDiameter(double pipeInnerDiameter);

    bool setPipeOuterDiameter(double pipeOuterDiameter);

    bool setBurialDepth(double burialDepth);

    bool setSoilThermalConductivity(double soilThermalConductivity);

    bool setSoilDensity(double soilDensity);

    bool setSoilSpecificHeat(double soilSpecificHeat);

    bool setPipeThermalConductivity(double pipeThermalConductivity);

    bool setPipeDensity(double pipeDensity);

    bool setPipeSpecificHeat(double pipeSpecificHeat);

    bool setSoilMoistureContentPercent(double soilMoistureContentPercent);

    bool setSoilMoistureContentPercentatSaturation(double soilMoistureContentPercentatSaturation);

    // cppcheck-suppress functionStatic
    OS_DEPRECATED(3, 6, 0) bool setGroundTemperatureModel(const std::string& groundTemperatureModel);

    // cppcheck-suppress functionStatic
    OS_DEPRECATED(3, 6, 0) void resetGroundTemperatureModel();

    OS_DEPRECATED(3, 6, 0) bool setKusudaAchenbachAverageSurfaceTemperature(double kusudaAchenbachAverageSurfaceTemperature);

    OS_DEPRECATED(3, 6, 0) bool setKusudaAchenbachAverageAmplitudeofSurfaceTemperature(double kusudaAchenbachAverageAmplitudeofSurfaceTemperature);

    OS_DEPRECATED(3, 6, 0) bool setKusudaAchenbachPhaseShiftofMinimumSurfaceTemperature(double kusudaAchenbachPhaseShiftofMinimumSurfaceTemperature);

    bool setEvapotranspirationGroundCoverParameter(double evapotranspirationGroundCoverParameter);

    bool setUndisturbedGroundTemperatureModel(const ModelObject& undisturbedGroundTemperatureModel);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::GroundHeatExchangerHorizontalTrench_Impl;

    explicit GroundHeatExchangerHorizontalTrench(std::shared_ptr<detail::GroundHeatExchangerHorizontalTrench_Impl> impl);

    friend class detail::GroundHeatExchangerHorizontalTrench_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.GroundHeatExchangerHorizontalTrench");
  };

  /** \relates GroundHeatExchangerHorizontalTrench*/
  using OptionalGroundHeatExchangerHorizontalTrench = boost::optional<GroundHeatExchangerHorizontalTrench>;

  /** \relates GroundHeatExchangerHorizontalTrench*/
  using GroundHeatExchangerHorizontalTrenchVector = std::vector<GroundHeatExchangerHorizontalTrench>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GROUNDHEATEXCHANGERHORIZONTALTRENCH_HPP
