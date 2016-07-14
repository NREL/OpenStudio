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

    if (constructionBase.isOpaque()){
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
  
    return result;
  }
    
} // gbxml
} // openstudio
