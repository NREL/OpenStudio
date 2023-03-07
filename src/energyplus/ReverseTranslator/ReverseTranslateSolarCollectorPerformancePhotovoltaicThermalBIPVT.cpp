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

// TODO: Check the following class names against object getters and setters.
#include "../../model/OSCM.hpp"
#include "../../model/OSCM_Impl.hpp"

#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include <utilities/idd/SolarCollectorPerformance_PhotovoltaicThermal_BIPVT_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateSolarCollectorPerformancePhotovoltaicThermalBIPVT(const WorkspaceObject& workspaceObject) {
    boost::optional<ModelObject> result;
    boost::optional<WorkspaceObject> _wo;
    boost::optional<ModelObject> _mo;

    // Instantiate an object of the class to store the values,
    // but we don't return it until we know it's ok
    // TODO: check constructor, it might need other objects
    openstudio::model::SolarCollectorPerformancePhotovoltaicThermalBIPVT modelObject(m_model);

    // TODO: Note JM 2018-10-17
    // You are responsible for implementing any additional logic based on choice fields, etc.
    // The ReverseTranslator generator script is meant to facilitate your work, not get you 100% of the way

    // Name
    if (boost::optional<std::string> _name = workspaceObject.name()) {
      modelObject.setName(_name.get());
    }

    // Boundary Conditions Model Name: Required Object
    if ((_wo = workspaceObject.getTarget(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::BoundaryConditionsModelName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        // TODO: check return types
        if (boost::optional<OSCM> _boundaryConditionsModel = _mo->optionalCast<OSCM>()) {
          modelObject.setBoundaryConditionsModel(_boundaryConditionsModel.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Boundary Conditions Model Name'");
        }
      } else {
        LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot reverse translate required object 'Boundary Conditions Model Name'");
        return result;
      }
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required object 'Boundary Conditions Model Name'");
      return result;
    }
    // Availability Schedule Name: Optional Object
    if ((_wo = workspaceObject.getTarget(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::AvailabilityScheduleName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        // TODO: check return types
        if (boost::optional<Schedule> _availabilitySchedule = _mo->optionalCast<Schedule>()) {
          modelObject.setAvailabilitySchedule(_availabilitySchedule.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Availability Schedule Name'");
        }
      }
    }
    // Effective Plenum Gap Thickness Behind PV Modules: Required Double
    if (boost::optional<double> _effectivePlenumGapThicknessBehindPVModules =
          workspaceObject.getDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::EffectivePlenumGapThicknessBehindPVModules)) {
      modelObject.setEffectivePlenumGapThicknessBehindPVModules(_effectivePlenumGapThicknessBehindPVModules.get());
    } else {
      LOG(Error,
          "For " << workspaceObject.briefDescription() << ", cannot find required property 'Effective Plenum Gap Thickness Behind PV Modules'");
      return result;
    }

    // PV Cell Normal Transmittance-Absorptance Product: Optional Double
    if (boost::optional<double> _pVCellNormalTransmittanceAbsorptanceProduct =
          workspaceObject.getDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::PVCellNormalTransmittanceAbsorptanceProduct)) {
      modelObject.setPVCellNormalTransmittanceAbsorptanceProduct(_pVCellNormalTransmittanceAbsorptanceProduct.get());
    }

    // Backing Material Normal Transmittance-Absorptance Product: Optional Double
    if (boost::optional<double> _backingMaterialNormalTransmittanceAbsorptanceProduct = workspaceObject.getDouble(
          SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::BackingMaterialNormalTransmittanceAbsorptanceProduct)) {
      modelObject.setBackingMaterialNormalTransmittanceAbsorptanceProduct(_backingMaterialNormalTransmittanceAbsorptanceProduct.get());
    }

    // Cladding Normal Transmittance-Absorptance Product: Optional Double
    if (boost::optional<double> _claddingNormalTransmittanceAbsorptanceProduct =
          workspaceObject.getDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::CladdingNormalTransmittanceAbsorptanceProduct)) {
      modelObject.setCladdingNormalTransmittanceAbsorptanceProduct(_claddingNormalTransmittanceAbsorptanceProduct.get());
    }

    // Fraction of Collector Gross Area Covered by PV Module: Optional Double
    if (boost::optional<double> _fractionofCollectorGrossAreaCoveredbyPVModule =
          workspaceObject.getDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::FractionofCollectorGrossAreaCoveredbyPVModule)) {
      modelObject.setFractionofCollectorGrossAreaCoveredbyPVModule(_fractionofCollectorGrossAreaCoveredbyPVModule.get());
    }

    // Fraction of PV Cell Area to PV Module Area: Optional Double
    if (boost::optional<double> _fractionofPVCellAreatoPVModuleArea =
          workspaceObject.getDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::FractionofPVCellAreatoPVModuleArea)) {
      modelObject.setFractionofPVCellAreatoPVModuleArea(_fractionofPVCellAreatoPVModuleArea.get());
    }

    // PV Module Top Thermal Resistance: Optional Double
    if (boost::optional<double> _pVModuleTopThermalResistance =
          workspaceObject.getDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::PVModuleTopThermalResistance)) {
      modelObject.setPVModuleTopThermalResistance(_pVModuleTopThermalResistance.get());
    }

    // PV Module Bottom Thermal Resistance: Optional Double
    if (boost::optional<double> _pVModuleBottomThermalResistance =
          workspaceObject.getDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::PVModuleBottomThermalResistance)) {
      modelObject.setPVModuleBottomThermalResistance(_pVModuleBottomThermalResistance.get());
    }

    // PV Module Front Longwave Emissivity: Optional Double
    if (boost::optional<double> _pVModuleFrontLongwaveEmissivity =
          workspaceObject.getDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::PVModuleFrontLongwaveEmissivity)) {
      modelObject.setPVModuleFrontLongwaveEmissivity(_pVModuleFrontLongwaveEmissivity.get());
    }

    // PV Module Back Longwave Emissivity: Optional Double
    if (boost::optional<double> _pVModuleBackLongwaveEmissivity =
          workspaceObject.getDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::PVModuleBackLongwaveEmissivity)) {
      modelObject.setPVModuleBackLongwaveEmissivity(_pVModuleBackLongwaveEmissivity.get());
    }

    // Glass Thickness: Optional Double
    if (boost::optional<double> _glassThickness =
          workspaceObject.getDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::GlassThickness)) {
      modelObject.setGlassThickness(_glassThickness.get());
    }

    // Glass Refraction Index: Optional Double
    if (boost::optional<double> _glassRefractionIndex =
          workspaceObject.getDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::GlassRefractionIndex)) {
      modelObject.setGlassRefractionIndex(_glassRefractionIndex.get());
    }

    // Glass Extinction Coefficient: Optional Double
    if (boost::optional<double> _glassExtinctionCoefficient =
          workspaceObject.getDouble(SolarCollectorPerformance_PhotovoltaicThermal_BIPVTFields::GlassExtinctionCoefficient)) {
      modelObject.setGlassExtinctionCoefficient(_glassExtinctionCoefficient.get());
    }

    result = modelObject;
    return result;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
