/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"
#include "../../model/BoilerSteam.hpp"
#include "../../model/BoilerSteam_Impl.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/Boiler_Steam_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateBoilerSteam(BoilerSteam& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> value;

    IdfObject idfObject(IddObjectType::Boiler_Steam);

    m_idfObjects.push_back(idfObject);

    // Name

    s = modelObject.name();
    if (s) {
      idfObject.setName(*s);
    }

    // FuelType

    if ((s = modelObject.fuelType())) {
      idfObject.setString(Boiler_SteamFields::FuelType, s.get());
    }

    // MaximumOperatingPressure

    if ((value = modelObject.maximumOperatingPressure())) {
      idfObject.setDouble(Boiler_SteamFields::MaximumOperatingPressure, value.get());
    }

    // TheoreticalEfficiency

    if ((value = modelObject.theoreticalEfficiency())) {
      idfObject.setDouble(Boiler_SteamFields::TheoreticalEfficiency, value.get());
    }

    // DesignOutletSteamTemperature

    if ((value = modelObject.designOutletSteamTemperature())) {
      idfObject.setDouble(Boiler_SteamFields::DesignOutletSteamTemperature, value.get());
    }

    // NominalCapacity

    if (modelObject.isNominalCapacityAutosized()) {
      idfObject.setString(Boiler_SteamFields::NominalCapacity, "Autosize");
    } else if ((value = modelObject.nominalCapacity())) {
      idfObject.setDouble(Boiler_SteamFields::NominalCapacity, value.get());
    }

    // MinimumPartLoadRatio

    if ((value = modelObject.minimumPartLoadRatio())) {
      idfObject.setDouble(Boiler_SteamFields::MinimumPartLoadRatio, value.get());
    }

    // MaximumPartLoadRatio

    if ((value = modelObject.maximumPartLoadRatio())) {
      idfObject.setDouble(Boiler_SteamFields::MaximumPartLoadRatio, value.get());
    }

    // OptimumPartLoadRatio

    if ((value = modelObject.optimumPartLoadRatio())) {
      idfObject.setDouble(Boiler_SteamFields::OptimumPartLoadRatio, value.get());
    }

    // Coefficient1ofFuelUseFunctionofPartLoadRatioCurve

    if ((value = modelObject.coefficient1ofFuelUseFunctionofPartLoadRatioCurve())) {
      idfObject.setDouble(Boiler_SteamFields::Coefficient1ofFuelUseFunctionofPartLoadRatioCurve, value.get());
    }

    // Coefficient2ofFuelUseFunctionofPartLoadRatioCurve

    if ((value = modelObject.coefficient2ofFuelUseFunctionofPartLoadRatioCurve())) {
      idfObject.setDouble(Boiler_SteamFields::Coefficient2ofFuelUseFunctionofPartLoadRatioCurve, value.get());
    }

    // Coefficient3ofFuelUseFunctionofPartLoadRatioCurve

    if ((value = modelObject.coefficient3ofFuelUseFunctionofPartLoadRatioCurve())) {
      idfObject.setDouble(Boiler_SteamFields::Coefficient3ofFuelUseFunctionofPartLoadRatioCurve, value.get());
    }

    // WaterInletNodeName

    if (boost::optional<ModelObject> mo = modelObject.inletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(Boiler_SteamFields::WaterInletNodeName, node->name().get());
      }
    }

    // SteamOutletNodeName

    if (boost::optional<ModelObject> mo = modelObject.outletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(Boiler_SteamFields::SteamOutletNodeName, node->name().get());
      }
    }

    // SizingFactor

    if ((value = modelObject.sizingFactor())) {
      idfObject.setDouble(Boiler_SteamFields::SizingFactor, value.get());
    }

    // End Use Subcategory
    if ((s = modelObject.endUseSubcategory())) {
      idfObject.setString(Boiler_SteamFields::EndUseSubcategory, s.get());
    }

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
