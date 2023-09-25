/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/CurveTriquadratic.hpp"
#include "../../model/CurveTriquadratic_Impl.hpp"

#include <utilities/idd/Curve_Triquadratic_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {
namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateCurveTriquadratic(const WorkspaceObject& workspaceObject) {
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
    if ((s = workspaceObject.getString(Curve_TriquadraticFields::InputUnitTypeforX, false, true))) {
      curve.setInputUnitTypeforX(*s);
    }
    if ((s = workspaceObject.getString(Curve_TriquadraticFields::InputUnitTypeforY, false, true))) {
      curve.setInputUnitTypeforY(*s);
    }
    if ((s = workspaceObject.getString(Curve_TriquadraticFields::InputUnitTypeforZ, false, true))) {
      curve.setInputUnitTypeforZ(*s);
    }
    if ((s = workspaceObject.getString(Curve_TriquadraticFields::OutputUnitType, false, true))) {
      curve.setOutputUnitType(*s);
    }

    return curve;
  }

}  // namespace energyplus
}  // namespace openstudio
