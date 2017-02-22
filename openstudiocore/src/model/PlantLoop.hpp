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
class PlantEquipmentOperationScheme;
class PlantEquipmentOperationHeatingLoad;
class PlantEquipmentOperationCoolingLoad;
class Schedule;

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

  static std::vector<std::string> loadDistributionSchemeValues();

  /** Prior to OS 1.11.0 the options where
      Optimal, Sequential, and Uniform.
      E+ changed the available options to.
      Optimal, SequentialLoad, UniformLoad, UniformPLR, SequentialUniformPLR
      in version 8.2.0. 

      OS did not catch up to the new options until 1.11.0. In 1.11.0 existing OS files will
      be upgraded to use the new options. Sequential will be version translated to
      SequentialLoad and Uniform will be translated to UniformLoad.

      Existing API clients may continue using the old enumerations of Sequential and Uniform, however
      these options are deprecated.

      The current options supported by OS are now consisitent with E+,
      Optimal, SequentialLoad, UniformLoad, UniformPLR, SequentialUniformPLR
  **/
  std::string loadDistributionScheme();

  bool setLoadDistributionScheme(std::string scheme);

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

  /** In OpenStudio there are three levels of "priority" for PlantEquipmentOperationScheme instances.
    * Priority here means that if there are multiple operation schemes that list the same equipment, 
    * the one with the highest priority will define operation for that equipment.
    * The operation scheme defined for primaryPlantEquipmentOperationScheme() is the highest priority,
    * followed by any component setpoint operation. Heating and cooling load operation,
    * defined by plantEquipmentOperationHeatingLoad() and plantEquipmentOperationCoolingLoad() has the lowest priority.
    *
    * No operation scheme is required for plant operation. OpenStudio will provide suitable defaults if none are provied.
    * If any operation schemes, including primary, heating load, or cooling load, are defined then the default logic is 
    * entirely disabled.
    *
    * OpenStudio does not define a PlantEquipmentOperationComponentSetpoint, which is defined in EnergyPlus, 
    * but the funcitonality is supported. In OpenStudio placing a setpoint manager on a component outlet
    * node will trigger OpenStudio to produce a component setpoint operation scheme on export to EnergyPlus.
    * This component setpoint behavior is in place even if there are other primary, heating load, or cooling load schemes defined.
    */
  boost::optional<PlantEquipmentOperationHeatingLoad> plantEquipmentOperationHeatingLoad() const;

  bool setPlantEquipmentOperationHeatingLoad(const PlantEquipmentOperationHeatingLoad& plantOperation);

  void resetPlantEquipmentOperationHeatingLoad();

  /** Set the hours of operation for which the PlantEquipmentOperationHeatingLoad, if any, applies. **/
  bool setPlantEquipmentOperationHeatingLoadSchedule(Schedule &);

  void resetPlantEquipmentOperationHeatingLoadSchedule();

  boost::optional<Schedule> plantEquipmentOperationHeatingLoadSchedule() const;

  boost::optional<PlantEquipmentOperationCoolingLoad> plantEquipmentOperationCoolingLoad() const;

  bool setPlantEquipmentOperationCoolingLoad(const PlantEquipmentOperationCoolingLoad& plantOperation);

  void resetPlantEquipmentOperationCoolingLoad();

  /** Set the hours of operation for which the PlantEquipmentOperationCoolingLoad, if any, applies. **/
  bool setPlantEquipmentOperationCoolingLoadSchedule(Schedule &);

  boost::optional<Schedule> plantEquipmentOperationCoolingLoadSchedule() const;

  void resetPlantEquipmentOperationCoolingLoadSchedule();

  boost::optional<PlantEquipmentOperationScheme> primaryPlantEquipmentOperationScheme() const;

  bool setPrimaryPlantEquipmentOperationScheme(const PlantEquipmentOperationScheme& plantOperation);

  void resetPrimaryPlantEquipmentOperationScheme();

  /** Set the hours of operation for which the PrimaryPlantEquipmentOperationScheme, if any, applies. **/
  bool setPrimaryPlantEquipmentOperationSchemeSchedule(Schedule &);

  void resetPrimaryPlantEquipmentOperationSchemeSchedule();

  boost::optional<Schedule> primaryPlantEquipmentOperationSchemeSchedule() const;

  /** Set the hours of operation for which the ComponentSetpointOperationScheme, if any, applies. 
   *  The existance of ComponentSetpointOperationSchemes is controlled by the existance of 
   *  setpoint managers on the plant component outlet nodes.
  **/
  bool setComponentSetpointOperationSchemeSchedule(Schedule &);

  void resetComponentSetpointOperationSchemeSchedule();

  boost::optional<Schedule> componentSetpointOperationSchemeSchedule() const;

  Node supplyInletNode() const override;

  Node supplyOutletNode() const override;

  std::vector<Node> supplyOutletNodes() const override;

  Node demandInletNode() const override;

  std::vector<Node> demandInletNodes() const override;

  Node demandOutletNode() const override;

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
  bool addDemandBranchForComponent( HVACComponent hvacComponent, bool tertiary = false );

  /** Removes the demand side branch that contains the specified hvacComponent.
   *  Does not remove the component from the model, but may remove surrounding,
   *  components on the branch if they are not attached to other HVAC objects.
   */
  bool removeDemandBranchWithComponent( HVACComponent hvacComponent );

  virtual std::vector<openstudio::IdfObject> remove() override;

  virtual ModelObject clone(Model model) const override;

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
