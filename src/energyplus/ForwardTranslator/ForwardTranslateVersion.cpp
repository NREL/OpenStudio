/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Version.hpp"
#include <utilities/idd/Version_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include "../../utilities/idd/IddObject.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

using namespace std;

namespace openstudio {
namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateVersion(Version& modelObject) {
    IdfObject version(openstudio::IddObjectType::Version);
    OS_ASSERT(istringEqual(version.iddObject().name(), "Version"));

    m_idfObjects.push_back(version);

    OptionalString s = modelObject.name();
    if (s) {
      version.setName(*s);
    }

    version.setString(openstudio::VersionFields::VersionIdentifier, ENERGYPLUS_VERSION);

    return boost::optional<IdfObject>(version);
  }

}  // namespace energyplus
}  // namespace openstudio
