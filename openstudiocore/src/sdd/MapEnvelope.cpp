/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

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
#include "../utilities/units/IPUnit.hpp"
#include "../utilities/units/SIUnit.hpp"
#include "../utilities/units/BTUUnit.hpp"
#include "../utilities/units/WhUnit.hpp"
#include "../utilities/core/Assert.hpp"

#include <QFile>
#include <QDomDocument>
#include <QDomElement>

namespace openstudio {
namespace sdd {

  boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateConstructAssembly(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model)
  {
    boost::optional<openstudio::model::ModelObject> result;

    QDomElement nameElement = element.firstChildElement("Name");
    std::string name;
    if (nameElement.isNull()){
      LOG(Error, "ConsAssm element 'Name' is empty.")
    } else{
      name = escapeName(nameElement.text());
    }

    QDomElement specificationElement = element.firstChildElement("SpecMthd");
    if (specificationElement.isNull()){
      LOG(Error, "ConsAssm element 'SpecMthd' is empty for construction named '" << name << "'. Construction will not be created");
      return boost::none;
    }

    if (specificationElement.text() == "Layers"){

      openstudio::model::Construction construction(model);
      construction.setName(name);

      std::vector<model::Material> materials;
      QDomNodeList materialElements = element.elementsByTagName("MatRef");
      for (int i = 0; i < materialElements.count(); i++){
        QDomElement materialElement = materialElements.at(i).toElement();
        std::string materialName = escapeName(materialElement.text());
        boost::optional<model::Material> material = model.getModelObjectByName<model::Material>(materialName);
        if( ! material )
        {
          LOG(Error,"Construction: " << construction.name().get() << " references material: " << materialName << " that is not defined.");
        
          // DLM: what to do?  Remove the construction?
        } else{
          materials.push_back(*material);
        }
      }

      bool test = construction.setLayers(materials);
      if (!test){
        LOG(Error, "Failed to set material layers for Construction: " << construction.name().get() << ".");

        // DLM: what to do?  Remove the construction?
      }

      bool wasFastNaming = model.fastNaming();
      model.setFastNaming(false);

      unsigned n = (unsigned) materials.size();
      construction.ensureUniqueLayers();
      materials = construction.layers(); // DLM: get materials again in case new ones were cloned

      OS_ASSERT(n == materials.size());
 
      model.setFastNaming(wasFastNaming);


      if (n > 0){

        // DLM: are these fields only on layered constructions? 
        QDomElement extRoughnessElement = element.firstChildElement("ExtRoughness"); // enumerated value
        QDomElement extSolAbsElement = element.firstChildElement("ExtSolAbs"); 
        QDomElement extThrmlAbsElement = element.firstChildElement("ExtThrmlAbs"); 
        QDomElement extVisAbsElement = element.firstChildElement("ExtVisAbs"); 
        QDomElement intSolAbsElement = element.firstChildElement("IntSolAbs"); 
        QDomElement intThrmlAbsElement = element.firstChildElement("IntThrmlAbs"); 
        QDomElement intVisAbsElement = element.firstChildElement("IntVisAbs"); 

        if (materials[0].optionalCast<model::StandardOpaqueMaterial>()){
          model::StandardOpaqueMaterial extMaterial = materials[0].cast<model::StandardOpaqueMaterial>();
          
          std::string roughness = "MediumRough";
          if (!extRoughnessElement.isNull()){
            roughness = extRoughnessElement.text().toStdString();
          }
          extMaterial.setRoughness(roughness);
      
          if (!extThrmlAbsElement.isNull()){
            extMaterial.setThermalAbsorptance(extThrmlAbsElement.text().toDouble());
          }

          if (!extSolAbsElement.isNull()){
            extMaterial.setSolarAbsorptance(extSolAbsElement.text().toDouble());
          }
            
          if (!extVisAbsElement.isNull()){
            extMaterial.setVisibleAbsorptance(extVisAbsElement.text().toDouble());
          }
        }else if (materials[0].optionalCast<model::MasslessOpaqueMaterial>()){
          model::MasslessOpaqueMaterial extMaterial = materials[0].cast<model::MasslessOpaqueMaterial>();
          
          std::string roughness = "MediumRough";
          if (!extRoughnessElement.isNull()){
            roughness = extRoughnessElement.text().toStdString();
          }
          extMaterial.setRoughness(roughness);
      
          if (!extThrmlAbsElement.isNull()){
            extMaterial.setThermalAbsorptance(extThrmlAbsElement.text().toDouble());
          }

          if (!extSolAbsElement.isNull()){
            extMaterial.setSolarAbsorptance(extSolAbsElement.text().toDouble());
          }
            
          if (!extVisAbsElement.isNull()){
            extMaterial.setVisibleAbsorptance(extVisAbsElement.text().toDouble());
          }
        }

        // DLM: if only one layer use properties from exterior material
        if (n > 1){
          if (materials[n - 1].optionalCast<model::StandardOpaqueMaterial>()){
            model::StandardOpaqueMaterial intMaterial = materials[n - 1].cast<model::StandardOpaqueMaterial>();

            if (!intThrmlAbsElement.isNull()){
              intMaterial.setThermalAbsorptance(intThrmlAbsElement.text().toDouble());
            }

            if (!intSolAbsElement.isNull()){
              intMaterial.setSolarAbsorptance(intSolAbsElement.text().toDouble());
            }

            if (!intVisAbsElement.isNull()){
              intMaterial.setVisibleAbsorptance(intVisAbsElement.text().toDouble());
            }
          } else if (materials[n - 1].optionalCast<model::MasslessOpaqueMaterial>()){
            model::MasslessOpaqueMaterial intMaterial = materials[n - 1].cast<model::MasslessOpaqueMaterial>();

            if (!intThrmlAbsElement.isNull()){
              intMaterial.setThermalAbsorptance(intThrmlAbsElement.text().toDouble());
            }

            if (!intSolAbsElement.isNull()){
              intMaterial.setSolarAbsorptance(intSolAbsElement.text().toDouble());
            }

            if (!intVisAbsElement.isNull()){
              intMaterial.setVisibleAbsorptance(intVisAbsElement.text().toDouble());
            }
          }
        }
      }

      result = construction;

    }else if (specificationElement.text() == "CFactor"){
  
      // DLM: per input from David Reddy this construction will be cloned 
      // for each surface that uses it and height set per surface
      // 

      QDomElement cFactorElement = element.firstChildElement("CFactor");
      if (cFactorElement.isNull()){
        LOG(Error, "ConsAssm required element 'CFactor' is empty for construction named '" << name << "'. Construction will not be created");
        return boost::none;
      }

      openstudio::model::CFactorUndergroundWallConstruction construction(model);
      construction.setName(name);

      // sdd units = Btu/(hr*ft^2*F), os units = W/(m^2*K) 
      Quantity cFactorIP(cFactorElement.text().toDouble(), BTUUnit(BTUExpnt(1,-2,-1,-1)));
      OptionalQuantity cFactorSI = QuantityConverter::instance().convert(cFactorIP, UnitSystem(UnitSystem::Wh));
      OS_ASSERT(cFactorSI);
      OS_ASSERT(cFactorSI->units() == WhUnit(WhExpnt(1,0,-2,-1)));
      construction.setCFactor(cFactorSI->value());

      result = construction;

    }else if (specificationElement.text() == "FFactor"){

      // DLM: per input from David Reddy this construction will be cloned 
      // for each surface that uses it and area and perimeter set per surface

      QDomElement fFactorElement = element.firstChildElement("FFactor");
      if (fFactorElement.isNull()){
        LOG(Error, "ConsAssm required element 'FFactor' is empty for construction named '" << name << "'. Construction will not be created");
        return boost::none;
      }

      openstudio::model::FFactorGroundFloorConstruction construction(model);
      construction.setName(name);
            
      // sdd units = Btu/(hr*ft*F), os units = W/(m*K)
      Quantity fFactorIP(fFactorElement.text().toDouble(), BTUUnit(BTUExpnt(1,-1,-1,-1)));
      OptionalQuantity fFactorSI = QuantityConverter::instance().convert(fFactorIP, UnitSystem(UnitSystem::Wh));
      OS_ASSERT(fFactorSI);
      OS_ASSERT(fFactorSI->units() == WhUnit(WhExpnt(1,0,-1,-1)));
      construction.setFFactor(fFactorSI->value());

      result = construction;

    }else if (specificationElement.text() == "UFactor"){

      // DLM: todo create object, do we need this now?

    }

    return result;
  }

  boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateDoorConstruction(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model)
  {
    //SIExpnt(int kg=0,int m=0,int s=0,int K=0,..)
    //IPExpnt(int lbm=0,int ft=0,int s=0,int R=0,..)
    //BTUExpnt(int btu=0,int ft=0,int h=0,int R=0,..)
    //WhExpnt(int W=0,int h=0,int m=0,int K=0,..)

    UnitSystem siSys(UnitSystem::SI);
    UnitSystem whSys(UnitSystem::Wh);

    QDomElement nameElement = element.firstChildElement("Name");
    QDomElement uFactorElement = element.firstChildElement("UFactorSim"); // Btu/h-ft2-F

    if (nameElement.isNull()){
      LOG(Error, "DrCons element 'Name' is empty.  Construction will not be created.");
    }
    if (uFactorElement.isNull()){
      LOG(Error, "DrCons element 'UFactorSim' is empty.  Construction will not be created.");
    }

    if (!nameElement.isNull() && !uFactorElement.isNull()){
      
      // sdd units = Btu/(hr*ft^2*F), os units = W/m^2-K
      Quantity uFactorIP(uFactorElement.text().toDouble(), BTUUnit(BTUExpnt(1,-2,-1,-1)));
      OptionalQuantity uFactorWh = QuantityConverter::instance().convert(uFactorIP, whSys);
      OS_ASSERT(uFactorWh);
      OS_ASSERT(uFactorWh->units() == WhUnit(WhExpnt(1,0,-2,-1)));

      // m^2-K/W
      double rValueSI = 1.0 / uFactorWh->value();

      std::string name = escapeName(nameElement.text());

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

  boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateFenestrationConstruction(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model)
  {
    //SIExpnt(int kg=0,int m=0,int s=0,int K=0,..)
    //IPExpnt(int lbm=0,int ft=0,int s=0,int R=0,..)
    //BTUExpnt(int btu=0,int ft=0,int h=0,int R=0,..)
    //WhExpnt(int W=0,int h=0,int m=0,int K=0,..)

    UnitSystem siSys(UnitSystem::SI);
    UnitSystem whSys(UnitSystem::Wh);

    QDomElement nameElement = element.firstChildElement("Name");
    QDomElement shgcElement = element.firstChildElement("SHGC"); // unitless
    QDomElement uFactorElement = element.firstChildElement("UFactorSim"); // Btu/h-ft2-F
    QDomElement vtElement = element.firstChildElement("VT"); // unitless

    if (nameElement.isNull()){
      LOG(Error, "FenCons element 'Name' is empty.  Construction will not be created.");
    }
    if (shgcElement.isNull()){
      LOG(Error, "FenCons element 'SHGC' is empty.  Construction will not be created.");
    }
    if (uFactorElement.isNull()){
      LOG(Error, "FenCons element 'UFactorSim' is empty.  Construction will not be created.");
    }
    if (vtElement.isNull()){
      LOG(Error, "FenCons element 'VT' is empty.  Construction will not be created.");
    }

    if (!nameElement.isNull() && !shgcElement.isNull() && !uFactorElement.isNull() && !vtElement.isNull()){
      
      std::string name = escapeName(nameElement.text());

      openstudio::model::SimpleGlazing material(model);
      material.setName(name + " Material");
      material.setSolarHeatGainCoefficient(shgcElement.text().toDouble());
      material.setVisibleTransmittance(vtElement.text().toDouble());

      // sdd units = Btu/(hr*ft^2*F), os units = W/m^2-K
      Quantity uFactorIP(uFactorElement.text().toDouble(), BTUUnit(BTUExpnt(1,-2,-1,-1)));
      OptionalQuantity uFactorWh = QuantityConverter::instance().convert(uFactorIP, whSys);
      OS_ASSERT(uFactorWh);
      OS_ASSERT(uFactorWh->units() == WhUnit(WhExpnt(1,0,-2,-1)));
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
  
  boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateMaterial(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model)
  {
   
    //SIExpnt(int kg=0,int m=0,int s=0,int K=0,..)
    //IPExpnt(int lbm=0,int ft=0,int s=0,int R=0,..)
    //BTUExpnt(int btu=0,int ft=0,int h=0,int R=0,..)
    //WhExpnt(int W=0,int h=0,int m=0,int K=0,..)

    UnitSystem siSys(UnitSystem::SI);
    UnitSystem whSys(UnitSystem::Wh);

    QDomElement nameElement = element.firstChildElement("Name");
    //QDomElement roughnessElement = element.firstChildElement("Roughness"); // {VeryRough, Rough, MediumRough, MediumSmooth, Smooth, VerySmooth}
    QDomElement thicknessElement = element.firstChildElement("Thkns"); // in
    QDomElement thermalConductivityElement = element.firstChildElement("ThrmlCndct"); // Btu/(hr*ft*F)
    QDomElement densityElement = element.firstChildElement("Dens"); // lb/ft^3 // DLM: is this right? schema says could also be lb/ft^2
    QDomElement specificHeatElement = element.firstChildElement("SpecHt"); // Btu/(lb*F)
    //QDomElement thermalAbsorptanceElement = element.firstChildElement("ThrmlAbs"); // 0-1
    //QDomElement solarAbsorbanceElement = element.firstChildElement("SolAbs"); // 0-1 
    //QDomElement visibleAbsorptanceElement = element.firstChildElement("VisAbs"); // 0-1
    QDomElement rValueElement = element.firstChildElement("RVal"); // hr*ft2*degF/Btu

    if (nameElement.isNull()){
      LOG(Error, "Mat element 'Name' is empty.  Material will not be created.");
      return boost::none;
    }

    std::string name = escapeName(nameElement.text());

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
    if (!nameElement.isNull() && !thicknessElement.isNull() && !thermalConductivityElement.isNull() && !densityElement.isNull() && !specificHeatElement.isNull()){
      
      openstudio::model::StandardOpaqueMaterial material(model);

      material.setName(name);

      // DLM: set on construction
      //std::string roughness = "MediumRough";
      //if (!roughnessElement.isNull()){
      //  roughness = roughnessElement.text().toStdString();
      //}
      //material.setRoughness(roughness);

      // sdd units = in, os units = m
      double thicknessInches = thicknessElement.text().toDouble();
      double thicknessFeet = thicknessInches/12.0;
      Quantity thicknessIP(thicknessFeet, IPUnit(IPExpnt(0,1,0)));
      OptionalQuantity thicknessSI = QuantityConverter::instance().convert(thicknessIP, siSys);
      OS_ASSERT(thicknessSI);
      OS_ASSERT(thicknessSI->units() == SIUnit(SIExpnt(0,1,0)));
      material.setThickness(thicknessSI->value());

      // sdd units = Btu/(hr*ft*F), os units = W/m-K
      Quantity thermalConductivityIP(thermalConductivityElement.text().toDouble(), BTUUnit(BTUExpnt(1,-1,-1,-1)));
      OptionalQuantity thermalConductivityWh = QuantityConverter::instance().convert(thermalConductivityIP, whSys);
      OS_ASSERT(thermalConductivityWh);
      OS_ASSERT(thermalConductivityWh->units() == WhUnit(WhExpnt(1,0,-1,-1)));
      material.setThermalConductivity(thermalConductivityWh->value());

      // sdd units = lb/ft^3, os units = kg/m3
      Quantity densityIP(densityElement.text().toDouble(), IPUnit(IPExpnt(1,-3,0)));
      OptionalQuantity densitySI = QuantityConverter::instance().convert(densityIP, siSys);
      OS_ASSERT(densitySI);
      OS_ASSERT(densitySI->units() == SIUnit(SIExpnt(1,-3,0)));
      material.setDensity(densitySI->value());

      // sdd units = Btu/(lb*F), os units = J/kg-K = m^2/(s^2*K)
      Quantity specificHeatIP(specificHeatElement.text().toDouble(), BTUUnit(BTUExpnt(1,0,0,-1))*IPUnit(IPExpnt(-1)));
      OptionalQuantity specificHeatSI = QuantityConverter::instance().convert(specificHeatIP, siSys);
      OS_ASSERT(specificHeatSI);
      OS_ASSERT(specificHeatSI->units() == SIUnit(SIExpnt(0,2,-2,-1)));
      material.setSpecificHeat(specificHeatSI->value());

      // DLM: set on construction
      //if (!thermalAbsorptanceElement.isNull()){
      //  material.setThermalAbsorptance(thermalAbsorptanceElement.text().toDouble());
      //}

      // DLM: set on construction
      //if (!solarAbsorbanceElement.isNull()){
      //  material.setSolarAbsorptance(solarAbsorbanceElement.text().toDouble());
      //}
        
      // DLM: set on construction
      //if (!visibleAbsorptanceElement.isNull()){
      //  material.setVisibleAbsorptance(visibleAbsorptanceElement.text().toDouble());
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
    if (!nameElement.isNull() && !rValueElement.isNull()){
      
      openstudio::model::MasslessOpaqueMaterial material(model);

      material.setName(name);

      // DLM: set on construction
      //std::string roughness = "MediumRough";
      //if (!roughnessElement.isNull()){
      //  roughness = roughnessElement.text().toStdString();
      //}
      //material.setRoughness(roughness);

      // sdd units = hr*ft2*degF/Btu, os units = m2-K/W
      Quantity rValueIP(rValueElement.text().toDouble(), BTUUnit(BTUExpnt(-1,2,1,1)));
      OptionalQuantity rValueWh = QuantityConverter::instance().convert(rValueIP, whSys);
      OS_ASSERT(rValueWh);
      OS_ASSERT(rValueWh->units() == WhUnit(WhExpnt(-1,0,2,1)));
      material.setThermalResistance(rValueWh->value());

      // DLM: set on construction
      //if (!thermalAbsorptanceElement.isNull()){
      //  material.setThermalAbsorptance(thermalAbsorptanceElement.text().toDouble());
      //}

      // DLM: set on construction
      //if (!solarAbsorbanceElement.isNull()){
      //  material.setSolarAbsorptance(solarAbsorbanceElement.text().toDouble());
      //}
        
      // DLM: set on construction
      //if (!visibleAbsorptanceElement.isNull()){
      //  material.setVisibleAbsorptance(visibleAbsorptanceElement.text().toDouble());
      //}

      return material;
    }

    LOG(Error, "Material named '" << name << "' could not be created.")

    return boost::none;
  }

  boost::optional<QDomElement> ForwardTranslator::translateConstructionBase(const openstudio::model::ConstructionBase& constructionBase, QDomDocument& doc)
  {
    QDomElement result;
    
    if (constructionBase.optionalCast<model::LayeredConstruction>()){
      model::LayeredConstruction construction = constructionBase.cast<model::LayeredConstruction>();
      model::StandardsInformationConstruction info = constructionBase.standardsInformation();

      bool heated = false;
      if (construction.optionalCast<model::Construction>()){
        heated = false;
      } else if (construction.optionalCast<model::ConstructionWithInternalSource>()){
        heated = true;
      } else {
        return boost::none;
      }

      // check if any layer has not been translated, constructions using simple glazing material will
      // be skipped here because the simple glazing material is not recorded as mapped
      std::vector<model::Material> layers = construction.layers();
      for (const model::Material& material : layers){
        if (m_translatedObjects.find(material.handle()) ==  m_translatedObjects.end()){
          return boost::none;
        }
      }
      
      result = doc.createElement("ConsAssm");
      
      // name
      std::string name = construction.name().get();
      QDomElement nameElement = doc.createElement("Name");
      result.appendChild(nameElement);
      nameElement.appendChild(doc.createTextNode(escapeName(name)));

      // DLM: prescribed, not input
      // specification method
      //QDomElement specMthdElement = doc.createElement("SpecMthd");
      //result.appendChild(specMthdElement);
      //specMthdElement.appendChild(doc.createTextNode("Layers"));

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
      if (intendedSurfaceType){
        if (istringEqual("ExteriorWall", *intendedSurfaceType)){
          compatibleSurfType = "ExteriorWall";
        } else if (istringEqual("AtticRoof", *intendedSurfaceType) ||
                   istringEqual("ExteriorRoof", *intendedSurfaceType)){
          compatibleSurfType = "Roof";
        } else if (istringEqual("ExteriorFloor", *intendedSurfaceType)){
          compatibleSurfType = "ExteriorFloor";
        } else if (istringEqual("GroundContactWall", *intendedSurfaceType)){
          compatibleSurfType = "UndergroundWall";
        } else if (istringEqual("GroundContactFloor", *intendedSurfaceType)){
          compatibleSurfType = "UndergroundFloor";
        } else if (istringEqual("InteriorWall", *intendedSurfaceType)){
          compatibleSurfType = "InteriorWall";
        } else if (istringEqual("InteriorCeiling", *intendedSurfaceType)){
          compatibleSurfType = "Ceiling";
        } else if (istringEqual("InteriorFloor", *intendedSurfaceType)){
          compatibleSurfType = "InteriorFloor";
        } else{
          // DLM: warn?
        }
      }
      if (compatibleSurfType){
        QDomElement compatibleSurfTypeElement = doc.createElement("CompatibleSurfType");
        result.appendChild(compatibleSurfTypeElement);
        compatibleSurfTypeElement.appendChild(doc.createTextNode(toQString(*compatibleSurfType)));
      } else{
        // DLM: warn?
      }

      unsigned n = (unsigned) layers.size();
      if (n > 0){

        std::string extRoughness;
        boost::optional<double> extSolarAbsorptance;
        boost::optional<double> extThermalAbsorptance;
        boost::optional<double> extVisibleAbsorptance;
        boost::optional<double> intSolarAbsorptance;
        boost::optional<double> intThermalAbsorptance;
        boost::optional<double> intVisibleAbsorptance;

        if (layers[0].optionalCast<model::StandardOpaqueMaterial>()){
          model::StandardOpaqueMaterial extMaterial = layers[0].cast<model::StandardOpaqueMaterial>();
          extRoughness = extMaterial.roughness();
          extSolarAbsorptance = extMaterial.solarAbsorptance();
          extThermalAbsorptance = extMaterial.thermalAbsorptance();
          extVisibleAbsorptance = extMaterial.visibleAbsorptance();
        }else if (layers[0].optionalCast<model::MasslessOpaqueMaterial>()){
          model::MasslessOpaqueMaterial extMaterial = layers[0].cast<model::MasslessOpaqueMaterial>();
          extRoughness = extMaterial.roughness();
          extSolarAbsorptance = extMaterial.solarAbsorptance();
          extThermalAbsorptance = extMaterial.thermalAbsorptance();
          extVisibleAbsorptance = extMaterial.visibleAbsorptance();
        }

        if (layers[n-1].optionalCast<model::StandardOpaqueMaterial>()){
          model::StandardOpaqueMaterial intMaterial = layers[n-1].cast<model::StandardOpaqueMaterial>();
          intSolarAbsorptance = intMaterial.solarAbsorptance();
          intThermalAbsorptance = intMaterial.thermalAbsorptance();
          intVisibleAbsorptance = intMaterial.visibleAbsorptance();
        }else if (layers[n-1].optionalCast<model::MasslessOpaqueMaterial>()){
          model::MasslessOpaqueMaterial intMaterial = layers[n-1].cast<model::MasslessOpaqueMaterial>();
          intSolarAbsorptance = intMaterial.solarAbsorptance();
          intThermalAbsorptance = intMaterial.thermalAbsorptance();
          intVisibleAbsorptance = intMaterial.visibleAbsorptance();
        }

        // DLM: Not input
        // ext roughness
        //if (!extRoughness.empty()){
        //  QDomElement roughnessElement = doc.createElement("ExtRoughness");
        //  result.appendChild(roughnessElement);
        //  roughnessElement.appendChild(doc.createTextNode(toQString(extRoughness)));
        //}

        // DLM: Not input
        // ext solarAbsorptance
        //if (extSolarAbsorptance){
        //  QDomElement solarAbsorptanceElement = doc.createElement("ExtSolAbs"); 
        //  result.appendChild(solarAbsorptanceElement);
        //  solarAbsorptanceElement.appendChild(doc.createTextNode(QString::number(*extSolarAbsorptance)));
        //}

        // DLM: Not input
        // ext thermalAbsorptance
        //if (extThermalAbsorptance){
        //  QDomElement thermalAbsorptanceElement = doc.createElement("ExtThrmlAbs");
        //  result.appendChild(thermalAbsorptanceElement);
        //  thermalAbsorptanceElement.appendChild(doc.createTextNode(QString::number(*extThermalAbsorptance)));
        //}

        // DLM: Not input
        // ext visibleAbsorptance
        //if (extVisibleAbsorptance){
        //  QDomElement visibleAbsorptanceElement = doc.createElement("ExtVisAbs");
        //  result.appendChild(visibleAbsorptanceElement);
        //  visibleAbsorptanceElement.appendChild(doc.createTextNode(QString::number(*extVisibleAbsorptance)));
        //}

        // DLM: Not input
        // int solarAbsorptance
        //if (intSolarAbsorptance){
        //  QDomElement solarAbsorptanceElement = doc.createElement("IntSolAbs"); 
        //  result.appendChild(solarAbsorptanceElement);
        //  solarAbsorptanceElement.appendChild(doc.createTextNode(QString::number(*intSolarAbsorptance)));
        //}

        // DLM: Not input
        // int thermalAbsorptance
        //if (intThermalAbsorptance){
        //  QDomElement thermalAbsorptanceElement = doc.createElement("IntThrmlAbs");
        //  result.appendChild(thermalAbsorptanceElement);
        //  thermalAbsorptanceElement.appendChild(doc.createTextNode(QString::number(*intThermalAbsorptance)));
        //}

        // DLM: Not input
        // int visibleAbsorptance
        //if (intVisibleAbsorptance){
        //  QDomElement visibleAbsorptanceElement = doc.createElement("IntVisAbs");
        //  result.appendChild(visibleAbsorptanceElement);
        //  visibleAbsorptanceElement.appendChild(doc.createTextNode(QString::number(*intVisibleAbsorptance)));
        //}
      }

      for (const model::Material& material : layers){

        // materialReference
        std::string materialName = material.name().get();
        QDomElement materialReferenceElement = doc.createElement("MatRef");
        result.appendChild(materialReferenceElement);
        materialReferenceElement.appendChild(doc.createTextNode(escapeName(materialName)));
      }

      m_translatedObjects[construction.handle()] = result;

    }else if (constructionBase.optionalCast<model::FFactorGroundFloorConstruction>()){
      // DLM: I think this is out of date
      model::FFactorGroundFloorConstruction construction =  constructionBase.cast<model::FFactorGroundFloorConstruction>();

      result = doc.createElement("ConsAssm");
      
      // name
      std::string name = construction.name().get();
      QDomElement nameElement = doc.createElement("Name");
      result.appendChild(nameElement);
      nameElement.appendChild(doc.createTextNode(escapeName(name)));

      // DLM: prescribed, not input
      // specification method
      //QDomElement specMthdElement = doc.createElement("SpecMthd");
      //result.appendChild(specMthdElement);
      //specMthdElement.appendChild(doc.createTextNode("FFactor"));

      // DLM: not input
      // specification method
      //QDomElement fFactorElement = doc.createElement("FFactor");
      //result.appendChild(fFactorElement);
      //// sdd units = Btu/(hr*ft*F), os units = W/(m*K)
      //Quantity fFactorSI(construction.fFactor(), WhUnit(WhExpnt(1,0,-1,-1)));
      //OptionalQuantity fFactorIP = QuantityConverter::instance().convert(fFactorSI, UnitSystem(UnitSystem::BTU));
      //OS_ASSERT(fFactorIP);
      //OS_ASSERT(fFactorIP->units() == BTUUnit(BTUExpnt(1,-1,-1,-1)));
      //fFactorElement.appendChild(doc.createTextNode(QString::number(fFactorIP->value())));

      // DLM: cannot write out
      //<IntSolAbs>0.7</IntSolAbs>
      //<IntThrmlAbs>0.9</IntThrmlAbs>
      //<IntVisAbs>0.5</IntVisAbs>
      //<MatRef index="0">NACM_Concrete 4in</MatRef>
      //<MatRef index="1">NACM_Carpet Pad</MatRef>

      m_translatedObjects[construction.handle()] = result;

    }else if (constructionBase.optionalCast<model::CFactorUndergroundWallConstruction>()){
      // DLM: I think this is out of date
      model::CFactorUndergroundWallConstruction construction =  constructionBase.cast<model::CFactorUndergroundWallConstruction>();

      result = doc.createElement("ConsAssm");
      
      // name
      std::string name = construction.name().get();
      QDomElement nameElement = doc.createElement("Name");
      result.appendChild(nameElement);
      nameElement.appendChild(doc.createTextNode(escapeName(name)));

      // DLM: prescribed, not input
      // specification method
      //QDomElement specMthdElement = doc.createElement("SpecMthd");
      //result.appendChild(specMthdElement);
      //specMthdElement.appendChild(doc.createTextNode("CFactor"));

      // DLM: not input
      // specification method
      //QDomElement cFactorElement = doc.createElement("CFactor");
      //result.appendChild(cFactorElement);
      //// sdd units = Btu/(hr*ft^2*F), os units = W/(m^2*K)
      //Quantity cFactorSI(construction.cFactor(), WhUnit(WhExpnt(1,0,-2,-1)));
      //OptionalQuantity cFactorIP = QuantityConverter::instance().convert(cFactorSI, UnitSystem(UnitSystem::BTU));
      //OS_ASSERT(cFactorIP);
      //OS_ASSERT(cFactorIP->units() == BTUUnit(BTUExpnt(1,-2,-1,-1)));
      //cFactorElement.appendChild(doc.createTextNode(QString::number(cFactorIP->value())));

      // DLM: cannot write out
      //<IntSolAbs>0.7</IntSolAbs>
      //<IntThrmlAbs>0.9</IntThrmlAbs>
      //<IntVisAbs>0.5</IntVisAbs>
      //<MatRef index="0">NACM_Concrete 4in</MatRef>
      //<MatRef index="1">NACM_Carpet Pad</MatRef>

      m_translatedObjects[construction.handle()] = result;

    }
    
    // todo: handle u-factor constructions

    return result;
  }

  boost::optional<QDomElement> ForwardTranslator::translateDoorConstruction(const openstudio::model::ConstructionBase& constructionBase, QDomDocument& doc)
  {
    boost::optional<QDomElement> result;

    UnitSystem ipSys(UnitSystem::IP);
    UnitSystem btuSys(UnitSystem::BTU);

    if (constructionBase.optionalCast<model::Construction>()){
      model::Construction construction = constructionBase.cast<model::Construction>();
      model::StandardsInformationConstruction info = constructionBase.standardsInformation();

      result = doc.createElement("DrCons");
      
      // name
      std::string name = construction.name().get();
      QDomElement nameElement = doc.createElement("Name");
      result->appendChild(nameElement);
      nameElement.appendChild(doc.createTextNode(escapeName(name)));

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
      if (layers.size() == 1){
        if (layers[0].optionalCast<model::MasslessOpaqueMaterial>()){

          model::MasslessOpaqueMaterial material = layers[0].cast<model::MasslessOpaqueMaterial>();

          // os units = W/m2-K, sdd units = Btu/(hr*f2t*F)
          Quantity uFactorSI(material.thermalConductance(), WhUnit(WhExpnt(1, 0, -2, -1)));
          OptionalQuantity uFactorIP = QuantityConverter::instance().convert(uFactorSI, btuSys);
          OS_ASSERT(uFactorIP);
          OS_ASSERT(uFactorIP->units() == BTUUnit(BTUExpnt(1,-2,-1,-1)));
          uFactor = uFactorIP->value();
        }
      }

      if (!uFactor){
        certificationMthd.reset();
      }

      boost::optional<std::string> fenestrationType = info.fenestrationType();
      if (fenestrationType){
        if (istringEqual("Swinging Door", *fenestrationType)){
          open = "Swinging";
        } else if (istringEqual("Non-Swinging Door", *fenestrationType)){
          open = "NonSwinging";
        }
      }

      // write the xml
      if (type){
        QDomElement typeElement = doc.createElement("Type");
        result->appendChild(typeElement);
        typeElement.appendChild(doc.createTextNode(toQString(*type)));
      }
      if (certificationMthd){
        QDomElement certificationMthdElement = doc.createElement("CertificationMthd");
        result->appendChild(certificationMthdElement);
        certificationMthdElement.appendChild(doc.createTextNode(toQString(*certificationMthd)));
      }
      if (uFactor){
        QDomElement uFactorElement = doc.createElement("UFactor");
        result->appendChild(uFactorElement);
        uFactorElement.appendChild(doc.createTextNode(QString::number(*uFactor)));
      } else{
        //Do not want this logged, http://code.google.com/p/cbecc/issues/detail?id=695
        //LOG(Error, "Could not calculate UFactor for DrCons '" << name << "'");
      }
      if (open){
        QDomElement openElement = doc.createElement("Open");
        result->appendChild(openElement);
        openElement.appendChild(doc.createTextNode(toQString(*open)));
      }

      // mark the construction as translated, not the material
      m_translatedObjects[construction.handle()] = result.get();
    }

    return result;
  }

  boost::optional<QDomElement> ForwardTranslator::translateFenestrationConstruction(const openstudio::model::ConstructionBase& constructionBase, QDomDocument& doc)
  {
    boost::optional<QDomElement> result;

    UnitSystem ipSys(UnitSystem::IP);
    UnitSystem btuSys(UnitSystem::BTU);

    if (constructionBase.optionalCast<model::Construction>()){
      model::Construction construction = constructionBase.cast<model::Construction>();
      model::StandardsInformationConstruction info = constructionBase.standardsInformation();

      result = doc.createElement("FenCons");
      
      // name
      std::string name = construction.name().get();
      QDomElement nameElement = doc.createElement("Name");
      result->appendChild(nameElement);
      nameElement.appendChild(doc.createTextNode(escapeName(name)));

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
      if (fenestrationType){
        if (istringEqual("Glass Skylight with Curb", *fenestrationType)){
          fenType = "Skylight";
          skyltGlz = "Glass";
          skyltCurb = "CurbMounted";
        } else if (istringEqual("Plastic Skylight with Curb", *fenestrationType)){
          fenType = "Skylight";
          skyltGlz = "Plastic";
          skyltCurb = "CurbMounted";
        } else if (istringEqual("Glass Skylight without Curb", *fenestrationType)){
          fenType = "Skylight";
          skyltGlz = "Glass";
          skyltCurb = "DeckMounted";
        } else if (istringEqual("Plastic Skylight without Curb", *fenestrationType)){
          fenType = "Skylight";
          skyltGlz = "Plastic";
          skyltCurb = "DeckMounted";
        } else{
          fenType = "VerticalFenestration";

          if (istringEqual("Fixed Window", *fenestrationType)){
            fenProdType = "FixedWindow";
          } else if (istringEqual("Operable Window", *fenestrationType)){
            fenProdType = "OperableWindow";
            
            // DLM: todo set operableWinConfiguration
            //operableWinConfiguration = "CasementAwning" or "Sliding"
          } else if (istringEqual("Curtain Wall", *fenestrationType)){
            fenProdType = "CurtainWall";
          } else if (istringEqual("Glazed Door", *fenestrationType)){
            fenProdType = "GlazedDoor";
          } else {
            // 'Swinging Door' or 'Non-Swinging Door'
            // These should have been grouped with door constructions, potentially warn
          }

          boost::optional<std::string> fenestrationTint = info.fenestrationTint();
          if (fenestrationTint){
            if (istringEqual("Clear", *fenestrationTint)){
              glzTint = "ClearGlazing";
            } else{
              glzTint = "TintedGlazing";
            }
          }
        }
      }

      boost::optional<std::string> assmContext;
      boost::optional<std::string> certificationMthd = std::string("NFRCRated"); // default
      boost::optional<std::string> fenestrationAssemblyContext = info.fenestrationAssemblyContext();
      if (fenestrationAssemblyContext){
        if (istringEqual("Manufactured", *fenestrationAssemblyContext)){
          assmContext = "Manufactured";
        } else if (istringEqual("Field Fabricated", *fenestrationAssemblyContext)){
          assmContext = "FieldFabricated";
          certificationMthd = "CECDefaultPerformance"; // right?
        } else if (istringEqual("Site Built", *fenestrationAssemblyContext)){
          assmContext = "SiteBuilt";
        }
      }
      
      boost::optional<std::string> greenhouseGardenWin;

      boost::optional<std::string> fenFrm;
      boost::optional<std::string> fenestrationFrameType = info.fenestrationFrameType();
      if (fenestrationFrameType){
        if (istringEqual("Metal Framing", *fenestrationFrameType)){
          fenFrm = "MetalFraming";
        } else if (istringEqual("Metal Framing with Thermal Break", *fenestrationFrameType)){
          fenFrm = "MetalFramingWithThermalBreak";
        } else if (istringEqual("Non-Metal Framing", *fenestrationFrameType)){
          fenFrm = "NonMetalFraming";
        }
      }

      boost::optional<std::string> fenPanes;
      boost::optional<std::string> fenestrationNumberOfPanes = info.fenestrationNumberOfPanes();
      if (fenestrationNumberOfPanes){
        if (istringEqual("Single Pane", *fenestrationNumberOfPanes)){
          fenPanes = "SinglePane";
        } else if (istringEqual("Double Pane", *fenestrationNumberOfPanes)){
          fenPanes = "DoublePane";
        } else if (istringEqual("Triple Pane", *fenestrationNumberOfPanes)){
          fenPanes = "DoublePane";
        } else if (istringEqual("Quadruple Pane", *fenestrationNumberOfPanes)){
          fenPanes = "DoublePane";
        } else if (istringEqual("Glass Block", *fenestrationNumberOfPanes)){
          fenPanes = "GlassBlock";
        }
      }

      boost::optional<std::string> winDivider;
      boost::optional<std::string> fenestrationDividerType = info.fenestrationDividerType();
      if (fenestrationDividerType){
        if (istringEqual("True Divided Lite", *fenestrationDividerType)){
          winDivider = "TrueDividedLite";
        } else if (istringEqual("Between Panes < 7/16\"", *fenestrationDividerType)){
          winDivider = "DividerBtwnPanesLessThan7_16in";
        } else if (istringEqual("Between Panes >= 7/16\"", *fenestrationDividerType)){
          winDivider = "DividerBtwnPanesGreaterThanOrEqualTo7_16in";
        }
      }

      boost::optional<std::string> diffusing;

      boost::optional<double> shgc;
      boost::optional<double> uFactor;
      boost::optional<double> vt;

      // can only get SHGC, TVis, UFactor for special cases
      std::vector<model::Material> layers = construction.layers();
      if (layers.size() == 1){
        if (layers[0].optionalCast<model::SimpleGlazing>()){

          model::SimpleGlazing simpleGlazing = layers[0].cast<model::SimpleGlazing>();
          
          shgc = simpleGlazing.solarHeatGainCoefficient();

          // os units = W/m2-K, sdd units = Btu/(hr*f2t*F)
          Quantity uFactorSI(simpleGlazing.uFactor(), WhUnit(WhExpnt(1, 0, -2, -1)));
          OptionalQuantity uFactorIP = QuantityConverter::instance().convert(uFactorSI, btuSys);
          OS_ASSERT(uFactorIP);
          OS_ASSERT(uFactorIP->units() == BTUUnit(BTUExpnt(1,-2,-1,-1)));
          uFactor = uFactorIP->value();

          vt = simpleGlazing.visibleTransmittance();
        }
      }

      // if performance is not known use defaults if possible
      if (!shgc || !uFactor || !vt){
        if (fenFrm && fenPanes){
          certificationMthd = "CECDefaultPerformance";
        }
      }

      // write to XML
      if (fenType){
        QDomElement fenTypeElement = doc.createElement("FenType");
        result->appendChild(fenTypeElement);
        fenTypeElement.appendChild(doc.createTextNode(toQString(*fenType)));
      }
      if (fenProdType){
        QDomElement fenProdTypeElement = doc.createElement("FenProdType");
        result->appendChild(fenProdTypeElement);
        fenProdTypeElement.appendChild(doc.createTextNode(toQString(*fenProdType)));
      }
      if (assmContext){
        QDomElement assmContextElement = doc.createElement("AssmContext");
        result->appendChild(assmContextElement);
        assmContextElement.appendChild(doc.createTextNode(toQString(*assmContext)));
      }
      if (certificationMthd){
        QDomElement certificationMthdElement = doc.createElement("CertificationMthd");
        result->appendChild(certificationMthdElement);
        certificationMthdElement.appendChild(doc.createTextNode(toQString(*certificationMthd)));
      }
      if (skyltGlz){
        QDomElement skyltGlzElement = doc.createElement("SkyltGlz");
        result->appendChild(skyltGlzElement);
        skyltGlzElement.appendChild(doc.createTextNode(toQString(*skyltGlz)));
      }
      if (skyltCurb){
        QDomElement skyltCurbElement = doc.createElement("SkyltCurb");
        result->appendChild(skyltCurbElement);
        skyltCurbElement.appendChild(doc.createTextNode(toQString(*skyltCurb)));
      }
      if (operableWinConfiguration){
        QDomElement operableWinConfigurationElement = doc.createElement("OperableWinConfiguration");
        result->appendChild(operableWinConfigurationElement);
        operableWinConfigurationElement.appendChild(doc.createTextNode(toQString(*operableWinConfiguration)));
      }
      if (greenhouseGardenWin){
        QDomElement greenhouseGardenWinElement = doc.createElement("GreenhouseGardenWin");
        result->appendChild(greenhouseGardenWinElement);
        greenhouseGardenWinElement.appendChild(doc.createTextNode(toQString(*greenhouseGardenWin)));
      }
      if (fenFrm){
        QDomElement fenFrmElement = doc.createElement("FenFrm");
        result->appendChild(fenFrmElement);
        fenFrmElement.appendChild(doc.createTextNode(toQString(*fenFrm)));
      }
      if (fenPanes){
        QDomElement fenPanesElement = doc.createElement("FenPanes");
        result->appendChild(fenPanesElement);
        fenPanesElement.appendChild(doc.createTextNode(toQString(*fenPanes)));
      }
      if (glzTint){
        QDomElement glzTintElement = doc.createElement("GlzTint");
        result->appendChild(glzTintElement);
        glzTintElement.appendChild(doc.createTextNode(toQString(*glzTint)));
      }
      if (winDivider){
        QDomElement winDividerElement = doc.createElement("WinDivider");
        result->appendChild(winDividerElement);
        winDividerElement.appendChild(doc.createTextNode(toQString(*winDivider)));
      }
      if (diffusing){
        QDomElement diffusingElement = doc.createElement("Diffusing");
        result->appendChild(diffusingElement);
        diffusingElement.appendChild(doc.createTextNode(toQString(*diffusing)));
      }
      
      

      if (shgc){
        QDomElement shgcElement = doc.createElement("SHGC");
        result->appendChild(shgcElement);
        shgcElement.appendChild(doc.createTextNode(QString::number(*shgc)));
      } else{
        //Do not want this logged, http://code.google.com/p/cbecc/issues/detail?id=695
        //LOG(Error, "Could not calculate SHGC for FenCons '" << name << "'");
      }
      if (uFactor){
        QDomElement uFactorElement = doc.createElement("UFactor");
        result->appendChild(uFactorElement);
        uFactorElement.appendChild(doc.createTextNode(QString::number(*uFactor)));
      } else{
        //Do not want this logged, http://code.google.com/p/cbecc/issues/detail?id=695
        //LOG(Error, "Could not calculate UFactor for FenCons '" << name << "'");
      }
      if (vt){
        QDomElement vtElement = doc.createElement("VT");
        result->appendChild(vtElement);
        vtElement.appendChild(doc.createTextNode(QString::number(*vt)));
      } else{
        //Do not want this logged, http://code.google.com/p/cbecc/issues/detail?id=695
        //LOG(Error, "Could not calculate VT for FenCons '" << name << "'");
      }

      // mark the construction as translated, not the material
      m_translatedObjects[construction.handle()] = result.get();
    }

    return result;
  }

  boost::optional<QDomElement> ForwardTranslator::translateMaterial(const openstudio::model::Material& material, QDomDocument& doc)
  {

    if (!(material.optionalCast<model::StandardOpaqueMaterial>() ||
          material.optionalCast<model::MasslessOpaqueMaterial>() ||
          material.optionalCast<model::AirGap>())){
      return boost::none;
    }

    model::StandardsInformationMaterial info = material.standardsInformation();

    QDomElement result = doc.createElement("Mat");
    m_translatedObjects[material.handle()] = result;

    // name
    std::string name = material.name().get();
    QDomElement nameElement = doc.createElement("Name");
    result.appendChild(nameElement);
    nameElement.appendChild(doc.createTextNode(escapeName(name)));

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
    if (standardsCategory && info.isCompositeMaterial()){
      standardsCategory = "Composite";
    }

    boost::optional<std::string> standardsIdentifier = info.standardsIdentifier();
    boost::optional<std::string> compositeFramingMaterial = info.compositeFramingMaterial();
    boost::optional<std::string> compositeFramingConfiguration = info.compositeFramingConfiguration();
    boost::optional<std::string> compositeFramingDepth = info.compositeFramingDepth();
    boost::optional<std::string> compositeFramingSize = info.compositeFramingSize();
    boost::optional<std::string> compositeCavityInsulation = info.compositeCavityInsulation();
  
    if (standardsCategory){
      QDomElement element = doc.createElement("CodeCat");
      result.appendChild(element);
      element.appendChild(doc.createTextNode(toQString(*standardsCategory)));
    }

    if (standardsIdentifier){
      QDomElement element = doc.createElement("CodeItem");
      result.appendChild(element);
      element.appendChild(doc.createTextNode(toQString(*standardsIdentifier)));
    }

    if (compositeFramingMaterial){
      QDomElement element = doc.createElement("FrmMat");
      result.appendChild(element);
      element.appendChild(doc.createTextNode(toQString(*compositeFramingMaterial)));
    }

    if (compositeFramingConfiguration){
      QDomElement element = doc.createElement("FrmConfig");
      result.appendChild(element);
      element.appendChild(doc.createTextNode(toQString(*compositeFramingConfiguration)));
    }

    if (compositeFramingDepth){
      QDomElement element = doc.createElement("FrmDepth");
      result.appendChild(element);
      element.appendChild(doc.createTextNode(toQString(*compositeFramingDepth)));
    }

    if (compositeCavityInsulation){

      // DLM: this is a stupid switch, this should get fixed in CBECC rules
      if (compositeFramingMaterial && istringEqual("Metal", *compositeFramingMaterial)){
        bool isNumber;
        QString value = toQString(*compositeCavityInsulation);
        value.toDouble(&isNumber);
        if (isNumber){
          value = QString("R-") + value;
        }
        QDomElement element = doc.createElement("CavityInsOpt");
        result.appendChild(element);
        element.appendChild(doc.createTextNode(value));
      } else {
        QDomElement element = doc.createElement("CavityIns");
        result.appendChild(element);
        element.appendChild(doc.createTextNode(toQString(*compositeCavityInsulation)));
      }
    }

    return result;
  }

    
} // sdd
} // openstudio
