/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_FOUNDATIONKIVASETTINGS_HPP
#define MODEL_FOUNDATIONKIVASETTINGS_HPP

#include <vector>
#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class FoundationKivaSettings_Impl;

  }  // namespace detail

  /** FoundationKivaSettings is a ModelObject that wraps the OpenStudio IDD object 'OS:Foundation:Kiva:Settings'. */
  class MODEL_API FoundationKivaSettings : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~FoundationKivaSettings() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    FoundationKivaSettings(const FoundationKivaSettings& other) = default;
    FoundationKivaSettings(FoundationKivaSettings&& other) = default;
    FoundationKivaSettings& operator=(const FoundationKivaSettings&) = default;
    FoundationKivaSettings& operator=(FoundationKivaSettings&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    double soilConductivity() const;

    bool isSoilConductivityDefaulted() const;

    double soilDensity() const;

    bool isSoilDensityDefaulted() const;

    double soilSpecificHeat() const;

    bool isSoilSpecificHeatDefaulted() const;

    double groundSolarAbsorptivity() const;

    bool isGroundSolarAbsorptivityDefaulted() const;

    double groundThermalAbsorptivity() const;

    bool isGroundThermalAbsorptivityDefaulted() const;

    double groundSurfaceRoughness() const;

    bool isGroundSurfaceRoughnessDefaulted() const;

    double farFieldWidth() const;

    bool isFarFieldWidthDefaulted() const;

    std::string deepGroundBoundaryCondition() const;

    bool isDeepGroundBoundaryConditionAutoselected();

    boost::optional<double> deepGroundDepth();

    bool isDeepGroundDepthAutocalculated();

    double minimumCellDimension() const;

    bool isMinimumCellDimensionDefaulted() const;

    double maximumCellGrowthCoefficient() const;

    bool isMaximumCellGrowthCoefficientDefaulted() const;

    std::string simulationTimestep() const;

    bool isSimulationTimestepDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setSoilConductivity(double soilConductivity);

    void resetSoilConductivity();

    bool setSoilDensity(double soilDensity);

    void resetSoilDensity();

    bool setSoilSpecificHeat(double soilSpecificHeat);

    void resetSoilSpecificHeat();

    bool setGroundSolarAbsorptivity(double groundSolarAbsorptivity);

    void resetGroundSolarAbsorptivity();

    bool setGroundThermalAbsorptivity(double groundThermalAbsorptivity);

    void resetGroundThermalAbsorptivity();

    bool setGroundSurfaceRoughness(double groundSurfaceRoughness);

    void resetGroundSurfaceRoughness();

    bool setFarFieldWidth(double farFieldWidth);

    void resetFarFieldWidth();

    bool setDeepGroundBoundaryCondition(const std::string& deepGroundBoundaryCondition);

    void resetDeepGroundBoundaryCondition();

    bool setDeepGroundDepth(double deepGroundDepth);

    void autocalculateDeepGroundDepth();

    bool setMinimumCellDimension(double minimumCellDimension);

    void resetMinimumCellDimension();

    bool setMaximumCellGrowthCoefficient(double maximumCellGrowthCoefficient);

    void resetMaximumCellGrowthCoefficient();

    bool setSimulationTimestep(const std::string& simulationTimestep);

    void resetSimulationTimestep();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    explicit FoundationKivaSettings(Model& model);

    /// @cond
    using ImplType = detail::FoundationKivaSettings_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class detail::FoundationKivaSettings_Impl;

    explicit FoundationKivaSettings(std::shared_ptr<detail::FoundationKivaSettings_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.FoundationKivaSettings");
  };

  /** \relates FoundationKivaSettings*/
  using OptionalFoundationKivaSettings = boost::optional<FoundationKivaSettings>;

  /** \relates FoundationKivaSettings*/
  using FoundationKivaSettingsVector = std::vector<FoundationKivaSettings>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_FOUNDATIONKIVASETTINGS_HPP
