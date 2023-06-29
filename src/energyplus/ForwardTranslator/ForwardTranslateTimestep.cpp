/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Timestep.hpp"
#include <utilities/idd/Timestep_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateTimestep(Timestep& modelObject) {
    IdfObject idfObject(openstudio::IddObjectType::Timestep);

    idfObject.setInt(TimestepFields::NumberofTimestepsperHour, modelObject.numberOfTimestepsPerHour());

    m_idfObjects.push_back(idfObject);

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
