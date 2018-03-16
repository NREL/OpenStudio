#ifndef ENERGYPLUS_FORWARDTRANSLATOR_FORWARDTRANSLATEPLANTEQUIPMENTOPERATIONSCHEMES_HPP
#define ENERGYPLUS_FORWARDTRANSLATOR_FORWARDTRANSLATEPLANTEQUIPMENTOPERATIONSCHEMES_HPP

#include <vector>
#include <boost/optional.hpp>

namespace openstudio {


namespace model {

  class ModelObject;
  class HVACComponent;
  class PlantLoop;
  class Node;

}

namespace energyplus{

  enum class ComponentType {HEATING, COOLING, BOTH, NONE};

  ComponentType componentType(const openstudio::model::HVACComponent & component);

  bool _isSetpointComponent(const openstudio::model::PlantLoop & plantLoop,const openstudio::model::ModelObject & comp);

  boost::optional<openstudio::model::Node> inletNode(const openstudio::model::PlantLoop & plantLoop, const openstudio::model::HVACComponent & component);

  boost::optional<double> flowrate(const openstudio::model::HVACComponent & component);

  boost::optional<openstudio::model::Node> outletNode(const openstudio::model::PlantLoop & plantLoop, const openstudio::model::HVACComponent & component);

  /*
   * Check the overall type of a plantLoop by checking what is on the supply side
   * The logic is as follows:
   * * If there is only heating, no cooling, no "both": Heating
   * * If there is only cooling, no heating, no "both": Cooling
   * * If there is no cooling, no heating, no "both": None
   * * All other cases: "both"
   */
  ComponentType plantLoopType(const openstudio::model::PlantLoop & plantLoop);

  /* Some plant components air in a containingHVACComponent() and it is that
   * container which needs to go on the plant operation scheme. Here is a filter to figure that out.
   */
  openstudio::model::HVACComponent operationSchemeComponent(const openstudio::model::HVACComponent & component);

  std::vector<openstudio::model::HVACComponent> setpointComponents(const openstudio::model::PlantLoop & plantLoop);

  /* Loops on all plant loop supply side components to find the ones that have a type cooling.
   * Calls operationSchemeComponent(comp) for each comp of type ComponentType::Cooling */
  std::vector<openstudio::model::HVACComponent> coolingComponents(const openstudio::model::PlantLoop & plantLoop);

  std::vector<openstudio::model::HVACComponent> heatingComponents(const openstudio::model::PlantLoop & plantLoop);

  std::vector<openstudio::model::HVACComponent> uncontrolledComponents(const openstudio::model::PlantLoop & plantLoop);


} // energyplus

} // openstudio

#endif // ENERGYPLUS_FORWARDTRANSLATOR_FORWARDTRANSLATEPLANTEQUIPMENTOPERATIONSCHEMES_HPP
