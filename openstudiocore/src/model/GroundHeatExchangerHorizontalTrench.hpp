/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#ifndef MODEL_GROUNDHEATEXCHANGERHORIZONTALTRENCH_HPP
#define MODEL_GROUNDHEATEXCHANGERHORIZONTALTRENCH_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

namespace detail {

  class GroundHeatExchangerHorizontalTrench_Impl;

} // detail

/** GroundHeatExchangerHorizontalTrench is a StraightComponent that wraps the OpenStudio IDD object 'OS:GroundHeatExchanger:HorizontalTrench'. */
class MODEL_API GroundHeatExchangerHorizontalTrench : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit GroundHeatExchangerHorizontalTrench(const Model& model);

  virtual ~GroundHeatExchangerHorizontalTrench() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> groundTemperatureModelValues();

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

  std::string groundTemperatureModel() const;

  bool isGroundTemperatureModelDefaulted() const;

  double kusudaAchenbachAverageSurfaceTemperature() const;

  double kusudaAchenbachAverageAmplitudeofSurfaceTemperature() const;

  double kusudaAchenbachPhaseShiftofMinimumSurfaceTemperature() const;

  double evapotranspirationGroundCoverParameter() const;

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

  bool setGroundTemperatureModel(std::string groundTemperatureModel);

  void resetGroundTemperatureModel();

  void setKusudaAchenbachAverageSurfaceTemperature(double kusudaAchenbachAverageSurfaceTemperature);

  void setKusudaAchenbachAverageAmplitudeofSurfaceTemperature(double kusudaAchenbachAverageAmplitudeofSurfaceTemperature);

  void setKusudaAchenbachPhaseShiftofMinimumSurfaceTemperature(double kusudaAchenbachPhaseShiftofMinimumSurfaceTemperature);

  bool setEvapotranspirationGroundCoverParameter(double evapotranspirationGroundCoverParameter);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::GroundHeatExchangerHorizontalTrench_Impl ImplType;

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
typedef boost::optional<GroundHeatExchangerHorizontalTrench> OptionalGroundHeatExchangerHorizontalTrench;

/** \relates GroundHeatExchangerHorizontalTrench*/
typedef std::vector<GroundHeatExchangerHorizontalTrench> GroundHeatExchangerHorizontalTrenchVector;

} // model
} // openstudio

#endif // MODEL_GROUNDHEATEXCHANGERHORIZONTALTRENCH_HPP

