/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

