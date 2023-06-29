/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"
#include "../../model/SizingParameters.hpp"
#include "../../model/SizingParameters_Impl.hpp"
#include <utilities/idd/Sizing_Parameters_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateSizingParameters(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::Sizing_Parameters) {
      LOG(Error, "WorkspaceObject is not IddObjectType: Sizing_Parameters");
      return boost::none;
    }

    auto mo = m_model.getUniqueModelObject<SizingParameters>();

    boost::optional<double> value = workspaceObject.getDouble(Sizing_ParametersFields::HeatingSizingFactor);
    if (value) {
      mo.setHeatingSizingFactor(value.get());
    }

    value = workspaceObject.getDouble(Sizing_ParametersFields::CoolingSizingFactor);
    if (value) {
      mo.setCoolingSizingFactor(value.get());
    }

    boost::optional<int> i = workspaceObject.getInt(Sizing_ParametersFields::TimestepsinAveragingWindow);
    if (i) {
      mo.setTimestepsinAveragingWindow(i.get());
    }

    return mo;
  }

}  // namespace energyplus

}  // namespace openstudio
