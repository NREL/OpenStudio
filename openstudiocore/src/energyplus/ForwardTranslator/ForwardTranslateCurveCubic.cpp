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

#include <energyplus/ForwardTranslator.hpp>

#include <model/CurveCubic.hpp>
#include <model/CurveCubic_Impl.hpp>

#include <utilities/core/Assert.hpp>

#include <utilities/idd/Curve_Cubic_FieldEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {
namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateCurveCubic( 
    CurveCubic& modelObject)
{
  IdfObject idfObject(IddObjectType::Curve_Cubic);

  m_idfObjects.push_back(idfObject);

  OptionalString s;
  OptionalDouble d;

  if ((s = modelObject.name())) {
    idfObject.setName(*s);
  }

  idfObject.setDouble(Curve_CubicFields::Coefficient1Constant,modelObject.coefficient1Constant());
  idfObject.setDouble(Curve_CubicFields::Coefficient2x,modelObject.coefficient2x());
  idfObject.setDouble(Curve_CubicFields::Coefficient3x_POW_2,modelObject.coefficient3xPOW2());
  idfObject.setDouble(Curve_CubicFields::Coefficient4x_POW_3,modelObject.coefficient4xPOW3());
  idfObject.setDouble(Curve_CubicFields::MinimumValueofx,modelObject.minimumValueofx());
  idfObject.setDouble(Curve_CubicFields::MaximumValueofx,modelObject.maximumValueofx());
  if ((d = modelObject.minimumCurveOutput())) {
    idfObject.setDouble(Curve_CubicFields::MinimumCurveOutput,*d);
  }
  if ((d = modelObject.maximumCurveOutput())) {
    idfObject.setDouble(Curve_CubicFields::MaximumCurveOutput,*d);
  }
  if (!modelObject.isInputUnitTypeforXDefaulted()) {
    idfObject.setString(Curve_CubicFields::InputUnitTypeforX,modelObject.inputUnitTypeforX());
  }
  if (!modelObject.isOutputUnitTypeDefaulted()) {
    idfObject.setString(Curve_CubicFields::OutputUnitType,modelObject.outputUnitType());
  }

  return idfObject;
}

} // energyplus
} // openstudio
