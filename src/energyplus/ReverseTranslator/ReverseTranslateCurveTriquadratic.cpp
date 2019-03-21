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

#include "../ReverseTranslator.hpp"

#include "../../model/CurveTriquadratic.hpp"
#include "../../model/CurveTriquadratic_Impl.hpp"

#include <utilities/idd/Curve_Triquadratic_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {
namespace energyplus {

boost::optional<ModelObject> ReverseTranslator::translateCurveTriquadratic(
    const WorkspaceObject& workspaceObject)
{
  CurveTriquadratic curve(m_model);

  OptionalString s;
  OptionalDouble d;

  if ((s = workspaceObject.name())) {
    curve.setName(*s);
  }

  if ((d = workspaceObject.getDouble(Curve_TriquadraticFields::Coefficient1Constant))) {
    curve.setCoefficient1Constant(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_TriquadraticFields::Coefficient2x_POW_2))) {
    curve.setCoefficient2xPOW2(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_TriquadraticFields::Coefficient3x))) {
    curve.setCoefficient3x(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_TriquadraticFields::Coefficient4y_POW_2))) {
    curve.setCoefficient4yPOW2(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_TriquadraticFields::Coefficient5y))) {
    curve.setCoefficient5y(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_TriquadraticFields::Coefficient6z_POW_2))) {
    curve.setCoefficient6zPOW2(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_TriquadraticFields::Coefficient7z))) {
    curve.setCoefficient7z(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_TriquadraticFields::Coefficient8x_POW_2_TIMES_y_POW_2))) {
    curve.setCoefficient8xPOW2TIMESYPOW2(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_TriquadraticFields::Coefficient9x_TIMES_y))) {
    curve.setCoefficient9xTIMESY(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_TriquadraticFields::Coefficient10x_TIMES_y_POW_2))) {
    curve.setCoefficient10xTIMESYPOW2(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_TriquadraticFields::Coefficient11x_POW_2_TIMES_y))) {
    curve.setCoefficient11xPOW2TIMESY(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_TriquadraticFields::Coefficient12x_POW_2_TIMES_z_POW_2))) {
    curve.setCoefficient12xPOW2TIMESZPOW2(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_TriquadraticFields::Coefficient13x_TIMES_z))) {
    curve.setCoefficient13xTIMESZ(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_TriquadraticFields::Coefficient14x_TIMES_z_POW_2))) {
    curve.setCoefficient14xTIMESZPOW2(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_TriquadraticFields::Coefficient15x_POW_2_TIMES_z))) {
    curve.setCoefficient15xPOW2TIMESZ(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_TriquadraticFields::Coefficient16y_POW_2_TIMES_z_POW_2))) {
    curve.setCoefficient16yPOW2TIMESZPOW2(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_TriquadraticFields::Coefficient17y_TIMES_z))) {
    curve.setCoefficient17yTIMESZ(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_TriquadraticFields::Coefficient18y_TIMES_z_POW_2))) {
    curve.setCoefficient18yTIMESZPOW2(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_TriquadraticFields::Coefficient19y_POW_2_TIMES_z))) {
    curve.setCoefficient19yPOW2TIMESZ(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_TriquadraticFields::Coefficient20x_POW_2_TIMES_y_POW_2_TIMES_z_POW_2))) {
    curve.setCoefficient20xPOW2TIMESYPOW2TIMESZPOW2(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_TriquadraticFields::Coefficient21x_POW_2_TIMES_y_POW_2_TIMES_z))) {
    curve.setCoefficient21xPOW2TIMESYPOW2TIMESZ(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_TriquadraticFields::Coefficient22x_POW_2_TIMES_y_TIMES_z_POW_2))) {
    curve.setCoefficient22xPOW2TIMESYTIMESZPOW2(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_TriquadraticFields::Coefficient23x_TIMES_y_POW_2_TIMES_z_POW_2))) {
    curve.setCoefficient23xTIMESYPOW2TIMESZPOW2(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_TriquadraticFields::Coefficient24x_POW_2_TIMES_y_TIMES_z))) {
    curve.setCoefficient24xPOW2TIMESYTIMESZ(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_TriquadraticFields::Coefficient25x_TIMES_y_POW_2_TIMES_z))) {
    curve.setCoefficient25xTIMESYPOW2TIMESZ(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_TriquadraticFields::Coefficient26x_TIMES_y_TIMES_z_POW_2))) {
    curve.setCoefficient26xTIMESYTIMESZPOW2(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_TriquadraticFields::Coefficient27x_TIMES_y_TIMES_z))) {
    curve.setCoefficient27xTIMESYTIMESZ(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_TriquadraticFields::MinimumValueofx))) {
    curve.setMinimumValueofx(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_TriquadraticFields::MaximumValueofx))) {
    curve.setMaximumValueofx(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_TriquadraticFields::MinimumValueofy))) {
    curve.setMinimumValueofy(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_TriquadraticFields::MaximumValueofy))) {
    curve.setMaximumValueofy(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_TriquadraticFields::MinimumValueofz))) {
    curve.setMinimumValueofz(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_TriquadraticFields::MaximumValueofz))) {
    curve.setMaximumValueofz(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_TriquadraticFields::MinimumCurveOutput))) {
    curve.setMinimumCurveOutput(*d);
  }
  if ((d = workspaceObject.getDouble(Curve_TriquadraticFields::MaximumCurveOutput))) {
    curve.setMaximumCurveOutput(*d);
  }
  if ((s = workspaceObject.getString(Curve_TriquadraticFields::InputUnitTypeforX,false,true))) {
    curve.setInputUnitTypeforX(*s);
  }
  if ((s = workspaceObject.getString(Curve_TriquadraticFields::InputUnitTypeforY,false,true))) {
    curve.setInputUnitTypeforY(*s);
  }
  if ((s = workspaceObject.getString(Curve_TriquadraticFields::InputUnitTypeforZ,false,true))) {
    curve.setInputUnitTypeforZ(*s);
  }
  if ((s = workspaceObject.getString(Curve_TriquadraticFields::OutputUnitType,false,true))) {
    curve.setOutputUnitType(*s);
  }

  return curve;
}

} // energyplus
} // openstudio
