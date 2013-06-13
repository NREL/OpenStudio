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

#include <model/GroundHeatExchangerVertical.hpp>
#include <model/GroundHeatExchangerVertical_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/Model.hpp>
#include <model/Model_Impl.hpp>

// TODO: Check the following class names against object getters and setters.
//#include <model/Connection.hpp>
//#include <model/Connection_Impl.hpp>
//#include <model/Connection.hpp>
//#include <model/Connection_Impl.hpp>

#include <utilities/idd/OS_GroundHeatExchanger_Vertical_FieldEnums.hxx>

#include <utilities/units/Unit.hpp>

#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace model {

namespace detail {

  GroundHeatExchangerVertical_Impl::GroundHeatExchangerVertical_Impl(const IdfObject& idfObject,
                                                                     Model_Impl* model,
                                                                     bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    BOOST_ASSERT(idfObject.iddObject().type() == GroundHeatExchangerVertical::iddObjectType());
  }

  GroundHeatExchangerVertical_Impl::GroundHeatExchangerVertical_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                     Model_Impl* model,
                                                                     bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    BOOST_ASSERT(other.iddObject().type() == GroundHeatExchangerVertical::iddObjectType());
  }

  GroundHeatExchangerVertical_Impl::GroundHeatExchangerVertical_Impl(const GroundHeatExchangerVertical_Impl& other,
                                                                     Model_Impl* model,
                                                                     bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& GroundHeatExchangerVertical_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType GroundHeatExchangerVertical_Impl::iddObjectType() const {
    return GroundHeatExchangerVertical::iddObjectType();
  }

//  Connection GroundHeatExchangerVertical_Impl::inletNode() const {
//    boost::optional<Connection> value = optionalInletNode();
//    if (!value) {
//      LOG_AND_THROW(briefDescription() << " does not have an Inlet Node attached.");
//    }
//    return value.get();
//  }

//  Connection GroundHeatExchangerVertical_Impl::outletNode() const {
//    boost::optional<Connection> value = optionalOutletNode();
//    if (!value) {
//      LOG_AND_THROW(briefDescription() << " does not have an Outlet Node attached.");
//    }
//    return value.get();
//  }

  boost::optional<double> GroundHeatExchangerVertical_Impl::maximumFlowRate() const {
    return getDouble(OS_GroundHeatExchanger_VerticalFields::MaximumFlowRate,true);
  }

//  OSOptionalQuantity GroundHeatExchangerVertical_Impl::getMaximumFlowRate(bool returnIP) const {
//    OptionalDouble value = maximumFlowRate();
//    return getQuantityFromDouble(OS_GroundHeatExchanger_VerticalFields::MaximumFlowRate, value, returnIP);
//  }

  boost::optional<int> GroundHeatExchangerVertical_Impl::numberofBoreHoles() const {
    return getInt(OS_GroundHeatExchanger_VerticalFields::NumberofBoreHoles,true);
  }

  boost::optional<double> GroundHeatExchangerVertical_Impl::boreHoleLength() const {
    return getDouble(OS_GroundHeatExchanger_VerticalFields::BoreHoleLength,true);
  }

//  OSOptionalQuantity GroundHeatExchangerVertical_Impl::getBoreHoleLength(bool returnIP) const {
//    OptionalDouble value = boreHoleLength();
//    return getQuantityFromDouble(OS_GroundHeatExchanger_VerticalFields::BoreHoleLength, value, returnIP);
//  }

  boost::optional<double> GroundHeatExchangerVertical_Impl::boreHoleRadius() const {
    return getDouble(OS_GroundHeatExchanger_VerticalFields::BoreHoleRadius,true);
  }

//  OSOptionalQuantity GroundHeatExchangerVertical_Impl::getBoreHoleRadius(bool returnIP) const {
//    OptionalDouble value = boreHoleRadius();
//    return getQuantityFromDouble(OS_GroundHeatExchanger_VerticalFields::BoreHoleRadius, value, returnIP);
//  }

  boost::optional<double> GroundHeatExchangerVertical_Impl::groundThermalConductivity() const {
    return getDouble(OS_GroundHeatExchanger_VerticalFields::GroundThermalConductivity,true);
  }

//  OSOptionalQuantity GroundHeatExchangerVertical_Impl::getGroundThermalConductivity(bool returnIP) const {
//    OptionalDouble value = groundThermalConductivity();
//    return getQuantityFromDouble(OS_GroundHeatExchanger_VerticalFields::GroundThermalConductivity, value, returnIP);
//  }

  boost::optional<double> GroundHeatExchangerVertical_Impl::groundThermalHeatCapacity() const {
    return getDouble(OS_GroundHeatExchanger_VerticalFields::GroundThermalHeatCapacity,true);
  }

//  OSOptionalQuantity GroundHeatExchangerVertical_Impl::getGroundThermalHeatCapacity(bool returnIP) const {
//    OptionalDouble value = groundThermalHeatCapacity();
//    return getQuantityFromDouble(OS_GroundHeatExchanger_VerticalFields::GroundThermalHeatCapacity, value, returnIP);
//  }

  boost::optional<double> GroundHeatExchangerVertical_Impl::groundTemperature() const {
    return getDouble(OS_GroundHeatExchanger_VerticalFields::GroundTemperature,true);
  }

//  OSOptionalQuantity GroundHeatExchangerVertical_Impl::getGroundTemperature(bool returnIP) const {
//    OptionalDouble value = groundTemperature();
//    return getQuantityFromDouble(OS_GroundHeatExchanger_VerticalFields::GroundTemperature, value, returnIP);
//  }

  boost::optional<double> GroundHeatExchangerVertical_Impl::designFlowRate() const {
    return getDouble(OS_GroundHeatExchanger_VerticalFields::DesignFlowRate,true);
  }

//  OSOptionalQuantity GroundHeatExchangerVertical_Impl::getDesignFlowRate(bool returnIP) const {
//    OptionalDouble value = designFlowRate();
//    return getQuantityFromDouble(OS_GroundHeatExchanger_VerticalFields::DesignFlowRate, value, returnIP);
//  }

  boost::optional<double> GroundHeatExchangerVertical_Impl::groutThermalConductivity() const {
    return getDouble(OS_GroundHeatExchanger_VerticalFields::GroutThermalConductivity,true);
  }

//  OSOptionalQuantity GroundHeatExchangerVertical_Impl::getGroutThermalConductivity(bool returnIP) const {
//    OptionalDouble value = groutThermalConductivity();
//    return getQuantityFromDouble(OS_GroundHeatExchanger_VerticalFields::GroutThermalConductivity, value, returnIP);
//  }

  boost::optional<double> GroundHeatExchangerVertical_Impl::pipeThermalConductivity() const {
    return getDouble(OS_GroundHeatExchanger_VerticalFields::PipeThermalConductivity,true);
  }

//  OSOptionalQuantity GroundHeatExchangerVertical_Impl::getPipeThermalConductivity(bool returnIP) const {
//    OptionalDouble value = pipeThermalConductivity();
//    return getQuantityFromDouble(OS_GroundHeatExchanger_VerticalFields::PipeThermalConductivity, value, returnIP);
//  }

  boost::optional<double> GroundHeatExchangerVertical_Impl::pipeOutDiameter() const {
    return getDouble(OS_GroundHeatExchanger_VerticalFields::PipeOutDiameter,true);
  }

//  OSOptionalQuantity GroundHeatExchangerVertical_Impl::getPipeOutDiameter(bool returnIP) const {
//    OptionalDouble value = pipeOutDiameter();
//    return getQuantityFromDouble(OS_GroundHeatExchanger_VerticalFields::PipeOutDiameter, value, returnIP);
//  }

  boost::optional<double> GroundHeatExchangerVertical_Impl::uTubeDistance() const {
    return getDouble(OS_GroundHeatExchanger_VerticalFields::UTubeDistance,true);
  }

//  OSOptionalQuantity GroundHeatExchangerVertical_Impl::getUTubeDistance(bool returnIP) const {
//    OptionalDouble value = uTubeDistance();
//    return getQuantityFromDouble(OS_GroundHeatExchanger_VerticalFields::UTubeDistance, value, returnIP);
//  }

  boost::optional<double> GroundHeatExchangerVertical_Impl::pipeThickness() const {
    return getDouble(OS_GroundHeatExchanger_VerticalFields::PipeThickness,true);
  }

//  OSOptionalQuantity GroundHeatExchangerVertical_Impl::getPipeThickness(bool returnIP) const {
//    OptionalDouble value = pipeThickness();
//    return getQuantityFromDouble(OS_GroundHeatExchanger_VerticalFields::PipeThickness, value, returnIP);
//  }

  boost::optional<double> GroundHeatExchangerVertical_Impl::maximumLengthofSimulation() const {
    return getDouble(OS_GroundHeatExchanger_VerticalFields::MaximumLengthofSimulation,true);
  }

//  OSOptionalQuantity GroundHeatExchangerVertical_Impl::getMaximumLengthofSimulation(bool returnIP) const {
//    OptionalDouble value = maximumLengthofSimulation();
//    return getQuantityFromDouble(OS_GroundHeatExchanger_VerticalFields::MaximumLengthofSimulation, value, returnIP);
//  }

  double GroundHeatExchangerVertical_Impl::gFunctionReferenceRatio() const {
    boost::optional<double> value = getDouble(OS_GroundHeatExchanger_VerticalFields::GFunctionReferenceRatio,true);
    BOOST_ASSERT(value);
    return value.get();
  }

//  Quantity GroundHeatExchangerVertical_Impl::getGFunctionReferenceRatio(bool returnIP) const {
//    OptionalDouble value = gFunctionReferenceRatio();
//    OSOptionalQuantity result = getQuantityFromDouble(OS_GroundHeatExchanger_VerticalFields::GFunctionReferenceRatio, value, returnIP);
//    BOOST_ASSERT(result.isSet());
//    return result.get();
//  }

  bool GroundHeatExchangerVertical_Impl::isGFunctionReferenceRatioDefaulted() const {
    return isEmpty(OS_GroundHeatExchanger_VerticalFields::GFunctionReferenceRatio);
  }

  boost::optional<int> GroundHeatExchangerVertical_Impl::numberofDataPairsoftheGFunction() const {
    return getInt(OS_GroundHeatExchanger_VerticalFields::NumberofDataPairsoftheGFunction,true);
  }

//  bool GroundHeatExchangerVertical_Impl::setInletNode(const Connection& connection) {
//    bool result = setPointer(OS_GroundHeatExchanger_VerticalFields::InletNodeName, connection.handle());
//    return result;
//  }

//  bool GroundHeatExchangerVertical_Impl::setOutletNode(const Connection& connection) {
//    bool result = setPointer(OS_GroundHeatExchanger_VerticalFields::OutletNodeName, connection.handle());
//    return result;
//  }

  bool GroundHeatExchangerVertical_Impl::setMaximumFlowRate(boost::optional<double> maximumFlowRate) {
    bool result(false);
    if (maximumFlowRate) {
      result = setDouble(OS_GroundHeatExchanger_VerticalFields::MaximumFlowRate, maximumFlowRate.get());
    }
    else {
      resetMaximumFlowRate();
      result = true;
    }
    return result;
  }

//  bool GroundHeatExchangerVertical_Impl::setMaximumFlowRate(const OSOptionalQuantity& maximumFlowRate) {
//    bool result(false);
//    OptionalDouble value;
//    if (maximumFlowRate.isSet()) {
//      value = getDoubleFromQuantity(OS_GroundHeatExchanger_VerticalFields::MaximumFlowRate,maximumFlowRate.get());
//      if (value) {
//        result = setMaximumFlowRate(value);
//      }
//    }
//    else {
//      result = setMaximumFlowRate(value);
//    }
//    return result;
//  }

  void GroundHeatExchangerVertical_Impl::resetMaximumFlowRate() {
    bool result = setString(OS_GroundHeatExchanger_VerticalFields::MaximumFlowRate, "");
    BOOST_ASSERT(result);
  }

  bool GroundHeatExchangerVertical_Impl::setNumberofBoreHoles(boost::optional<int> numberofBoreHoles) {
    bool result(false);
    if (numberofBoreHoles) {
      result = setInt(OS_GroundHeatExchanger_VerticalFields::NumberofBoreHoles, numberofBoreHoles.get());
    }
    else {
      resetNumberofBoreHoles();
      result = true;
    }
    return result;
  }

  void GroundHeatExchangerVertical_Impl::resetNumberofBoreHoles() {
    bool result = setString(OS_GroundHeatExchanger_VerticalFields::NumberofBoreHoles, "");
    BOOST_ASSERT(result);
  }

  bool GroundHeatExchangerVertical_Impl::setBoreHoleLength(boost::optional<double> boreHoleLength) {
    bool result(false);
    if (boreHoleLength) {
      result = setDouble(OS_GroundHeatExchanger_VerticalFields::BoreHoleLength, boreHoleLength.get());
    }
    else {
      resetBoreHoleLength();
      result = true;
    }
    return result;
  }

//  bool GroundHeatExchangerVertical_Impl::setBoreHoleLength(const OSOptionalQuantity& boreHoleLength) {
//    bool result(false);
//    OptionalDouble value;
//    if (boreHoleLength.isSet()) {
//      value = getDoubleFromQuantity(OS_GroundHeatExchanger_VerticalFields::BoreHoleLength,boreHoleLength.get());
//      if (value) {
//        result = setBoreHoleLength(value);
//      }
//    }
//    else {
//      result = setBoreHoleLength(value);
//    }
//    return result;
//  }

  void GroundHeatExchangerVertical_Impl::resetBoreHoleLength() {
    bool result = setString(OS_GroundHeatExchanger_VerticalFields::BoreHoleLength, "");
    BOOST_ASSERT(result);
  }

  bool GroundHeatExchangerVertical_Impl::setBoreHoleRadius(boost::optional<double> boreHoleRadius) {
    bool result(false);
    if (boreHoleRadius) {
      result = setDouble(OS_GroundHeatExchanger_VerticalFields::BoreHoleRadius, boreHoleRadius.get());
    }
    else {
      resetBoreHoleRadius();
      result = true;
    }
    return result;
  }

//  bool GroundHeatExchangerVertical_Impl::setBoreHoleRadius(const OSOptionalQuantity& boreHoleRadius) {
//    bool result(false);
//    OptionalDouble value;
//    if (boreHoleRadius.isSet()) {
//      value = getDoubleFromQuantity(OS_GroundHeatExchanger_VerticalFields::BoreHoleRadius,boreHoleRadius.get());
//      if (value) {
//        result = setBoreHoleRadius(value);
//      }
//    }
//    else {
//      result = setBoreHoleRadius(value);
//    }
//    return result;
//  }

  void GroundHeatExchangerVertical_Impl::resetBoreHoleRadius() {
    bool result = setString(OS_GroundHeatExchanger_VerticalFields::BoreHoleRadius, "");
    BOOST_ASSERT(result);
  }

  bool GroundHeatExchangerVertical_Impl::setGroundThermalConductivity(boost::optional<double> groundThermalConductivity) {
    bool result(false);
    if (groundThermalConductivity) {
      result = setDouble(OS_GroundHeatExchanger_VerticalFields::GroundThermalConductivity, groundThermalConductivity.get());
    }
    else {
      resetGroundThermalConductivity();
      result = true;
    }
    return result;
  }

//  bool GroundHeatExchangerVertical_Impl::setGroundThermalConductivity(const OSOptionalQuantity& groundThermalConductivity) {
//    bool result(false);
//    OptionalDouble value;
//    if (groundThermalConductivity.isSet()) {
//      value = getDoubleFromQuantity(OS_GroundHeatExchanger_VerticalFields::GroundThermalConductivity,groundThermalConductivity.get());
//      if (value) {
//        result = setGroundThermalConductivity(value);
//      }
//    }
//    else {
//      result = setGroundThermalConductivity(value);
//    }
//    return result;
//  }

  void GroundHeatExchangerVertical_Impl::resetGroundThermalConductivity() {
    bool result = setString(OS_GroundHeatExchanger_VerticalFields::GroundThermalConductivity, "");
    BOOST_ASSERT(result);
  }

  bool GroundHeatExchangerVertical_Impl::setGroundThermalHeatCapacity(boost::optional<double> groundThermalHeatCapacity) {
    bool result(false);
    if (groundThermalHeatCapacity) {
      result = setDouble(OS_GroundHeatExchanger_VerticalFields::GroundThermalHeatCapacity, groundThermalHeatCapacity.get());
    }
    else {
      resetGroundThermalHeatCapacity();
      result = true;
    }
    return result;
  }

//  bool GroundHeatExchangerVertical_Impl::setGroundThermalHeatCapacity(const OSOptionalQuantity& groundThermalHeatCapacity) {
//    bool result(false);
//    OptionalDouble value;
//    if (groundThermalHeatCapacity.isSet()) {
//      value = getDoubleFromQuantity(OS_GroundHeatExchanger_VerticalFields::GroundThermalHeatCapacity,groundThermalHeatCapacity.get());
//      if (value) {
//        result = setGroundThermalHeatCapacity(value);
//      }
//    }
//    else {
//      result = setGroundThermalHeatCapacity(value);
//    }
//    return result;
//  }

  void GroundHeatExchangerVertical_Impl::resetGroundThermalHeatCapacity() {
    bool result = setString(OS_GroundHeatExchanger_VerticalFields::GroundThermalHeatCapacity, "");
    BOOST_ASSERT(result);
  }

  bool GroundHeatExchangerVertical_Impl::setGroundTemperature(boost::optional<double> groundTemperature) {
    bool result(false);
    if (groundTemperature) {
      result = setDouble(OS_GroundHeatExchanger_VerticalFields::GroundTemperature, groundTemperature.get());
    }
    else {
      resetGroundTemperature();
      result = true;
    }
    return result;
  }

//  bool GroundHeatExchangerVertical_Impl::setGroundTemperature(const OSOptionalQuantity& groundTemperature) {
//    bool result(false);
//    OptionalDouble value;
//    if (groundTemperature.isSet()) {
//      value = getDoubleFromQuantity(OS_GroundHeatExchanger_VerticalFields::GroundTemperature,groundTemperature.get());
//      if (value) {
//        result = setGroundTemperature(value);
//      }
//    }
//    else {
//      result = setGroundTemperature(value);
//    }
//    return result;
//  }

  void GroundHeatExchangerVertical_Impl::resetGroundTemperature() {
    bool result = setString(OS_GroundHeatExchanger_VerticalFields::GroundTemperature, "");
    BOOST_ASSERT(result);
  }

  bool GroundHeatExchangerVertical_Impl::setDesignFlowRate(boost::optional<double> designFlowRate) {
    bool result(false);
    if (designFlowRate) {
      result = setDouble(OS_GroundHeatExchanger_VerticalFields::DesignFlowRate, designFlowRate.get());
    }
    else {
      resetDesignFlowRate();
      result = true;
    }
    return result;
  }

//  bool GroundHeatExchangerVertical_Impl::setDesignFlowRate(const OSOptionalQuantity& designFlowRate) {
//    bool result(false);
//    OptionalDouble value;
//    if (designFlowRate.isSet()) {
//      value = getDoubleFromQuantity(OS_GroundHeatExchanger_VerticalFields::DesignFlowRate,designFlowRate.get());
//      if (value) {
//        result = setDesignFlowRate(value);
//      }
//    }
//    else {
//      result = setDesignFlowRate(value);
//    }
//    return result;
//  }

  void GroundHeatExchangerVertical_Impl::resetDesignFlowRate() {
    bool result = setString(OS_GroundHeatExchanger_VerticalFields::DesignFlowRate, "");
    BOOST_ASSERT(result);
  }

  bool GroundHeatExchangerVertical_Impl::setGroutThermalConductivity(boost::optional<double> groutThermalConductivity) {
    bool result(false);
    if (groutThermalConductivity) {
      result = setDouble(OS_GroundHeatExchanger_VerticalFields::GroutThermalConductivity, groutThermalConductivity.get());
    }
    else {
      resetGroutThermalConductivity();
      result = true;
    }
    return result;
  }

//  bool GroundHeatExchangerVertical_Impl::setGroutThermalConductivity(const OSOptionalQuantity& groutThermalConductivity) {
//    bool result(false);
//    OptionalDouble value;
//    if (groutThermalConductivity.isSet()) {
//      value = getDoubleFromQuantity(OS_GroundHeatExchanger_VerticalFields::GroutThermalConductivity,groutThermalConductivity.get());
//      if (value) {
//        result = setGroutThermalConductivity(value);
//      }
//    }
//    else {
//      result = setGroutThermalConductivity(value);
//    }
//    return result;
//  }

  void GroundHeatExchangerVertical_Impl::resetGroutThermalConductivity() {
    bool result = setString(OS_GroundHeatExchanger_VerticalFields::GroutThermalConductivity, "");
    BOOST_ASSERT(result);
  }

  bool GroundHeatExchangerVertical_Impl::setPipeThermalConductivity(boost::optional<double> pipeThermalConductivity) {
    bool result(false);
    if (pipeThermalConductivity) {
      result = setDouble(OS_GroundHeatExchanger_VerticalFields::PipeThermalConductivity, pipeThermalConductivity.get());
    }
    else {
      resetPipeThermalConductivity();
      result = true;
    }
    return result;
  }

//  bool GroundHeatExchangerVertical_Impl::setPipeThermalConductivity(const OSOptionalQuantity& pipeThermalConductivity) {
//    bool result(false);
//    OptionalDouble value;
//    if (pipeThermalConductivity.isSet()) {
//      value = getDoubleFromQuantity(OS_GroundHeatExchanger_VerticalFields::PipeThermalConductivity,pipeThermalConductivity.get());
//      if (value) {
//        result = setPipeThermalConductivity(value);
//      }
 //   }
//    else {
//      result = setPipeThermalConductivity(value);
//    }
//    return result;
//  }

  void GroundHeatExchangerVertical_Impl::resetPipeThermalConductivity() {
    bool result = setString(OS_GroundHeatExchanger_VerticalFields::PipeThermalConductivity, "");
    BOOST_ASSERT(result);
  }

  bool GroundHeatExchangerVertical_Impl::setPipeOutDiameter(boost::optional<double> pipeOutDiameter) {
    bool result(false);
    if (pipeOutDiameter) {
      result = setDouble(OS_GroundHeatExchanger_VerticalFields::PipeOutDiameter, pipeOutDiameter.get());
    }
    else {
      resetPipeOutDiameter();
      result = true;
    }
    return result;
  }

//  bool GroundHeatExchangerVertical_Impl::setPipeOutDiameter(const OSOptionalQuantity& pipeOutDiameter) {
//    bool result(false);
//    OptionalDouble value;
//    if (pipeOutDiameter.isSet()) {
//      value = getDoubleFromQuantity(OS_GroundHeatExchanger_VerticalFields::PipeOutDiameter,pipeOutDiameter.get());
//      if (value) {
//        result = setPipeOutDiameter(value);
//      }
//    }
//    else {
//      result = setPipeOutDiameter(value);
//    }
//    return result;
//  }

  void GroundHeatExchangerVertical_Impl::resetPipeOutDiameter() {
    bool result = setString(OS_GroundHeatExchanger_VerticalFields::PipeOutDiameter, "");
    BOOST_ASSERT(result);
  }

  bool GroundHeatExchangerVertical_Impl::setUTubeDistance(boost::optional<double> uTubeDistance) {
    bool result(false);
    if (uTubeDistance) {
      result = setDouble(OS_GroundHeatExchanger_VerticalFields::UTubeDistance, uTubeDistance.get());
    }
    else {
      resetUTubeDistance();
      result = true;
    }
    return result;
  }

//  bool GroundHeatExchangerVertical_Impl::setUTubeDistance(const OSOptionalQuantity& uTubeDistance) {
//    bool result(false);
//    OptionalDouble value;
//    if (uTubeDistance.isSet()) {
//      value = getDoubleFromQuantity(OS_GroundHeatExchanger_VerticalFields::UTubeDistance,uTubeDistance.get());
//      if (value) {
//        result = setUTubeDistance(value);
//      }
//    }
//    else {
//      result = setUTubeDistance(value);
//    }
//    return result;
//  }

  void GroundHeatExchangerVertical_Impl::resetUTubeDistance() {
    bool result = setString(OS_GroundHeatExchanger_VerticalFields::UTubeDistance, "");
    BOOST_ASSERT(result);
  }

  bool GroundHeatExchangerVertical_Impl::setPipeThickness(boost::optional<double> pipeThickness) {
    bool result(false);
    if (pipeThickness) {
      result = setDouble(OS_GroundHeatExchanger_VerticalFields::PipeThickness, pipeThickness.get());
    }
    else {
      resetPipeThickness();
      result = true;
    }
    return result;
  }

//  bool GroundHeatExchangerVertical_Impl::setPipeThickness(const OSOptionalQuantity& pipeThickness) {
//    bool result(false);
  //  OptionalDouble value;
  //  if (pipeThickness.isSet()) {
  //    value = getDoubleFromQuantity(OS_GroundHeatExchanger_VerticalFields::PipeThickness,pipeThickness.get());
  //    if (value) {
  //      result = setPipeThickness(value);
  //    }
  //  }
  //  else {
  //    result = setPipeThickness(value);
  //  }
  //  return result;
  //}

  void GroundHeatExchangerVertical_Impl::resetPipeThickness() {
    bool result = setString(OS_GroundHeatExchanger_VerticalFields::PipeThickness, "");
    BOOST_ASSERT(result);
  }

  bool GroundHeatExchangerVertical_Impl::setMaximumLengthofSimulation(boost::optional<double> maximumLengthofSimulation) {
    bool result(false);
    if (maximumLengthofSimulation) {
      result = setDouble(OS_GroundHeatExchanger_VerticalFields::MaximumLengthofSimulation, maximumLengthofSimulation.get());
    }
    else {
      resetMaximumLengthofSimulation();
      result = true;
    }
    return result;
  }

  //bool GroundHeatExchangerVertical_Impl::setMaximumLengthofSimulation(const OSOptionalQuantity& maximumLengthofSimulation) {
  //  bool result(false);
  //  OptionalDouble value;
  //  if (maximumLengthofSimulation.isSet()) {
  //    value = getDoubleFromQuantity(OS_GroundHeatExchanger_VerticalFields::MaximumLengthofSimulation,maximumLengthofSimulation.get());
  //    if (value) {
  //      result = setMaximumLengthofSimulation(value);
  //    }
  //  }
  //  else {
  //    result = setMaximumLengthofSimulation(value);
  //  }
  //  return result;
  //}

  void GroundHeatExchangerVertical_Impl::resetMaximumLengthofSimulation() {
    bool result = setString(OS_GroundHeatExchanger_VerticalFields::MaximumLengthofSimulation, "");
    BOOST_ASSERT(result);
  }

  bool GroundHeatExchangerVertical_Impl::setGFunctionReferenceRatio(double gFunctionReferenceRatio) {
    bool result = setDouble(OS_GroundHeatExchanger_VerticalFields::GFunctionReferenceRatio, gFunctionReferenceRatio);
    return result;
  }

  //bool GroundHeatExchangerVertical_Impl::setGFunctionReferenceRatio(const Quantity& gFunctionReferenceRatio) {
  //  OptionalDouble value = getDoubleFromQuantity(OS_GroundHeatExchanger_VerticalFields::GFunctionReferenceRatio,gFunctionReferenceRatio);
  //  if (!value) {
  //    return false;
  //  }
  //  return setGFunctionReferenceRatio(value.get());
  //}

  void GroundHeatExchangerVertical_Impl::resetGFunctionReferenceRatio() {
    bool result = setString(OS_GroundHeatExchanger_VerticalFields::GFunctionReferenceRatio, "");
    BOOST_ASSERT(result);
  }

  bool GroundHeatExchangerVertical_Impl::setNumberofDataPairsoftheGFunction(boost::optional<int> numberofDataPairsoftheGFunction) {
    bool result(false);
    if (numberofDataPairsoftheGFunction) {
      result = setInt(OS_GroundHeatExchanger_VerticalFields::NumberofDataPairsoftheGFunction, numberofDataPairsoftheGFunction.get());
    }
    else {
      resetNumberofDataPairsoftheGFunction();
      result = true;
    }
    return result;
  }

  void GroundHeatExchangerVertical_Impl::resetNumberofDataPairsoftheGFunction() {
    bool result = setString(OS_GroundHeatExchanger_VerticalFields::NumberofDataPairsoftheGFunction, "");
    BOOST_ASSERT(result);
  }

//  boost::optional<Connection> GroundHeatExchangerVertical_Impl::optionalInletNode() const {
//    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_GroundHeatExchanger_VerticalFields::InletNodeName);
//  }

//  boost::optional<Connection> GroundHeatExchangerVertical_Impl::optionalOutletNode() const {
//    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_GroundHeatExchanger_VerticalFields::OutletNodeName);
//  }

  //openstudio::OSOptionalQuantity GroundHeatExchangerVertical_Impl::maximumFlowRate_SI() const {
  //  return getMaximumFlowRate(false);
  //}

  //openstudio::OSOptionalQuantity GroundHeatExchangerVertical_Impl::maximumFlowRate_IP() const {
  //  return getMaximumFlowRate(true);
  //}

  //openstudio::OSOptionalQuantity GroundHeatExchangerVertical_Impl::boreHoleLength_SI() const {
  //  return getBoreHoleLength(false);
  //}

  //openstudio::OSOptionalQuantity GroundHeatExchangerVertical_Impl::boreHoleLength_IP() const {
  //  return getBoreHoleLength(true);
  //}

  //openstudio::OSOptionalQuantity GroundHeatExchangerVertical_Impl::boreHoleRadius_SI() const {
  //  return getBoreHoleRadius(false);
  //}

  //openstudio::OSOptionalQuantity GroundHeatExchangerVertical_Impl::boreHoleRadius_IP() const {
  //  return getBoreHoleRadius(true);
  //}

  //openstudio::OSOptionalQuantity GroundHeatExchangerVertical_Impl::groundThermalConductivity_SI() const {
  //  return getGroundThermalConductivity(false);
  //}

  //openstudio::OSOptionalQuantity GroundHeatExchangerVertical_Impl::groundThermalConductivity_IP() const {
  //  return getGroundThermalConductivity(true);
  //}

  //openstudio::OSOptionalQuantity GroundHeatExchangerVertical_Impl::groundThermalHeatCapacity_SI() const {
  //  return getGroundThermalHeatCapacity(false);
  //}

  //openstudio::OSOptionalQuantity GroundHeatExchangerVertical_Impl::groundThermalHeatCapacity_IP() const {
  //  return getGroundThermalHeatCapacity(true);
  //}

  //openstudio::OSOptionalQuantity GroundHeatExchangerVertical_Impl::groundTemperature_SI() const {
  //  return getGroundTemperature(false);
  //}

  //openstudio::OSOptionalQuantity GroundHeatExchangerVertical_Impl::groundTemperature_IP() const {
  //  return getGroundTemperature(true);
  //}

  //openstudio::OSOptionalQuantity GroundHeatExchangerVertical_Impl::designFlowRate_SI() const {
  //  return getDesignFlowRate(false);
  //}

  //openstudio::OSOptionalQuantity GroundHeatExchangerVertical_Impl::designFlowRate_IP() const {
  //  return getDesignFlowRate(true);
  //}

  //openstudio::OSOptionalQuantity GroundHeatExchangerVertical_Impl::groutThermalConductivity_SI() const {
  //  return getGroutThermalConductivity(false);
  //}

  //openstudio::OSOptionalQuantity GroundHeatExchangerVertical_Impl::groutThermalConductivity_IP() const {
  //  return getGroutThermalConductivity(true);
  //}

  //openstudio::OSOptionalQuantity GroundHeatExchangerVertical_Impl::pipeThermalConductivity_SI() const {
  //  return getPipeThermalConductivity(false);
  //}

  //openstudio::OSOptionalQuantity GroundHeatExchangerVertical_Impl::pipeThermalConductivity_IP() const {
  //  return getPipeThermalConductivity(true);
  //}

  //openstudio::OSOptionalQuantity GroundHeatExchangerVertical_Impl::pipeOutDiameter_SI() const {
  //  return getPipeOutDiameter(false);
  //}

  //openstudio::OSOptionalQuantity GroundHeatExchangerVertical_Impl::pipeOutDiameter_IP() const {
  //  return getPipeOutDiameter(true);
  //}

  //openstudio::OSOptionalQuantity GroundHeatExchangerVertical_Impl::uTubeDistance_SI() const {
  //  return getUTubeDistance(false);
  //}

  //openstudio::OSOptionalQuantity GroundHeatExchangerVertical_Impl::uTubeDistance_IP() const {
  //  return getUTubeDistance(true);
  //}

  //openstudio::OSOptionalQuantity GroundHeatExchangerVertical_Impl::pipeThickness_SI() const {
  //  return getPipeThickness(false);
  //}

  //openstudio::OSOptionalQuantity GroundHeatExchangerVertical_Impl::pipeThickness_IP() const {
  //  return getPipeThickness(true);
  //}

  //openstudio::OSOptionalQuantity GroundHeatExchangerVertical_Impl::maximumLengthofSimulation_SI() const {
  //  return getMaximumLengthofSimulation(false);
  //}

  //openstudio::OSOptionalQuantity GroundHeatExchangerVertical_Impl::maximumLengthofSimulation_IP() const {
  //  return getMaximumLengthofSimulation(true);
  //}

  //openstudio::Quantity GroundHeatExchangerVertical_Impl::gFunctionReferenceRatio_SI() const {
  //  return getGFunctionReferenceRatio(false);
  //}

  //openstudio::Quantity GroundHeatExchangerVertical_Impl::gFunctionReferenceRatio_IP() const {
  //  return getGFunctionReferenceRatio(true);
  //}

//  boost::optional<ModelObject> GroundHeatExchangerVertical_Impl::inletNodeAsModelObject() const {
//    OptionalModelObject result = inletNode();
//    OptionalModelObject result = InletNode();
//    return result;
//  }

//  boost::optional<ModelObject> GroundHeatExchangerVertical_Impl::outletNodeAsModelObject() const {
//    OptionalModelObject result = outletNode();
//    return result;
//  }

//  bool GroundHeatExchangerVertical_Impl::setInletNodeAsModelObject(const boost::optional<ModelObject>& modelObject) {
//    if (modelObject) {
//      OptionalConnection intermediate = modelObject->optionalCast<Connection>();
//      if (intermediate) {
//        Connection connection(*intermediate);
//        return setInletNode(connection);
//      }
//    }
//    return false;
//  }

//  bool GroundHeatExchangerVertical_Impl::setOutletNodeAsModelObject(const boost::optional<ModelObject>& modelObject) {
//    if (modelObject) {
//      OptionalConnection intermediate = modelObject->optionalCast<Connection>();
//      if (intermediate) {
//        Connection connection(*intermediate);
//        return setOutletNode(connection);
//      }
//    }
//    return false;
//  }

  unsigned GroundHeatExchangerVertical_Impl::inletPort()
  {
    return OS_GroundHeatExchanger_VerticalFields::InletNodeName;
  }

  unsigned GroundHeatExchangerVertical_Impl::outletPort()
  {
    return OS_GroundHeatExchanger_VerticalFields::OutletNodeName;
  }


  // addToNode
  bool GroundHeatExchangerVertical_Impl::addToNode(Node & node)
  {
    if( node.airLoopHVAC() )
    {
      return false;
    }
    else
    {
      if( boost::optional<PlantLoop> plantLoop = node.plantLoop() )
      {
        if( plantLoop->demandComponent(node.handle()) )
        {
          return false;
        }
        else
        {
          bool result = StraightComponent_Impl::addToNode(node);

          if( result )
          {
            plantLoop->setFluidType("Water");  
          }

          return result;
        }
      }
    }

    return false;
  }

  //clone object
  ModelObject GroundHeatExchangerVertical_Impl::clone(Model model) const
  {
    GroundHeatExchangerVertical newGroundHeatExchanger = ModelObject_Impl::clone(model).cast<GroundHeatExchangerVertical>();

    return newGroundHeatExchanger;
  }

} // detail

