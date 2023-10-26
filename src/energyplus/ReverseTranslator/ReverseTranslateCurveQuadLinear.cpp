/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/CurveQuadLinear.hpp"

#include <utilities/idd/Curve_QuadLinear_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateCurveQuadLinear(const WorkspaceObject& workspaceObject) {
    boost::optional<ModelObject> result;

    // Instantiate an object of the class to store the values,
    // but we don't return it until we know it's ok
    // TODO: check constructor, it might need other objects
    openstudio::model::CurveQuadLinear modelObject(m_model);

    // TODO: Note JM 2018-10-17
    // You are responsible for implementing any additional logic based on choice fields, etc.
    // The ReverseTranslator generator script is meant to facilitate your work, not get you 100% of the way

    // Name
    if (boost::optional<std::string> _name = workspaceObject.name()) {
      modelObject.setName(_name.get());
    }

    // Coefficient1 Constant: Required Double
    if (boost::optional<double> _coefficient1Constant = workspaceObject.getDouble(Curve_QuadLinearFields::Coefficient1Constant)) {
      modelObject.setCoefficient1Constant(_coefficient1Constant.get());
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Coefficient1 Constant'");
      return result;
    }

    // Coefficient2 w: Required Double
    if (boost::optional<double> _coefficient2w = workspaceObject.getDouble(Curve_QuadLinearFields::Coefficient2w)) {
      modelObject.setCoefficient2w(_coefficient2w.get());
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Coefficient2 w'");
      return result;
    }

    // Coefficient3 x: Required Double
    if (boost::optional<double> _coefficient3x = workspaceObject.getDouble(Curve_QuadLinearFields::Coefficient3x)) {
      modelObject.setCoefficient3x(_coefficient3x.get());
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Coefficient3 x'");
      return result;
    }

    // Coefficient4 y: Required Double
    if (boost::optional<double> _coefficient4y = workspaceObject.getDouble(Curve_QuadLinearFields::Coefficient4y)) {
      modelObject.setCoefficient4y(_coefficient4y.get());
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Coefficient4 y'");
      return result;
    }

    // Coefficient5 z: Required Double
    if (boost::optional<double> _coefficient5z = workspaceObject.getDouble(Curve_QuadLinearFields::Coefficient5z)) {
      modelObject.setCoefficient5z(_coefficient5z.get());
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Coefficient5 z'");
      return result;
    }

    // Minimum Value of w: Required Double
    if (boost::optional<double> _minimumValueofw = workspaceObject.getDouble(Curve_QuadLinearFields::MinimumValueofw)) {
      modelObject.setMinimumValueofw(_minimumValueofw.get());
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Minimum Value of w'");
      return result;
    }

    // Maximum Value of w: Required Double
    if (boost::optional<double> _maximumValueofw = workspaceObject.getDouble(Curve_QuadLinearFields::MaximumValueofw)) {
      modelObject.setMaximumValueofw(_maximumValueofw.get());
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Maximum Value of w'");
      return result;
    }

    // Minimum Value of x: Required Double
    if (boost::optional<double> _minimumValueofx = workspaceObject.getDouble(Curve_QuadLinearFields::MinimumValueofx)) {
      modelObject.setMinimumValueofx(_minimumValueofx.get());
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Minimum Value of x'");
      return result;
    }

    // Maximum Value of x: Required Double
    if (boost::optional<double> _maximumValueofx = workspaceObject.getDouble(Curve_QuadLinearFields::MaximumValueofx)) {
      modelObject.setMaximumValueofx(_maximumValueofx.get());
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Maximum Value of x'");
      return result;
    }

    // Minimum Value of y: Required Double
    if (boost::optional<double> _minimumValueofy = workspaceObject.getDouble(Curve_QuadLinearFields::MinimumValueofy)) {
      modelObject.setMinimumValueofy(_minimumValueofy.get());
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Minimum Value of y'");
      return result;
    }

    // Maximum Value of y: Required Double
    if (boost::optional<double> _maximumValueofy = workspaceObject.getDouble(Curve_QuadLinearFields::MaximumValueofy)) {
      modelObject.setMaximumValueofy(_maximumValueofy.get());
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Maximum Value of y'");
      return result;
    }

    // Minimum Value of z: Required Double
    if (boost::optional<double> _minimumValueofz = workspaceObject.getDouble(Curve_QuadLinearFields::MinimumValueofz)) {
      modelObject.setMinimumValueofz(_minimumValueofz.get());
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Minimum Value of z'");
      return result;
    }

    // Maximum Value of z: Required Double
    if (boost::optional<double> _maximumValueofz = workspaceObject.getDouble(Curve_QuadLinearFields::MaximumValueofz)) {
      modelObject.setMaximumValueofz(_maximumValueofz.get());
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Maximum Value of z'");
      return result;
    }

    // Minimum Curve Output: Optional Double
    if (boost::optional<double> _minimumCurveOutput = workspaceObject.getDouble(Curve_QuadLinearFields::MinimumCurveOutput)) {
      modelObject.setMinimumCurveOutput(_minimumCurveOutput.get());
    }

    // Maximum Curve Output: Optional Double
    if (boost::optional<double> _maximumCurveOutput = workspaceObject.getDouble(Curve_QuadLinearFields::MaximumCurveOutput)) {
      modelObject.setMaximumCurveOutput(_maximumCurveOutput.get());
    }

    // Input Unit Type for w: Optional String
    if (boost::optional<std::string> _inputUnitTypeforw = workspaceObject.getString(Curve_QuadLinearFields::InputUnitTypeforw)) {
      modelObject.setInputUnitTypeforw(_inputUnitTypeforw.get());
    }

    // Input Unit Type for x: Optional String
    if (boost::optional<std::string> _inputUnitTypeforx = workspaceObject.getString(Curve_QuadLinearFields::InputUnitTypeforx)) {
      modelObject.setInputUnitTypeforx(_inputUnitTypeforx.get());
    }

    // Input Unit Type for y: Optional String
    if (boost::optional<std::string> _inputUnitTypefory = workspaceObject.getString(Curve_QuadLinearFields::InputUnitTypefory)) {
      modelObject.setInputUnitTypefory(_inputUnitTypefory.get());
    }

    // Input Unit Type for z: Optional String
    if (boost::optional<std::string> _inputUnitTypeforz = workspaceObject.getString(Curve_QuadLinearFields::InputUnitTypeforz)) {
      modelObject.setInputUnitTypeforz(_inputUnitTypeforz.get());
    }

    result = modelObject;
    return result;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
