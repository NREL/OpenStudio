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

#include "../../model/WaterHeaterSizing.hpp"

#include "../../model/WaterToWaterComponent.hpp"

#include <utilities/idd/WaterHeater_Sizing_FieldEnums.hxx>
// #include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateWaterHeaterSizing(model::WaterHeaterSizing& modelObject) {

    // It doesn't have a name
    IdfObject idfObject(openstudio::IddObjectType::WaterHeater_Sizing);
    // WaterHeater Name: Required Object
    auto waterHeater = modelObject.waterHeater();
    if (boost::optional<IdfObject> _owo = translateAndMapModelObject(waterHeater)) {
      idfObject.setString(WaterHeater_SizingFields::WaterHeaterName, _owo->nameString());
    } else {
      return boost::none;
    }
    m_idfObjects.push_back(idfObject);

    // Design Mode: boost::optional<std::string>
    if (boost::optional<std::string> _designMode = modelObject.designMode()) {
      idfObject.setString(WaterHeater_SizingFields::DesignMode, _designMode.get());
    }

    // Time Storage Can Meet Peak Draw: boost::optional<double>
    if (boost::optional<double> _timeStorageCanMeetPeakDraw = modelObject.timeStorageCanMeetPeakDraw()) {
      idfObject.setDouble(WaterHeater_SizingFields::TimeStorageCanMeetPeakDraw, _timeStorageCanMeetPeakDraw.get());
    }

    // Time for Tank Recovery: boost::optional<double>
    if (boost::optional<double> _timeforTankRecovery = modelObject.timeforTankRecovery()) {
      idfObject.setDouble(WaterHeater_SizingFields::TimeforTankRecovery, _timeforTankRecovery.get());
    }

    // Nominal Tank Volume for Autosizing Plant Connections: boost::optional<double>
    if (boost::optional<double> _nominalTankVolumeforAutosizingPlantConnections = modelObject.nominalTankVolumeforAutosizingPlantConnections()) {
      idfObject.setDouble(WaterHeater_SizingFields::NominalTankVolumeforAutosizingPlantConnections,
                          _nominalTankVolumeforAutosizingPlantConnections.get());
    }

    // Number of Bedrooms: boost::optional<int>
    if (boost::optional<int> _numberofBedrooms = modelObject.numberofBedrooms()) {
      idfObject.setInt(WaterHeater_SizingFields::NumberofBedrooms, _numberofBedrooms.get());
    }

    // Number of Bathrooms: boost::optional<int>
    if (boost::optional<int> _numberofBathrooms = modelObject.numberofBathrooms()) {
      idfObject.setInt(WaterHeater_SizingFields::NumberofBathrooms, _numberofBathrooms.get());
    }

    // Storage Capacity per Person: boost::optional<double>
    if (boost::optional<double> _storageCapacityperPerson = modelObject.storageCapacityperPerson()) {
      idfObject.setDouble(WaterHeater_SizingFields::StorageCapacityperPerson, _storageCapacityperPerson.get());
    }

    // Recovery Capacity per Person: boost::optional<double>
    if (boost::optional<double> _recoveryCapacityperPerson = modelObject.recoveryCapacityperPerson()) {
      idfObject.setDouble(WaterHeater_SizingFields::RecoveryCapacityperPerson, _recoveryCapacityperPerson.get());
    }

    // Storage Capacity per Floor Area: boost::optional<double>
    if (boost::optional<double> _storageCapacityperFloorArea = modelObject.storageCapacityperFloorArea()) {
      idfObject.setDouble(WaterHeater_SizingFields::StorageCapacityperFloorArea, _storageCapacityperFloorArea.get());
    }

    // Recovery Capacity per Floor Area: boost::optional<double>
    if (boost::optional<double> _recoveryCapacityperFloorArea = modelObject.recoveryCapacityperFloorArea()) {
      idfObject.setDouble(WaterHeater_SizingFields::RecoveryCapacityperFloorArea, _recoveryCapacityperFloorArea.get());
    }

    // Number of Units: boost::optional<double>
    if (boost::optional<double> _numberofUnits = modelObject.numberofUnits()) {
      idfObject.setDouble(WaterHeater_SizingFields::NumberofUnits, _numberofUnits.get());
    }

    // Storage Capacity per Unit: boost::optional<double>
    if (boost::optional<double> _storageCapacityperUnit = modelObject.storageCapacityperUnit()) {
      idfObject.setDouble(WaterHeater_SizingFields::StorageCapacityperUnit, _storageCapacityperUnit.get());
    }

    // Recovery Capacity PerUnit: boost::optional<double>
    if (boost::optional<double> _recoveryCapacityPerUnit = modelObject.recoveryCapacityPerUnit()) {
      idfObject.setDouble(WaterHeater_SizingFields::RecoveryCapacityPerUnit, _recoveryCapacityPerUnit.get());
    }

    // Storage Capacity per Collector Area: boost::optional<double>
    if (boost::optional<double> _storageCapacityperCollectorArea = modelObject.storageCapacityperCollectorArea()) {
      idfObject.setDouble(WaterHeater_SizingFields::StorageCapacityperCollectorArea, _storageCapacityperCollectorArea.get());
    }

    // Height Aspect Ratio: boost::optional<double>
    if (boost::optional<double> _heightAspectRatio = modelObject.heightAspectRatio()) {
      idfObject.setDouble(WaterHeater_SizingFields::HeightAspectRatio, _heightAspectRatio.get());
    }

    return idfObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
