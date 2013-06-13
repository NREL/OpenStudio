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

//class Quantity;
//class OSOptionalQuantity;

namespace model {

// TODO: Check the following class names against object getters and setters.
//class Connection;
//class Connection;

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

  /** @name Getters */
  //@{

  // TODO: Check return type. From object lists, some candidates are: Connection.
//  Connection inletNode() const;

  // TODO: Check return type. From object lists, some candidates are: Connection.
//  Connection outletNode() const;

  boost::optional<double> maximumFlowRate() const;

//  OSOptionalQuantity getMaximumFlowRate(bool returnIP=false) const;

  boost::optional<int> numberofBoreHoles() const;

  boost::optional<double> boreHoleLength() const;

//  OSOptionalQuantity getBoreHoleLength(bool returnIP=false) const;

  boost::optional<double> boreHoleRadius() const;

//  OSOptionalQuantity getBoreHoleRadius(bool returnIP=false) const;

  boost::optional<double> groundThermalConductivity() const;

//  OSOptionalQuantity getGroundThermalConductivity(bool returnIP=false) const;

  boost::optional<double> groundThermalHeatCapacity() const;

//  OSOptionalQuantity getGroundThermalHeatCapacity(bool returnIP=false) const;

  boost::optional<double> groundTemperature() const;

//  OSOptionalQuantity getGroundTemperature(bool returnIP=false) const;

  boost::optional<double> designFlowRate() const;

//  OSOptionalQuantity getDesignFlowRate(bool returnIP=false) const;

  boost::optional<double> groutThermalConductivity() const;

//  OSOptionalQuantity getGroutThermalConductivity(bool returnIP=false) const;

  boost::optional<double> pipeThermalConductivity() const;

//  OSOptionalQuantity getPipeThermalConductivity(bool returnIP=false) const;

  boost::optional<double> pipeOutDiameter() const;

//  OSOptionalQuantity getPipeOutDiameter(bool returnIP=false) const;

  boost::optional<double> uTubeDistance() const;

//  OSOptionalQuantity getUTubeDistance(bool returnIP=false) const;

  boost::optional<double> pipeThickness() const;

//  OSOptionalQuantity getPipeThickness(bool returnIP=false) const;

  boost::optional<double> maximumLengthofSimulation() const;

//  OSOptionalQuantity getMaximumLengthofSimulation(bool returnIP=false) const;

  double gFunctionReferenceRatio() const;

//  Quantity getGFunctionReferenceRatio(bool returnIP=false) const;

  bool isGFunctionReferenceRatioDefaulted() const;

  boost::optional<int> numberofDataPairsoftheGFunction() const;

  // TODO: Handle this object's extensible fields.

  //@}
  /** @name Setters */

  //@{

  // TODO: Check argument type. From object lists, some candidates are: Connection.
//  bool setInletNode(const Connection& connection);

  // TODO: Check argument type. From object lists, some candidates are: Connection.
//  bool setOutletNode(const Connection& connection);

  bool setMaximumFlowRate(double maximumFlowRate);

//  bool setMaximumFlowRate(const Quantity& maximumFlowRate);

  void resetMaximumFlowRate();

  bool setNumberofBoreHoles(int numberofBoreHoles);

  void resetNumberofBoreHoles();

  bool setBoreHoleLength(double boreHoleLength);

//  bool setBoreHoleLength(const Quantity& boreHoleLength);

  void resetBoreHoleLength();

  bool setBoreHoleRadius(double boreHoleRadius);

//  bool setBoreHoleRadius(const Quantity& boreHoleRadius);

  void resetBoreHoleRadius();

  bool setGroundThermalConductivity(double groundThermalConductivity);

//  bool setGroundThermalConductivity(const Quantity& groundThermalConductivity);

  void resetGroundThermalConductivity();

  bool setGroundThermalHeatCapacity(double groundThermalHeatCapacity);

//  bool setGroundThermalHeatCapacity(const Quantity& groundThermalHeatCapacity);

  void resetGroundThermalHeatCapacity();

  bool setGroundTemperature(double groundTemperature);

//  bool setGroundTemperature(const Quantity& groundTemperature);

  void resetGroundTemperature();

  bool setDesignFlowRate(double designFlowRate);

//  bool setDesignFlowRate(const Quantity& designFlowRate);

  void resetDesignFlowRate();

  bool setGroutThermalConductivity(double groutThermalConductivity);

//  bool setGroutThermalConductivity(const Quantity& groutThermalConductivity);

  void resetGroutThermalConductivity();

  bool setPipeThermalConductivity(double pipeThermalConductivity);

//  bool setPipeThermalConductivity(const Quantity& pipeThermalConductivity);

  void resetPipeThermalConductivity();

  bool setPipeOutDiameter(double pipeOutDiameter);

//  bool setPipeOutDiameter(const Quantity& pipeOutDiameter);

  void resetPipeOutDiameter();

  bool setUTubeDistance(double uTubeDistance);

//  bool setUTubeDistance(const Quantity& uTubeDistance);

  void resetUTubeDistance();

  bool setPipeThickness(double pipeThickness);

//  bool setPipeThickness(const Quantity& pipeThickness);

  void resetPipeThickness();

  bool setMaximumLengthofSimulation(double maximumLengthofSimulation);

//  bool setMaximumLengthofSimulation(const Quantity& maximumLengthofSimulation);

  void resetMaximumLengthofSimulation();

  bool setGFunctionReferenceRatio(double gFunctionReferenceRatio);

//  bool setGFunctionReferenceRatio(const Quantity& gFunctionReferenceRatio);

  void resetGFunctionReferenceRatio();

  bool setNumberofDataPairsoftheGFunction(int numberofDataPairsoftheGFunction);

  void resetNumberofDataPairsoftheGFunction();

  // TODO: Handle this object's extensible fields.

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

