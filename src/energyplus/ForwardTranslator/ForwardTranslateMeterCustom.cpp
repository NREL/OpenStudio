/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/MeterCustom.hpp"
#include "../../model/MeterCustom_Impl.hpp"

#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include <utilities/idd/Meter_Custom_FieldEnums.hxx>

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

  boost::optional<IdfObject> ForwardTranslator::translateMeterCustom(MeterCustom& modelObject) {

    boost::optional<std::string> s;
    boost::optional<double> value;

    IdfObject idfObject(IddObjectType::Meter_Custom);

    m_idfObjects.push_back(idfObject);

    s = modelObject.name();
    if (s) {
      idfObject.setName(*s);
    }

    // FuelType
    if ((s = modelObject.fuelType())) {
      idfObject.setString(Meter_CustomFields::ResourceType, s.get());
    }

    // Handle the (Key Name, Output Variable or Meter Name) pairs
    std::vector<std::pair<std::string, std::string>> keyVarGroups = modelObject.keyVarGroups();

    if (!keyVarGroups.empty()) {
      for (const auto& keyVarGroup : keyVarGroups) {
        IdfExtensibleGroup eg = idfObject.pushExtensibleGroup();

        eg.setString(Meter_CustomExtensibleFields::KeyName, keyVarGroup.first);
        eg.setString(Meter_CustomExtensibleFields::OutputVariableorMeterName, keyVarGroup.second);
      }
    } else {
      LOG(Error, modelObject.briefDescription() << ": At least one pair of (Key Name, Output Variable or Meter Name) required");
      return boost::none;
    }

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
