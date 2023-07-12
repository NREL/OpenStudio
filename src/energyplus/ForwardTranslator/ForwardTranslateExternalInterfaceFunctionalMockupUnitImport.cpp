/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/ModelObject.hpp"
#include "../../model/ExternalInterfaceFunctionalMockupUnitImport.hpp"
#include "../../model/ExternalInterfaceFunctionalMockupUnitImport_Impl.hpp"

#include <utilities/idd/ExternalInterface_FunctionalMockupUnitImport_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translateExternalInterfaceFunctionalMockupUnitImport(ExternalInterfaceFunctionalMockupUnitImport& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> d;

    IdfObject idfObject(openstudio::IddObjectType::ExternalInterface_FunctionalMockupUnitImport);
    m_idfObjects.push_back(idfObject);
    //Name
    s = modelObject.name();
    if (s) {
      idfObject.setName(*s);
    }

    s = modelObject.fMUFileName();
    if (s.is_initialized()) {
      idfObject.setString(ExternalInterface_FunctionalMockupUnitImportFields::FMUFileName, s.get());
    }

    d = modelObject.fMUTimeout();
    if (d.is_initialized()) {
      idfObject.setDouble(ExternalInterface_FunctionalMockupUnitImportFields::FMUTimeout, d.get());
    }

    d = modelObject.fMULoggingOn();
    if (d.is_initialized()) {
      idfObject.setDouble(ExternalInterface_FunctionalMockupUnitImportFields::FMULoggingOn, d.get());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
