/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/AvailabilityManagerAssignmentList.hpp"
#include "../../model/AvailabilityManagerAssignmentList_Impl.hpp"

#include "../../model/AvailabilityManager.hpp"
#include "../../model/AvailabilityManagerHybridVentilation.hpp"
#include "../../model/AvailabilityManagerHybridVentilation_Impl.hpp"

#include <utilities/idd/AvailabilityManagerAssignmentList_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateAvailabilityManagerAssignmentList(AvailabilityManagerAssignmentList& modelObject) {

    std::vector<AvailabilityManager> avmVector = modelObject.availabilityManagers();

    if (avmVector.empty()) {
      // do not write out this object
      return boost::none;
    }

    // If there is nothing but AvailabilityManagerHybridVentilation then stop
    // This one is an odd duck, it should stand by itself
    if (subsetCastVector<model::AvailabilityManagerHybridVentilation>(avmVector).size() == avmVector.size()) {
      return boost::none;
    }

    // Create an AVM list
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::AvailabilityManagerAssignmentList, modelObject);

    for (auto& avm : avmVector) {
      // Skip the AVM:HybridVentilation
      if (!(avm.optionalCast<model::AvailabilityManagerHybridVentilation>())) {
        if (boost::optional<IdfObject> _avm = translateAndMapModelObject(avm)) {
          IdfExtensibleGroup eg = idfObject.pushExtensibleGroup();
          eg.setString(AvailabilityManagerAssignmentListExtensibleFields::AvailabilityManagerObjectType, _avm->iddObject().name());
          eg.setString(AvailabilityManagerAssignmentListExtensibleFields::AvailabilityManagerName, _avm->name().get());
        }
      }
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
