/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
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

#ifndef MODEL_PLANTLOOP_IMPL_HPP
#define MODEL_PLANTLOOP_IMPL_HPP

#include "Loop_Impl.hpp"

namespace openstudio {

namespace model {

class Node;

class HVACComponent;

class Mixer;

class Splitter;

class SizingPlant;

namespace detail {

class Model_Impl;

class MODEL_API PlantLoop_Impl : public Loop_Impl {
 public:

  PlantLoop_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

  PlantLoop_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                   Model_Impl* model,
                   bool keepHandle);

  PlantLoop_Impl(const PlantLoop_Impl& other, Model_Impl* model, bool keepHandle);

  virtual ~PlantLoop_Impl() {}

  virtual IddObjectType iddObjectType() const;

  std::string fluidType();

  void setFluidType( const std::string & value );

  double maximumLoopTemperature();

  void setMaximumLoopTemperature( double value );

  double minimumLoopTemperature();

  void setMinimumLoopTemperature( double value );

  boost::optional<double> maximumLoopFlowRate();

  void setMaximumLoopFlowRate( double value );

  bool isMaximumLoopFlowRateAutosized();

  void autosizeMaximumLoopFlowRate();

  boost::optional<double> minimumLoopFlowRate();

  void setMinimumLoopFlowRate( double value );

  bool isMinimumLoopFlowRateAutosized();

  void autosizeMinimumLoopFlowRate();

  boost::optional<double> plantLoopVolume();

  void setPlantLoopVolume( double value );

  bool isPlantLoopVolumeAutocalculated();

  void autocalculatePlantLoopVolume();

  boost::optional<std::string> commonPipeSimulation() const;

  bool setCommonPipeSimulation(const std::string & value);

  void resetCommonPipeSimulation();

  //std::vector<ModelObject> supplyComponents(HVACComponent inletComp,
  //                                          HVACComponent outletComp,
  //                                          openstudio::IddObjectType type = IddObjectType::Catchall);

  std::vector<ModelObject> demandComponents(HVACComponent inletComp,
                                            HVACComponent outletComp,
                                            openstudio::IddObjectType type = IddObjectType::Catchall);

  //std::vector<ModelObject> supplyComponents(openstudio::IddObjectType type=IddObjectType::Catchall);

  std::vector<ModelObject> demandComponents(openstudio::IddObjectType type=IddObjectType::Catchall);

  //std::vector<ModelObject> components(openstudio::IddObjectType type = IddObjectType::Catchall);

  boost::optional<ModelObject> component(openstudio::Handle handle);

  //std::vector<ModelObject> supplyComponents(std::vector<HVACComponent> inletComps,
  //                                          std::vector<HVACComponent> outletComps,
  //                                          openstudio::IddObjectType type );

  std::vector<ModelObject> demandComponents(std::vector<HVACComponent> inletComps,
                                            std::vector<HVACComponent> outletComps,
                                            openstudio::IddObjectType type );

  virtual std::vector<openstudio::IdfObject> remove();

  virtual ModelObject clone(Model model) const;

  unsigned supplyInletPort() const;

  unsigned supplyOutletPort() const;

  unsigned demandInletPort() const;

  unsigned demandOutletPort() const;

  virtual Node supplyInletNode() const;

  virtual Node supplyOutletNode() const;

  virtual Node demandInletNode() const;

  virtual Node demandOutletNode() const;

  Mixer supplyMixer();

  Splitter supplySplitter();

  Mixer demandMixer();

  Splitter demandSplitter();

  bool addSupplyBranchForComponent( HVACComponent component );

  bool removeSupplyBranchWithComponent( HVACComponent hvacComponent );

  bool addDemandBranchForComponent( HVACComponent component );

  bool removeDemandBranchWithComponent( HVACComponent hvacComponent );

  Node loopTemperatureSetpointNode();

  void setLoopTemperatureSetpointNode( Node & node );

  std::vector<ModelObject> children() const;

  SizingPlant sizingPlant() const;

 private:

  bool isDemandBranchEmpty();

  bool removeBranchWithComponent( HVACComponent component, Splitter splitter, Mixer mixer, bool isSupplyComponent );

  REGISTER_LOGGER("openstudio.model.PlantLoop");

};

} // detail

} // model

} // openstudio

#endif // MODEL_PLANTLOOP_IMPL_HPP

