/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/RefrigerationSystem.hpp"
#include "../../model/RefrigerationSubcoolerMechanical.hpp"

#include <utilities/idd/Refrigeration_Subcooler_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateRefrigerationSubcoolerMechanical(RefrigerationSubcoolerMechanical& modelObject) {
    OptionalModelObject temp;
    OptionalString optS;
    boost::optional<std::string> s;
    boost::optional<double> d;

    // Name
    IdfObject subcoolerMechanical = createRegisterAndNameIdfObject(openstudio::IddObjectType::Refrigeration_Subcooler, modelObject);

    subcoolerMechanical.setString(Refrigeration_SubcoolerFields::SubcoolerType, "Mechanical");

    // Capacity-Providing System
    boost::optional<RefrigerationSystem> capacityProvidingSystem = modelObject.capacityProvidingSystem();

    if (capacityProvidingSystem) {
      boost::optional<IdfObject> _capacityProvidingSystem = translateAndMapModelObject(capacityProvidingSystem.get());

      if (_capacityProvidingSystem && _capacityProvidingSystem->name()) {
        subcoolerMechanical.setString(Refrigeration_SubcoolerFields::CapacityProvidingSystem, _capacityProvidingSystem->name().get());
      }
    }

    // Outlet Control Temperature
    d = modelObject.outletControlTemperature();
    if (d) {
      subcoolerMechanical.setDouble(Refrigeration_SubcoolerFields::OutletControlTemperature, d.get());
    }

    return subcoolerMechanical;
  }
}  // namespace energyplus
}  // namespace openstudio
