/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#include "ForwardTranslator.hpp"

#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/ModelObject.hpp"
#include "../model/ModelObject_Impl.hpp"
#include "../model/Material.hpp"
#include "../model/Material_Impl.hpp"
#include "../model/ConstructionBase.hpp"
#include "../model/ConstructionBase_Impl.hpp"
#include "../model/Site.hpp"
#include "../model/Site_Impl.hpp"
#include "../model/Facility.hpp"
#include "../model/Facility_Impl.hpp"
#include "../model/Building.hpp"
#include "../model/Building_Impl.hpp"
#include "../model/ThermalZone.hpp"
#include "../model/ThermalZone_Impl.hpp"
#include "../model/Surface.hpp"
#include "../model/Surface_Impl.hpp"
#include "../model/SubSurface.hpp"
#include "../model/SubSurface_Impl.hpp"
#include "../model/ShadingSurface.hpp"
#include "../model/ShadingSurface_Impl.hpp"
#include "../model/ShadingSurfaceGroup.hpp"
#include "../model/ShadingSurfaceGroup_Impl.hpp"

#include "../utilities/plot/ProgressBar.hpp"
#include "../utilities/core/Assert.hpp"

#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QThread>

namespace openstudio {
namespace sdd {

  ForwardTranslator::ForwardTranslator()
  {
    m_logSink.setLogLevel(Warn);
    m_logSink.setChannelRegex(boost::regex("openstudio\\.sdd\\.ForwardTranslator"));
    m_logSink.setThreadId(QThread::currentThread());
  }

  ForwardTranslator::~ForwardTranslator()
  {
  }

  bool ForwardTranslator::modelToSDD(const openstudio::model::Model& model, const openstudio::path& path, ProgressBar* progressBar)
  {
    m_progressBar = progressBar;

    m_logSink.setThreadId(QThread::currentThread());

    m_logSink.resetStringStream();

    model::Model modelCopy = model.clone().cast<model::Model>();

    // remove unused resource objects
    modelCopy.purgeUnusedResourceObjects();

    boost::optional<QDomDocument> doc = this->translateModel(modelCopy);
    if (!doc){
      return false;
    }

    if (exists(path)){
      remove(path);
    }

    if (!exists(path.parent_path())){
      create_directory(path.parent_path());
    }

    QFile file(toQString(path));
    if (file.open(QFile::WriteOnly)){
      QTextStream textStream(&file);
      textStream << doc->toString(2);
      file.close();
      return true;
    }

    return false;
  }

  std::vector<LogMessage> ForwardTranslator::warnings() const
  {
    std::vector<LogMessage> result;

    for (LogMessage logMessage : m_logSink.logMessages()){
      if (logMessage.logLevel() == Warn){
        result.push_back(logMessage);
      }
    }

    return result;
  }

  std::vector<LogMessage> ForwardTranslator::errors() const
  {
    std::vector<LogMessage> result;

    for (LogMessage logMessage : m_logSink.logMessages()){
      if (logMessage.logLevel() > Warn){
        result.push_back(logMessage);
      }
    }

    return result;
  }

  QString ForwardTranslator::escapeName(const std::string& name)
  {
    return toQString(name);
  }

  boost::optional<QDomDocument> ForwardTranslator::translateModel(const openstudio::model::Model& model)
  {
    QDomDocument doc;
    doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"");

    QDomElement sddElement = doc.createElement("SDDXML");
    sddElement.setAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
    sddElement.setAttribute("xmlns:xsd", "http://www.w3.org/2001/XMLSchema");
    doc.appendChild(sddElement);

    // set ruleset, where should this data come from?
    QDomElement rulesetFilenameElement = doc.createElement("RulesetFilename");
    sddElement.appendChild(rulesetFilenameElement);
    rulesetFilenameElement.setAttribute("file", "unknown");

    // set project, where should this data come from?
    QDomElement projectElement = doc.createElement("Proj");
    sddElement.appendChild(projectElement);

    QDomElement projectNameElement = doc.createElement("Name");
    projectElement.appendChild(projectNameElement);
    projectNameElement.appendChild( doc.createTextNode( "unknown"));

    // site data
    QDomElement projectClimateZoneElement = doc.createElement("CliZn");
    projectElement.appendChild(projectClimateZoneElement);
    projectClimateZoneElement.appendChild( doc.createTextNode( "unknown"));

    // set lat, lon, elev
    // DLM: do not translate forward,  Issue 242: Forward Translator - Remove Proj:Lat/Lon/Elevation translation
    /*
    boost::optional<model::Site> site = model.getOptionalUniqueModelObject<model::Site>();
    if (site){
      double latitude = site->latitude();
      QDomElement latElement = doc.createElement("Lat");
      projectElement.appendChild(latElement);
      latElement.appendChild( doc.createTextNode(QString::number(latitude)));

      double longitude = site->longitude();
      QDomElement longElement = doc.createElement("Long");
      projectElement.appendChild(longElement);
      longElement.appendChild( doc.createTextNode(QString::number(longitude)));

      double elevationSI = site->elevation();
      double elevationIP = elevationSI/0.3048;
      QDomElement elevationElement = doc.createElement("Elevation");
      projectElement.appendChild(elevationElement);
      elevationElement.appendChild( doc.createTextNode(QString::number(elevationIP)));
    }
    */

