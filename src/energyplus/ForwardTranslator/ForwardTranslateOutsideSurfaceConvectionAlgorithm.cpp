/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/OutsideSurfaceConvectionAlgorithm.hpp"
#include <utilities/idd/SurfaceConvectionAlgorithm_Outside_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateOutsideSurfaceConvectionAlgorithm(OutsideSurfaceConvectionAlgorithm& modelObject) {
    IdfObject idfObject(openstudio::IddObjectType::SurfaceConvectionAlgorithm_Outside);

    idfObject.setString(SurfaceConvectionAlgorithm_OutsideFields::Algorithm, modelObject.algorithm());

    m_idfObjects.push_back(idfObject);

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
