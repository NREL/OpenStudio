/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
