/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../../model/Construction.hpp"
#include "../../model/Construction_Impl.hpp"
#include "../../model/Material.hpp"
#include "../../model/Material_Impl.hpp"
#include "../../model/OpaqueMaterial.hpp"
#include "../../model/OpaqueMaterial_Impl.hpp"
#include "../../model/FenestrationMaterial.hpp"
#include "../../model/FenestrationMaterial_Impl.hpp"
#include "../../model/ModelPartitionMaterial.hpp"
#include "../../model/ModelPartitionMaterial_Impl.hpp"

#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/Construction_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateConstruction( const WorkspaceObject & workspaceObject )
{
  if( workspaceObject.iddObject().type() != IddObjectType::Construction ){
    LOG(Error, "WorkspaceObject is not IddObjectType: Construction");
    return boost::none;
  }

  Construction construction(m_model);
  OS_ASSERT(construction.numLayers() == 0u);
  OptionalString optS = workspaceObject.name();
  if (optS) {
    construction.setName(*optS);
  }

  unsigned n = workspaceObject.numExtensibleGroups();
  // now we get the workspace objects and try to find them and place them in the model object
  // Loop over all the fields except the first, which is the name
  OptionalWorkspaceObject owo;
  OptionalModelObject temp;
  for( unsigned i = 0; i < n; ++i) {
    owo = workspaceObject.getExtensibleGroup(i).cast<WorkspaceExtensibleGroup>().getTarget(ConstructionExtensibleFields::Layer);
    if( owo ) {
      temp = translateAndMapWorkspaceObject( *owo );
    }
    if( ! (owo && temp) )
    {
      LOG(Error, "Finding Construction Layer in workspace failed.");
      construction.remove();
      return boost::none;
    }
    // Assuming names of materials are unique
    OptionalMaterial mat = temp->optionalCast<Material>();

    bool inserted(false);
    if (mat) {
      OptionalOpaqueMaterial opaqueMaterial = mat->optionalCast<OpaqueMaterial>();
      if(opaqueMaterial) {
        inserted = construction.insertLayer(i, *opaqueMaterial);
      }
      else {
        OptionalFenestrationMaterial fenestrationMaterial = mat->optionalCast<FenestrationMaterial>();
        if (fenestrationMaterial) {
          inserted = construction.insertLayer(i, *fenestrationMaterial );
        }
        else {
          OptionalModelPartitionMaterial modelPartitionMaterial = mat->optionalCast<ModelPartitionMaterial>();
          if (modelPartitionMaterial) {
            if (construction.numLayers() == 0u) {
              inserted = construction.setLayer(*modelPartitionMaterial);
            }
          }
        }
      }
    }

    if( !inserted ) {
      LOG(Error, "Insertion of Construction Layer failed.");
      construction.remove();
      return boost::none;
    }
  }

  return construction;
}

} // energyplus

} // openstudio

