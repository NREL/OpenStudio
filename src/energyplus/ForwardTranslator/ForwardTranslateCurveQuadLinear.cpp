/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"

#include "../../model/CurveQuadLinear.hpp"

#include <utilities/idd/Curve_QuadLinear_FieldEnums.hxx>
// #include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateCurveQuadLinear(model::CurveQuadLinear& modelObject) {
    boost::optional<WorkspaceObject> result;

    // Instantiate an IdfObject of the class to store the values,
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::Curve_QuadLinear, modelObject);
    // If it doesn't have a name, or if you aren't sure you are going to want to return it
    // IdfObject idfObject( openstudio::IddObjectType::Curve_QuadLinear );
    // m_idfObjects.push_back(idfObject);

    // TODO: Note JM 2018-10-17
    // You are responsible for implementing any additional logic based on choice fields, etc.
    // The ForwardTranslator generator script is meant to facilitate your work, not get you 100% of the way

    // TODO: If you keep createRegisterAndNameIdfObject above, you don't need this.
    // But in some cases, you'll want to handle failure without pushing to the map
    // Name
    if (boost::optional<std::string> moName = modelObject.name()) {
      idfObject.setName(*moName);
    }

    // Coefficient1 Constant: Required Double
    double coefficient1Constant = modelObject.coefficient1Constant();
    idfObject.setDouble(Curve_QuadLinearFields::Coefficient1Constant, coefficient1Constant);

    // Coefficient2 w: Required Double
    double coefficient2w = modelObject.coefficient2w();
    idfObject.setDouble(Curve_QuadLinearFields::Coefficient2w, coefficient2w);

    // Coefficient3 x: Required Double
    double coefficient3x = modelObject.coefficient3x();
    idfObject.setDouble(Curve_QuadLinearFields::Coefficient3x, coefficient3x);

    // Coefficient4 y: Required Double
    double coefficient4y = modelObject.coefficient4y();
    idfObject.setDouble(Curve_QuadLinearFields::Coefficient4y, coefficient4y);

    // Coefficient5 z: Required Double
    double coefficient5z = modelObject.coefficient5z();
    idfObject.setDouble(Curve_QuadLinearFields::Coefficient5z, coefficient5z);

    // Minimum Value of w: Required Double
    double minimumValueofw = modelObject.minimumValueofw();
    idfObject.setDouble(Curve_QuadLinearFields::MinimumValueofw, minimumValueofw);

    // Maximum Value of w: Required Double
    double maximumValueofw = modelObject.maximumValueofw();
    idfObject.setDouble(Curve_QuadLinearFields::MaximumValueofw, maximumValueofw);

    // Minimum Value of x: Required Double
    double minimumValueofx = modelObject.minimumValueofx();
    idfObject.setDouble(Curve_QuadLinearFields::MinimumValueofx, minimumValueofx);

    // Maximum Value of x: Required Double
    double maximumValueofx = modelObject.maximumValueofx();
    idfObject.setDouble(Curve_QuadLinearFields::MaximumValueofx, maximumValueofx);

    // Minimum Value of y: Required Double
    double minimumValueofy = modelObject.minimumValueofy();
    idfObject.setDouble(Curve_QuadLinearFields::MinimumValueofy, minimumValueofy);

    // Maximum Value of y: Required Double
    double maximumValueofy = modelObject.maximumValueofy();
    idfObject.setDouble(Curve_QuadLinearFields::MaximumValueofy, maximumValueofy);

    // Minimum Value of z: Required Double
    double minimumValueofz = modelObject.minimumValueofz();
    idfObject.setDouble(Curve_QuadLinearFields::MinimumValueofz, minimumValueofz);

    // Maximum Value of z: Required Double
    double maximumValueofz = modelObject.maximumValueofz();
    idfObject.setDouble(Curve_QuadLinearFields::MaximumValueofz, maximumValueofz);

    // Minimum Curve Output: boost::optional<double>
    if (boost::optional<double> _minimumCurveOutput = modelObject.minimumCurveOutput()) {
      idfObject.setDouble(Curve_QuadLinearFields::MinimumCurveOutput, _minimumCurveOutput.get());
    }

    // Maximum Curve Output: boost::optional<double>
    if (boost::optional<double> _maximumCurveOutput = modelObject.maximumCurveOutput()) {
      idfObject.setDouble(Curve_QuadLinearFields::MaximumCurveOutput, _maximumCurveOutput.get());
    }

    // Input Unit Type for w: Optional String
    std::string inputUnitTypeforw = modelObject.inputUnitTypeforw();
    idfObject.setString(Curve_QuadLinearFields::InputUnitTypeforw, inputUnitTypeforw);

    // Input Unit Type for x: Optional String
    std::string inputUnitTypeforx = modelObject.inputUnitTypeforx();
    idfObject.setString(Curve_QuadLinearFields::InputUnitTypeforx, inputUnitTypeforx);

    // Input Unit Type for y: Optional String
    std::string inputUnitTypefory = modelObject.inputUnitTypefory();
    idfObject.setString(Curve_QuadLinearFields::InputUnitTypefory, inputUnitTypefory);

    // Input Unit Type for z: Optional String
    std::string inputUnitTypeforz = modelObject.inputUnitTypeforz();
    idfObject.setString(Curve_QuadLinearFields::InputUnitTypeforz, inputUnitTypeforz);

    return idfObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
