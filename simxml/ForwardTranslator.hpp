/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty o f
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#ifndef SIMXML_FORWARDTRANSLATOR_HPP
#define SIMXML_FORWARDTRANSLATOR_HPP

#include "SimXMLAPI.hpp"

#include <model/Model.hpp>

#include <SimXmlDoc.hpp>

#include <utilities/core/Path.hpp>
#include <utilities/core/Optional.hpp>

namespace simbldg {
  class SimBuilding;
  class SimBuildingStory;
  class SimSpatialZone;
  class SimSpace;
  class SimSlab;
  class SimWall;
  class SimWindow;
}
namespace simres {
  class SimSpaceBoundary;
}

namespace openstudio {
  class Point3d;
namespace model {
  class ModelObject;
  class Building;
  class BuildingStory;
  class ThermalZone;
  class Space;
  class Surface;
  class SubSurface;
}
namespace simxml {

  class SIMXML_API ForwardTranslator {
  public:

    static ::simxml::SimXmlDoc modelToSimXml(const openstudio::model::Model& model);

  private:

    ForwardTranslator(const openstudio::model::Model& model);

    ::simxml::SimXmlDoc m_doc;
    openstudio::model::Model m_model;
    std::map<openstudio::model::ModelObject, std::string> m_modelObjectToRefIdMap;

    ::simxml::SimXmlDoc convert();
    boost::optional<std::string> findRefId(const openstudio::model::ModelObject& modelObject) const;
    void registerModelObject(const openstudio::model::ModelObject& modelObject, const std::string& refId);
    simres::SimSpaceBoundary makeSpaceBoundaryForVertices(const std::vector<openstudio::Point3d>& vertices);

    simbldg::SimBuilding translateBuilding(const openstudio::model::Building& building);
    simbldg::SimBuildingStory translateBuildingStory(const openstudio::model::BuildingStory& buildingStory);
    simbldg::SimSpatialZone translateThermalZone(const openstudio::model::ThermalZone& thermalZone);
    simbldg::SimSpace translateSpace(const openstudio::model::Space& space);
    ::simxml::SimXmlElement translateSurface(const openstudio::model::Surface& surface, const simbldg::SimSpace& simSpace);
    ::simxml::SimXmlElement translateSubSurface(const openstudio::model::SubSurface& subSurface, const ::simxml::SimXmlElement& buildingElement, const simbldg::SimSpace& simSpace);
  };

} // simxml
} // openstudio

#endif // SIMXML_FORWARDTRANSLATOR_HPP
