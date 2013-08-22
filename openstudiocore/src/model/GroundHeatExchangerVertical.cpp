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

#include <utilities/idf/WorkspaceExtensibleGroup.hpp>

#include <utilities/idd/OS_GroundHeatExchanger_Vertical_FieldEnums.hxx>
#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace model {

namespace detail {

  GroundHeatExchangerVertical_Impl::GroundHeatExchangerVertical_Impl(const IdfObject& idfObject,
                                                                     Model_Impl* model,
                                                                     bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == GroundHeatExchangerVertical::iddObjectType());
  }

  GroundHeatExchangerVertical_Impl::GroundHeatExchangerVertical_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                     Model_Impl* model,
                                                                     bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == GroundHeatExchangerVertical::iddObjectType());
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

  boost::optional<double> GroundHeatExchangerVertical_Impl::maximumFlowRate() const {
    return getDouble(OS_GroundHeatExchanger_VerticalFields::MaximumFlowRate,true);
  }

  boost::optional<int> GroundHeatExchangerVertical_Impl::numberofBoreHoles() const {
    return getInt(OS_GroundHeatExchanger_VerticalFields::NumberofBoreHoles,true);
  }

  boost::optional<double> GroundHeatExchangerVertical_Impl::boreHoleLength() const {
    return getDouble(OS_GroundHeatExchanger_VerticalFields::BoreHoleLength,true);
  }

  boost::optional<double> GroundHeatExchangerVertical_Impl::boreHoleRadius() const {
    return getDouble(OS_GroundHeatExchanger_VerticalFields::BoreHoleRadius,true);
  }

  boost::optional<double> GroundHeatExchangerVertical_Impl::groundThermalConductivity() const {
    return getDouble(OS_GroundHeatExchanger_VerticalFields::GroundThermalConductivity,true);
  }

  boost::optional<double> GroundHeatExchangerVertical_Impl::groundThermalHeatCapacity() const {
    return getDouble(OS_GroundHeatExchanger_VerticalFields::GroundThermalHeatCapacity,true);
  }

  boost::optional<double> GroundHeatExchangerVertical_Impl::groundTemperature() const {
    return getDouble(OS_GroundHeatExchanger_VerticalFields::GroundTemperature,true);
  }

  boost::optional<double> GroundHeatExchangerVertical_Impl::designFlowRate() const {
    return getDouble(OS_GroundHeatExchanger_VerticalFields::DesignFlowRate,true);
  }

  boost::optional<double> GroundHeatExchangerVertical_Impl::groutThermalConductivity() const {
    return getDouble(OS_GroundHeatExchanger_VerticalFields::GroutThermalConductivity,true);
  }

  boost::optional<double> GroundHeatExchangerVertical_Impl::pipeThermalConductivity() const {
    return getDouble(OS_GroundHeatExchanger_VerticalFields::PipeThermalConductivity,true);
  }

  boost::optional<double> GroundHeatExchangerVertical_Impl::pipeOutDiameter() const {
    return getDouble(OS_GroundHeatExchanger_VerticalFields::PipeOutDiameter,true);
  }

  boost::optional<double> GroundHeatExchangerVertical_Impl::uTubeDistance() const {
    return getDouble(OS_GroundHeatExchanger_VerticalFields::UTubeDistance,true);
  }

  boost::optional<double> GroundHeatExchangerVertical_Impl::pipeThickness() const {
    return getDouble(OS_GroundHeatExchanger_VerticalFields::PipeThickness,true);
  }

  boost::optional<double> GroundHeatExchangerVertical_Impl::maximumLengthofSimulation() const {
    return getDouble(OS_GroundHeatExchanger_VerticalFields::MaximumLengthofSimulation,true);
  }

  double GroundHeatExchangerVertical_Impl::gFunctionReferenceRatio() const {
    boost::optional<double> value = getDouble(OS_GroundHeatExchanger_VerticalFields::GFunctionReferenceRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool GroundHeatExchangerVertical_Impl::isGFunctionReferenceRatioDefaulted() const {
    return isEmpty(OS_GroundHeatExchanger_VerticalFields::GFunctionReferenceRatio);
  }

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

  void GroundHeatExchangerVertical_Impl::resetMaximumFlowRate() {
    bool result = setString(OS_GroundHeatExchanger_VerticalFields::MaximumFlowRate, "");
    OS_ASSERT(result);
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
    OS_ASSERT(result);
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

  void GroundHeatExchangerVertical_Impl::resetBoreHoleLength() {
    bool result = setString(OS_GroundHeatExchanger_VerticalFields::BoreHoleLength, "");
    OS_ASSERT(result);
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

  void GroundHeatExchangerVertical_Impl::resetBoreHoleRadius() {
    bool result = setString(OS_GroundHeatExchanger_VerticalFields::BoreHoleRadius, "");
    OS_ASSERT(result);
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

  void GroundHeatExchangerVertical_Impl::resetGroundThermalConductivity() {
    bool result = setString(OS_GroundHeatExchanger_VerticalFields::GroundThermalConductivity, "");
    OS_ASSERT(result);
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

  void GroundHeatExchangerVertical_Impl::resetGroundThermalHeatCapacity() {
    bool result = setString(OS_GroundHeatExchanger_VerticalFields::GroundThermalHeatCapacity, "");
    OS_ASSERT(result);
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

  void GroundHeatExchangerVertical_Impl::resetGroundTemperature() {
    bool result = setString(OS_GroundHeatExchanger_VerticalFields::GroundTemperature, "");
    OS_ASSERT(result);
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

  void GroundHeatExchangerVertical_Impl::resetDesignFlowRate() {
    bool result = setString(OS_GroundHeatExchanger_VerticalFields::DesignFlowRate, "");
    OS_ASSERT(result);
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

  void GroundHeatExchangerVertical_Impl::resetGroutThermalConductivity() {
    bool result = setString(OS_GroundHeatExchanger_VerticalFields::GroutThermalConductivity, "");
    OS_ASSERT(result);
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

  void GroundHeatExchangerVertical_Impl::resetPipeThermalConductivity() {
    bool result = setString(OS_GroundHeatExchanger_VerticalFields::PipeThermalConductivity, "");
    OS_ASSERT(result);
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

  void GroundHeatExchangerVertical_Impl::resetPipeOutDiameter() {
    bool result = setString(OS_GroundHeatExchanger_VerticalFields::PipeOutDiameter, "");
    OS_ASSERT(result);
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
  void GroundHeatExchangerVertical_Impl::resetUTubeDistance() {
    bool result = setString(OS_GroundHeatExchanger_VerticalFields::UTubeDistance, "");
    OS_ASSERT(result);
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

  void GroundHeatExchangerVertical_Impl::resetPipeThickness() {
    bool result = setString(OS_GroundHeatExchanger_VerticalFields::PipeThickness, "");
    OS_ASSERT(result);
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

  void GroundHeatExchangerVertical_Impl::resetMaximumLengthofSimulation() {
    bool result = setString(OS_GroundHeatExchanger_VerticalFields::MaximumLengthofSimulation, "");
    OS_ASSERT(result);
  }

  bool GroundHeatExchangerVertical_Impl::setGFunctionReferenceRatio(double gFunctionReferenceRatio) {
    bool result = setDouble(OS_GroundHeatExchanger_VerticalFields::GFunctionReferenceRatio, gFunctionReferenceRatio);
    return result;
  }

  void GroundHeatExchangerVertical_Impl::resetGFunctionReferenceRatio() {
    bool result = setString(OS_GroundHeatExchanger_VerticalFields::GFunctionReferenceRatio, "");
    OS_ASSERT(result);
  }

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
    GroundHeatExchangerVertical newGroundHeatExchanger = StraightComponent_Impl::clone(model).cast<GroundHeatExchangerVertical>();

    return newGroundHeatExchanger;
  }

  bool GroundHeatExchangerVertical_Impl::addGFunction(double gFunctionLN, double gFunctionGValue)
  {
    WorkspaceExtensibleGroup eg = getObject<ModelObject>().pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();

    bool temp = eg.setDouble(OS_GroundHeatExchanger_VerticalExtensibleFields::GFunctionLn_T_Ts_Value,gFunctionLN);
    bool ok = eg.setDouble(OS_GroundHeatExchanger_VerticalExtensibleFields::GFunctionGValue,gFunctionGValue);

    if(temp) {
      temp = ok;
    }

    if( !temp ) {
      getObject<ModelObject>().eraseExtensibleGroup(eg.groupIndex());
      return temp;
    }
    return temp;
  }

  void GroundHeatExchangerVertical_Impl::removeGFunction(unsigned groupIndex)
  {
    unsigned numberofDataPairs = numExtensibleGroups();
    if(groupIndex < numberofDataPairs) {
      getObject<ModelObject>().eraseExtensibleGroup(groupIndex);
    }
  }

  void GroundHeatExchangerVertical_Impl::removeAllGFunctions() 
  {
    getObject<ModelObject>().clearExtensibleGroups();
  }

  std::vector< std::pair<double,double> > GroundHeatExchangerVertical_Impl::gFunctions() 
  {
    std::vector< std::pair<double,double> > result;

    std::vector<IdfExtensibleGroup> groups = extensibleGroups();

    for( std::vector<IdfExtensibleGroup>::iterator it = groups.begin();
         it != groups.end();
         it++ )
    {
      boost::optional<double> gFunctionLN = it->cast<WorkspaceExtensibleGroup>().getDouble(OS_GroundHeatExchanger_VerticalExtensibleFields::GFunctionLn_T_Ts_Value);
      boost::optional<double> gFunctionGValue = it->cast<WorkspaceExtensibleGroup>().getDouble(OS_GroundHeatExchanger_VerticalExtensibleFields::GFunctionGValue);

      if(gFunctionLN && gFunctionGValue) {
        result.push_back( std::make_pair(gFunctionLN.get(), gFunctionGValue.get()) );
      }
    }

    return result;
  }

} // detail

GroundHeatExchangerVertical::GroundHeatExchangerVertical(const Model& model)
  : StraightComponent(GroundHeatExchangerVertical::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::GroundHeatExchangerVertical_Impl>());

  setMaximumFlowRate(0.0033);
  setNumberofBoreHoles(120);
  setBoreHoleLength(76.2);
  setBoreHoleRadius(0.635080E-01);
  setGroundThermalConductivity(0.692626);
  setGroundThermalHeatCapacity(0.234700E+07);
  setGroundTemperature(13.375);
  setDesignFlowRate(0.0033);
  setGroutThermalConductivity(0.692626);
  setPipeThermalConductivity(0.391312);
  setPipeOutDiameter(2.66667E-02);
  setUTubeDistance(2.53977E-02);
  setPipeThickness(2.41285E-03);
  setMaximumLengthofSimulation(2);
  setGFunctionReferenceRatio(0.0005);
  addGFunction(-15.2996, -0.348322);
  addGFunction(-14.201, 0.022208);
  addGFunction(-13.2202, 0.412345);
  addGFunction(-12.2086, 0.867498);
  addGFunction(-11.1888, 1.357839);
  addGFunction(-10.1816, 1.852024);
  addGFunction(-9.1815, 2.345656);
  addGFunction(-8.6809, 2.593958);
  addGFunction(-8.5, 2.679);
  addGFunction(-7.8, 3.023);
  addGFunction(-7.2, 3.32);
  addGFunction(-6.5, 3.681);
  addGFunction(-5.9, 4.071);
  addGFunction(-5.2, 4.828);
  addGFunction(-4.5, 6.253);
  addGFunction(-3.963, 7.894);
  addGFunction(-3.27, 11.82);
  addGFunction(-2.864, 15.117);
  addGFunction(-2.577, 18.006);
  addGFunction(-2.171, 22.887);
  addGFunction(-1.884, 26.924);
  addGFunction(-1.191, 38.004);
  addGFunction(-0.497, 49.919);
  addGFunction(-0.274, 53.407);
  addGFunction(-0.051, 56.632);
  addGFunction(0.196, 59.825);
  addGFunction(0.419, 62.349);
  addGFunction(0.642, 64.524);
  addGFunction(0.873, 66.412);
  addGFunction(1.112, 67.993);
  addGFunction(1.335, 69.162);
  addGFunction(1.679, 70.476);
  addGFunction(2.028, 71.361);
  addGFunction(2.275, 71.79);
  addGFunction(3.003, 72.511);

}

IddObjectType GroundHeatExchangerVertical::iddObjectType() {
  return IddObjectType(IddObjectType::OS_GroundHeatExchanger_Vertical);
}

bool GroundHeatExchangerVertical::addGFunction(double gFunctionLN, double gFunctionGValue){
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->addGFunction(gFunctionLN, gFunctionGValue);
}

void GroundHeatExchangerVertical::removeGFunction(int groupIndex){
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->removeGFunction(groupIndex);
}

void GroundHeatExchangerVertical::removeAllGFunctions(){
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->removeAllGFunctions();
}

std::vector< std::pair<double,double> > GroundHeatExchangerVertical::gFunctions(){
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->gFunctions();
}

boost::optional<double> GroundHeatExchangerVertical::maximumFlowRate() const {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->maximumFlowRate();
}

boost::optional<int> GroundHeatExchangerVertical::numberofBoreHoles() const {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->numberofBoreHoles();
}

boost::optional<double> GroundHeatExchangerVertical::boreHoleLength() const {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->boreHoleLength();
}

boost::optional<double> GroundHeatExchangerVertical::boreHoleRadius() const {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->boreHoleRadius();
}

boost::optional<double> GroundHeatExchangerVertical::groundThermalConductivity() const {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->groundThermalConductivity();
}

boost::optional<double> GroundHeatExchangerVertical::groundThermalHeatCapacity() const {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->groundThermalHeatCapacity();
}

boost::optional<double> GroundHeatExchangerVertical::groundTemperature() const {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->groundTemperature();
}

boost::optional<double> GroundHeatExchangerVertical::designFlowRate() const {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->designFlowRate();
}

boost::optional<double> GroundHeatExchangerVertical::groutThermalConductivity() const {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->groutThermalConductivity();
}

boost::optional<double> GroundHeatExchangerVertical::pipeThermalConductivity() const {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->pipeThermalConductivity();
}

boost::optional<double> GroundHeatExchangerVertical::pipeOutDiameter() const {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->pipeOutDiameter();
}

boost::optional<double> GroundHeatExchangerVertical::uTubeDistance() const {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->uTubeDistance();
}

boost::optional<double> GroundHeatExchangerVertical::pipeThickness() const {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->pipeThickness();
}

boost::optional<double> GroundHeatExchangerVertical::maximumLengthofSimulation() const {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->maximumLengthofSimulation();
}

double GroundHeatExchangerVertical::gFunctionReferenceRatio() const {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->gFunctionReferenceRatio();
}

bool GroundHeatExchangerVertical::isGFunctionReferenceRatioDefaulted() const {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->isGFunctionReferenceRatioDefaulted();
}

bool GroundHeatExchangerVertical::setMaximumFlowRate(double maximumFlowRate) {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->setMaximumFlowRate(maximumFlowRate);
}

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

void GroundHeatExchangerVertical::resetBoreHoleLength() {
  getImpl<detail::GroundHeatExchangerVertical_Impl>()->resetBoreHoleLength();
}

bool GroundHeatExchangerVertical::setBoreHoleRadius(double boreHoleRadius) {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->setBoreHoleRadius(boreHoleRadius);
}

void GroundHeatExchangerVertical::resetBoreHoleRadius() {
  getImpl<detail::GroundHeatExchangerVertical_Impl>()->resetBoreHoleRadius();
}

bool GroundHeatExchangerVertical::setGroundThermalConductivity(double groundThermalConductivity) {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->setGroundThermalConductivity(groundThermalConductivity);
}

void GroundHeatExchangerVertical::resetGroundThermalConductivity() {
  getImpl<detail::GroundHeatExchangerVertical_Impl>()->resetGroundThermalConductivity();
}

bool GroundHeatExchangerVertical::setGroundThermalHeatCapacity(double groundThermalHeatCapacity) {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->setGroundThermalHeatCapacity(groundThermalHeatCapacity);
}

void GroundHeatExchangerVertical::resetGroundThermalHeatCapacity() {
  getImpl<detail::GroundHeatExchangerVertical_Impl>()->resetGroundThermalHeatCapacity();
}

bool GroundHeatExchangerVertical::setGroundTemperature(double groundTemperature) {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->setGroundTemperature(groundTemperature);
}

void GroundHeatExchangerVertical::resetGroundTemperature() {
  getImpl<detail::GroundHeatExchangerVertical_Impl>()->resetGroundTemperature();
}

bool GroundHeatExchangerVertical::setDesignFlowRate(double designFlowRate) {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->setDesignFlowRate(designFlowRate);
}

void GroundHeatExchangerVertical::resetDesignFlowRate() {
  getImpl<detail::GroundHeatExchangerVertical_Impl>()->resetDesignFlowRate();
}

bool GroundHeatExchangerVertical::setGroutThermalConductivity(double groutThermalConductivity) {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->setGroutThermalConductivity(groutThermalConductivity);
}

void GroundHeatExchangerVertical::resetGroutThermalConductivity() {
  getImpl<detail::GroundHeatExchangerVertical_Impl>()->resetGroutThermalConductivity();
}

bool GroundHeatExchangerVertical::setPipeThermalConductivity(double pipeThermalConductivity) {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->setPipeThermalConductivity(pipeThermalConductivity);
}

void GroundHeatExchangerVertical::resetPipeThermalConductivity() {
  getImpl<detail::GroundHeatExchangerVertical_Impl>()->resetPipeThermalConductivity();
}

bool GroundHeatExchangerVertical::setPipeOutDiameter(double pipeOutDiameter) {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->setPipeOutDiameter(pipeOutDiameter);
}

void GroundHeatExchangerVertical::resetPipeOutDiameter() {
  getImpl<detail::GroundHeatExchangerVertical_Impl>()->resetPipeOutDiameter();
}

bool GroundHeatExchangerVertical::setUTubeDistance(double uTubeDistance) {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->setUTubeDistance(uTubeDistance);
}

void GroundHeatExchangerVertical::resetUTubeDistance() {
  getImpl<detail::GroundHeatExchangerVertical_Impl>()->resetUTubeDistance();
}

bool GroundHeatExchangerVertical::setPipeThickness(double pipeThickness) {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->setPipeThickness(pipeThickness);
}

void GroundHeatExchangerVertical::resetPipeThickness() {
  getImpl<detail::GroundHeatExchangerVertical_Impl>()->resetPipeThickness();
}

bool GroundHeatExchangerVertical::setMaximumLengthofSimulation(double maximumLengthofSimulation) {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->setMaximumLengthofSimulation(maximumLengthofSimulation);
}

void GroundHeatExchangerVertical::resetMaximumLengthofSimulation() {
  getImpl<detail::GroundHeatExchangerVertical_Impl>()->resetMaximumLengthofSimulation();
}

bool GroundHeatExchangerVertical::setGFunctionReferenceRatio(double gFunctionReferenceRatio) {
  return getImpl<detail::GroundHeatExchangerVertical_Impl>()->setGFunctionReferenceRatio(gFunctionReferenceRatio);
}

void GroundHeatExchangerVertical::resetGFunctionReferenceRatio() {
  getImpl<detail::GroundHeatExchangerVertical_Impl>()->resetGFunctionReferenceRatio();
}

/// @cond
GroundHeatExchangerVertical::GroundHeatExchangerVertical(boost::shared_ptr<detail::GroundHeatExchangerVertical_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond

} // model
} // openstudio

