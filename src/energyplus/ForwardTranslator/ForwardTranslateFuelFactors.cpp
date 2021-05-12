/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2021, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

// TODO: Check the following class names against object getters and setters.
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include <utilities/idd/FuelFactors_FieldEnums.hxx>
// #include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateFuelFactors(model::FuelFactors& modelObject) {
    boost::optional<IdfObject> result;
    boost::optional<WorkspaceObject> _wo;
    boost::optional<ModelObject> _mo;

    // Instantiate an IdfObject of the class to store the values,
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::FuelFactors, modelObject);
    // If it doesn't have a name, or if you aren't sure you are going to want to return it
    // IdfObject idfObject( openstudio::IddObjectType::FuelFactors );
    // m_idfObjects.push_back(idfObject);

    // TODO: Note JM 2018-10-17
    // You are responsible for implementing any additional logic based on choice fields, etc.
    // The ForwardTranslator generator script is meant to facilitate your work, not get you 100% of the way

    // Existing Fuel Resource Name: boost::optional<std::string>
    if (boost::optional<std::string> _existingFuelResourceName = modelObject.existingFuelResourceName()) {
      idfObject.setString(FuelFactorsFields::ExistingFuelResourceName, _existingFuelResourceName.get());
    }

    // Units of Measure: boost::optional<std::string>
    if (boost::optional<std::string> _unitsofMeasure = modelObject.unitsofMeasure()) {
      idfObject.setString(FuelFactorsFields::UnitsofMeasure, _unitsofMeasure.get());
    }

    // Energy per Unit Factor: boost::optional<double>
    if (boost::optional<double> _energyperUnitFactor = modelObject.energyperUnitFactor()) {
      idfObject.setDouble(FuelFactorsFields::EnergyperUnitFactor, _energyperUnitFactor.get());
    }

    // Source Energy Factor: boost::optional<double>
    if (boost::optional<double> _sourceEnergyFactor = modelObject.sourceEnergyFactor()) {
      idfObject.setDouble(FuelFactorsFields::SourceEnergyFactor, _sourceEnergyFactor.get());
    }

    // Source Energy Schedule Name: Optional Object
    if (boost::optional<Schedule> _sourceEnergySchedule = modelObject.sourceEnergySchedule()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_sourceEnergySchedule.get())) {
        idfObject.setString(FuelFactorsFields::SourceEnergyScheduleName, _owo->nameString());
      }
    }

    // CO2 Emission Factor: boost::optional<double>
    if (boost::optional<double> _cO2EmissionFactor = modelObject.cO2EmissionFactor()) {
      idfObject.setDouble(FuelFactorsFields::CO2EmissionFactor, _cO2EmissionFactor.get());
    }

    // CO2 Emission Factor Schedule Name: Optional Object
    if (boost::optional<Schedule> _cO2EmissionFactorSchedule = modelObject.cO2EmissionFactorSchedule()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_cO2EmissionFactorSchedule.get())) {
        idfObject.setString(FuelFactorsFields::CO2EmissionFactorScheduleName, _owo->nameString());
      }
    }

    // CO Emission Factor: boost::optional<double>
    if (boost::optional<double> _cOEmissionFactor = modelObject.cOEmissionFactor()) {
      idfObject.setDouble(FuelFactorsFields::COEmissionFactor, _cOEmissionFactor.get());
    }

    // CO Emission Factor Schedule Name: Optional Object
    if (boost::optional<Schedule> _cOEmissionFactorSchedule = modelObject.cOEmissionFactorSchedule()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_cOEmissionFactorSchedule.get())) {
        idfObject.setString(FuelFactorsFields::COEmissionFactorScheduleName, _owo->nameString());
      }
    }

    // CH4 Emission Factor: boost::optional<double>
    if (boost::optional<double> _cH4EmissionFactor = modelObject.cH4EmissionFactor()) {
      idfObject.setDouble(FuelFactorsFields::CH4EmissionFactor, _cH4EmissionFactor.get());
    }

    // CH4 Emission Factor Schedule Name: Optional Object
    if (boost::optional<Schedule> _cH4EmissionFactorSchedule = modelObject.cH4EmissionFactorSchedule()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_cH4EmissionFactorSchedule.get())) {
        idfObject.setString(FuelFactorsFields::CH4EmissionFactorScheduleName, _owo->nameString());
      }
    }

    // NOx Emission Factor: boost::optional<double>
    if (boost::optional<double> _nOxEmissionFactor = modelObject.nOxEmissionFactor()) {
      idfObject.setDouble(FuelFactorsFields::NOxEmissionFactor, _nOxEmissionFactor.get());
    }

    // NOx Emission Factor Schedule Name: Optional Object
    if (boost::optional<Schedule> _nOxEmissionFactorSchedule = modelObject.nOxEmissionFactorSchedule()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_nOxEmissionFactorSchedule.get())) {
        idfObject.setString(FuelFactorsFields::NOxEmissionFactorScheduleName, _owo->nameString());
      }
    }

    // N2O Emission Factor: boost::optional<double>
    if (boost::optional<double> _n2OEmissionFactor = modelObject.n2OEmissionFactor()) {
      idfObject.setDouble(FuelFactorsFields::N2OEmissionFactor, _n2OEmissionFactor.get());
    }

    // N2O Emission Factor Schedule Name: Optional Object
    if (boost::optional<Schedule> _n2OEmissionFactorSchedule = modelObject.n2OEmissionFactorSchedule()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_n2OEmissionFactorSchedule.get())) {
        idfObject.setString(FuelFactorsFields::N2OEmissionFactorScheduleName, _owo->nameString());
      }
    }

    // SO2 Emission Factor: boost::optional<double>
    if (boost::optional<double> _sO2EmissionFactor = modelObject.sO2EmissionFactor()) {
      idfObject.setDouble(FuelFactorsFields::SO2EmissionFactor, _sO2EmissionFactor.get());
    }

    // SO2 Emission Factor Schedule Name: Optional Object
    if (boost::optional<Schedule> _sO2EmissionFactorSchedule = modelObject.sO2EmissionFactorSchedule()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_sO2EmissionFactorSchedule.get())) {
        idfObject.setString(FuelFactorsFields::SO2EmissionFactorScheduleName, _owo->nameString());
      }
    }

    // PM Emission Factor: boost::optional<double>
    if (boost::optional<double> _pMEmissionFactor = modelObject.pMEmissionFactor()) {
      idfObject.setDouble(FuelFactorsFields::PMEmissionFactor, _pMEmissionFactor.get());
    }

    // PM Emission Factor Schedule Name: Optional Object
    if (boost::optional<Schedule> _pMEmissionFactorSchedule = modelObject.pMEmissionFactorSchedule()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_pMEmissionFactorSchedule.get())) {
        idfObject.setString(FuelFactorsFields::PMEmissionFactorScheduleName, _owo->nameString());
      }
    }

    // PM10 Emission Factor: boost::optional<double>
    if (boost::optional<double> _pM10EmissionFactor = modelObject.pM10EmissionFactor()) {
      idfObject.setDouble(FuelFactorsFields::PM10EmissionFactor, _pM10EmissionFactor.get());
    }

    // PM10 Emission Factor Schedule Name: Optional Object
    if (boost::optional<Schedule> _pM10EmissionFactorSchedule = modelObject.pM10EmissionFactorSchedule()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_pM10EmissionFactorSchedule.get())) {
        idfObject.setString(FuelFactorsFields::PM10EmissionFactorScheduleName, _owo->nameString());
      }
    }

    // PM2.5 Emission Factor: boost::optional<double>
    if (boost::optional<double> _pM25EmissionFactor = modelObject.pM25EmissionFactor()) {
      idfObject.setDouble(FuelFactorsFields::PM25EmissionFactor, _pM25EmissionFactor.get());
    }

    // PM2.5 Emission Factor Schedule Name: Optional Object
    if (boost::optional<Schedule> _pM25EmissionFactorSchedule = modelObject.pM25EmissionFactorSchedule()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_pM25EmissionFactorSchedule.get())) {
        idfObject.setString(FuelFactorsFields::PM25EmissionFactorScheduleName, _owo->nameString());
      }
    }

    // NH3 Emission Factor: boost::optional<double>
    if (boost::optional<double> _nH3EmissionFactor = modelObject.nH3EmissionFactor()) {
      idfObject.setDouble(FuelFactorsFields::NH3EmissionFactor, _nH3EmissionFactor.get());
    }

    // NH3 Emission Factor Schedule Name: Optional Object
    if (boost::optional<Schedule> _nH3EmissionFactorSchedule = modelObject.nH3EmissionFactorSchedule()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_nH3EmissionFactorSchedule.get())) {
        idfObject.setString(FuelFactorsFields::NH3EmissionFactorScheduleName, _owo->nameString());
      }
    }

    // NMVOC Emission Factor: boost::optional<double>
    if (boost::optional<double> _nMVOCEmissionFactor = modelObject.nMVOCEmissionFactor()) {
      idfObject.setDouble(FuelFactorsFields::NMVOCEmissionFactor, _nMVOCEmissionFactor.get());
    }

    // NMVOC Emission Factor Schedule Name: Optional Object
    if (boost::optional<Schedule> _nMVOCEmissionFactorSchedule = modelObject.nMVOCEmissionFactorSchedule()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_nMVOCEmissionFactorSchedule.get())) {
        idfObject.setString(FuelFactorsFields::NMVOCEmissionFactorScheduleName, _owo->nameString());
      }
    }

    // Hg Emission Factor: boost::optional<double>
    if (boost::optional<double> _hgEmissionFactor = modelObject.hgEmissionFactor()) {
      idfObject.setDouble(FuelFactorsFields::HgEmissionFactor, _hgEmissionFactor.get());
    }

    // Hg Emission Factor Schedule Name: Optional Object
    if (boost::optional<Schedule> _hgEmissionFactorSchedule = modelObject.hgEmissionFactorSchedule()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_hgEmissionFactorSchedule.get())) {
        idfObject.setString(FuelFactorsFields::HgEmissionFactorScheduleName, _owo->nameString());
      }
    }

    // Pb Emission Factor: boost::optional<double>
    if (boost::optional<double> _pbEmissionFactor = modelObject.pbEmissionFactor()) {
      idfObject.setDouble(FuelFactorsFields::PbEmissionFactor, _pbEmissionFactor.get());
    }

    // Pb Emission Factor Schedule Name: Optional Object
    if (boost::optional<Schedule> _pbEmissionFactorSchedule = modelObject.pbEmissionFactorSchedule()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_pbEmissionFactorSchedule.get())) {
        idfObject.setString(FuelFactorsFields::PbEmissionFactorScheduleName, _owo->nameString());
      }
    }

    // Water Emission Factor: boost::optional<double>
    if (boost::optional<double> _waterEmissionFactor = modelObject.waterEmissionFactor()) {
      idfObject.setDouble(FuelFactorsFields::WaterEmissionFactor, _waterEmissionFactor.get());
    }

    // Water Emission Factor Schedule Name: Optional Object
    if (boost::optional<Schedule> _waterEmissionFactorSchedule = modelObject.waterEmissionFactorSchedule()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_waterEmissionFactorSchedule.get())) {
        idfObject.setString(FuelFactorsFields::WaterEmissionFactorScheduleName, _owo->nameString());
      }
    }

    // Nuclear High Level Emission Factor: boost::optional<double>
    if (boost::optional<double> _nuclearHighLevelEmissionFactor = modelObject.nuclearHighLevelEmissionFactor()) {
      idfObject.setDouble(FuelFactorsFields::NuclearHighLevelEmissionFactor, _nuclearHighLevelEmissionFactor.get());
    }

    // Nuclear High Level Emission Factor Schedule Name: Optional Object
    if (boost::optional<Schedule> _nuclearHighLevelEmissionFactorSchedule = modelObject.nuclearHighLevelEmissionFactorSchedule()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_nuclearHighLevelEmissionFactorSchedule.get())) {
        idfObject.setString(FuelFactorsFields::NuclearHighLevelEmissionFactorScheduleName, _owo->nameString());
      }
    }

    // Nuclear Low Level Emission Factor: boost::optional<double>
    if (boost::optional<double> _nuclearLowLevelEmissionFactor = modelObject.nuclearLowLevelEmissionFactor()) {
      idfObject.setDouble(FuelFactorsFields::NuclearLowLevelEmissionFactor, _nuclearLowLevelEmissionFactor.get());
    }

    // Nuclear Low Level Emission Factor Schedule Name: Optional Object
    if (boost::optional<Schedule> _nuclearLowLevelEmissionFactorSchedule = modelObject.nuclearLowLevelEmissionFactorSchedule()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_nuclearLowLevelEmissionFactorSchedule.get())) {
        idfObject.setString(FuelFactorsFields::NuclearLowLevelEmissionFactorScheduleName, _owo->nameString());
      }
    }

    result = IdfObject;
    return result;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio