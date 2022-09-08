/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../../model/SpaceInfiltrationFlowCoefficient.hpp"
#include "../../model/SpaceInfiltrationFlowCoefficient_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/SpaceType_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include <utilities/idd/ZoneInfiltration_FlowCoefficient_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateSpaceInfiltrationFlowCoefficient(SpaceInfiltrationFlowCoefficient& modelObject) {

    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::ZoneInfiltration_FlowCoefficient, modelObject);
    IdfObject parentIdfObject = getSpaceLoadParent(modelObject, false);  // We do not allow spaceType!
    idfObject.setString(ZoneInfiltration_FlowCoefficientFields::ZoneorSpaceName, parentIdfObject.nameString());

    boost::optional<Schedule> schedule_ = modelObject.schedule();
    if (!schedule_) {
      schedule_ = modelObject.model().alwaysOnDiscreteSchedule();
    }
    if (auto idf_sch_ = translateAndMapModelObject(schedule_.get())) {
      idfObject.setString(ZoneInfiltration_FlowCoefficientFields::ScheduleName, idf_sch_->nameString());
    }

    // Flow Coefficient: Required Double
    double flowCoefficient = modelObject.flowCoefficient();
    idfObject.setDouble(ZoneInfiltration_FlowCoefficientFields::FlowCoefficient, flowCoefficient);

    // Stack Coefficient: Required Double
    double stackCoefficient = modelObject.stackCoefficient();
    idfObject.setDouble(ZoneInfiltration_FlowCoefficientFields::StackCoefficient, stackCoefficient);

    // Pressure Exponent: Optional Double
    double pressureExponent = modelObject.pressureExponent();
    idfObject.setDouble(ZoneInfiltration_FlowCoefficientFields::PressureExponent, pressureExponent);

    // Wind Coefficient: Required Double
    double windCoefficient = modelObject.windCoefficient();
    idfObject.setDouble(ZoneInfiltration_FlowCoefficientFields::WindCoefficient, windCoefficient);

    // Shelter Factor: Required Double
    double shelterFactor = modelObject.shelterFactor();
    idfObject.setDouble(ZoneInfiltration_FlowCoefficientFields::ShelterFactor, shelterFactor);

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
