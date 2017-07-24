/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "ModelMerger.hpp"

#include "RenderingColor.hpp"
#include "ConstructionBase.hpp"
#include "ConstructionBase_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "SpaceType.hpp"
#include "SpaceType_Impl.hpp"
#include "BuildingStory.hpp"
#include "BuildingStory_Impl.hpp"
#include "BuildingUnit.hpp"
#include "BuildingUnit_Impl.hpp"
#include "Surface.hpp"
#include "Surface_Impl.hpp"
#include "SubSurface.hpp"
#include "SubSurface_Impl.hpp"
#include "ShadingSurface.hpp"
#include "ShadingSurface_Impl.hpp"
#include "InteriorPartitionSurface.hpp"
#include "InteriorPartitionSurface_Impl.hpp"
#include "PlanarSurfaceGroup.hpp"
#include "PlanarSurfaceGroup_Impl.hpp"
#include "Space.hpp"
#include "Space_Impl.hpp"
#include "DefaultConstructionSet.hpp"
#include "DefaultConstructionSet_Impl.hpp"
#include "ShadingSurfaceGroup.hpp"
#include "InteriorPartitionSurfaceGroup.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Compare.hpp"
#include "../utilities/geometry/Point3d.hpp"
#include "../utilities/geometry/Plane.hpp"
#include "../utilities/geometry/BoundingBox.hpp"
#include "../utilities/geometry/Transformation.hpp"
#include "../utilities/geometry/Geometry.hpp"

#include <cmath>

namespace openstudio
{
  namespace model
  {

    ModelMerger::ModelMerger()
    {
    }

    boost::optional<UUID> ModelMerger::getNewModelHandle(const UUID& currentHandle)
    {
      auto it = m_currentToNewHandleMapping.find(currentHandle);
      if (it != m_currentToNewHandleMapping.end()){
        return it->second;
      }
      return boost::none;
    }
    
    boost::optional<UUID> ModelMerger::getCurrentModelHandle(const UUID& newHandle)
    {
      auto it = m_newToCurrentHandleMapping.find(newHandle);
      if (it != m_newToCurrentHandleMapping.end()){
        return it->second;
      }
      return boost::none;
    }

    void ModelMerger::mergeSpace(Space& currentSpace, const Space& newSpace)
    {
      if (m_newMergedHandles.find(newSpace.handle()) != m_newMergedHandles.end()){
        // already merged
        return;
      }
      m_newMergedHandles.insert(newSpace.handle());

      currentSpace.setName(newSpace.nameString());

      // remove current surfaces
      for (auto& currentSurface : currentSpace.surfaces()){
        currentSurface.remove();
      }

      // add new surfaces
      for (const auto& newSurface : newSpace.surfaces()){
        Surface clone = newSurface.clone(m_currentModel).cast<Surface>();
        clone.setSpace(currentSpace);
      }

      // thermal zone
      if (boost::optional<ThermalZone> newThermalZone = newSpace.thermalZone()){
        boost::optional<UUID> currentHandle = getCurrentModelHandle(newThermalZone->handle());
        boost::optional<ThermalZone> currentThermalZone;
        if (currentHandle){
          currentThermalZone = m_newModel.getModelObject<ThermalZone>(*currentHandle);
        }
        if (!currentThermalZone){
          currentThermalZone = model::ThermalZone(m_currentModel);
          m_currentToNewHandleMapping[currentThermalZone->handle()] = newThermalZone->handle();
          m_newToCurrentHandleMapping[newThermalZone->handle()] = currentThermalZone->handle();
        }
        OS_ASSERT(currentThermalZone);

        mergeThermalZone(*currentThermalZone, *newThermalZone);

        currentSpace.setThermalZone(*currentThermalZone);
      } else{
        currentSpace.resetThermalZone();
      }
/*
    std::string surfaceTypeMaterialName() const;
    std::string constructionName() const;
    std::string constructionHandle() const;
    std::string constructionMaterialName() const;
    std::string spaceName() const;
    std::string spaceHandle() const;
    std::string thermalZoneName() const;
    std::string thermalZoneHandle() const;
    std::string thermalZoneMaterialName() const;
    std::string spaceTypeName() const;
    std::string spaceTypeHandle() const;
    std::string spaceTypeMaterialName() const;
    std::string buildingStoryName() const;
    std::string buildingStoryHandle() const;
    std::string buildingStoryMaterialName() const;
    std::string buildingUnitName() const;
    std::string buildingUnitHandle() const;
    std::string buildingUnitMaterialName() const;
    std::string constructionSetName() const;
    std::string constructionSetHandle() const;
    std::string constructionSetMaterialName() const;
    std::string outsideBoundaryCondition() const;
    std::string outsideBoundaryConditionObjectName() const;
    std::string outsideBoundaryConditionObjectHandle() const;
    std::string boundaryMaterialName() const;
*/

    }

