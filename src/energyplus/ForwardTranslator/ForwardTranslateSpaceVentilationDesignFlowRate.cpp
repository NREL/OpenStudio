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
#include "../../model/SpaceVentilationDesignFlowRate.hpp"
#include "../../model/SpaceVentilationDesignFlowRate_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/SpaceType_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include <utilities/idd/ZoneVentilation_DesignFlowRate_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateSpaceVentilationDesignFlowRate(SpaceVentilationDesignFlowRate& modelObject) {

    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::ZoneVentilation_DesignFlowRate, modelObject);
    IdfObject parentIdfObject = getSpaceLoadParent(modelObject, true);  // We **do** allow spaceType!
    idfObject.setString(ZoneVentilation_DesignFlowRateFields::ZoneorZoneListorSpaceorSpaceListName, parentIdfObject.nameString());

    boost::optional<Schedule> schedule = modelObject.schedule();
    if (schedule) {
      idfObject.setString(ZoneVentilation_DesignFlowRateFields::ScheduleName, schedule->nameString());
    }

    std::string designFlowRateCalculationMethod = modelObject.designFlowRateCalculationMethod();
    if (istringEqual("Flow/Space", designFlowRateCalculationMethod)) {
      designFlowRateCalculationMethod = "Flow/Zone";
    }
    idfObject.setString(ZoneVentilation_DesignFlowRateFields::DesignFlowRateCalculationMethod, designFlowRateCalculationMethod);

    OptionalDouble d = modelObject.designFlowRate();
    if (d) {
      idfObject.setDouble(ZoneVentilation_DesignFlowRateFields::DesignFlowRate, *d);
    }

    d = modelObject.flowperSpaceFloorArea();
    if (d) {
      idfObject.setDouble(ZoneVentilation_DesignFlowRateFields::FlowRateperFloorArea, *d);
    }

    d = modelObject.flowperExteriorSurfaceArea();
    if (d) {
      idfObject.setDouble(ZoneVentilation_DesignFlowRateFields::FlowRateperExteriorSurfaceArea, *d);
    }

    d = modelObject.flowperExteriorWallArea();
    if (d) {
      idfObject.setDouble(ZoneVentilation_DesignFlowRateFields::FlowRateperExteriorSurfaceArea, *d);
    }

    d = modelObject.airChangesperHour();
    if (d) {
      idfObject.setDouble(ZoneVentilation_DesignFlowRateFields::AirChangesperHour, *d);
    }

    if (!modelObject.isConstantTermCoefficientDefaulted()) {
      idfObject.setDouble(ZoneVentilation_DesignFlowRateFields::ConstantTermCoefficient, modelObject.constantTermCoefficient());
    }

    if (!modelObject.isTemperatureTermCoefficientDefaulted()) {
      idfObject.setDouble(ZoneVentilation_DesignFlowRateFields::TemperatureTermCoefficient, modelObject.temperatureTermCoefficient());
    }

    if (!modelObject.isVelocityTermCoefficientDefaulted()) {
      idfObject.setDouble(ZoneVentilation_DesignFlowRateFields::VelocityTermCoefficient, modelObject.velocityTermCoefficient());
    }

    if (!modelObject.isVelocitySquaredTermCoefficientDefaulted()) {
      idfObject.setDouble(ZoneVentilation_DesignFlowRateFields::VelocitySquaredTermCoefficient, modelObject.velocitySquaredTermCoefficient());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
