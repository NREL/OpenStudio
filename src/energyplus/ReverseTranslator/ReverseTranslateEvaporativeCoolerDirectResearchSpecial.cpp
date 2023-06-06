/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"
#include "../../model/EvaporativeCoolerDirectResearchSpecial.hpp"
#include "../../model/EvaporativeCoolerDirectResearchSpecial_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include <utilities/idd/EvaporativeCooler_Direct_ResearchSpecial_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateEvaporativeCoolerDirectResearchSpecial(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::EvaporativeCooler_Direct_ResearchSpecial) {
      LOG(Error, "WorkspaceObject is not IddObjectType: EvaporativeCooler_Direct_ResearchSpecial");
      return boost::none;
    }

    boost::optional<Schedule> schedule;
    boost::optional<EvaporativeCoolerDirectResearchSpecial> mo;

    boost::optional<WorkspaceObject> wo = workspaceObject.getTarget(EvaporativeCooler_Direct_ResearchSpecialFields::AvailabilityScheduleName);
    if (wo) {
      boost::optional<ModelObject> mo2 = translateAndMapWorkspaceObject(wo.get());
      if (mo2) {
        if (!(schedule = mo2->optionalCast<Schedule>())) {
          LOG(Error, workspaceObject.briefDescription() << " does not have an associated availability schedule");

          return boost::none;
        }
      }
    }

    if (schedule) {
      mo = EvaporativeCoolerDirectResearchSpecial(m_model, schedule.get());
    }

    if (mo) {
      boost::optional<std::string> s = workspaceObject.getString(EvaporativeCooler_Direct_ResearchSpecialFields::Name);
      if (s) {
        mo->setName(s.get());
      }

      boost::optional<double> value = workspaceObject.getDouble(EvaporativeCooler_Direct_ResearchSpecialFields::CoolerDesignEffectiveness);
      if (s) {
        mo->setCoolerEffectiveness(value.get());
      }

      value = workspaceObject.getDouble(EvaporativeCooler_Direct_ResearchSpecialFields::RecirculatingWaterPumpDesignPower);
      if (value) {
        mo->setRecirculatingWaterPumpPowerConsumption(value.get());
      }

      value = workspaceObject.getDouble(EvaporativeCooler_Direct_ResearchSpecialFields::DriftLossFraction);
      if (value) {
        mo->setDriftLossFraction(value.get());
      }

      value = workspaceObject.getDouble(EvaporativeCooler_Direct_ResearchSpecialFields::BlowdownConcentrationRatio);
      if (value) {
        mo->setBlowdownConcentrationRatio(value.get());
      }

      return mo.get();
    } else {
      LOG(Error, "Unknown error translating " << workspaceObject.briefDescription());

      return boost::none;
    }
  }

}  // namespace energyplus

}  // namespace openstudio
