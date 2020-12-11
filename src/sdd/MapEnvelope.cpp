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

#include "ReverseTranslator.hpp"
#include "ForwardTranslator.hpp"

#include "../model/Model.hpp"
#include "../model/ModelObject.hpp"
#include "../model/ModelObject_Impl.hpp"
#include "../model/Construction.hpp"
#include "../model/Construction_Impl.hpp"
#include "../model/MasslessOpaqueMaterial.hpp"
#include "../model/MasslessOpaqueMaterial_Impl.hpp"
#include "../model/AirGap.hpp"
#include "../model/AirGap_Impl.hpp"
#include "../model/StandardOpaqueMaterial.hpp"
#include "../model/StandardOpaqueMaterial_Impl.hpp"
#include "../model/ConstructionWithInternalSource.hpp"
#include "../model/ConstructionWithInternalSource_Impl.hpp"
#include "../model/FFactorGroundFloorConstruction.hpp"
#include "../model/FFactorGroundFloorConstruction_Impl.hpp"
#include "../model/CFactorUndergroundWallConstruction.hpp"
#include "../model/CFactorUndergroundWallConstruction_Impl.hpp"
#include "../model/FenestrationMaterial.hpp"
#include "../model/FenestrationMaterial_Impl.hpp"
#include "../model/ModelPartitionMaterial.hpp"
#include "../model/ModelPartitionMaterial_Impl.hpp"
#include "../model/SimpleGlazing.hpp"
#include "../model/SimpleGlazing_Impl.hpp"
#include "../model/StandardsInformationConstruction.hpp"
#include "../model/StandardsInformationConstruction_Impl.hpp"
#include "../model/StandardsInformationMaterial.hpp"
#include "../model/StandardsInformationMaterial_Impl.hpp"

#include "../utilities/units/QuantityConverter.hpp"
#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/IPUnit.hpp"
#include "../utilities/units/SIUnit.hpp"
#include "../utilities/units/BTUUnit.hpp"
#include "../utilities/units/WhUnit.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/core/StringHelpers.hpp"

#include <pugixml.hpp>

namespace openstudio {
namespace sdd {

  boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateConstructAssembly(const pugi::xml_node& element,
                                                                                                openstudio::model::Model& model) {
    boost::optional<openstudio::model::ModelObject> result;

    pugi::xml_node nameElement = element.child("Name");
    std::string name;
    if (!nameElement) {
      LOG(Error, "ConsAssm element 'Name' is empty.")
    } else {
      name = escapeName(nameElement.text().as_string());
    }

    pugi::xml_node specificationElement = element.child("SpecMthd");
    if (!specificationElement) {
      LOG(Error, "ConsAssm element 'SpecMthd' is empty for construction named '" << name << "'. Construction will not be created");
      return boost::none;
    }

    if (openstudio::istringEqual(specificationElement.text().as_string(), "Layers")) {

      openstudio::model::Construction construction(model);
      construction.setName(name);

      std::vector<model::Material> materials;
      for (const pugi::xml_node& materialElement : element.children("MatRef")) {
        std::string materialName = escapeName(materialElement.text().as_string());
        boost::optional<model::Material> material = model.getModelObjectByName<model::Material>(materialName);
        if (!material) {
          LOG(Error, "Construction: " << construction.name().get() << " references material: " << materialName << " that is not defined.");

          // DLM: what to do?  Remove the construction?
        } else {
          materials.push_back(*material);
        }
      }

      bool test = construction.setLayers(materials);
      if (!test) {
        LOG(Error, "Failed to set material layers for Construction: " << construction.name().get() << ".");

        // DLM: what to do?  Remove the construction?
      }

      bool wasFastNaming = model.fastNaming();
      model.setFastNaming(false);

      unsigned n = (unsigned)materials.size();
      construction.ensureUniqueLayers();
      materials = construction.layers();  // DLM: get materials again in case new ones were cloned

      OS_ASSERT(n == materials.size());

      model.setFastNaming(wasFastNaming);

      if (n > 0) {

        // DLM: are these fields only on layered constructions?
        pugi::xml_node extRoughnessElement = element.child("ExtRoughness");  // enumerated value
        pugi::xml_node extSolAbsElement = element.child("ExtSolAbs");
        pugi::xml_node extThrmlAbsElement = element.child("ExtThrmlAbs");
        pugi::xml_node extVisAbsElement = element.child("ExtVisAbs");
        pugi::xml_node intSolAbsElement = element.child("IntSolAbs");
        pugi::xml_node intThrmlAbsElement = element.child("IntThrmlAbs");
        pugi::xml_node intVisAbsElement = element.child("IntVisAbs");

        if (materials[0].optionalCast<model::StandardOpaqueMaterial>()) {
          model::StandardOpaqueMaterial extMaterial = materials[0].cast<model::StandardOpaqueMaterial>();

          std::string roughness = "MediumRough";
          if (extRoughnessElement) {
            roughness = extRoughnessElement.text().as_string();
          }
          extMaterial.setRoughness(roughness);

          if (extThrmlAbsElement) {
            extMaterial.setThermalAbsorptance(extThrmlAbsElement.text().as_double());
          }

          if (extSolAbsElement) {
            extMaterial.setSolarAbsorptance(extSolAbsElement.text().as_double());
          }

          if (extVisAbsElement) {
            extMaterial.setVisibleAbsorptance(extVisAbsElement.text().as_double());
          }
        } else if (materials[0].optionalCast<model::MasslessOpaqueMaterial>()) {
          model::MasslessOpaqueMaterial extMaterial = materials[0].cast<model::MasslessOpaqueMaterial>();

          std::string roughness = "MediumRough";
          if (extRoughnessElement) {
            roughness = extRoughnessElement.text().as_string();
          }
          extMaterial.setRoughness(roughness);

          if (extThrmlAbsElement) {
            extMaterial.setThermalAbsorptance(extThrmlAbsElement.text().as_double());
          }

          if (extSolAbsElement) {
            extMaterial.setSolarAbsorptance(extSolAbsElement.text().as_double());
          }

          if (extVisAbsElement) {
            extMaterial.setVisibleAbsorptance(extVisAbsElement.text().as_double());
          }
        }

        // DLM: if only one layer use properties from exterior material
        if (n > 1) {
          if (materials[n - 1].optionalCast<model::StandardOpaqueMaterial>()) {
            model::StandardOpaqueMaterial intMaterial = materials[n - 1].cast<model::StandardOpaqueMaterial>();

            if (intThrmlAbsElement) {
              intMaterial.setThermalAbsorptance(intThrmlAbsElement.text().as_double());
            }

            if (intSolAbsElement) {
              intMaterial.setSolarAbsorptance(intSolAbsElement.text().as_double());
            }

            if (intVisAbsElement) {
              intMaterial.setVisibleAbsorptance(intVisAbsElement.text().as_double());
            }
          } else if (materials[n - 1].optionalCast<model::MasslessOpaqueMaterial>()) {
            model::MasslessOpaqueMaterial intMaterial = materials[n - 1].cast<model::MasslessOpaqueMaterial>();

            if (intThrmlAbsElement) {
              intMaterial.setThermalAbsorptance(intThrmlAbsElement.text().as_double());
            }

            if (intSolAbsElement) {
              intMaterial.setSolarAbsorptance(intSolAbsElement.text().as_double());
            }

            if (intVisAbsElement) {
              intMaterial.setVisibleAbsorptance(intVisAbsElement.text().as_double());
            }
          }
        }
      }

      result = construction;

    } else if (openstudio::istringEqual(specificationElement.text().as_string(), "CFactor")) {

      // DLM: per input from David Reddy this construction will be cloned
      // for each surface that uses it and height set per surface
      //

      pugi::xml_node cFactorElement = element.child("CFactor");
      if (!cFactorElement) {
        LOG(Error, "ConsAssm required element 'CFactor' is empty for construction named '" << name << "'. Construction will not be created");
        return boost::none;
      }

      openstudio::model::CFactorUndergroundWallConstruction construction(model);
      construction.setName(name);

      // sdd units = Btu/(hr*ft^2*F), os units = W/(m^2*K)
      Quantity cFactorIP(cFactorElement.text().as_double(), BTUUnit(BTUExpnt(1, -2, -1, -1)));
      OptionalQuantity cFactorSI = QuantityConverter::instance().convert(cFactorIP, UnitSystem(UnitSystem::Wh));
      OS_ASSERT(cFactorSI);
      OS_ASSERT(cFactorSI->units() == WhUnit(WhExpnt(1, 0, -2, -1)));
      construction.setCFactor(cFactorSI->value());

      result = construction;

    } else if (openstudio::istringEqual(specificationElement.text().as_string(), "FFactor")) {

      // DLM: per input from David Reddy this construction will be cloned
      // for each surface that uses it and area and perimeter set per surface

      pugi::xml_node fFactorElement = element.child("FFactor");
      if (!fFactorElement) {
        LOG(Error, "ConsAssm required element 'FFactor' is empty for construction named '" << name << "'. Construction will not be created");
        return boost::none;
      }

      openstudio::model::FFactorGroundFloorConstruction construction(model);
      construction.setName(name);

      // sdd units = Btu/(hr*ft*F), os units = W/(m*K)
      Quantity fFactorIP(fFactorElement.text().as_double(), BTUUnit(BTUExpnt(1, -1, -1, -1)));
      OptionalQuantity fFactorSI = QuantityConverter::instance().convert(fFactorIP, UnitSystem(UnitSystem::Wh));
      OS_ASSERT(fFactorSI);
      OS_ASSERT(fFactorSI->units() == WhUnit(WhExpnt(1, 0, -1, -1)));
      construction.setFFactor(fFactorSI->value());

      result = construction;

    } else if (openstudio::istringEqual(specificationElement.text().as_string(), "UFactor")) {

      // DLM: todo create object, do we need this now?
    }

    return result;
  }

  boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateDoorConstruction(const pugi::xml_node& element,
                                                                                               openstudio::model::Model& model) {
    //SIExpnt(int kg=0,int m=0,int s=0,int K=0,..)
    //IPExpnt(int lbm=0,int ft=0,int s=0,int R=0,..)
    //BTUExpnt(int btu=0,int ft=0,int h=0,int R=0,..)
    //WhExpnt(int W=0,int h=0,int m=0,int K=0,..)

    UnitSystem siSys(UnitSystem::SI);
    UnitSystem whSys(UnitSystem::Wh);

    pugi::xml_node nameElement = element.child("Name");
    pugi::xml_node uFactorElement = element.child("UFactorSim");  // Btu/h-ft2-F

    if (!nameElement) {
      LOG(Error, "DrCons element 'Name' is empty.  Construction will not be created.");
    }
    if (!uFactorElement) {
      LOG(Error, "DrCons element 'UFactorSim' is empty.  Construction will not be created.");
    }

    if (nameElement && uFactorElement) {

      // sdd units = Btu/(hr*ft^2*F), os units = W/m^2-K
      Quantity uFactorIP(uFactorElement.text().as_double(), BTUUnit(BTUExpnt(1, -2, -1, -1)));
      OptionalQuantity uFactorWh = QuantityConverter::instance().convert(uFactorIP, whSys);
      OS_ASSERT(uFactorWh);
      OS_ASSERT(uFactorWh->units() == WhUnit(WhExpnt(1, 0, -2, -1)));

      // m^2-K/W
      double rValueSI = 1.0 / uFactorWh->value();

      // 5.6783 hr-ft2-F/Btu = 1 m^2-K/W
      const double intFilmRValueSI = 0.170 / 5.6783;  // 0.170 hr-ft2-F/Btu
      const double extFilmRValueSI = 0.680 / 5.6783;  // 0.680 hr-ft2-F/Btu

      // if total rvalue is less than sum of interior and exterior film rvalue, surface coefficients will be added to door separately
      rValueSI = std::max(rValueSI - intFilmRValueSI - extFilmRValueSI, 0.001);

      std::string name = escapeName(nameElement.text().as_string());

      model::MasslessOpaqueMaterial material(model);
      material.setName(name + " Material");
      material.setRoughness("MediumSmooth");
      material.setThermalResistance(rValueSI);

      openstudio::model::MaterialVector layers;
      layers.push_back(material);

      openstudio::model::Construction construction(model);
      construction.setName(name);
      construction.setLayers(layers);

      return construction;
    }

    return boost::none;
  }

  boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateFenestrationConstruction(const pugi::xml_node& element,
                                                                                                       openstudio::model::Model& model) {
    //SIExpnt(int kg=0,int m=0,int s=0,int K=0,..)
    //IPExpnt(int lbm=0,int ft=0,int s=0,int R=0,..)
    //BTUExpnt(int btu=0,int ft=0,int h=0,int R=0,..)
    //WhExpnt(int W=0,int h=0,int m=0,int K=0,..)

    UnitSystem siSys(UnitSystem::SI);
    UnitSystem whSys(UnitSystem::Wh);

    pugi::xml_node nameElement = element.child("Name");
    pugi::xml_node shgcElement = element.child("SHGC");           // unitless
    pugi::xml_node uFactorElement = element.child("UFactorSim");  // Btu/h-ft2-F
    pugi::xml_node vtElement = element.child("VT");               // unitless

    if (!nameElement) {
      LOG(Error, "FenCons element 'Name' is empty.  Construction will not be created.");
    }
    if (!shgcElement) {
      LOG(Error, "FenCons element 'SHGC' is empty.  Construction will not be created.");
    }
    if (!uFactorElement) {
      LOG(Error, "FenCons element 'UFactorSim' is empty.  Construction will not be created.");
    }
    if (!vtElement) {
      LOG(Error, "FenCons element 'VT' is empty.  Construction will not be created.");
    }

    if (nameElement && shgcElement && uFactorElement && vtElement) {

      std::string name = escapeName(nameElement.text().as_string());

      openstudio::model::SimpleGlazing material(model);
      material.setName(name + " Material");
      material.setSolarHeatGainCoefficient(shgcElement.text().as_double());
      material.setVisibleTransmittance(vtElement.text().as_double());

      // sdd units = Btu/(hr*ft^2*F), os units = W/m^2-K
      Quantity uFactorIP(uFactorElement.text().as_double(), BTUUnit(BTUExpnt(1, -2, -1, -1)));
      OptionalQuantity uFactorWh = QuantityConverter::instance().convert(uFactorIP, whSys);
      OS_ASSERT(uFactorWh);
      OS_ASSERT(uFactorWh->units() == WhUnit(WhExpnt(1, 0, -2, -1)));
      material.setUFactor(uFactorWh->value());

      openstudio::model::MaterialVector layers;
      layers.push_back(material);

      openstudio::model::Construction construction(model);
      construction.setName(name);
      construction.setLayers(layers);

      return construction;
    }
    return boost::none;
  }

  boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateMaterial(const pugi::xml_node& element,
                                                                                       openstudio::model::Model& model) {

    //SIExpnt(int kg=0,int m=0,int s=0,int K=0,..)
    //IPExpnt(int lbm=0,int ft=0,int s=0,int R=0,..)
    //BTUExpnt(int btu=0,int ft=0,int h=0,int R=0,..)
    //WhExpnt(int W=0,int h=0,int m=0,int K=0,..)

    UnitSystem siSys(UnitSystem::SI);
    UnitSystem whSys(UnitSystem::Wh);

    pugi::xml_node nameElement = element.child("Name");
    //pugi::xml_node roughnessElement = element.child("Roughness"); // {VeryRough, Rough, MediumRough, MediumSmooth, Smooth, VerySmooth}
    pugi::xml_node thicknessElement = element.child("Thkns");                 // in
    pugi::xml_node thermalConductivityElement = element.child("ThrmlCndct");  // Btu/(hr*ft*F)
    pugi::xml_node densityElement = element.child("Dens");                    // lb/ft^3 // DLM: is this right? schema says could also be lb/ft^2
    pugi::xml_node specificHeatElement = element.child("SpecHt");             // Btu/(lb*F)
    //pugi::xml_node thermalAbsorptanceElement = element.child("ThrmlAbs"); // 0-1
    //pugi::xml_node solarAbsorbanceElement = element.child("SolAbs"); // 0-1
    //pugi::xml_node visibleAbsorptanceElement = element.child("VisAbs"); // 0-1
    pugi::xml_node rValueElement = element.child("RVal");  // hr*ft2*degF/Btu

    if (!nameElement) {
      LOG(Error, "Mat element 'Name' is empty.  Material will not be created.");
      return boost::none;
    }

    std::string name = escapeName(nameElement.text().as_string());

    /*
OS:Material
Name - required
Roughness - default to MediumRough
Thickness - required, m
Conductivity - required, W/m-K
Density - required, kg/m3
Specific Heat - required, J/kg-K
Thermal Absorptance, IDD default of 0.9
Solar Absorptance, IDD default of 0.7
Visible Absorptance, IDD default of 0.7
*/
    if (nameElement && thicknessElement && thermalConductivityElement && densityElement && specificHeatElement) {

      openstudio::model::StandardOpaqueMaterial material(model);

      material.setName(name);

      // DLM: set on construction
      //std::string roughness = "MediumRough";
      //if (roughnessElement) {
      //  roughness = roughnessElement.text().as_string();
      //}
      //material.setRoughness(roughness);

      // sdd units = in, os units = m
      double thicknessInches = thicknessElement.text().as_double();
      double thicknessFeet = thicknessInches / 12.0;
      Quantity thicknessIP(thicknessFeet, IPUnit(IPExpnt(0, 1, 0)));
      OptionalQuantity thicknessSI = QuantityConverter::instance().convert(thicknessIP, siSys);
      OS_ASSERT(thicknessSI);
      OS_ASSERT(thicknessSI->units() == SIUnit(SIExpnt(0, 1, 0)));
      material.setThickness(thicknessSI->value());

      // sdd units = Btu/(hr*ft*F), os units = W/m-K
      Quantity thermalConductivityIP(thermalConductivityElement.text().as_double(), BTUUnit(BTUExpnt(1, -1, -1, -1)));
      OptionalQuantity thermalConductivityWh = QuantityConverter::instance().convert(thermalConductivityIP, whSys);
      OS_ASSERT(thermalConductivityWh);
      OS_ASSERT(thermalConductivityWh->units() == WhUnit(WhExpnt(1, 0, -1, -1)));
      material.setThermalConductivity(thermalConductivityWh->value());

      // sdd units = lb/ft^3, os units = kg/m3
      Quantity densityIP(densityElement.text().as_double(), IPUnit(IPExpnt(1, -3, 0)));
      OptionalQuantity densitySI = QuantityConverter::instance().convert(densityIP, siSys);
      OS_ASSERT(densitySI);
      OS_ASSERT(densitySI->units() == SIUnit(SIExpnt(1, -3, 0)));
      material.setDensity(densitySI->value());

      // sdd units = Btu/(lb*F), os units = J/kg-K = m^2/(s^2*K)
      Quantity specificHeatIP(specificHeatElement.text().as_double(), BTUUnit(BTUExpnt(1, 0, 0, -1)) * IPUnit(IPExpnt(-1)));
      OptionalQuantity specificHeatSI = QuantityConverter::instance().convert(specificHeatIP, siSys);
      OS_ASSERT(specificHeatSI);
      OS_ASSERT(specificHeatSI->units() == SIUnit(SIExpnt(0, 2, -2, -1)));
      material.setSpecificHeat(specificHeatSI->value());

      // DLM: set on construction
      //if (thermalAbsorptanceElement) {
      //  material.setThermalAbsorptance(thermalAbsorptanceElement.text().as_double());
      //}

      // DLM: set on construction
      //if (solarAbsorbanceElement) {
      //  material.setSolarAbsorptance(solarAbsorbanceElement.text().as_double());
      //}

      // DLM: set on construction
      //if (visibleAbsorptanceElement) {
      //  material.setVisibleAbsorptance(visibleAbsorptanceElement.text().as_double());
      //}

      return material;
    }

    /*
OS:Material:NoMass,
Name - required
Roughness - default to MediumRough
Thermal Resistance - required, m2-K/W
Thermal Absorptance, IDD default of 0.9
Solar Absorptance, IDD default of 0.7
Visible Absorptance, IDD default of 0.7
*/
    if (nameElement && rValueElement) {

      openstudio::model::MasslessOpaqueMaterial material(model);

      material.setName(name);

      // DLM: set on construction
      //std::string roughness = "MediumRough";
      //if (roughnessElement) {
      //  roughness = roughnessElement.text().as_string();
      //}
      //material.setRoughness(roughness);

      // sdd units = hr*ft2*degF/Btu, os units = m2-K/W
      Quantity rValueIP(rValueElement.text().as_double(), BTUUnit(BTUExpnt(-1, 2, 1, 1)));
      OptionalQuantity rValueWh = QuantityConverter::instance().convert(rValueIP, whSys);
      OS_ASSERT(rValueWh);
      OS_ASSERT(rValueWh->units() == WhUnit(WhExpnt(-1, 0, 2, 1)));
      material.setThermalResistance(rValueWh->value());

      // DLM: set on construction
      //if (thermalAbsorptanceElement) {
      //  material.setThermalAbsorptance(thermalAbsorptanceElement.text().as_double());
      //}

      // DLM: set on construction
      //if (solarAbsorbanceElement) {
      //  material.setSolarAbsorptance(solarAbsorbanceElement.text().as_double());
      //}

      // DLM: set on construction
      //if (visibleAbsorptanceElement) {
      //  material.setVisibleAbsorptance(visibleAbsorptanceElement.text().as_double());
      //}

      return material;
    }

    LOG(Error, "Material named '" << name << "' could not be created.")

    return boost::none;
  }

