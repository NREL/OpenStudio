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

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/MeterCustomDecrement.hpp"
#include "../../model/MeterCustomDecrement_Impl.hpp"

#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include <utilities/idd/Meter_CustomDecrement_FieldEnums.hxx>

#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

// I don't think I'm using this
//using namespace std;

// I don't know if I need this
using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateMeterCustomDecrement(MeterCustomDecrement& modelObject) {

    boost::optional<std::string> s;
    boost::optional<double> value;

    IdfObject idfObject(IddObjectType::Meter_CustomDecrement);

    m_idfObjects.push_back(idfObject);

    s = modelObject.name();
    if (s) {
      idfObject.setName(*s);
    }

    // FuelType
    if ((s = modelObject.fuelType())) {
      idfObject.setString(Meter_CustomDecrementFields::ResourceType, s.get());
    }

    // Source Meter Name
    std::string sourceMeterName = modelObject.sourceMeterName();
    idfObject.setString(Meter_CustomDecrementFields::SourceMeterName, sourceMeterName);

    // Handle the (Key Name, Output Variable or Meter Name) pairs
    std::vector<std::pair<std::string, std::string>> keyVarGroups = modelObject.keyVarGroups();

    if (!keyVarGroups.empty()) {
      for (const auto& keyVarGroup : keyVarGroups) {
        IdfExtensibleGroup eg = idfObject.pushExtensibleGroup();

        eg.setString(Meter_CustomDecrementExtensibleFields::KeyName, keyVarGroup.first);
        eg.setString(Meter_CustomDecrementExtensibleFields::OutputVariableorMeterName, keyVarGroup.second);
      }
    } else {
      LOG(Error, modelObject.briefDescription() << ": At least one pair of (Key Name, Output Variable or Meter Name) required");
      return boost::none;
    }

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio