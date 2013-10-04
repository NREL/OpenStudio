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

#include <model/CurveFanPressureRise.hpp>
#include <model/CurveFanPressureRise_Impl.hpp>

#include <utilities/idd/Curve_FanPressureRise_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {
namespace energyplus {

boost::optional<ModelObject> ReverseTranslator::translateCurveFanPressureRise( 
    const WorkspaceObject& workspaceObject )
{
  CurveFanPressureRise curve(m_model);

  OptionalString s;
  OptionalDouble d;
  
  if ((s = workspaceObject.name())) {
    curve.setName(*s);
  }

  if ((d = workspaceObject.getDouble(Curve_FanPressureRiseFields::Coefficient1C1))) {
    curve.setCoefficient1C1(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_FanPressureRiseFields::Coefficient2C2))) {
    curve.setCoefficient2C2(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_FanPressureRiseFields::Coefficient3C3))) {
    curve.setCoefficient3C3(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_FanPressureRiseFields::Coefficient4C4))) {
    curve.setCoefficient4C4(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_FanPressureRiseFields::MinimumValueofQfan))) {
    curve.setMinimumValueofQfan(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_FanPressureRiseFields::MaximumValueofQfan))) {
    curve.setMaximumValueofQfan(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_FanPressureRiseFields::MinimumValueofPsm))) {
    curve.setMinimumValueofPsm(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_FanPressureRiseFields::MaximumValueofPsm))) {
    curve.setMaximumValueofPsm(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_FanPressureRiseFields::MinimumCurveOutput))) {
    curve.setMinimumCurveOutput(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_FanPressureRiseFields::MaximumCurveOutput))) {
    curve.setMaximumCurveOutput(*d);
  }

  return curve;
}

} // energyplus
} // openstudio

