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

#include "../ReverseTranslator.hpp"

#include "../../model/FuelFactors.hpp"

#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include <utilities/idd/FuelFactors_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateFuelFactors(const WorkspaceObject& workspaceObject) {
    boost::optional<WorkspaceObject> _wo;
    boost::optional<ModelObject> _mo;

    // Existing Fuel Resource Name: Optional String
    std::string resourceName;
    if (boost::optional<std::string> _existingFuelResourceName = workspaceObject.getString(FuelFactorsFields::ExistingFuelResourceName)) {
      resourceName = _existingFuelResourceName.get();
    } else {
      return boost::none;
    }

    openstudio::model::FuelFactors modelObject(m_model);
    modelObject.setExistingFuelResourceName(resourceName);

    // Source Energy Factor: Optional Double
    if (boost::optional<double> _sourceEnergyFactor = workspaceObject.getDouble(FuelFactorsFields::SourceEnergyFactor)) {
      modelObject.setSourceEnergyFactor(_sourceEnergyFactor.get());
    }

    // Source Energy Schedule Name: Optional Object
    if ((_wo = workspaceObject.getTarget(FuelFactorsFields::SourceEnergyScheduleName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        if (boost::optional<Schedule> _sourceEnergySchedule = _mo->optionalCast<Schedule>()) {
          modelObject.setSourceEnergySchedule(_sourceEnergySchedule.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Source Energy Schedule Name'");
        }
      }
    }
    // CO2 Emission Factor: Optional Double
    if (boost::optional<double> _cO2EmissionFactor = workspaceObject.getDouble(FuelFactorsFields::CO2EmissionFactor)) {
      modelObject.setCO2EmissionFactor(_cO2EmissionFactor.get());
    }

    // CO2 Emission Factor Schedule Name: Optional Object
    if ((_wo = workspaceObject.getTarget(FuelFactorsFields::CO2EmissionFactorScheduleName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        if (boost::optional<Schedule> _cO2EmissionFactorSchedule = _mo->optionalCast<Schedule>()) {
          modelObject.setCO2EmissionFactorSchedule(_cO2EmissionFactorSchedule.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'CO2 Emission Factor Schedule Name'");
        }
      }
    }
    // CO Emission Factor: Optional Double
    if (boost::optional<double> _cOEmissionFactor = workspaceObject.getDouble(FuelFactorsFields::COEmissionFactor)) {
      modelObject.setCOEmissionFactor(_cOEmissionFactor.get());
    }

    // CO Emission Factor Schedule Name: Optional Object
    if ((_wo = workspaceObject.getTarget(FuelFactorsFields::COEmissionFactorScheduleName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        if (boost::optional<Schedule> _cOEmissionFactorSchedule = _mo->optionalCast<Schedule>()) {
          modelObject.setCOEmissionFactorSchedule(_cOEmissionFactorSchedule.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'CO Emission Factor Schedule Name'");
        }
      }
    }
    // CH4 Emission Factor: Optional Double
    if (boost::optional<double> _cH4EmissionFactor = workspaceObject.getDouble(FuelFactorsFields::CH4EmissionFactor)) {
      modelObject.setCH4EmissionFactor(_cH4EmissionFactor.get());
    }

    // CH4 Emission Factor Schedule Name: Optional Object
    if ((_wo = workspaceObject.getTarget(FuelFactorsFields::CH4EmissionFactorScheduleName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        if (boost::optional<Schedule> _cH4EmissionFactorSchedule = _mo->optionalCast<Schedule>()) {
          modelObject.setCH4EmissionFactorSchedule(_cH4EmissionFactorSchedule.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'CH4 Emission Factor Schedule Name'");
        }
      }
    }
    // NOx Emission Factor: Optional Double
    if (boost::optional<double> _nOxEmissionFactor = workspaceObject.getDouble(FuelFactorsFields::NOxEmissionFactor)) {
      modelObject.setNOxEmissionFactor(_nOxEmissionFactor.get());
    }

    // NOx Emission Factor Schedule Name: Optional Object
    if ((_wo = workspaceObject.getTarget(FuelFactorsFields::NOxEmissionFactorScheduleName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        if (boost::optional<Schedule> _nOxEmissionFactorSchedule = _mo->optionalCast<Schedule>()) {
          modelObject.setNOxEmissionFactorSchedule(_nOxEmissionFactorSchedule.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'NOx Emission Factor Schedule Name'");
        }
      }
    }
    // N2O Emission Factor: Optional Double
    if (boost::optional<double> _n2OEmissionFactor = workspaceObject.getDouble(FuelFactorsFields::N2OEmissionFactor)) {
      modelObject.setN2OEmissionFactor(_n2OEmissionFactor.get());
    }

    // N2O Emission Factor Schedule Name: Optional Object
    if ((_wo = workspaceObject.getTarget(FuelFactorsFields::N2OEmissionFactorScheduleName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        if (boost::optional<Schedule> _n2OEmissionFactorSchedule = _mo->optionalCast<Schedule>()) {
          modelObject.setN2OEmissionFactorSchedule(_n2OEmissionFactorSchedule.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'N2O Emission Factor Schedule Name'");
        }
      }
    }
    // SO2 Emission Factor: Optional Double
    if (boost::optional<double> _sO2EmissionFactor = workspaceObject.getDouble(FuelFactorsFields::SO2EmissionFactor)) {
      modelObject.setSO2EmissionFactor(_sO2EmissionFactor.get());
    }

    // SO2 Emission Factor Schedule Name: Optional Object
    if ((_wo = workspaceObject.getTarget(FuelFactorsFields::SO2EmissionFactorScheduleName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        if (boost::optional<Schedule> _sO2EmissionFactorSchedule = _mo->optionalCast<Schedule>()) {
          modelObject.setSO2EmissionFactorSchedule(_sO2EmissionFactorSchedule.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'SO2 Emission Factor Schedule Name'");
        }
      }
    }
    // PM Emission Factor: Optional Double
    if (boost::optional<double> _pMEmissionFactor = workspaceObject.getDouble(FuelFactorsFields::PMEmissionFactor)) {
      modelObject.setPMEmissionFactor(_pMEmissionFactor.get());
    }

    // PM Emission Factor Schedule Name: Optional Object
    if ((_wo = workspaceObject.getTarget(FuelFactorsFields::PMEmissionFactorScheduleName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        if (boost::optional<Schedule> _pMEmissionFactorSchedule = _mo->optionalCast<Schedule>()) {
          modelObject.setPMEmissionFactorSchedule(_pMEmissionFactorSchedule.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'PM Emission Factor Schedule Name'");
        }
      }
    }
    // PM10 Emission Factor: Optional Double
    if (boost::optional<double> _pM10EmissionFactor = workspaceObject.getDouble(FuelFactorsFields::PM10EmissionFactor)) {
      modelObject.setPM10EmissionFactor(_pM10EmissionFactor.get());
    }

    // PM10 Emission Factor Schedule Name: Optional Object
    if ((_wo = workspaceObject.getTarget(FuelFactorsFields::PM10EmissionFactorScheduleName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        if (boost::optional<Schedule> _pM10EmissionFactorSchedule = _mo->optionalCast<Schedule>()) {
          modelObject.setPM10EmissionFactorSchedule(_pM10EmissionFactorSchedule.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'PM10 Emission Factor Schedule Name'");
        }
      }
    }
    // PM2.5 Emission Factor: Optional Double
    if (boost::optional<double> _pM25EmissionFactor = workspaceObject.getDouble(FuelFactorsFields::PM25EmissionFactor)) {
      modelObject.setPM25EmissionFactor(_pM25EmissionFactor.get());
    }

    // PM2.5 Emission Factor Schedule Name: Optional Object
    if ((_wo = workspaceObject.getTarget(FuelFactorsFields::PM25EmissionFactorScheduleName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        if (boost::optional<Schedule> _pM25EmissionFactorSchedule = _mo->optionalCast<Schedule>()) {
          modelObject.setPM25EmissionFactorSchedule(_pM25EmissionFactorSchedule.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'PM2.5 Emission Factor Schedule Name'");
        }
      }
    }
    // NH3 Emission Factor: Optional Double
    if (boost::optional<double> _nH3EmissionFactor = workspaceObject.getDouble(FuelFactorsFields::NH3EmissionFactor)) {
      modelObject.setNH3EmissionFactor(_nH3EmissionFactor.get());
    }

    // NH3 Emission Factor Schedule Name: Optional Object
    if ((_wo = workspaceObject.getTarget(FuelFactorsFields::NH3EmissionFactorScheduleName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        if (boost::optional<Schedule> _nH3EmissionFactorSchedule = _mo->optionalCast<Schedule>()) {
          modelObject.setNH3EmissionFactorSchedule(_nH3EmissionFactorSchedule.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'NH3 Emission Factor Schedule Name'");
        }
      }
    }
    // NMVOC Emission Factor: Optional Double
    if (boost::optional<double> _nMVOCEmissionFactor = workspaceObject.getDouble(FuelFactorsFields::NMVOCEmissionFactor)) {
      modelObject.setNMVOCEmissionFactor(_nMVOCEmissionFactor.get());
    }

    // NMVOC Emission Factor Schedule Name: Optional Object
    if ((_wo = workspaceObject.getTarget(FuelFactorsFields::NMVOCEmissionFactorScheduleName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        if (boost::optional<Schedule> _nMVOCEmissionFactorSchedule = _mo->optionalCast<Schedule>()) {
          modelObject.setNMVOCEmissionFactorSchedule(_nMVOCEmissionFactorSchedule.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'NMVOC Emission Factor Schedule Name'");
        }
      }
    }
    // Hg Emission Factor: Optional Double
    if (boost::optional<double> _hgEmissionFactor = workspaceObject.getDouble(FuelFactorsFields::HgEmissionFactor)) {
      modelObject.setHgEmissionFactor(_hgEmissionFactor.get());
    }

    // Hg Emission Factor Schedule Name: Optional Object
    if ((_wo = workspaceObject.getTarget(FuelFactorsFields::HgEmissionFactorScheduleName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        if (boost::optional<Schedule> _hgEmissionFactorSchedule = _mo->optionalCast<Schedule>()) {
          modelObject.setHgEmissionFactorSchedule(_hgEmissionFactorSchedule.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Hg Emission Factor Schedule Name'");
        }
      }
    }
    // Pb Emission Factor: Optional Double
    if (boost::optional<double> _pbEmissionFactor = workspaceObject.getDouble(FuelFactorsFields::PbEmissionFactor)) {
      modelObject.setPbEmissionFactor(_pbEmissionFactor.get());
    }

    // Pb Emission Factor Schedule Name: Optional Object
    if ((_wo = workspaceObject.getTarget(FuelFactorsFields::PbEmissionFactorScheduleName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        if (boost::optional<Schedule> _pbEmissionFactorSchedule = _mo->optionalCast<Schedule>()) {
          modelObject.setPbEmissionFactorSchedule(_pbEmissionFactorSchedule.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Pb Emission Factor Schedule Name'");
        }
      }
    }
    // Water Emission Factor: Optional Double
    if (boost::optional<double> _waterEmissionFactor = workspaceObject.getDouble(FuelFactorsFields::WaterEmissionFactor)) {
      modelObject.setWaterEmissionFactor(_waterEmissionFactor.get());
    }

    // Water Emission Factor Schedule Name: Optional Object
    if ((_wo = workspaceObject.getTarget(FuelFactorsFields::WaterEmissionFactorScheduleName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        if (boost::optional<Schedule> _waterEmissionFactorSchedule = _mo->optionalCast<Schedule>()) {
          modelObject.setWaterEmissionFactorSchedule(_waterEmissionFactorSchedule.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Water Emission Factor Schedule Name'");
        }
      }
    }
    // Nuclear High Level Emission Factor: Optional Double
    if (boost::optional<double> _nuclearHighLevelEmissionFactor = workspaceObject.getDouble(FuelFactorsFields::NuclearHighLevelEmissionFactor)) {
      modelObject.setNuclearHighLevelEmissionFactor(_nuclearHighLevelEmissionFactor.get());
    }

    // Nuclear High Level Emission Factor Schedule Name: Optional Object
    if ((_wo = workspaceObject.getTarget(FuelFactorsFields::NuclearHighLevelEmissionFactorScheduleName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        if (boost::optional<Schedule> _nuclearHighLevelEmissionFactorSchedule = _mo->optionalCast<Schedule>()) {
          modelObject.setNuclearHighLevelEmissionFactorSchedule(_nuclearHighLevelEmissionFactorSchedule.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Nuclear High Level Emission Factor Schedule Name'");
        }
      }
    }
    // Nuclear Low Level Emission Factor: Optional Double
    if (boost::optional<double> _nuclearLowLevelEmissionFactor = workspaceObject.getDouble(FuelFactorsFields::NuclearLowLevelEmissionFactor)) {
      modelObject.setNuclearLowLevelEmissionFactor(_nuclearLowLevelEmissionFactor.get());
    }

    // Nuclear Low Level Emission Factor Schedule Name: Optional Object
    if ((_wo = workspaceObject.getTarget(FuelFactorsFields::NuclearLowLevelEmissionFactorScheduleName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        if (boost::optional<Schedule> _nuclearLowLevelEmissionFactorSchedule = _mo->optionalCast<Schedule>()) {
          modelObject.setNuclearLowLevelEmissionFactorSchedule(_nuclearLowLevelEmissionFactorSchedule.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Nuclear Low Level Emission Factor Schedule Name'");
        }
      }
    }

    return modelObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
