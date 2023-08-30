/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/RefrigerationSystem.hpp"
#include "../../model/RefrigerationSubcoolerLiquidSuction.hpp"

#include <utilities/idd/Refrigeration_Subcooler_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateRefrigerationSubcoolerLiquidSuction(RefrigerationSubcoolerLiquidSuction& modelObject) {
    OptionalModelObject temp;
    OptionalString optS;
    boost::optional<std::string> s;
    boost::optional<double> d;

    // Name
    IdfObject subcoolerLiquidSuction = createRegisterAndNameIdfObject(openstudio::IddObjectType::Refrigeration_Subcooler, modelObject);

    // Subcooler Type
    subcoolerLiquidSuction.setString(Refrigeration_SubcoolerFields::SubcoolerType, "LiquidSuction");

    // Liquid Suction Design Subcooling Temperature Difference
    d = modelObject.liquidSuctionDesignSubcoolingTemperatureDifference();
    if (d) {
      subcoolerLiquidSuction.setDouble(Refrigeration_SubcoolerFields::LiquidSuctionDesignSubcoolingTemperatureDifference, d.get());
    }

    // Design Liquid Inlet Temperature
    d = modelObject.designLiquidInletTemperature();
    if (d) {
      subcoolerLiquidSuction.setDouble(Refrigeration_SubcoolerFields::DesignLiquidInletTemperature, d.get());
    }

    // Design Vapor Inlet Temperature
    d = modelObject.designVaporInletTemperature();
    if (d) {
      subcoolerLiquidSuction.setDouble(Refrigeration_SubcoolerFields::DesignVaporInletTemperature, d.get());
    }

    return subcoolerLiquidSuction;
  }
}  // namespace energyplus
}  // namespace openstudio