  boost::optional<pugi::xml_node> ForwardTranslator::translateConstructionBase(const openstudio::model::ConstructionBase& constructionBase,
                                                                               pugi::xml_node& root) {
    boost::optional<pugi::xml_node> result;

    if (constructionBase.optionalCast<model::LayeredConstruction>()) {
      model::LayeredConstruction construction = constructionBase.cast<model::LayeredConstruction>();
      model::StandardsInformationConstruction info = constructionBase.standardsInformation();

      // TODO: unused
      [[maybe_unused]] bool heated = false;
      if (construction.optionalCast<model::Construction>()) {
        heated = false;
      } else if (construction.optionalCast<model::ConstructionWithInternalSource>()) {
        heated = true;
      } else {
        return boost::none;
      }

      // check if any layer has not been translated, constructions using simple glazing material will
      // be skipped here because the simple glazing material is not recorded as mapped
      std::vector<model::Material> layers = construction.layers();
      for (const model::Material& material : layers) {
        if (m_translatedObjects.find(material.handle()) == m_translatedObjects.end()) {
          return boost::none;
        }
      }

      result = root.append_child("ConsAssm");

      // name
      std::string name = escapeName(construction.name().get());

      pugi::xml_node subElement = result->append_child("Name");
      subElement.text().set(name.c_str());

      // DLM: prescribed, not input
      // specification method
      //pugi::xml_node specMthdElement = result->append_child("SpecMthd");
      //specMthdElement.text().set("Layers");

      // SDD:
      // CompatibleSurfType - required, done
      // ExtSolAbs - optional, done
      // ExtThrmlAbs - optional, done
      // ExtVisAbs - optional, done
      // IntSolAbs - optional, done
      // IntThrmlAbs - optional, done
      // IntVisAbs - optional, done
      // SlabType - optional, skipping until we better handle slabs
      // SlabInsOrientation - optional, skipping until we better handle slabs
      // SlabInsThrmlR - optional, skipping until we better handle slabs
      // FieldAppliedCoating - optional, skipping
      // CRRCInitialRefl - optional, skipping
      // CRRCAgedRefl - optional, skipping
      // CRRCInitialEmittance - optional, skipping
      // CRRCAgedEmittance - optional, skipping
      // CRRCInitialSRI - optional, skipping
      // CRRCAgedSRI - optional, skipping
      // CRRCProdID - optional, skipping

      boost::optional<std::string> compatibleSurfType;
      boost::optional<std::string> intendedSurfaceType = info.intendedSurfaceType();
      if (intendedSurfaceType) {
        if (istringEqual("ExteriorWall", *intendedSurfaceType)) {
          compatibleSurfType = "ExteriorWall";
        } else if (istringEqual("AtticRoof", *intendedSurfaceType) || istringEqual("ExteriorRoof", *intendedSurfaceType)) {
          compatibleSurfType = "Roof";
        } else if (istringEqual("ExteriorFloor", *intendedSurfaceType)) {
          compatibleSurfType = "ExteriorFloor";
        } else if (istringEqual("GroundContactWall", *intendedSurfaceType)) {
          compatibleSurfType = "UndergroundWall";
        } else if (istringEqual("GroundContactFloor", *intendedSurfaceType)) {
          compatibleSurfType = "UndergroundFloor";
        } else if (istringEqual("InteriorWall", *intendedSurfaceType)) {
          compatibleSurfType = "InteriorWall";
        } else if (istringEqual("InteriorCeiling", *intendedSurfaceType)) {
          compatibleSurfType = "Ceiling";
        } else if (istringEqual("InteriorFloor", *intendedSurfaceType)) {
          compatibleSurfType = "InteriorFloor";
        } else {
          // DLM: warn?
        }
      }
      if (compatibleSurfType) {
        pugi::xml_node compatibleSurfTypeElement = result->append_child("CompatibleSurfType");
        compatibleSurfTypeElement.text() = (*compatibleSurfType).c_str();
      } else {
        // DLM: warn?
      }

      unsigned n = (unsigned)layers.size();
      if (n > 0) {

        std::string extRoughness;
        boost::optional<double> extSolarAbsorptance;
        boost::optional<double> extThermalAbsorptance;
        boost::optional<double> extVisibleAbsorptance;
        boost::optional<double> intSolarAbsorptance;
        boost::optional<double> intThermalAbsorptance;
        boost::optional<double> intVisibleAbsorptance;

        if (layers[0].optionalCast<model::StandardOpaqueMaterial>()) {
          model::StandardOpaqueMaterial extMaterial = layers[0].cast<model::StandardOpaqueMaterial>();
          extRoughness = extMaterial.roughness();
          extSolarAbsorptance = extMaterial.solarAbsorptance();
          extThermalAbsorptance = extMaterial.thermalAbsorptance();
          extVisibleAbsorptance = extMaterial.visibleAbsorptance();
        } else if (layers[0].optionalCast<model::MasslessOpaqueMaterial>()) {
          model::MasslessOpaqueMaterial extMaterial = layers[0].cast<model::MasslessOpaqueMaterial>();
          extRoughness = extMaterial.roughness();
          extSolarAbsorptance = extMaterial.solarAbsorptance();
          extThermalAbsorptance = extMaterial.thermalAbsorptance();
          extVisibleAbsorptance = extMaterial.visibleAbsorptance();
        }

        if (layers[n - 1].optionalCast<model::StandardOpaqueMaterial>()) {
          model::StandardOpaqueMaterial intMaterial = layers[n - 1].cast<model::StandardOpaqueMaterial>();
          intSolarAbsorptance = intMaterial.solarAbsorptance();
          intThermalAbsorptance = intMaterial.thermalAbsorptance();
          intVisibleAbsorptance = intMaterial.visibleAbsorptance();
        } else if (layers[n - 1].optionalCast<model::MasslessOpaqueMaterial>()) {
          model::MasslessOpaqueMaterial intMaterial = layers[n - 1].cast<model::MasslessOpaqueMaterial>();
          intSolarAbsorptance = intMaterial.solarAbsorptance();
          intThermalAbsorptance = intMaterial.thermalAbsorptance();
          intVisibleAbsorptance = intMaterial.visibleAbsorptance();
        }

        // DLM: Not input
        // ext roughness
        //if (!extRoughness.empty()) {
        //  pugi::xml_node roughnessElement = result->append_child("ExtRoughness");
        //  roughnessElement.text() = openstudio::string_conversions::number(*extRoughness).c_str()
        //}

        // DLM: Not input
        // ext solarAbsorptance
        //if (extSolarAbsorptance) {
        //  pugi::xml_node solarAbsorptanceElement = result->append_child("ExtSolAbs");
        //  solarAbsorptanceElement.text() = openstudio::string_conversions::number(*extSolarAbsorptance).c_str();
        //}

        // DLM: Not input
        // ext thermalAbsorptance
        //if (extThermalAbsorptance) {
        //  pugi::xml_node thermalAbsorptanceElement = result->append_child("ExtThrmlAbs");
        //  thermalAbsorptanceElement.text() = openstudio::string_conversions::number(*extThermalAbsorptance).c_str();
        //}

        // DLM: Not input
        // ext visibleAbsorptance
        //if (extVisibleAbsorptance) {
        //  pugi::xml_node visibleAbsorptanceElement = result->append_child("ExtVisAbs");
        //  visibleAbsorptanceElement.text() = openstudio::string_conversions::number(*extVisibleAbsorptance).c_str();
        //}

        // DLM: Not input
        // int solarAbsorptance
        //if (intSolarAbsorptance) {
        //  pugi::xml_node solarAbsorptanceElement = result->append_child("IntSolAbs");
        //  solarAbsorptanceElement.text() = openstudio::string_conversions::number(*intSolarAbsorptance).c_str();
        //}

        // DLM: Not input
        // int thermalAbsorptance
        //if (intThermalAbsorptance) {
        //  pugi::xml_node thermalAbsorptanceElement = result->append_child("IntThrmlAbs");
        //  thermalAbsorptanceElement.text() = openstudio::string_conversions::number(*intThermalAbsorptance).c_str();
        //}

        // DLM: Not input
        // int visibleAbsorptance
        //if (intVisibleAbsorptance) {
        //  pugi::xml_node visibleAbsorptanceElement = result->append_child("IntVisAbs");
        //  visibleAbsorptanceElement.text() = openstudio::string_conversions::number(*intVisibleAbsorptance).c_str();
        //}
      }

      for (const model::Material& material : layers) {

        // materialReference
        std::string materialName = material.name().get();
        pugi::xml_node materialReferenceElement = result->append_child("MatRef");
        materialReferenceElement.text() = escapeName(materialName).c_str();
      }

      m_translatedObjects[construction.handle()] = *result;

    } else if (constructionBase.optionalCast<model::FFactorGroundFloorConstruction>()) {
      // DLM: I think this is out of date
      model::FFactorGroundFloorConstruction construction = constructionBase.cast<model::FFactorGroundFloorConstruction>();

      result = root.append_child("ConsAssm");

      // name
      std::string name = construction.name().get();
      pugi::xml_node nameElement = result->append_child("Name");
      nameElement.text() = escapeName(name).c_str();

      // DLM: prescribed, not input
      // specification method
      //pugi::xml_node specMthdElement = result->append_child("SpecMthd");
      //specMthdElement.text() = "FFactor";

      // DLM: not input
      // specification method
      //pugi::xml_node fFactorElement = result->append_child("FFactor");
      //// sdd units = Btu/(hr*ft*F), os units = W/(m*K)
      //Quantity fFactorSI(construction.fFactor(), WhUnit(WhExpnt(1,0,-1,-1)));
      //OptionalQuantity fFactorIP = QuantityConverter::instance().convert(fFactorSI, UnitSystem(UnitSystem::BTU));
      //OS_ASSERT(fFactorIP);
      //OS_ASSERT(fFactorIP->units() == BTUUnit(BTUExpnt(1,-1,-1,-1)));
      //fFactorElement.text() = openstudio::string_conversions::number(fFactorIP->value()).c_str();

      // DLM: cannot write out
      //<IntSolAbs>0.7</IntSolAbs>
      //<IntThrmlAbs>0.9</IntThrmlAbs>
      //<IntVisAbs>0.5</IntVisAbs>
      //<MatRef index="0">NACM_Concrete 4in</MatRef>
      //<MatRef index="1">NACM_Carpet Pad</MatRef>

      m_translatedObjects[construction.handle()] = *result;

    } else if (constructionBase.optionalCast<model::CFactorUndergroundWallConstruction>()) {
      // DLM: I think this is out of date
      model::CFactorUndergroundWallConstruction construction = constructionBase.cast<model::CFactorUndergroundWallConstruction>();

      result = root.append_child("ConsAssm");

      // name
      std::string name = construction.name().get();
      pugi::xml_node nameElement = result->append_child("Name");
      nameElement.text() = escapeName(name).c_str();

      // DLM: prescribed, not input
      // specification method
      //pugi::xml_node specMthdElement = result->append_child("SpecMthd");
      //specMthdElement.text() = "CFactor";

      // DLM: not input
      // specification method
      //pugi::xml_node cFactorElement = result->append_child("CFactor");
      //// sdd units = Btu/(hr*ft^2*F), os units = W/(m^2*K)
      //Quantity cFactorSI(construction.cFactor(), WhUnit(WhExpnt(1,0,-2,-1)));
      //OptionalQuantity cFactorIP = QuantityConverter::instance().convert(cFactorSI, UnitSystem(UnitSystem::BTU));
      //OS_ASSERT(cFactorIP);
      //OS_ASSERT(cFactorIP->units() == BTUUnit(BTUExpnt(1,-2,-1,-1)));
      //cFactorElement.text() = openstudio::string_conversions::number(cFactorIP->value()).c_str();

      // DLM: cannot write out
      //<IntSolAbs>0.7</IntSolAbs>
      //<IntThrmlAbs>0.9</IntThrmlAbs>
      //<IntVisAbs>0.5</IntVisAbs>
      //<MatRef index="0">NACM_Concrete 4in</MatRef>
      //<MatRef index="1">NACM_Carpet Pad</MatRef>

      m_translatedObjects[construction.handle()] = *result;
    }

    // TODO: handle u-factor constructions

    return result;
  }

