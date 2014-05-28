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

#ifndef MODEL_PLANTLOOP_HPP
#define MODEL_PLANTLOOP_HPP

#include "ModelAPI.hpp"
#include "Loop.hpp"

namespace openstudio {

namespace model {

namespace detail {
  class PlantLoop_Impl;
};

class Node;

class Splitter;

class Mixer;

class HVACComponent;

class SizingPlant;

/** PlantLoop is an interface to the EnergyPlus IDD object
 *  named "PlantLoop"
 *
 *  The purpose of this class is to simplify the construction and manipulation
 *  of the EnergyPlus PlantLoop object.
 */
class MODEL_API PlantLoop : public Loop {

  public:

  /** Constructs a new PlantLoop object and places it inside the
   *  model.  The loop is fully initialized with all companion objects.
   */
  explicit PlantLoop(Model& model);

  virtual ~PlantLoop() {}

  std::string fluidType();

  void setFluidType( const std::string & value );

  Node loopTemperatureSetpointNode();

  void setLoopTemperatureSetpointNode( Node & node );

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

  Node supplyInletNode() const;

  Node supplyOutletNode() const;

  Node demandInletNode() const;

  Node demandOutletNode() const;

  /** Returns all of the demand side hvac equipment between
   * inletComps and outletComps.  If type is given then the results will
   * be limited to the given IddObjectType.  Multiple inlet and outlet nodes
   * can be provided.
   */
  std::vector<ModelObject> demandComponents(std::vector<HVACComponent> inletComps,
                                            std::vector<HVACComponent> outletComps,
                                            openstudio::IddObjectType type = IddObjectType::Catchall);

  /** Returns all of the demand side hvac equipment between
   * inletComp and outletComp.  If type is given then the results will
   * be limited to the given IddObjectType.  Only one inlet and outlet node
   * can be given.
   */
  std::vector<ModelObject> demandComponents(HVACComponent inletComp,
                                            HVACComponent outletComp,
                                            openstudio::IddObjectType type = IddObjectType::Catchall);

  /** Returns all of the demand side HVAC equipment within the air loop.
   * If type is given then the results will be limited to the given IddObjectType.
   */
  std::vector<ModelObject> demandComponents(openstudio::IddObjectType type = IddObjectType::Catchall);

  /** Returns all of the HVAC equipment within the air loop including both
   * the supply and demand sides of the loop.
   * If type is given then the results will be limited to the given IddObjectType.
   */
  //std::vector<ModelObject> components(openstudio::IddObjectType type = IddObjectType::Catchall);

  /** Returns an optional ModelObject with the given handle.
   * If the handle is not within the PlantLoop then the optional will be false
   */
  boost::optional<ModelObject> component(openstudio::Handle handle);

  /** Returns the supply side Mixer. **/
  Mixer supplyMixer();

  /** Returns the supply side Splitter. **/
  Splitter supplySplitter();

  /** Returns the demand side Mixer. **/
  Mixer demandMixer();

  /** Returns the demand side Splitter. **/
  Splitter demandSplitter();

  /** Adds a new demand branch for component and returns a bool indicating success. 
   */
  bool addSupplyBranchForComponent( HVACComponent hvacComponent );

  /** Removes the demand side branch that contains the specified hvacComponent.
   *  Does not remove the component from the model, but may remove surrounding,
   *  components on the branch if they are not attached to other HVAC objects.
   */
  bool removeSupplyBranchWithComponent( HVACComponent hvacComponent );

  /** Adds a new demand branch for component and returns a bool indicating success.
   * This method will create a new ControllerWaterCoil if hvacComponent is a
   * CoilCoolingWater or a CoilHeatingWater.
   */
  bool addDemandBranchForComponent( HVACComponent hvacComponent );

  /** Removes the demand side branch that contains the specified hvacComponent.
   *  Does not remove the component from the model, but may remove surrounding,
   *  components on the branch if they are not attached to other HVAC objects.
   */
  bool removeDemandBranchWithComponent( HVACComponent hvacComponent );

  virtual std::vector<openstudio::IdfObject> remove();

  virtual ModelObject clone(Model model) const;

  static IddObjectType iddObjectType();

  SizingPlant sizingPlant() const;

  protected:

  friend class Model;

  friend class openstudio::IdfObject;

  /// @cond

  typedef detail::PlantLoop_Impl ImplType;

  explicit PlantLoop(std::shared_ptr<ImplType> impl);

  private:

  unsigned supplyInletPort() const;

  unsigned supplyOutletPort() const;

  unsigned demandInletPort() const;

  unsigned demandOutletPort() const;

  REGISTER_LOGGER("openstudio.model.PlantLoop");

  /// @endcond

};

/** \relates PlantLoop */
typedef boost::optional<PlantLoop> OptionalPlantLoop;

} // model
} // openstudio

#endif // MODEL_PLANTLOOP_HPP
