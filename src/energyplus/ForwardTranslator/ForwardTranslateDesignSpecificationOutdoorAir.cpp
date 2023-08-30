/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/DesignSpecificationOutdoorAir.hpp"
#include "../../model/DesignSpecificationOutdoorAir_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/DesignSpecification_OutdoorAir_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateDesignSpecificationOutdoorAir(DesignSpecificationOutdoorAir& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> value;

    IdfObject idfObject(IddObjectType::DesignSpecification_OutdoorAir);

    idfObject.setString(DesignSpecification_OutdoorAirFields::Name, modelObject.name().get());

    std::string outdoorAirMethod = modelObject.outdoorAirMethod();
    idfObject.setString(DesignSpecification_OutdoorAirFields::OutdoorAirMethod, outdoorAirMethod);

    double flowPerPerson = modelObject.outdoorAirFlowperPerson();
    double flowPerArea = modelObject.outdoorAirFlowperFloorArea();
    double flowPerZone = modelObject.outdoorAirFlowRate();
    double ach = modelObject.outdoorAirFlowAirChangesperHour();

    if (istringEqual(outdoorAirMethod, "Sum") || istringEqual(outdoorAirMethod, "Maximum")) {

      idfObject.setDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowperPerson, flowPerPerson);
      idfObject.setDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowperZoneFloorArea, flowPerArea);
      idfObject.setDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowperZone, flowPerZone);
      idfObject.setDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowAirChangesperHour, ach);

    } else {
      LOG(Error, "Unknown OutdoorAirMethod '" << outdoorAirMethod << "' specified for OS:DesignSpecification:OutdoorAir named '"
                                              << modelObject.name().get() << "'");
      return boost::none;
    }

    boost::optional<Schedule> schedule = modelObject.outdoorAirFlowRateFractionSchedule();
    if (schedule) {
      idfObject.setString(DesignSpecification_OutdoorAirFields::OutdoorAirScheduleName, schedule->name().get());
    }

    m_idfObjects.push_back(idfObject);

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
