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

#include <model/CurveBicubic.hpp>
#include <model/CurveBicubic_Impl.hpp>

#include <utilities/idd/Curve_Bicubic_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {
namespace energyplus {

boost::optional<ModelObject> ReverseTranslator::translateCurveBicubic( 
    const WorkspaceObject& workspaceObject)
{
  CurveBicubic curve(m_model);

  OptionalString s;
  OptionalDouble d;
  
  if((s = workspaceObject.name())) {
    curve.setName(*s);
  }

  if ((d = workspaceObject.getDouble(Curve_BicubicFields::Coefficient1Constant))) {
    curve.setCoefficient1Constant(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_BicubicFields::Coefficient2x))) {
    curve.setCoefficient2x(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_BicubicFields::Coefficient3x_POW_2))) {
    curve.setCoefficient3xPOW2(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_BicubicFields::Coefficient4y))) {
    curve.setCoefficient4y(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_BicubicFields::Coefficient5y_POW_2))) {
    curve.setCoefficient5yPOW2(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_BicubicFields::Coefficient6x_TIMES_y))) {
    curve.setCoefficient6xTIMESY(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_BicubicFields::Coefficient7x_POW_3))) {
    curve.setCoefficient7xPOW3(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_BicubicFields::Coefficient8y_POW_3))) {
    curve.setCoefficient8yPOW3(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_BicubicFields::Coefficient9x_POW_2_TIMES_y))) {
    curve.setCoefficient9xPOW2TIMESY(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_BicubicFields::Coefficient10x_TIMES_y_POW_2))) {
    curve.setCoefficient10xTIMESYPOW2(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_BicubicFields::MinimumValueofx))) {
    curve.setMinimumValueofx(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_BicubicFields::MaximumValueofx))) {
    curve.setMaximumValueofx(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_BicubicFields::MinimumValueofy))) {
    curve.setMinimumValueofy(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_BicubicFields::MaximumValueofy))) {
    curve.setMaximumValueofy(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_BicubicFields::MinimumCurveOutput))) {
    curve.setMinimumCurveOutput(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_BicubicFields::MaximumCurveOutput))) {
    curve.setMaximumCurveOutput(*d);
  }
  if ((s = workspaceObject.getString(Curve_BicubicFields::InputUnitTypeforX,false,true))) {
    curve.setInputUnitTypeforX(*s);
  }
  if ((s = workspaceObject.getString(Curve_BicubicFields::InputUnitTypeforY,false,true))) {
    curve.setInputUnitTypeforY(*s);
  }
  if ((s = workspaceObject.getString(Curve_BicubicFields::OutputUnitType,false,true))) {
    curve.setOutputUnitType(*s);
  }

  return curve;
}

} // energyplus
} // openstudio
