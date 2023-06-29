/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/ModelObject.hpp"
#include "../../model/ExternalInterface.hpp"
#include "../../model/ExternalInterface_Impl.hpp"

#include <utilities/idd/ExternalInterface_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateExternalInterface(ExternalInterface& modelObject) {
    boost::optional<std::string> s;

    IdfObject idfObject(openstudio::IddObjectType::ExternalInterface);
    m_idfObjects.push_back(idfObject);
    //TODO check if this Name field exists in the unique object
    s = modelObject.name();
    if (s) {
      idfObject.setName(*s);
    }

    s = modelObject.nameofExternalInterface();
    if (s.is_initialized()) {
      idfObject.setString(ExternalInterfaceFields::NameofExternalInterface, s.get());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