    // todo: write out epw file path
    // todo: write out ddy file and set path
    //<DDWeatherFile>C:/svn-CBECC-Com-130521/CBECC-Com13/Data/EPW/SACRAMENTO-EXECUTIVE_724830_CZ2010.ddy</DDWeatherFile>
    //<AnnualWeatherFile>C:/svn-CBECC-Com-130521/CBECC-Com13/Data/EPW/SACRAMENTO-EXECUTIVE_724830_CZ2010.epw</AnnualWeatherFile>
    //<HVACAutoSizing>1</HVACAutoSizing>
    //<SimDsgnDays>1</SimDsgnDays>
    //<RunPeriodBeginMonth>0</RunPeriodBeginMonth>
    //<RunPeriodBeginDay>0</RunPeriodBeginDay>
    //<RunPeriodEndMonth>0</RunPeriodEndMonth>
    //<RunPeriodEndDay>0</RunPeriodEndDay>
    //<RunPeriodYear>0</RunPeriodYear>

    // do materials before constructions 
    std::vector<model::Material> materials = model.getModelObjects<model::Material>();
    std::sort(materials.begin(), materials.end(), WorkspaceObjectNameLess());

    if (m_progressBar){
      m_progressBar->setWindowTitle(toString("Translating Materials"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum(materials.size());
      m_progressBar->setValue(0);
    }

    for (const model::Material& material : materials){

      boost::optional<QDomElement> materialElement = translateMaterial(material, doc);
      if (materialElement){
        projectElement.appendChild(*materialElement);
      }

      if (m_progressBar){
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }
  
    // do constructions before geometry

    std::vector<model::ConstructionBase> constructions = model.getModelObjects<model::ConstructionBase>();
    std::sort(constructions.begin(), constructions.end(), WorkspaceObjectNameLess());

    if (m_progressBar){
      m_progressBar->setWindowTitle(toString("Translating Constructions"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum(3*constructions.size()); // three loops below
      m_progressBar->setValue(0);
    }

    std::set<Handle> surfaceConstructions;
    for (const model::Surface& surface : model.getConcreteModelObjects<model::Surface>()){
      boost::optional<model::ConstructionBase> construction = surface.construction();
      if (construction){
        surfaceConstructions.insert(construction->handle());
      }
    }

    std::set<Handle> doorConstructions;
    std::set<Handle> fenestrationConstructions;
    for (const model::SubSurface& subSurface : model.getConcreteModelObjects<model::SubSurface>()){
      boost::optional<model::ConstructionBase> construction = subSurface.construction();
      if (construction){
        std::string subSurfaceType = subSurface.subSurfaceType();
        if (istringEqual("Door", subSurfaceType) || istringEqual("OverheadDoor", subSurfaceType)){
          doorConstructions.insert(construction->handle());
        }else{
          fenestrationConstructions.insert(construction->handle());
        }
      }
    }

    // translate surface constructions
    for (const model::ConstructionBase& constructionBase : constructions){
      if (surfaceConstructions.find(constructionBase.handle()) == surfaceConstructions.end()){
        continue;
      }

      boost::optional<QDomElement> constructionElement = translateConstructionBase(constructionBase, doc);
      if (constructionElement){
        projectElement.appendChild(*constructionElement);
      }
            
      if (m_progressBar){
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    // translate door constructions
    for (const model::ConstructionBase& constructionBase : constructions){
      if (doorConstructions.find(constructionBase.handle()) == doorConstructions.end()){
        continue;
      }

      boost::optional<QDomElement> constructionElement = translateDoorConstruction(constructionBase, doc);
      if (constructionElement){
        projectElement.appendChild(*constructionElement);
      }
            
      if (m_progressBar){
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    // translate fenestration constructions
    for (const model::ConstructionBase& constructionBase : constructions){
      if (fenestrationConstructions.find(constructionBase.handle()) == fenestrationConstructions.end()){
        continue;
      }

      boost::optional<QDomElement> constructionElement = translateFenestrationConstruction(constructionBase, doc);
      if (constructionElement){
        projectElement.appendChild(*constructionElement);
      }

      if (m_progressBar){
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    // translate site shading
    std::vector<model::ShadingSurfaceGroup> shadingSurfaceGroups = model.getConcreteModelObjects<model::ShadingSurfaceGroup>();
    std::sort(shadingSurfaceGroups.begin(), shadingSurfaceGroups.end(), WorkspaceObjectNameLess());

    if (m_progressBar){
      m_progressBar->setWindowTitle(toString("Translating Site Shading"));
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum(shadingSurfaceGroups.size()); 
      m_progressBar->setValue(0);
    }

    for (const model::ShadingSurfaceGroup& shadingSurfaceGroup : shadingSurfaceGroups){
      if (istringEqual(shadingSurfaceGroup.shadingSurfaceType(), "Site")){

        Transformation transformation = shadingSurfaceGroup.siteTransformation();

        for (const model::ShadingSurface& shadingSurface : shadingSurfaceGroup.shadingSurfaces()){
          boost::optional<QDomElement> shadingSurfaceElement = translateShadingSurface(shadingSurface, transformation, doc);
          if (shadingSurfaceElement){
            projectElement.appendChild(*shadingSurfaceElement);
          }
        }
      }

      if (m_progressBar){
        m_progressBar->setValue(m_progressBar->value() + 1);
      }
    }

    // translate the building
    boost::optional<model::Building> building = model.getOptionalUniqueModelObject<model::Building>();
    if (building){
      boost::optional<QDomElement> buildingElement = translateBuilding(*building, doc);
      if (buildingElement){
        projectElement.appendChild(*buildingElement);
      }
    }

    return doc;
  }

} // sdd
} // openstudio
