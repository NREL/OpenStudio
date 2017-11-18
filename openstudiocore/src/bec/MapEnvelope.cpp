/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#include <QDomDocument>
#include <QDomElement>

namespace openstudio {
namespace bec {
  
    boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateConstruction(const QDomElement& element, const QDomNodeList& layerElements, const QDomDocument& doc, openstudio::model::Model& model) {
          // Krishnan, this constructor should only be used for unique objects like Building and Site
          //openstudio::model::Construction construction = model.getUniqueModelObject<openstudio::model::Construction>();

          openstudio::model::Construction construction(model);
          QString constructionId = element.attribute("id");
          m_idToObjectMap.insert(std::make_pair(constructionId, construction));

          QString constructionName = element.firstChildElement("Name").toElement().text();
          construction.setName(escapeName(constructionId, constructionName));

          QDomNodeList layerIdList = element.elementsByTagName("LayerId");

          if (layerIdList.isEmpty()) {
            return construction;
        }
        
          QString layerId = layerIdList.at(0).toElement().attribute("layerIdRef");

          std::vector<openstudio::model::Material> materials;
          for (int i = 0; i < layerElements.count(); i++) {
            QDomElement layerElement = layerElements.at(i).toElement();
            if (layerId == layerElement.attribute("id")) {
              QDomNodeList materialIdElements = layerElement.elementsByTagName("MaterialId");
              for (int j = 0; j < materialIdElements.count(); j++) {
                QString materialId = materialIdElements.at(j).toElement().attribute("materialIdRef");
                auto materialIt = m_idToObjectMap.find(materialId);
                if (materialIt != m_idToObjectMap.end()) {
                  boost::optional<openstudio::model::Material> material = materialIt->second.optionalCast<openstudio::model::Material>();
                  OS_ASSERT(material); // Krishnan, what type of error handling do you want?
                  materials.push_back(*material);
                }
              }
              break;
            }
          }


          // now assign all layers to real material, does gbXML have same layer order convention as E+?
          for (unsigned i = 0; i < materials.size(); ++i) {
            bool test = false;

            if (materials[i].optionalCast<openstudio::model::OpaqueMaterial>()) {
              test = construction.insertLayer(i, materials[i].cast<openstudio::model::OpaqueMaterial>());
            } else if (materials[i].optionalCast<openstudio::model::FenestrationMaterial>()) {
              test = construction.insertLayer(i, materials[i].cast<openstudio::model::FenestrationMaterial>());
            } else if (materials[i].optionalCast<openstudio::model::ModelPartitionMaterial>()) {
              test = construction.setLayer(materials[i].cast<openstudio::model::ModelPartitionMaterial>());
            }

            OS_ASSERT(test); // Krishnan, what type of error handling do you want?
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

      if (!(densityElements.isEmpty() || conductivityElements.isEmpty() || thicknessElements.isEmpty() || specificHeatElements.isEmpty())) {

        double density = densityElements.at(0).toElement().text().toDouble();
        double conductivity = conductivityElements.at(0).toElement().text().toDouble();
        double thickness = thicknessElements.at(0).toElement().text().toDouble();
        double specificHeat = specificHeatElements.at(0).toElement().text().toDouble();

        std::string roughness = "MediumRough";
        if (!roughnessElements.isEmpty()) {
          roughness = roughnessElements.at(0).toElement().text().toStdString();
        }

        openstudio::model::StandardOpaqueMaterial material(model);
        result = material;

        QString id = element.attribute("id");
        m_idToObjectMap.insert(std::make_pair(id, material));

        QString name = element.firstChildElement("Name").toElement().text();
        material.setName(escapeName(id, name));

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

        QString id = element.attribute("id");
        m_idToObjectMap.insert(std::make_pair(id, material));

        QString name = element.firstChildElement("Name").toElement().text();
        material.setName(escapeName(id, name));

        material.setThermalResistance(rvalue);

      } else {

        // make a stub material
        openstudio::model::MasslessOpaqueMaterial material(model);
        result = material;

        QString id = element.attribute("id");
        m_idToObjectMap.insert(std::make_pair(id, material));

        QString name = element.firstChildElement("Name").toElement().text();
        material.setName(escapeName(id, name));

        LOG(Warn, "Creating stub material '" << name.toStdString() << "'");

        material.setThermalResistance(.001);
      }

      return result;
    }

    
} // bec
} // openstudio
