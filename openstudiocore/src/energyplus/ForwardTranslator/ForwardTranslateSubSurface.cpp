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

#include <energyplus/ForwardTranslator.hpp>

#include <model/Model.hpp>
#include <model/SubSurface.hpp>
#include <model/SubSurface_Impl.hpp>
#include <model/Surface.hpp>
#include <model/Surface_Impl.hpp>
#include <model/ConstructionBase.hpp>
#include <model/ConstructionBase_Impl.hpp>
#include <model/ShadingControl.hpp>
#include <model/ShadingControl_Impl.hpp>

#include <utilities/idf/IdfExtensibleGroup.hpp>

#include <utilities/idd/FenestrationSurface_Detailed_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateSubSurface( model::SubSurface & modelObject )
{
  IdfObject idfObject(openstudio::IddObjectType::FenestrationSurface_Detailed);
  
  idfObject.setString(FenestrationSurface_DetailedFields::Name, modelObject.name().get());

  std::string subSurfaceType = modelObject.subSurfaceType();
  if (istringEqual("FixedWindow", subSurfaceType)){
    subSurfaceType = "Window";
  }else if (istringEqual("OperableWindow", subSurfaceType)){
    subSurfaceType = "Window";
  }else if (istringEqual("OverheadDoor", subSurfaceType)){
    subSurfaceType = "Door";
  }else if (istringEqual("Skylight", subSurfaceType)){
    subSurfaceType = "Window";
  }
  
  idfObject.setString(FenestrationSurface_DetailedFields::SurfaceType, subSurfaceType);

  boost::optional<ConstructionBase> construction = modelObject.construction();
  if (construction){
    idfObject.setString(FenestrationSurface_DetailedFields::ConstructionName, construction->name().get());
  }

  boost::optional<Surface> surface = modelObject.surface();
  if (surface){
    idfObject.setString(FenestrationSurface_DetailedFields::BuildingSurfaceName, surface->name().get());
  }

  boost::optional<SubSurface> adjacentSubSurface = modelObject.adjacentSubSurface();
  if (adjacentSubSurface){
    idfObject.setString(FenestrationSurface_DetailedFields::OutsideBoundaryConditionObject, adjacentSubSurface->name().get());
  }

  boost::optional<double> viewFactortoGround = modelObject.viewFactortoGround();
  if (viewFactortoGround){
    idfObject.setDouble(FenestrationSurface_DetailedFields::ViewFactortoGround, *viewFactortoGround);
  }

  boost::optional<ShadingControl> shadingControl = modelObject.shadingControl();
  if (shadingControl){
    idfObject.setString(FenestrationSurface_DetailedFields::ShadingControlName, shadingControl->name().get());
  }

  // TODO: \field Frame and Divider Name

  if(!modelObject.isMultiplierDefaulted()){
    idfObject.setDouble(FenestrationSurface_DetailedFields::Multiplier, modelObject.multiplier());
  }

  idfObject.clearExtensibleGroups();
  BOOST_FOREACH(const Point3d& point, modelObject.vertices()){
    IdfExtensibleGroup group = idfObject.pushExtensibleGroup();
    if (group.empty()) {
      LOG(Error,"Currently unable to translate " << modelObject.briefDescription() 
          << ", because it has more vertices than allowed by EnergyPlus.");
      return boost::none;
    }
    group.setDouble(0, point.x());
    group.setDouble(1, point.y());
    group.setDouble(2, point.z());
  }
  
  m_idfObjects.push_back(idfObject);

  return idfObject;

}

} // energyplus

} // openstudio