GroundHeatExchangerVertical::GroundHeatExchangerVertical(const Model& model)
  : StraightComponent(GroundHeatExchangerVertical::iddObjectType(),model)
{
  BOOST_ASSERT(getImpl<detail::GroundHeatExchangerVertical_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  //     OS_GroundHeatExchanger_VerticalFields::InletNodeName
  //     OS_GroundHeatExchanger_VerticalFields::OutletNodeName

  setMaximumFlowRate(0.0033);
  setNumberofBoreHoles(120);
  setBoreHoleLength(76.2);
  setBoreHoleRadius(0.635080E-01);
  setGroundThermalConductivity(0.692626E+00);
  setGroundThermalHeatCapacity(0.234700E+07);
  setGroundTemperature(13.375);
  setDesignFlowRate(0.0033);
  setGroutThermalConductivity(0.692626E+00);
  setPipeThermalConductivity(0.391312E+00);
  setPipeOutDiameter(2.66667E-02);
  setUTubeDistance(2.53977E-02);
  setPipeThickness(2.41285E-03);
  setMaximumLengthofSimulation(2);
  setGFunctionReferenceRatio(0.0005);
  setNumberofDataPairsoftheGFunction(35);
//  setGFunctionLn(T/Ts)Value(-15.2996);
//  setGFunctionGValue(-0.348322);
//    -14.201,                 !- G-Function Ln(T/Ts) Value 2
//    0.022208,                !- G-Function G Value 2
//    -13.2202,                !- G-Function Ln(T/Ts) Value 3
//    0.412345,                !- G-Function G Value 3
//    -12.2086,                !- G-Function Ln(T/Ts) Value 4
//    0.867498,                !- G-Function G Value 4
//    -11.1888,                !- G-Function Ln(T/Ts) Value 5
//    1.357839,                !- G-Function G Value 5
//    -10.1816,                !- G-Function Ln(T/Ts) Value 6
//    1.852024,                !- G-Function G Value 6
//    -9.1815,                 !- G-Function Ln(T/Ts) Value 7
//    2.345656,                !- G-Function G Value 7
//    -8.6809,                 !- G-Function Ln(T/Ts) Value 8
//    2.593958,                !- G-Function G Value 8
//    -8.5,                    !- G-Function Ln(T/Ts) Value 9
//    2.679,                   !- G-Function G Value 9
//    -7.8,                    !- G-Function Ln(T/Ts) Value 10
//    3.023,                   !- G-Function G Value 10
//    -7.2,                    !- G-Function Ln(T/Ts) Value 11
//    3.32,                    !- G-Function G Value 11
//    -6.5,                    !- G-Function Ln(T/Ts) Value 12
//    3.681,                   !- G-Function G Value 12
//    -5.9,                    !- G-Function Ln(T/Ts) Value 13
//    4.071,                   !- G-Function G Value 13
//    -5.2,                    !- G-Function Ln(T/Ts) Value 14
//    4.828,                   !- G-Function G Value 14
//    -4.5,                    !- G-Function Ln(T/Ts) Value 15
//    6.253,                   !- G-Function G Value 15
//    -3.963,                  !- G-Function Ln(T/Ts) Value 16
//    7.894,                   !- G-Function G Value 16
//    -3.27,                   !- G-Function Ln(T/Ts) Value 17
//    11.82,                   !- G-Function G Value 17
//    -2.864,                  !- G-Function Ln(T/Ts) Value 18
//    15.117,                  !- G-Function G Value 18
//    -2.577,                  !- G-Function Ln(T/Ts) Value 19
//    18.006,                  !- G-Function G Value 19
//    -2.171,                  !- G-Function Ln(T/Ts) Value 20
//    22.887,                  !- G-Function G Value 20
//    -1.884,                  !- G-Function Ln(T/Ts) Value 21
//    26.924,                  !- G-Function G Value 21
//    -1.191,                  !- G-Function Ln(T/Ts) Value 22
//    38.004,                  !- G-Function G Value 22
//    -0.497,                  !- G-Function Ln(T/Ts) Value 23
//    49.919,                  !- G-Function G Value 23
//    -0.274,                  !- G-Function Ln(T/Ts) Value 24
//    53.407,                  !- G-Function G Value 24
//    -0.051,                  !- G-Function Ln(T/Ts) Value 25
//    56.632,                  !- G-Function G Value 25
//    0.196,                   !- G-Function Ln(T/Ts) Value 26
//    59.825,                  !- G-Function G Value 26
//    0.419,                   !- G-Function Ln(T/Ts) Value 27
//    62.349,                  !- G-Function G Value 27
//    0.642,                   !- G-Function Ln(T/Ts) Value 28
//    64.524,                  !- G-Function G Value 28
//    0.873,                   !- G-Function Ln(T/Ts) Value 29
//    66.412,                  !- G-Function G Value 29
//    1.112,                   !- G-Function Ln(T/Ts) Value 30
//    67.993,                  !- G-Function G Value 30
//    1.335,                   !- G-Function Ln(T/Ts) Value 31
//    69.162,                  !- G-Function G Value 31
//    1.679,                   !- G-Function Ln(T/Ts) Value 32
//    70.476,                  !- G-Function G Value 32
//    2.028,                   !- G-Function Ln(T/Ts) Value 33
//    71.361,                  !- G-Function G Value 33
//    2.275,                   !- G-Function Ln(T/Ts) Value 34
//    71.79,                   !- G-Function G Value 34
//    3.003,                   !- G-Function Ln(T/Ts) Value 35
//    72.511;                  !- G-Function G Value 35

  bool ok = true;
  // ok = setHandle();
  BOOST_ASSERT(ok);
  // ok = setInletNode();
  BOOST_ASSERT(ok);
  // ok = setOutletNode();
  BOOST_ASSERT(ok);
}

IddObjectType GroundHeatExchangerVertical::iddObjectType() {
  return IddObjectType(IddObjectType::OS_GroundHeatExchanger_Vertical);
}

//Connection GroundHeatExchangerVertical::inletNode() const {
//  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->inletNode();
//}

//Connection GroundHeatExchangerVertical::outletNode() const {
//  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->outletNode();
//}

boost::optional<double> GroundHeatExchangerVertical::maximumFlowRate() const {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->maximumFlowRate();
}

//OSOptionalQuantity GroundHeatExchangerVertical::getMaximumFlowRate(bool returnIP) const {
//  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->getMaximumFlowRate(returnIP);
//}

boost::optional<int> GroundHeatExchangerVertical::numberofBoreHoles() const {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->numberofBoreHoles();
}

boost::optional<double> GroundHeatExchangerVertical::boreHoleLength() const {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->boreHoleLength();
}

//OSOptionalQuantity GroundHeatExchangerVertical::getBoreHoleLength(bool returnIP) const {
//  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->getBoreHoleLength(returnIP);
//}

boost::optional<double> GroundHeatExchangerVertical::boreHoleRadius() const {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->boreHoleRadius();
}

//OSOptionalQuantity GroundHeatExchangerVertical::getBoreHoleRadius(bool returnIP) const {
//  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->getBoreHoleRadius(returnIP);
//}

boost::optional<double> GroundHeatExchangerVertical::groundThermalConductivity() const {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->groundThermalConductivity();
}

//OSOptionalQuantity GroundHeatExchangerVertical::getGroundThermalConductivity(bool returnIP) const {
//  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->getGroundThermalConductivity(returnIP);
//}

boost::optional<double> GroundHeatExchangerVertical::groundThermalHeatCapacity() const {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->groundThermalHeatCapacity();
}

//OSOptionalQuantity GroundHeatExchangerVertical::getGroundThermalHeatCapacity(bool returnIP) const {
//  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->getGroundThermalHeatCapacity(returnIP);
//}

boost::optional<double> GroundHeatExchangerVertical::groundTemperature() const {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->groundTemperature();
}

//OSOptionalQuantity GroundHeatExchangerVertical::getGroundTemperature(bool returnIP) const {
//  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->getGroundTemperature(returnIP);
//}

boost::optional<double> GroundHeatExchangerVertical::designFlowRate() const {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->designFlowRate();
}

//OSOptionalQuantity GroundHeatExchangerVertical::getDesignFlowRate(bool returnIP) const {
//  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->getDesignFlowRate(returnIP);
//}

boost::optional<double> GroundHeatExchangerVertical::groutThermalConductivity() const {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->groutThermalConductivity();
}

//OSOptionalQuantity GroundHeatExchangerVertical::getGroutThermalConductivity(bool returnIP) const {
//  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->getGroutThermalConductivity(returnIP);
//}

boost::optional<double> GroundHeatExchangerVertical::pipeThermalConductivity() const {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->pipeThermalConductivity();
}

//OSOptionalQuantity GroundHeatExchangerVertical::getPipeThermalConductivity(bool returnIP) const {
//  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->getPipeThermalConductivity(returnIP);
//}

boost::optional<double> GroundHeatExchangerVertical::pipeOutDiameter() const {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->pipeOutDiameter();
}

//OSOptionalQuantity GroundHeatExchangerVertical::getPipeOutDiameter(bool returnIP) const {
//  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->getPipeOutDiameter(returnIP);
//}

boost::optional<double> GroundHeatExchangerVertical::uTubeDistance() const {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->uTubeDistance();
}

//OSOptionalQuantity GroundHeatExchangerVertical::getUTubeDistance(bool returnIP) const {
//  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->getUTubeDistance(returnIP);
//}

boost::optional<double> GroundHeatExchangerVertical::pipeThickness() const {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->pipeThickness();
}

//OSOptionalQuantity GroundHeatExchangerVertical::getPipeThickness(bool returnIP) const {
//  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->getPipeThickness(returnIP);
//}

boost::optional<double> GroundHeatExchangerVertical::maximumLengthofSimulation() const {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->maximumLengthofSimulation();
}

//OSOptionalQuantity GroundHeatExchangerVertical::getMaximumLengthofSimulation(bool returnIP) const {
//  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->getMaximumLengthofSimulation(returnIP);
//}

double GroundHeatExchangerVertical::gFunctionReferenceRatio() const {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->gFunctionReferenceRatio();
}

//Quantity GroundHeatExchangerVertical::getGFunctionReferenceRatio(bool returnIP) const {
//  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->getGFunctionReferenceRatio(returnIP);
//}

bool GroundHeatExchangerVertical::isGFunctionReferenceRatioDefaulted() const {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->isGFunctionReferenceRatioDefaulted();
}

boost::optional<int> GroundHeatExchangerVertical::numberofDataPairsoftheGFunction() const {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->numberofDataPairsoftheGFunction();
}

//bool GroundHeatExchangerVertical::setInletNode(const Connection& connection) {
//  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->setInletNode(connection);
//}

//bool GroundHeatExchangerVertical::setOutletNode(const Connection& connection) {
//  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->setOutletNode(connection);
//}

bool GroundHeatExchangerVertical::setMaximumFlowRate(double maximumFlowRate) {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->setMaximumFlowRate(maximumFlowRate);
}

//bool GroundHeatExchangerVertical::setMaximumFlowRate(const Quantity& maximumFlowRate) {
//  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->setMaximumFlowRate(maximumFlowRate);
//}

void GroundHeatExchangerVertical::resetMaximumFlowRate() {
  getImpl<detail::GroundHeatExchangerVertical_Impl>()->resetMaximumFlowRate();
}

bool GroundHeatExchangerVertical::setNumberofBoreHoles(int numberofBoreHoles) {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->setNumberofBoreHoles(numberofBoreHoles);
}

void GroundHeatExchangerVertical::resetNumberofBoreHoles() {
  getImpl<detail::GroundHeatExchangerVertical_Impl>()->resetNumberofBoreHoles();
}

bool GroundHeatExchangerVertical::setBoreHoleLength(double boreHoleLength) {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->setBoreHoleLength(boreHoleLength);
}

//bool GroundHeatExchangerVertical::setBoreHoleLength(const Quantity& boreHoleLength) {
//  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->setBoreHoleLength(boreHoleLength);
//}

void GroundHeatExchangerVertical::resetBoreHoleLength() {
  getImpl<detail::GroundHeatExchangerVertical_Impl>()->resetBoreHoleLength();
}

bool GroundHeatExchangerVertical::setBoreHoleRadius(double boreHoleRadius) {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->setBoreHoleRadius(boreHoleRadius);
}

//bool GroundHeatExchangerVertical::setBoreHoleRadius(const Quantity& boreHoleRadius) {
//  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->setBoreHoleRadius(boreHoleRadius);
//}

void GroundHeatExchangerVertical::resetBoreHoleRadius() {
  getImpl<detail::GroundHeatExchangerVertical_Impl>()->resetBoreHoleRadius();
}

bool GroundHeatExchangerVertical::setGroundThermalConductivity(double groundThermalConductivity) {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->setGroundThermalConductivity(groundThermalConductivity);
}

//bool GroundHeatExchangerVertical::setGroundThermalConductivity(const Quantity& groundThermalConductivity) {
//  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->setGroundThermalConductivity(groundThermalConductivity);
//}

void GroundHeatExchangerVertical::resetGroundThermalConductivity() {
  getImpl<detail::GroundHeatExchangerVertical_Impl>()->resetGroundThermalConductivity();
}

bool GroundHeatExchangerVertical::setGroundThermalHeatCapacity(double groundThermalHeatCapacity) {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->setGroundThermalHeatCapacity(groundThermalHeatCapacity);
}

//bool GroundHeatExchangerVertical::setGroundThermalHeatCapacity(const Quantity& groundThermalHeatCapacity) {
//  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->setGroundThermalHeatCapacity(groundThermalHeatCapacity);
//}

void GroundHeatExchangerVertical::resetGroundThermalHeatCapacity() {
  getImpl<detail::GroundHeatExchangerVertical_Impl>()->resetGroundThermalHeatCapacity();
}

bool GroundHeatExchangerVertical::setGroundTemperature(double groundTemperature) {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->setGroundTemperature(groundTemperature);
}

//bool GroundHeatExchangerVertical::setGroundTemperature(const Quantity& groundTemperature) {
//  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->setGroundTemperature(groundTemperature);
//}

void GroundHeatExchangerVertical::resetGroundTemperature() {
  getImpl<detail::GroundHeatExchangerVertical_Impl>()->resetGroundTemperature();
}

bool GroundHeatExchangerVertical::setDesignFlowRate(double designFlowRate) {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->setDesignFlowRate(designFlowRate);
}

//bool GroundHeatExchangerVertical::setDesignFlowRate(const Quantity& designFlowRate) {
//  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->setDesignFlowRate(designFlowRate);
//}

void GroundHeatExchangerVertical::resetDesignFlowRate() {
  getImpl<detail::GroundHeatExchangerVertical_Impl>()->resetDesignFlowRate();
}

bool GroundHeatExchangerVertical::setGroutThermalConductivity(double groutThermalConductivity) {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->setGroutThermalConductivity(groutThermalConductivity);
}

//bool GroundHeatExchangerVertical::setGroutThermalConductivity(const Quantity& groutThermalConductivity) {
//  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->setGroutThermalConductivity(groutThermalConductivity);
//}

void GroundHeatExchangerVertical::resetGroutThermalConductivity() {
  getImpl<detail::GroundHeatExchangerVertical_Impl>()->resetGroutThermalConductivity();
}

bool GroundHeatExchangerVertical::setPipeThermalConductivity(double pipeThermalConductivity) {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->setPipeThermalConductivity(pipeThermalConductivity);
}

//bool GroundHeatExchangerVertical::setPipeThermalConductivity(const Quantity& pipeThermalConductivity) {
//  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->setPipeThermalConductivity(pipeThermalConductivity);
//}

void GroundHeatExchangerVertical::resetPipeThermalConductivity() {
  getImpl<detail::GroundHeatExchangerVertical_Impl>()->resetPipeThermalConductivity();
}

bool GroundHeatExchangerVertical::setPipeOutDiameter(double pipeOutDiameter) {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->setPipeOutDiameter(pipeOutDiameter);
}

//bool GroundHeatExchangerVertical::setPipeOutDiameter(const Quantity& pipeOutDiameter) {
//  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->setPipeOutDiameter(pipeOutDiameter);
//}

void GroundHeatExchangerVertical::resetPipeOutDiameter() {
  getImpl<detail::GroundHeatExchangerVertical_Impl>()->resetPipeOutDiameter();
}

bool GroundHeatExchangerVertical::setUTubeDistance(double uTubeDistance) {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->setUTubeDistance(uTubeDistance);
}

//bool GroundHeatExchangerVertical::setUTubeDistance(const Quantity& uTubeDistance) {
//  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->setUTubeDistance(uTubeDistance);
//}

void GroundHeatExchangerVertical::resetUTubeDistance() {
  getImpl<detail::GroundHeatExchangerVertical_Impl>()->resetUTubeDistance();
}

bool GroundHeatExchangerVertical::setPipeThickness(double pipeThickness) {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->setPipeThickness(pipeThickness);
}

//bool GroundHeatExchangerVertical::setPipeThickness(const Quantity& pipeThickness) {
//  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->setPipeThickness(pipeThickness);
//}

void GroundHeatExchangerVertical::resetPipeThickness() {
  getImpl<detail::GroundHeatExchangerVertical_Impl>()->resetPipeThickness();
}

bool GroundHeatExchangerVertical::setMaximumLengthofSimulation(double maximumLengthofSimulation) {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->setMaximumLengthofSimulation(maximumLengthofSimulation);
}

//bool GroundHeatExchangerVertical::setMaximumLengthofSimulation(const Quantity& maximumLengthofSimulation) {
//  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->setMaximumLengthofSimulation(maximumLengthofSimulation);
//}

void GroundHeatExchangerVertical::resetMaximumLengthofSimulation() {
  getImpl<detail::GroundHeatExchangerVertical_Impl>()->resetMaximumLengthofSimulation();
}

bool GroundHeatExchangerVertical::setGFunctionReferenceRatio(double gFunctionReferenceRatio) {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->setGFunctionReferenceRatio(gFunctionReferenceRatio);
}

//bool GroundHeatExchangerVertical::setGFunctionReferenceRatio(const Quantity& gFunctionReferenceRatio) {
//  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->setGFunctionReferenceRatio(gFunctionReferenceRatio);
//}

void GroundHeatExchangerVertical::resetGFunctionReferenceRatio() {
  getImpl<detail::GroundHeatExchangerVertical_Impl>()->resetGFunctionReferenceRatio();
}

bool GroundHeatExchangerVertical::setNumberofDataPairsoftheGFunction(int numberofDataPairsoftheGFunction) {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->setNumberofDataPairsoftheGFunction(numberofDataPairsoftheGFunction);
}

void GroundHeatExchangerVertical::resetNumberofDataPairsoftheGFunction() {
  getImpl<detail::GroundHeatExchangerVertical_Impl>()->resetNumberofDataPairsoftheGFunction();
}

/// @cond
GroundHeatExchangerVertical::GroundHeatExchangerVertical(boost::shared_ptr<detail::GroundHeatExchangerVertical_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond

} // model
} // openstudio

