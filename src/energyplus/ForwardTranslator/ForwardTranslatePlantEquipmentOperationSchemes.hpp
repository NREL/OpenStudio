/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef ENERGYPLUS_FORWARDTRANSLATOR_FORWARDTRANSLATEPLANTEQUIPMENTOPERATIONSCHEMES_HPP
#define ENERGYPLUS_FORWARDTRANSLATOR_FORWARDTRANSLATEPLANTEQUIPMENTOPERATIONSCHEMES_HPP

#include <vector>
#include <boost/optional.hpp>
#include "../EnergyPlusAPI.hpp"
#include "../../utilities/core/Deprecated.hpp"

namespace openstudio {

class ComponentType;

namespace model {

  class ModelObject;
  class HVACComponent;
  class PlantLoop;
  class Node;

}  // namespace model

namespace energyplus {

  enum class ComponentType
  {
    HEATING,
    COOLING,
    BOTH,
    NONE
  };

  OS_DEPRECATED ENERGYPLUS_API openstudio::energyplus::ComponentType componentType(const openstudio::model::HVACComponent& component);

  bool _isSetpointComponent(const openstudio::model::PlantLoop& plantLoop, const openstudio::model::ModelObject& comp);

  ENERGYPLUS_API boost::optional<openstudio::model::Node> inletNode(const openstudio::model::PlantLoop& plantLoop,
                                                                    const openstudio::model::HVACComponent& component);

  ENERGYPLUS_API boost::optional<double> flowrate(const openstudio::model::HVACComponent& component);

  ENERGYPLUS_API boost::optional<openstudio::model::Node> outletNode(const openstudio::model::PlantLoop& plantLoop,
                                                                     const openstudio::model::HVACComponent& component);

  /*
   * Check the overall type of a plantLoop by checking what is on the supply side
   * The logic is as follows:
   * * If there is only heating, no cooling, no "both": Heating
   * * If there is only cooling, no heating, no "both": Cooling
   * * If there is no cooling, no heating, no "both": None
   * * All other cases: "both"
   */
  OS_DEPRECATED ENERGYPLUS_API openstudio::energyplus::ComponentType plantLoopType(const openstudio::model::PlantLoop& plantLoop);

  /* Some plant components air in a containingHVACComponent() and it is that
   * container which needs to go on the plant operation scheme. Here is a filter to figure that out.
   */
  ENERGYPLUS_API openstudio::model::HVACComponent operationSchemeComponent(const openstudio::model::HVACComponent& component);

  ENERGYPLUS_API std::vector<openstudio::model::HVACComponent> setpointComponents(const openstudio::model::PlantLoop& plantLoop);

  /* Loops on all plant loop supply side components to find the ones that have a type cooling.
   * Calls operationSchemeComponent(comp) for each comp of type ComponentType::Cooling */
  ENERGYPLUS_API std::vector<openstudio::model::HVACComponent> coolingComponents(const openstudio::model::PlantLoop& plantLoop);

  ENERGYPLUS_API std::vector<openstudio::model::HVACComponent> heatingComponents(const openstudio::model::PlantLoop& plantLoop);

  ENERGYPLUS_API std::vector<openstudio::model::HVACComponent> uncontrolledComponents(const openstudio::model::PlantLoop& plantLoop);

}  // namespace energyplus

}  // namespace openstudio

#endif  // ENERGYPLUS_FORWARDTRANSLATOR_FORWARDTRANSLATEPLANTEQUIPMENTOPERATIONSCHEMES_HPP
