/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_FOUNDATIONKIVASETTINGS_IMPL_HPP
#define MODEL_FOUNDATIONKIVASETTINGS_IMPL_HPP

#include <vector>
#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** FoundationKivaSettings_Impl is a ModelObject_Impl that is the implementation class for FoundationKivaSettings.*/
    class MODEL_API FoundationKivaSettings_Impl : public ModelObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      FoundationKivaSettings_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      FoundationKivaSettings_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      FoundationKivaSettings_Impl(const FoundationKivaSettings_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~FoundationKivaSettings_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual boost::optional<ParentObject> parent() const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
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
     private:
      REGISTER_LOGGER("openstudio.model.FoundationKivaSettings");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_FOUNDATIONKIVASETTINGS_IMPL_HPP
