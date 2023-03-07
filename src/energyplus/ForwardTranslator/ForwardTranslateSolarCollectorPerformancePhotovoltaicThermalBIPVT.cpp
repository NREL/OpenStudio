/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
