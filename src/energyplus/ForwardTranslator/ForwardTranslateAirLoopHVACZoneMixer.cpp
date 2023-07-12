/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/AirLoopHVACZoneMixer.hpp"
#include <utilities/idd/AirLoopHVAC_ZoneMixer_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateAirLoopHVACZoneMixer(AirLoopHVACZoneMixer& modelObject) {
    std::string s;
    OptionalString optS;
    OptionalModelObject temp;

    IdfObject idfObject(openstudio::IddObjectType::AirLoopHVAC_ZoneMixer);

    m_idfObjects.push_back(idfObject);

    ///////////////////////////////////////////////////////////////////////////
    // Field: Name ////////////////////////////////////////////////////////////
    s = modelObject.name().get();
    idfObject.setName(s);

    ////////////////////////////////////////////////////////////////////////////
    // Field: Outlet Node Name /////////////////////////////////////////////////
    temp = modelObject.outletModelObject();
    if (temp) {
      optS = temp->name();
      if (optS) {
        idfObject.setString(openstudio::AirLoopHVAC_ZoneMixerFields::OutletNodeName, *optS);
      }
    }
    ///////////////////////////////////////////////////////////////////////////
    // Field: Inlet 1-N Node Name ///////////////////////////////////////////////
    std::vector<ModelObject> inletModelObjects = modelObject.inletModelObjects();
    int i = 2;
    for (const auto& inletModelObject : inletModelObjects) {
      optS = inletModelObject.name();
      if (optS) {
        idfObject.setString(i, *optS);
      }
      i++;
    }

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
