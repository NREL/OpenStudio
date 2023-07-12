/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/SolarCollectorPerformanceFlatPlate.hpp"

#include <utilities/idd/SolarCollectorPerformance_FlatPlate_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateSolarCollectorPerformanceFlatPlate(model::SolarCollectorPerformanceFlatPlate& modelObject) {
    IdfObject idfObject(openstudio::IddObjectType::SolarCollectorPerformance_FlatPlate);

    m_idfObjects.push_back(idfObject);

    boost::optional<double> d;

    idfObject.setString(SolarCollectorPerformance_FlatPlateFields::Name, modelObject.name().get());

    idfObject.setDouble(SolarCollectorPerformance_FlatPlateFields::GrossArea, modelObject.grossArea());

    idfObject.setString(SolarCollectorPerformance_FlatPlateFields::TestFluid, modelObject.testFluid());

    idfObject.setDouble(SolarCollectorPerformance_FlatPlateFields::TestFlowRate, modelObject.testFlowRate());

    idfObject.setString(SolarCollectorPerformance_FlatPlateFields::TestCorrelationType, modelObject.testCorrelationType());

    idfObject.setDouble(SolarCollectorPerformance_FlatPlateFields::Coefficient1ofEfficiencyEquation, modelObject.coefficient1ofEfficiencyEquation());

    idfObject.setDouble(SolarCollectorPerformance_FlatPlateFields::Coefficient2ofEfficiencyEquation, modelObject.coefficient2ofEfficiencyEquation());

    d = modelObject.coefficient3ofEfficiencyEquation();
    if (d) {
      idfObject.setDouble(SolarCollectorPerformance_FlatPlateFields::Coefficient3ofEfficiencyEquation, *d);
    }

    d = modelObject.coefficient2ofIncidentAngleModifier();
    if (d) {
      idfObject.setDouble(SolarCollectorPerformance_FlatPlateFields::Coefficient2ofIncidentAngleModifier, *d);
    }

    d = modelObject.coefficient3ofIncidentAngleModifier();
    if (d) {
      idfObject.setDouble(SolarCollectorPerformance_FlatPlateFields::Coefficient3ofIncidentAngleModifier, *d);
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
