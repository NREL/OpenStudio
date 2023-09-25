/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"
#include "../../model/CoilCoolingDXSingleSpeed.hpp"
#include "../../model/CoilCoolingDXSingleSpeed_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/CurveBiquadratic.hpp"
#include "../../model/CurveBiquadratic_Impl.hpp"
#include "../../model/CurveQuadratic.hpp"
#include "../../model/CurveQuadratic_Impl.hpp"
#include <utilities/idd/CoilSystem_Cooling_DX_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateCoilSystemCoolingDX(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::CoilSystem_Cooling_DX) {
      LOG(Error, "WorkspaceObject is not IddObjectType: CoilSystem:Cooling:DX");
      return boost::none;
    }

    Workspace workspace = workspaceObject.workspace();

    boost::optional<std::string> coolingCoilObjectType;
    boost::optional<std::string> coolingCoilName;

    coolingCoilObjectType = workspaceObject.getString(CoilSystem_Cooling_DXFields::CoolingCoilObjectType);
    coolingCoilName = workspaceObject.getString(CoilSystem_Cooling_DXFields::CoolingCoilName);

    boost::optional<WorkspaceObject> wo;

    if (coolingCoilObjectType && coolingCoilName) {
      wo = workspace.getObjectByTypeAndName(IddObjectType(coolingCoilObjectType.get()), coolingCoilName.get());
    }

    if (wo) {
      return translateAndMapWorkspaceObject(wo.get());
    } else {
      return boost::none;
    }
  }

}  // namespace energyplus

}  // namespace openstudio
