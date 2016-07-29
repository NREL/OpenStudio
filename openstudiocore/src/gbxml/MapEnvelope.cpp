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
#include "../model/AirGap.hpp"
#include "../model/AirGap_Impl.hpp"
#include "../model/MasslessOpaqueMaterial.hpp"
#include "../model/MasslessOpaqueMaterial_Impl.hpp"
#include "../model/StandardOpaqueMaterial.hpp"
#include "../model/StandardOpaqueMaterial_Impl.hpp"
#include "../model/FenestrationMaterial.hpp"
#include "../model/FenestrationMaterial_Impl.hpp"
#include "../model/SimpleGlazing.hpp"
#include "../model/SimpleGlazing_Impl.hpp"
#include "../model/ModelPartitionMaterial.hpp"
#include "../model/ModelPartitionMaterial_Impl.hpp"
#include "../utilities/core/Assert.hpp"

#include <QFile>
#include <QDomDocument>
#include <QDomElement>

namespace openstudio {
namespace gbxml {
 
  boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateConstruction(const QDomElement& element, const QDomNodeList& layerElements, const QDomDocument& doc, openstudio::model::Model& model)
  {
    // Krishnan, this constructor should only be used for unique objects like Building and Site
    //openstudio::model::Construction construction = model.getUniqueModelObject<openstudio::model::Construction>();

    openstudio::model::Construction construction(model);
    QString constructionId = element.attribute("id");
    construction.setName(escapeName(constructionId));
        
    QDomNodeList layerIdList = element.elementsByTagName("LayerId");

    if (layerIdList.isEmpty()){
      return construction;
    }

    QString layerId = layerIdList.at(0).toElement().attribute("layerIdRef");

    std::vector<openstudio::model::Material> materials;
    for (int i = 0; i < layerElements.count(); i++){
      QDomElement layerElement = layerElements.at(i).toElement();
      if (layerId == layerElement.attribute("id")){
        QDomNodeList materialIdElements = layerElement.elementsByTagName("MaterialId");
        for (int j = 0; j < materialIdElements.count(); j++){
          QString materialId = materialIdElements.at(j).toElement().attribute("materialIdRef");
              
          // we are naming openstudio objects with id to guarantee unique names, there should be a material with this name in the openstudio model
          std::string materialName = materialId.toStdString();
          boost::optional<openstudio::model::Material> material = model.getModelObjectByName<openstudio::model::Material>(materialName);
          OS_ASSERT(material); // Krishnan, what type of error handling do you want?
          materials.push_back(*material);
        }
        break;
      }
    }

    // now assign all layers to real material, does gbXML have same layer order convention as E+?
    for (unsigned i = 0; i < materials.size(); ++i){
      bool test = false;
          
      if (materials[i].optionalCast<openstudio::model::OpaqueMaterial>()){
        test = construction.insertLayer(i, materials[i].cast<openstudio::model::OpaqueMaterial>());
      }else if (materials[i].optionalCast<openstudio::model::FenestrationMaterial>()){
        test = construction.insertLayer(i, materials[i].cast<openstudio::model::FenestrationMaterial>());
      }else if (materials[i].optionalCast<openstudio::model::ModelPartitionMaterial>()){
        test = construction.setLayer(materials[i].cast<openstudio::model::ModelPartitionMaterial>());
      }
          
      OS_ASSERT(test); // Krishnan, what type of error handling do you want?
    }

    return construction;
  }

  boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateWindowType(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model)
  {  
    openstudio::model::Construction construction(model);
    QString windowTypeId = element.attribute("id");
    construction.setName(escapeName(windowTypeId));

    boost::optional<double> uValue;
    boost::optional<double> shgc;
    boost::optional<double> tVis;

    QDomNodeList uValueElements = element.elementsByTagName("U-value");
    for (int i = 0; i < uValueElements.count(); i++){
      QDomElement uValueElement = uValueElements.at(i).toElement();
      if (uValueElement.attribute("unit") == "WPerSquareMeterK"){
        uValue = uValueElement.text().toDouble();
        break;
      }
    }
        
    QDomNodeList shgcElements = element.elementsByTagName("SolarHeatGainCoeff");
    for (int i = 0; i < shgcElements.count(); i++){
      QDomElement shgcElement = shgcElements.at(i).toElement();
      if (shgcElement.attribute("unit") == "Fraction"){
        shgc = shgcElement.text().toDouble();
        break;
      }
    }
        
    QDomNodeList transmittanceElements = element.elementsByTagName("Transmittance");
    for (int i = 0; i < transmittanceElements.count(); i++){
      QDomElement transmittanceElement = transmittanceElements.at(i).toElement();
      if (transmittanceElement.attribute("type") == "Visible"){
        tVis = transmittanceElement.text().toDouble();
        break;
      }
    }

    if (uValue && shgc && tVis){
      model::SimpleGlazing glazing(model);
      glazing.setUFactor(*uValue);
      glazing.setSolarHeatGainCoefficient(*shgc);
      glazing.setVisibleTransmittance(*tVis);

      std::vector<model::Material> layers;
      layers.push_back(glazing);
      construction.setLayers(layers);
    }
    
    return construction;
  }

  boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateMaterial(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model)
  {
    boost::optional<openstudio::model::ModelObject> result;

    QDomNodeList rvalueElements = element.elementsByTagName("R-value");
    QDomNodeList densityElements = element.elementsByTagName("Density");
    QDomNodeList conductivityElements = element.elementsByTagName("Conductivity");
    QDomNodeList thicknessElements = element.elementsByTagName("Thickness");
    QDomNodeList specificHeatElements = element.elementsByTagName("SpecificHeat");
    QDomNodeList roughnessElements = element.elementsByTagName("Roughness");

    if (!(densityElements.isEmpty() || conductivityElements.isEmpty() || thicknessElements.isEmpty() || specificHeatElements.isEmpty())){

      double density = densityElements.at(0).toElement().text().toDouble();
      double conductivity = conductivityElements.at(0).toElement().text().toDouble();
      double thickness = thicknessElements.at(0).toElement().text().toDouble();
      double specificHeat = specificHeatElements.at(0).toElement().text().toDouble();

      std::string roughness = "MediumRough";
      if (!roughnessElements.isEmpty()){
        roughness = roughnessElements.at(0).toElement().text().toStdString();
      }

      openstudio::model::StandardOpaqueMaterial material(model);
      result = material;
        
      QString name = element.attribute("id");
      material.setName(escapeName(name));
        
      material.setDensity(density);
      material.setThermalConductivity(conductivity);
      material.setThickness(thickness);
      material.setSpecificHeat(specificHeat);
      material.setRoughness(roughness);

    } else if (!rvalueElements.isEmpty())  //Material no mass that has only R-value
    {
      // Krishnan, this constructor should only be used for unique objects like Building and Site
      //openstudio::model::MasslessOpaqueMaterial material = model.getUniqueModelObject<openstudio::model::MasslessOpaqueMaterial>();

      double rvalue = rvalueElements.at(0).toElement().text().toDouble();

      // the idd specifies a minimum value of .001 for rvalue
      rvalue = std::max(rvalue, .001);

      openstudio::model::MasslessOpaqueMaterial material(model);
      result = material;

      QString name = element.attribute("id");
      material.setName(escapeName(name));

      material.setThermalResistance(rvalue);

    } else { 
        
        // make a stub material
      openstudio::model::MasslessOpaqueMaterial material(model);
      result = material;

      QString name = element.attribute("id");
      material.setName(escapeName(name));

      LOG(Warn, "Creating stub material '" << name.toStdString() << "'");

      material.setThermalResistance(.001);
    }

    return result;
  }

  boost::optional<QDomElement> ForwardTranslator::translateConstructionBase(const openstudio::model::ConstructionBase& constructionBase, QDomDocument& doc)
  {
    boost::optional<QDomElement> result;

    bool isOpaque = constructionBase.isOpaque();

    if (isOpaque){
      result = doc.createElement("Construction");
      m_translatedObjects[constructionBase.handle()] = *result;
    } else{
      result = doc.createElement("WindowType");
      m_translatedObjects[constructionBase.handle()] = *result;
    }

    std::string name = constructionBase.name().get();

    // id
    result->setAttribute("id", escapeName(name));

    // name
    QDomElement nameElement = doc.createElement("Name");
    result->appendChild(nameElement);
    nameElement.appendChild(doc.createTextNode(QString::fromStdString(name)));
    
    if (isOpaque){
      if (constructionBase.optionalCast<model::LayeredConstruction>()){
        for (const auto& layer : constructionBase.cast<model::LayeredConstruction>().layers()){
          QDomElement layerIdElement = doc.createElement("LayerId");
          result->appendChild(layerIdElement);
          layerIdElement.setAttribute("layerIdRef", escapeName(layer.name().get() + " Layer"));

          m_materials.insert(layer);
        }
      }
    } else{
      boost::optional<double> visibleTransmittance;
      boost::optional<double> uFactor;
      boost::optional<double> solarHeatGainCoefficient;

      if (constructionBase.optionalCast<model::LayeredConstruction>()){
        std::vector<model::Material> layers = constructionBase.cast<model::LayeredConstruction>().layers();
        if (layers.size() == 1u){
          if (layers[0].optionalCast<model::SimpleGlazing>()){
            model::SimpleGlazing glazing = layers[0].cast<model::SimpleGlazing>();
            visibleTransmittance = glazing.visibleTransmittance();
            uFactor = glazing.uFactor();
            solarHeatGainCoefficient = glazing.solarHeatGainCoefficient();
          }
        }
      }

      if (!visibleTransmittance){
        visibleTransmittance = constructionBase.visibleTransmittance();
      }
      if (!uFactor){
        uFactor = constructionBase.uFactor();
      }
      //if (!solarHeatGainCoefficient){
      //  solarHeatGainCoefficient = constructionBase.solarHeatGainCoefficient();
      //}


      if (visibleTransmittance){
        QDomElement element = doc.createElement("Transmittance");
        result->appendChild(element);
        element.appendChild(doc.createTextNode(QString::number(*visibleTransmittance)));
        element.setAttribute("unit", "Fraction");
        element.setAttribute("type", "Visible");
        element.setAttribute("surfaceType", "Both");
      }

      if (uFactor){
        QDomElement element = doc.createElement("U-value");
        result->appendChild(element);
        element.appendChild(doc.createTextNode(QString::number(*uFactor)));
        element.setAttribute("unit", "WPerSquareMeterK");
      }

      if (solarHeatGainCoefficient){
        QDomElement element = doc.createElement("SolarHeatGainCoeff");
        result->appendChild(element);
        element.appendChild(doc.createTextNode(QString::number(*solarHeatGainCoefficient)));
        element.setAttribute("unit", "Fraction");
      }
    }
    return result;
  }

