/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
