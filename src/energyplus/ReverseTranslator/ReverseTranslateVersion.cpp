/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/Version.hpp"
#include "../../model/Version_Impl.hpp"

#include <utilities/idd/Version_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateVersion(const WorkspaceObject& /*workspaceObject*/) {
    return boost::none;
    //openstudio::model::Version version( m_model );

    //OptionalString optS = workspaceObject.getString(OS_VersionFields::VersionIdentifier);
    //if(optS)
    //{
    //  version.setVersionIdentifier(*optS);
    //}

    //return version;
  }

}  // namespace energyplus

}  // namespace openstudio
