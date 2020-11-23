/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../ReverseTranslator.hpp"
#include "../../model/AirLoopHVACOutdoorAirSystem.hpp"
#include "../../model/AirLoopHVACOutdoorAirSystem_Impl.hpp"
#include "../../model/ControllerOutdoorAir.hpp"
#include "../../model/ControllerOutdoorAir_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include <utilities/idd/AirLoopHVAC_OutdoorAirSystem_FieldEnums.hxx>
#include <utilities/idd/OutdoorAir_Mixer_FieldEnums.hxx>
#include <utilities/idd/EvaporativeCooler_Direct_ResearchSpecial_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateAirLoopHVACOutdoorAirSystem(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::AirLoopHVAC_OutdoorAirSystem) {
      LOG(Error, "WorkspaceObject is not IddObjectType: AirLoopHVAC_OutdoorAirSystem");
      return boost::none;
    }

    Workspace _workspace = workspaceObject.workspace();

    boost::optional<WorkspaceObject> _controllerList;
    boost::optional<WorkspaceObject> _controllerOutdoorAir;
    boost::optional<std::string> controllerName;
    boost::optional<std::string> controllerType;
    boost::optional<ControllerOutdoorAir> oaController;

    _controllerList = workspaceObject.getTarget(AirLoopHVAC_OutdoorAirSystemFields::ControllerListName);

    if (_controllerList) {
      for (int i = 1; _controllerList->getString(i); i = i + 2) {
        controllerType = _controllerList->getString(i);
        controllerName = _controllerList->getString(i + 1);
        if (controllerType) {
          if (istringEqual(controllerType.get(), "Controller:OutdoorAir")) {
            break;
          }
        }
      }
    }

    if (controllerName && controllerType) {
      boost::optional<WorkspaceObject> wo = _workspace.getObjectByTypeAndName(IddObjectType(controllerType.get()), controllerName.get());
      if (wo) {
        boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(wo.get());
        if (mo) {
          oaController = mo->optionalCast<ControllerOutdoorAir>();
        }
      }
    }

    if (oaController) {
      AirLoopHVACOutdoorAirSystem mo(m_model, oaController.get());

      boost::optional<std::string> name = workspaceObject.getString(AirLoopHVAC_OutdoorAirSystemFields::Name);
      if (name) {
        mo.setName(name.get());
      }

      Node outboardOANode = mo.outboardOANode().get();

      boost::optional<WorkspaceObject> _oaEquipmentList;
      boost::optional<WorkspaceObject> _outdoorAirMixer;
      std::vector<WorkspaceObject> equipmentVector;
      _oaEquipmentList = workspaceObject.getTarget(AirLoopHVAC_OutdoorAirSystemFields::OutdoorAirEquipmentListName);

      if (_oaEquipmentList) {
        for (int i = 1; _oaEquipmentList->getString(i); i = i + 2) {
          boost::optional<std::string> equipmentName;
          boost::optional<std::string> equipmentType;

          equipmentType = _oaEquipmentList->getString(i);
          equipmentName = _oaEquipmentList->getString(i + 1);

          if (equipmentName && equipmentType) {
            boost::optional<WorkspaceObject> wo = _workspace.getObjectByTypeAndName(IddObjectType(equipmentType.get()), equipmentName.get());
            if (wo) {
              equipmentVector.push_back(wo.get());

              if (wo->iddObject().type() == IddObjectType::OutdoorAir_Mixer) {
                _outdoorAirMixer = wo;
              }
            }
          }
        }
        if (_outdoorAirMixer) {
          boost::optional<std::string> mixerOAInletNodeName = _outdoorAirMixer->getString(OutdoorAir_MixerFields::OutdoorAirStreamNodeName);
          boost::optional<std::string> mixerOAReliefNodeName = _outdoorAirMixer->getString(OutdoorAir_MixerFields::ReliefAirStreamNodeName);
          if (mixerOAInletNodeName) {
            mo.outdoorAirModelObject()->cast<Node>().setName(mixerOAInletNodeName.get());
          }
          if (mixerOAReliefNodeName) {
            mo.reliefAirModelObject()->cast<Node>().setName(mixerOAReliefNodeName.get());
          }

          boost::optional<std::string> oaStreamInletNodeName;
          boost::optional<std::string> oaStreamOutletNodeName;

          oaStreamInletNodeName = _outdoorAirMixer->getString(OutdoorAir_MixerFields::OutdoorAirStreamNodeName);

          while (oaStreamInletNodeName) {
            boost::optional<ModelObject> oaComponentModelObject;
            boost::optional<std::string> newOAStreamInletNodeName;

            for (const auto& equipmentElem : equipmentVector) {
              switch (equipmentElem.iddObject().type().value()) {
                case openstudio::IddObjectType::EvaporativeCooler_Direct_ResearchSpecial: {
                  oaStreamOutletNodeName = equipmentElem.getString(EvaporativeCooler_Direct_ResearchSpecialFields::AirOutletNodeName);
                  if (oaStreamOutletNodeName) {
                    if (istringEqual(oaStreamOutletNodeName.get(), oaStreamInletNodeName.get())) {
                      newOAStreamInletNodeName = equipmentElem.getString(EvaporativeCooler_Direct_ResearchSpecialFields::AirInletNodeName);

                      oaComponentModelObject = translateAndMapWorkspaceObject(equipmentElem);
                    }
                  }
                  break;
                }
                default: {
                  break;
                }
              }
              if (newOAStreamInletNodeName) {
                break;
              }
            }

            oaStreamInletNodeName = newOAStreamInletNodeName;

            if (oaComponentModelObject) {
              bool success = false;

              if (boost::optional<HVACComponent> hvacComponent = oaComponentModelObject->optionalCast<HVACComponent>()) {
                success = hvacComponent->addToNode(outboardOANode);

                if (success) {
                  if (boost::optional<StraightComponent> straightComponent = hvacComponent->optionalCast<StraightComponent>()) {
                    if (oaStreamInletNodeName) {
                      straightComponent->inletModelObject()->cast<Node>().setName(oaStreamInletNodeName.get());
                    }
                    if (oaStreamOutletNodeName) {
                      straightComponent->outletModelObject()->cast<Node>().setName(oaStreamOutletNodeName.get());
                    }
                  }
                }
              }

              if (!success) {
                oaComponentModelObject->remove();
              }
            }
          }
        }
      }

      return mo;
    } else {
      return boost::none;
    }
  }

}  // namespace energyplus

}  // namespace openstudio
