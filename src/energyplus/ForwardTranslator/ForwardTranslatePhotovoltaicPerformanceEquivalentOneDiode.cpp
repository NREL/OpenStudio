/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/PhotovoltaicPerformanceEquivalentOneDiode.hpp"
#include "../../model/PhotovoltaicPerformanceEquivalentOneDiode_Impl.hpp"

#include <utilities/idd/PhotovoltaicPerformance_EquivalentOneDiode_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translatePhotovoltaicPerformanceEquivalentOneDiode(model::PhotovoltaicPerformanceEquivalentOneDiode& modelObject) {
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::PhotovoltaicPerformance_EquivalentOneDiode, modelObject);

    if (modelObject.name()) {
      idfObject.setString(PhotovoltaicPerformance_EquivalentOneDiodeFields::Name, modelObject.name().get());
    }

    idfObject.setString(PhotovoltaicPerformance_EquivalentOneDiodeFields::Celltype, modelObject.celltype());

    idfObject.setInt(PhotovoltaicPerformance_EquivalentOneDiodeFields::NumberofCellsinSeries, modelObject.numberOfCellsInSeries());

    idfObject.setDouble(PhotovoltaicPerformance_EquivalentOneDiodeFields::ActiveArea, modelObject.activeArea());

    idfObject.setDouble(PhotovoltaicPerformance_EquivalentOneDiodeFields::TransmittanceAbsorptanceProduct,
                        modelObject.transmittanceAbsorptanceProduct());

    idfObject.setDouble(PhotovoltaicPerformance_EquivalentOneDiodeFields::SemiconductorBandgap, modelObject.semiconductorBandgap());

    idfObject.setDouble(PhotovoltaicPerformance_EquivalentOneDiodeFields::ShuntResistance, modelObject.shuntResistance());

    idfObject.setDouble(PhotovoltaicPerformance_EquivalentOneDiodeFields::ShortCircuitCurrent, modelObject.shortCircuitCurrent());

    idfObject.setDouble(PhotovoltaicPerformance_EquivalentOneDiodeFields::OpenCircuitVoltage, modelObject.openCircuitVoltage());

    idfObject.setDouble(PhotovoltaicPerformance_EquivalentOneDiodeFields::ReferenceTemperature, modelObject.referenceTemperature());

    idfObject.setDouble(PhotovoltaicPerformance_EquivalentOneDiodeFields::ReferenceInsolation, modelObject.referenceInsolation());

    idfObject.setDouble(PhotovoltaicPerformance_EquivalentOneDiodeFields::ModuleCurrentatMaximumPower, modelObject.moduleCurrentatMaximumPower());

    idfObject.setDouble(PhotovoltaicPerformance_EquivalentOneDiodeFields::ModuleVoltageatMaximumPower, modelObject.moduleVoltageatMaximumPower());

    idfObject.setDouble(PhotovoltaicPerformance_EquivalentOneDiodeFields::TemperatureCoefficientofShortCircuitCurrent,
                        modelObject.temperatureCoefficientofShortCircuitCurrent());

    idfObject.setDouble(PhotovoltaicPerformance_EquivalentOneDiodeFields::TemperatureCoefficientofOpenCircuitVoltage,
                        modelObject.temperatureCoefficientofOpenCircuitVoltage());

    idfObject.setDouble(PhotovoltaicPerformance_EquivalentOneDiodeFields::NominalOperatingCellTemperatureTestAmbientTemperature,
                        modelObject.nominalOperatingCellTemperatureTestAmbientTemperature());

    idfObject.setDouble(PhotovoltaicPerformance_EquivalentOneDiodeFields::NominalOperatingCellTemperatureTestCellTemperature,
                        modelObject.nominalOperatingCellTemperatureTestCellTemperature());

    idfObject.setDouble(PhotovoltaicPerformance_EquivalentOneDiodeFields::NominalOperatingCellTemperatureTestInsolation,
                        modelObject.nominalOperatingCellTemperatureTestInsolation());

    idfObject.setDouble(PhotovoltaicPerformance_EquivalentOneDiodeFields::ModuleHeatLossCoefficient, modelObject.moduleHeatLossCoefficient());

    idfObject.setDouble(PhotovoltaicPerformance_EquivalentOneDiodeFields::TotalHeatCapacity, modelObject.totalHeatCapacity());

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
