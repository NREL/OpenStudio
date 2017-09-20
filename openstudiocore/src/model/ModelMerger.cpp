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

#include <QThread>

#include <cmath>

namespace openstudio
{
  namespace model
  {

    ModelMerger::ModelMerger()
    {
      m_logSink.setLogLevel(Warn);
      //m_logSink.setChannelRegex(boost::regex("openstudio\\.model\\.ThreeJSReverseTranslator"));
      m_logSink.setThreadId(QThread::currentThread());    
    }

    std::vector<LogMessage> ModelMerger::warnings() const
    {
      std::vector<LogMessage> result;

      for (LogMessage logMessage : m_logSink.logMessages()){
        if (logMessage.logLevel() == Warn){
          result.push_back(logMessage);
        }
      }

      return result;
    }

    std::vector<LogMessage> ModelMerger::errors() const
    {
      std::vector<LogMessage> result;

      for (LogMessage logMessage : m_logSink.logMessages()){
        if (logMessage.logLevel() > Warn){
          result.push_back(logMessage);
        }
      }

      return result;
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
        // DLM: this should probably be moved to a mergeSurface method
        Surface clone = newSurface.clone(m_currentModel).cast<Surface>();
        clone.setSpace(currentSpace);

        m_newMergedHandles.insert(newSurface.handle());
        m_currentToNewHandleMapping[clone.handle()] = newSurface.handle();
        m_newToCurrentHandleMapping[newSurface.handle()] = clone.handle();

        boost::optional<Surface> newAdjacentSurface = newSurface.adjacentSurface();
        if (newAdjacentSurface){
          boost::optional<UUID> currentAdjacentSurfaceHandle = getCurrentModelHandle(newAdjacentSurface->handle());
          if (currentAdjacentSurfaceHandle){
            boost::optional<Surface> currentAdjacentSurface = m_currentModel.getModelObject<Surface>(*currentAdjacentSurfaceHandle);
            if (currentAdjacentSurface){
              clone.setAdjacentSurface(*currentAdjacentSurface);
            }
          }
        }
      }

      // DLM: TODO shadingSurfaceGroups
      // DLM: TODO interiorPartitionSurfaceGroups

      // thermal zone
      if (boost::optional<ThermalZone> newThermalZone = newSpace.thermalZone()){
        boost::optional<WorkspaceObject> currentObject = getCurrentModelObject(*newThermalZone);
        if (currentObject){
          ThermalZone currentThermalZone = currentObject->cast<ThermalZone>();
          currentSpace.setThermalZone(currentThermalZone);
        } else{
          currentSpace.resetThermalZone();
        }
      } else{
        currentSpace.resetThermalZone();
      }

      // space type
      if (boost::optional<SpaceType> newSpaceType = newSpace.spaceType()){
        boost::optional<WorkspaceObject> currentObject = getCurrentModelObject(*newSpaceType);
        if (currentObject){
          SpaceType currentSpaceType = currentObject->cast<SpaceType>();
          currentSpace.setSpaceType(currentSpaceType);
        } else{
          currentSpace.resetSpaceType();
        }
      } else{
        currentSpace.resetSpaceType();
      }

      // building story
      if (boost::optional<BuildingStory> newBuildingStory = newSpace.buildingStory()){
        boost::optional<WorkspaceObject> currentObject = getCurrentModelObject(*newBuildingStory);
        if (currentObject){
          BuildingStory currentBuildingStory = currentObject->cast<BuildingStory>();
          currentSpace.setBuildingStory(currentBuildingStory);
        } else{
          currentSpace.resetBuildingStory();
        }
      } else{
        currentSpace.resetBuildingStory();
      }

      // building unit
      if (boost::optional<BuildingUnit> newBuildingUnit = newSpace.buildingUnit()){
        boost::optional<WorkspaceObject> currentObject = getCurrentModelObject(*newBuildingUnit);
        if (currentObject){
          BuildingUnit currentBuildingUnit = currentObject->cast<BuildingUnit>();
          currentSpace.setBuildingUnit(currentBuildingUnit);
        } else{
          currentSpace.resetBuildingUnit();
        }
      } else{
        currentSpace.resetBuildingUnit();
      }

      // default construction set
      if (boost::optional<DefaultConstructionSet> newDefaultConstructionSet = newSpace.defaultConstructionSet()){
        boost::optional<WorkspaceObject> currentObject = getCurrentModelObject(*newDefaultConstructionSet);
        if (currentObject){
          DefaultConstructionSet currentDefaultConstructionSet = currentObject->cast<DefaultConstructionSet>();
          currentSpace.setDefaultConstructionSet(currentDefaultConstructionSet);
        } else{
          currentSpace.resetDefaultConstructionSet();
        }
      } else{
        currentSpace.resetDefaultConstructionSet();
      }

    }

