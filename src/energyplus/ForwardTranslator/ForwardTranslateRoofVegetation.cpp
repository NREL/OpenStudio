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
#include "../../model/RoofVegetation.hpp"
#include <utilities/idd/OS_Material_RoofVegetation_FieldEnums.hxx>

#include <utilities/idd/Material_RoofVegetation_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateRoofVegetation(RoofVegetation& modelObject) {
    IdfObject idfObject(openstudio::IddObjectType::Material_RoofVegetation);

    m_idfObjects.push_back(idfObject);

    idfObject.setString(Material_RoofVegetationFields::Name, modelObject.name().get());

    OptionalDouble d = modelObject.getDouble(OS_Material_RoofVegetationFields::HeightofPlants, false);
    if (d) {
      idfObject.setDouble(Material_RoofVegetationFields::HeightofPlants, *d);
    }

    d = modelObject.getDouble(OS_Material_RoofVegetationFields::LeafAreaIndex, false);
    if (d) {
      idfObject.setDouble(Material_RoofVegetationFields::LeafAreaIndex, *d);
    }

    d = modelObject.getDouble(OS_Material_RoofVegetationFields::LeafReflectivity, false);
    if (d) {
      idfObject.setDouble(Material_RoofVegetationFields::LeafReflectivity, *d);
    }

    d = modelObject.getDouble(OS_Material_RoofVegetationFields::LeafEmissivity, false);
    if (d) {
      idfObject.setDouble(Material_RoofVegetationFields::LeafEmissivity, *d);
    }

    d = modelObject.getDouble(OS_Material_RoofVegetationFields::MinimumStomatalResistance, false);
    if (d) {
      idfObject.setDouble(Material_RoofVegetationFields::MinimumStomatalResistance, *d);
    }

    OptionalString s = modelObject.getString(OS_Material_RoofVegetationFields::SoilLayerName, false, true);
    if (s) {
      idfObject.setString(Material_RoofVegetationFields::SoilLayerName, *s);
    }

    s = modelObject.getString(OS_Material_RoofVegetationFields::Roughness, false, true);
    if (s) {
      idfObject.setString(Material_RoofVegetationFields::Roughness, *s);
    }

    d = modelObject.getDouble(OS_Material_RoofVegetationFields::Thickness, false);
    if (d) {
      idfObject.setDouble(Material_RoofVegetationFields::Thickness, *d);
    }

    d = modelObject.getDouble(OS_Material_RoofVegetationFields::ConductivityofDrySoil, false);
    if (d) {
      idfObject.setDouble(Material_RoofVegetationFields::ConductivityofDrySoil, *d);
    }

    d = modelObject.getDouble(OS_Material_RoofVegetationFields::DensityofDrySoil, false);
    if (d) {
      idfObject.setDouble(Material_RoofVegetationFields::DensityofDrySoil, *d);
    }

    d = modelObject.getDouble(OS_Material_RoofVegetationFields::SpecificHeatofDrySoil, false);
    if (d) {
      idfObject.setDouble(Material_RoofVegetationFields::SpecificHeatofDrySoil, *d);
    }

    d = modelObject.getDouble(OS_Material_RoofVegetationFields::ThermalAbsorptance, false);
    if (d) {
      idfObject.setDouble(Material_RoofVegetationFields::ThermalAbsorptance, *d);
    }

    d = modelObject.getDouble(OS_Material_RoofVegetationFields::SolarAbsorptance, false);
    if (d) {
      idfObject.setDouble(Material_RoofVegetationFields::SolarAbsorptance, *d);
    }

    d = modelObject.getDouble(OS_Material_RoofVegetationFields::VisibleAbsorptance, false);
    if (d) {
      idfObject.setDouble(Material_RoofVegetationFields::VisibleAbsorptance, *d);
    }

    d = modelObject.getDouble(OS_Material_RoofVegetationFields::SaturationVolumetricMoistureContentoftheSoilLayer, false);
    if (d) {
      idfObject.setDouble(Material_RoofVegetationFields::SaturationVolumetricMoistureContentoftheSoilLayer, *d);
    }

    d = modelObject.getDouble(OS_Material_RoofVegetationFields::ResidualVolumetricMoistureContentoftheSoilLayer, false);
    if (d) {
      idfObject.setDouble(Material_RoofVegetationFields::ResidualVolumetricMoistureContentoftheSoilLayer, *d);
    }

    d = modelObject.getDouble(OS_Material_RoofVegetationFields::InitialVolumetricMoistureContentoftheSoilLayer, false);
    if (d) {
      idfObject.setDouble(Material_RoofVegetationFields::InitialVolumetricMoistureContentoftheSoilLayer, *d);
    }

    s = modelObject.getString(OS_Material_RoofVegetationFields::MoistureDiffusionCalculationMethod, false, true);
    if (s) {
      idfObject.setString(Material_RoofVegetationFields::MoistureDiffusionCalculationMethod, *s);
    }

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