  boost::optional<pugi::xml_node> ForwardTranslator::translateDoorConstruction(const openstudio::model::ConstructionBase& constructionBase,
                                                                               pugi::xml_node& root) {
    boost::optional<pugi::xml_node> result;

    UnitSystem ipSys(UnitSystem::IP);
    UnitSystem btuSys(UnitSystem::BTU);

    if (constructionBase.optionalCast<model::Construction>()) {
      model::Construction construction = constructionBase.cast<model::Construction>();
      model::StandardsInformationConstruction info = constructionBase.standardsInformation();

      result = root.append_child("DrCons");

      // name
      std::string name = construction.name().get();
      pugi::xml_node nameElement = result->append_child("Name");
      nameElement.text() = escapeName(name).c_str();

      // SDD:
      // Type - required, skipping
      // CertificationMthd - required, defaulted to NFRC unless missing performance data
      // UFactor - optional, done
      // Open - optional, done

      boost::optional<std::string> type;
      boost::optional<std::string> certificationMthd = std::string("NFRCRated");
      boost::optional<double> uFactor;
      boost::optional<std::string> open;

      // can only get UFactor for special cases
      std::vector<model::Material> layers = construction.layers();
      if (layers.size() == 1) {
        if (layers[0].optionalCast<model::MasslessOpaqueMaterial>()) {

          model::MasslessOpaqueMaterial material = layers[0].cast<model::MasslessOpaqueMaterial>();

          // os units = W/m2-K, sdd units = Btu/(hr*f2t*F)
          Quantity uFactorSI(material.thermalConductance(), WhUnit(WhExpnt(1, 0, -2, -1)));
          OptionalQuantity uFactorIP = QuantityConverter::instance().convert(uFactorSI, btuSys);
          OS_ASSERT(uFactorIP);
          OS_ASSERT(uFactorIP->units() == BTUUnit(BTUExpnt(1, -2, -1, -1)));
          uFactor = uFactorIP->value();
        }
      }

      if (!uFactor) {
        certificationMthd.reset();
      }

      boost::optional<std::string> fenestrationType = info.fenestrationType();
      if (fenestrationType) {
        if (istringEqual("Swinging Door", *fenestrationType)) {
          open = "Swinging";
        } else if (istringEqual("Non-Swinging Door", *fenestrationType)) {
          open = "NonSwinging";
        }
      }

      // write the xml
      if (type) {
        pugi::xml_node typeElement = result->append_child("Type");
        typeElement.text() = type.get().c_str();
      }
      if (certificationMthd) {
        pugi::xml_node certificationMthdElement = result->append_child("CertificationMthd");
        certificationMthdElement.text() = certificationMthd.get().c_str();
      }
      if (uFactor) {
        pugi::xml_node uFactorElement = result->append_child("UFactor");
        uFactorElement.text() = openstudio::string_conversions::number(*uFactor).c_str();
      } else {
        //Do not want this logged, http://code.google.com/p/cbecc/issues/detail?id=695
        //LOG(Error, "Could not calculate UFactor for DrCons '" << name << "'");
      }
      if (open) {
        pugi::xml_node openElement = result->append_child("Open");
        openElement.text() = open.get().c_str();
      }

      // mark the construction as translated, not the material
      m_translatedObjects[construction.handle()] = result.get();
    }

    return result;
  }

