/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/SolarCollectorPerformancePhotovoltaicThermalBIPVT.hpp"
#include "../../model/Model.hpp"
#include "../../model/SurfacePropertyOtherSideConditionsModel.hpp"
#include "../../model/Schedule.hpp"

#include <utilities/idd/SolarCollectorPerformance_PhotovoltaicThermal_BIPVT_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateSolarCollectorPerformancePhotovoltaicThermalBIPVT(
    model::SolarCollectorPerformancePhotovoltaicThermalBIPVT& modelObject) {

    // Instantiate an IdfObject of the class to store the values
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::SolarCollectorPerformance_PhotovoltaicThermal_BIPVT, modelObject);

    // Boundary Conditions Model Name: Required Object
    {
      auto boundaryConditionsModel = modelObject.boundaryConditionsModel();
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(boundaryConditionsModel)) {
        idfObject.setString(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::BoundaryConditionsModelName, wo_->nameString());
      }
    }

    // Availability Schedule Name: Optional Object (but required in model)
    {
      auto availabilitySchedule = modelObject.availabilitySchedule();
      if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(availabilitySchedule)) {
        idfObject.setString(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::AvailabilityScheduleName, wo_->nameString());
      }
    }

    // Effective Plenum Gap Thickness Behind PV Modules: Required Double
    idfObject.setDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::EffectivePlenumGapThicknessBehindPVModules,
                        modelObject.effectivePlenumGapThicknessBehindPVModules());

    // PV Cell Normal Transmittance-Absorptance Product: Optional Double (but required in model)
    idfObject.setDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::PVCellNormalTransmittanceAbsorptanceProduct,
                        modelObject.pVCellNormalTransmittanceAbsorptanceProduct());

    // Backing Material Normal Transmittance-Absorptance Product: Optional Double (but required in model)
    idfObject.setDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::BackingMaterialNormalTransmittanceAbsorptanceProduct,
                        modelObject.backingMaterialNormalTransmittanceAbsorptanceProduct());

    // Cladding Normal Transmittance-Absorptance Product: Optional Double (but required in model)
    idfObject.setDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::CladdingNormalTransmittanceAbsorptanceProduct,
                        modelObject.claddingNormalTransmittanceAbsorptanceProduct());

    // Fraction of Collector Gross Area Covered by PV Module: Optional Double (but required in model)
    idfObject.setDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::FractionofCollectorGrossAreaCoveredbyPVModule,
                        modelObject.fractionofCollectorGrossAreaCoveredbyPVModule());

    // Fraction of PV Cell Area to PV Module Area: Optional Double (but required in model)
    idfObject.setDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::FractionofPVCellAreatoPVModuleArea,
                        modelObject.fractionofPVCellAreatoPVModuleArea());

    // PV Module Top Thermal Resistance: Optional Double (but required in model)
    idfObject.setDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::PVModuleTopThermalResistance,
                        modelObject.pVModuleTopThermalResistance());

    // PV Module Bottom Thermal Resistance: Optional Double (but required in model)
    idfObject.setDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::PVModuleBottomThermalResistance,
                        modelObject.pVModuleBottomThermalResistance());

    // PV Module Front Longwave Emissivity: Optional Double (but required in model)
    idfObject.setDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::PVModuleFrontLongwaveEmissivity,
                        modelObject.pVModuleFrontLongwaveEmissivity());

    // PV Module Back Longwave Emissivity: Optional Double (but required in model)
    idfObject.setDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::PVModuleBackLongwaveEmissivity,
                        modelObject.pVModuleBackLongwaveEmissivity());

    // Glass Thickness: Optional Double (but required in model)
    idfObject.setDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::GlassThickness, modelObject.glassThickness());

    // Glass Refraction Index: Optional Double (but required in model)
    idfObject.setDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::GlassRefractionIndex, modelObject.glassRefractionIndex());

    // Glass Extinction Coefficient: Optional Double (but required in model)
    idfObject.setDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::GlassExtinctionCoefficient,
                        modelObject.glassExtinctionCoefficient());

    return idfObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
