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

#include <model/CurveExponent.hpp>
#include <model/CurveExponent_Impl.hpp>

#include <utilities/core/Assert.hpp>

#include <utilities/idd/Curve_Exponent_FieldEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {
namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateCurveExponent( 
    CurveExponent& modelObject)
{
  IdfObject idfObject(IddObjectType::Curve_Exponent);

  m_idfObjects.push_back(idfObject);

  OptionalString s;
  OptionalDouble d;

  if ((s = modelObject.name())) {
    idfObject.setName(*s);
  }

  idfObject.setDouble(Curve_ExponentFields::Coefficient1Constant,modelObject.coefficient1Constant());
  idfObject.setDouble(Curve_ExponentFields::Coefficient2Constant,modelObject.coefficient2Constant());
  idfObject.setDouble(Curve_ExponentFields::Coefficient3Constant,modelObject.coefficient3Constant());
  idfObject.setDouble(Curve_ExponentFields::MinimumValueofx,modelObject.minimumValueofx());
  idfObject.setDouble(Curve_ExponentFields::MaximumValueofx,modelObject.maximumValueofx());
  if ((d = modelObject.minimumCurveOutput())) {
    idfObject.setDouble(Curve_ExponentFields::MinimumCurveOutput,*d);
  }
  if ((d = modelObject.maximumCurveOutput())) {
    idfObject.setDouble(Curve_ExponentFields::MaximumCurveOutput,*d);
  }
  if (!modelObject.isInputUnitTypeforXDefaulted()) {
    idfObject.setString(Curve_ExponentFields::InputUnitTypeforX,modelObject.inputUnitTypeforX());
  }
  if (!modelObject.isOutputUnitTypeDefaulted()) {
    idfObject.setString(Curve_ExponentFields::OutputUnitType,modelObject.outputUnitType());
  }

  return idfObject;
}

} // energyplus
} // openstudio
