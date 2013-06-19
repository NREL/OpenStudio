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
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#ifndef SIMXML_REVERSETRANSLATOR_HPP
#define SIMXML_REVERSETRANSLATOR_HPP

#include "SimXMLAPI.hpp"

#include <model/Model.hpp>
#include <model/Construction.hpp>

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

  class SIMXML_API ReverseTranslator {
  public:

    static boost::optional<openstudio::model::Model> SimXmlToModel(const openstudio::path& path);

  private:

    ReverseTranslator(const ::simxml::SimXmlDoc& doc);

    ::simxml::SimXmlDoc m_doc;
    openstudio::model::Model m_model;
    boost::optional<openstudio::model::Construction> m_airWallConstruction;
    std::map<std::string, openstudio::model::ModelObject> m_refIdToModelObjectMap;

    openstudio::model::Model convert();

    boost::optional<openstudio::model::ModelObject> findModelObject(const std::string& refId) const;
    void registerModelObject(const std::string& refId, const openstudio::model::ModelObject& modelObject);
    std::vector<openstudio::Point3d> getSpaceBoundaryVertices(const simres::SimSpaceBoundary& simSpaceBoundary);

    boost::optional<openstudio::model::Building> translateSimBuilding(const simbldg::SimBuilding& simSimBuilding);
    boost::optional<openstudio::model::BuildingStory> translateSimBuildingStory(const simbldg::SimBuildingStory& simSimBuildingStory);
    boost::optional<openstudio::model::ThermalZone> translateSimSpatialZone(const simbldg::SimSpatialZone& simSpatialZone);
    boost::optional<openstudio::model::Space> translateSimSpace(const simbldg::SimSpace& simSpace);
    boost::optional<openstudio::model::Surface> translateSimSlab(const simbldg::SimSlab& simSlab);
    boost::optional<openstudio::model::Surface> translateSimWall(const simbldg::SimWall& simWall);
    boost::optional<openstudio::model::SubSurface> translateSimWindow(const simbldg::SimWindow& simWindow);
  };

} // simxml
} // openstudio

#endif // SIMXML_REVERSETRANSLATOR_HPP
