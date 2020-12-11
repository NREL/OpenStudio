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
#include "../../model/ShadowCalculation.hpp"

#include "../../model/ThermalZone.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include <utilities/idd/ShadowCalculation_FieldEnums.hxx>
#include <utilities/idd/ZoneList_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateShadowCalculation(ShadowCalculation& modelObject) {
    IdfObject idfObject(openstudio::IddObjectType::ShadowCalculation);

    // Shading Calculation Method
    idfObject.setString(ShadowCalculationFields::ShadingCalculationMethod, modelObject.shadingCalculationMethod());

    // Shading Calculation Update Frequency Method
    idfObject.setString(ShadowCalculationFields::ShadingCalculationUpdateFrequencyMethod, modelObject.shadingCalculationUpdateFrequencyMethod());

    // Shading Calculation Update Frequency
    idfObject.setInt(ShadowCalculationFields::ShadingCalculationUpdateFrequency, modelObject.shadingCalculationUpdateFrequency());

    // Maximum Figures in Shadow Overlap Calculations
    idfObject.setInt(ShadowCalculationFields::MaximumFiguresinShadowOverlapCalculations, modelObject.maximumFiguresInShadowOverlapCalculations());

    // Polygon Clipping Algorithm
    idfObject.setString(ShadowCalculationFields::PolygonClippingAlgorithm, modelObject.polygonClippingAlgorithm());

    // Sky Diffuse Modeling Algorithm
    idfObject.setString(ShadowCalculationFields::SkyDiffuseModelingAlgorithm, modelObject.skyDiffuseModelingAlgorithm());

    // Pixel Counting Resolution
    idfObject.setInt(ShadowCalculationFields::PixelCountingResolution, modelObject.pixelCountingResolution());

    // Output External Shading Calculation Results
    if (modelObject.outputExternalShadingCalculationResults()) {
      idfObject.setString(ShadowCalculationFields::OutputExternalShadingCalculationResults, "Yes");
    } else {
      idfObject.setString(ShadowCalculationFields::OutputExternalShadingCalculationResults, "No");
    }

    // Disable Self-Shading Within Shading Zone Groups
    if (modelObject.disableSelfShadingWithinShadingZoneGroups()) {
      idfObject.setString(ShadowCalculationFields::DisableSelfShadingWithinShadingZoneGroups, "Yes");
    } else {
      idfObject.setString(ShadowCalculationFields::DisableSelfShadingWithinShadingZoneGroups, "No");
    }

    // Disable Self-Shading From Shading Zone Groups to Other Zones
    if (modelObject.disableSelfShadingFromShadingZoneGroupstoOtherZones()) {
      idfObject.setString(ShadowCalculationFields::DisableSelfShadingFromShadingZoneGroupstoOtherZones, "Yes");
    } else {
      idfObject.setString(ShadowCalculationFields::DisableSelfShadingFromShadingZoneGroupstoOtherZones, "No");
    }

    // Shading Zone Group (E+:  Shading Zone Group 1 ZoneList Name)
    // **Extensible** for ZoneLists
    for (unsigned i = 0; i < modelObject.numberofShadingZoneGroups(); ++i) {

      std::vector<ThermalZone> thermalZones = modelObject.getShadingZoneGroup(i);
      if (!thermalZones.empty()) {

        IdfObject zoneList(IddObjectType::ZoneList);
        std::string zoneListName = "ShadowCalculation Shading Zone Group" + std::to_string(i + 1);
        zoneList.setName(zoneListName);
        // Register it
        m_idfObjects.push_back(zoneList);

        for (const ThermalZone& tz : thermalZones) {
          auto eg = zoneList.pushExtensibleGroup();
          eg.setString(ZoneListExtensibleFields::ZoneName, tz.name().get());
        }

        auto eg = idfObject.pushExtensibleGroup();
        eg.setString(ShadowCalculationExtensibleFields::ShadingZoneGroupZoneListName, zoneListName);
      }
    }

    m_idfObjects.push_back(idfObject);

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
