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

#include "../ReverseTranslator.hpp"

#include "../../model/SolarCollectorPerformancePhotovoltaicThermalBIPVT.hpp"

#include "../../model/SurfacePropertyOtherSideConditionsModel.hpp"
#include "../../model/SurfacePropertyOtherSideConditionsModel_Impl.hpp"

#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include <utilities/idd/SolarCollectorPerformance_PhotovoltaicThermal_BIPVT_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateSolarCollectorPerformancePhotovoltaicThermalBIPVT(const WorkspaceObject& workspaceObject) {

    openstudio::model::SolarCollectorPerformancePhotovoltaicThermalBIPVT modelObject(m_model);

    // Name
    if (boost::optional<std::string> name_ = workspaceObject.name()) {
      modelObject.setName(name_.get());
    }

    // Boundary Conditions Model Name: Required Object
    if (boost::optional<WorkspaceObject> wo_ =
          workspaceObject.getTarget(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::BoundaryConditionsModelName)) {
      if (boost::optional<ModelObject> mo_ = translateAndMapWorkspaceObject(wo_.get())) {
        // TODO: check return types
        if (boost::optional<SurfacePropertyOtherSideConditionsModel> boundaryConditionsModel_ =
              mo_->optionalCast<SurfacePropertyOtherSideConditionsModel>()) {
          // The Ctor creates a BoundaryConditionsModel for us, so we can remove it now that we found the right one
          auto defaultOSCM = modelObject.boundaryConditionsModel();
          modelObject.setBoundaryConditionsModel(boundaryConditionsModel_.get());
          defaultOSCM.remove();
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Boundary Conditions Model Name'");
        }
      } else {
        LOG(Error, "For " << workspaceObject.briefDescription()
                          << ", cannot reverse translate required object 'Boundary Conditions Model Name'. Using defaulted one.");
      }
      LOG(Warn,
          "For " << workspaceObject.briefDescription() << ", cannot find required object 'Boundary Conditions Model Name'. Using defaulted one.");
    }
    // Availability Schedule Name: Optional Object
    if (boost::optional<WorkspaceObject> wo_ =
          workspaceObject.getTarget(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::AvailabilityScheduleName)) {
      if (boost::optional<ModelObject> mo_ = translateAndMapWorkspaceObject(wo_.get())) {
        // TODO: check return types
        if (boost::optional<Schedule> availabilitySchedule_ = mo_->optionalCast<Schedule>()) {
          modelObject.setAvailabilitySchedule(availabilitySchedule_.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Availability Schedule Name'");
        }
      }
    }
    // Effective Plenum Gap Thickness Behind PV Modules: Required Double
    if (boost::optional<double> effectivePlenumGapThicknessBehindPVModules_ =
          workspaceObject.getDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::EffectivePlenumGapThicknessBehindPVModules)) {
      modelObject.setEffectivePlenumGapThicknessBehindPVModules(effectivePlenumGapThicknessBehindPVModules_.get());
    } else {
      LOG(Error,
          "For " << workspaceObject.briefDescription() << ", cannot find required property 'Effective Plenum Gap Thickness Behind PV Modules'");
      return boost::none;
    }

    // PV Cell Normal Transmittance-Absorptance Product: Optional Double
    if (boost::optional<double> pVCellNormalTransmittanceAbsorptanceProduct_ =
          workspaceObject.getDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::PVCellNormalTransmittanceAbsorptanceProduct)) {
      modelObject.setPVCellNormalTransmittanceAbsorptanceProduct(pVCellNormalTransmittanceAbsorptanceProduct_.get());
    }

    // Backing Material Normal Transmittance-Absorptance Product: Optional Double
    if (boost::optional<double> backingMaterialNormalTransmittanceAbsorptanceProduct_ = workspaceObject.getDouble(
          SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::BackingMaterialNormalTransmittanceAbsorptanceProduct)) {
      modelObject.setBackingMaterialNormalTransmittanceAbsorptanceProduct(backingMaterialNormalTransmittanceAbsorptanceProduct_.get());
    }

    // Cladding Normal Transmittance-Absorptance Product: Optional Double
    if (boost::optional<double> claddingNormalTransmittanceAbsorptanceProduct_ =
          workspaceObject.getDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::CladdingNormalTransmittanceAbsorptanceProduct)) {
      modelObject.setCladdingNormalTransmittanceAbsorptanceProduct(claddingNormalTransmittanceAbsorptanceProduct_.get());
    }

    // Fraction of Collector Gross Area Covered by PV Module: Optional Double
    if (boost::optional<double> fractionofCollectorGrossAreaCoveredbyPVModule_ =
          workspaceObject.getDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::FractionofCollectorGrossAreaCoveredbyPVModule)) {
      modelObject.setFractionofCollectorGrossAreaCoveredbyPVModule(fractionofCollectorGrossAreaCoveredbyPVModule_.get());
    }

    // Fraction of PV Cell Area to PV Module Area: Optional Double
    if (boost::optional<double> fractionofPVCellAreatoPVModuleArea_ =
          workspaceObject.getDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::FractionofPVCellAreatoPVModuleArea)) {
      modelObject.setFractionofPVCellAreatoPVModuleArea(fractionofPVCellAreatoPVModuleArea_.get());
    }

    // PV Module Top Thermal Resistance: Optional Double
    if (boost::optional<double> pVModuleTopThermalResistance_ =
          workspaceObject.getDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::PVModuleTopThermalResistance)) {
      modelObject.setPVModuleTopThermalResistance(pVModuleTopThermalResistance_.get());
    }

    // PV Module Bottom Thermal Resistance: Optional Double
    if (boost::optional<double> pVModuleBottomThermalResistance_ =
          workspaceObject.getDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::PVModuleBottomThermalResistance)) {
      modelObject.setPVModuleBottomThermalResistance(pVModuleBottomThermalResistance_.get());
    }

    // PV Module Front Longwave Emissivity: Optional Double
    if (boost::optional<double> pVModuleFrontLongwaveEmissivity_ =
          workspaceObject.getDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::PVModuleFrontLongwaveEmissivity)) {
      modelObject.setPVModuleFrontLongwaveEmissivity(pVModuleFrontLongwaveEmissivity_.get());
    }

    // PV Module Back Longwave Emissivity: Optional Double
    if (boost::optional<double> pVModuleBackLongwaveEmissivity_ =
          workspaceObject.getDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::PVModuleBackLongwaveEmissivity)) {
      modelObject.setPVModuleBackLongwaveEmissivity(pVModuleBackLongwaveEmissivity_.get());
    }

    // Glass Thickness: Optional Double
    if (boost::optional<double> glassThickness_ =
          workspaceObject.getDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::GlassThickness)) {
      modelObject.setGlassThickness(glassThickness_.get());
    }

    // Glass Refraction Index: Optional Double
    if (boost::optional<double> glassRefractionIndex_ =
          workspaceObject.getDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::GlassRefractionIndex)) {
      modelObject.setGlassRefractionIndex(glassRefractionIndex_.get());
    }

    // Glass Extinction Coefficient: Optional Double
    if (boost::optional<double> glassExtinctionCoefficient_ =
          workspaceObject.getDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::GlassExtinctionCoefficient)) {
      modelObject.setGlassExtinctionCoefficient(glassExtinctionCoefficient_.get());
    }

    return modelObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
