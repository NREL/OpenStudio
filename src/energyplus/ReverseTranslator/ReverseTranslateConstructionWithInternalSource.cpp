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

#include "../ReverseTranslator.hpp"

#include "../../model/ConstructionWithInternalSource.hpp"
#include "../../model/ConstructionWithInternalSource_Impl.hpp"
#include "../../model/ModelPartitionMaterial.hpp"
#include "../../model/ModelPartitionMaterial_Impl.hpp"

#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/Construction_InternalSource_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateConstructionWithInternalSource(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::Construction_InternalSource) {
      LOG(Error, "WorkspaceObject is not IddObjectType: Construction_InternalSource");
      return boost::none;
    }

    openstudio::model::ConstructionWithInternalSource construction(m_model);

    OptionalString s;
    OptionalDouble d;
    OptionalInt i;
    OptionalWorkspaceObject target;

    s = workspaceObject.name();
    if (s) {
      construction.setName(*s);
    }

    i = workspaceObject.getInt(Construction_InternalSourceFields::SourcePresentAfterLayerNumber);
    if (i) {
      construction.setSourcePresentAfterLayerNumber(*i);
    }

    i = workspaceObject.getInt(Construction_InternalSourceFields::TemperatureCalculationRequestedAfterLayerNumber);
    if (i) {
      construction.setTemperatureCalculationRequestedAfterLayerNumber(*i);
    }

    i = workspaceObject.getInt(Construction_InternalSourceFields::DimensionsfortheCTFCalculation);
    if (i) {
      construction.setDimensionsForTheCTFCalculation(*i);
    }

    d = workspaceObject.getDouble(Construction_InternalSourceFields::TubeSpacing);
    if (d) {
      construction.setTubeSpacing(*d);
    }

    d = workspaceObject.getDouble(Construction_InternalSourceFields::TwoDimensionalTemperatureCalculationPosition);
    if (d) {
      construction.setTwoDimensionalTemperatureCalculationPosition(*d);
    }

    // get extensible groups for layers
    for (const IdfExtensibleGroup& idfGroup : workspaceObject.extensibleGroups()) {
      WorkspaceExtensibleGroup workspaceGroup = idfGroup.cast<WorkspaceExtensibleGroup>();
      OptionalWorkspaceObject target = workspaceGroup.getTarget(Construction_InternalSourceExtensibleFields::Layer);
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);

      // set the layer
      construction.setLayer(modelObject->cast<ModelPartitionMaterial>());
    }

    return construction;
  }

}  // namespace energyplus

}  // namespace openstudio
