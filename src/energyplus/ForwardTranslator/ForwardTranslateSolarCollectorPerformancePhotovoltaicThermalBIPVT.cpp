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
#include "../../model/Model.hpp"

#include "../../model/SolarCollectorPerformancePhotovoltaicThermalBIPVT.hpp"

// TODO: Check the following class names against object getters and setters.
#include "../../model/OSCM.hpp"
#include "../../model/OSCM_Impl.hpp"

#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include <utilities/idd/SolarCollectorPerformance_PhotovoltaicThermal_BIPVT_FieldEnums.hxx>
// #include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateSolarCollectorPerformancePhotovoltaicThermalBIPVT(
    model::SolarCollectorPerformancePhotovoltaicThermalBIPVT& modelObject) {
    boost::optional<IdfObject> result;
    boost::optional<WorkspaceObject> _wo;
    boost::optional<ModelObject> _mo;

    // Instantiate an IdfObject of the class to store the values
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::SolarCollectorPerformance_PhotovoltaicThermal_BIPVT, modelObject);
    // If it doesn't have a name, or if you aren't sure you are going to want to return it
    // IdfObject idfObject( openstudio::IddObjectType::SolarCollectorPerformance_PhotovoltaicThermal_BIPVT );
    // m_idfObjects.push_back(idfObject);

    // TODO: Note JM 2018-10-17
    // You are responsible for implementing any additional logic based on choice fields, etc.
    // The ForwardTranslator generator script is meant to facilitate your work, not get you 100% of the way

    // TODO: If you keep createRegisterAndNameIdfObject above, you don't need this.
    // But in some cases, you'll want to handle failure without pushing to the map
    // Name
    if (boost::optional<std::string> moName = modelObject.name()) {
      idfObject.setName(*moName);
    }

    // Boundary Conditions Model Name: Required Object
    OSCM boundaryConditionsModel = modelObject.boundaryConditionsModel();
    if (boost::optional<IdfObject> _owo = translateAndMapModelObject(boundaryConditionsModel)) {
      idfObject.setString(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::BoundaryConditionsModelName, _owo->nameString());
    }

    // Availability Schedule Name: Optional Object
    if (boost::optional<Schedule> _availabilitySchedule = modelObject.availabilitySchedule()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_availabilitySchedule.get())) {
        idfObject.setString(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::AvailabilityScheduleName, _owo->nameString());
      }
    }

    // Effective Plenum Gap Thickness Behind PV Modules: Required Double
    double effectivePlenumGapThicknessBehindPVModules = modelObject.effectivePlenumGapThicknessBehindPVModules();
    idfObject.setDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::EffectivePlenumGapThicknessBehindPVModules,
                        effectivePlenumGapThicknessBehindPVModules);

    // PV Cell Normal Transmittance-Absorptance Product: Optional Double
    double pVCellNormalTransmittanceAbsorptanceProduct = modelObject.pVCellNormalTransmittanceAbsorptanceProduct();
    idfObject.setDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::PVCellNormalTransmittanceAbsorptanceProduct,
                        pVCellNormalTransmittanceAbsorptanceProduct);

    // Backing Material Normal Transmittance-Absorptance Product: Optional Double
    double backingMaterialNormalTransmittanceAbsorptanceProduct = modelObject.backingMaterialNormalTransmittanceAbsorptanceProduct();
    idfObject.setDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::BackingMaterialNormalTransmittanceAbsorptanceProduct,
                        backingMaterialNormalTransmittanceAbsorptanceProduct);

    // Cladding Normal Transmittance-Absorptance Product: Optional Double
    double claddingNormalTransmittanceAbsorptanceProduct = modelObject.claddingNormalTransmittanceAbsorptanceProduct();
    idfObject.setDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::CladdingNormalTransmittanceAbsorptanceProduct,
                        claddingNormalTransmittanceAbsorptanceProduct);

    // Fraction of Collector Gross Area Covered by PV Module: Optional Double
    double fractionofCollectorGrossAreaCoveredbyPVModule = modelObject.fractionofCollectorGrossAreaCoveredbyPVModule();
    idfObject.setDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::FractionofCollectorGrossAreaCoveredbyPVModule,
                        fractionofCollectorGrossAreaCoveredbyPVModule);

    // Fraction of PV Cell Area to PV Module Area: Optional Double
    double fractionofPVCellAreatoPVModuleArea = modelObject.fractionofPVCellAreatoPVModuleArea();
    idfObject.setDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::FractionofPVCellAreatoPVModuleArea,
                        fractionofPVCellAreatoPVModuleArea);

    // PV Module Top Thermal Resistance: Optional Double
    double pVModuleTopThermalResistance = modelObject.pVModuleTopThermalResistance();
    idfObject.setDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::PVModuleTopThermalResistance, pVModuleTopThermalResistance);

    // PV Module Bottom Thermal Resistance: Optional Double
    double pVModuleBottomThermalResistance = modelObject.pVModuleBottomThermalResistance();
    idfObject.setDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::PVModuleBottomThermalResistance, pVModuleBottomThermalResistance);

    // PV Module Front Longwave Emissivity: Optional Double
    double pVModuleFrontLongwaveEmissivity = modelObject.pVModuleFrontLongwaveEmissivity();
    idfObject.setDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::PVModuleFrontLongwaveEmissivity, pVModuleFrontLongwaveEmissivity);

    // PV Module Back Longwave Emissivity: Optional Double
    double pVModuleBackLongwaveEmissivity = modelObject.pVModuleBackLongwaveEmissivity();
    idfObject.setDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::PVModuleBackLongwaveEmissivity, pVModuleBackLongwaveEmissivity);

    // Glass Thickness: Optional Double
    double glassThickness = modelObject.glassThickness();
    idfObject.setDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::GlassThickness, glassThickness);

    // Glass Refraction Index: Optional Double
    double glassRefractionIndex = modelObject.glassRefractionIndex();
    idfObject.setDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::GlassRefractionIndex, glassRefractionIndex);

    // Glass Extinction Coefficient: Optional Double
    double glassExtinctionCoefficient = modelObject.glassExtinctionCoefficient();
    idfObject.setDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::GlassExtinctionCoefficient, glassExtinctionCoefficient);

    return idfObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
