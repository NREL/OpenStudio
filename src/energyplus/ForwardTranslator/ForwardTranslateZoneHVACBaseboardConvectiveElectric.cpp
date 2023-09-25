/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ModelObject.hpp"
#include "../../model/ModelObject_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/ZoneHVACBaseboardConvectiveElectric.hpp"
#include "../../model/ZoneHVACBaseboardConvectiveElectric_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/StraightComponent.hpp"
#include "../../model/StraightComponent_Impl.hpp"
#include <utilities/idd/ZoneHVAC_Baseboard_Convective_Electric_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateZoneHVACBaseboardConvectiveElectric(ZoneHVACBaseboardConvectiveElectric& modelObject) {
    // Makes sure the modelObject gets put in the map, and that the new idfObject gets put in
    // the final file. Also set's the idfObject's name.
    IdfObject idfObject = createRegisterAndNameIdfObject(IddObjectType::ZoneHVAC_Baseboard_Convective_Electric, modelObject);

    boost::optional<std::string> s;
    boost::optional<double> value;
    boost::optional<ModelObject> temp;

    // AvailabilityScheduleName
    Schedule availabilitySchedule = modelObject.availabilitySchedule();
    translateAndMapModelObject(availabilitySchedule);
    idfObject.setString(ZoneHVAC_Baseboard_Convective_ElectricFields::AvailabilityScheduleName, availabilitySchedule.name().get());

    // NominalCapacity

    if (modelObject.isNominalCapacityAutosized()) {
      idfObject.setString(ZoneHVAC_Baseboard_Convective_ElectricFields::HeatingDesignCapacity, "Autosize");
    } else if ((value = modelObject.nominalCapacity())) {
      idfObject.setDouble(ZoneHVAC_Baseboard_Convective_ElectricFields::HeatingDesignCapacity, value.get());
    }

    // Efficiency

    if ((value = modelObject.efficiency())) {
      idfObject.setDouble(ZoneHVAC_Baseboard_Convective_ElectricFields::Efficiency, value.get());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
