/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
