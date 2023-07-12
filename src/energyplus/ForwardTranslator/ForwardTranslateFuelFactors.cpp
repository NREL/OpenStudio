/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