  boost::optional<pugi::xml_node> ForwardTranslator::translateFenestrationConstruction(const openstudio::model::ConstructionBase& constructionBase,
                                                                                       pugi::xml_node& root) {
    boost::optional<pugi::xml_node> result;

    UnitSystem ipSys(UnitSystem::IP);
    UnitSystem btuSys(UnitSystem::BTU);

    if (constructionBase.optionalCast<model::Construction>()) {
      model::Construction construction = constructionBase.cast<model::Construction>();
      model::StandardsInformationConstruction info = constructionBase.standardsInformation();

      result = root.append_child("FenCons");

      // name
      std::string name = construction.name().get();
      pugi::xml_node nameElement = result->append_child("Name");
      nameElement.text() = escapeName(name).c_str();

      // SDD:
      // FenType - required, done
      // FenProdType - optional, done
      // AssmContext - required, done
      // CertificationMthd - required, defaulted to NFRCRated unless field fabricated or missing performance data, might be incorrect?
      // SkyltGlz - optional, done
      // SkyltCurb - optional, done
      // OperableWinConfiguration - optional, skipping
      // GreenhouseGardenWin - optional, skipping
      // FenFrm - optional, done
      // FenPanes - optional, done
      // GlzTint - optional, done
      // WinDivider - optional, done
      // Diffusing - optional, skipping
      // SHGC - optional, done
      // SHGCCOG - optional, not supported
      // UFactor - optional, done
      // UFactorCOG - optional, not supported
      // VT - optional, done
      // VTCOG - optional, not supported

      boost::optional<std::string> fenestrationType = info.fenestrationType();
      boost::optional<std::string> fenType;
      boost::optional<std::string> fenProdType;
      boost::optional<std::string> skyltGlz;
      boost::optional<std::string> skyltCurb;
      boost::optional<std::string> operableWinConfiguration;
      boost::optional<std::string> glzTint;
      if (fenestrationType) {
        if (istringEqual("Glass Skylight with Curb", *fenestrationType)) {
          fenType = "Skylight";
          skyltGlz = "Glass";
          skyltCurb = "CurbMounted";
        } else if (istringEqual("Plastic Skylight with Curb", *fenestrationType)) {
          fenType = "Skylight";
          skyltGlz = "Plastic";
          skyltCurb = "CurbMounted";
        } else if (istringEqual("Glass Skylight without Curb", *fenestrationType)) {
          fenType = "Skylight";
          skyltGlz = "Glass";
          skyltCurb = "DeckMounted";
        } else if (istringEqual("Plastic Skylight without Curb", *fenestrationType)) {
          fenType = "Skylight";
          skyltGlz = "Plastic";
          skyltCurb = "DeckMounted";
        } else {
          fenType = "VerticalFenestration";

          if (istringEqual("Fixed Window", *fenestrationType)) {
            fenProdType = "FixedWindow";
          } else if (istringEqual("Operable Window", *fenestrationType)) {
            fenProdType = "OperableWindow";

            // DLM: todo set operableWinConfiguration
            //operableWinConfiguration = "CasementAwning" or "Sliding"
          } else if (istringEqual("Curtain Wall", *fenestrationType)) {
            fenProdType = "CurtainWall";
          } else if (istringEqual("Glazed Door", *fenestrationType)) {
            fenProdType = "GlazedDoor";
          } else {
            // 'Swinging Door' or 'Non-Swinging Door'
            // These should have been grouped with door constructions, potentially warn
          }

          boost::optional<std::string> fenestrationTint = info.fenestrationTint();
          if (fenestrationTint) {
            if (istringEqual("Clear", *fenestrationTint)) {
              glzTint = "ClearGlazing";
            } else {
              glzTint = "TintedGlazing";
            }
          }
        }
      }

      boost::optional<std::string> assmContext;
      boost::optional<std::string> certificationMthd = std::string("NFRCRated");  // default
      boost::optional<std::string> fenestrationAssemblyContext = info.fenestrationAssemblyContext();
      if (fenestrationAssemblyContext) {
        if (istringEqual("Manufactured", *fenestrationAssemblyContext)) {
          assmContext = "Manufactured";
        } else if (istringEqual("Field Fabricated", *fenestrationAssemblyContext)) {
          assmContext = "FieldFabricated";
          certificationMthd = "CECDefaultPerformance";  // right?
        } else if (istringEqual("Site Built", *fenestrationAssemblyContext)) {
          assmContext = "SiteBuilt";
        }
      }

      boost::optional<std::string> greenhouseGardenWin;

      boost::optional<std::string> fenFrm;
      boost::optional<std::string> fenestrationFrameType = info.fenestrationFrameType();
      if (fenestrationFrameType) {
        if (istringEqual("Metal Framing", *fenestrationFrameType)) {
          fenFrm = "MetalFraming";
        } else if (istringEqual("Metal Framing with Thermal Break", *fenestrationFrameType)) {
          fenFrm = "MetalFramingWithThermalBreak";
        } else if (istringEqual("Non-Metal Framing", *fenestrationFrameType)) {
          fenFrm = "NonMetalFraming";
        }
      }

      boost::optional<std::string> fenPanes;
      boost::optional<std::string> fenestrationNumberOfPanes = info.fenestrationNumberOfPanes();
      if (fenestrationNumberOfPanes) {
        if (istringEqual("Single Pane", *fenestrationNumberOfPanes)) {
          fenPanes = "SinglePane";
        } else if (istringEqual("Double Pane", *fenestrationNumberOfPanes)) {
          fenPanes = "DoublePane";
        } else if (istringEqual("Triple Pane", *fenestrationNumberOfPanes)) {
          fenPanes = "DoublePane";
        } else if (istringEqual("Quadruple Pane", *fenestrationNumberOfPanes)) {
          fenPanes = "DoublePane";
        } else if (istringEqual("Glass Block", *fenestrationNumberOfPanes)) {
          fenPanes = "GlassBlock";
        }
      }

      boost::optional<std::string> winDivider;
      boost::optional<std::string> fenestrationDividerType = info.fenestrationDividerType();
      if (fenestrationDividerType) {
        if (istringEqual("True Divided Lite", *fenestrationDividerType)) {
          winDivider = "TrueDividedLite";
        } else if (istringEqual("Between Panes < 7/16\"", *fenestrationDividerType)) {
          winDivider = "DividerBtwnPanesLessThan7_16in";
        } else if (istringEqual("Between Panes >= 7/16\"", *fenestrationDividerType)) {
          winDivider = "DividerBtwnPanesGreaterThanOrEqualTo7_16in";
        }
      }

      boost::optional<std::string> diffusing;

      boost::optional<double> shgc;
      boost::optional<double> uFactor;
      boost::optional<double> vt;

      // can only get SHGC, TVis, UFactor for special cases
      std::vector<model::Material> layers = construction.layers();
      if (layers.size() == 1) {
        if (layers[0].optionalCast<model::SimpleGlazing>()) {

          model::SimpleGlazing simpleGlazing = layers[0].cast<model::SimpleGlazing>();

          shgc = simpleGlazing.solarHeatGainCoefficient();

          // os units = W/m2-K, sdd units = Btu/(hr*f2t*F)
          Quantity uFactorSI(simpleGlazing.uFactor(), WhUnit(WhExpnt(1, 0, -2, -1)));
          OptionalQuantity uFactorIP = QuantityConverter::instance().convert(uFactorSI, btuSys);
          OS_ASSERT(uFactorIP);
          OS_ASSERT(uFactorIP->units() == BTUUnit(BTUExpnt(1, -2, -1, -1)));
          uFactor = uFactorIP->value();

          vt = simpleGlazing.visibleTransmittance();
        }
      }

      // if performance is not known use defaults if possible
      if (!shgc || !uFactor || !vt) {
        if (fenFrm && fenPanes) {
          certificationMthd = "CECDefaultPerformance";
        }
      }

      // write to XML
      if (fenType) {
        pugi::xml_node fenTypeElement = result->append_child("FenType");
        fenTypeElement.text() = fenType.get().c_str();
      }
      if (fenProdType) {
        pugi::xml_node fenProdTypeElement = result->append_child("FenProdType");
        fenProdTypeElement.text() = fenProdType.get().c_str();
      }
      if (assmContext) {
        pugi::xml_node assmContextElement = result->append_child("AssmContext");
        assmContextElement.text() = assmContext.get().c_str();
      }
      if (certificationMthd) {
        pugi::xml_node certificationMthdElement = result->append_child("CertificationMthd");
        certificationMthdElement.text() = certificationMthd.get().c_str();
      }
      if (skyltGlz) {
        pugi::xml_node skyltGlzElement = result->append_child("SkyltGlz");
        skyltGlzElement.text() = skyltGlz.get().c_str();
      }
      if (skyltCurb) {
        pugi::xml_node skyltCurbElement = result->append_child("SkyltCurb");
        skyltCurbElement.text() = skyltCurb.get().c_str();
      }
      if (operableWinConfiguration) {
        pugi::xml_node operableWinConfigurationElement = result->append_child("OperableWinConfiguration");
        operableWinConfigurationElement.text() = operableWinConfiguration.get().c_str();
      }
      if (greenhouseGardenWin) {
        pugi::xml_node greenhouseGardenWinElement = result->append_child("GreenhouseGardenWin");
        greenhouseGardenWinElement.text() = greenhouseGardenWin.get().c_str();
      }
      if (fenFrm) {
        pugi::xml_node fenFrmElement = result->append_child("FenFrm");
        fenFrmElement.text() = fenFrm.get().c_str();
      }
      if (fenPanes) {
        pugi::xml_node fenPanesElement = result->append_child("FenPanes");
        fenPanesElement.text() = fenPanes.get().c_str();
      }
      if (glzTint) {
        pugi::xml_node glzTintElement = result->append_child("GlzTint");
        glzTintElement.text() = glzTint.get().c_str();
      }
      if (winDivider) {
        pugi::xml_node winDividerElement = result->append_child("WinDivider");
        winDividerElement.text() = glzTint.get().c_str();
      }
      if (diffusing) {
        pugi::xml_node diffusingElement = result->append_child("Diffusing");
        diffusingElement.text() = diffusing.get().c_str();
      }

      // Numbers
      if (shgc) {
        pugi::xml_node shgcElement = result->append_child("SHGC");
        shgcElement.text() = openstudio::string_conversions::number(*shgc).c_str();
      } else {
        //Do not want this logged, http://code.google.com/p/cbecc/issues/detail?id=695
        //LOG(Error, "Could not calculate SHGC for FenCons '" << name << "'");
      }
      if (uFactor) {
        pugi::xml_node uFactorElement = result->append_child("UFactor");
        uFactorElement.text() = openstudio::string_conversions::number(*uFactor).c_str();
      } else {
        //Do not want this logged, http://code.google.com/p/cbecc/issues/detail?id=695
        //LOG(Error, "Could not calculate UFactor for FenCons '" << name << "'");
      }
      if (vt) {
        pugi::xml_node vtElement = result->append_child("VT");
        vtElement.text() = openstudio::string_conversions::number(*vt).c_str();
      } else {
        //Do not want this logged, http://code.google.com/p/cbecc/issues/detail?id=695
        //LOG(Error, "Could not calculate VT for FenCons '" << name << "'");
      }

      // mark the construction as translated, not the material
      m_translatedObjects[construction.handle()] = result.get();
    }

    return result;
  }

