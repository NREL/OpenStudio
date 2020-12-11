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

#include "../../model/MeterCustomDecrement.hpp"
#include "../../model/MeterCustomDecrement_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/Meter_CustomDecrement_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateMeterCustomDecrement(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::Meter_CustomDecrement) {
      LOG(Error, "WorkspaceObject is not IddObjectType: Meter:CustomDecrement");
      return boost::none;
    }

    boost::optional<MeterCustomDecrement> optMeterCustomDecrement;
    boost::optional<std::string> s;

    // Source Meter Name: get it and pass it to the constructor
    s = workspaceObject.getString(Meter_CustomDecrementFields::SourceMeterName);
    if (s) {
      // Create an OS:Meter:CustomDecrement object
      optMeterCustomDecrement = MeterCustomDecrement(m_model, s.get());
    } else {
      LOG(Error, workspaceObject.briefDescription() << " does not have a Source Meter Name which is required. It will not be translated!");
    }

    MeterCustomDecrement meterCustomDecrement = optMeterCustomDecrement.get();

    // Name
    s = workspaceObject.getString(Meter_CustomDecrementFields::Name);
    if (s) {
      meterCustomDecrement.setName(s.get());
    }

    // Fuel Type
    s = workspaceObject.getString(Meter_CustomDecrementFields::ResourceType);
    if (s) {
      // TODO: JM to DLM: should I also check it's part of the validFuelTypes?
      meterCustomDecrement.setFuelType(s.get());
    }

    // Get all the (key, var) extensible groups from IDF
    std::vector<IdfExtensibleGroup> keyVarGroups = workspaceObject.extensibleGroups();

    // Clean out the (key, var) groups (just in case: the constructor doesn't default any, for now at least!)
    meterCustomDecrement.removeAllKeyVarGroups();

    // Push them all to the object
    for (const auto& keyVarGroup : keyVarGroups) {
      meterCustomDecrement.pushExtensibleGroup(keyVarGroup.fields());
    }

    return meterCustomDecrement;
  }

}  // namespace energyplus

}  // namespace openstudio
