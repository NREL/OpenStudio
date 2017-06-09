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
class PlantEquipmentOperationScheme;
class PlantEquipmentOperationHeatingLoad;
class PlantEquipmentOperationCoolingLoad;
class AvailabilityManager;


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

  virtual IddObjectType iddObjectType() const override;

  std::string loadDistributionScheme();

  bool setLoadDistributionScheme(std::string scheme);

  boost::optional<AvailabilityManager> availabilityManager() const;

  bool setAvailabilityManager(const AvailabilityManager & availabilityManager);

  void resetAvailabilityManager();

  std::string fluidType();

  bool setFluidType( const std::string & value );

  void setGlycolConcentration(int glycolConcentration);

  int glycolConcentration() const;

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

  boost::optional<PlantEquipmentOperationHeatingLoad> plantEquipmentOperationHeatingLoad() const;

  bool setPlantEquipmentOperationHeatingLoad(const boost::optional<PlantEquipmentOperationHeatingLoad>& plantOperation);

  void resetPlantEquipmentOperationHeatingLoad();

  boost::optional<PlantEquipmentOperationCoolingLoad> plantEquipmentOperationCoolingLoad() const;

  bool setPlantEquipmentOperationCoolingLoad(const boost::optional<PlantEquipmentOperationCoolingLoad>& plantOperation);

  void resetPlantEquipmentOperationCoolingLoad();

  boost::optional<PlantEquipmentOperationScheme> primaryPlantEquipmentOperationScheme() const;

  bool setPrimaryPlantEquipmentOperationScheme(const boost::optional<PlantEquipmentOperationScheme>& plantOperation);

  void resetPrimaryPlantEquipmentOperationScheme();

  bool setPlantEquipmentOperationHeatingLoadSchedule(Schedule &);

  void resetPlantEquipmentOperationHeatingLoadSchedule();

  boost::optional<Schedule> plantEquipmentOperationHeatingLoadSchedule() const;

  bool setPlantEquipmentOperationCoolingLoadSchedule(Schedule &);

  boost::optional<Schedule> plantEquipmentOperationCoolingLoadSchedule() const;

  void resetPlantEquipmentOperationCoolingLoadSchedule();

  bool setPrimaryPlantEquipmentOperationSchemeSchedule(Schedule &);

  void resetPrimaryPlantEquipmentOperationSchemeSchedule();

  boost::optional<Schedule> primaryPlantEquipmentOperationSchemeSchedule() const;

  bool setComponentSetpointOperationSchemeSchedule(Schedule &);

  boost::optional<Schedule> componentSetpointOperationSchemeSchedule() const;

  void resetComponentSetpointOperationSchemeSchedule();

  virtual std::vector<openstudio::IdfObject> remove() override;

  virtual ModelObject clone(Model model) const override;

  unsigned supplyInletPort() const;

  unsigned supplyOutletPort() const;

  unsigned demandInletPort() const;

  unsigned demandOutletPort() const;

  virtual Node supplyInletNode() const override;

  virtual Node supplyOutletNode() const override;

  virtual std::vector<Node> supplyOutletNodes() const override;

  virtual Node demandInletNode() const override;

  virtual std::vector<Node> demandInletNodes() const override;

  virtual Node demandOutletNode() const override;

  Mixer supplyMixer() const;

  void setSupplyMixer(Mixer const & mixer);

  Splitter supplySplitter() const;

  void setSupplySplitter(Splitter const & splitter);

  Mixer demandMixer() override;

  void setDemandMixer(Mixer const & mixer);

  Splitter demandSplitter() override;

  void setDemandSplitter(Splitter const & splitter);

  bool addSupplyBranchForComponent( HVACComponent component );

  bool removeSupplyBranchWithComponent( HVACComponent hvacComponent );

  bool addDemandBranchForComponent( HVACComponent component, bool tertiary = false );

  bool removeDemandBranchWithComponent( HVACComponent hvacComponent );

  Node loopTemperatureSetpointNode();

  void setLoopTemperatureSetpointNode( Node & node );

  std::vector<ModelObject> children() const override;

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

