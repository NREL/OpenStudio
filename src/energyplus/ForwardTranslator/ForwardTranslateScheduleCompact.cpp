/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/ScheduleTypeLimits.hpp"
#include "../../model/ScheduleTypeLimits_Impl.hpp"
#include "../../model/ScheduleCompact.hpp"
#include "../../model/ScheduleCompact_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/Schedule_Compact_FieldEnums.hxx>

#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateScheduleCompact(ScheduleCompact& modelObject) {
    IdfObject scheduleCompact = createAndRegisterIdfObject(openstudio::IddObjectType::Schedule_Compact, modelObject);

    OptionalString os;
    if ((os = modelObject.name())) {
      scheduleCompact.setName(*os);
    }

    boost::optional<ScheduleTypeLimits> scheduleTypeLimits = modelObject.scheduleTypeLimits();
    if (scheduleTypeLimits) {
      boost::optional<IdfObject> idfScheduleTypeLimits = translateAndMapModelObject(*scheduleTypeLimits);
      if (idfScheduleTypeLimits) {
        scheduleCompact.setString(Schedule_CompactFields::ScheduleTypeLimitsName, idfScheduleTypeLimits->name().get());
      }
    }

    for (IdfExtensibleGroup& eg : modelObject.extensibleGroups()) {
      for (unsigned i = 0; i < eg.numFields(); ++i) {
        if (auto value = eg.getString(i)) {
          if (istringEqual(value.get(), "Interpolate:Yes")) {
            eg.setString(i, "Interpolate:Average");
          }
        }
      }
      scheduleCompact.pushExtensibleGroup(eg.fields());
    }

    return boost::optional<IdfObject>(scheduleCompact);
  }

}  // namespace energyplus

}  // namespace openstudio