  boost::optional<pugi::xml_node> ForwardTranslator::translateMaterial(const openstudio::model::Material& material, pugi::xml_node& root) {

    if (!(material.optionalCast<model::StandardOpaqueMaterial>() || material.optionalCast<model::MasslessOpaqueMaterial>()
          || material.optionalCast<model::AirGap>())) {
      return boost::none;
    }

    model::StandardsInformationMaterial info = material.standardsInformation();

    pugi::xml_node result = root.append_child("Mat");
    m_translatedObjects[material.handle()] = result;

    // name
    std::string name = material.name().get();
    pugi::xml_node nameElement = result.append_child("Name");
    nameElement.text() = escapeName(name).c_str();

    // SDD:
    // CodeCat - compulsory, done
    // CodeItem - compulsory, done
    // FrmMat - optional, done
    // FrmConfig - optional, done
    // FrmDepth - optional, done
    // CavityIns - optional, done
    // CavityInsOpt - optional, done
    // CompositeMatNotes - optional, skipping
    // HeaderIns - optional, not used
    // CMUWt - optional, not used
    // CMUFill - optional, not used
    // SpandrelPanelIns - optional, not used
    // ICCESRptNum - optional, skipping
    // InsOutsdWtrprfMemb - optional, skipping

    boost::optional<std::string> standardsCategory = info.standardsCategory();
    if (standardsCategory && info.isCompositeMaterial()) {
      standardsCategory = "Composite";
    }

    boost::optional<std::string> standardsIdentifier = info.standardsIdentifier();
    boost::optional<std::string> compositeFramingMaterial = info.compositeFramingMaterial();
    boost::optional<std::string> compositeFramingConfiguration = info.compositeFramingConfiguration();
    boost::optional<std::string> compositeFramingDepth = info.compositeFramingDepth();
    boost::optional<std::string> compositeFramingSize = info.compositeFramingSize();
    boost::optional<std::string> compositeCavityInsulation = info.compositeCavityInsulation();

    if (standardsCategory) {
      pugi::xml_node element = result.append_child("CodeCat");
      element.text() = standardsCategory.get().c_str();
    }

    if (standardsIdentifier) {
      pugi::xml_node element = result.append_child("CodeItem");
      element.text() = standardsIdentifier.get().c_str();
    }

    if (compositeFramingMaterial) {
      pugi::xml_node element = result.append_child("FrmMat");
      element.text() = compositeFramingMaterial.get().c_str();
    }

    if (compositeFramingConfiguration) {
      pugi::xml_node element = result.append_child("FrmConfig");
      element.text() = compositeFramingConfiguration.get().c_str();
    }

    if (compositeFramingDepth) {
      pugi::xml_node element = result.append_child("FrmDepth");
      element.text() = compositeFramingDepth.get().c_str();
    }

    if (compositeCavityInsulation) {

      // DLM: this is a stupid switch, this should get fixed in CBECC rules
      if (compositeFramingMaterial && istringEqual("Metal", *compositeFramingMaterial)) {
        std::string value = compositeCavityInsulation.get();
        try {
          double valueNum = boost::lexical_cast<double>(value);
          value = "R-" + std::to_string(valueNum);
        } catch (const boost::bad_lexical_cast&) {
        }
        pugi::xml_node element = result.append_child("CavityInsOpt");
        element.text() = value.c_str();

      } else {
        pugi::xml_node element = result.append_child("CavityIns");
        element.text() = compositeCavityInsulation.get().c_str();
      }
    }

    return result;
  }

}  // namespace sdd
}  // namespace openstudio
