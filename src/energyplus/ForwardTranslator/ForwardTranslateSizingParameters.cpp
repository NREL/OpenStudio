/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/SizingParameters.hpp"
#include "../../model/SizingParameters_Impl.hpp"

#include <utilities/idd/Sizing_Parameters_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateSizingParameters(SizingParameters& modelObject) {
    IdfObject idfObject(openstudio::IddObjectType::Sizing_Parameters);

    m_idfObjects.push_back(idfObject);

    if (!modelObject.isHeatingSizingFactorDefaulted()) {
      idfObject.setDouble(Sizing_ParametersFields::HeatingSizingFactor, modelObject.heatingSizingFactor());
    }

    if (!modelObject.isCoolingSizingFactorDefaulted()) {
      idfObject.setDouble(Sizing_ParametersFields::CoolingSizingFactor, modelObject.coolingSizingFactor());
    }

    if (modelObject.timestepsinAveragingWindow()) {
      idfObject.setInt(Sizing_ParametersFields::TimestepsinAveragingWindow, modelObject.timestepsinAveragingWindow().get());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
