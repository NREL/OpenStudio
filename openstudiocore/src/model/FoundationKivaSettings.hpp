/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef MODEL_FOUNDATIONKIVASETTINGS_HPP
#define MODEL_FOUNDATIONKIVASETTINGS_HPP

#include <vector>
#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class FoundationKivaSettings_Impl;

} // detail

/** FoundationKivaSettings is a ModelObject that wraps the OpenStudio IDD object 'OS:Foundation:Kiva:Settings'. */
class MODEL_API FoundationKivaSettings : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~FoundationKivaSettings() {}

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
  
  bool setDeepGroundBoundaryCondition(std::string deepGroundBoundaryCondition);
  
  void resetDeepGroundBoundaryCondition();
  
  bool setDeepGroundDepth(double deepGroundDepth);
  
  void autocalculateDeepGroundDepth();
  
  bool setMinimumCellDimension(double minimumCellDimension);
  
  void resetMinimumCellDimension();
  
  bool setMaximumCellGrowthCoefficient(double maximumCellGrowthCoefficient);
  
  void resetMaximumCellGrowthCoefficient();
  
  bool setSimulationTimestep(std::string simulationTimestep);
  
  void resetSimulationTimestep();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
 
  explicit FoundationKivaSettings(Model& model);
 
  /// @cond
  typedef detail::FoundationKivaSettings_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class detail::FoundationKivaSettings_Impl;

  explicit FoundationKivaSettings(std::shared_ptr<detail::FoundationKivaSettings_Impl> impl);

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.FoundationKivaSettings");
};

/** \relates FoundationKivaSettings*/
typedef boost::optional<FoundationKivaSettings> OptionalFoundationKivaSettings;

/** \relates FoundationKivaSettings*/
typedef std::vector<FoundationKivaSettings> FoundationKivaSettingsVector;

} // model
} // openstudio

#endif // MODEL_FOUNDATIONKIVASETTINGS_HPP