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

#include <model/CurveCubic.hpp>
#include <model/CurveCubic_Impl.hpp>

#include <utilities/idd/Curve_Cubic_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {
namespace energyplus {

boost::optional<ModelObject> ReverseTranslator::translateCurveCubic( 
    const WorkspaceObject& workspaceObject )
{
  CurveCubic curve(m_model);

  OptionalString s;
  OptionalDouble d;
  
  if ((s = workspaceObject.name())) {
    curve.setName(*s);
  }

  if ((d = workspaceObject.getDouble(Curve_CubicFields::Coefficient1Constant))) {
    curve.setCoefficient1Constant(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_CubicFields::Coefficient2x))) {
    curve.setCoefficient2x(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_CubicFields::Coefficient3x_POW_2))) {
    curve.setCoefficient3xPOW2(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_CubicFields::Coefficient4x_POW_3))) {
    curve.setCoefficient4xPOW3(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_CubicFields::MinimumValueofx))) {
    curve.setMinimumValueofx(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_CubicFields::MaximumValueofx))) {
    curve.setMaximumValueofx(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_CubicFields::MinimumCurveOutput))) {
    curve.setMinimumCurveOutput(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_CubicFields::MaximumCurveOutput))) {
    curve.setMaximumCurveOutput(*d);
  }
  if ((s = workspaceObject.getString(Curve_CubicFields::InputUnitTypeforX,false,true))) {
    curve.setInputUnitTypeforX(*s);
  }
  if ((s = workspaceObject.getString(Curve_CubicFields::OutputUnitType,false,true))) {
    curve.setOutputUnitType(*s);
  }

  return curve;
}

} // energyplus
} // openstudio

