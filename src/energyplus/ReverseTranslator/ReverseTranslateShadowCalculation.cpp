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

#include "../../model/ShadowCalculation.hpp"
#include "../../model/ShadowCalculation_Impl.hpp"

#include "../../model/ThermalZone.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"
#include <utilities/idd/ShadowCalculation_FieldEnums.hxx>
#include <utilities/idd/ZoneList_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"
#include "../../utilities/core/Optional.hpp"

using namespace openstudio::model;

namespace openstudio {
namespace energyplus {

  boost::optional<model::ModelObject> ReverseTranslator::translateShadowCalculation(const WorkspaceObject& workspaceObject) {
    OS_ASSERT(workspaceObject.iddObject().type() == IddObjectType::ShadowCalculation);

    ShadowCalculation sc = m_model.getUniqueModelObject<ShadowCalculation>();

    OptionalString _s;
    OptionalInt _i;

    // Shading Calculation Method
    if ((_s = workspaceObject.getString(ShadowCalculationFields::ShadingCalculationMethod)) && !_s->empty()) {
      sc.setShadingCalculationMethod(_s.get());
    }

    // Shading Calculation Update Frequency Method
    if ((_s = workspaceObject.getString(ShadowCalculationFields::ShadingCalculationUpdateFrequencyMethod)) && !_s->empty()) {
      sc.setShadingCalculationUpdateFrequencyMethod(_s.get());
    }

    // Shading Calculation Update Frequency
    if ((_i = workspaceObject.getInt(ShadowCalculationFields::ShadingCalculationUpdateFrequency))) {
      sc.setShadingCalculationUpdateFrequency(_i.get());
    }

    // Maximum Figures in Shadow Overlap Calculations
    if ((_i = workspaceObject.getInt(ShadowCalculationFields::MaximumFiguresinShadowOverlapCalculations))) {
      sc.setMaximumFiguresInShadowOverlapCalculations(_i.get());
    }

    // Polygon Clipping Algorithm
    if ((_s = workspaceObject.getString(ShadowCalculationFields::PolygonClippingAlgorithm)) && !_s->empty()) {
      sc.setPolygonClippingAlgorithm(_s.get());
    }

    // Sky Diffuse Modeling Algorithm
    if ((_s = workspaceObject.getString(ShadowCalculationFields::SkyDiffuseModelingAlgorithm)) && !_s->empty()) {
      sc.setSkyDiffuseModelingAlgorithm(_s.get());
    }

    // Pixel Counting Resolution
    if ((_i = workspaceObject.getInt(ShadowCalculationFields::PixelCountingResolution))) {
      sc.setPixelCountingResolution(_i.get());
    }

    // Output External Shading Calculation Results
    if ((_s = workspaceObject.getString(ShadowCalculationFields::OutputExternalShadingCalculationResults))) {
      if (istringEqual("Yes", _s.get())) {
        sc.setOutputExternalShadingCalculationResults(true);
      } else if (istringEqual("No", _s.get())) {
        sc.setOutputExternalShadingCalculationResults(false);
      }
    }

    // Disable Self-Shading Within Shading Zone Groups
    if ((_s = workspaceObject.getString(ShadowCalculationFields::DisableSelfShadingWithinShadingZoneGroups))) {
      if (istringEqual("Yes", _s.get())) {
        sc.setDisableSelfShadingWithinShadingZoneGroups(true);
      } else if (istringEqual("No", _s.get())) {
        sc.setDisableSelfShadingWithinShadingZoneGroups(false);
      }
    }

    // Disable Self-Shading From Shading Zone Groups to Other Zones
    if ((_s = workspaceObject.getString(ShadowCalculationFields::DisableSelfShadingFromShadingZoneGroupstoOtherZones))) {
      if (istringEqual("Yes", _s.get())) {
        sc.setDisableSelfShadingFromShadingZoneGroupstoOtherZones(true);
      } else if (istringEqual("No", _s.get())) {
        sc.setDisableSelfShadingFromShadingZoneGroupstoOtherZones(false);
      }
    }

    // Shading Zone Group (E+:  Shading Zone Group 1 ZoneList Name)
    // **Extensible** for ZoneLists
    for (const IdfExtensibleGroup& eg : workspaceObject.extensibleGroups()) {

      OptionalWorkspaceObject target = eg.cast<WorkspaceExtensibleGroup>().getTarget(ShadowCalculationExtensibleFields::ShadingZoneGroupZoneListName);

      if (target) {

        // We populate a vector of WorkspaceObject (which are 'Zone'),
        // so we handle the case when it's a Zone and the case when it's a ZoneList in the same way later down
        std::vector<WorkspaceObject> i_zones;
        if (target->iddObject().type() == IddObjectType::Zone) {
          i_zones.push_back(target.get());

        } else if (target->iddObject().type() == IddObjectType::ZoneList) {
          // ZoneLists are translated back to a space type, which isn't even something we do want to do here, except it'll still be default RT'ed.
          // Anyways, using the returned spacetype is useless since if a zone is referenced by multiple ZoneLists, then the sapcetype assigned for that
          // zone is the last ZoneList found...
          for (const IdfExtensibleGroup& zeg : target->extensibleGroups()) {
            OptionalWorkspaceObject target_zone = zeg.cast<WorkspaceExtensibleGroup>().getTarget(ZoneListExtensibleFields::ZoneName);
            if (target_zone) {
              i_zones.push_back(target_zone.get());
            }
          }

        } else {
          LOG(Error, "For ShadowCalculation, an extensible group entry is neither a Zone or ZoneList, skipping.");
          continue;
        }

        // Now we translate all of these WorkspaceObjects, and get corresponding (Model) ThermalZones in a vector
        std::vector<ThermalZone> thermalZones;
        for (const WorkspaceObject& i_zone : i_zones) {
          // Zone is translated, and a Space is returned instead
          OptionalModelObject _mo = translateAndMapWorkspaceObject(i_zone);
          if (_mo) {
            if (boost::optional<Space> _space = _mo->optionalCast<Space>()) {
              if (auto _zone = _space->thermalZone()) {
                thermalZones.push_back(_zone.get());
              }
            }
          }
        }

        // And now we add that as a Shading Zone Group
        if (!thermalZones.empty()) {
          sc.addShadingZoneGroup(thermalZones);
        }
      }
    }

    return sc.cast<ModelObject>();
  }

}  // namespace energyplus
}  // namespace openstudio
