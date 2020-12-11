/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