    void ModelMerger::mergeThermalZone(ThermalZone& currentThermalZone, const ThermalZone& newThermalZone)
    {
      if (m_newMergedHandles.find(newThermalZone.handle()) != m_newMergedHandles.end()){
        // already merged
        return;
      }
      m_newMergedHandles.insert(newThermalZone.handle());

      currentThermalZone.setName(newThermalZone.nameString());

      // DLM: TODO multiplier
      // DLM: TODO ceilingHeight
      // DLM: TODO volume
      // DLM: TODO zoneInsideConvectionAlgorithm
      // DLM: TODO zoneOutsideConvectionAlgorithm
      // DLM: TODO zoneConditioningEquipmentListName ?
      // DLM: TODO thermostat
      // DLM: TODO zoneControlHumidistat
      // DLM: TODO zoneControlContaminantController
      // DLM: TODO sizingZone
    }
  
    void ModelMerger::mergeSpaceType(SpaceType& currentSpaceType, const SpaceType& newSpaceType)
    {
      if (m_newMergedHandles.find(newSpaceType.handle()) != m_newMergedHandles.end()){
        // already merged
        return;
      }
      m_newMergedHandles.insert(newSpaceType.handle());

      currentSpaceType.setName(newSpaceType.nameString());

      //default construction set.
      if (boost::optional<DefaultConstructionSet> newDefaultConstructionSet = newSpaceType.defaultConstructionSet()){
        boost::optional<WorkspaceObject> currentObject = getCurrentModelObject(*newDefaultConstructionSet);
        if (currentObject){
          DefaultConstructionSet currentDefaultConstructionSet = currentObject->cast<DefaultConstructionSet>();
          currentSpaceType.setDefaultConstructionSet(currentDefaultConstructionSet);
        } else{
          currentSpaceType.resetDefaultConstructionSet();
        }
      } else{
        currentSpaceType.resetDefaultConstructionSet();
      }
        
      // DLM: TODO default schedule set

      // DLM: TODO rendering color

      // DLM: TODO standardsBuildingType

      // DLM: TODO standardsSpaceType

      // bring over child loads
      for (const auto& newChild : newSpaceType.children()){
        ModelObject currentChild = newChild.clone(m_currentModel).cast<ModelObject>();
        currentChild.setParent(currentSpaceType);
      }
    }

    void ModelMerger::mergeBuildingStory(BuildingStory& currentBuildingStory, const BuildingStory& newBuildingStory)
    {
      if (m_newMergedHandles.find(newBuildingStory.handle()) != m_newMergedHandles.end()){
        // already merged
        return;
      }
      m_newMergedHandles.insert(newBuildingStory.handle());

      currentBuildingStory.setName(newBuildingStory.nameString());

      // DLM: TODO nominalZCoordinate() const;

      // DLM: TODO nominalFloortoFloorHeight() const;

      // DLM: TODO nominalFloortoCeilingHeight() const;

      // DLM: TODO spaces() const;

      // DLM: TODO defaultConstructionSet() const;

      // DLM: TODO defaultScheduleSet() const;

      // DLM: TODO renderingColor() const;

    }

    void ModelMerger::mergeBuildingUnit(BuildingUnit& currentBuildingUnit, const BuildingUnit& newBuildingUnit)
    {
      if (m_newMergedHandles.find(newBuildingUnit.handle()) != m_newMergedHandles.end()){
        // already merged
        return;
      }
      m_newMergedHandles.insert(newBuildingUnit.handle());

      currentBuildingUnit.setName(newBuildingUnit.nameString());

      // DLM: TODO renderingColor() const;

      // DLM: TODO buildingUnitType() const;

      // DLM: TODO spaces() const;

      // DLM: TODO featureNames() const;
    }

    void ModelMerger::mergeDefaultConstructionSet(DefaultConstructionSet& currentDefaultConstructionSet, const DefaultConstructionSet& newDefaultConstructionSet)
    {
      if (m_newMergedHandles.find(newDefaultConstructionSet.handle()) != m_newMergedHandles.end()){
        // already merged
        return;
      }
      m_newMergedHandles.insert(newDefaultConstructionSet.handle());

      currentDefaultConstructionSet.setName(newDefaultConstructionSet.nameString());

      // DLM: TODO defaultExteriorSurfaceConstructions() const;

      // DLM: TODO defaultInteriorSurfaceConstructions() const;

      // DLM: TODO defaultGroundContactSurfaceConstructions() const;

      // DLM: TODO defaultExteriorSubSurfaceConstructions() const;

      // DLM: TODO defaultInteriorSubSurfaceConstructions() const;

      // DLM: TODO interiorPartitionConstruction() const;

      // DLM: TODO spaceShadingConstruction() const;

      // DLM: TODO buildingShadingConstruction() const;

      // DLM: TODO siteShadingConstruction() const;
    }

