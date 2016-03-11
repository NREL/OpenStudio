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

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/ShadingSurface.hpp"
#include "../../model/ShadingSurface_Impl.hpp"
#include "../../model/ShadingSurfaceGroup.hpp"
#include "../../model/ShadingSurfaceGroup_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/Surface.hpp"
#include "../../model/Surface_Impl.hpp"
#include "../../model/Building.hpp"
#include "../../model/Building_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Construction.hpp"
#include "../../model/Construction_Impl.hpp"
#include "../../model/StandardOpaqueMaterial.hpp"
#include "../../model/StandardOpaqueMaterial_Impl.hpp"
#include "../../model/MasslessOpaqueMaterial.hpp"
#include "../../model/MasslessOpaqueMaterial_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/geometry/Transformation.hpp"
#include "../../utilities/geometry/Geometry.hpp"

#include <utilities/idd/Shading_Site_Detailed_FieldEnums.hxx>
#include <utilities/idd/Shading_Building_Detailed_FieldEnums.hxx>
#include <utilities/idd/Shading_Zone_Detailed_FieldEnums.hxx>
#include <utilities/idd/ShadingProperty_Reflectance_FieldEnums.hxx>

#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateShadingSurface( model::ShadingSurface & modelObject )
{
  boost::optional<IdfObject> idfObject;

  boost::optional<Schedule> transmittanceSchedule = modelObject.transmittanceSchedule();
  Transformation transformation;
  Point3dVector points;

  boost::optional<ShadingSurfaceGroup> shadingSurfaceGroup = modelObject.shadingSurfaceGroup();
  if (shadingSurfaceGroup){

    transformation = shadingSurfaceGroup->transformation();
    points = transformation * modelObject.vertices();

    if (istringEqual("Space", shadingSurfaceGroup->shadingSurfaceType())){

      idfObject = IdfObject(openstudio::IddObjectType::Shading_Zone_Detailed);
      idfObject->setString(Shading_Zone_DetailedFields::Name, modelObject.name().get());

      boost::optional<Space> space = shadingSurfaceGroup->space();
      if (space){
        
        boost::optional<Surface> baseSurface;
        double minDistance = std::numeric_limits<double>::max();

        // at this point zone has one space and internal surfaces have already been combined
        for (const Surface& surface : space->surfaces()){
          if (istringEqual(surface.outsideBoundaryCondition(), "Outdoors")){
            Point3dVector surfaceVertices = surface.vertices();
            for (const Point3d& point : points){
              for (const Point3d& surfaceVertex : surfaceVertices){
                double distance = getDistance(point, surfaceVertex);
                if (distance < minDistance){
                  baseSurface = surface;
                  minDistance = distance;
                }
              }
            }
          }
        }

        if (!baseSurface){
          LOG(Error, "Cannot find appropriate base surface for shading surface '" << modelObject.name().get() << 
                     "', the shading surface will not be translated");
          return boost::none;
        }

        idfObject->setString(Shading_Zone_DetailedFields::BaseSurfaceName, baseSurface->name().get());
      }

      if (transmittanceSchedule){
        idfObject->setString(Shading_Zone_DetailedFields::TransmittanceScheduleName, transmittanceSchedule->name().get());
      }

    }else if (istringEqual("Site", shadingSurfaceGroup->shadingSurfaceType())){

      idfObject = IdfObject(openstudio::IddObjectType::Shading_Site_Detailed);
      idfObject->setString(Shading_Site_DetailedFields::Name, modelObject.name().get());
      
      if (transmittanceSchedule){
        idfObject->setString(Shading_Site_DetailedFields::TransmittanceScheduleName, transmittanceSchedule->name().get());
      }

    }else{
      boost::optional<Building> building = modelObject.model().getUniqueModelObject<Building>();
      if (building){
        transformation = building->transformation().inverse()*transformation;
      }

      idfObject = IdfObject(openstudio::IddObjectType::Shading_Building_Detailed);
      idfObject->setString(Shading_Building_DetailedFields::Name, modelObject.name().get());
      
      if (transmittanceSchedule){
        idfObject->setString(Shading_Building_DetailedFields::TransmittanceScheduleName, transmittanceSchedule->name().get());
      }
    }

  }else{
    idfObject = IdfObject(openstudio::IddObjectType::Shading_Building_Detailed);
    idfObject->setString(Shading_Building_DetailedFields::Name, modelObject.name().get());
    
    if (transmittanceSchedule){
      idfObject->setString(Shading_Building_DetailedFields::TransmittanceScheduleName, transmittanceSchedule->name().get());
    }
  }

  m_idfObjects.push_back(*idfObject);

  idfObject->clearExtensibleGroups();

  for (const Point3d& point : points){
    IdfExtensibleGroup group = idfObject->pushExtensibleGroup();
    OS_ASSERT(group.numFields() == 3);
    group.setDouble(0, point.x());
    group.setDouble(1, point.y());
    group.setDouble(2, point.z());
  }

  // get reflectance properties from construction if possible
  bool addShadingPropertyObject = false;

  IdfObject shadingPropertyObject = IdfObject(openstudio::IddObjectType::ShadingProperty_Reflectance);
  shadingPropertyObject.setString(ShadingProperty_ReflectanceFields::ShadingSurfaceName, modelObject.name().get());

  boost::optional<model::ConstructionBase> constructionBase = modelObject.construction();
  if (constructionBase){
    if (constructionBase->isFenestration()){

      shadingPropertyObject.setDouble(ShadingProperty_ReflectanceFields::FractionofShadingSurfaceThatIsGlazed, 1.0);
      shadingPropertyObject.setString(ShadingProperty_ReflectanceFields::GlazingConstructionName, constructionBase->name().get());
      addShadingPropertyObject = true;

    }else{

      shadingPropertyObject.setDouble(ShadingProperty_ReflectanceFields::FractionofShadingSurfaceThatIsGlazed, 0.0);

      boost::optional<model::Construction> construction = constructionBase->optionalCast<model::Construction>();
      if (construction){

        std::vector<model::Material> layers = construction->layers();

        // we want the outer layer
        if (!layers.empty()){

          if (layers[0].optionalCast<model::StandardOpaqueMaterial>()){
            model::StandardOpaqueMaterial outerMaterial = layers[0].cast<model::StandardOpaqueMaterial>();

            boost::optional<double> solRefl = outerMaterial.solarReflectance();
            if (solRefl){
              shadingPropertyObject.setDouble(ShadingProperty_ReflectanceFields::DiffuseSolarReflectanceofUnglazedPartofShadingSurface, *solRefl);
              addShadingPropertyObject = true;
            }

            boost::optional<double> visRefl = outerMaterial.visibleReflectance();
            if (visRefl){
              shadingPropertyObject.setDouble(ShadingProperty_ReflectanceFields::DiffuseVisibleReflectanceofUnglazedPartofShadingSurface, *visRefl);
              addShadingPropertyObject = true;
            }
          }

          
          if (layers[0].optionalCast<model::MasslessOpaqueMaterial>()){
            model::MasslessOpaqueMaterial outerMaterial = layers[0].cast<model::MasslessOpaqueMaterial>();

            boost::optional<double> solRefl = outerMaterial.solarReflectance();
            if (solRefl){
              shadingPropertyObject.setDouble(ShadingProperty_ReflectanceFields::DiffuseSolarReflectanceofUnglazedPartofShadingSurface, *solRefl);
              addShadingPropertyObject = true;
            }

            boost::optional<double> visRefl = outerMaterial.visibleReflectance();
            if (visRefl){
              shadingPropertyObject.setDouble(ShadingProperty_ReflectanceFields::DiffuseVisibleReflectanceofUnglazedPartofShadingSurface, *visRefl);
              addShadingPropertyObject = true;
            }
          }

        }
      }
    }
  }

  if (addShadingPropertyObject){
    m_idfObjects.push_back(shadingPropertyObject);
  }

  return idfObject;
}

} // energyplus

} // openstudio

