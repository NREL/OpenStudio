/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"
#include "../../model/RefrigerationCompressor.hpp"
#include "../../model/RefrigerationCompressor_Impl.hpp"
#include "../../model/CurveBicubic.hpp"
#include "../../model/CurveBicubic_Impl.hpp"
#include <utilities/idd/Refrigeration_Compressor_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateRefrigerationCompressor(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::Refrigeration_Compressor) {
      LOG(Error, "WorkspaceObject is not IddObjectType: Refrigeration_Compressor");
      return boost::none;
    }

    boost::optional<RefrigerationCompressor> refrigerationCompressor = RefrigerationCompressor(m_model);
    boost::optional<WorkspaceObject> wo;

    if (refrigerationCompressor) {
      boost::optional<double> value;
      boost::optional<std::string> s = workspaceObject.getString(Refrigeration_CompressorFields::Name);

      // Name
      if (s) {
        refrigerationCompressor->setName(s.get());
      }

      // RefrigerationCompressorPowerCurveName
      if ((wo = workspaceObject.getTarget(Refrigeration_CompressorFields::RefrigerationCompressorPowerCurveName))) {
        if (boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(wo.get())) {
          if (boost::optional<CurveBicubic> curve = mo->optionalCast<CurveBicubic>()) {
            refrigerationCompressor->setRefrigerationCompressorPowerCurve(curve.get());
          }
        }
      }
      // RefrigerationCompressorCapacityCurveName
      if ((wo = workspaceObject.getTarget(Refrigeration_CompressorFields::RefrigerationCompressorCapacityCurveName))) {
        if (boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(wo.get())) {
          if (boost::optional<CurveBicubic> curve = mo->optionalCast<CurveBicubic>()) {
            refrigerationCompressor->setRefrigerationCompressorCapacityCurve(curve.get());
          }
        }
      }
      // RatedSuperheat
      value = workspaceObject.getDouble(Refrigeration_CompressorFields::RatedSuperheat);
      if (value) {
        refrigerationCompressor->setRatedSuperheat(value.get());
      }
      // RatedReturnGasTemperature
      value = workspaceObject.getDouble(Refrigeration_CompressorFields::RatedReturnGasTemperature);
      if (value) {
        refrigerationCompressor->setRatedReturnGasTemperature(value.get());
      }
      // RatedLiquidTemperature
      value = workspaceObject.getDouble(Refrigeration_CompressorFields::RatedLiquidTemperature);
      if (value) {
        refrigerationCompressor->setRatedLiquidTemperature(value.get());
      }
      // RatedSubcooling
      value = workspaceObject.getDouble(Refrigeration_CompressorFields::RatedSubcooling);
      if (value) {
        refrigerationCompressor->setRatedSubcooling(value.get());
      }
      // EndUseSubcategory
      s = workspaceObject.getString(Refrigeration_CompressorFields::EndUseSubcategory);
      if (s) {
        refrigerationCompressor->setEndUseSubcategory(s.get());
      }
      // ModeofOperation
      // TranscriticalCompressorPowerCurveName
      if ((wo = workspaceObject.getTarget(Refrigeration_CompressorFields::TranscriticalCompressorPowerCurveName))) {
        if (boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(wo.get())) {
          if (boost::optional<CurveBicubic> curve = mo->optionalCast<CurveBicubic>()) {
            refrigerationCompressor->setTranscriticalCompressorPowerCurve(curve.get());
          }
        }
      }
      // TranscriticalCompressorCapacityCurveName
      if ((wo = workspaceObject.getTarget(Refrigeration_CompressorFields::TranscriticalCompressorCapacityCurveName))) {
        if (boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(wo.get())) {
          if (boost::optional<CurveBicubic> curve = mo->optionalCast<CurveBicubic>()) {
            refrigerationCompressor->setTranscriticalCompressorCapacityCurve(curve.get());
          }
        }
      }

      return refrigerationCompressor.get();
    } else {
      LOG(Error, "Unknown error translating " << workspaceObject.briefDescription());

      return boost::none;
    }
  }

}  // namespace energyplus

}  // namespace openstudio
