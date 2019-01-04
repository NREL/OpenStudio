/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../../model/CurveBicubic.hpp"
#include "../../model/CurveBicubic_Impl.hpp"

#include "../../utilities/core/Assert.hpp"

#include <utilities/idd/Curve_Bicubic_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {
namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateCurveBicubic(CurveBicubic& modelObject)
{
  IdfObject idfObject(IddObjectType::Curve_Bicubic);

  m_idfObjects.push_back(idfObject);

  OptionalString s;
  OptionalDouble d;

  if((s = modelObject.name())) {
    idfObject.setName(*s);
  }

  idfObject.setDouble(Curve_BicubicFields::Coefficient1Constant,modelObject.coefficient1Constant());
  idfObject.setDouble(Curve_BicubicFields::Coefficient2x,modelObject.coefficient2x());
  idfObject.setDouble(Curve_BicubicFields::Coefficient3x_POW_2,modelObject.coefficient3xPOW2());
  idfObject.setDouble(Curve_BicubicFields::Coefficient4y,modelObject.coefficient4y());
  idfObject.setDouble(Curve_BicubicFields::Coefficient5y_POW_2,modelObject.coefficient5yPOW2());
  idfObject.setDouble(Curve_BicubicFields::Coefficient6x_TIMES_y,modelObject.coefficient6xTIMESY());
  idfObject.setDouble(Curve_BicubicFields::Coefficient7x_POW_3,modelObject.coefficient7xPOW3());
  idfObject.setDouble(Curve_BicubicFields::Coefficient8y_POW_3,modelObject.coefficient8yPOW3());
  idfObject.setDouble(Curve_BicubicFields::Coefficient9x_POW_2_TIMES_y,modelObject.coefficient9xPOW2TIMESY());
  idfObject.setDouble(Curve_BicubicFields::Coefficient10x_TIMES_y_POW_2,modelObject.coefficient10xTIMESYPOW2());
  idfObject.setDouble(Curve_BicubicFields::MinimumValueofx,modelObject.minimumValueofx());
  idfObject.setDouble(Curve_BicubicFields::MaximumValueofx,modelObject.maximumValueofx());
  idfObject.setDouble(Curve_BicubicFields::MinimumValueofy,modelObject.minimumValueofy());
  idfObject.setDouble(Curve_BicubicFields::MaximumValueofy,modelObject.maximumValueofy());
  if ((d = modelObject.minimumCurveOutput())) {
    idfObject.setDouble(Curve_BicubicFields::MinimumCurveOutput,*d);
  }
  if ((d = modelObject.maximumCurveOutput())) {
    idfObject.setDouble(Curve_BicubicFields::MaximumCurveOutput,*d);
  }
  if (!modelObject.isInputUnitTypeforXDefaulted()) {
    idfObject.setString(Curve_BicubicFields::InputUnitTypeforX,modelObject.inputUnitTypeforX());
  }
  if (!modelObject.isInputUnitTypeforYDefaulted()) {
    idfObject.setString(Curve_BicubicFields::InputUnitTypeforY,modelObject.inputUnitTypeforY());
  }
  if (!modelObject.isOutputUnitTypeDefaulted()) {
    idfObject.setString(Curve_BicubicFields::OutputUnitType,modelObject.outputUnitType());
  }

  return idfObject;
}

} // energyplus
} // openstudio

