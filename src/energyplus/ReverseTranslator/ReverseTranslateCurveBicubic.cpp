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

#include "../ReverseTranslator.hpp"

#include "../../model/CurveBicubic.hpp"
#include "../../model/CurveBicubic_Impl.hpp"

#include <utilities/idd/Curve_Bicubic_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {
namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateCurveBicubic(const WorkspaceObject& workspaceObject) {
    CurveBicubic curve(m_model);

    OptionalString s;
    OptionalDouble d;

    if ((s = workspaceObject.name())) {
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
    if ((s = workspaceObject.getString(Curve_BicubicFields::InputUnitTypeforX, false, true))) {
      curve.setInputUnitTypeforX(*s);
    }
    if ((s = workspaceObject.getString(Curve_BicubicFields::InputUnitTypeforY, false, true))) {
      curve.setInputUnitTypeforY(*s);
    }
    if ((s = workspaceObject.getString(Curve_BicubicFields::OutputUnitType, false, true))) {
      curve.setOutputUnitType(*s);
    }

    return curve;
  }

}  // namespace energyplus
}  // namespace openstudio
