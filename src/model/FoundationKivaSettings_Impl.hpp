/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

      virtual ~FoundationKivaSettings_Impl() = default;

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
