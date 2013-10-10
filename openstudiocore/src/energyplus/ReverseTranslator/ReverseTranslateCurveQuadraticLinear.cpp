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

#include <energyplus/ReverseTranslator.hpp>

#include <model/CurveQuadraticLinear.hpp>
#include <model/CurveQuadraticLinear_Impl.hpp>

#include <utilities/idd/Curve_QuadraticLinear_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {
namespace energyplus {

boost::optional<ModelObject> ReverseTranslator::translateCurveQuadraticLinear( 
    const WorkspaceObject& workspaceObject)
{
  CurveQuadraticLinear curve(m_model);

  OptionalString s;
  OptionalDouble d;
  
  if ((s = workspaceObject.name())) {
    curve.setName(*s);
  }

  if ((d = workspaceObject.getDouble(Curve_QuadraticLinearFields::Coefficient1Constant))) {
    curve.setCoefficient1Constant(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_QuadraticLinearFields::Coefficient2x))) {
    curve.setCoefficient2x(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_QuadraticLinearFields::Coefficient3x_POW_2))) {
    curve.setCoefficient3xPOW2(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_QuadraticLinearFields::Coefficient4y))) {
    curve.setCoefficient4y(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_QuadraticLinearFields::Coefficient5x_TIMES_y))) {
    curve.setCoefficient5xTIMESY(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_QuadraticLinearFields::Coefficient6x_POW_2_TIMES_y))) {
    curve.setCoefficient6xPOW2TIMESY(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_QuadraticLinearFields::MinimumValueofx))) {
    curve.setMinimumValueofx(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_QuadraticLinearFields::MaximumValueofx))) {
    curve.setMaximumValueofx(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_QuadraticLinearFields::MinimumValueofy))) {
    curve.setMinimumValueofy(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_QuadraticLinearFields::MaximumValueofy))) {
    curve.setMaximumValueofy(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_QuadraticLinearFields::MinimumCurveOutput))) {
    curve.setMinimumCurveOutput(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_QuadraticLinearFields::MaximumCurveOutput))) {
    curve.setMaximumCurveOutput(*d);
  }
  if ((s = workspaceObject.getString(Curve_QuadraticLinearFields::InputUnitTypeforX,false,true))) {
    curve.setInputUnitTypeforX(*s);
  }
  if ((s = workspaceObject.getString(Curve_QuadraticLinearFields::InputUnitTypeforY,false,true))) {
    curve.setInputUnitTypeforY(*s);
  }
  if ((s = workspaceObject.getString(Curve_QuadraticLinearFields::OutputUnitType,false,true))) {
    curve.setOutputUnitType(*s);
  }

  return curve;
}

} // energyplus
} // openstudio
