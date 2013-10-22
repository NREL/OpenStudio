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

#include <model/CurveRectangularHyperbola1.hpp>
#include <model/CurveRectangularHyperbola1_Impl.hpp>

#include <utilities/core/Assert.hpp>

#include <utilities/idd/Curve_RectangularHyperbola1_FieldEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {
namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateCurveRectangularHyperbola1( 
    CurveRectangularHyperbola1& modelObject)
{
  IdfObject idfObject(IddObjectType::Curve_RectangularHyperbola1);

  m_idfObjects.push_back(idfObject);

  OptionalString s;
  OptionalDouble d;

  if ((s = modelObject.name())) {
    idfObject.setName(*s);
  }

  idfObject.setDouble(Curve_RectangularHyperbola1Fields::Coefficient1C1,modelObject.coefficient1C1());
  idfObject.setDouble(Curve_RectangularHyperbola1Fields::Coefficient2C2,modelObject.coefficient2C2());
  idfObject.setDouble(Curve_RectangularHyperbola1Fields::Coefficient3C3,modelObject.coefficient3C3());
  idfObject.setDouble(Curve_RectangularHyperbola1Fields::MinimumValueofx,modelObject.minimumValueofx());
  idfObject.setDouble(Curve_RectangularHyperbola1Fields::MaximumValueofx,modelObject.maximumValueofx());
  if ((d = modelObject.minimumCurveOutput())) {
    idfObject.setDouble(Curve_RectangularHyperbola1Fields::MinimumCurveOutput,*d);
  }
  if ((d = modelObject.maximumCurveOutput())) {
    idfObject.setDouble(Curve_RectangularHyperbola1Fields::MaximumCurveOutput,*d);
  }
  if (!modelObject.isInputUnitTypeforxDefaulted()) {
    idfObject.setString(Curve_RectangularHyperbola1Fields::InputUnitTypeforx,modelObject.inputUnitTypeforx());
  }
  if (!modelObject.isOutputUnitTypeDefaulted()) {
    idfObject.setString(Curve_RectangularHyperbola1Fields::OutputUnitType,modelObject.outputUnitType());
  }

  return idfObject;
}

} // energyplus
} // openstudio
