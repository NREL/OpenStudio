/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/RefrigerationCompressor.hpp"
#include "../../model/CurveBicubic.hpp"

#include <utilities/idd/Refrigeration_Compressor_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateRefrigerationCompressor(RefrigerationCompressor& modelObject) {
    OptionalModelObject temp;
    OptionalString optS;
    boost::optional<std::string> s;
    boost::optional<double> d;

    // Name
    IdfObject object = createRegisterAndNameIdfObject(openstudio::IddObjectType::Refrigeration_Compressor, modelObject);

    //Refrigeration Compressor Power Curve Name
    boost::optional<CurveBicubic> powerCurve = modelObject.refrigerationCompressorPowerCurve();

    if (powerCurve) {
      boost::optional<IdfObject> _powerCurve = translateAndMapModelObject(powerCurve.get());

      if (_powerCurve && _powerCurve->name()) {
        object.setString(Refrigeration_CompressorFields::RefrigerationCompressorPowerCurveName, _powerCurve->name().get());
      }
    }

    //Refrigeration Compressor Capacity Curve name
    boost::optional<CurveBicubic> capacityCurve = modelObject.refrigerationCompressorCapacityCurve();

    if (capacityCurve) {
      boost::optional<IdfObject> _capacityCurve = translateAndMapModelObject(capacityCurve.get());

      if (_capacityCurve && _capacityCurve->name()) {
        object.setString(Refrigeration_CompressorFields::RefrigerationCompressorCapacityCurveName, _capacityCurve->name().get());
      }
    }

    //Rated Superheat
    d = modelObject.ratedSuperheat();
    if (d) {
      object.setDouble(Refrigeration_CompressorFields::RatedSuperheat, d.get());
    }

    //Rated Return Gas Temperature
    d = modelObject.ratedReturnGasTemperature();
    if (d) {
      object.setDouble(Refrigeration_CompressorFields::RatedReturnGasTemperature, d.get());
    }

    //Rated Liquid Temperature
    d = modelObject.ratedLiquidTemperature();
    if (d) {
      object.setDouble(Refrigeration_CompressorFields::RatedLiquidTemperature, d.get());
    }

    //Rated Subcooling
    d = modelObject.ratedSubcooling();
    if (d) {
      object.setDouble(Refrigeration_CompressorFields::RatedSubcooling, d.get());
    }

    //End-Use Subcategory
    s = modelObject.endUseSubcategory();
    if (s) {
      object.setString(Refrigeration_CompressorFields::EndUseSubcategory, s.get());
    }

    //Mode of Operation
    s = modelObject.modeofOperation();
    if (s) {
      object.setString(Refrigeration_CompressorFields::ModeofOperation, s.get());
    }

    //Transcritical Compressor Power Curve Name
    boost::optional<CurveBicubic> transcriticalPowerCurve = modelObject.transcriticalCompressorPowerCurve();

    if (transcriticalPowerCurve) {
      boost::optional<IdfObject> _transcriticalPowerCurve = translateAndMapModelObject(transcriticalPowerCurve.get());

      if (_transcriticalPowerCurve && _transcriticalPowerCurve->name()) {
        object.setString(Refrigeration_CompressorFields::TranscriticalCompressorPowerCurveName, _transcriticalPowerCurve->name().get());
      }
    }

    //Transcritical Compressor Capacity Curve Name
    boost::optional<CurveBicubic> transcriticalCapacityCurve = modelObject.transcriticalCompressorCapacityCurve();

    if (transcriticalCapacityCurve) {
      boost::optional<IdfObject> _transcriticalCapacityCurve = translateAndMapModelObject(transcriticalCapacityCurve.get());

      if (_transcriticalCapacityCurve && _transcriticalCapacityCurve->name()) {
        object.setString(Refrigeration_CompressorFields::TranscriticalCompressorCapacityCurveName, _transcriticalCapacityCurve->name().get());
      }
    }

    return object;
  }
}  // namespace energyplus
}  // namespace openstudio
