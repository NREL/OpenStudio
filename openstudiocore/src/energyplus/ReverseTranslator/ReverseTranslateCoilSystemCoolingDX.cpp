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

OptionalModelObject ReverseTranslator::translateCoilSystemCoolingDX( const WorkspaceObject & workspaceObject )
{
  if( workspaceObject.iddObject().type() != IddObjectType::CoilSystem_Cooling_DX )
  {
     LOG(Error, "WorkspaceObject is not IddObjectType: CoilSystem:Cooling:DX");
     return boost::none;
  }

  Workspace workspace = workspaceObject.workspace();

  boost::optional<std::string> coolingCoilObjectType;
  boost::optional<std::string> coolingCoilName;

  coolingCoilObjectType = workspaceObject.getString(CoilSystem_Cooling_DXFields::CoolingCoilObjectType);
  coolingCoilName = workspaceObject.getString(CoilSystem_Cooling_DXFields::CoolingCoilName);

  boost::optional<WorkspaceObject> wo;

  if( coolingCoilObjectType && coolingCoilName )
  {
    wo = workspace.getObjectByTypeAndName(IddObjectType(coolingCoilObjectType.get()),coolingCoilName.get());
  }

  if( wo )
  {
    return translateAndMapWorkspaceObject(wo.get());
  }
  else
  {
    return boost::none;
  }
}

} // energyplus

} // openstudio

