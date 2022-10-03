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

#include "../../model/FuelFactors.hpp"

#include "../../model/Schedule.hpp"

#include <utilities/idd/FuelFactors_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateFuelFactors(model::FuelFactors& modelObject) {
    boost::optional<WorkspaceObject> _wo;
    boost::optional<ModelObject> _mo;

    // If it doesn't have a name
    IdfObject idfObject(openstudio::IddObjectType::FuelFactors);
    m_idfObjects.push_back(idfObject);

    // Existing Fuel Resource Name
    idfObject.setString(FuelFactorsFields::ExistingFuelResourceName, modelObject.existingFuelResourceName());

    // Source Energy Factor
    idfObject.setDouble(FuelFactorsFields::SourceEnergyFactor, modelObject.sourceEnergyFactor());

    // Source Energy Schedule Name: Optional Object
    if (boost::optional<Schedule> _sourceEnergySchedule = modelObject.sourceEnergySchedule()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_sourceEnergySchedule.get())) {
        idfObject.setString(FuelFactorsFields::SourceEnergyScheduleName, _owo->nameString());
      }
    }

    // CO2 Emission Factor
    idfObject.setDouble(FuelFactorsFields::CO2EmissionFactor, modelObject.cO2EmissionFactor());

    // CO2 Emission Factor Schedule Name: Optional Object
    if (boost::optional<Schedule> _cO2EmissionFactorSchedule = modelObject.cO2EmissionFactorSchedule()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_cO2EmissionFactorSchedule.get())) {
        idfObject.setString(FuelFactorsFields::CO2EmissionFactorScheduleName, _owo->nameString());
      }
    }

    // CO Emission Factor
    idfObject.setDouble(FuelFactorsFields::COEmissionFactor, modelObject.cOEmissionFactor());

    // CO Emission Factor Schedule Name: Optional Object
    if (boost::optional<Schedule> _cOEmissionFactorSchedule = modelObject.cOEmissionFactorSchedule()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_cOEmissionFactorSchedule.get())) {
        idfObject.setString(FuelFactorsFields::COEmissionFactorScheduleName, _owo->nameString());
      }
    }

    // CH4 Emission Factor
    idfObject.setDouble(FuelFactorsFields::CH4EmissionFactor, modelObject.cH4EmissionFactor());

    // CH4 Emission Factor Schedule Name: Optional Object
    if (boost::optional<Schedule> _cH4EmissionFactorSchedule = modelObject.cH4EmissionFactorSchedule()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_cH4EmissionFactorSchedule.get())) {
        idfObject.setString(FuelFactorsFields::CH4EmissionFactorScheduleName, _owo->nameString());
      }
    }

    // NOx Emission Factor
    idfObject.setDouble(FuelFactorsFields::NOxEmissionFactor, modelObject.nOxEmissionFactor());

    // NOx Emission Factor Schedule Name: Optional Object
    if (boost::optional<Schedule> _nOxEmissionFactorSchedule = modelObject.nOxEmissionFactorSchedule()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_nOxEmissionFactorSchedule.get())) {
        idfObject.setString(FuelFactorsFields::NOxEmissionFactorScheduleName, _owo->nameString());
      }
    }

    // N2O Emission Factor
    idfObject.setDouble(FuelFactorsFields::N2OEmissionFactor, modelObject.n2OEmissionFactor());

    // N2O Emission Factor Schedule Name: Optional Object
    if (boost::optional<Schedule> _n2OEmissionFactorSchedule = modelObject.n2OEmissionFactorSchedule()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_n2OEmissionFactorSchedule.get())) {
        idfObject.setString(FuelFactorsFields::N2OEmissionFactorScheduleName, _owo->nameString());
      }
    }

    // SO2 Emission Factor
    idfObject.setDouble(FuelFactorsFields::SO2EmissionFactor, modelObject.sO2EmissionFactor());

    // SO2 Emission Factor Schedule Name: Optional Object
    if (boost::optional<Schedule> _sO2EmissionFactorSchedule = modelObject.sO2EmissionFactorSchedule()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_sO2EmissionFactorSchedule.get())) {
        idfObject.setString(FuelFactorsFields::SO2EmissionFactorScheduleName, _owo->nameString());
      }
    }

    // PM Emission Factor
    idfObject.setDouble(FuelFactorsFields::PMEmissionFactor, modelObject.pMEmissionFactor());

    // PM Emission Factor Schedule Name: Optional Object
    if (boost::optional<Schedule> _pMEmissionFactorSchedule = modelObject.pMEmissionFactorSchedule()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_pMEmissionFactorSchedule.get())) {
        idfObject.setString(FuelFactorsFields::PMEmissionFactorScheduleName, _owo->nameString());
      }
    }

    // PM10 Emission Factor
    idfObject.setDouble(FuelFactorsFields::PM10EmissionFactor, modelObject.pM10EmissionFactor());

    // PM10 Emission Factor Schedule Name: Optional Object
    if (boost::optional<Schedule> _pM10EmissionFactorSchedule = modelObject.pM10EmissionFactorSchedule()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_pM10EmissionFactorSchedule.get())) {
        idfObject.setString(FuelFactorsFields::PM10EmissionFactorScheduleName, _owo->nameString());
      }
    }

    // PM2.5 Emission Factor
    idfObject.setDouble(FuelFactorsFields::PM25EmissionFactor, modelObject.pM25EmissionFactor());

    // PM2.5 Emission Factor Schedule Name: Optional Object
    if (boost::optional<Schedule> _pM25EmissionFactorSchedule = modelObject.pM25EmissionFactorSchedule()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_pM25EmissionFactorSchedule.get())) {
        idfObject.setString(FuelFactorsFields::PM25EmissionFactorScheduleName, _owo->nameString());
      }
    }

    // NH3 Emission Factor
    idfObject.setDouble(FuelFactorsFields::NH3EmissionFactor, modelObject.nH3EmissionFactor());

    // NH3 Emission Factor Schedule Name: Optional Object
    if (boost::optional<Schedule> _nH3EmissionFactorSchedule = modelObject.nH3EmissionFactorSchedule()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_nH3EmissionFactorSchedule.get())) {
        idfObject.setString(FuelFactorsFields::NH3EmissionFactorScheduleName, _owo->nameString());
      }
    }

    // NMVOC Emission Factor
    idfObject.setDouble(FuelFactorsFields::NMVOCEmissionFactor, modelObject.nMVOCEmissionFactor());

    // NMVOC Emission Factor Schedule Name: Optional Object
    if (boost::optional<Schedule> _nMVOCEmissionFactorSchedule = modelObject.nMVOCEmissionFactorSchedule()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_nMVOCEmissionFactorSchedule.get())) {
        idfObject.setString(FuelFactorsFields::NMVOCEmissionFactorScheduleName, _owo->nameString());
      }
    }

    // Hg Emission Factor
    idfObject.setDouble(FuelFactorsFields::HgEmissionFactor, modelObject.hgEmissionFactor());

    // Hg Emission Factor Schedule Name: Optional Object
    if (boost::optional<Schedule> _hgEmissionFactorSchedule = modelObject.hgEmissionFactorSchedule()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_hgEmissionFactorSchedule.get())) {
        idfObject.setString(FuelFactorsFields::HgEmissionFactorScheduleName, _owo->nameString());
      }
    }

    // Pb Emission Factor
    idfObject.setDouble(FuelFactorsFields::PbEmissionFactor, modelObject.pbEmissionFactor());

    // Pb Emission Factor Schedule Name: Optional Object
    if (boost::optional<Schedule> _pbEmissionFactorSchedule = modelObject.pbEmissionFactorSchedule()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_pbEmissionFactorSchedule.get())) {
        idfObject.setString(FuelFactorsFields::PbEmissionFactorScheduleName, _owo->nameString());
      }
    }

    // Water Emission Factor
    idfObject.setDouble(FuelFactorsFields::WaterEmissionFactor, modelObject.waterEmissionFactor());

    // Water Emission Factor Schedule Name: Optional Object
    if (boost::optional<Schedule> _waterEmissionFactorSchedule = modelObject.waterEmissionFactorSchedule()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_waterEmissionFactorSchedule.get())) {
        idfObject.setString(FuelFactorsFields::WaterEmissionFactorScheduleName, _owo->nameString());
      }
    }

    // Nuclear High Level Emission Factor
    idfObject.setDouble(FuelFactorsFields::NuclearHighLevelEmissionFactor, modelObject.nuclearHighLevelEmissionFactor());

    // Nuclear High Level Emission Factor Schedule Name: Optional Object
    if (boost::optional<Schedule> _nuclearHighLevelEmissionFactorSchedule = modelObject.nuclearHighLevelEmissionFactorSchedule()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_nuclearHighLevelEmissionFactorSchedule.get())) {
        idfObject.setString(FuelFactorsFields::NuclearHighLevelEmissionFactorScheduleName, _owo->nameString());
      }
    }

    // Nuclear Low Level Emission Factor
    idfObject.setDouble(FuelFactorsFields::NuclearLowLevelEmissionFactor, modelObject.nuclearLowLevelEmissionFactor());

    // Nuclear Low Level Emission Factor Schedule Name: Optional Object
    if (boost::optional<Schedule> _nuclearLowLevelEmissionFactorSchedule = modelObject.nuclearLowLevelEmissionFactorSchedule()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_nuclearLowLevelEmissionFactorSchedule.get())) {
        idfObject.setString(FuelFactorsFields::NuclearLowLevelEmissionFactorScheduleName, _owo->nameString());
      }
    }

    return idfObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
