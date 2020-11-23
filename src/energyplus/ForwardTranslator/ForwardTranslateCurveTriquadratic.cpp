/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/CurveTriquadratic.hpp"
#include "../../model/CurveTriquadratic_Impl.hpp"

#include "../../utilities/core/Assert.hpp"

#include <utilities/idd/Curve_Triquadratic_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {
namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateCurveTriquadratic(CurveTriquadratic& modelObject) {
    IdfObject idfObject(IddObjectType::Curve_Triquadratic);

    m_idfObjects.push_back(idfObject);

    OptionalString s;
    OptionalDouble d;

    if ((s = modelObject.name())) {
      idfObject.setName(*s);
    }

    if ((d = modelObject.coefficient1Constant())) {
      idfObject.setDouble(Curve_TriquadraticFields::Coefficient1Constant, *d);
    }
    if ((d = modelObject.coefficient2xPOW2())) {
      idfObject.setDouble(Curve_TriquadraticFields::Coefficient2x_POW_2, *d);
    }
    if ((d = modelObject.coefficient3x())) {
      idfObject.setDouble(Curve_TriquadraticFields::Coefficient3x, *d);
    }
    if ((d = modelObject.coefficient4yPOW2())) {
      idfObject.setDouble(Curve_TriquadraticFields::Coefficient4y_POW_2, *d);
    }
    if ((d = modelObject.coefficient5y())) {
      idfObject.setDouble(Curve_TriquadraticFields::Coefficient5y, *d);
    }
    if ((d = modelObject.coefficient6zPOW2())) {
      idfObject.setDouble(Curve_TriquadraticFields::Coefficient6z_POW_2, *d);
    }
    if ((d = modelObject.coefficient7z())) {
      idfObject.setDouble(Curve_TriquadraticFields::Coefficient7z, *d);
    }
    if ((d = modelObject.coefficient8xPOW2TIMESYPOW2())) {
      idfObject.setDouble(Curve_TriquadraticFields::Coefficient8x_POW_2_TIMES_y_POW_2, *d);
    }
    if ((d = modelObject.coefficient9xTIMESY())) {
      idfObject.setDouble(Curve_TriquadraticFields::Coefficient9x_TIMES_y, *d);
    }
    if ((d = modelObject.coefficient10xTIMESYPOW2())) {
      idfObject.setDouble(Curve_TriquadraticFields::Coefficient10x_TIMES_y_POW_2, *d);
    }
    if ((d = modelObject.coefficient11xPOW2TIMESY())) {
      idfObject.setDouble(Curve_TriquadraticFields::Coefficient11x_POW_2_TIMES_y, *d);
    }
    if ((d = modelObject.coefficient12xPOW2TIMESZPOW2())) {
      idfObject.setDouble(Curve_TriquadraticFields::Coefficient12x_POW_2_TIMES_z_POW_2, *d);
    }
    if ((d = modelObject.coefficient13xTIMESZ())) {
      idfObject.setDouble(Curve_TriquadraticFields::Coefficient13x_TIMES_z, *d);
    }
    if ((d = modelObject.coefficient14xTIMESZPOW2())) {
      idfObject.setDouble(Curve_TriquadraticFields::Coefficient14x_TIMES_z_POW_2, *d);
    }
    if ((d = modelObject.coefficient15xPOW2TIMESZ())) {
      idfObject.setDouble(Curve_TriquadraticFields::Coefficient15x_POW_2_TIMES_z, *d);
    }
    if ((d = modelObject.coefficient16yPOW2TIMESZPOW2())) {
      idfObject.setDouble(Curve_TriquadraticFields::Coefficient16y_POW_2_TIMES_z_POW_2, *d);
    }
    if ((d = modelObject.coefficient17yTIMESZ())) {
      idfObject.setDouble(Curve_TriquadraticFields::Coefficient17y_TIMES_z, *d);
    }
    if ((d = modelObject.coefficient18yTIMESZPOW2())) {
      idfObject.setDouble(Curve_TriquadraticFields::Coefficient18y_TIMES_z_POW_2, *d);
    }
    if ((d = modelObject.coefficient19yPOW2TIMESZ())) {
      idfObject.setDouble(Curve_TriquadraticFields::Coefficient19y_POW_2_TIMES_z, *d);
    }
    if ((d = modelObject.coefficient20xPOW2TIMESYPOW2TIMESZPOW2())) {
      idfObject.setDouble(Curve_TriquadraticFields::Coefficient20x_POW_2_TIMES_y_POW_2_TIMES_z_POW_2, *d);
    }
    if ((d = modelObject.coefficient21xPOW2TIMESYPOW2TIMESZ())) {
      idfObject.setDouble(Curve_TriquadraticFields::Coefficient21x_POW_2_TIMES_y_POW_2_TIMES_z, *d);
    }
    if ((d = modelObject.coefficient22xPOW2TIMESYTIMESZPOW2())) {
      idfObject.setDouble(Curve_TriquadraticFields::Coefficient22x_POW_2_TIMES_y_TIMES_z_POW_2, *d);
    }
    if ((d = modelObject.coefficient23xTIMESYPOW2TIMESZPOW2())) {
      idfObject.setDouble(Curve_TriquadraticFields::Coefficient23x_TIMES_y_POW_2_TIMES_z_POW_2, *d);
    }
    if ((d = modelObject.coefficient24xPOW2TIMESYTIMESZ())) {
      idfObject.setDouble(Curve_TriquadraticFields::Coefficient24x_POW_2_TIMES_y_TIMES_z, *d);
    }
    if ((d = modelObject.coefficient25xTIMESYPOW2TIMESZ())) {
      idfObject.setDouble(Curve_TriquadraticFields::Coefficient25x_TIMES_y_POW_2_TIMES_z, *d);
    }
    if ((d = modelObject.coefficient26xTIMESYTIMESZPOW2())) {
      idfObject.setDouble(Curve_TriquadraticFields::Coefficient26x_TIMES_y_TIMES_z_POW_2, *d);
    }
    if ((d = modelObject.coefficient27xTIMESYTIMESZ())) {
      idfObject.setDouble(Curve_TriquadraticFields::Coefficient27x_TIMES_y_TIMES_z, *d);
    }
    if ((d = modelObject.minimumValueofx())) {
      idfObject.setDouble(Curve_TriquadraticFields::MinimumValueofx, *d);
    }
    if ((d = modelObject.maximumValueofx())) {
      idfObject.setDouble(Curve_TriquadraticFields::MaximumValueofx, *d);
    }
    if ((d = modelObject.minimumValueofy())) {
      idfObject.setDouble(Curve_TriquadraticFields::MinimumValueofy, *d);
    }
    if ((d = modelObject.maximumValueofy())) {
      idfObject.setDouble(Curve_TriquadraticFields::MaximumValueofy, *d);
    }
    if ((d = modelObject.minimumValueofz())) {
      idfObject.setDouble(Curve_TriquadraticFields::MinimumValueofz, *d);
    }
    if ((d = modelObject.maximumValueofz())) {
      idfObject.setDouble(Curve_TriquadraticFields::MaximumValueofz, *d);
    }
    if ((d = modelObject.minimumCurveOutput())) {
      idfObject.setDouble(Curve_TriquadraticFields::MinimumCurveOutput, *d);
    }
    if ((d = modelObject.maximumCurveOutput())) {
      idfObject.setDouble(Curve_TriquadraticFields::MaximumCurveOutput, *d);
    }
    if (!modelObject.isInputUnitTypeforXDefaulted()) {
      idfObject.setString(Curve_TriquadraticFields::InputUnitTypeforX, modelObject.inputUnitTypeforX());
    }
    if (!modelObject.isInputUnitTypeforYDefaulted()) {
      idfObject.setString(Curve_TriquadraticFields::InputUnitTypeforY, modelObject.inputUnitTypeforY());
    }
    if (!modelObject.isInputUnitTypeforZDefaulted()) {
      idfObject.setString(Curve_TriquadraticFields::InputUnitTypeforZ, modelObject.inputUnitTypeforZ());
    }
    if (!modelObject.isOutputUnitTypeDefaulted()) {
      idfObject.setString(Curve_TriquadraticFields::OutputUnitType, modelObject.outputUnitType());
    }

    return idfObject;
  }

}  // namespace energyplus
}  // namespace openstudio
