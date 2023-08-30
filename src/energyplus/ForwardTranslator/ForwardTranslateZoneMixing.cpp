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
#include "../../model/ZoneMixing.hpp"
#include "../../model/ZoneMixing_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"

#include <utilities/idd/ZoneMixing_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateZoneMixing(ZoneMixing& modelObject) {

    ModelObject zoneOrSpace = modelObject.zoneOrSpace();
    boost::optional<ModelObject> sourceZoneOrSpace = modelObject.sourceZoneOrSpace();

    auto getParentObjectName = [this](const ModelObject& mo) {
      if (!m_forwardTranslatorOptions.excludeSpaceTranslation()) {
        return mo.nameString();
      }

      if (auto space_ = mo.optionalCast<Space>()) {
        if (auto thermalZone_ = space_->thermalZone()) {
          return thermalZone_->nameString();
        } else {
          OS_ASSERT(false);  // This shouldn't happen, since we removed all orphaned spaces earlier in the FT
        }
      }

      return mo.nameString();
    };

    if (!sourceZoneOrSpace) {
      if (m_forwardTranslatorOptions.excludeSpaceTranslation() && modelObject.space()) {
        LOG(Warn, modelObject.briefDescription()
                    << " doesn't have a Source Zone or Space, it will not be translated. As you were using Space-Level ZoneMixing, and you are not "
                       "translating to Spaces, it's possible it was pointing to two spaces inside the same zone");
      } else {
        LOG(Warn, modelObject.briefDescription() << " doesn't have a Source Zone or Space, it will not be translated.");
      }
      return boost::none;
    }

    if (zoneOrSpace == sourceZoneOrSpace.get()) {
      // This isn't going to happen, because zm.setSourceSpace(newSpace) in ThermalZone::combineSpaces will be rejected
      // Let's play it safe though
      LOG(Warn, modelObject.briefDescription() << " has the same Receiving and Source Zone or Space, it will not be translated.");
      if (!m_forwardTranslatorOptions.excludeSpaceTranslation()) {
        // We don't allow this at model time, the only reason we expect this to happen is when m_forwardTranslatorOptions.excludeSpaceTranslation()
        // is true, we call combineSpaces, and if the user has a ZoneMixing pointing to two spaces from the same ThermalZone,
        // you end up with matching Receiving and Source Spaces
        OS_ASSERT(false);
      }
      return boost::none;
    }

    // Makes sure the modelObject gets put in the map, and that the new idfObject gets put in
    // the final file. Also set's the idfObject's name.
    IdfObject idfObject = createRegisterAndNameIdfObject(IddObjectType::ZoneMixing, modelObject);

    boost::optional<double> value;

    // ZoneorSpaceName
    translateAndMapModelObject(zoneOrSpace);
    idfObject.setString(ZoneMixingFields::ZoneorSpaceName, getParentObjectName(zoneOrSpace));

    // ScheduleName
    Schedule schedule = modelObject.schedule();
    translateAndMapModelObject(schedule);
    idfObject.setString(ZoneMixingFields::ScheduleName, schedule.nameString());

    // DesignFlowRateCalculationMethod
    idfObject.setString(ZoneMixingFields::DesignFlowRateCalculationMethod, modelObject.designFlowRateCalculationMethod());

    // DesignFlowRate
    value = modelObject.designFlowRate();
    if (value) {
      idfObject.setDouble(ZoneMixingFields::DesignFlowRate, *value);
    }

    // FlowRateperFloorArea
    value = modelObject.flowRateperFloorArea();
    if (value) {
      idfObject.setDouble(ZoneMixingFields::FlowRateperFloorArea, *value);
    }

    // FlowRateperPerson
    value = modelObject.flowRateperPerson();
    if (value) {
      idfObject.setDouble(ZoneMixingFields::FlowRateperPerson, *value);
    }

    // AirChangesperHour
    value = modelObject.airChangesperHour();
    if (value) {
      idfObject.setDouble(ZoneMixingFields::AirChangesperHour, *value);
    }

    // SourceZoneorSpaceName
    // DLM: do not translate source zone now, it will be translated at the right time
    idfObject.setString(ZoneMixingFields::SourceZoneorSpaceName, getParentObjectName(sourceZoneOrSpace.get()));

    // DeltaTemperature
    value = modelObject.deltaTemperature();
    if (value) {
      idfObject.setDouble(ZoneMixingFields::DeltaTemperature, *value);
    }

    // DeltaTemperatureScheduleName
    boost::optional<Schedule> optSchedule = modelObject.deltaTemperatureSchedule();
    if (optSchedule) {
      translateAndMapModelObject(*optSchedule);
      idfObject.setString(ZoneMixingFields::DeltaTemperatureScheduleName, optSchedule->nameString());
    }

    // MinimumReceivingTemperatureScheduleName
    optSchedule = modelObject.minimumReceivingTemperatureSchedule();
    if (optSchedule) {
      translateAndMapModelObject(*optSchedule);
      idfObject.setString(ZoneMixingFields::MinimumReceivingTemperatureScheduleName, optSchedule->nameString());
    }

    // MaximumReceivingTemperatureScheduleName
    optSchedule = modelObject.maximumReceivingTemperatureSchedule();
    if (optSchedule) {
      translateAndMapModelObject(*optSchedule);
      idfObject.setString(ZoneMixingFields::MaximumReceivingTemperatureScheduleName, optSchedule->nameString());
    }

    // MinimumSourceTemperatureScheduleName
    optSchedule = modelObject.minimumSourceTemperatureSchedule();
    if (optSchedule) {
      translateAndMapModelObject(*optSchedule);
      idfObject.setString(ZoneMixingFields::MinimumSourceTemperatureScheduleName, optSchedule->nameString());
    }

    // MaximumSourceTemperatureScheduleName
    optSchedule = modelObject.maximumSourceTemperatureSchedule();
    if (optSchedule) {
      translateAndMapModelObject(*optSchedule);
      idfObject.setString(ZoneMixingFields::MaximumSourceTemperatureScheduleName, optSchedule->nameString());
    }

    // MinimumOutdoorTemperatureScheduleName
    optSchedule = modelObject.minimumOutdoorTemperatureSchedule();
    if (optSchedule) {
      translateAndMapModelObject(*optSchedule);
      idfObject.setString(ZoneMixingFields::MinimumOutdoorTemperatureScheduleName, optSchedule->nameString());
    }

    // MaximumOutdoorTemperatureScheduleName
    optSchedule = modelObject.maximumOutdoorTemperatureSchedule();
    if (optSchedule) {
      translateAndMapModelObject(*optSchedule);
      idfObject.setString(ZoneMixingFields::MaximumOutdoorTemperatureScheduleName, optSchedule->nameString());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