  boost::optional<QDomElement> ForwardTranslator::translateLayer(const openstudio::model::Material& material, QDomDocument& doc)
  {
    boost::optional<QDomElement> result;

    result = doc.createElement("Layer");

    std::string materialName = material.name().get();
    std::string layerName = materialName + " Layer";

    // id
    result->setAttribute("id", escapeName(layerName));

    // name
    QDomElement nameElement = doc.createElement("Name");
    result->appendChild(nameElement);
    nameElement.appendChild(doc.createTextNode(QString::fromStdString(layerName)));

    // name
    QDomElement materialIdElement = doc.createElement("MaterialId");
    result->appendChild(materialIdElement);
    materialIdElement.setAttribute("materialIdRef", escapeName(materialName));

    return result;
  }
  
  boost::optional<QDomElement> ForwardTranslator::translateMaterial(const openstudio::model::Material& material, QDomDocument& doc)
  {
    boost::optional<QDomElement> result;

    result = doc.createElement("Material");

    std::string name = material.name().get();

    // id
    result->setAttribute("id", escapeName(name));

    // name
    QDomElement nameElement = doc.createElement("Name");
    result->appendChild(nameElement);
    nameElement.appendChild(doc.createTextNode(QString::fromStdString(name)));

    boost::optional<double> thermalReflectance;
    boost::optional<double> solarReflectance;
    boost::optional<double> visibleReflectance;
    boost::optional<std::string> roughness;
    boost::optional<double> thickness;
    boost::optional<double> conductivity;
    boost::optional<double> resistance;
    boost::optional<double> density;
    boost::optional<double> specificHeat;
    boost::optional<double> thermalAbsorptance;
    boost::optional<double> solarAbsorptance;
    boost::optional<double> visibleAbsorptance;

    if (material.optionalCast<openstudio::model::StandardOpaqueMaterial>()){
      openstudio::model::StandardOpaqueMaterial som = material.cast<openstudio::model::StandardOpaqueMaterial>();
      thermalReflectance = som.thermalReflectance();
      solarReflectance = som.solarReflectance();
      visibleReflectance = som.visibleReflectance();
      roughness = som.roughness();
      thickness = som.thickness();
      conductivity = som.conductivity();
      density = som.density();
      specificHeat = som.specificHeat(); 
      thermalAbsorptance = som.thermalAbsorptance();
      solarAbsorptance = som.solarAbsorptance();
      visibleAbsorptance = som.visibleAbsorptance();
    }else if (material.optionalCast<openstudio::model::MasslessOpaqueMaterial>()){
      openstudio::model::MasslessOpaqueMaterial mom = material.cast<openstudio::model::MasslessOpaqueMaterial>();
      roughness = mom.roughness();
      resistance = mom.thermalResistance();
      thermalAbsorptance = mom.thermalAbsorptance();
      solarAbsorptance = mom.solarAbsorptance();
      visibleAbsorptance = mom.visibleAbsorptance();
    }else if (material.optionalCast<openstudio::model::AirGap>()){
      openstudio::model::AirGap gap = material.cast<openstudio::model::AirGap>();
      resistance = gap.thermalResistance();
    }

    if (thermalReflectance){
      QDomElement element = doc.createElement("Reflectance");
      result->appendChild(element);
      element.appendChild(doc.createTextNode(QString::number(*thermalReflectance)));
      element.setAttribute("unit", "Fraction");
      element.setAttribute("type", "ExtIR");
      element.setAttribute("surfaceType", "Both");

      element = doc.createElement("Reflectance");
      result->appendChild(element);
      element.appendChild(doc.createTextNode(QString::number(*thermalReflectance)));
      element.setAttribute("unit", "Fraction");
      element.setAttribute("type", "IntIR");
      element.setAttribute("surfaceType", "Both");
    }

    if (solarReflectance){
      QDomElement element = doc.createElement("Reflectance");
      result->appendChild(element);
      element.appendChild(doc.createTextNode(QString::number(*solarReflectance)));
      element.setAttribute("unit", "Fraction");
      element.setAttribute("type", "ExtSolar");
      element.setAttribute("surfaceType", "Both");

      element = doc.createElement("Reflectance");
      result->appendChild(element);
      element.appendChild(doc.createTextNode(QString::number(*solarReflectance)));
      element.setAttribute("unit", "Fraction");
      element.setAttribute("type", "IntSolar");
      element.setAttribute("surfaceType", "Both");
    }

    if (visibleReflectance){
      QDomElement element = doc.createElement("Reflectance");
      result->appendChild(element);
      element.appendChild(doc.createTextNode(QString::number(*visibleReflectance)));
      element.setAttribute("unit", "Fraction");
      element.setAttribute("type", "ExtVisible");
      element.setAttribute("surfaceType", "Both");

      element = doc.createElement("Reflectance");
      result->appendChild(element);
      element.appendChild(doc.createTextNode(QString::number(*visibleReflectance)));
      element.setAttribute("unit", "Fraction");
      element.setAttribute("type", "IntVisible");
      element.setAttribute("surfaceType", "Both");
    }

    if (roughness){
      QDomElement element = doc.createElement("Roughness");
      result->appendChild(element);
      element.setAttribute("value", QString::fromStdString(*roughness));
    }

    if (thickness){
      QDomElement element = doc.createElement("Thickness");
      result->appendChild(element);
      element.appendChild(doc.createTextNode(QString::number(*thickness)));
      element.setAttribute("unit", "Meters");
    }
    
    if (conductivity){
      QDomElement element = doc.createElement("Conductivity");
      result->appendChild(element);
      element.appendChild(doc.createTextNode(QString::number(*conductivity)));
      element.setAttribute("unit", "WPerMeterK");
    }

    if (resistance){
      QDomElement element = doc.createElement("R-value");
      result->appendChild(element);
      element.appendChild(doc.createTextNode(QString::number(*resistance)));
      element.setAttribute("unit", "SquareMeterKPerW");
    }
    
    if (density){
      QDomElement element = doc.createElement("Density");
      result->appendChild(element);
      element.appendChild(doc.createTextNode(QString::number(*density)));
      element.setAttribute("unit", "KgPerCubicM");
    }

    if (specificHeat){
      QDomElement element = doc.createElement("SpecificHeat");
      result->appendChild(element);
      element.appendChild(doc.createTextNode(QString::number(*specificHeat)));
      element.setAttribute("unit", "JPerKgK");
    }

    if (thermalAbsorptance){
      QDomElement element = doc.createElement("Absorptance");
      result->appendChild(element);
      element.appendChild(doc.createTextNode(QString::number(*thermalAbsorptance)));
      element.setAttribute("unit", "Fraction");
      element.setAttribute("type", "ExtIR");

      element = doc.createElement("Absorptance");
      result->appendChild(element);
      element.appendChild(doc.createTextNode(QString::number(*thermalAbsorptance)));
      element.setAttribute("unit", "Fraction");
      element.setAttribute("type", "IntIR");
    }

    if (solarAbsorptance){
      QDomElement element = doc.createElement("Absorptance");
      result->appendChild(element);
      element.appendChild(doc.createTextNode(QString::number(*solarAbsorptance)));
      element.setAttribute("unit", "Fraction");
      element.setAttribute("type", "ExtSolar");
      element = doc.createElement("Absorptance");
      result->appendChild(element);
      element.appendChild(doc.createTextNode(QString::number(*solarAbsorptance)));
      element.setAttribute("unit", "Fraction");
      element.setAttribute("type", "IntSolar");
    }

    if (visibleAbsorptance){
      QDomElement element = doc.createElement("Absorptance");
      result->appendChild(element);
      element.appendChild(doc.createTextNode(QString::number(*visibleAbsorptance)));
      element.setAttribute("unit", "Fraction");
      element.setAttribute("type", "ExtVisible");
      element = doc.createElement("Absorptance");
      result->appendChild(element);
      element.appendChild(doc.createTextNode(QString::number(*visibleAbsorptance)));
      element.setAttribute("unit", "Fraction");
      element.setAttribute("type", "IntVisible");
    }
    return result;
  }
  
} // gbxml
} // openstudio