    void ModelMerger::mergeThermalZone(ThermalZone& currentThermalZone, const ThermalZone& newThermalZone)
    {
      if (m_newMergedHandles.find(newThermalZone.handle()) != m_newMergedHandles.end()){
        // already merged
        return;
      }
      m_newMergedHandles.insert(newThermalZone.handle());

      currentThermalZone.setName(newThermalZone.nameString());
    }
    
    void ModelMerger::mergeModels(Model& currentModel, const Model& newModel, const std::map<UUID, UUID>& handleMapping)
    {
      m_currentModel = currentModel;
      m_newModel = newModel;

      m_newMergedHandles.clear();
      m_currentToNewHandleMapping = handleMapping;
      m_newToCurrentHandleMapping.clear();
      for (const auto& it : handleMapping){
        m_newToCurrentHandleMapping[it.second] = it.first;
      }

      //** Remove objects from current model that are not in new model **//
      
      // Remove spaces
      for (auto& currentSpace : currentModel.getConcreteModelObjects<model::Space>()){
        if (m_currentToNewHandleMapping.find(currentSpace.handle()) == m_currentToNewHandleMapping.end()){
          currentSpace.remove();
        }
      }

      // Remove thermal zones
      for (auto& currentThermalZone : currentModel.getConcreteModelObjects<model::ThermalZone>()){
        if (m_currentToNewHandleMapping.find(currentThermalZone.handle()) == m_currentToNewHandleMapping.end()){
          currentThermalZone.remove();
        }
      }

      //** Merge objects from new model into curret model **//

      // merge spaces 
      for (auto& newSpace : newModel.getConcreteModelObjects<model::Space>()){
        boost::optional<UUID> currentHandle = getCurrentModelHandle(newSpace.handle());
        boost::optional<Space> currentSpace;
        if (currentHandle){
          currentSpace = currentModel.getModelObject<model::Space>(*currentHandle);
        }
        if (!currentSpace){
          currentSpace = model::Space(currentModel);
          m_currentToNewHandleMapping[currentSpace->handle()] = newSpace.handle();
          m_newToCurrentHandleMapping[newSpace.handle()] = currentSpace->handle();
        }
        OS_ASSERT(currentSpace);

        mergeSpace(*currentSpace, newSpace);
      }

      // merge thermal zones 
      for (auto& newThermalZone : newModel.getConcreteModelObjects<model::ThermalZone>()){
        boost::optional<UUID> currentHandle = getCurrentModelHandle(newThermalZone.handle());
        boost::optional<ThermalZone> currentThermalZone;
        if (currentHandle){
          currentThermalZone = currentModel.getModelObject<model::ThermalZone>(*currentHandle);
        }
        if (!currentThermalZone){
          currentThermalZone = model::ThermalZone(currentModel);
          m_currentToNewHandleMapping[currentThermalZone->handle()] = newThermalZone.handle();
          m_newToCurrentHandleMapping[newThermalZone.handle()] = currentThermalZone->handle();
        }
        OS_ASSERT(currentThermalZone);

        mergeThermalZone(*currentThermalZone, newThermalZone);
      }
    }
    
  }//model
}//openstudio