    boost::optional<WorkspaceObject> ModelMerger::getCurrentModelObject(const WorkspaceObject& newObject)
    {
      // find object in current model
      IddObjectType iddObjectType = newObject.iddObject().type();
      boost::optional<UUID> currentHandle = getCurrentModelHandle(newObject.handle());
      boost::optional<WorkspaceObject> currentObject;
      if (currentHandle){
        currentObject = m_currentModel.getObject(*currentHandle);
        if (!currentObject){
          LOG(Error, "Could not find object in current model for handle " << *currentHandle);
        }
      }

      // create object in current model if needed
      if (!currentObject){
        switch (iddObjectType.value()){
        case IddObjectType::OS_Space:
          currentObject = model::Space(m_currentModel);
          break;
        case IddObjectType::OS_ThermalZone:
          currentObject = model::ThermalZone(m_currentModel);
          break;
        case IddObjectType::OS_SpaceType:
          currentObject = model::SpaceType(m_currentModel);
          break;
        case IddObjectType::OS_BuildingStory:
          currentObject = model::BuildingStory(m_currentModel);
          break;
        case IddObjectType::OS_BuildingUnit:
          currentObject = model::BuildingUnit(m_currentModel);
          break;
        case IddObjectType::OS_DefaultConstructionSet:
          currentObject = model::DefaultConstructionSet(m_currentModel);
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
      {
        Space currentSpace = currentObject->cast<Space>();
        Space newSpace = newObject.cast<Space>();
        mergeSpace(currentSpace, newSpace);
      }
        break;
      case IddObjectType::OS_ThermalZone:
      {
        ThermalZone currentThermalZone = currentObject->cast<ThermalZone>();
        ThermalZone newThermalZone = newObject.cast<ThermalZone>();
        mergeThermalZone(currentThermalZone, newThermalZone);
      }
        break;
      case IddObjectType::OS_SpaceType:
      {
        SpaceType currentSpaceType = currentObject->cast<SpaceType>();
        SpaceType newSpaceType = newObject.cast<SpaceType>();
        mergeSpaceType(currentSpaceType, newSpaceType);
      }
        break;
      case IddObjectType::OS_BuildingStory:
      {
        BuildingStory currentBuildingStory = currentObject->cast<BuildingStory>();
        BuildingStory newBuildingStory = newObject.cast<BuildingStory>();
        mergeBuildingStory(currentBuildingStory, newBuildingStory);
      }
        break;
      case IddObjectType::OS_BuildingUnit:
      {
        BuildingUnit currentBuildingUnit = currentObject->cast<BuildingUnit>();
        BuildingUnit newBuildingUnit = newObject.cast<BuildingUnit>();
        mergeBuildingUnit(currentBuildingUnit, newBuildingUnit);
      }
        break;
      case IddObjectType::OS_DefaultConstructionSet:
      {
        DefaultConstructionSet currentDefaultConstructionSet = currentObject->cast<DefaultConstructionSet>();
        DefaultConstructionSet newDefaultConstructionSet = newObject.cast<DefaultConstructionSet>();
        mergeDefaultConstructionSet(currentDefaultConstructionSet, newDefaultConstructionSet);
      }
        break;
      default:
        LOG(Error, "No merge function registered for IddObjectType " << iddObjectType.valueName());
      }

      OS_ASSERT(currentObject);
      OS_ASSERT(!currentObject->handle().isNull());
      return *currentObject;
    }

    void ModelMerger::mergeModels(Model& currentModel, const Model& newModel, const std::map<UUID, UUID>& handleMapping)
    {
      m_logSink.setThreadId(QThread::currentThread());
      m_logSink.resetStringStream();

      m_currentModel = currentModel;
      m_newModel = newModel;

      m_newMergedHandles.clear();
      m_currentToNewHandleMapping = handleMapping;
      m_newToCurrentHandleMapping.clear();
      for (const auto& it : handleMapping){
        m_newToCurrentHandleMapping[it.second] = it.first;
      }

      // DLM: TODO expose this to user to give more control over merging?
      m_iddObjectTypesToMerge.push_back(IddObjectType::OS_Space);
      m_iddObjectTypesToMerge.push_back(IddObjectType::OS_ThermalZone);
      m_iddObjectTypesToMerge.push_back(IddObjectType::OS_SpaceType);
      m_iddObjectTypesToMerge.push_back(IddObjectType::OS_BuildingStory);
      m_iddObjectTypesToMerge.push_back(IddObjectType::OS_BuildingUnit);
      m_iddObjectTypesToMerge.push_back(IddObjectType::OS_DefaultConstructionSet);

      //** Remove objects from current model that are not in new model **//
      for (const auto& iddObjectType : m_iddObjectTypesToMerge){
        for (auto& currenObject : currentModel.getObjectsByType(iddObjectType)){
          if (m_currentToNewHandleMapping.find(currenObject.handle()) == m_currentToNewHandleMapping.end()){
            currenObject.remove();
          }
        }
      }

      //** Merge objects from new model into curret model **//
      for (const auto& iddObjectType : m_iddObjectTypesToMerge){
        for (auto& newObject : newModel.getObjectsByType(iddObjectType)){
          getCurrentModelObject(newObject);
        }
      }
    }
    
  }//model
}//openstudio
