/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/CurveSigmoid.hpp"
#include "../../model/CurveSigmoid_Impl.hpp"

#include "../../utilities/core/Assert.hpp"

#include <utilities/idd/Curve_Sigmoid_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {
namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateCurveSigmoid(
    CurveSigmoid& modelObject)
{
  IdfObject idfObject(IddObjectType::Curve_Sigmoid);

  m_idfObjects.push_back(idfObject);

  OptionalString s;
  OptionalDouble d;

  if ((s = modelObject.name())) {
    idfObject.setName(*s);
  }

  idfObject.setDouble(Curve_SigmoidFields::Coefficient1C1,modelObject.coefficient1C1());
  idfObject.setDouble(Curve_SigmoidFields::Coefficient2C2,modelObject.coefficient2C2());
  idfObject.setDouble(Curve_SigmoidFields::Coefficient3C3,modelObject.coefficient3C3());
  idfObject.setDouble(Curve_SigmoidFields::Coefficient4C4,modelObject.coefficient4C4());
  idfObject.setDouble(Curve_SigmoidFields::Coefficient5C5,modelObject.coefficient5C5());
  idfObject.setDouble(Curve_SigmoidFields::MinimumValueofx,modelObject.minimumValueofx());
  idfObject.setDouble(Curve_SigmoidFields::MaximumValueofx,modelObject.maximumValueofx());
  if ((d = modelObject.minimumCurveOutput())) {
    idfObject.setDouble(Curve_SigmoidFields::MinimumCurveOutput,*d);
  }
  if ((d = modelObject.maximumCurveOutput()) ) {
    idfObject.setDouble(Curve_SigmoidFields::MaximumCurveOutput,*d);
  }
  if (!modelObject.isInputUnitTypeforxDefaulted()) {
    idfObject.setString(Curve_SigmoidFields::InputUnitTypeforx,modelObject.inputUnitTypeforx());
  }
  if (!modelObject.isOutputUnitTypeDefaulted()) {
    idfObject.setString(Curve_SigmoidFields::OutputUnitType,modelObject.outputUnitType());
  }

  return idfObject;
}

} // energyplus
} // openstudio
