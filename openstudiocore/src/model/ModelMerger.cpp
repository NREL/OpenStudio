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

#include <utilities/idd/IddFactory.hxx>

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
          currentThermalZone = m_currentModel.getModelObject<ThermalZone>(*currentHandle);
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
  
    void ModelMerger::mergeSpaceType(SpaceType& currentSpaceType, const SpaceType& newSpaceType)
    {
      if (m_newMergedHandles.find(newSpaceType.handle()) != m_newMergedHandles.end()){
        // already merged
        return;
      }
      m_newMergedHandles.insert(newSpaceType.handle());

      currentSpaceType.setName(newSpaceType.nameString());
    }

    void ModelMerger::mergeBuildingStory(BuildingStory& currentBuildingStory, const BuildingStory& newBuildingStory)
    {
      if (m_newMergedHandles.find(newBuildingStory.handle()) != m_newMergedHandles.end()){
        // already merged
        return;
      }
      m_newMergedHandles.insert(newBuildingStory.handle());

      currentBuildingStory.setName(newBuildingStory.nameString());
    }

    void ModelMerger::mergeBuildingUnit(BuildingUnit& currentBuildingUnit, const BuildingUnit& newBuildingUnit)
    {
      if (m_newMergedHandles.find(newBuildingUnit.handle()) != m_newMergedHandles.end()){
        // already merged
        return;
      }
      m_newMergedHandles.insert(newBuildingUnit.handle());

      currentBuildingUnit.setName(newBuildingUnit.nameString());
    }

    void ModelMerger::mergeDefaultConstructionSet(DefaultConstructionSet& currentDefaultConstructionSet, const DefaultConstructionSet& newDefaultConstructionSet)
    {
      if (m_newMergedHandles.find(newDefaultConstructionSet.handle()) != m_newMergedHandles.end()){
        // already merged
        return;
      }
      m_newMergedHandles.insert(newDefaultConstructionSet.handle());

      currentDefaultConstructionSet.setName(newDefaultConstructionSet.nameString());
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

      // DLM: TODO expose this to user to give more control over merging?
      std::vector<IddObjectType> iddObjectTypesToMerge;
      iddObjectTypesToMerge.push_back(IddObjectType::OS_Space);
      iddObjectTypesToMerge.push_back(IddObjectType::OS_ThermalZone);
      iddObjectTypesToMerge.push_back(IddObjectType::OS_SpaceType);
      iddObjectTypesToMerge.push_back(IddObjectType::OS_BuildingStory);
      iddObjectTypesToMerge.push_back(IddObjectType::OS_BuildingUnit);
      iddObjectTypesToMerge.push_back(IddObjectType::OS_DefaultConstructionSet);

      //** Remove objects from current model that are not in new model **//
      for (const auto& iddObjectType : iddObjectTypesToMerge){
        for (auto& currenObject : currentModel.getObjectsByType(iddObjectType)){
          if (m_currentToNewHandleMapping.find(currenObject.handle()) == m_currentToNewHandleMapping.end()){
            currenObject.remove();
          }
        }
      }

      //** Merge objects from new model into curret model **//
      for (const auto& iddObjectType : iddObjectTypesToMerge){

        for (auto& newObject : newModel.getObjectsByType(iddObjectType)){
         
          // find object in current model
          boost::optional<UUID> currentHandle = getCurrentModelHandle(newObject.handle());
          boost::optional<WorkspaceObject> currentObject;
          if (currentHandle){
            currentObject = currentModel.getObject(*currentHandle);
            if (!currentObject){
              LOG(Error, "Could not find object in current model for handle " << *currentHandle);
            }
          }

          // create object in current model if needed
          if (!currentObject){
            switch (iddObjectType.value()){
            case IddObjectType::OS_Space:
              currentObject = model::Space(currentModel);
              break;
            case IddObjectType::OS_ThermalZone:
              currentObject = model::ThermalZone(currentModel);
              break;
            case IddObjectType::OS_SpaceType:
              currentObject = model::SpaceType(currentModel);
              break;
            case IddObjectType::OS_BuildingStory:
              currentObject = model::BuildingStory(currentModel);
              break;
            case IddObjectType::OS_BuildingUnit:
              currentObject = model::BuildingUnit(currentModel);
              break;
            case IddObjectType::OS_DefaultConstructionSet:
              currentObject = model::DefaultConstructionSet(currentModel);
              break;
            default:
              LOG(Error, "No constructor registered for IddObjectType " << iddObjectType.valueName());
            }
            
            OS_ASSERT(currentObject);
            m_currentToNewHandleMapping[currentObject->handle()] = newObject.handle();
            m_newToCurrentHandleMapping[newObject.handle()] = currentObject->handle();
          }
          
          // merge objects
          switch (iddObjectType.value()){
          case IddObjectType::OS_Space:
            mergeSpace(currentObject->cast<Space>(), newObject.cast<Space>());
            break;
          case IddObjectType::OS_ThermalZone:
            mergeThermalZone(currentObject->cast<ThermalZone>(), newObject.cast<ThermalZone>());
            break;
          case IddObjectType::OS_SpaceType:
            mergeSpaceType(currentObject->cast<SpaceType>(), newObject.cast<SpaceType>());
            break;
          case IddObjectType::OS_BuildingStory:
            mergeBuildingStory(currentObject->cast<BuildingStory>(), newObject.cast<BuildingStory>());
            break;
          case IddObjectType::OS_BuildingUnit:
            mergeBuildingUnit(currentObject->cast<BuildingUnit>(), newObject.cast<BuildingUnit>());
            break;
          case IddObjectType::OS_DefaultConstructionSet:
            mergeDefaultConstructionSet(currentObject->cast<DefaultConstructionSet>(), newObject.cast<DefaultConstructionSet>());
            break;
          default:
            LOG(Error, "No merge function registered for IddObjectType " << iddObjectType.valueName());
          }
          
        }
      }
    }
    
  }//model
}//openstudio
