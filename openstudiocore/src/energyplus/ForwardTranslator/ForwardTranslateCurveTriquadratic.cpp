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

#include <model/CurveTriquadratic.hpp>
#include <model/CurveTriquadratic_Impl.hpp>

#include <utilities/core/Assert.hpp>

#include <utilities/idd/Curve_Triquadratic_FieldEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {
namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateCurveTriquadratic(CurveTriquadratic& modelObject)
{
  IdfObject idfObject(IddObjectType::Curve_Triquadratic);

  m_idfObjects.push_back(idfObject);

  OptionalString s;
  OptionalDouble d;

  if((s = modelObject.name())) {
    idfObject.setName(*s);
  }

  if ((d = modelObject.coefficient1Constant())) {
    idfObject.setDouble(Curve_TriquadraticFields::Coefficient1Constant,*d);
  }
  if ((d = modelObject.coefficient2xPOW2())) {
    idfObject.setDouble(Curve_TriquadraticFields::Coefficient2x_POW_2,*d);
  }
  if ((d = modelObject.coefficient3x())) {
    idfObject.setDouble(Curve_TriquadraticFields::Coefficient3x,*d);
  }
  if ((d = modelObject.coefficient4yPOW2())) {
    idfObject.setDouble(Curve_TriquadraticFields::Coefficient4y_POW_2,*d);
  }
  if ((d = modelObject.coefficient5y())) {
    idfObject.setDouble(Curve_TriquadraticFields::Coefficient5y,*d);
  }
  if ((d = modelObject.coefficient6zPOW2())) {
    idfObject.setDouble(Curve_TriquadraticFields::Coefficient6z_POW_2,*d);
  }
  if ((d = modelObject.coefficient7z())) {
    idfObject.setDouble(Curve_TriquadraticFields::Coefficient7z,*d);
  }
  if ((d = modelObject.coefficient8xPOW2TIMESYPOW2())) {
    idfObject.setDouble(Curve_TriquadraticFields::Coefficient8x_POW_2_TIMES_y_POW_2,*d);
  }
  if ((d = modelObject.coefficient9xTIMESY())) {
    idfObject.setDouble(Curve_TriquadraticFields::Coefficient9x_TIMES_y,*d);
  }
  if ((d = modelObject.coefficient10xTIMESYPOW2())) {
    idfObject.setDouble(Curve_TriquadraticFields::Coefficient10x_TIMES_y_POW_2,*d);
  }
  if ((d = modelObject.coefficient11xPOW2TIMESY())) {
    idfObject.setDouble(Curve_TriquadraticFields::Coefficient11x_POW_2_TIMES_y,*d);
  }
  if ((d = modelObject.coefficient12xPOW2TIMESZPOW2())) {
    idfObject.setDouble(Curve_TriquadraticFields::Coefficient12x_POW_2_TIMES_z_POW_2,*d);
  }
  if ((d = modelObject.coefficient13xTIMESZ())) {
    idfObject.setDouble(Curve_TriquadraticFields::Coefficient13x_TIMES_z,*d);
  }
  if ((d = modelObject.coefficient14xTIMESZPOW2())) {
    idfObject.setDouble(Curve_TriquadraticFields::Coefficient14x_TIMES_z_POW_2,*d);
  }
  if ((d = modelObject.coefficient15xPOW2TIMESZ())) {
    idfObject.setDouble(Curve_TriquadraticFields::Coefficient15x_POW_2_TIMES_z,*d);
  }
  if ((d = modelObject.coefficient16yPOW2TIMESZPOW2())) {
    idfObject.setDouble(Curve_TriquadraticFields::Coefficient16y_POW_2_TIMES_z_POW_2,*d);
  }
  if ((d = modelObject.coefficient17yTIMESZ())) {
    idfObject.setDouble(Curve_TriquadraticFields::Coefficient17y_TIMES_z,*d);
  }
  if ((d = modelObject.coefficient18yTIMESZPOW2())) {
    idfObject.setDouble(Curve_TriquadraticFields::Coefficient18y_TIMES_z_POW_2,*d);
  }
  if ((d = modelObject.coefficient19yPOW2TIMESZ())) {
    idfObject.setDouble(Curve_TriquadraticFields::Coefficient19y_POW_2_TIMES_z,*d);
  }
  if ((d = modelObject.coefficient20xPOW2TIMESYPOW2TIMESZPOW2())) {
    idfObject.setDouble(Curve_TriquadraticFields::Coefficient20x_POW_2_TIMES_y_POW_2_TIMES_z_POW_2,*d);
  }
  if ((d = modelObject.coefficient21xPOW2TIMESYPOW2TIMESZ())) {
    idfObject.setDouble(Curve_TriquadraticFields::Coefficient21x_POW_2_TIMES_y_POW_2_TIMES_z,*d);
  }
  if ((d = modelObject.coefficient22xPOW2TIMESYTIMESZPOW2())) {
    idfObject.setDouble(Curve_TriquadraticFields::Coefficient22x_POW_2_TIMES_y_TIMES_z_POW_2,*d);
  }
  if ((d = modelObject.coefficient23xTIMESYPOW2TIMESZPOW2())) {
    idfObject.setDouble(Curve_TriquadraticFields::Coefficient23x_TIMES_y_POW_2_TIMES_z_POW_2,*d);
  }
  if ((d = modelObject.coefficient24xPOW2TIMESYTIMESZ())) {
    idfObject.setDouble(Curve_TriquadraticFields::Coefficient24x_POW_2_TIMES_y_TIMES_z,*d);
  }
  if ((d = modelObject.coefficient25xTIMESYPOW2TIMESZ())) {
    idfObject.setDouble(Curve_TriquadraticFields::Coefficient25x_TIMES_y_POW_2_TIMES_z,*d);
  }
  if ((d = modelObject.coefficient26xTIMESYTIMESZPOW2())) {
    idfObject.setDouble(Curve_TriquadraticFields::Coefficient26x_TIMES_y_TIMES_z_POW_2,*d);
  }
  if ((d = modelObject.coefficient27xTIMESYTIMESZ())) {
    idfObject.setDouble(Curve_TriquadraticFields::Coefficient27x_TIMES_y_TIMES_z,*d);
  }
  if ((d = modelObject.minimumValueofx())) {
    idfObject.setDouble(Curve_TriquadraticFields::MinimumValueofx,*d);
  }
  if ((d = modelObject.maximumValueofx())) {
    idfObject.setDouble(Curve_TriquadraticFields::MaximumValueofx,*d); 
  }
  if ((d = modelObject.minimumValueofy())) {
    idfObject.setDouble(Curve_TriquadraticFields::MinimumValueofy,*d);
  }
  if ((d = modelObject.maximumValueofy())) {
    idfObject.setDouble(Curve_TriquadraticFields::MaximumValueofy,*d);
  }
  if ((d = modelObject.minimumValueofz())) {
    idfObject.setDouble(Curve_TriquadraticFields::MinimumValueofz,*d);
  }
  if ((d = modelObject.maximumValueofz())) {
    idfObject.setDouble(Curve_TriquadraticFields::MaximumValueofz,*d);
  }
  if ((d = modelObject.minimumCurveOutput())) {
    idfObject.setDouble(Curve_TriquadraticFields::MinimumCurveOutput,*d);
  }
  if ((d = modelObject.maximumCurveOutput())) {
    idfObject.setDouble(Curve_TriquadraticFields::MaximumCurveOutput,*d);
  }
  if (!modelObject.isInputUnitTypeforXDefaulted()) {
    idfObject.setString(Curve_TriquadraticFields::InputUnitTypeforX,modelObject.inputUnitTypeforX());
  }
  if (!modelObject.isInputUnitTypeforYDefaulted()) {
    idfObject.setString(Curve_TriquadraticFields::InputUnitTypeforY,modelObject.inputUnitTypeforY());
  }
  if (!modelObject.isInputUnitTypeforZDefaulted()) {
    idfObject.setString(Curve_TriquadraticFields::InputUnitTypeforZ,modelObject.inputUnitTypeforZ());
  }
  if (!modelObject.isOutputUnitTypeDefaulted()) {
    idfObject.setString(Curve_TriquadraticFields::OutputUnitType,modelObject.outputUnitType());
  }

  return idfObject;
}

} // energyplus
} // openstudio
