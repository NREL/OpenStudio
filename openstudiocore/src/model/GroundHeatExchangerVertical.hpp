/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#ifndef MODEL_GROUNDHEATEXCHANGERVERTICAL_HPP
#define MODEL_GROUNDHEATEXCHANGERVERTICAL_HPP

#include <model/ModelAPI.hpp>
#include <model/StraightComponent.hpp>

namespace openstudio {
namespace model {
namespace detail {

  class GroundHeatExchangerVertical_Impl;

} // detail

/** GroundHeatExchangerVertical is a StraightComponent that wraps the OpenStudio IDD object 'OS:GroundHeatExchanger:Vertical'. */
class MODEL_API GroundHeatExchangerVertical : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit GroundHeatExchangerVertical(const Model& model);

  virtual ~GroundHeatExchangerVertical() {}

  //@}

  static IddObjectType iddObjectType();

  bool addGFunction(double gFunctionLN, double gFunctionGValue);

  void removeGFunction(int groupIndex);

  void removeAllGFunctions();

  std::vector< std::pair<double,double> > gFunctions();

  /** @name Getters */
  //@{

  boost::optional<double> maximumFlowRate() const;

  boost::optional<int> numberofBoreHoles() const;

  boost::optional<double> boreHoleLength() const;

  boost::optional<double> boreHoleRadius() const;

  boost::optional<double> groundThermalConductivity() const;

  boost::optional<double> groundThermalHeatCapacity() const;

  boost::optional<double> groundTemperature() const;

  boost::optional<double> designFlowRate() const;

  boost::optional<double> groutThermalConductivity() const;

  boost::optional<double> pipeThermalConductivity() const;

  boost::optional<double> pipeOutDiameter() const;

  boost::optional<double> uTubeDistance() const;

  boost::optional<double> pipeThickness() const;

  boost::optional<double> maximumLengthofSimulation() const;

  double gFunctionReferenceRatio() const;

  bool isGFunctionReferenceRatioDefaulted() const;

  //@}
  /** @name Setters */

  //@{

  bool setMaximumFlowRate(double maximumFlowRate);

  void resetMaximumFlowRate();

  bool setNumberofBoreHoles(int numberofBoreHoles);

  void resetNumberofBoreHoles();

  bool setBoreHoleLength(double boreHoleLength);

  void resetBoreHoleLength();

  bool setBoreHoleRadius(double boreHoleRadius);

  void resetBoreHoleRadius();

  bool setGroundThermalConductivity(double groundThermalConductivity);

  void resetGroundThermalConductivity();

  bool setGroundThermalHeatCapacity(double groundThermalHeatCapacity);

  void resetGroundThermalHeatCapacity();

  bool setGroundTemperature(double groundTemperature);

  void resetGroundTemperature();

  bool setDesignFlowRate(double designFlowRate);

  void resetDesignFlowRate();

  bool setGroutThermalConductivity(double groutThermalConductivity);

  void resetGroutThermalConductivity();

  bool setPipeThermalConductivity(double pipeThermalConductivity);

  void resetPipeThermalConductivity();

  bool setPipeOutDiameter(double pipeOutDiameter);

  void resetPipeOutDiameter();

  bool setUTubeDistance(double uTubeDistance);

  void resetUTubeDistance();

  bool setPipeThickness(double pipeThickness);

  void resetPipeThickness();

  bool setMaximumLengthofSimulation(double maximumLengthofSimulation);

  void resetMaximumLengthofSimulation();

  bool setGFunctionReferenceRatio(double gFunctionReferenceRatio);

  void resetGFunctionReferenceRatio();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::GroundHeatExchangerVertical_Impl ImplType;

  explicit GroundHeatExchangerVertical(boost::shared_ptr<detail::GroundHeatExchangerVertical_Impl> impl);

  friend class detail::GroundHeatExchangerVertical_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.GroundHeatExchangerVertical");
};

/** \relates GroundHeatExchangerVertical*/
typedef boost::optional<GroundHeatExchangerVertical> OptionalGroundHeatExchangerVertical;

/** \relates GroundHeatExchangerVertical*/
typedef std::vector<GroundHeatExchangerVertical> GroundHeatExchangerVerticalVector;

} // model
} // openstudio

#endif // MODEL_GROUNDHEATEXCHANGERVERTICAL_HPP

