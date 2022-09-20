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
#include "../../model/CoilHeatingElectricMultiStage.hpp"
#include "../../model/CoilHeatingElectricMultiStage_Impl.hpp"
#include "../../model/CoilHeatingElectricMultiStageStageData.hpp"
#include "../../model/CoilHeatingElectricMultiStageStageData_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include <utilities/idd/Coil_Heating_Electric_MultiStage_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateCoilHeatingElectricMultiStage(CoilHeatingElectricMultiStage& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> value;

    // Name
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::Coil_Heating_Electric_MultiStage, modelObject);

    // AvailabilityScheduleName
    {
      auto schedule = modelObject.availabilitySchedule();
      if (boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule)) {
        idfObject.setString(Coil_Heating_Electric_MultiStageFields::AvailabilityScheduleName, _schedule->name().get());
      }
    }

    // AirInletNodeName
    if (auto node = modelObject.inletModelObject()) {
      idfObject.setString(Coil_Heating_Electric_MultiStageFields::AirInletNodeName, node->name().get());
    }

    // AirOutletNodeName
    // TemperatureSetpointNodeName
    if (auto node = modelObject.outletModelObject()) {
      idfObject.setString(Coil_Heating_Electric_MultiStageFields::AirOutletNodeName, node->name().get());
      idfObject.setString(Coil_Heating_Electric_MultiStageFields::TemperatureSetpointNodeName, node->name().get());
    }

    // NumberofStages
    {
      auto num = modelObject.stages().size();
      idfObject.setInt(Coil_Heating_Electric_MultiStageFields::NumberofStages, num);
    }

    for (auto stage : modelObject.stages()) {
      auto eg = idfObject.pushExtensibleGroup();

      // Stage1Efficiency
      if ((value = stage.efficiency())) {
        eg.setDouble(Coil_Heating_Electric_MultiStageExtensibleFields::StageEfficiency, value.get());
      }

      // Stage1NominalCapacity
      if (stage.isNominalCapacityAutosized()) {
        eg.setString(Coil_Heating_Electric_MultiStageExtensibleFields::StageNominalCapacity, "AutoSize");
      } else if ((value = stage.nominalCapacity())) {
        eg.setDouble(Coil_Heating_Electric_MultiStageExtensibleFields::StageNominalCapacity, value.get());
      }
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
