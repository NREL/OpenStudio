/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "EnergyPlusAPI.hpp"

#include <src/energyplus/embedded_files.hxx>

#include "ForwardTranslator.hpp"

#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/Surface.hpp"
#include "../model/Surface_Impl.hpp"
#include "../model/Construction.hpp"
#include "../model/Construction_Impl.hpp"
#include "../model/ConstructionWithInternalSource.hpp"
#include "../model/ConstructionWithInternalSource_Impl.hpp"
#include "../model/WaterUseEquipment.hpp"
#include "../model/WaterUseEquipment_Impl.hpp"
#include "../model/RunPeriod.hpp"
#include "../model/RunPeriod_Impl.hpp"
#include "../model/RunPeriodControlSpecialDays.hpp"
#include "../model/RunPeriodControlSpecialDays_Impl.hpp"
#include "../model/SimulationControl.hpp"
#include "../model/SimulationControl_Impl.hpp"
#include "../model/Building.hpp"
#include "../model/Building_Impl.hpp"
#include "../model/UtilityBill.hpp"
#include "../model/UtilityBill_Impl.hpp"
#include "../model/ElectricLoadCenterDistribution.hpp"
#include "../model/ElectricLoadCenterDistribution_Impl.hpp"
#include "../model/ShadingControl.hpp"
#include "../model/ShadingControl_Impl.hpp"
#include "../model/AdditionalProperties.hpp"
#include "../model/ConcreteModelObjects.hpp"
#include "../model/SpaceLoad.hpp"
#include "../model/SpaceLoad_Impl.hpp"
#include "../model/SpaceType.hpp"
#include "../model/SpaceInfiltrationDesignFlowRate.hpp"
#include "../model/SpaceInfiltrationDesignFlowRate_Impl.hpp"
#include "../model/SpaceInfiltrationEffectiveLeakageArea.hpp"
#include "../model/SpaceInfiltrationEffectiveLeakageArea_Impl.hpp"
#include "../model/SpaceInfiltrationFlowCoefficient.hpp"
#include "../model/SpaceInfiltrationFlowCoefficient_Impl.hpp"
#include "../model/ElectricEquipmentITEAirCooled.hpp"
#include "../model/ElectricEquipmentITEAirCooled_Impl.hpp"
#include "../model/OutputControlTableStyle.hpp"
#include "../model/OutputControlTableStyle_Impl.hpp"
#include "../model/OutputSQLite.hpp"
#include "../model/OutputSQLite_Impl.hpp"

#include "../utilities/idf/Workspace.hpp"
#include "../utilities/idf/IdfExtensibleGroup.hpp"
#include "../utilities/idf/IdfFile.hpp"
#include "../utilities/idf/WorkspaceObjectOrder.hpp"
#include "../utilities/core/Logger.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/core/FilesystemHelpers.hpp"
#include "../utilities/geometry/BoundingBox.hpp"
#include "../utilities/time/Time.hpp"
#include "../utilities/plot/ProgressBar.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/FluidProperties_Name_FieldEnums.hxx>
#include <utilities/idd/FluidProperties_GlycolConcentration_FieldEnums.hxx>
#include <utilities/idd/GlobalGeometryRules_FieldEnums.hxx>
#include <utilities/idd/Output_Table_SummaryReports_FieldEnums.hxx>
#include <utilities/idd/OutputControl_Table_Style_FieldEnums.hxx>
#include <utilities/idd/Output_VariableDictionary_FieldEnums.hxx>
#include <utilities/idd/Output_SQLite_FieldEnums.hxx>
#include <utilities/idd/LifeCycleCost_NonrecurringCost_FieldEnums.hxx>
#include <utilities/idd/SetpointManager_MixedAir_FieldEnums.hxx>

#include "../utilities/idd/IddEnums.hpp"

#include "../utilities/core/Deprecated.hpp"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <thread>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  ForwardTranslator::ForwardTranslator() : m_progressBar(nullptr) {
    m_logSink.setLogLevel(Warn);
    m_logSink.setChannelRegex(boost::regex("openstudio\\.energyplus\\.ForwardTranslator"));
    m_logSink.setThreadId(std::this_thread::get_id());
    createFluidPropertiesMap();
  }

  ForwardTranslatorOptions ForwardTranslator::forwardTranslatorOptions() const {
    return m_forwardTranslatorOptions;
  }

  void ForwardTranslator::setForwardTranslatorOptions(ForwardTranslatorOptions forwardTranslatorOptions) {
    m_forwardTranslatorOptions = std::move(forwardTranslatorOptions);
  }

  Workspace ForwardTranslator::translateModel(const Model& model, ProgressBar* progressBar) {

    // When m_forwardTranslatorOptions.excludeSpaceTranslation() is false, could we skip the (expensive) clone since we aren't combining spaces?
    // No, we are still doing stuff like removing orphan loads, spaces not part of a thermal zone, etc
    auto modelCopy = model.clone(true).cast<Model>();

    m_progressBar = progressBar;
    if (m_progressBar) {
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum(model.numObjects());
    }

    return translateModelPrivate(modelCopy, true);
  }

  Workspace ForwardTranslator::translateModelObject(ModelObject& modelObject) {
    Model modelCopy;
    modelObject.clone(modelCopy);

    m_progressBar = nullptr;

    return translateModelPrivate(modelCopy, false);
  }

  std::vector<LogMessage> ForwardTranslator::warnings() const {
    std::vector<LogMessage> allMessages = m_logSink.logMessages();
    std::vector<LogMessage> result;
    std::copy_if(allMessages.cbegin(), allMessages.cend(), std::back_inserter(result),
                 [](const auto& logMessage) { return logMessage.logLevel() == Warn; });
    return result;
  }

  std::vector<LogMessage> ForwardTranslator::errors() const {
    std::vector<LogMessage> allMessages = m_logSink.logMessages();
    std::vector<LogMessage> result;
    std::copy_if(allMessages.cbegin(), allMessages.cend(), std::back_inserter(result),
                 [](const auto& logMessage) { return logMessage.logLevel() > Warn; });
    return result;
  }

  void ForwardTranslator::setKeepRunControlSpecialDays(bool keepRunControlSpecialDays) {
    m_forwardTranslatorOptions.setKeepRunControlSpecialDays(keepRunControlSpecialDays);
  }

  void ForwardTranslator::setIPTabularOutput(bool isIP) {
    m_forwardTranslatorOptions.setIPTabularOutput(isIP);
  }

  void ForwardTranslator::setExcludeLCCObjects(bool excludeLCCObjects) {
    m_forwardTranslatorOptions.setExcludeLCCObjects(excludeLCCObjects);
  }

  void ForwardTranslator::setExcludeSQliteOutputReport(bool excludeSQliteOutputReport) {
    m_forwardTranslatorOptions.setExcludeSQliteOutputReport(excludeSQliteOutputReport);
  }

  void ForwardTranslator::setExcludeHTMLOutputReport(bool excludeHTMLOutputReport) {
    m_forwardTranslatorOptions.setExcludeHTMLOutputReport(excludeHTMLOutputReport);
  }

  void ForwardTranslator::setExcludeVariableDictionary(bool excludeVariableDictionary) {
    m_forwardTranslatorOptions.setExcludeVariableDictionary(excludeVariableDictionary);
  }

  void ForwardTranslator::setExcludeSpaceTranslation(bool excludeSpaceTranslation) {
    m_forwardTranslatorOptions.setExcludeSpaceTranslation(excludeSpaceTranslation);
  }

  // Figure out which object
  // * If the load is assigned to a space,
  //     * m_forwardTranslatorOptions.excludeSpaceTranslation() = true: translate and return the IdfObject for the Zone
  //     * m_forwardTranslatorOptions.excludeSpaceTranslation() = false: translate and return the IdfObject for Space
  // * If the load is assigned to a spaceType:
  //     * translateAndMapModelObjec(spaceType) (which will return a ZoneList if m_forwardTranslatorOptions.excludeSpaceTranslation() is true, SpaceList otherwise)
  IdfObject ForwardTranslator::getSpaceLoadParent(const model::SpaceLoad& sp, bool allowSpaceType) {

    OptionalIdfObject relatedIdfObject;

    if (boost::optional<Space> space_ = sp.space()) {
      if (m_forwardTranslatorOptions.excludeSpaceTranslation()) {
        if (auto thermalZone_ = space_->thermalZone()) {
          relatedIdfObject = translateAndMapModelObject(thermalZone_.get());
        } else {
          OS_ASSERT(false);  // This shouldn't happen, since we removed all orphaned spaces earlier in the FT
        }
      } else {
        relatedIdfObject = translateAndMapModelObject(space_.get());
      }
    } else if (boost::optional<SpaceType> spaceType_ = sp.spaceType()) {
      if (allowSpaceType) {
        relatedIdfObject = translateAndMapModelObject(spaceType_.get());
      } else {
        OS_ASSERT(false);
      }
    }

    OS_ASSERT(relatedIdfObject);
    return relatedIdfObject.get();
  };

  Workspace ForwardTranslator::translateModelPrivate(model::Model& model, bool fullModelTranslation) {
    reset();

    // translate Version first
    auto version = model.getUniqueModelObject<model::Version>();
    translateAndMapModelObject(version);

    // translate Timestep second (this initializes it if need be)
    auto timestep = model.getUniqueModelObject<model::Timestep>();
    translateAndMapModelObject(timestep);

    // resolve surface marching conflicts before combining thermal zones or removing spaces
    // as those operations may change search distances
    resolveMatchedSurfaceConstructionConflicts(model);
    resolveMatchedSubSurfaceConstructionConflicts(model);

    // remove subsurfaces from air walls
    for (const auto& surface : model.getConcreteModelObjects<Surface>()) {
      if (surface.isAirWall()) {
        for (auto& subSurface : surface.subSurfaces()) {
          LOG(Warn, "Removing SubSurface '" << subSurface.nameString() << "' from air wall Surface '" << surface.nameString() << "'.");
          subSurface.remove();
        }
      }
    }

    // check for spaces not in a thermal zone
    for (auto& space : model.getConcreteModelObjects<Space>()) {
      if (!space.thermalZone()) {
        LOG(Warn, "Space " << space.name().get() << " is not associated with a ThermalZone, it will not be translated.");
        space.remove();
      }
    }

    // remove orphan surfaces
    for (auto& planarSurface : model.getModelObjects<PlanarSurface>()) {
      if (!planarSurface.planarSurfaceGroup()) {
        // a sub surface may have already been removed if the parent surface was removed
        if (!planarSurface.handle().isNull()) {
          LOG(Warn, planarSurface.briefDescription() << " is not associated with a PlanarSurfaceGroup, it will not be translated.");
          planarSurface.remove();
        }
      }
    }

    // remove orphan loads
    for (auto& spaceLoad : model.getModelObjects<SpaceLoad>()) {
      if (spaceLoad.optionalCast<model::WaterUseEquipment>()) {
        // WaterUseEquipment is not required to be attached to a space
        continue;
      }
      if ((!spaceLoad.space()) && (!spaceLoad.spaceType())) {
        LOG(Warn, spaceLoad.briefDescription() << " is not associated with a Space or SpaceType, it will not be translated.");
        spaceLoad.remove();
      }
    }

    if (m_forwardTranslatorOptions.excludeSpaceTranslation()) {
      // next thing to do is combine all spaces in each thermal zone
      // after this each zone will have 0 or 1 spaces and each space will have 0 or 1 zone
      for (auto& thermalZone : model.getConcreteModelObjects<ThermalZone>()) {
        thermalZone.combineSpaces();
      }
    } else {

      // The SpaceInfiltration:EffectiveLeakageAreas and FlowCoefficients (unlike the SpaceInfiltration:DesignFlowRate),
      // and the ElectricEquipment:ITE:AirCooled only accept a Zone or a Space, not a ZoneList nor a SpaceList
      // So we need to put them on the spaces to avoid problems. But we do not need to hardSize() them (they end up going on a Space).
      // then remove the spacetype ones to be safe (make 100% sure they won't get translated)

      for (auto& sp : model.getConcreteModelObjects<SpaceType>()) {
        // auto spi = sp.spaceInfiltrationDesignFlowRates();
        auto spiel = sp.spaceInfiltrationEffectiveLeakageAreas();
        auto spifc = sp.spaceInfiltrationFlowCoefficients();
        auto ites = sp.electricEquipmentITEAirCooled();
        std::vector<SpaceLoad> loads;
        loads.reserve(spiel.size() + spifc.size() + ites.size());
        loads.insert(loads.end(), spiel.begin(), spiel.end());
        loads.insert(loads.end(), spifc.begin(), spifc.end());
        loads.insert(loads.end(), ites.begin(), ites.end());

        for (auto& infil : loads) {
          if (infil.spaceType()) {
            for (auto& space : sp.spaces()) {
              auto infilClone = infil.clone(model).cast<SpaceLoad>();
              infilClone.setParent(space);
            }
            infil.remove();
          }
        }
      }
    }

    // remove unused space types
    std::vector<SpaceType> spaceTypes = model.getConcreteModelObjects<SpaceType>();
    for (auto& spaceType : spaceTypes) {
      std::vector<Space> spaces = spaceType.spaces();
      if (spaces.empty()) {
        LOG(Info, "SpaceType " << spaceType.name().get() << " is not referenced by any space, it will not be translated.");
        spaceType.remove();
      } else if (spaces.size() == 1) {
        //LOG(Info, "SpaceType " << spaceType.name().get() << " is referenced by one space, loads will be transfered to the space and the space type removed.");
        // hard apply space type adds a dummy space type to prevent inheriting building space type
        //spaces[0].hardApplySpaceType(false);
        //spaceType.remove();
      }
    }

    //Fix for Bug 717 - Take any OtherEquipment objects that still point to a spacetype and make
    //a new instance of them for every space that that spacetype points to then delete the one
    //that pointed to a spacetype
    //
    // TODO JM 2021-10-14: combineSpaces already does that. The only reason this code block is here is because:
    // 1. ThermalZone::combineSpaces doesn't touch the initial Space Types, it's just that they are unused
    // 2. This object is part of iddObjectToTranslate() which is a mistake to begin with: spaces/spaceTypes should be responsible for translating
    // their loads!
    // 3. Removing the unused space types right above should have taken care of the problem
    std::vector<OtherEquipment> otherEquipments = model.getConcreteModelObjects<OtherEquipment>();
    for (auto& otherEquipment : otherEquipments) {
      boost::optional<SpaceType> spaceTypeOfOtherEquipment = otherEquipment.spaceType();
      if (spaceTypeOfOtherEquipment) {
        //loop through the spaces in this space type and make a new instance for each one
        std::vector<Space> spaces = spaceTypeOfOtherEquipment.get().spaces();
        for (const auto& space : spaces) {
          auto otherEquipmentForSpace = otherEquipment.clone().cast<OtherEquipment>();
          otherEquipmentForSpace.setSpace(space);
          //make a nice name for the thing

          //std::string otherEquipmentForSpaceName = otherEquipment.name()
          //otherEquipment.setName("newName")
        }
        //now, delete the one that points to a spacetype
        otherEquipment.remove();
      }
    }

    // Energyplus only allows single zone input for ITE object. If space type is assigned in OS,
    // will translate to multiple ITE objects assigned to each zone under the same space type.
    // then delete the one that pointed to a spacetype.
    // By doing this, we can solve the potential problem that if this load is applied to a space type,
    // the load gets copied to each space of the space type, which may cause conflict of supply air node.
    //
    // TODO JM 2021-10-14: combineSpaces already does that. The only reason this code block is here is because:
    // 1. ThermalZone::combineSpaces doesn't touch the initial Space Types, it's just that they are unused
    // 2. This object is part of iddObjectToTranslate() which is a mistake to begin with: spaces/spaceTypes should be responsible for translating
    // their loads!
    // 3. Removing the unused space types right above should have taken care of the problem
    std::vector<ElectricEquipmentITEAirCooled> iTEAirCooledEquipments = model.getConcreteModelObjects<ElectricEquipmentITEAirCooled>();
    for (auto& iTequipment : iTEAirCooledEquipments) {
      boost::optional<SpaceType> spaceTypeOfITEquipment = iTequipment.spaceType();
      if (spaceTypeOfITEquipment) {
        //loop through the spaces in this space type and make a new instance for each one
        std::vector<Space> spaces = spaceTypeOfITEquipment.get().spaces();
        for (const auto& space : spaces) {
          auto iTEquipmentForSpace = iTequipment.clone().cast<ElectricEquipmentITEAirCooled>();
          iTEquipmentForSpace.setSpace(space);
        }
        //now, delete the one that points to a spacetype
        iTequipment.remove();
      }
    }

    // Temporary workaround for EnergyPlusTeam #4451
    // requested by http://code.google.com/p/cbecc/issues/detail?id=736
    // do this after combining spaces to avoid suprises about relative coordinate changes
    for (const auto& thermalZone : model.getConcreteModelObjects<ThermalZone>()) {
      boost::optional<DaylightingControl> dc = thermalZone.secondaryDaylightingControl();
      if (dc) {
        double z = dc->positionZCoordinate();
        if (z < 0) {
          // find lowest point in thermalZone and move space origin down to that point
          // lowest point will have z = 0 in relative coordinates
          std::vector<Space> spaces = thermalZone.spaces();
          OS_ASSERT(spaces.size() == 1);

          double minZ = z;
          BoundingBox bb = spaces[0].boundingBox();
          if (bb.minZ()) {
            minZ = std::min(minZ, bb.minZ().get());
          }
          OS_ASSERT(minZ < 0);

          Transformation currentT = spaces[0].transformation();
          Transformation newT = Transformation::translation(Vector3d(0, 0, minZ)) * currentT;
          bool test = spaces[0].changeTransformation(newT);
          OS_ASSERT(test);
        }
      }
    }

    // TODO: Is this still needed?
    // ensure shading controls only reference windows in a single zone and determine control sequence number
    // DLM: ideally E+ would not need to know the zone, shading controls could work across zones
    std::vector<ShadingControl> shadingControls = model.getConcreteModelObjects<ShadingControl>();
    std::sort(shadingControls.begin(), shadingControls.end(), WorkspaceObjectNameLess());
    std::map<Handle, ShadingControlVector> zoneHandleToShadingControlVectorMap;
    for (auto& shadingControl : shadingControls) {
      std::set<Handle> thisZoneHandleSet;
      for (auto& subSurface : shadingControl.subSurfaces()) {
        boost::optional<Space> space = subSurface.space();
        if (space) {
          boost::optional<ThermalZone> thermalZone = space->thermalZone();
          if (thermalZone) {
            Handle zoneHandle = thermalZone->handle();
            if (thisZoneHandleSet.empty()) {
              // first thermal zone, no clone
              thisZoneHandleSet.insert(zoneHandle);
              auto it = zoneHandleToShadingControlVectorMap.find(zoneHandle);
              if (it == zoneHandleToShadingControlVectorMap.end()) {
                zoneHandleToShadingControlVectorMap.insert(std::make_pair(zoneHandle, std::vector<ShadingControl>()));
              }
              it = zoneHandleToShadingControlVectorMap.find(zoneHandle);
              OS_ASSERT(it != zoneHandleToShadingControlVectorMap.end());
              it->second.push_back(shadingControl);
              shadingControl.additionalProperties().setFeature("Shading Control Sequence Number", static_cast<int>(it->second.size()));
            } else if (thisZoneHandleSet.find(zoneHandle) != thisZoneHandleSet.end()) {
              // already in here, good to go

            } else {
              // additional thermal zone, must clone
              thisZoneHandleSet.insert(zoneHandle);
              auto clone = shadingControl.clone(model).cast<ShadingControl>();
              // assign clone to control subSurface
              clone.addSubSurface(subSurface);
              // Try insert
              auto [it, success] = zoneHandleToShadingControlVectorMap.insert({zoneHandle, std::vector<ShadingControl>()});
              OS_ASSERT(success);
              OS_ASSERT(it != zoneHandleToShadingControlVectorMap.end());
              it->second.push_back(clone);
              clone.additionalProperties().setFeature("Shading Control Sequence Number", static_cast<int>(it->second.size()));
            }
          } else {
            LOG(Warn, "Cannot find ThermalZone for " << subSurface.briefDescription() << " referencing " << shadingControl.briefDescription());
          }
        } else {
          LOG(Warn, "Cannot find Space for " << subSurface.briefDescription() << " referencing " << shadingControl.briefDescription());
        }
      }
    }

    if (!m_forwardTranslatorOptions.keepRunControlSpecialDays()) {
      LOG(Warn, "You have manually choosen to not translate the RunPeriodControlSpecialDays, ignoring them.");
      for (model::RunPeriodControlSpecialDays holiday : model.getConcreteModelObjects<model::RunPeriodControlSpecialDays>()) {
        holiday.remove();
      }
    }

    if (fullModelTranslation) {

      // translate life cycle cost parameters
      if (!m_forwardTranslatorOptions.excludeLCCObjects()) {
        boost::optional<LifeCycleCostParameters> lifeCycleCostParameters = model.lifeCycleCostParameters();
        if (!lifeCycleCostParameters) {
          // only warn if costs are present
          if (!model.getConcreteModelObjects<LifeCycleCost>().empty()) {
            LOG(Warn, "No LifeCycleCostParameters but LifeCycleCosts are present, adding default LifeCycleCostParameters.");
          }

          // always add this object so E+ results section exists
          lifeCycleCostParameters = model.getUniqueModelObject<LifeCycleCostParameters>();
        }
        translateAndMapModelObject(*lifeCycleCostParameters);
      }

      // ensure that building exists
      boost::optional<model::Building> building = model.building();
      if (!building) {
        building = model.getUniqueModelObject<model::Building>();
      }
      translateAndMapModelObject(*building);

      // ensure that simulation control exists
      boost::optional<model::SimulationControl> simulationControl = model.getOptionalUniqueModelObject<model::SimulationControl>();
      if (!simulationControl) {
        simulationControl = model.getUniqueModelObject<model::SimulationControl>();
      }
      translateAndMapModelObject(*simulationControl);

      // ensure that sizing parameters control exists
      boost::optional<model::SizingParameters> sizingParameters = model.getOptionalUniqueModelObject<model::SizingParameters>();
      if (!sizingParameters) {
        sizingParameters = model.getUniqueModelObject<model::SizingParameters>();
      }
      translateAndMapModelObject(*sizingParameters);

      // ensure that run period exists
      // DLM: should this only be done if there is a WeatherFile object?
      boost::optional<model::RunPeriod> runPeriod = model.runPeriod();
      if (!runPeriod) {
        runPeriod = model.getUniqueModelObject<model::RunPeriod>();
      }
      translateAndMapModelObject(*runPeriod);

      // ensure that output table summary reports exists
      // If the user manually added an OutputTableSummaryReports, but he also opted-in to exclude it on the FT, which decision do we keep?
      // Given that it's a much harder to set the option on the FT, I'll respect that one
      if (!m_forwardTranslatorOptions.excludeHTMLOutputReport()) {
        auto optOutputTableSummaryReports = model.getOptionalUniqueModelObject<model::OutputTableSummaryReports>();
        // Add default one if none explicitly specified
        if (!optOutputTableSummaryReports) {
          auto outputTableSummaryReports = model.getUniqueModelObject<model::OutputTableSummaryReports>();
          outputTableSummaryReports.addSummaryReport("AllSummary");
          translateAndMapModelObject(outputTableSummaryReports);
        }
      }

      // NOTE: General note about taking a reference via emplace_back in the FT. It is faster to construct an object in place, but you must be careful
      // that m_idfObjects is NOT going to be resized while you are using the reference or it would be invalidated
      // eg:
      //    auto& objectRef = m_idfObjects.emplace_back(xxxx);
      //    translateAndMapModelObject(object.child()); // UH OH: THIS IS LIKELY GOING TO RESIZE m_idfObjects and if so &objectRef is now invalid!!

      // add a global geometry rules object
      auto& globalGeometryRules = m_idfObjects.emplace_back(openstudio::IddObjectType::GlobalGeometryRules);
      globalGeometryRules.setString(openstudio::GlobalGeometryRulesFields::StartingVertexPosition, "UpperLeftCorner");
      globalGeometryRules.setString(openstudio::GlobalGeometryRulesFields::VertexEntryDirection, "Counterclockwise");
      globalGeometryRules.setString(openstudio::GlobalGeometryRulesFields::CoordinateSystem, "Relative");
      globalGeometryRules.setString(openstudio::GlobalGeometryRulesFields::DaylightingReferencePointCoordinateSystem, "Relative");
      globalGeometryRules.setString(openstudio::GlobalGeometryRulesFields::RectangularSurfaceCoordinateSystem, "Relative");

      // create meters for utility bill objects
      std::vector<UtilityBill> utilityBills = model.getConcreteModelObjects<UtilityBill>();
      for (const auto& utilityBill : utilityBills) {
        // these meters and variables will be translated later
        OutputMeter consumptionMeter = utilityBill.consumptionMeter();
        boost::optional<OutputMeter> peakDemandMeter = utilityBill.peakDemandMeter();
      }
    }

    translateConstructions(model);
    translateSchedules(model);

    // Translate the Outdoor Air Node
    {
      auto node = model.outdoorAirNode();
      // Create a new IddObjectType::OutdoorAir_Node
      IdfObject idfObject(IddObjectType::OutdoorAir_Node);
      m_idfObjects.push_back(idfObject);
      idfObject.setName(node.name().get());
    }

    // get AirLoopHVACDedicatedOutdoorAirSystem in sorted order
    std::vector<AirLoopHVACDedicatedOutdoorAirSystem> doass = model.getConcreteModelObjects<AirLoopHVACDedicatedOutdoorAirSystem>();
    std::sort(doass.begin(), doass.end(), WorkspaceObjectNameLess());
    for (auto& doas : doass) {
      translateAndMapModelObject(doas);
    }

    // get air loops in sorted order
    std::vector<AirLoopHVAC> airLoops = model.getConcreteModelObjects<AirLoopHVAC>();
    std::sort(airLoops.begin(), airLoops.end(), WorkspaceObjectNameLess());
    for (auto& airLoop : airLoops) {
      translateAndMapModelObject(airLoop);
    }

    // get AirConditionerVariableRefrigerantFlow objects in sorted order
    std::vector<AirConditionerVariableRefrigerantFlow> vrfs = model.getConcreteModelObjects<AirConditionerVariableRefrigerantFlow>();
    std::sort(vrfs.begin(), vrfs.end(), WorkspaceObjectNameLess());
    for (auto& vrf : vrfs) {
      translateAndMapModelObject(vrf);
    }

    // get AirConditionerVariableRefrigerantFlowFluidTemperatureControl objects in sorted order
    std::vector<AirConditionerVariableRefrigerantFlowFluidTemperatureControl> vrfftcs =
      model.getConcreteModelObjects<AirConditionerVariableRefrigerantFlowFluidTemperatureControl>();
    std::sort(vrfftcs.begin(), vrfftcs.end(), WorkspaceObjectNameLess());
    for (auto& vrfftc : vrfftcs) {
      translateAndMapModelObject(vrfftc);
    }

    // get AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR objects in sorted order
    std::vector<AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR> vrfftchrs =
      model.getConcreteModelObjects<AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR>();
    std::sort(vrfftchrs.begin(), vrfftchrs.end(), WorkspaceObjectNameLess());
    for (auto& vrfftchr : vrfftchrs) {
      translateAndMapModelObject(vrfftchr);
    }

    // get plant loops in sorted order
    std::vector<PlantLoop> plantLoops = model.getConcreteModelObjects<PlantLoop>();
    std::sort(plantLoops.begin(), plantLoops.end(), WorkspaceObjectNameLess());
    for (auto& plantLoop : plantLoops) {
      translateAndMapModelObject(plantLoop);
    }

    // translate AFN
    translateAirflowNetwork(model);

    // now loop over all objects
    for (const IddObjectType& iddObjectType : iddObjectsToTranslate()) {

      // get objects by type in sorted order
      std::vector<WorkspaceObject> objects = model.getObjectsByType(iddObjectType);
      std::sort(objects.begin(), objects.end(), WorkspaceObjectNameLess());

      for (const WorkspaceObject& workspaceObject : objects) {
        auto modelObject = workspaceObject.cast<ModelObject>();
        translateAndMapModelObject(modelObject);
      }
    }

    if (fullModelTranslation) {
      // add output requests
      this->createStandardOutputRequests(model);
    }

    Workspace workspace(StrictnessLevel::Minimal, IddFileType::EnergyPlus);
    OptionalWorkspaceObject vo = workspace.versionObject();
    OS_ASSERT(vo);
    workspace.removeObject(vo->handle());

    workspace.setFastNaming(true);
    workspace.addObjects(m_idfObjects);
    workspace.setFastNaming(false);
    OS_ASSERT(workspace.getObjectsByType(IddObjectType::Version).size() == 1U);

    return workspace;
  }

  // struct for sorting children in forward translator
  struct ChildSorter
  {
    ChildSorter(std::vector<IddObjectType>& iddObjectTypes) : m_iddObjectTypes(iddObjectTypes) {}

    // sort first by position in iddObjectTypes and then by name
    bool operator()(const model::ModelObject& a, const model::ModelObject& b) const {
      auto ita = std::find(m_iddObjectTypes.begin(), m_iddObjectTypes.end(), a.iddObject().type());
      auto itb = std::find(m_iddObjectTypes.begin(), m_iddObjectTypes.end(), b.iddObject().type());

      if (ita < itb) {
        return true;
      } else if (ita > itb) {
        return false;
      }

      std::string aname;
      boost::optional<std::string> oaname = a.name();
      if (oaname) {
        aname = *oaname;
      }
      std::string bname;
      boost::optional<std::string> obname = b.name();
      if (obname) {
        bname = *obname;
      }
      return istringLess(aname, bname);
    }

    std::vector<IddObjectType> m_iddObjectTypes;
  };

  // NOLINTBEGIN(readability-function-size, bugprone-branch-clone)
  boost::optional<IdfObject> ForwardTranslator::translateAndMapModelObject(ModelObject& modelObject) {
    boost::optional<IdfObject> retVal;

    // if already translated then exit
    auto objInMapIt = m_map.find(modelObject.handle());
    if (objInMapIt != m_map.end()) {
      return boost::optional<IdfObject>(objInMapIt->second);
    }

    LOG(Trace, "Translating " << modelObject.briefDescription() << ".");

    switch (modelObject.iddObject().type().value()) {
      case openstudio::IddObjectType::OS_AdditionalProperties: {
        // no op
        break;
      }
      case openstudio::IddObjectType::OS_AirConditioner_VariableRefrigerantFlow: {
        auto vrf = modelObject.cast<AirConditionerVariableRefrigerantFlow>();
        retVal = translateAirConditionerVariableRefrigerantFlow(vrf);
        break;
      }
      case openstudio::IddObjectType::OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl: {
        model::AirConditionerVariableRefrigerantFlowFluidTemperatureControl vrf =
          modelObject.cast<AirConditionerVariableRefrigerantFlowFluidTemperatureControl>();
        retVal = translateAirConditionerVariableRefrigerantFlowFluidTemperatureControl(vrf);
        break;
      }
      case openstudio::IddObjectType::OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HR: {
        model::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR vrf =
          modelObject.cast<AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR>();
        retVal = translateAirConditionerVariableRefrigerantFlowFluidTemperatureControlHR(vrf);
        break;
      }
      case openstudio::IddObjectType::OS_AirLoopHVAC: {
        auto airLoopHVAC = modelObject.cast<AirLoopHVAC>();
        retVal = translateAirLoopHVAC(airLoopHVAC);
        break;
      }
      case openstudio::IddObjectType::OS_AirLoopHVAC_ReturnPlenum: {
        auto airLoopHVACReturnPlenum = modelObject.cast<AirLoopHVACReturnPlenum>();
        retVal = translateAirLoopHVACReturnPlenum(airLoopHVACReturnPlenum);
        break;
      }
      case openstudio::IddObjectType::OS_AirLoopHVAC_SupplyPlenum: {
        auto airLoopHVACSupplyPlenum = modelObject.cast<AirLoopHVACSupplyPlenum>();
        retVal = translateAirLoopHVACSupplyPlenum(airLoopHVACSupplyPlenum);
        break;
      }
      case openstudio::IddObjectType::OS_AirTerminal_DualDuct_ConstantVolume: {
        auto mo = modelObject.cast<AirTerminalDualDuctConstantVolume>();
        retVal = translateAirTerminalDualDuctConstantVolume(mo);
        break;
      }
      case openstudio::IddObjectType::OS_AirTerminal_DualDuct_VAV: {
        auto mo = modelObject.cast<AirTerminalDualDuctVAV>();
        retVal = translateAirTerminalDualDuctVAV(mo);
        break;
      }
      case openstudio::IddObjectType::OS_AirTerminal_DualDuct_VAV_OutdoorAir: {
        auto mo = modelObject.cast<AirTerminalDualDuctVAVOutdoorAir>();
        retVal = translateAirTerminalDualDuctVAVOutdoorAir(mo);
        break;
      }
      case openstudio::IddObjectType::OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInduction: {
        auto airTerminal = modelObject.cast<AirTerminalSingleDuctConstantVolumeFourPipeInduction>();
        retVal = translateAirTerminalSingleDuctConstantVolumeFourPipeInduction(airTerminal);
        break;
      }
      case openstudio::IddObjectType::OS_AirTerminal_SingleDuct_ConstantVolume_Reheat: {
        auto airTerminal = modelObject.cast<AirTerminalSingleDuctConstantVolumeReheat>();
        retVal = translateAirTerminalSingleDuctConstantVolumeReheat(airTerminal);
        break;
      }
      case openstudio::IddObjectType::OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeam: {
        auto airTerminal = modelObject.cast<AirTerminalSingleDuctConstantVolumeCooledBeam>();
        retVal = translateAirTerminalSingleDuctConstantVolumeCooledBeam(airTerminal);
        break;
      }
      case openstudio::IddObjectType::OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeBeam: {
        auto airTerminal = modelObject.cast<AirTerminalSingleDuctConstantVolumeFourPipeBeam>();
        retVal = translateAirTerminalSingleDuctConstantVolumeFourPipeBeam(airTerminal);
        break;
      }
      case openstudio::IddObjectType::OS_AirTerminal_SingleDuct_ParallelPIU_Reheat: {
        auto airTerminal = modelObject.cast<AirTerminalSingleDuctParallelPIUReheat>();
        retVal = translateAirTerminalSingleDuctParallelPIUReheat(airTerminal);
        break;
      }
      case openstudio::IddObjectType::OS_AirTerminal_SingleDuct_SeriesPIU_Reheat: {
        auto airTerminal = modelObject.cast<AirTerminalSingleDuctSeriesPIUReheat>();
        retVal = translateAirTerminalSingleDuctSeriesPIUReheat(airTerminal);
        break;
      }
      case openstudio::IddObjectType::OS_AirTerminal_SingleDuct_ConstantVolume_NoReheat: {
        auto airTerminal = modelObject.cast<AirTerminalSingleDuctConstantVolumeNoReheat>();
        retVal = translateAirTerminalSingleDuctConstantVolumeNoReheat(airTerminal);
        break;
      }
      case openstudio::IddObjectType::OS_AirTerminal_SingleDuct_VAV_NoReheat: {
        auto airTerminal = modelObject.cast<AirTerminalSingleDuctVAVNoReheat>();
        retVal = translateAirTerminalSingleDuctVAVNoReheat(airTerminal);
        break;
      }
      case openstudio::IddObjectType::OS_AirTerminal_SingleDuct_VAV_Reheat: {
        auto airTerminal = modelObject.cast<AirTerminalSingleDuctVAVReheat>();
        retVal = translateAirTerminalSingleDuctVAVReheat(airTerminal);
        break;
      }
      case openstudio::IddObjectType::OS_AirTerminal_SingleDuct_InletSideMixer: {
        auto airTerminal = modelObject.cast<AirTerminalSingleDuctInletSideMixer>();
        retVal = translateAirTerminalSingleDuctInletSideMixer(airTerminal);
        break;
      }
      case openstudio::IddObjectType::OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheat: {
        auto airTerminal = modelObject.cast<AirTerminalSingleDuctVAVHeatAndCoolNoReheat>();
        retVal = translateAirTerminalSingleDuctVAVHeatAndCoolNoReheat(airTerminal);
        break;
      }
      case openstudio::IddObjectType::OS_AirTerminal_SingleDuct_VAV_HeatAndCool_Reheat: {
        auto airTerminal = modelObject.cast<AirTerminalSingleDuctVAVHeatAndCoolReheat>();
        retVal = translateAirTerminalSingleDuctVAVHeatAndCoolReheat(airTerminal);
        break;
      }
      case openstudio::IddObjectType::OS_AirLoopHVAC_ZoneSplitter: {
        auto splitter = modelObject.cast<AirLoopHVACZoneSplitter>();
        retVal = translateAirLoopHVACZoneSplitter(splitter);
        break;
      }
      case openstudio::IddObjectType::OS_AirLoopHVAC_ZoneMixer: {
        auto mixer = modelObject.cast<AirLoopHVACZoneMixer>();
        retVal = translateAirLoopHVACZoneMixer(mixer);
        break;
      }
      case openstudio::IddObjectType::OS_AirLoopHVAC_OutdoorAirSystem: {
        auto oaSystem = modelObject.cast<AirLoopHVACOutdoorAirSystem>();
        retVal = translateAirLoopHVACOutdoorAirSystem(oaSystem);
        break;
      }
      case openstudio::IddObjectType::OS_AirLoopHVAC_DedicatedOutdoorAirSystem: {
        auto doaSystem = modelObject.cast<AirLoopHVACDedicatedOutdoorAirSystem>();
        retVal = translateAirLoopHVACDedicatedOutdoorAirSystem(doaSystem);
        break;
      }
      case openstudio::IddObjectType::OS_AirLoopHVAC_UnitaryHeatPump_AirToAir: {
        auto unitary = modelObject.cast<AirLoopHVACUnitaryHeatPumpAirToAir>();
        retVal = translateAirLoopHVACUnitaryHeatPumpAirToAir(unitary);
        break;
      }
      case openstudio::IddObjectType::OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypass: {
        auto unitary = modelObject.cast<AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass>();
        retVal = translateAirLoopHVACUnitaryHeatCoolVAVChangeoverBypass(unitary);
        break;
      }
      case openstudio::IddObjectType::OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeed: {
        auto unitary = modelObject.cast<AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed>();
        retVal = translateAirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed(unitary);
        break;
      }
      case openstudio::IddObjectType::OS_AirLoopHVAC_UnitarySystem: {
        auto unitary = modelObject.cast<AirLoopHVACUnitarySystem>();
        retVal = translateAirLoopHVACUnitarySystem(unitary);
        break;
      }
      case openstudio::IddObjectType::OS_AvailabilityManagerAssignmentList: {
        auto mo = modelObject.cast<AvailabilityManagerAssignmentList>();
        retVal = translateAvailabilityManagerAssignmentList(mo);
        break;
      }
      case openstudio::IddObjectType::OS_AvailabilityManager_Scheduled: {
        auto mo = modelObject.cast<AvailabilityManagerScheduled>();
        retVal = translateAvailabilityManagerScheduled(mo);
        break;
      }
      case openstudio::IddObjectType::OS_AvailabilityManager_ScheduledOn: {
        auto mo = modelObject.cast<AvailabilityManagerScheduledOn>();
        retVal = translateAvailabilityManagerScheduledOn(mo);
        break;
      }
      case openstudio::IddObjectType::OS_AvailabilityManager_ScheduledOff: {
        auto mo = modelObject.cast<AvailabilityManagerScheduledOff>();
        retVal = translateAvailabilityManagerScheduledOff(mo);
        break;
      }
      case openstudio::IddObjectType::OS_AvailabilityManager_HybridVentilation: {
        auto mo = modelObject.cast<AvailabilityManagerHybridVentilation>();
        retVal = translateAvailabilityManagerHybridVentilation(mo);
        break;
      }
      case openstudio::IddObjectType::OS_AvailabilityManager_OptimumStart: {
        auto mo = modelObject.cast<AvailabilityManagerOptimumStart>();
        retVal = translateAvailabilityManagerOptimumStart(mo);
        break;
      }
      case openstudio::IddObjectType::OS_AvailabilityManager_DifferentialThermostat: {
        auto mo = modelObject.cast<AvailabilityManagerDifferentialThermostat>();
        retVal = translateAvailabilityManagerDifferentialThermostat(mo);
        break;
      }
      case openstudio::IddObjectType::OS_AvailabilityManager_NightVentilation: {
        auto mo = modelObject.cast<AvailabilityManagerNightVentilation>();
        retVal = translateAvailabilityManagerNightVentilation(mo);
        break;
      }
      case openstudio::IddObjectType::OS_AvailabilityManager_NightCycle: {
        auto mo = modelObject.cast<AvailabilityManagerNightCycle>();
        retVal = translateAvailabilityManagerNightCycle(mo);
        break;
      }
      case openstudio::IddObjectType::OS_AvailabilityManager_HighTemperatureTurnOn: {
        auto mo = modelObject.cast<AvailabilityManagerHighTemperatureTurnOn>();
        retVal = translateAvailabilityManagerHighTemperatureTurnOn(mo);
        break;
      }
      case openstudio::IddObjectType::OS_AvailabilityManager_HighTemperatureTurnOff: {
        auto mo = modelObject.cast<AvailabilityManagerHighTemperatureTurnOff>();
        retVal = translateAvailabilityManagerHighTemperatureTurnOff(mo);
        break;
      }
      case openstudio::IddObjectType::OS_AvailabilityManager_LowTemperatureTurnOn: {
        auto mo = modelObject.cast<AvailabilityManagerLowTemperatureTurnOn>();
        retVal = translateAvailabilityManagerLowTemperatureTurnOn(mo);
        break;
      }
      case openstudio::IddObjectType::OS_AvailabilityManager_LowTemperatureTurnOff: {
        auto mo = modelObject.cast<AvailabilityManagerLowTemperatureTurnOff>();
        retVal = translateAvailabilityManagerLowTemperatureTurnOff(mo);
        break;
      }
      case openstudio::IddObjectType::OS_Boiler_HotWater: {
        auto boiler = modelObject.cast<BoilerHotWater>();
        retVal = translateBoilerHotWater(boiler);
        break;
      }
      case openstudio::IddObjectType::OS_Boiler_Steam: {
        auto boiler = modelObject.cast<BoilerSteam>();
        retVal = translateBoilerSteam(boiler);
        break;
      }
      case openstudio::IddObjectType::OS_Building: {
        auto building = modelObject.cast<Building>();
        retVal = translateBuilding(building);
        break;
      }
      case openstudio::IddObjectType::OS_BuildingStory: {
        // no-op
        return retVal;
      }
      case openstudio::IddObjectType::OS_CentralHeatPumpSystem: {
        auto mo = modelObject.cast<CentralHeatPumpSystem>();
        retVal = translateCentralHeatPumpSystem(mo);
        break;
      }
      case openstudio::IddObjectType::OS_CentralHeatPumpSystem_Module: {
        // no-op
        return retVal;
      }
      case openstudio::IddObjectType::OS_Chiller_Absorption: {
        auto mo = modelObject.cast<ChillerAbsorption>();
        retVal = translateChillerAbsorption(mo);
        break;
      }
      case openstudio::IddObjectType::OS_Chiller_Absorption_Indirect: {
        auto mo = modelObject.cast<ChillerAbsorptionIndirect>();
        retVal = translateChillerAbsorptionIndirect(mo);
        break;
      }
      case openstudio::IddObjectType::OS_Chiller_Electric_ASHRAE205: {
        auto mo = modelObject.cast<ChillerElectricASHRAE205>();
        retVal = translateChillerElectricASHRAE205(mo);
        break;
      }
      case openstudio::IddObjectType::OS_Chiller_Electric_EIR: {
        auto chiller = modelObject.cast<ChillerElectricEIR>();
        retVal = translateChillerElectricEIR(chiller);
        break;
      }
      case openstudio::IddObjectType::OS_Chiller_Electric_ReformulatedEIR: {
        auto chiller = modelObject.cast<ChillerElectricReformulatedEIR>();
        retVal = translateChillerElectricReformulatedEIR(chiller);
        break;
      }
      case openstudio::IddObjectType::OS_ChillerHeaterPerformance_Electric_EIR: {
        auto mo = modelObject.cast<ChillerHeaterPerformanceElectricEIR>();
        retVal = translateChillerHeaterPerformanceElectricEIR(mo);
        break;
      }
      case openstudio::IddObjectType::OS_ClimateZones: {
        // no-op
        return retVal;
      }
      case openstudio::IddObjectType::OS_Construction_CfactorUndergroundWall: {
        auto construction = modelObject.cast<CFactorUndergroundWallConstruction>();
        retVal = translateCFactorUndergroundWallConstruction(construction);
        break;
      }
      case openstudio::IddObjectType::OS_ConvergenceLimits: {
        auto limits = modelObject.cast<ConvergenceLimits>();
        retVal = translateConvergenceLimits(limits);
        break;
      }
      case openstudio::IddObjectType::OS_Coil_Cooling_CooledBeam: {
        // This is handled directly in ATU:SingleDuct:ConstantVolume::CooledBeam
        break;
      }
      case openstudio::IddObjectType::OS_Coil_Cooling_FourPipeBeam: {
        // This is handled directly in ATU:SingleDuct:ConstantVolume::FourPipeBeam
        break;
      }
      case openstudio::IddObjectType::OS_Coil_Heating_FourPipeBeam: {
        // This is handled directly in ATU:SingleDuct:ConstantVolume::FourPipeBeam
        break;
      }
      case openstudio::IddObjectType::OS_Coil_Cooling_DX: {
        auto coil = modelObject.cast<CoilCoolingDX>();
        if (this->isHVACComponentWithinUnitary(coil)) {
          retVal = translateCoilCoolingDXWithoutUnitary(coil);
        } else {
          retVal = translateCoilCoolingDX(coil);
        }
        break;
      }
      case openstudio::IddObjectType::OS_Coil_Cooling_DX_CurveFit_Performance: {
        auto performance = modelObject.cast<CoilCoolingDXCurveFitPerformance>();
        retVal = translateCoilCoolingDXCurveFitPerformance(performance);
        break;
      }
      case openstudio::IddObjectType::OS_Coil_Cooling_DX_CurveFit_OperatingMode: {
        auto operatingMode = modelObject.cast<CoilCoolingDXCurveFitOperatingMode>();
        retVal = translateCoilCoolingDXCurveFitOperatingMode(operatingMode);
        break;
      }
      case openstudio::IddObjectType::OS_Coil_Cooling_DX_CurveFit_Speed: {
        auto speed = modelObject.cast<CoilCoolingDXCurveFitSpeed>();
        retVal = translateCoilCoolingDXCurveFitSpeed(speed);
        break;
      }
      case openstudio::IddObjectType::OS_Coil_Cooling_DX_SingleSpeed: {
        auto coil = modelObject.cast<CoilCoolingDXSingleSpeed>();
        if (isHVACComponentWithinUnitary(coil)) {
          retVal = translateCoilCoolingDXSingleSpeedWithoutUnitary(coil);
        } else {
          retVal = translateCoilCoolingDXSingleSpeed(coil);
        }
        break;
      }
      case openstudio::IddObjectType::OS_Coil_Cooling_DX_SingleSpeed_ThermalStorage: {
        auto coil = modelObject.cast<CoilCoolingDXSingleSpeedThermalStorage>();
        if (isHVACComponentWithinUnitary(coil)) {
          retVal = translateCoilCoolingDXSingleSpeedThermalStorageWithoutUnitary(coil);
        } else {
          retVal = translateCoilCoolingDXSingleSpeedThermalStorage(coil);
        }
        break;
      }
      case openstudio::IddObjectType::OS_Coil_Cooling_DX_MultiSpeed: {
        auto coil = modelObject.cast<CoilCoolingDXMultiSpeed>();
        retVal = translateCoilCoolingDXMultiSpeed(coil);
        break;
      }
      case openstudio::IddObjectType::OS_Coil_Cooling_DX_MultiSpeed_StageData: {
        return retVal;
      }
      case openstudio::IddObjectType::OS_Coil_Cooling_DX_TwoSpeed: {
        auto coil = modelObject.cast<CoilCoolingDXTwoSpeed>();
        if (isHVACComponentWithinUnitary(coil)) {
          retVal = translateCoilCoolingDXTwoSpeedWithoutUnitary(coil);
        } else {
          retVal = translateCoilCoolingDXTwoSpeed(coil);
        }
        break;
      }
      case openstudio::IddObjectType::OS_Coil_Cooling_DX_TwoStageWithHumidityControlMode: {
        auto coil = modelObject.cast<CoilCoolingDXTwoStageWithHumidityControlMode>();
        if (isHVACComponentWithinUnitary(coil)) {
          retVal = translateCoilCoolingDXTwoStageWithHumidityControlModeWithoutUnitary(coil);
        } else {
          retVal = translateCoilCoolingDXTwoStageWithHumidityControlMode(coil);
        }
        break;
      }
      case openstudio::IddObjectType::OS_Coil_Cooling_DX_VariableRefrigerantFlow: {
        auto coil = modelObject.cast<CoilCoolingDXVariableRefrigerantFlow>();
        retVal = translateCoilCoolingDXVariableRefrigerantFlow(coil);
        break;
      }
      case openstudio::IddObjectType::OS_Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControl: {
        model::CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl coil =
          modelObject.cast<CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl>();
        retVal = translateCoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl(coil);
        break;
      }
      case openstudio::IddObjectType::OS_Coil_Cooling_DX_VariableSpeed: {
        auto coil = modelObject.cast<CoilCoolingDXVariableSpeed>();
        if (isHVACComponentWithinUnitary(coil)) {
          retVal = translateCoilCoolingDXVariableSpeedWithoutUnitary(coil);
        } else {
          retVal = translateCoilCoolingDXVariableSpeed(coil);
        }
        break;
      }
      case openstudio::IddObjectType::OS_Coil_Cooling_DX_VariableSpeed_SpeedData: {
        // DLM: is this a no-op?
        break;
      }
      case openstudio::IddObjectType::OS_Coil_Cooling_LowTemperatureRadiant_ConstantFlow: {
        // no-op
        return retVal;
      }
      case openstudio::IddObjectType::OS_Coil_Cooling_LowTemperatureRadiant_VariableFlow: {
        // no-op
        return retVal;
      }
      case openstudio::IddObjectType::OS_Coil_Cooling_Water: {
        auto coil = modelObject.cast<CoilCoolingWater>();
        retVal = translateCoilCoolingWater(coil);
        break;
      }
      case openstudio::IddObjectType::OS_Coil_Cooling_Water_Panel_Radiant: {
        // no-op
        return retVal;
      }
      case openstudio::IddObjectType::OS_Coil_Cooling_WaterToAirHeatPump_EquationFit: {
        auto coil = modelObject.cast<CoilCoolingWaterToAirHeatPumpEquationFit>();
        retVal = translateCoilCoolingWaterToAirHeatPumpEquationFit(coil);
        break;
      }
      case openstudio::IddObjectType::OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit: {
        auto coil = modelObject.cast<CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit>();
        retVal = translateCoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit(coil);
        break;
      }
      case openstudio::IddObjectType::OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedData: {
        // DLM: is this a no-op?
        break;
      }
      case openstudio::IddObjectType::OS_Coil_Heating_Desuperheater: {
        auto coil = modelObject.cast<CoilHeatingDesuperheater>();
        retVal = translateCoilHeatingDesuperheater(coil);
        break;
      }
      case openstudio::IddObjectType::OS_Coil_Heating_DX_SingleSpeed: {
        auto coil = modelObject.cast<CoilHeatingDXSingleSpeed>();
        if (isHVACComponentWithinUnitary(coil)) {
          retVal = translateCoilHeatingDXSingleSpeedWithoutUnitary(coil);
        } else {
          retVal = translateCoilHeatingDXSingleSpeed(coil);
        }
        break;
      }
      case openstudio::IddObjectType::OS_Coil_Heating_DX_MultiSpeed: {
        auto coil = modelObject.cast<CoilHeatingDXMultiSpeed>();
        retVal = translateCoilHeatingDXMultiSpeed(coil);
        break;
      }
      case openstudio::IddObjectType::OS_Coil_Heating_DX_MultiSpeed_StageData: {
        return retVal;
      }
      case openstudio::IddObjectType::OS_Coil_Heating_DX_VariableSpeed: {
        auto coil = modelObject.cast<CoilHeatingDXVariableSpeed>();
        if (isHVACComponentWithinUnitary(coil)) {
          retVal = translateCoilHeatingDXVariableSpeedWithoutUnitary(coil);
        } else {
          retVal = translateCoilHeatingDXVariableSpeed(coil);
        }
        break;
      }
      case openstudio::IddObjectType::OS_Coil_Heating_DX_VariableSpeed_SpeedData: {
        //DLM: is this a no-op?
        break;
      }
      case openstudio::IddObjectType::OS_Coil_Heating_Electric: {
        auto coil = modelObject.cast<CoilHeatingElectric>();
        retVal = translateCoilHeatingElectric(coil);
        break;
      }
      case openstudio::IddObjectType::OS_Coil_Heating_Electric_MultiStage: {
        auto coil = modelObject.cast<CoilHeatingElectricMultiStage>();
        retVal = translateCoilHeatingElectricMultiStage(coil);
        break;
      }
      case openstudio::IddObjectType::OS_Coil_Heating_Electric_MultiStage_StageData: {
        // no-op
        return retVal;
      }
      case openstudio::IddObjectType::OS_Coil_Heating_Gas: {
        auto coil = modelObject.cast<CoilHeatingGas>();
        retVal = translateCoilHeatingGas(coil);
        break;
      }
      case openstudio::IddObjectType::OS_Coil_Heating_Gas_MultiStage: {
        auto coil = modelObject.cast<CoilHeatingGasMultiStage>();
        retVal = translateCoilHeatingGasMultiStage(coil);
        break;
      }
      case openstudio::IddObjectType::OS_Coil_Heating_Gas_MultiStage_StageData: {
        // no-op
        return retVal;
      }
      case openstudio::IddObjectType::OS_Coil_Heating_LowTemperatureRadiant_ConstantFlow: {
        // no-op
        return retVal;
      }
      case openstudio::IddObjectType::OS_Coil_Heating_LowTemperatureRadiant_VariableFlow: {
        // no-op
        return retVal;
      }
      case openstudio::IddObjectType::OS_Coil_Heating_DX_VariableRefrigerantFlow: {
        auto coil = modelObject.cast<CoilHeatingDXVariableRefrigerantFlow>();
        retVal = translateCoilHeatingDXVariableRefrigerantFlow(coil);
        break;
      }
      case openstudio::IddObjectType::OS_Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControl: {
        model::CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl coil =
          modelObject.cast<CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl>();
        retVal = translateCoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl(coil);
        break;
      }
      case openstudio::IddObjectType::OS_Coil_Heating_Water: {
        auto coil = modelObject.cast<CoilHeatingWater>();
        retVal = translateCoilHeatingWater(coil);
        break;
      }
      case openstudio::IddObjectType::OS_Coil_Heating_WaterToAirHeatPump_EquationFit: {
        auto coil = modelObject.cast<CoilHeatingWaterToAirHeatPumpEquationFit>();
        retVal = translateCoilHeatingWaterToAirHeatPumpEquationFit(coil);
        break;
      }
      case openstudio::IddObjectType::OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit: {
        auto coil = modelObject.cast<CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit>();
        retVal = translateCoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit(coil);
        break;
      }
      case openstudio::IddObjectType::OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedData: {
        // DLM: is this a no-op?
        break;
      }
      case openstudio::IddObjectType::OS_Coil_Heating_Water_Baseboard: {
        // DLM: is this a no-op?
        break;
      }
      case openstudio::IddObjectType::OS_Coil_Heating_Water_Baseboard_Radiant: {
        // no-op
        return retVal;
      }
      case openstudio::IddObjectType::OS_CoilPerformance_DX_Cooling: {
        auto mo = modelObject.cast<CoilPerformanceDXCooling>();
        retVal = translateCoilPerformanceDXCooling(mo);
        break;
      }
      case openstudio::IddObjectType::OS_CoilSystem_Cooling_Water_HeatExchangerAssisted: {
        auto mo = modelObject.cast<CoilSystemCoolingWaterHeatExchangerAssisted>();
        retVal = translateCoilSystemCoolingWaterHeatExchangerAssisted(mo);
        break;
      }
      case openstudio::IddObjectType::OS_CoilSystem_Cooling_DX_HeatExchangerAssisted: {
        auto mo = modelObject.cast<CoilSystemCoolingDXHeatExchangerAssisted>();
        retVal = translateCoilSystemCoolingDXHeatExchangerAssisted(mo);
        break;
      }
      case openstudio::IddObjectType::OS_CoilSystem_IntegratedHeatPump_AirSource: {
        auto mo = modelObject.cast<CoilSystemIntegratedHeatPumpAirSource>();
        retVal = translateCoilSystemIntegratedHeatPumpAirSource(mo);
        break;
      }
      case openstudio::IddObjectType::OS_Coil_UserDefined: {
        auto coil = modelObject.cast<CoilUserDefined>();
        retVal = translateCoilUserDefined(coil);
        break;
      }
      case openstudio::IddObjectType::OS_Coil_WaterHeating_Desuperheater: {
        auto coil = modelObject.cast<CoilWaterHeatingDesuperheater>();
        retVal = translateCoilWaterHeatingDesuperheater(coil);
        break;
      }
      case openstudio::IddObjectType::OS_Coil_WaterHeating_AirToWaterHeatPump: {
        auto mo = modelObject.cast<CoilWaterHeatingAirToWaterHeatPump>();
        retVal = translateCoilWaterHeatingAirToWaterHeatPump(mo);
        break;
      }
      case openstudio::IddObjectType::OS_Coil_WaterHeating_AirToWaterHeatPump_VariableSpeed: {
        auto mo = modelObject.cast<CoilWaterHeatingAirToWaterHeatPumpVariableSpeed>();
        retVal = translateCoilWaterHeatingAirToWaterHeatPumpVariableSpeed(mo);
        break;
      }
      case openstudio::IddObjectType::OS_Coil_WaterHeating_AirToWaterHeatPump_Wrapped: {
        auto mo = modelObject.cast<CoilWaterHeatingAirToWaterHeatPumpWrapped>();
        retVal = translateCoilWaterHeatingAirToWaterHeatPumpWrapped(mo);
        break;
      }
      case openstudio::IddObjectType::OS_ComponentData: {
        // no-op
        return retVal;
      }
      case openstudio::IddObjectType::OS_ComponentCost_Adjustments: {
        LOG(Warn, "OS:ComponentCost:Adjustments '" << modelObject.name().get() << "' not translated to EnergyPlus.");
        return retVal;
      }
      case openstudio::IddObjectType::OS_Connection: {
        // no-op
        return retVal;
      }
      case openstudio::IddObjectType::OS_Connector_Mixer: {
        // no-op
        return retVal;
      }
      case openstudio::IddObjectType::OS_Connector_Splitter: {
        // no-op
        return retVal;
      }
      case openstudio::IddObjectType::OS_Construction: {
        auto construction = modelObject.cast<Construction>();
        retVal = translateConstruction(construction);
        break;
      }
      case openstudio::IddObjectType::OS_Construction_AirBoundary: {
        auto constructionAirBoundary = modelObject.cast<ConstructionAirBoundary>();
        retVal = translateConstructionAirBoundary(constructionAirBoundary);
        break;
      }
      case openstudio::IddObjectType::OS_Construction_InternalSource: {
        auto constructionIntSource = modelObject.cast<ConstructionWithInternalSource>();
        retVal = translateConstructionWithInternalSource(constructionIntSource);
        break;
      }
      case openstudio::IddObjectType::OS_Construction_FfactorGroundFloor: {
        auto mo = modelObject.cast<FFactorGroundFloorConstruction>();
        retVal = translateFFactorGroundFloorConstruction(mo);
        break;
      }
      case openstudio::IddObjectType::OS_Construction_WindowDataFile: {
        LOG(Warn, "OS_WindowDataFile is not currently translated");
        break;
      }
      case openstudio::IddObjectType::OS_Controller_MechanicalVentilation: {
        auto controller = modelObject.cast<ControllerMechanicalVentilation>();
        retVal = translateControllerMechanicalVentilation(controller);
        break;
      }
      case openstudio::IddObjectType::OS_Controller_OutdoorAir: {
        auto controller = modelObject.cast<ControllerOutdoorAir>();
        retVal = translateControllerOutdoorAir(controller);
        break;
      }
      case openstudio::IddObjectType::OS_Controller_WaterCoil: {
        auto controller = modelObject.cast<ControllerWaterCoil>();
        retVal = translateControllerWaterCoil(controller);
        break;
      }
      case openstudio::IddObjectType::OS_CoolingTowerPerformance_CoolTools: {
        auto mo = modelObject.cast<CoolingTowerPerformanceCoolTools>();
        retVal = translateCoolingTowerPerformanceCoolTools(mo);
        break;
      }
      case openstudio::IddObjectType::OS_CoolingTowerPerformance_YorkCalc: {
        auto mo = modelObject.cast<CoolingTowerPerformanceYorkCalc>();
        retVal = translateCoolingTowerPerformanceYorkCalc(mo);
        break;
      }
      case openstudio::IddObjectType::OS_CoolingTower_SingleSpeed: {
        auto tower = modelObject.cast<CoolingTowerSingleSpeed>();
        retVal = translateCoolingTowerSingleSpeed(tower);
        break;
      }
      case openstudio::IddObjectType::OS_CoolingTower_TwoSpeed: {
        auto tower = modelObject.cast<CoolingTowerTwoSpeed>();
        retVal = translateCoolingTowerTwoSpeed(tower);
        break;
      }
      case openstudio::IddObjectType::OS_CoolingTower_VariableSpeed: {
        auto tower = modelObject.cast<CoolingTowerVariableSpeed>();
        retVal = translateCoolingTowerVariableSpeed(tower);
        break;
      }
      case openstudio::IddObjectType::OS_CurrencyType: {
        auto mo = modelObject.cast<CurrencyType>();
        retVal = translateCurrencyType(mo);
        break;
      }
      case openstudio::IddObjectType::OS_Curve_Bicubic: {
        auto curve = modelObject.cast<CurveBicubic>();
        retVal = translateCurveBicubic(curve);
        break;
      }
      case openstudio::IddObjectType::OS_Curve_Biquadratic: {
        auto curve = modelObject.cast<CurveBiquadratic>();
        retVal = translateCurveBiquadratic(curve);
        break;
      }
      case openstudio::IddObjectType::OS_Curve_Cubic: {
        auto curve = modelObject.cast<CurveCubic>();
        retVal = translateCurveCubic(curve);
        break;
      }
      case openstudio::IddObjectType::OS_Curve_DoubleExponentialDecay: {
        auto curve = modelObject.cast<CurveDoubleExponentialDecay>();
        retVal = translateCurveDoubleExponentialDecay(curve);
        break;
      }
      case openstudio::IddObjectType::OS_Curve_Exponent: {
        auto curve = modelObject.cast<CurveExponent>();
        retVal = translateCurveExponent(curve);
        break;
      }
      case openstudio::IddObjectType::OS_Curve_ExponentialDecay: {
        auto curve = modelObject.cast<CurveExponentialDecay>();
        retVal = translateCurveExponentialDecay(curve);
        break;
      }
      case openstudio::IddObjectType::OS_Curve_ExponentialSkewNormal: {
        auto curve = modelObject.cast<CurveExponentialSkewNormal>();
        retVal = translateCurveExponentialSkewNormal(curve);
        break;
      }
      case openstudio::IddObjectType::OS_Curve_FanPressureRise: {
        auto curve = modelObject.cast<CurveFanPressureRise>();
        retVal = translateCurveFanPressureRise(curve);
        break;
      }
      case openstudio::IddObjectType::OS_Curve_Functional_PressureDrop: {
        auto curve = modelObject.cast<CurveFunctionalPressureDrop>();
        retVal = translateCurveFunctionalPressureDrop(curve);
        break;
      }
      case openstudio::IddObjectType::OS_Curve_Linear: {
        auto curve = modelObject.cast<CurveLinear>();
        retVal = translateCurveLinear(curve);
        break;
      }
      case openstudio::IddObjectType::OS_Curve_QuadLinear: {
        auto curve = modelObject.cast<CurveQuadLinear>();
        retVal = translateCurveQuadLinear(curve);
        break;
      }
      case openstudio::IddObjectType::OS_Curve_QuintLinear: {
        auto curve = modelObject.cast<CurveQuintLinear>();
        retVal = translateCurveQuintLinear(curve);
        break;
      }
      case openstudio::IddObjectType::OS_Curve_Quadratic: {
        auto curve = modelObject.cast<CurveQuadratic>();
        retVal = translateCurveQuadratic(curve);
        break;
      }
      case openstudio::IddObjectType::OS_Curve_QuadraticLinear: {
        auto curve = modelObject.cast<CurveQuadraticLinear>();
        retVal = translateCurveQuadraticLinear(curve);
        break;
      }
      case openstudio::IddObjectType::OS_Curve_Quartic: {
        auto curve = modelObject.cast<CurveQuartic>();
        retVal = translateCurveQuartic(curve);
        break;
      }
      case openstudio::IddObjectType::OS_Curve_RectangularHyperbola1: {
        auto curve = modelObject.cast<CurveRectangularHyperbola1>();
        retVal = translateCurveRectangularHyperbola1(curve);
        break;
      }
      case openstudio::IddObjectType::OS_Curve_RectangularHyperbola2: {
        auto curve = modelObject.cast<CurveRectangularHyperbola2>();
        retVal = translateCurveRectangularHyperbola2(curve);
        break;
      }
      case openstudio::IddObjectType::OS_Curve_Sigmoid: {
        auto curve = modelObject.cast<CurveSigmoid>();
        retVal = translateCurveSigmoid(curve);
        break;
      }
      case openstudio::IddObjectType::OS_Curve_Triquadratic: {
        auto curve = modelObject.cast<CurveTriquadratic>();
        retVal = translateCurveTriquadratic(curve);
        break;
      }
      case openstudio::IddObjectType::OS_SizingPeriod_DesignDay: {
        auto designDay = modelObject.cast<DesignDay>();
        retVal = translateDesignDay(designDay);
        break;
      }
      case openstudio::IddObjectType::OS_SizingPeriod_WeatherFileConditionType: {
        LOG(Warn, "OS_SizingPeriod_WeatherFileConditionType is not currently translated");
        return retVal;
      }
      case openstudio::IddObjectType::OS_SizingPeriod_WeatherFileDays: {
        LOG(Warn, "OS_SizingPeriod_WeatherFileDays is not currently translated");
        return retVal;
      }
      case openstudio::IddObjectType::OS_Sizing_Plant: {
        auto sizingPlant = modelObject.cast<SizingPlant>();
        retVal = translateSizingPlant(sizingPlant);
        break;
      }
      case openstudio::IddObjectType::OS_Sizing_System: {
        auto mo = modelObject.cast<SizingSystem>();
        retVal = translateSizingSystem(mo);
        break;
      }
      case openstudio::IddObjectType::OS_Sizing_Zone: {
        auto mo = modelObject.cast<SizingZone>();
        retVal = translateSizingZone(mo);
        break;
      }
      case openstudio::IddObjectType::OS_Daylighting_Control: {
        auto daylightingControl = modelObject.cast<DaylightingControl>();
        retVal = translateDaylightingControl(daylightingControl);
        break;
      }

      case openstudio::IddObjectType::OS_DaylightingDevice_Shelf: {
        auto daylightingDeviceShelf = modelObject.cast<DaylightingDeviceShelf>();
        retVal = translateDaylightingDeviceShelf(daylightingDeviceShelf);
        break;
      }
      case openstudio::IddObjectType::OS_DaylightingDevice_Tubular: {
        auto daylightingDeviceTubular = modelObject.cast<DaylightingDeviceTubular>();
        retVal = translateDaylightingDeviceTubular(daylightingDeviceTubular);
        break;
      }
      case openstudio::IddObjectType::OS_DaylightingDevice_LightWell: {
        auto daylightingDeviceLightWell = modelObject.cast<DaylightingDeviceLightWell>();
        retVal = translateDaylightingDeviceLightWell(daylightingDeviceLightWell);
        break;
      }
      case openstudio::IddObjectType::OS_DefaultConstructionSet: {
        // no-op
        return retVal;
      }
      case openstudio::IddObjectType::OS_DefaultScheduleSet: {
        // no-op
        return retVal;
      }
      case openstudio::IddObjectType::OS_DefaultSurfaceConstructions: {
        // no-op
        return retVal;
      }
      case openstudio::IddObjectType::OS_DefaultSubSurfaceConstructions: {
        // no-op
        return retVal;
      }
      case openstudio::IddObjectType::OS_DesignSpecification_OutdoorAir: {
        auto designSpecificationOutdoorAir = modelObject.cast<DesignSpecificationOutdoorAir>();
        retVal = translateDesignSpecificationOutdoorAir(designSpecificationOutdoorAir);
        break;
      }
      case openstudio::IddObjectType::OS_DesignSpecification_ZoneAirDistribution: {
        // DLM: appears to be translated in SizingZone
        break;
      }
      case openstudio::IddObjectType::OS_DistrictCooling: {
        auto districtCooling = modelObject.cast<DistrictCooling>();
        retVal = translateDistrictCooling(districtCooling);
        break;
      }
      case openstudio::IddObjectType::OS_DistrictHeating_Water: {
        auto districtHeating = modelObject.cast<DistrictHeatingWater>();
        retVal = translateDistrictHeatingWater(districtHeating);
        break;
      }
      case openstudio::IddObjectType::OS_DistrictHeating_Steam: {
        auto districtHeatingSteam = modelObject.cast<DistrictHeatingSteam>();
        retVal = translateDistrictHeatingSteam(districtHeatingSteam);
        break;
      }
      case openstudio::IddObjectType::OS_Duct: {
        auto duct = modelObject.cast<Duct>();
        retVal = translateDuct(duct);
        break;
      }
      case openstudio::IddObjectType::OS_ElectricEquipment: {
        auto equipment = modelObject.cast<ElectricEquipment>();
        retVal = translateElectricEquipment(equipment);
        break;
      }
      case openstudio::IddObjectType::OS_ElectricEquipment_Definition: {
        // no-op
        break;
      }
      case openstudio::IddObjectType::OS_ElectricEquipment_ITE_AirCooled: {
        auto equipmentITE = modelObject.cast<ElectricEquipmentITEAirCooled>();
        retVal = translateElectricEquipmentITEAirCooled(equipmentITE);
        break;
      }
      case openstudio::IddObjectType::OS_ElectricEquipment_ITE_AirCooled_Definition: {
        // no-op
        break;
      }
      case openstudio::IddObjectType::OS_ElectricLoadCenter_Distribution: {
        auto temp = modelObject.cast<ElectricLoadCenterDistribution>();
        retVal = translateElectricLoadCenterDistribution(temp);
        break;
      }
      case openstudio::IddObjectType::OS_ElectricLoadCenter_Inverter_LookUpTable: {
        auto temp = modelObject.cast<ElectricLoadCenterInverterLookUpTable>();
        retVal = translateElectricLoadCenterInverterLookUpTable(temp);
        break;
      }
      case openstudio::IddObjectType::OS_ElectricLoadCenter_Inverter_Simple: {
        auto temp = modelObject.cast<ElectricLoadCenterInverterSimple>();
        retVal = translateElectricLoadCenterInverterSimple(temp);
        break;
      }
      case openstudio::IddObjectType::OS_ElectricLoadCenter_Inverter_PVWatts: {
        auto temp = modelObject.cast<ElectricLoadCenterInverterPVWatts>();
        retVal = translateElectricLoadCenterInverterPVWatts(temp);
        break;
      }
      case openstudio::IddObjectType::OS_ElectricLoadCenter_Storage_Simple: {
        auto temp = modelObject.cast<ElectricLoadCenterStorageSimple>();
        retVal = translateElectricLoadCenterStorageSimple(temp);
        break;
      }
      case openstudio::IddObjectType::OS_ElectricLoadCenter_Storage_Converter: {
        auto temp = modelObject.cast<ElectricLoadCenterStorageConverter>();
        retVal = translateElectricLoadCenterStorageConverter(temp);
        break;
      }
      case openstudio::IddObjectType::OS_ElectricLoadCenter_Storage_LiIonNMCBattery: {
        auto temp = modelObject.cast<ElectricLoadCenterStorageLiIonNMCBattery>();
        retVal = translateElectricLoadCenterStorageLiIonNMCBattery(temp);
        break;
      }
      case openstudio::IddObjectType::OS_ElectricLoadCenter_Transformer: {
        auto temp = modelObject.cast<ElectricLoadCenterTransformer>();
        retVal = translateElectricLoadCenterTransformer(temp);
        break;
      }
      case openstudio::IddObjectType::OS_EnergyManagementSystem_Actuator: {
        auto actuator = modelObject.cast<EnergyManagementSystemActuator>();
        retVal = translateEnergyManagementSystemActuator(actuator);
        break;
      }
      case openstudio::IddObjectType::OS_EnergyManagementSystem_ConstructionIndexVariable: {
        auto civ = modelObject.cast<EnergyManagementSystemConstructionIndexVariable>();
        retVal = translateEnergyManagementSystemConstructionIndexVariable(civ);
        break;
      }
      case openstudio::IddObjectType::OS_EnergyManagementSystem_CurveOrTableIndexVariable: {
        auto cotiv = modelObject.cast<EnergyManagementSystemCurveOrTableIndexVariable>();
        retVal = translateEnergyManagementSystemCurveOrTableIndexVariable(cotiv);
        break;
      }
      case openstudio::IddObjectType::OS_EnergyManagementSystem_GlobalVariable: {
        auto globalVariable = modelObject.cast<EnergyManagementSystemGlobalVariable>();
        retVal = translateEnergyManagementSystemGlobalVariable(globalVariable);
        break;
      }
      case openstudio::IddObjectType::OS_EnergyManagementSystem_InternalVariable: {
        auto internalVariable = modelObject.cast<EnergyManagementSystemInternalVariable>();
        retVal = translateEnergyManagementSystemInternalVariable(internalVariable);
        break;
      }
      case openstudio::IddObjectType::OS_EnergyManagementSystem_MeteredOutputVariable: {
        auto mov = modelObject.cast<EnergyManagementSystemMeteredOutputVariable>();
        retVal = translateEnergyManagementSystemMeteredOutputVariable(mov);
        break;
      }
      case openstudio::IddObjectType::OS_EnergyManagementSystem_OutputVariable: {
        auto outputVariable = modelObject.cast<EnergyManagementSystemOutputVariable>();
        retVal = translateEnergyManagementSystemOutputVariable(outputVariable);
        break;
      }
      case openstudio::IddObjectType::OS_EnergyManagementSystem_Program: {
        auto program = modelObject.cast<EnergyManagementSystemProgram>();
        retVal = translateEnergyManagementSystemProgram(program);
        break;
      }
      case openstudio::IddObjectType::OS_EnergyManagementSystem_ProgramCallingManager: {
        auto programCallingManager = modelObject.cast<EnergyManagementSystemProgramCallingManager>();
        retVal = translateEnergyManagementSystemProgramCallingManager(programCallingManager);
        break;
      }
      case openstudio::IddObjectType::OS_EnergyManagementSystem_Sensor: {
        auto sensor = modelObject.cast<EnergyManagementSystemSensor>();
        retVal = translateEnergyManagementSystemSensor(sensor);
        break;
      }
      case openstudio::IddObjectType::OS_EnergyManagementSystem_Subroutine: {
        auto subroutine = modelObject.cast<EnergyManagementSystemSubroutine>();
        retVal = translateEnergyManagementSystemSubroutine(subroutine);
        break;
      }
      case openstudio::IddObjectType::OS_EnergyManagementSystem_TrendVariable: {
        auto trendVariable = modelObject.cast<EnergyManagementSystemTrendVariable>();
        retVal = translateEnergyManagementSystemTrendVariable(trendVariable);
        break;
      }
      case openstudio::IddObjectType::OS_EvaporativeCooler_Direct_ResearchSpecial: {
        auto evap = modelObject.cast<EvaporativeCoolerDirectResearchSpecial>();
        retVal = translateEvaporativeCoolerDirectResearchSpecial(evap);
        break;
      }
      case openstudio::IddObjectType::OS_EvaporativeCooler_Indirect_ResearchSpecial: {
        auto evap = modelObject.cast<EvaporativeCoolerIndirectResearchSpecial>();
        retVal = translateEvaporativeCoolerIndirectResearchSpecial(evap);
        break;
      }
      case openstudio::IddObjectType::OS_EvaporativeFluidCooler_SingleSpeed: {
        auto evap = modelObject.cast<EvaporativeFluidCoolerSingleSpeed>();
        retVal = translateEvaporativeFluidCoolerSingleSpeed(evap);
        break;
      }
      case openstudio::IddObjectType::OS_EvaporativeFluidCooler_TwoSpeed: {
        auto mo = modelObject.cast<EvaporativeFluidCoolerTwoSpeed>();
        retVal = translateEvaporativeFluidCoolerTwoSpeed(mo);
        break;
      }
      case openstudio::IddObjectType::OS_Exterior_Lights: {
        auto lights = modelObject.cast<ExteriorLights>();
        retVal = translateExteriorLights(lights);
        break;
      }
      case openstudio::IddObjectType::OS_Exterior_Lights_Definition: {
        // no-op
        break;
      }

      case openstudio::IddObjectType::OS_Exterior_FuelEquipment: {
        auto mo = modelObject.cast<ExteriorFuelEquipment>();
        retVal = translateExteriorFuelEquipment(mo);
        break;
      }
      case openstudio::IddObjectType::OS_Exterior_FuelEquipment_Definition: {
        // no-op
        break;
      }

      case openstudio::IddObjectType::OS_Exterior_WaterEquipment: {
        auto mo = modelObject.cast<ExteriorWaterEquipment>();
        retVal = translateExteriorWaterEquipment(mo);
        break;
      }
      case openstudio::IddObjectType::OS_Exterior_WaterEquipment_Definition: {
        // no-op
        break;
      }

      case openstudio::IddObjectType::OS_ExternalInterface: {
        auto ei = modelObject.cast<ExternalInterface>();
        retVal = translateExternalInterface(ei);
        break;
      }
      case openstudio::IddObjectType::OS_ExternalInterface_Actuator: {
        auto ei = modelObject.cast<ExternalInterfaceActuator>();
        retVal = translateExternalInterfaceActuator(ei);
        break;
      }
      case openstudio::IddObjectType::OS_ExternalInterface_FunctionalMockupUnitExport_From_Variable: {
        auto ei = modelObject.cast<ExternalInterfaceFunctionalMockupUnitExportFromVariable>();
        retVal = translateExternalInterfaceFunctionalMockupUnitExportFromVariable(ei);
        break;
      }
      case openstudio::IddObjectType::OS_ExternalInterface_FunctionalMockupUnitExport_To_Actuator: {
        auto ei = modelObject.cast<ExternalInterfaceFunctionalMockupUnitExportToActuator>();
        retVal = translateExternalInterfaceFunctionalMockupUnitExportToActuator(ei);
        break;
      }
      case openstudio::IddObjectType::OS_ExternalInterface_FunctionalMockupUnitExport_To_Schedule: {
        auto ei = modelObject.cast<ExternalInterfaceFunctionalMockupUnitExportToSchedule>();
        retVal = translateExternalInterfaceFunctionalMockupUnitExportToSchedule(ei);
        break;
      }
      case openstudio::IddObjectType::OS_ExternalInterface_FunctionalMockupUnitExport_To_Variable: {
        auto ei = modelObject.cast<ExternalInterfaceFunctionalMockupUnitExportToVariable>();
        retVal = translateExternalInterfaceFunctionalMockupUnitExportToVariable(ei);
        break;
      }
      case openstudio::IddObjectType::OS_ExternalInterface_FunctionalMockupUnitImport: {
        auto ei = modelObject.cast<ExternalInterfaceFunctionalMockupUnitImport>();
        retVal = translateExternalInterfaceFunctionalMockupUnitImport(ei);
        break;
      }
      case openstudio::IddObjectType::OS_ExternalInterface_FunctionalMockupUnitImport_From_Variable: {
        auto ei = modelObject.cast<ExternalInterfaceFunctionalMockupUnitImportFromVariable>();
        retVal = translateExternalInterfaceFunctionalMockupUnitImportFromVariable(ei);
        break;
      }
      case openstudio::IddObjectType::OS_ExternalInterface_FunctionalMockupUnitImport_To_Actuator: {
        auto ei = modelObject.cast<ExternalInterfaceFunctionalMockupUnitImportToActuator>();
        retVal = translateExternalInterfaceFunctionalMockupUnitImportToActuator(ei);
        break;
      }
      case openstudio::IddObjectType::OS_ExternalInterface_FunctionalMockupUnitImport_To_Schedule: {
        auto ei = modelObject.cast<ExternalInterfaceFunctionalMockupUnitImportToSchedule>();
        retVal = translateExternalInterfaceFunctionalMockupUnitImportToSchedule(ei);
        break;
      }
      case openstudio::IddObjectType::OS_ExternalInterface_FunctionalMockupUnitImport_To_Variable: {
        auto ei = modelObject.cast<ExternalInterfaceFunctionalMockupUnitImportToVariable>();
        retVal = translateExternalInterfaceFunctionalMockupUnitImportToVariable(ei);
        break;
      }
      case openstudio::IddObjectType::OS_ExternalInterface_Schedule: {
        auto ei = modelObject.cast<ExternalInterfaceSchedule>();
        retVal = translateExternalInterfaceSchedule(ei);
        break;
      }
      case openstudio::IddObjectType::OS_ExternalInterface_Variable: {
        auto ei = modelObject.cast<ExternalInterfaceVariable>();
        retVal = translateExternalInterfaceVariable(ei);
        break;
      }
      case openstudio::IddObjectType::OS_Facility: {
        // no-op
        return retVal;
      }
      case openstudio::IddObjectType::OS_Fan_ComponentModel: {
        auto fan = modelObject.cast<FanComponentModel>();
        retVal = translateFanComponentModel(fan);
        break;
      }
      case openstudio::IddObjectType::OS_Fan_ConstantVolume: {
        auto fan = modelObject.cast<FanConstantVolume>();
        retVal = translateFanConstantVolume(fan);
        break;
      }
      case openstudio::IddObjectType::OS_Fan_OnOff: {
        auto fan = modelObject.cast<FanOnOff>();
        retVal = translateFanOnOff(fan);
        break;
      }
      case openstudio::IddObjectType::OS_Fan_SystemModel: {
        auto fan = modelObject.cast<FanSystemModel>();
        retVal = translateFanSystemModel(fan);
        break;
      }
      case openstudio::IddObjectType::OS_Fan_VariableVolume: {
        auto fan = modelObject.cast<FanVariableVolume>();
        retVal = translateFanVariableVolume(fan);
        break;
      }

      case openstudio::IddObjectType::OS_Fan_ZoneExhaust: {
        auto fan = modelObject.cast<FanZoneExhaust>();
        retVal = translateFanZoneExhaust(fan);
        break;
      }
      case openstudio::IddObjectType::OS_FluidCooler_SingleSpeed: {
        auto fluidCoolerSingleSpeed = modelObject.cast<FluidCoolerSingleSpeed>();
        retVal = translateFluidCoolerSingleSpeed(fluidCoolerSingleSpeed);
        break;
      }
      case openstudio::IddObjectType::OS_FluidCooler_TwoSpeed: {
        auto fluidCoolerTwoSpeed = modelObject.cast<FluidCoolerTwoSpeed>();
        retVal = translateFluidCoolerTwoSpeed(fluidCoolerTwoSpeed);
        break;
      }
      case openstudio::IddObjectType::OS_Foundation_Kiva: {
        auto kiva = modelObject.cast<FoundationKiva>();
        retVal = translateFoundationKiva(kiva);
        break;
      }
      case openstudio::IddObjectType::OS_Foundation_Kiva_Settings: {
        auto kivaSettings = modelObject.cast<FoundationKivaSettings>();
        retVal = translateFoundationKivaSettings(kivaSettings);
        break;
      }
      case openstudio::IddObjectType::OS_Generator_MicroTurbine: {
        // Will also translate the Generator:MicroTurbine:HeatRecovery if there is one attached to the Generator:MicroTurbine
        auto mchp = modelObject.cast<GeneratorMicroTurbine>();
        retVal = translateGeneratorMicroTurbine(mchp);
        break;
      }
      case openstudio::IddObjectType::OS_Generator_MicroTurbine_HeatRecovery: {
        // no-op, just Log a Trace message
        LOG(Trace, "OS_Generator_MicroTurbine_HeatRecovery is not translated by itself but in the parent GeneratorMicroTurbine");
        break;
      }
      case openstudio::IddObjectType::OS_Generator_FuelCell: {
        // Will also translate all children of Generator:FuelCell (Generator:FuelCell:AirSupply, etc)
        auto temp = modelObject.cast<GeneratorFuelCell>();
        retVal = translateGeneratorFuelCell(temp);
        break;
      }
      case openstudio::IddObjectType::OS_Generator_FuelCell_AirSupply: {
        auto temp = modelObject.cast<GeneratorFuelCellAirSupply>();
        retVal = translateGeneratorFuelCellAirSupply(temp);
        break;
      }
      case openstudio::IddObjectType::OS_Generator_FuelCell_AuxiliaryHeater: {
        auto temp = modelObject.cast<GeneratorFuelCellAuxiliaryHeater>();
        retVal = translateGeneratorFuelCellAuxiliaryHeater(temp);
        break;
      }
      case openstudio::IddObjectType::OS_Generator_FuelCell_ElectricalStorage: {
        auto temp = modelObject.cast<GeneratorFuelCellElectricalStorage>();
        retVal = translateGeneratorFuelCellElectricalStorage(temp);
        break;
      }
      case openstudio::IddObjectType::OS_Generator_FuelCell_ExhaustGasToWaterHeatExchanger: {
        auto temp = modelObject.cast<GeneratorFuelCellExhaustGasToWaterHeatExchanger>();
        retVal = translateGeneratorFuelCellExhaustGasToWaterHeatExchanger(temp);
        break;
      }
      case openstudio::IddObjectType::OS_Generator_FuelCell_Inverter: {
        auto temp = modelObject.cast<GeneratorFuelCellInverter>();
        retVal = translateGeneratorFuelCellInverter(temp);
        break;
      }
      case openstudio::IddObjectType::OS_Generator_FuelCell_PowerModule: {
        auto temp = modelObject.cast<GeneratorFuelCellPowerModule>();
        retVal = translateGeneratorFuelCellPowerModule(temp);
        break;
      }
      case openstudio::IddObjectType::OS_Generator_FuelCell_StackCooler: {
        auto temp = modelObject.cast<GeneratorFuelCellStackCooler>();
        retVal = translateGeneratorFuelCellStackCooler(temp);
        break;
      }
      case openstudio::IddObjectType::OS_Generator_FuelCell_WaterSupply: {
        auto temp = modelObject.cast<GeneratorFuelCellWaterSupply>();
        retVal = translateGeneratorFuelCellWaterSupply(temp);
        break;
      }
      case openstudio::IddObjectType::OS_Generator_FuelSupply: {
        auto temp = modelObject.cast<GeneratorFuelSupply>();
        retVal = translateGeneratorFuelSupply(temp);
        break;
      }
      // TODO: JM 2018-10-12 Placeholder for Generator:MicroCHP if/when it is implemented
      // to remember that translateGeneratorMicroCHP should be calling translation of child GeneratorFuelSupply as needed
      /*
   *case openstudio::IddObjectType::OS_Generator_MicroCHP:
   *{
   *  auto temp = modelObject.cast<GeneratorMicroCHP>();
   *  retVal = translateGeneratorMicroCHP(temp);
   *  break;
   *}
   */
      case openstudio::IddObjectType::OS_Generator_Photovoltaic: {
        auto temp = modelObject.cast<GeneratorPhotovoltaic>();
        retVal = translateGeneratorPhotovoltaic(temp);
        break;
      }
      case openstudio::IddObjectType::OS_Generator_PVWatts: {
        auto temp = modelObject.cast<GeneratorPVWatts>();
        retVal = translateGeneratorPVWatts(temp);
        break;
      }
      case openstudio::IddObjectType::OS_Generator_WindTurbine: {
        auto temp = modelObject.cast<GeneratorWindTurbine>();
        retVal = translateGeneratorWindTurbine(temp);
        break;
      }
      case openstudio::IddObjectType::OS_Glare_Sensor: {
        // no-op
        break;
      }
      case openstudio::IddObjectType::OS_GroundHeatExchanger_HorizontalTrench: {
        auto mo = modelObject.cast<GroundHeatExchangerHorizontalTrench>();
        retVal = translateGroundHeatExchangerHorizontalTrench(mo);
        break;
      }
      case openstudio::IddObjectType::OS_GroundHeatExchanger_Vertical: {
        auto groundHeatExchangerVertical = modelObject.cast<GroundHeatExchangerVertical>();
        retVal = translateGroundHeatExchangerVertical(groundHeatExchangerVertical);
        break;
      }
      case openstudio::IddObjectType::OS_HeaderedPumps_ConstantSpeed: {
        auto mo = modelObject.cast<HeaderedPumpsConstantSpeed>();
        retVal = translateHeaderedPumpsConstantSpeed(mo);
        break;
      }
      case openstudio::IddObjectType::OS_HeaderedPumps_VariableSpeed: {
        auto mo = modelObject.cast<HeaderedPumpsVariableSpeed>();
        retVal = translateHeaderedPumpsVariableSpeed(mo);
        break;
      }
      case openstudio::IddObjectType::OS_HeatBalanceAlgorithm: {
        auto mo = modelObject.cast<HeatBalanceAlgorithm>();
        retVal = translateHeatBalanceAlgorithm(mo);
        break;
      }
      case openstudio::IddObjectType::OS_HeatExchanger_AirToAir_SensibleAndLatent: {
        auto mo = modelObject.cast<HeatExchangerAirToAirSensibleAndLatent>();
        retVal = translateHeatExchangerAirToAirSensibleAndLatent(mo);
        break;
      }
      case openstudio::IddObjectType::OS_HeatExchanger_Desiccant_BalancedFlow: {
        auto mo = modelObject.cast<HeatExchangerDesiccantBalancedFlow>();
        retVal = translateHeatExchangerDesiccantBalancedFlow(mo);
        break;
      }
      case openstudio::IddObjectType::OS_HeatExchanger_Desiccant_BalancedFlow_PerformanceDataType1: {
        auto mo = modelObject.cast<HeatExchangerDesiccantBalancedFlowPerformanceDataType1>();
        retVal = translateHeatExchangerDesiccantBalancedFlowPerformanceDataType1(mo);
        break;
      }
      case openstudio::IddObjectType::OS_HeatExchanger_FluidToFluid: {
        auto mo = modelObject.cast<HeatExchangerFluidToFluid>();
        retVal = translateHeatExchangerFluidToFluid(mo);
        break;
      }

      case openstudio::IddObjectType::OS_HeatPump_AirToWater_FuelFired_Heating: {
        auto mo = modelObject.cast<HeatPumpAirToWaterFuelFiredHeating>();
        retVal = translateHeatPumpAirToWaterFuelFiredHeating(mo);
        break;
      }
      case openstudio::IddObjectType::OS_HeatPump_AirToWater_FuelFired_Cooling: {
        auto mo = modelObject.cast<HeatPumpAirToWaterFuelFiredCooling>();
        retVal = translateHeatPumpAirToWaterFuelFiredCooling(mo);
        break;
      }

      case openstudio::IddObjectType::OS_HeatPump_WaterToWater_EquationFit_Cooling: {
        auto mo = modelObject.cast<HeatPumpWaterToWaterEquationFitCooling>();
        retVal = translateHeatPumpWaterToWaterEquationFitCooling(mo);
        break;
      }
      case openstudio::IddObjectType::OS_HeatPump_WaterToWater_EquationFit_Heating: {
        auto mo = modelObject.cast<HeatPumpWaterToWaterEquationFitHeating>();
        retVal = translateHeatPumpWaterToWaterEquationFitHeating(mo);
        break;
      }
      case openstudio::IddObjectType::OS_HeatPump_PlantLoop_EIR_Cooling: {
        auto mo = modelObject.cast<HeatPumpPlantLoopEIRCooling>();
        retVal = translateHeatPumpPlantLoopEIRCooling(mo);
        break;
      }
      case openstudio::IddObjectType::OS_HeatPump_PlantLoop_EIR_Heating: {
        auto mo = modelObject.cast<HeatPumpPlantLoopEIRHeating>();
        retVal = translateHeatPumpPlantLoopEIRHeating(mo);
        break;
      }
      case openstudio::IddObjectType::OS_HotWaterEquipment: {
        auto hotWaterEquipment = modelObject.cast<HotWaterEquipment>();
        retVal = translateHotWaterEquipment(hotWaterEquipment);
        break;
      }
      case openstudio::IddObjectType::OS_HotWaterEquipment_Definition: {
        // no-op
        break;
      }
      case openstudio::IddObjectType::OS_Humidifier_Steam_Electric: {
        auto humidifierSteamElectric = modelObject.cast<HumidifierSteamElectric>();
        retVal = translateHumidifierSteamElectric(humidifierSteamElectric);
        break;
      }
      case openstudio::IddObjectType::OS_Humidifier_Steam_Gas: {
        auto humidifierSteamGas = modelObject.cast<HumidifierSteamGas>();
        retVal = translateHumidifierSteamGas(humidifierSteamGas);
        break;
      }
      case openstudio::IddObjectType::OS_WindowMaterial_Gas: {
        auto gas = modelObject.cast<Gas>();
        retVal = translateGas(gas);
        break;
      }
      case openstudio::IddObjectType::OS_GasEquipment: {
        auto equipment = modelObject.cast<GasEquipment>();
        retVal = translateGasEquipment(equipment);
        break;
      }
      case openstudio::IddObjectType::OS_GasEquipment_Definition: {
        // no-op
        break;
      }
      case openstudio::IddObjectType::OS_IlluminanceMap: {
        auto illuminanceMap = modelObject.cast<IlluminanceMap>();
        retVal = translateIlluminanceMap(illuminanceMap);
        break;
      }
      case openstudio::IddObjectType::OS_InteriorPartitionSurface: {
        auto interiorPartitionSurface = modelObject.cast<InteriorPartitionSurface>();
        retVal = translateInteriorPartitionSurface(interiorPartitionSurface);
        break;
      }
      case openstudio::IddObjectType::OS_InteriorPartitionSurfaceGroup: {
        auto interiorPartitionSurfaceGroup = modelObject.cast<InteriorPartitionSurfaceGroup>();
        retVal = translateInteriorPartitionSurfaceGroup(interiorPartitionSurfaceGroup);
        break;
      }
      case openstudio::IddObjectType::OS_InternalMass: {
        auto internalMass = modelObject.cast<InternalMass>();
        retVal = translateInternalMass(internalMass);
        break;
      }
      case openstudio::IddObjectType::OS_InternalMass_Definition: {
        // no-op
        break;
      }
      case openstudio::IddObjectType::OS_WaterHeater_Mixed: {
        auto waterHeaterMixed = modelObject.cast<WaterHeaterMixed>();
        retVal = translateWaterHeaterMixed(waterHeaterMixed);
        break;
      }
      case openstudio::IddObjectType::OS_WaterHeater_HeatPump: {
        auto mo = modelObject.cast<WaterHeaterHeatPump>();
        retVal = translateWaterHeaterHeatPump(mo);
        break;
      }
      case openstudio::IddObjectType::OS_WaterHeater_HeatPump_WrappedCondenser: {
        auto mo = modelObject.cast<WaterHeaterHeatPumpWrappedCondenser>();
        retVal = translateWaterHeaterHeatPumpWrappedCondenser(mo);
        break;
      }
      case openstudio::IddObjectType::OS_WaterHeater_Sizing: {
        auto waterHeaterSizing = modelObject.cast<WaterHeaterSizing>();
        retVal = translateWaterHeaterSizing(waterHeaterSizing);
        break;
      }
      case openstudio::IddObjectType::OS_WaterHeater_Stratified: {
        auto waterHeaterStratified = modelObject.cast<WaterHeaterStratified>();
        retVal = translateWaterHeaterStratified(waterHeaterStratified);
        break;
      }
      case openstudio::IddObjectType::OS_WaterUse_Connections: {
        auto waterUseConnections = modelObject.cast<WaterUseConnections>();
        retVal = translateWaterUseConnections(waterUseConnections);
        break;
      }
      case openstudio::IddObjectType::OS_WaterUse_Equipment: {
        auto waterUseEquipment = modelObject.cast<WaterUseEquipment>();
        retVal = translateWaterUseEquipment(waterUseEquipment);
        break;
      }
      case openstudio::IddObjectType::OS_WaterUse_Equipment_Definition: {
        // no-op
        break;
      }
      case openstudio::IddObjectType::OS_WindowMaterial_GasMixture: {
        auto gasMixture = modelObject.cast<GasMixture>();
        retVal = translateGasMixture(gasMixture);
        break;
      }
      case openstudio::IddObjectType::OS_LoadProfile_Plant: {
        auto mo = modelObject.cast<LoadProfilePlant>();
        retVal = translateLoadProfilePlant(mo);
        break;
      }
      case openstudio::IddObjectType::OS_LifeCycleCost: {
        auto lifeCycleCost = modelObject.cast<LifeCycleCost>();
        retVal = translateLifeCycleCost(lifeCycleCost);
        break;
      }
      case openstudio::IddObjectType::OS_LifeCycleCost_Parameters: {
        auto lifeCycleCostParameters = modelObject.cast<LifeCycleCostParameters>();
        retVal = translateLifeCycleCostParameters(lifeCycleCostParameters);
        break;
      }
      case openstudio::IddObjectType::OS_LifeCycleCost_UsePriceEscalation: {
        // DLM: these objects can be created from LifeCycleCostParameters
        LOG(Warn, "OS:LifeCycleCost:UsePriceEscalation '" << modelObject.name().get() << "' not translated to EnergyPlus.");
        return retVal;
      }
      case openstudio::IddObjectType::OS_LightingDesignDay: {
        // no-op
        break;
      }
      case openstudio::IddObjectType::OS_LightingSimulationControl: {
        // no-op
        break;
      }
      case openstudio::IddObjectType::OS_LightingSimulationZone: {
        // no-op
        break;
      }
      case openstudio::IddObjectType::OS_Lights: {
        auto lights = modelObject.cast<Lights>();
        retVal = translateLights(lights);
        break;
      }
      case openstudio::IddObjectType::OS_Lights_Definition: {
        // no-op
        break;
      }
      case openstudio::IddObjectType::OS_Luminaire: {
        auto luminaire = modelObject.cast<Luminaire>();
        retVal = translateLuminaire(luminaire);
        break;
      }
      case openstudio::IddObjectType::OS_Luminaire_Definition: {
        // no-op
        break;
      }
      case openstudio::IddObjectType::OS_Material: {
        auto material = modelObject.cast<StandardOpaqueMaterial>();
        retVal = translateStandardOpaqueMaterial(material);
        break;
      }
      case openstudio::IddObjectType::OS_Material_AirGap: {
        auto material = modelObject.cast<AirGap>();
        retVal = translateAirGap(material);
        break;
      }
      case openstudio::IddObjectType::OS_Material_InfraredTransparent: {
        auto material = modelObject.cast<InfraredTransparentMaterial>();
        retVal = translateInfraredTransparentMaterial(material);
        break;
      }
      case openstudio::IddObjectType::OS_Material_NoMass: {
        auto material = modelObject.cast<MasslessOpaqueMaterial>();
        retVal = translateMasslessOpaqueMaterial(material);
        break;
      }
      case openstudio::IddObjectType::OS_MaterialProperty_GlazingSpectralData: {
        auto spectralData = modelObject.cast<MaterialPropertyGlazingSpectralData>();
        retVal = translateMaterialPropertyGlazingSpectralData(spectralData);
        break;
      }
      case openstudio::IddObjectType::OS_MaterialProperty_MoisturePenetrationDepth_Settings: {
        auto empd = modelObject.cast<MaterialPropertyMoisturePenetrationDepthSettings>();
        retVal = translateMaterialPropertyMoisturePenetrationDepthSettings(empd);
        break;
      }
      case openstudio::IddObjectType::OS_MaterialProperty_PhaseChange: {
        auto phaseChange = modelObject.cast<MaterialPropertyPhaseChange>();
        retVal = translateMaterialPropertyPhaseChange(phaseChange);
        break;
      }
      case openstudio::IddObjectType::OS_MaterialProperty_PhaseChangeHysteresis: {
        auto phaseChangeHysteresis = modelObject.cast<MaterialPropertyPhaseChangeHysteresis>();
        retVal = translateMaterialPropertyPhaseChangeHysteresis(phaseChangeHysteresis);
        break;
      }
      case openstudio::IddObjectType::OS_Material_RoofVegetation: {
        auto material = modelObject.cast<RoofVegetation>();
        retVal = translateRoofVegetation(material);
        break;
      }
      case openstudio::IddObjectType::OS_Meter_Custom: {
        auto meterCustom = modelObject.cast<MeterCustom>();
        retVal = translateMeterCustom(meterCustom);
        break;
      }
      case openstudio::IddObjectType::OS_Meter_CustomDecrement: {
        auto meterCustomDecrement = modelObject.cast<MeterCustomDecrement>();
        retVal = translateMeterCustomDecrement(meterCustomDecrement);
        break;
      }
      case openstudio::IddObjectType::OS_ModelObjectList: {
        // no-op
        break;
      }
      case openstudio::IddObjectType::OS_Node: {
        auto node = modelObject.cast<Node>();
        retVal = translateNode(node);
        break;
      }
      case openstudio::IddObjectType::OS_OtherEquipment: {
        auto otherEquipment = modelObject.cast<OtherEquipment>();
        retVal = translateOtherEquipment(otherEquipment);
        break;
      }
      case openstudio::IddObjectType::OS_PhotovoltaicPerformance_EquivalentOneDiode: {
        auto temp = modelObject.cast<PhotovoltaicPerformanceEquivalentOneDiode>();
        retVal = translatePhotovoltaicPerformanceEquivalentOneDiode(temp);
        break;
      }
      case openstudio::IddObjectType::OS_PhotovoltaicPerformance_Sandia: {
        auto temp = modelObject.cast<PhotovoltaicPerformanceSandia>();
        retVal = translatePhotovoltaicPerformanceSandia(temp);
        break;
      }
      case openstudio::IddObjectType::OS_PhotovoltaicPerformance_Simple: {
        auto temp = modelObject.cast<PhotovoltaicPerformanceSimple>();
        retVal = translatePhotovoltaicPerformanceSimple(temp);
        break;
      }
      case openstudio::IddObjectType::OS_OtherEquipment_Definition: {
        //no-op
        break;
      }
      case openstudio::IddObjectType::OS_PlantLoop: {
        auto plantLoop = modelObject.cast<PlantLoop>();
        retVal = translatePlantLoop(plantLoop);
        break;
      }
      case openstudio::IddObjectType::OS_PlantComponent_TemperatureSource: {
        auto mo = modelObject.cast<PlantComponentTemperatureSource>();
        retVal = translatePlantComponentTemperatureSource(mo);
        break;
      }
      case openstudio::IddObjectType::OS_PlantComponent_UserDefined: {
        auto mo = modelObject.cast<PlantComponentUserDefined>();
        retVal = translatePlantComponentUserDefined(mo);
        break;
      }
      case openstudio::IddObjectType::OS_PlantEquipmentOperation_CoolingLoad: {
        auto mo = modelObject.cast<PlantEquipmentOperationCoolingLoad>();
        retVal = translatePlantEquipmentOperationCoolingLoad(mo);
        break;
      }
      case openstudio::IddObjectType::OS_PlantEquipmentOperation_HeatingLoad: {
        auto mo = modelObject.cast<PlantEquipmentOperationHeatingLoad>();
        retVal = translatePlantEquipmentOperationHeatingLoad(mo);
        break;
      }
      case openstudio::IddObjectType::OS_PlantEquipmentOperation_OutdoorDryBulb: {
        auto mo = modelObject.cast<PlantEquipmentOperationOutdoorDryBulb>();
        retVal = translatePlantEquipmentOperationOutdoorDryBulb(mo);
        break;
      }
      case openstudio::IddObjectType::OS_PlantEquipmentOperation_OutdoorWetBulb: {
        auto mo = modelObject.cast<PlantEquipmentOperationOutdoorWetBulb>();
        retVal = translatePlantEquipmentOperationOutdoorWetBulb(mo);
        break;
      }
      case openstudio::IddObjectType::OS_PlantEquipmentOperation_OutdoorDewpoint: {
        auto mo = modelObject.cast<PlantEquipmentOperationOutdoorDewpoint>();
        retVal = translatePlantEquipmentOperationOutdoorDewpoint(mo);
        break;
      }
      case openstudio::IddObjectType::OS_PlantEquipmentOperation_OutdoorDryBulbDifference: {
        auto mo = modelObject.cast<PlantEquipmentOperationOutdoorDryBulbDifference>();
        retVal = translatePlantEquipmentOperationOutdoorDryBulbDifference(mo);
        break;
      }
      case openstudio::IddObjectType::OS_PlantEquipmentOperation_OutdoorWetBulbDifference: {
        auto mo = modelObject.cast<PlantEquipmentOperationOutdoorWetBulbDifference>();
        retVal = translatePlantEquipmentOperationOutdoorWetBulbDifference(mo);
        break;
      }
      case openstudio::IddObjectType::OS_PlantEquipmentOperation_OutdoorDewpointDifference: {
        auto mo = modelObject.cast<PlantEquipmentOperationOutdoorDewpointDifference>();
        retVal = translatePlantEquipmentOperationOutdoorDewpointDifference(mo);
        break;
      }
      case openstudio::IddObjectType::OS_PlantEquipmentOperation_OutdoorRelativeHumidity: {
        auto mo = modelObject.cast<PlantEquipmentOperationOutdoorRelativeHumidity>();
        retVal = translatePlantEquipmentOperationOutdoorRelativeHumidity(mo);
        break;
      }
      case openstudio::IddObjectType::OS_Pump_ConstantSpeed: {
        auto pump = modelObject.cast<PumpConstantSpeed>();
        retVal = translatePumpConstantSpeed(pump);
        break;
      }
      case openstudio::IddObjectType::OS_Pump_VariableSpeed: {
        auto pump = modelObject.cast<PumpVariableSpeed>();
        retVal = translatePumpVariableSpeed(pump);
        break;
      }
      case openstudio::IddObjectType::OS_OutputControl_Files: {
        auto outputControlFiles = modelObject.cast<OutputControlFiles>();
        retVal = translateOutputControlFiles(outputControlFiles);
        break;
      }
      case openstudio::IddObjectType::OS_OutputControl_ReportingTolerances: {
        auto outputControlReportingTolerances = modelObject.cast<OutputControlReportingTolerances>();
        retVal = translateOutputControlReportingTolerances(outputControlReportingTolerances);
        break;
      }
      case openstudio::IddObjectType::OS_OutputControl_Table_Style: {
        auto outputControlTableStyle = modelObject.cast<OutputControlTableStyle>();
        retVal = translateOutputControlTableStyle(outputControlTableStyle);
        break;
      }
      case openstudio::IddObjectType::OS_OutputControl_Timestamp: {
        auto outputControlTimestamp = modelObject.cast<OutputControlTimestamp>();
        retVal = translateOutputControlTimestamp(outputControlTimestamp);
        break;
      }
      case openstudio::IddObjectType::OS_Output_DebuggingData: {
        auto mo = modelObject.cast<OutputDebuggingData>();
        retVal = translateOutputDebuggingData(mo);
        break;
      }
      case openstudio::IddObjectType::OS_Output_Diagnostics: {
        auto mo = modelObject.cast<OutputDiagnostics>();
        retVal = translateOutputDiagnostics(mo);
        break;
      }
      case openstudio::IddObjectType::OS_Output_JSON: {
        auto mo = modelObject.cast<OutputJSON>();
        retVal = translateOutputJSON(mo);
        break;
      }
      case openstudio::IddObjectType::OS_Output_SQLite: {
        auto mo = modelObject.cast<OutputSQLite>();
        retVal = translateOutputSQLite(mo);
        break;
      }
      case openstudio::IddObjectType::OS_Output_Schedules: {
        auto mo = modelObject.cast<OutputSchedules>();
        retVal = translateOutputSchedules(mo);
        break;
      }
      case openstudio::IddObjectType::OS_Output_Constructions: {
        auto mo = modelObject.cast<OutputConstructions>();
        retVal = translateOutputConstructions(mo);
        break;
      }
      case openstudio::IddObjectType::OS_Output_EnvironmentalImpactFactors: {
        auto mo = modelObject.cast<OutputEnvironmentalImpactFactors>();
        retVal = translateOutputEnvironmentalImpactFactors(mo);
        break;
      }
      case openstudio::IddObjectType::OS_EnvironmentalImpactFactors: {
        auto mo = modelObject.cast<EnvironmentalImpactFactors>();
        retVal = translateEnvironmentalImpactFactors(mo);
        break;
      }
      case openstudio::IddObjectType::OS_FuelFactors: {
        auto mo = modelObject.cast<FuelFactors>();
        retVal = translateFuelFactors(mo);
        break;
      }
      case openstudio::IddObjectType::OS_Output_Meter: {
        auto meter = modelObject.cast<OutputMeter>();
        retVal = translateOutputMeter(meter);
        break;
      }
      case openstudio::IddObjectType::OS_Output_Variable: {
        auto outputVariable = modelObject.cast<OutputVariable>();
        retVal = translateOutputVariable(outputVariable);
        break;
      }
      case openstudio::IddObjectType::OS_Output_EnergyManagementSystem: {
        auto outputEnergyManagementSystem = modelObject.cast<OutputEnergyManagementSystem>();
        retVal = translateOutputEnergyManagementSystem(outputEnergyManagementSystem);
        break;
      }
      case openstudio::IddObjectType::OS_Output_Table_SummaryReports: {
        auto summaryReports = modelObject.cast<OutputTableSummaryReports>();
        retVal = translateOutputTableSummaryReports(summaryReports);
        break;
      }
      case openstudio::IddObjectType::OS_People: {
        auto people = modelObject.cast<People>();
        retVal = translatePeople(people);
        break;
      }
      case openstudio::IddObjectType::OS_People_Definition: {
        // no-op
        break;
      }
      case openstudio::IddObjectType::OS_PerformancePrecisionTradeoffs: {
        auto performancePrecisionTradeoffs = modelObject.cast<PerformancePrecisionTradeoffs>();
        retVal = translatePerformancePrecisionTradeoffs(performancePrecisionTradeoffs);
        break;
      }
      case openstudio::IddObjectType::OS_Pipe_Adiabatic: {
        auto pipe = modelObject.cast<PipeAdiabatic>();
        retVal = translatePipeAdiabatic(pipe);
        break;
      }
      case openstudio::IddObjectType::OS_Pipe_Indoor: {
        auto pipe = modelObject.cast<PipeIndoor>();
        retVal = translatePipeIndoor(pipe);
        break;
      }
      case openstudio::IddObjectType::OS_Pipe_Outdoor: {
        auto pipe = modelObject.cast<PipeOutdoor>();
        retVal = translatePipeOutdoor(pipe);
        break;
      }
      case openstudio::IddObjectType::OS_PortList: {
        auto portList = modelObject.cast<PortList>();
        retVal = translatePortList(portList);
        break;
      }
      case openstudio::IddObjectType::OS_ProgramControl: {
        LOG(Warn, "OS_ProgramControl is not currently translated");
        break;
      }
      case openstudio::IddObjectType::OS_PythonPlugin_Instance: {
        auto obj = modelObject.cast<PythonPluginInstance>();
        retVal = translatePythonPluginInstance(obj);
        break;
      }
      case openstudio::IddObjectType::OS_PythonPlugin_Variable: {
        auto obj = modelObject.cast<PythonPluginVariable>();
        retVal = translatePythonPluginVariable(obj);
        break;
      }
      case openstudio::IddObjectType::OS_PythonPlugin_TrendVariable: {
        auto obj = modelObject.cast<PythonPluginTrendVariable>();
        retVal = translatePythonPluginTrendVariable(obj);
        break;
      }
      case openstudio::IddObjectType::OS_PythonPlugin_OutputVariable: {
        auto obj = modelObject.cast<PythonPluginOutputVariable>();
        retVal = translatePythonPluginOutputVariable(obj);
        break;
      }
      case openstudio::IddObjectType::OS_PythonPlugin_SearchPaths: {
        auto obj = modelObject.cast<PythonPluginSearchPaths>();
        retVal = translatePythonPluginSearchPaths(obj);
        break;
      }
      case openstudio::IddObjectType::OS_RadianceParameters: {
        // no-op
        break;
      }
      case openstudio::IddObjectType::OS_Refrigeration_AirChiller: {
        auto refrigerationAirChiller = modelObject.cast<RefrigerationAirChiller>();
        retVal = translateRefrigerationAirChiller(refrigerationAirChiller);
        break;
      }
      case openstudio::IddObjectType::OS_Refrigeration_Case: {
        auto refrigerationCase = modelObject.cast<RefrigerationCase>();
        retVal = translateRefrigerationCase(refrigerationCase);
        break;
      }
      case openstudio::IddObjectType::OS_Refrigeration_Compressor: {
        auto refrigerationCompressor = modelObject.cast<RefrigerationCompressor>();
        retVal = translateRefrigerationCompressor(refrigerationCompressor);
        break;
      }
      case openstudio::IddObjectType::OS_Refrigeration_CompressorRack: {
        auto refrigerationCompressorRack = modelObject.cast<RefrigerationCompressorRack>();
        retVal = translateRefrigerationCompressorRack(refrigerationCompressorRack);
        break;
      }
      case openstudio::IddObjectType::OS_Refrigeration_Condenser_AirCooled: {
        auto refrigerationCondenserAirCooled = modelObject.cast<RefrigerationCondenserAirCooled>();
        retVal = translateRefrigerationCondenserAirCooled(refrigerationCondenserAirCooled);
        break;
      }
      case openstudio::IddObjectType::OS_Refrigeration_Condenser_Cascade: {
        auto refrigerationCondenserCascade = modelObject.cast<RefrigerationCondenserCascade>();
        retVal = translateRefrigerationCondenserCascade(refrigerationCondenserCascade);
        break;
      }
      case openstudio::IddObjectType::OS_Refrigeration_Condenser_EvaporativeCooled: {
        auto refrigerationCondenserEvaporativeCooled = modelObject.cast<RefrigerationCondenserEvaporativeCooled>();
        retVal = translateRefrigerationCondenserEvaporativeCooled(refrigerationCondenserEvaporativeCooled);
        break;
      }
      case openstudio::IddObjectType::OS_Refrigeration_Condenser_WaterCooled: {
        auto refrigerationCondenserWaterCooled = modelObject.cast<RefrigerationCondenserWaterCooled>();
        retVal = translateRefrigerationCondenserWaterCooled(refrigerationCondenserWaterCooled);
        break;
      }
      case openstudio::IddObjectType::OS_Refrigeration_DefrostCycleParameters: {
        // DLM: is this a no-op?
        break;
      }
      case openstudio::IddObjectType::OS_Refrigeration_GasCooler_AirCooled: {
        auto refrigerationGasCoolerAirCooled = modelObject.cast<RefrigerationGasCoolerAirCooled>();
        retVal = translateRefrigerationGasCoolerAirCooled(refrigerationGasCoolerAirCooled);
        break;
      }
      case openstudio::IddObjectType::OS_Refrigeration_Subcooler_LiquidSuction: {
        auto refrigerationSubcoolerLiquidSuction = modelObject.cast<RefrigerationSubcoolerLiquidSuction>();
        retVal = translateRefrigerationSubcoolerLiquidSuction(refrigerationSubcoolerLiquidSuction);
        break;
      }
      case openstudio::IddObjectType::OS_Refrigeration_Subcooler_Mechanical: {
        auto refrigerationSubcoolerMechanical = modelObject.cast<RefrigerationSubcoolerMechanical>();
        retVal = translateRefrigerationSubcoolerMechanical(refrigerationSubcoolerMechanical);
        break;
      }
      case openstudio::IddObjectType::OS_Refrigeration_SecondarySystem: {
        auto refrigerationSecondarySystem = modelObject.cast<RefrigerationSecondarySystem>();
        retVal = translateRefrigerationSecondarySystem(refrigerationSecondarySystem);
        break;
      }
      case openstudio::IddObjectType::OS_Refrigeration_System: {
        auto refrigerationSystem = modelObject.cast<RefrigerationSystem>();
        retVal = translateRefrigerationSystem(refrigerationSystem);
        break;
      }
      case openstudio::IddObjectType::OS_Refrigeration_TranscriticalSystem: {
        auto refrigerationTranscriticalSystem = modelObject.cast<RefrigerationTranscriticalSystem>();
        retVal = translateRefrigerationTranscriticalSystem(refrigerationTranscriticalSystem);
        break;
      }
      case openstudio::IddObjectType::OS_Refrigeration_WalkIn: {
        auto refrigerationWalkIn = modelObject.cast<RefrigerationWalkIn>();
        retVal = translateRefrigerationWalkIn(refrigerationWalkIn);
        break;
      }
      case openstudio::IddObjectType::OS_Refrigeration_WalkIn_ZoneBoundary: {
        // DLM: is this a no-op
        break;
      }
      case openstudio::IddObjectType::OS_Rendering_Color: {
        // no-op
        return retVal;
      }
      case openstudio::IddObjectType::OS_RunPeriod: {
        auto runPeriod = modelObject.cast<RunPeriod>();
        retVal = translateRunPeriod(runPeriod);
        break;
      }
      case openstudio::IddObjectType::OS_RunPeriodControl_DaylightSavingTime: {
        auto mo = modelObject.cast<RunPeriodControlDaylightSavingTime>();
        retVal = translateRunPeriodControlDaylightSavingTime(mo);
        break;
      }
      case openstudio::IddObjectType::OS_RunPeriodControl_SpecialDays: {
        auto mo = modelObject.cast<RunPeriodControlSpecialDays>();
        retVal = translateRunPeriodControlSpecialDays(mo);
        break;
      }
      case openstudio::IddObjectType::OS_Schedule_Compact: {
        auto schedule = modelObject.cast<ScheduleCompact>();
        retVal = translateScheduleCompact(schedule);
        break;
      }
      case openstudio::IddObjectType::OS_Schedule_Constant: {
        auto schedule = modelObject.cast<ScheduleConstant>();
        retVal = translateScheduleConstant(schedule);
        break;
      }
      case openstudio::IddObjectType::OS_Schedule_Day: {
        auto schedule = modelObject.cast<ScheduleDay>();
        retVal = translateScheduleDay(schedule);
        break;
      }
      case openstudio::IddObjectType::OS_Schedule_FixedInterval: {
        auto schedule = modelObject.cast<ScheduleFixedInterval>();
        retVal = translateScheduleFixedInterval(schedule);
        break;
      }
      case openstudio::IddObjectType::OS_Schedule_File: {
        auto schedule = modelObject.cast<ScheduleFile>();
        retVal = translateScheduleFile(schedule);
        break;
      }
      case openstudio::IddObjectType::OS_Schedule_Rule: {
        // no-op
        break;
      }
      case openstudio::IddObjectType::OS_Schedule_Ruleset: {
        auto mo = modelObject.cast<ScheduleRuleset>();
        retVal = translateScheduleRuleset(mo);
        break;
      }
      case openstudio::IddObjectType::OS_ScheduleTypeLimits: {
        auto mo = modelObject.cast<ScheduleTypeLimits>();
        retVal = translateScheduleTypeLimits(mo);
        break;
      }
      case openstudio::IddObjectType::OS_Schedule_Week: {
        auto schedule = modelObject.cast<ScheduleWeek>();
        retVal = translateScheduleWeek(schedule);
        break;
      }
      case openstudio::IddObjectType::OS_Schedule_Year: {
        auto schedule = modelObject.cast<ScheduleYear>();
        retVal = translateScheduleYear(schedule);
        break;
      }
      case openstudio::IddObjectType::OS_Schedule_VariableInterval: {
        auto schedule = modelObject.cast<ScheduleVariableInterval>();
        retVal = translateScheduleVariableInterval(schedule);
        break;
      }
      case openstudio::IddObjectType::OS_SetpointManager_Coldest: {
        auto spm = modelObject.cast<SetpointManagerColdest>();
        retVal = translateSetpointManagerColdest(spm);
        break;
      }
      case openstudio::IddObjectType::OS_SetpointManager_FollowGroundTemperature: {
        auto spm = modelObject.cast<SetpointManagerFollowGroundTemperature>();
        retVal = translateSetpointManagerFollowGroundTemperature(spm);
        break;
      }
      case openstudio::IddObjectType::OS_SetpointManager_FollowOutdoorAirTemperature: {
        auto spm = modelObject.cast<SetpointManagerFollowOutdoorAirTemperature>();
        retVal = translateSetpointManagerFollowOutdoorAirTemperature(spm);
        break;
      }
      case openstudio::IddObjectType::OS_SetpointManager_FollowSystemNodeTemperature: {
        auto spm = modelObject.cast<SetpointManagerFollowSystemNodeTemperature>();
        retVal = translateSetpointManagerFollowSystemNodeTemperature(spm);
        break;
      }
      case openstudio::IddObjectType::OS_SetpointManager_MixedAir: {
        auto spm = modelObject.cast<SetpointManagerMixedAir>();
        retVal = translateSetpointManagerMixedAir(spm);
        break;
      }
      case openstudio::IddObjectType::OS_SetpointManager_MultiZone_Cooling_Average: {
        auto spm = modelObject.cast<SetpointManagerMultiZoneCoolingAverage>();
        retVal = translateSetpointManagerMultiZoneCoolingAverage(spm);
        break;
      }
      case openstudio::IddObjectType::OS_SetpointManager_MultiZone_Heating_Average: {
        auto spm = modelObject.cast<SetpointManagerMultiZoneHeatingAverage>();
        retVal = translateSetpointManagerMultiZoneHeatingAverage(spm);
        break;
      }
      case openstudio::IddObjectType::OS_SetpointManager_MultiZone_Humidity_Maximum: {
        auto spm = modelObject.cast<SetpointManagerMultiZoneHumidityMaximum>();
        retVal = translateSetpointManagerMultiZoneHumidityMaximum(spm);
        break;
      }
      case openstudio::IddObjectType::OS_SetpointManager_MultiZone_Humidity_Minimum: {
        auto spm = modelObject.cast<SetpointManagerMultiZoneHumidityMinimum>();
        retVal = translateSetpointManagerMultiZoneHumidityMinimum(spm);
        break;
      }
      case openstudio::IddObjectType::OS_SetpointManager_MultiZone_MaximumHumidity_Average: {
        auto spm = modelObject.cast<SetpointManagerMultiZoneMaximumHumidityAverage>();
        retVal = translateSetpointManagerMultiZoneMaximumHumidityAverage(spm);
        break;
      }
      case openstudio::IddObjectType::OS_SetpointManager_MultiZone_MinimumHumidity_Average: {
        auto spm = modelObject.cast<SetpointManagerMultiZoneMinimumHumidityAverage>();
        retVal = translateSetpointManagerMultiZoneMinimumHumidityAverage(spm);
        break;
      }
      case openstudio::IddObjectType::OS_SetpointManager_OutdoorAirPretreat: {
        auto spm = modelObject.cast<SetpointManagerOutdoorAirPretreat>();
        retVal = translateSetpointManagerOutdoorAirPretreat(spm);
        break;
      }
      case openstudio::IddObjectType::OS_SetpointManager_OutdoorAirReset: {
        auto spm = modelObject.cast<SetpointManagerOutdoorAirReset>();
        retVal = translateSetpointManagerOutdoorAirReset(spm);
        break;
      }
      case openstudio::IddObjectType::OS_SetpointManager_Scheduled: {
        auto spm = modelObject.cast<SetpointManagerScheduled>();
        retVal = translateSetpointManagerScheduled(spm);
        break;
      }
      case openstudio::IddObjectType::OS_SetpointManager_Scheduled_DualSetpoint: {
        auto spm = modelObject.cast<SetpointManagerScheduledDualSetpoint>();
        retVal = translateSetpointManagerScheduledDualSetpoint(spm);
        break;
      }
      case openstudio::IddObjectType::OS_SetpointManager_SingleZone_Cooling: {
        auto spm = modelObject.cast<SetpointManagerSingleZoneCooling>();
        retVal = translateSetpointManagerSingleZoneCooling(spm);
        break;
      }
      case openstudio::IddObjectType::OS_SetpointManager_SingleZone_Heating: {
        auto spm = modelObject.cast<SetpointManagerSingleZoneHeating>();
        retVal = translateSetpointManagerSingleZoneHeating(spm);
        break;
      }
      case openstudio::IddObjectType::OS_SetpointManager_SingleZone_Humidity_Maximum: {
        auto spm = modelObject.cast<SetpointManagerSingleZoneHumidityMaximum>();
        retVal = translateSetpointManagerSingleZoneHumidityMaximum(spm);
        break;
      }
      case openstudio::IddObjectType::OS_SetpointManager_SingleZone_Humidity_Minimum: {
        auto spm = modelObject.cast<SetpointManagerSingleZoneHumidityMinimum>();
        retVal = translateSetpointManagerSingleZoneHumidityMinimum(spm);
        break;
      }
      case openstudio::IddObjectType::OS_SetpointManager_SingleZone_OneStageCooling: {
        auto spm = modelObject.cast<SetpointManagerSingleZoneOneStageCooling>();
        retVal = translateSetpointManagerSingleZoneOneStageCooling(spm);
        break;
      }
      case openstudio::IddObjectType::OS_SetpointManager_SingleZone_OneStageHeating: {
        auto spm = modelObject.cast<SetpointManagerSingleZoneOneStageHeating>();
        retVal = translateSetpointManagerSingleZoneOneStageHeating(spm);
        break;
      }
      case openstudio::IddObjectType::OS_SetpointManager_SingleZone_Reheat: {
        auto spm = modelObject.cast<SetpointManagerSingleZoneReheat>();
        retVal = translateSetpointManagerSingleZoneReheat(spm);
        break;
      }
      case openstudio::IddObjectType::OS_SetpointManager_Warmest: {
        auto spm = modelObject.cast<SetpointManagerWarmest>();
        retVal = translateSetpointManagerWarmest(spm);
        break;
      }
      case openstudio::IddObjectType::OS_SetpointManager_WarmestTemperatureFlow: {
        auto spm = modelObject.cast<SetpointManagerWarmestTemperatureFlow>();
        retVal = translateSetpointManagerWarmestTemperatureFlow(spm);
        break;
      }
      case openstudio::IddObjectType::OS_SetpointManager_SystemNodeReset_Humidity: {
        auto spm = modelObject.cast<SetpointManagerSystemNodeResetHumidity>();
        retVal = translateSetpointManagerSystemNodeResetHumidity(spm);
        break;
      }
      case openstudio::IddObjectType::OS_SetpointManager_SystemNodeReset_Temperature: {
        auto spm = modelObject.cast<SetpointManagerSystemNodeResetTemperature>();
        retVal = translateSetpointManagerSystemNodeResetTemperature(spm);
        break;
      }
      case openstudio::IddObjectType::OS_ShadingControl: {
        auto shadingControl = modelObject.cast<ShadingControl>();
        retVal = translateShadingControl(shadingControl);
        break;
      }
      case openstudio::IddObjectType::OS_ShadingSurface: {
        auto shadingSurface = modelObject.cast<ShadingSurface>();
        retVal = translateShadingSurface(shadingSurface);
        break;
      }
      case openstudio::IddObjectType::OS_ShadingSurfaceGroup: {
        auto shadingSurfaceGroup = modelObject.cast<ShadingSurfaceGroup>();
        retVal = translateShadingSurfaceGroup(shadingSurfaceGroup);
        break;
      }
      case openstudio::IddObjectType::OS_ShadowCalculation: {
        auto mo = modelObject.cast<ShadowCalculation>();
        retVal = translateShadowCalculation(mo);
        break;
      }
      case openstudio::IddObjectType::OS_SimulationControl: {
        auto simulationControl = modelObject.cast<SimulationControl>();
        retVal = translateSimulationControl(simulationControl);
        break;
      }
      case openstudio::IddObjectType::OS_Space: {
        auto space = modelObject.cast<Space>();
        retVal = translateSpace(space);
        break;
      }
      case openstudio::IddObjectType::OS_SpaceInfiltration_DesignFlowRate: {
        auto spaceInfiltrationDesignFlowRate = modelObject.cast<SpaceInfiltrationDesignFlowRate>();
        retVal = translateSpaceInfiltrationDesignFlowRate(spaceInfiltrationDesignFlowRate);
        break;
      }
      case openstudio::IddObjectType::OS_SpaceInfiltration_EffectiveLeakageArea: {
        auto spaceInfiltrationEffectiveLeakageArea = modelObject.cast<SpaceInfiltrationEffectiveLeakageArea>();
        retVal = translateSpaceInfiltrationEffectiveLeakageArea(spaceInfiltrationEffectiveLeakageArea);
        break;
      }
      case openstudio::IddObjectType::OS_SpaceInfiltration_FlowCoefficient: {
        auto spaceInfiltrationFlowCoefficient = modelObject.cast<SpaceInfiltrationFlowCoefficient>();
        retVal = translateSpaceInfiltrationFlowCoefficient(spaceInfiltrationFlowCoefficient);
        break;
      }
      case openstudio::IddObjectType::OS_SpaceType: {
        auto spaceType = modelObject.cast<SpaceType>();
        retVal = translateSpaceType(spaceType);
        break;
      }
      case openstudio::IddObjectType::OS_Site: {
        auto site = modelObject.cast<Site>();
        retVal = translateSite(site);
        break;
      }
      case openstudio::IddObjectType::OS_Site_GroundReflectance: {
        auto mo = modelObject.cast<SiteGroundReflectance>();
        retVal = translateSiteGroundReflectance(mo);
        break;
      }
      case openstudio::IddObjectType::OS_Site_GroundTemperature_BuildingSurface: {
        auto mo = modelObject.cast<SiteGroundTemperatureBuildingSurface>();
        retVal = translateSiteGroundTemperatureBuildingSurface(mo);
        break;
      }
      case openstudio::IddObjectType::OS_Site_GroundTemperature_Deep: {
        auto mo = modelObject.cast<SiteGroundTemperatureDeep>();
        retVal = translateSiteGroundTemperatureDeep(mo);
        break;
      }
      case openstudio::IddObjectType::OS_Site_GroundTemperature_FCfactorMethod: {
        auto mo = modelObject.cast<SiteGroundTemperatureFCfactorMethod>();
        retVal = translateSiteGroundTemperatureFCfactorMethod(mo);
        break;
      }
      case openstudio::IddObjectType::OS_Site_GroundTemperature_Shallow: {
        auto mo = modelObject.cast<SiteGroundTemperatureShallow>();
        retVal = translateSiteGroundTemperatureShallow(mo);
        break;
      }
      case openstudio::IddObjectType::OS_Site_GroundTemperature_Undisturbed_KusudaAchenbach: {
        model::SiteGroundTemperatureUndisturbedKusudaAchenbach mo = modelObject.cast<SiteGroundTemperatureUndisturbedKusudaAchenbach>();
        retVal = translateSiteGroundTemperatureUndisturbedKusudaAchenbach(mo);
        break;
      }
      case openstudio::IddObjectType::OS_Site_GroundTemperature_Undisturbed_Xing: {
        model::SiteGroundTemperatureUndisturbedXing mo = modelObject.cast<SiteGroundTemperatureUndisturbedXing>();
        retVal = translateSiteGroundTemperatureUndisturbedXing(mo);
        break;
      }
      case openstudio::IddObjectType::OS_Site_WaterMainsTemperature: {
        auto mo = modelObject.cast<SiteWaterMainsTemperature>();
        retVal = translateSiteWaterMainsTemperature(mo);
        break;
      }
      case openstudio::IddObjectType::OS_Sizing_Parameters: {
        auto mo = modelObject.cast<SizingParameters>();
        retVal = translateSizingParameters(mo);
        break;
      }
      case openstudio::IddObjectType::OS_SolarCollector_FlatPlate_PhotovoltaicThermal: {
        auto mo = modelObject.cast<SolarCollectorFlatPlatePhotovoltaicThermal>();
        retVal = translateSolarCollectorFlatPlatePhotovoltaicThermal(mo);
        break;
      }
      case openstudio::IddObjectType::OS_SolarCollector_FlatPlate_Water: {
        auto mo = modelObject.cast<SolarCollectorFlatPlateWater>();
        retVal = translateSolarCollectorFlatPlateWater(mo);
        break;
      }

      case openstudio::IddObjectType::OS_SolarCollector_IntegralCollectorStorage: {
        auto mo = modelObject.cast<SolarCollectorIntegralCollectorStorage>();
        retVal = translateSolarCollectorIntegralCollectorStorage(mo);
        break;
      }
      case openstudio::IddObjectType::OS_SolarCollectorPerformance_FlatPlate: {
        auto mo = modelObject.cast<SolarCollectorPerformanceFlatPlate>();
        retVal = translateSolarCollectorPerformanceFlatPlate(mo);
        break;
      }
      case openstudio::IddObjectType::OS_SolarCollectorPerformance_IntegralCollectorStorage: {
        auto mo = modelObject.cast<SolarCollectorPerformanceIntegralCollectorStorage>();
        retVal = translateSolarCollectorPerformanceIntegralCollectorStorage(mo);
        break;
      }
      case openstudio::IddObjectType::OS_SolarCollectorPerformance_PhotovoltaicThermal_BIPVT: {
        auto mo = modelObject.cast<SolarCollectorPerformancePhotovoltaicThermalBIPVT>();
        retVal = translateSolarCollectorPerformancePhotovoltaicThermalBIPVT(mo);
        break;
      }
      case openstudio::IddObjectType::OS_SolarCollectorPerformance_PhotovoltaicThermal_Simple: {
        auto mo = modelObject.cast<SolarCollectorPerformancePhotovoltaicThermalSimple>();
        retVal = translateSolarCollectorPerformancePhotovoltaicThermalSimple(mo);
        break;
      }
      case openstudio::IddObjectType::OS_StandardsInformation_Construction: {
        // no-op
        return retVal;
      }
      case openstudio::IddObjectType::OS_StandardsInformation_Material: {
        // no-op
        return retVal;
      }
      case openstudio::IddObjectType::OS_SteamEquipment: {
        auto steamEquipment = modelObject.cast<SteamEquipment>();
        retVal = translateSteamEquipment(steamEquipment);
        break;
      }
      case openstudio::IddObjectType::OS_SteamEquipment_Definition: {
        // no-op
        break;
      }
      case openstudio::IddObjectType::OS_Surface: {
        auto surface = modelObject.cast<Surface>();
        retVal = translateSurface(surface);
        break;
      }
      case openstudio::IddObjectType::OS_SurfaceConvectionAlgorithm_Inside: {
        auto mo = modelObject.cast<InsideSurfaceConvectionAlgorithm>();
        retVal = translateInsideSurfaceConvectionAlgorithm(mo);
        break;
      }
      case openstudio::IddObjectType::OS_SurfaceConvectionAlgorithm_Outside: {
        auto mo = modelObject.cast<OutsideSurfaceConvectionAlgorithm>();
        retVal = translateOutsideSurfaceConvectionAlgorithm(mo);
        break;
      }
      case openstudio::IddObjectType::OS_SurfaceControl_MovableInsulation: {
        auto obj = modelObject.cast<SurfaceControlMovableInsulation>();
        retVal = translateSurfaceControlMovableInsulation(obj);
        break;
      }
      case openstudio::IddObjectType::OS_SurfaceProperty_ConvectionCoefficients: {
        auto obj = modelObject.cast<SurfacePropertyConvectionCoefficients>();
        retVal = translateSurfacePropertyConvectionCoefficients(obj);
        break;
      }
      case openstudio::IddObjectType::OS_SurfaceProperty_ConvectionCoefficients_MultipleSurface: {
        auto obj = modelObject.cast<SurfacePropertyConvectionCoefficientsMultipleSurface>();
        retVal = translateSurfacePropertyConvectionCoefficientsMultipleSurface(obj);
        break;
      }
      case openstudio::IddObjectType::OS_SurfaceProperty_LocalEnvironment: {
        auto obj = modelObject.cast<SurfacePropertyLocalEnvironment>();
        retVal = translateSurfacePropertyLocalEnvironment(obj);
        break;
      }
      case openstudio::IddObjectType::OS_SurfaceProperty_ExposedFoundationPerimeter: {
        auto obj = modelObject.cast<SurfacePropertyExposedFoundationPerimeter>();
        retVal = translateSurfacePropertyExposedFoundationPerimeter(obj);
        break;
      }
      case openstudio::IddObjectType::OS_SurfaceProperty_OtherSideCoefficients: {
        auto obj = modelObject.cast<SurfacePropertyOtherSideCoefficients>();
        retVal = translateSurfacePropertyOtherSideCoefficients(obj);
        break;
      }
      case openstudio::IddObjectType::OS_SurfaceProperty_OtherSideConditionsModel: {
        auto obj = modelObject.cast<SurfacePropertyOtherSideConditionsModel>();
        retVal = translateSurfacePropertyOtherSideConditionsModel(obj);
        break;
      }
      case openstudio::IddObjectType::OS_SurfaceProperty_SurroundingSurfaces: {
        auto obj = modelObject.cast<SurfacePropertySurroundingSurfaces>();
        retVal = translateSurfacePropertySurroundingSurfaces(obj);
        break;
      }
      case openstudio::IddObjectType::OS_SurfaceProperty_GroundSurfaces: {
        auto obj = modelObject.cast<SurfacePropertyGroundSurfaces>();
        retVal = translateSurfacePropertyGroundSurfaces(obj);
        break;
      }
      case openstudio::IddObjectType::OS_SurfaceProperty_IncidentSolarMultiplier: {
        auto obj = modelObject.cast<SurfacePropertyIncidentSolarMultiplier>();
        retVal = translateSurfacePropertyIncidentSolarMultiplier(obj);
        break;
      }
      case openstudio::IddObjectType::OS_SubSurface: {
        auto subSurface = modelObject.cast<SubSurface>();
        retVal = translateSubSurface(subSurface);
        break;
      }
      case openstudio::IddObjectType::OS_SwimmingPool_Indoor: {
        auto obj = modelObject.cast<SwimmingPoolIndoor>();
        retVal = translateSwimmingPoolIndoor(obj);
        break;
      }

#if defined(_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable : 4996)
#elif (defined(__GNUC__))
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
      case openstudio::IddObjectType::OS_Table_MultiVariableLookup: {
        auto table = modelObject.cast<TableMultiVariableLookup>();
        retVal = translateTableMultiVariableLookup(table);
        break;
      }
#if defined(_MSC_VER)
#  pragma warning(pop)
#elif (defined(__GNUC__))
#  pragma GCC diagnostic pop
#endif

      case openstudio::IddObjectType::OS_Table_Lookup: {
        auto table = modelObject.cast<TableLookup>();
        retVal = translateTableLookup(table);
        break;
      }
      case openstudio::IddObjectType::OS_Table_IndependentVariable: {
        auto tableIndependentVariable = modelObject.cast<TableIndependentVariable>();
        retVal = translateTableIndependentVariable(tableIndependentVariable);
        break;
      }
      case openstudio::IddObjectType::OS_TemperingValve: {
        auto mo = modelObject.cast<TemperingValve>();
        retVal = translateTemperingValve(mo);
        break;
      }
      case openstudio::IddObjectType::OS_ThermalZone: {
        auto zone = modelObject.cast<ThermalZone>();
        retVal = translateThermalZone(zone);
        break;
      }
      case openstudio::IddObjectType::OS_ThermalStorage_Ice_Detailed: {
        auto mo = modelObject.cast<ThermalStorageIceDetailed>();
        retVal = translateThermalStorageIceDetailed(mo);
        break;
      }
      case openstudio::IddObjectType::OS_ThermalStorage_ChilledWater_Stratified: {
        auto mo = modelObject.cast<ThermalStorageChilledWaterStratified>();
        retVal = translateThermalStorageChilledWaterStratified(mo);
        break;
      }
      case openstudio::IddObjectType::OS_ThermostatSetpoint_DualSetpoint: {
        auto thermostat = modelObject.cast<ThermostatSetpointDualSetpoint>();
        retVal = translateThermostatSetpointDualSetpoint(thermostat);
        break;
      }
      case openstudio::IddObjectType::OS_Timestep: {
        auto timestep = modelObject.cast<Timestep>();
        retVal = translateTimestep(timestep);
        break;
      }
      case openstudio::IddObjectType::OS_UnitarySystemPerformance_Multispeed: {
        auto sysPerfMultispeed = modelObject.cast<UnitarySystemPerformanceMultispeed>();
        retVal = translateUnitarySystemPerformanceMultispeed(sysPerfMultispeed);
        break;
      }
      case openstudio::IddObjectType::OS_UtilityBill: {
        // no-op
        break;
      }

        // TODO: once UtilityCost objects are wrapped
        //case openstudio::IddObjectType::OS_UtilityCost_Charge_Block:
        //{
        //LOG(Warn, "OS_UtilityCost_Charge_Block is not currently translated");
        //break;
        //}
        //case openstudio::IddObjectType::OS_UtilityCost_Charge_Simple:
        //{
        //LOG(Warn, "OS_UtilityCost_Charge_Simple is not currently translated");
        //break;
        //}
        //case openstudio::IddObjectType::OS_UtilityCost_Computation:
        //{
        //LOG(Warn, "OS_UtilityCost_Computation is not currently translated");
        //break;
        //}
        //case openstudio::IddObjectType::OS_UtilityCost_Qualify:
        //{
        //LOG(Warn, "OS_UtilityCost_Qualify is not currently translated");
        //break;
        //}
        //case openstudio::IddObjectType::OS_UtilityCost_Ratchet:
        //{
        //LOG(Warn, "OS_UtilityCost_Ratchet is not currently translated");
        //break;
        //}
        //case openstudio::IddObjectType::OS_UtilityCost_Tariff:
        //{
        //LOG(Warn, "OS_UtilityCost_Tariff is not currently translated");
        //break;
        //}
        //case openstudio::IddObjectType::OS_UtilityCost_Variable:
        //{
        //LOG(Warn, "OS_UtilityCost_Variable is not currently translated");
        //break;
        //}

      case openstudio::IddObjectType::OS_Version: {
        auto version = modelObject.cast<Version>();
        retVal = translateVersion(version);
        break;
      }
      case openstudio::IddObjectType::OS_WeatherFile: {
        // no-op
        return retVal;
      }
      case openstudio::IddObjectType::OS_WeatherProperty_SkyTemperature: {
        auto mo = modelObject.cast<SkyTemperature>();
        retVal = translateSkyTemperature(mo);
        break;
      }
      case openstudio::IddObjectType::OS_WindowMaterial_Blind: {
        auto blind = modelObject.cast<Blind>();
        retVal = translateBlind(blind);
        break;
      }
      case openstudio::IddObjectType::OS_WindowMaterial_DaylightRedirectionDevice: {
        auto drd = modelObject.cast<DaylightRedirectionDevice>();
        retVal = translateDaylightRedirectionDevice(drd);
        break;
      }
      case openstudio::IddObjectType::OS_WindowMaterial_Glazing_RefractionExtinctionMethod: {
        auto mo = modelObject.cast<RefractionExtinctionGlazing>();
        retVal = translateRefractionExtinctionGlazing(mo);
        break;
      }
      case openstudio::IddObjectType::OS_WindowMaterial_Screen: {
        auto mo = modelObject.cast<Screen>();
        retVal = translateScreen(mo);
        break;
      }
      case openstudio::IddObjectType::OS_WindowMaterial_Shade: {
        auto shade = modelObject.cast<Shade>();
        retVal = translateShade(shade);
        break;
      }
      case openstudio::IddObjectType::OS_WindowMaterial_SimpleGlazingSystem: {
        auto glazing = modelObject.cast<SimpleGlazing>();
        retVal = translateSimpleGlazing(glazing);
        break;
      }
      case openstudio::IddObjectType::OS_WindowMaterial_Glazing: {
        auto glazing = modelObject.cast<StandardGlazing>();
        retVal = translateStandardGlazing(glazing);
        break;
      }
      case openstudio::IddObjectType::OS_WindowMaterial_GlazingGroup_Thermochromic: {
        LOG(Warn, "OS_WindowMaterial_GlazingGroup_Thermochromic is not currently translated");
        break;
      }
      case openstudio::IddObjectType::OS_WindowProperty_FrameAndDivider: {
        auto frameAndDivider = modelObject.cast<WindowPropertyFrameAndDivider>();
        retVal = translateWindowPropertyFrameAndDivider(frameAndDivider);
        break;
      }
      case openstudio::IddObjectType::OS_YearDescription: {
        // no-op
        return retVal;
      }
      case openstudio::IddObjectType::OS_ZoneAirContaminantBalance: {
        auto mo = modelObject.cast<ZoneAirContaminantBalance>();
        retVal = translateZoneAirContaminantBalance(mo);
        break;
      }
      case openstudio::IddObjectType::OS_ZoneAirHeatBalanceAlgorithm: {
        auto mo = modelObject.cast<ZoneAirHeatBalanceAlgorithm>();
        retVal = translateZoneAirHeatBalanceAlgorithm(mo);
        break;
      }
      case openstudio::IddObjectType::OS_ZoneAirMassFlowConservation: {
        auto mo = modelObject.cast<ZoneAirMassFlowConservation>();
        retVal = translateZoneAirMassFlowConservation(mo);
        break;
      }
      case openstudio::IddObjectType::OS_ZoneCapacitanceMultiplier_ResearchSpecial: {
        auto mo = modelObject.cast<ZoneCapacitanceMultiplierResearchSpecial>();
        retVal = translateZoneCapacitanceMultiplierResearchSpecial(mo);
        break;
      }
      case openstudio::IddObjectType::OS_ZoneControl_ContaminantController: {
        auto mo = modelObject.cast<ZoneControlContaminantController>();
        retVal = translateZoneControlContaminantController(mo);
        break;
      }
      case openstudio::IddObjectType::OS_ZoneControl_Humidistat: {
        auto mo = modelObject.cast<ZoneControlHumidistat>();
        retVal = translateZoneControlHumidistat(mo);
        break;
      }
      case openstudio::IddObjectType::OS_ZoneControl_Thermostat_StagedDualSetpoint: {
        auto mo = modelObject.cast<ZoneControlThermostatStagedDualSetpoint>();
        retVal = translateZoneControlThermostatStagedDualSetpoint(mo);
        break;
      }
      case openstudio::IddObjectType::OS_ZoneHVAC_Baseboard_Convective_Electric: {
        auto mo = modelObject.cast<ZoneHVACBaseboardConvectiveElectric>();
        retVal = translateZoneHVACBaseboardConvectiveElectric(mo);
        break;
      }
      case openstudio::IddObjectType::OS_ZoneHVAC_Baseboard_Convective_Water: {
        auto mo = modelObject.cast<ZoneHVACBaseboardConvectiveWater>();
        retVal = translateZoneHVACBaseboardConvectiveWater(mo);
        break;
      }
      case openstudio::IddObjectType::OS_ZoneHVAC_Baseboard_RadiantConvective_Electric: {
        auto mo = modelObject.cast<ZoneHVACBaseboardRadiantConvectiveElectric>();
        retVal = translateZoneHVACBaseboardRadiantConvectiveElectric(mo);
        break;
      }
      case openstudio::IddObjectType::OS_ZoneHVAC_Baseboard_RadiantConvective_Water: {
        auto mo = modelObject.cast<ZoneHVACBaseboardRadiantConvectiveWater>();
        retVal = translateZoneHVACBaseboardRadiantConvectiveWater(mo);
        break;
      }
      case openstudio::IddObjectType::OS_ZoneHVAC_CoolingPanel_RadiantConvective_Water: {
        auto mo = modelObject.cast<ZoneHVACCoolingPanelRadiantConvectiveWater>();
        retVal = translateZoneHVACCoolingPanelRadiantConvectiveWater(mo);
        break;
      }
      case openstudio::IddObjectType::OS_ZoneHVAC_Dehumidifier_DX: {
        auto mo = modelObject.cast<ZoneHVACDehumidifierDX>();
        retVal = translateZoneHVACDehumidifierDX(mo);
        break;
      }
      case openstudio::IddObjectType::OS_ZoneHVAC_EnergyRecoveryVentilator: {
        auto mo = modelObject.cast<ZoneHVACEnergyRecoveryVentilator>();
        retVal = translateZoneHVACEnergyRecoveryVentilator(mo);
        break;
      }
      case openstudio::IddObjectType::OS_ZoneHVAC_EnergyRecoveryVentilator_Controller: {
        auto mo = modelObject.cast<ZoneHVACEnergyRecoveryVentilatorController>();
        retVal = translateZoneHVACEnergyRecoveryVentilatorController(mo);
        break;
      }
      case openstudio::IddObjectType::OS_ZoneHVAC_EquipmentList: {
        auto mo = modelObject.cast<ZoneHVACEquipmentList>();
        retVal = translateZoneHVACEquipmentList(mo);
        break;
      }
      case openstudio::IddObjectType::OS_ZoneHVAC_FourPipeFanCoil: {
        auto mo = modelObject.cast<ZoneHVACFourPipeFanCoil>();
        retVal = translateZoneHVACFourPipeFanCoil(mo);
        break;
      }
      case openstudio::IddObjectType::OS_ZoneHVAC_HighTemperatureRadiant: {
        auto mo = modelObject.cast<ZoneHVACHighTemperatureRadiant>();
        retVal = translateZoneHVACHighTemperatureRadiant(mo);
        break;
      }
      case openstudio::IddObjectType::OS_ZoneHVAC_IdealLoadsAirSystem: {
        auto mo = modelObject.cast<ZoneHVACIdealLoadsAirSystem>();
        retVal = translateZoneHVACIdealLoadsAirSystem(mo);
        break;
      }
      case openstudio::IddObjectType::OS_ZoneHVAC_LowTemperatureRadiant_Electric: {
        auto mo = modelObject.cast<ZoneHVACLowTemperatureRadiantElectric>();
        retVal = translateZoneHVACLowTemperatureRadiantElectric(mo);
        break;
      }
      case openstudio::IddObjectType::OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlow: {
        auto mo = modelObject.cast<ZoneHVACLowTempRadiantConstFlow>();
        retVal = translateZoneHVACLowTempRadiantConstFlow(mo);
        break;
      }
      case openstudio::IddObjectType::OS_ZoneHVAC_LowTemperatureRadiant_VariableFlow: {
        auto mo = modelObject.cast<ZoneHVACLowTempRadiantVarFlow>();
        retVal = translateZoneHVACLowTempRadiantVarFlow(mo);
        break;
      }
      case openstudio::IddObjectType::OS_ZoneHVAC_PackagedTerminalHeatPump: {
        auto mo = modelObject.cast<ZoneHVACPackagedTerminalHeatPump>();
        retVal = translateZoneHVACPackagedTerminalHeatPump(mo);
        break;
      }
      case openstudio::IddObjectType::OS_ZoneHVAC_PackagedTerminalAirConditioner: {
        auto mo = modelObject.cast<ZoneHVACPackagedTerminalAirConditioner>();
        retVal = translateZoneHVACPackagedTerminalAirConditioner(mo);
        break;
      }
      case openstudio::IddObjectType::OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlow: {
        auto mo = modelObject.cast<ZoneHVACTerminalUnitVariableRefrigerantFlow>();
        retVal = translateZoneHVACTerminalUnitVariableRefrigerantFlow(mo);
        break;
      }
      case openstudio::IddObjectType::OS_ZoneHVAC_UnitHeater: {
        auto mo = modelObject.cast<ZoneHVACUnitHeater>();
        retVal = translateZoneHVACUnitHeater(mo);
        break;
      }
      case openstudio::IddObjectType::OS_ZoneHVAC_UnitVentilator: {
        auto mo = modelObject.cast<ZoneHVACUnitVentilator>();
        retVal = translateZoneHVACUnitVentilator(mo);
        break;
      }
      case openstudio::IddObjectType::OS_ZoneHVAC_WaterToAirHeatPump: {
        auto mo = modelObject.cast<ZoneHVACWaterToAirHeatPump>();
        retVal = translateZoneHVACWaterToAirHeatPump(mo);
        break;
      }
      case openstudio::IddObjectType::OS_ZoneMixing: {
        auto mo = modelObject.cast<ZoneMixing>();
        retVal = translateZoneMixing(mo);
        break;
      }
      case openstudio::IddObjectType::OS_ZoneProperty_UserViewFactors_BySurfaceName: {
        auto mo = modelObject.cast<ZonePropertyUserViewFactorsBySurfaceName>();
        retVal = translateZonePropertyUserViewFactorsBySurfaceName(mo);
        break;
      }
      case openstudio::IddObjectType::OS_ZoneVentilation_DesignFlowRate: {
        auto mo = modelObject.cast<ZoneVentilationDesignFlowRate>();
        retVal = translateZoneVentilationDesignFlowRate(mo);
        break;
      }
      case openstudio::IddObjectType::OS_ZoneVentilation_WindandStackOpenArea: {
        auto mo = modelObject.cast<ZoneVentilationWindandStackOpenArea>();
        retVal = translateZoneVentilationWindandStackOpenArea(mo);
        break;
      }
      //If no case statement log a warning
      default: {
        LOG(Warn, "Unknown IddObjectType: '" << modelObject.iddObject().name() << "'");
        return retVal;
      }
    }

    if (retVal) {
      m_map.insert(make_pair(modelObject.handle(), retVal.get()));

      if (m_progressBar) {
        m_progressBar->setValue((int)m_map.size());
      }
    }

    // is this redundant?
    // ETH@20120112 Yes.
    OptionalParentObject opo = modelObject.optionalCast<ParentObject>();
    if (opo) {
      ModelObjectVector children = opo->children();
      IddObjectTypeVector types = iddObjectsToTranslate();

      // sort these objects as well
      std::sort(children.begin(), children.end(), ChildSorter(types));

      for (auto& elem : children) {
        if (std::find(types.begin(), types.end(), elem.iddObject().type()) != types.end()) {
          translateAndMapModelObject(elem);
        }
      }
    }

    return retVal;
  }
  // NOLINTEND(readability-function-size, bugprone-branch-clone)

  std::string ForwardTranslator::stripOS2(const string& s) {
    std::string result;
    if (s.substr(0, 3) == "OS:") {
      result = s.substr(3);
    } else {
      result = s;
    }
    return result;
  }

  std::vector<IddObjectType> ForwardTranslator::iddObjectsToTranslate() {
    static const std::vector<IddObjectType> result{

      IddObjectType::OS_Version,

      IddObjectType::OS_ComponentData,

      IddObjectType::OS_LifeCycleCost_Parameters,
      IddObjectType::OS_CurrencyType,
      IddObjectType::OS_ComponentCost_Adjustments,
      IddObjectType::OS_LifeCycleCost_UsePriceEscalation,

      IddObjectType::OS_SimulationControl,
      IddObjectType::OS_ProgramControl,
      IddObjectType::OS_ConvergenceLimits,
      IddObjectType::OS_HeatBalanceAlgorithm,
      IddObjectType::OS_RunPeriod,
      IddObjectType::OS_RunPeriodControl_DaylightSavingTime,
      IddObjectType::OS_RunPeriodControl_SpecialDays,
      IddObjectType::OS_ShadowCalculation,
      IddObjectType::OS_SurfaceConvectionAlgorithm_Inside,
      IddObjectType::OS_SurfaceConvectionAlgorithm_Outside,
      IddObjectType::OS_SurfaceProperty_ConvectionCoefficients_MultipleSurface,
      IddObjectType::OS_Timestep,
      IddObjectType::OS_Sizing_Parameters,
      IddObjectType::OS_ZoneAirContaminantBalance,
      IddObjectType::OS_ZoneAirHeatBalanceAlgorithm,
      IddObjectType::OS_ZoneAirMassFlowConservation,
      IddObjectType::OS_ZoneCapacitanceMultiplier_ResearchSpecial,
      IddObjectType::OS_OutputControl_Files,
      IddObjectType::OS_OutputControl_ReportingTolerances,
      IddObjectType::OS_OutputControl_Table_Style,
      IddObjectType::OS_OutputControl_Timestamp,
      IddObjectType::OS_Output_Constructions,
      IddObjectType::OS_Output_DebuggingData,
      IddObjectType::OS_Output_Diagnostics,
      IddObjectType::OS_Output_JSON,
      IddObjectType::OS_Output_Schedules,
      IddObjectType::OS_Output_SQLite,

      // Note: we just always translate Output:EnvironmentalImpactFactors, and in there (it exists), then trigger translatation of the two others
      IddObjectType::OS_Output_EnvironmentalImpactFactors,
      // IddObjectType::OS_EnvironmentalImpactFactors,
      // IddObjectType::OS_FuelFactors,

      IddObjectType::OS_Output_Table_SummaryReports,
      IddObjectType::OS_PerformancePrecisionTradeoffs,

      IddObjectType::OS_Site,
      IddObjectType::OS_Site_GroundReflectance,
      IddObjectType::OS_Site_GroundTemperature_BuildingSurface,
      IddObjectType::OS_Site_GroundTemperature_Deep,
      IddObjectType::OS_Site_GroundTemperature_FCfactorMethod,
      IddObjectType::OS_Site_GroundTemperature_Shallow,
      IddObjectType::OS_Site_WaterMainsTemperature,
      IddObjectType::OS_ClimateZones,
      IddObjectType::OS_SizingPeriod_DesignDay,
      IddObjectType::OS_SizingPeriod_WeatherFileConditionType,
      IddObjectType::OS_SizingPeriod_WeatherFileDays,
      IddObjectType::OS_Foundation_Kiva,
      IddObjectType::OS_Foundation_Kiva_Settings,

      // TODO: once UtilityCost objects are wrapped
      // IddObjectType::OS_UtilityCost_Charge_Block,
      // IddObjectType::OS_UtilityCost_Charge_Simple,
      // IddObjectType::OS_UtilityCost_Computation,
      // IddObjectType::OS_UtilityCost_Qualify,
      // IddObjectType::OS_UtilityCost_Ratchet,
      // IddObjectType::OS_UtilityCost_Tariff,
      // IddObjectType::OS_UtilityCost_Variable,

      IddObjectType::OS_WeatherFile,
      IddObjectType::OS_WeatherProperty_SkyTemperature,

      IddObjectType::OS_Rendering_Color,
      IddObjectType::OS_SpaceType,

      IddObjectType::OS_Facility,
      IddObjectType::OS_Building,
      IddObjectType::OS_BuildingStory,
      IddObjectType::OS_LightingSimulationZone,

      IddObjectType::OS_ThermalZone,
      IddObjectType::OS_Space,
      IddObjectType::OS_Surface,
      IddObjectType::OS_SubSurface,
      IddObjectType::OS_InteriorPartitionSurfaceGroup,
      IddObjectType::OS_InteriorPartitionSurface,
      IddObjectType::OS_ShadingSurfaceGroup,
      IddObjectType::OS_ShadingSurface,

      IddObjectType::OS_ZoneProperty_UserViewFactors_BySurfaceName,

      IddObjectType::OS_Daylighting_Control,
      IddObjectType::OS_DaylightingDevice_Shelf,
      IddObjectType::OS_DaylightingDevice_Tubular,
      IddObjectType::OS_DaylightingDevice_LightWell,
      IddObjectType::OS_IlluminanceMap,

      // Definition objects will be translated as needed by instance objects.
      IddObjectType::OS_InternalMass,
      IddObjectType::OS_People,
      IddObjectType::OS_Lights,
      IddObjectType::OS_Luminaire,
      IddObjectType::OS_ElectricEquipment,
      IddObjectType::OS_ElectricEquipment_ITE_AirCooled,
      IddObjectType::OS_GasEquipment,
      IddObjectType::OS_HotWaterEquipment,
      IddObjectType::OS_SteamEquipment,
      IddObjectType::OS_OtherEquipment,
      IddObjectType::OS_SpaceInfiltration_DesignFlowRate,
      IddObjectType::OS_SpaceInfiltration_EffectiveLeakageArea,
      IddObjectType::OS_SpaceInfiltration_FlowCoefficient,
      IddObjectType::OS_Exterior_Lights,
      IddObjectType::OS_Exterior_FuelEquipment,
      IddObjectType::OS_Exterior_WaterEquipment,

      IddObjectType::OS_AirLoopHVAC,

      // Translated by AirLoopHVAC
      // IddObjectType::OS_AirLoopHVAC_OutdoorAirSystem,

      IddObjectType::OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypass,
      IddObjectType::OS_AirLoopHVAC_UnitaryCoolOnly,
      IddObjectType::OS_AirLoopHVAC_ZoneMixer,
      IddObjectType::OS_AirLoopHVAC_ZoneSplitter,
      IddObjectType::OS_AirTerminal_DualDuct_ConstantVolume,
      IddObjectType::OS_AirTerminal_DualDuct_VAV_OutdoorAir,
      IddObjectType::OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeam,
      IddObjectType::OS_AirTerminal_SingleDuct_ConstantVolume_NoReheat,
      // Unlike other AVMs, this one doesn't live on the AVM AssignmentList, so need to tell it to translate all the time
      IddObjectType::OS_AvailabilityManager_HybridVentilation,
      IddObjectType::OS_Chiller_Electric_EIR,
      IddObjectType::OS_Chiller_Electric_ReformulatedEIR,

      // Coil:Cooling:DX will be translated by the UnitarySystem it's in, and will in turn translate CurveFitPerformance, which will translate
      // OperatingMode, which will translate Speed
      // IddObjectType::OS_Coil_Cooling_DX,
      // IddObjectType::OS_Coil_Cooling_DX_CurveFit_Performance,
      // IddObjectType::OS_Coil_Cooling_DX_CurveFit_OperatingMode,
      // IddObjectType::OS_Coil_Cooling_DX_CurveFit_Speed,

      IddObjectType::OS_Coil_Cooling_DX_SingleSpeed,
      // IddObjectType::OS_Coil_Cooling_DX_SingleSpeed_ThermalStorage,
      IddObjectType::OS_Coil_Cooling_DX_TwoSpeed,
      IddObjectType::OS_Coil_Cooling_Water,
      IddObjectType::OS_Coil_Cooling_WaterToAirHeatPump_EquationFit,
      IddObjectType::OS_Coil_Heating_Desuperheater,
      IddObjectType::OS_Coil_Heating_Gas,
      IddObjectType::OS_Coil_Heating_Water,
      IddObjectType::OS_Coil_Heating_WaterToAirHeatPump_EquationFit,
      IddObjectType::OS_Coil_WaterHeating_Desuperheater,
      // IddObjectType::OS_Coil_UserDefined,

      // If using a plantLoop, this will be translated by the PlantLoop. But WaterHeaters can also be used stand-alone, so always translate them
      // We'll check in their FT if the "Peak Use Flow Rate" is actually initialized as it's an indication that the WH was
      IddObjectType::OS_WaterHeater_Mixed,
      IddObjectType::OS_WaterHeater_Stratified,

      IddObjectType::OS_Connection,
      IddObjectType::OS_Connector_Mixer,
      IddObjectType::OS_Connector_Splitter,
      // IddObjectType::OS_Controller_OutdoorAir, // Will be translated by the AirLoopHVACOutdoorAirSystem
      IddObjectType::OS_CoolingTower_SingleSpeed,
      IddObjectType::OS_Curve_Bicubic,
      IddObjectType::OS_Curve_Biquadratic,
      IddObjectType::OS_Curve_Cubic,
      IddObjectType::OS_Curve_DoubleExponentialDecay,
      IddObjectType::OS_Curve_Exponent,
      IddObjectType::OS_Curve_ExponentialDecay,
      IddObjectType::OS_Curve_ExponentialSkewNormal,
      IddObjectType::OS_Curve_FanPressureRise,
      IddObjectType::OS_Curve_Functional_PressureDrop,
      IddObjectType::OS_Curve_Linear,
      IddObjectType::OS_Curve_QuadLinear,
      IddObjectType::OS_Curve_QuintLinear,
      IddObjectType::OS_Curve_Quadratic,
      IddObjectType::OS_Curve_QuadraticLinear,
      IddObjectType::OS_Curve_Quartic,
      IddObjectType::OS_Curve_RectangularHyperbola1,
      IddObjectType::OS_Curve_RectangularHyperbola2,
      IddObjectType::OS_Curve_Sigmoid,
      IddObjectType::OS_Curve_Triquadratic,
      IddObjectType::OS_Table_MultiVariableLookup,
      IddObjectType::OS_Table_Lookup,

      // Should be on a PlantLoop to be translated
      // IddObjectType::OS_DistrictCooling,
      // IddObjectType::OS_DistrictHeating_Water,
      // IddObjectType::OS_DistrictHeating_Steam,

      IddObjectType::OS_EvaporativeCooler_Direct_ResearchSpecial,

      IddObjectType::OS_Node,
      IddObjectType::OS_PlantLoop,
      IddObjectType::OS_Splitter,
      IddObjectType::OS_ZoneHVAC_Baseboard_Convective_Water,
      IddObjectType::OS_ZoneHVAC_IdealLoadsAirSystem,
      IddObjectType::OS_ZoneHVAC_HighTemperatureRadiant,
      IddObjectType::OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlow,
      IddObjectType::OS_ZoneHVAC_LowTemperatureRadiant_VariableFlow,
      IddObjectType::OS_ZoneHVAC_LowTemperatureRadiant_Electric,
      IddObjectType::OS_ZoneMixing,

      IddObjectType::OS_Refrigeration_CompressorRack,
      IddObjectType::OS_Refrigeration_System,
      IddObjectType::OS_Refrigeration_TranscriticalSystem,

      IddObjectType::OS_ElectricLoadCenter_Distribution,
      // ElectricLoadCenterDistribution is responsible for translating its generators/inverters/storages
      // IddObjectType::OS_Generator_MicroTurbine,
      // IddObjectType::OS_Generator_FuelCell,
      // IddObjectType::OS_Generator_Photovoltaic,
      // IddObjectType::OS_Generator_PVWatts,
      // IddObjectType::OS_Generator_WindTurbine,
      // Fuel Cell is responsible for translating these
      // IddObjectType::OS_Generator_FuelCell_AirSupply,
      // IddObjectType::OS_Generator_FuelCell_AuxiliaryHeater,
      // IddObjectType::OS_Generator_FuelCell_ElectricalStorage,
      // IddObjectType::OS_Generator_FuelCell_ExhaustGasToWaterHeatExchanger,
      // IddObjectType::OS_Generator_FuelCell_Inverter,
      // IddObjectType::OS_Generator_FuelCell_PowerModule,
      // IddObjectType::OS_Generator_FuelCell_StackCooler,
      // IddObjectType::OS_Generator_FuelCell_WaterSupply,
      // Fuel Cell (and MicroCHP when implemented) are responsible for translating this one
      // IddObjectType::OS_Generator_FuelSupply,
      // IddObjectType::OS_ElectricLoadCenter_Inverter_LookUpTable,
      // IddObjectType::OS_ElectricLoadCenter_Inverter_Simple,
      // IddObjectType::OS_ElectricLoadCenter_Inverter_PVWatts,
      // IddObjectType::OS_ElectricLoadCenter_Storage_Simple,
      // IddObjectType::OS_ElectricLoadCenter_Storage_Converter,
      // IddObjectType::OS_ElectricLoadCenter_Storage_LiIonNMCBattery,

      // Generator_Photovoltaic is responsible for translating these three
      // IddObjectType::OS_PhotovoltaicPerformance_EquivalentOneDiode,
      // IddObjectType::OS_PhotovoltaicPerformance_Simple,
      // IddObjectType::OS_PhotovoltaicPerformance_Sandia,

      // Transformer can be standalone, see ASHRAE9012016_OfficeMedium_Denver.idf for example
      IddObjectType::OS_ElectricLoadCenter_Transformer,

      // put these down here so they have a chance to be translated with their "parent"
      IddObjectType::OS_LifeCycleCost,

      IddObjectType::OS_Output_Meter,
      IddObjectType::OS_Meter_Custom,
      IddObjectType::OS_Meter_CustomDecrement,
      IddObjectType::OS_Output_Variable,

      IddObjectType::OS_EnergyManagementSystem_GlobalVariable,
      IddObjectType::OS_EnergyManagementSystem_InternalVariable,
      IddObjectType::OS_EnergyManagementSystem_Sensor,
      IddObjectType::OS_EnergyManagementSystem_Actuator,
      IddObjectType::OS_EnergyManagementSystem_ConstructionIndexVariable,
      IddObjectType::OS_EnergyManagementSystem_CurveOrTableIndexVariable,
      IddObjectType::OS_EnergyManagementSystem_MeteredOutputVariable,
      IddObjectType::OS_EnergyManagementSystem_Program,
      IddObjectType::OS_EnergyManagementSystem_Subroutine,
      IddObjectType::OS_EnergyManagementSystem_ProgramCallingManager,
      IddObjectType::OS_EnergyManagementSystem_OutputVariable,
      IddObjectType::OS_EnergyManagementSystem_TrendVariable,
      IddObjectType::OS_Output_EnergyManagementSystem,

      IddObjectType::OS_ExternalInterface,
      IddObjectType::OS_ExternalInterface_Actuator,
      IddObjectType::OS_ExternalInterface_Schedule,
      IddObjectType::OS_ExternalInterface_Variable,
      IddObjectType::OS_ExternalInterface_FunctionalMockupUnitExport_From_Variable,
      IddObjectType::OS_ExternalInterface_FunctionalMockupUnitExport_To_Actuator,
      IddObjectType::OS_ExternalInterface_FunctionalMockupUnitExport_To_Schedule,
      IddObjectType::OS_ExternalInterface_FunctionalMockupUnitExport_To_Variable,
      IddObjectType::OS_ExternalInterface_FunctionalMockupUnitImport,
      IddObjectType::OS_ExternalInterface_FunctionalMockupUnitImport_From_Variable,
      IddObjectType::OS_ExternalInterface_FunctionalMockupUnitImport_To_Actuator,
      IddObjectType::OS_ExternalInterface_FunctionalMockupUnitImport_To_Schedule,
      IddObjectType::OS_ExternalInterface_FunctionalMockupUnitImport_To_Variable,

      IddObjectType::OS_PythonPlugin_Instance,
      IddObjectType::OS_PythonPlugin_Variable,
      IddObjectType::OS_PythonPlugin_TrendVariable,
      IddObjectType::OS_PythonPlugin_OutputVariable,
      IddObjectType::OS_PythonPlugin_SearchPaths,
    };

    return result;
  }

  void ForwardTranslator::translateConstructions(const model::Model& model) {
    std::vector<IddObjectType> iddObjectTypes{
      IddObjectType::OS_MaterialProperty_GlazingSpectralData,
      IddObjectType::OS_Material,
      IddObjectType::OS_Material_AirGap,
      IddObjectType::OS_Material_InfraredTransparent,
      IddObjectType::OS_Material_NoMass,
      IddObjectType::OS_Material_RoofVegetation,

      IddObjectType::OS_WindowMaterial_Blind,
      IddObjectType::OS_WindowMaterial_DaylightRedirectionDevice,
      IddObjectType::OS_WindowMaterial_Gas,
      IddObjectType::OS_WindowMaterial_GasMixture,
      IddObjectType::OS_WindowMaterial_Glazing,
      IddObjectType::OS_WindowMaterial_GlazingGroup_Thermochromic,
      IddObjectType::OS_WindowMaterial_Glazing_RefractionExtinctionMethod,
      IddObjectType::OS_WindowMaterial_Screen,
      IddObjectType::OS_WindowMaterial_Shade,
      IddObjectType::OS_WindowMaterial_SimpleGlazingSystem,
      IddObjectType::OS_WindowProperty_FrameAndDivider,
      IddObjectType::OS_ShadingControl,

      IddObjectType::OS_Construction,
      IddObjectType::OS_Construction_AirBoundary,
      IddObjectType::OS_Construction_CfactorUndergroundWall,
      IddObjectType::OS_Construction_FfactorGroundFloor,
      IddObjectType::OS_Construction_InternalSource,
      IddObjectType::OS_Construction_WindowDataFile,
      IddObjectType::OS_StandardsInformation_Construction,

      IddObjectType::OS_DefaultSubSurfaceConstructions,
      IddObjectType::OS_DefaultSurfaceConstructions,
      IddObjectType::OS_DefaultConstructionSet,
      IddObjectType::OS_DefaultScheduleSet,

      // Translated by the object it references directly
      // IddObjectType::OS_MaterialProperty_MoisturePenetrationDepth_Settings,  // Material Only
      // IdObjectType::OS_MaterialProperty_PhaseChange,                         // Material Only
      // IddObjectType::OS_MaterialProperty_PhaseChangeHysteresis,              // Material Only
      // IddObjectType::OS_SurfaceControl_MovableInsulation,                    // Surface Only
      // IddObjectType::OS_SurfaceProperty_OtherSideCoefficients,               // Surface, SubSurface,
      // IddObjectType::OS_SurfaceProperty_OtherSideConditionsModel,            // Surface, SubSurface,
      // IddObjectType::OS_SurfaceProperty_ExposedFoundationPerimeter,          // Surface Only
      // IddObjectType::OS_SurfaceProperty_ConvectionCoefficients,              // Surface, SubSurface, or InternalMass
    };

    for (const IddObjectType& iddObjectType : iddObjectTypes) {

      // get objects by type in sorted order
      std::vector<WorkspaceObject> objects = model.getObjectsByType(iddObjectType);
      std::sort(objects.begin(), objects.end(), WorkspaceObjectNameLess());

      for (const WorkspaceObject& workspaceObject : objects) {
        auto modelObject = workspaceObject.cast<ModelObject>();
        boost::optional<IdfObject> result = translateAndMapModelObject(modelObject);

        if (auto constructionBase_ = modelObject.optionalCast<ConstructionBase>()) {
          if (istringEqual("Interior Partition Surface Construction", workspaceObject.name().get())) {
            m_interiorPartitionSurfaceConstruction = constructionBase_.get();
          }

          if (istringEqual("Shading Surface Construction", workspaceObject.name().get())) {
            m_exteriorSurfaceConstruction = constructionBase_.get();
          }
        }
      }
    }
  }

  void ForwardTranslator::translateSchedules(const model::Model& model) {

    // Make sure these get in the idf file
    {
      auto schedule = model.alwaysOnDiscreteSchedule();
      translateAndMapModelObject(schedule);
      schedule = model.alwaysOffDiscreteSchedule();
      translateAndMapModelObject(schedule);
      schedule = model.alwaysOnContinuousSchedule();
      translateAndMapModelObject(schedule);
    }

    // loop over schedule type limits
    std::vector<WorkspaceObject> objects = model.getObjectsByType(IddObjectType::OS_ScheduleTypeLimits);
    std::sort(objects.begin(), objects.end(), WorkspaceObjectNameLess());
    for (const WorkspaceObject& workspaceObject : objects) {
      auto modelObject = workspaceObject.cast<ModelObject>();
      translateAndMapModelObject(modelObject);
    }

    // now loop over all schedule types
    std::vector<IddObjectType> iddObjectTypes{IddObjectType::OS_Schedule_Compact,       IddObjectType::OS_Schedule_Constant,
                                              IddObjectType::OS_Schedule_Day,           IddObjectType::OS_Schedule_Week,
                                              IddObjectType::OS_Schedule_Year,          IddObjectType::OS_Schedule_Ruleset,
                                              IddObjectType::OS_Schedule_FixedInterval, IddObjectType::OS_Schedule_VariableInterval,
                                              IddObjectType::OS_Schedule_File};

    for (const IddObjectType& iddObjectType : iddObjectTypes) {

      // get objects by type in sorted order
      objects = model.getObjectsByType(iddObjectType);
      std::sort(objects.begin(), objects.end(), WorkspaceObjectNameLess());

      for (const WorkspaceObject& workspaceObject : objects) {
        auto modelObject = workspaceObject.cast<ModelObject>();
        translateAndMapModelObject(modelObject);
      }
    }
  }

  void ForwardTranslator::translateAirflowNetwork(const model::Model& model) {
    // translate AFN if there is a simulation control object
    boost::optional<model::AirflowNetworkSimulationControl> afnSimulationControl =
      model.getOptionalUniqueModelObject<model::AirflowNetworkSimulationControl>();
    if (afnSimulationControl) {
      translateAirflowNetworkSimulationControl(afnSimulationControl.get());

      // Zones
      std::vector<model::AirflowNetworkZone> zones = model.getConcreteModelObjects<model::AirflowNetworkZone>();
      std::sort(zones.begin(), zones.end(), WorkspaceObjectNameLess());
      for (auto& modelObject : zones) {
        LOG(Trace, "Translating " << modelObject.briefDescription() << ".");
        translateAirflowNetworkZone(modelObject);
      }

      // Reference Crack Conditions
      std::vector<model::AirflowNetworkReferenceCrackConditions> refcracks =
        model.getConcreteModelObjects<model::AirflowNetworkReferenceCrackConditions>();
      std::sort(refcracks.begin(), refcracks.end(), WorkspaceObjectNameLess());
      for (auto& modelObject : refcracks) {
        LOG(Trace, "Translating " << modelObject.briefDescription() << ".");
        translateAirflowNetworkReferenceCrackConditions(modelObject);
      }

      // Cracks
      std::vector<model::AirflowNetworkCrack> cracks = model.getConcreteModelObjects<model::AirflowNetworkCrack>();
      std::sort(cracks.begin(), cracks.end(), WorkspaceObjectNameLess());
      for (auto& modelObject : cracks) {
        LOG(Trace, "Translating " << modelObject.briefDescription() << ".");
        translateAirflowNetworkCrack(modelObject);
      }

      // Effective Leakage Area
      std::vector<model::AirflowNetworkEffectiveLeakageArea> elas = model.getConcreteModelObjects<model::AirflowNetworkEffectiveLeakageArea>();
      std::sort(elas.begin(), elas.end(), WorkspaceObjectNameLess());
      for (auto& modelObject : elas) {
        LOG(Trace, "Translating " << modelObject.briefDescription() << ".");
        translateAirflowNetworkEffectiveLeakageArea(modelObject);
      }

      // Simple Openings
      std::vector<model::AirflowNetworkSimpleOpening> simples = model.getConcreteModelObjects<model::AirflowNetworkSimpleOpening>();
      std::sort(simples.begin(), simples.end(), WorkspaceObjectNameLess());
      for (auto& modelObject : simples) {
        LOG(Trace, "Translating " << modelObject.briefDescription() << ".");
        translateAirflowNetworkSimpleOpening(modelObject);
      }

      // Detailed Openings
      std::vector<model::AirflowNetworkDetailedOpening> detaileds = model.getConcreteModelObjects<model::AirflowNetworkDetailedOpening>();
      std::sort(detaileds.begin(), detaileds.end(), WorkspaceObjectNameLess());
      for (auto& modelObject : detaileds) {
        LOG(Trace, "Translating " << modelObject.briefDescription() << ".");
        translateAirflowNetworkDetailedOpening(modelObject);
      }

      // Horizontal Openings
      std::vector<model::AirflowNetworkHorizontalOpening> horzs = model.getConcreteModelObjects<model::AirflowNetworkHorizontalOpening>();
      std::sort(horzs.begin(), horzs.end(), WorkspaceObjectNameLess());
      for (auto& modelObject : horzs) {
        LOG(Trace, "Translating " << modelObject.briefDescription() << ".");
        translateAirflowNetworkHorizontalOpening(modelObject);
      }

      // Specified Flow Rate
      std::vector<model::AirflowNetworkSpecifiedFlowRate> sfrs = model.getConcreteModelObjects<model::AirflowNetworkSpecifiedFlowRate>();
      std::sort(sfrs.begin(), sfrs.end(), WorkspaceObjectNameLess());
      for (auto& modelObject : sfrs) {
        LOG(Trace, "Translating " << modelObject.briefDescription() << ".");
        translateAirflowNetworkSpecifiedFlowRate(modelObject);
      }

      // Surfaces
      std::vector<model::AirflowNetworkSurface> surfs = model.getConcreteModelObjects<model::AirflowNetworkSurface>();
      std::sort(surfs.begin(), surfs.end(), WorkspaceObjectNameLess());
      for (auto& modelObject : surfs) {
        LOG(Trace, "Translating " << modelObject.briefDescription() << ".");
        translateAirflowNetworkSurface(modelObject);
      }

      // Nodes
      std::vector<model::AirflowNetworkDistributionNode> nodes = model.getConcreteModelObjects<model::AirflowNetworkDistributionNode>();
      std::sort(nodes.begin(), nodes.end(), WorkspaceObjectNameLess());
      for (auto& modelObject : nodes) {
        LOG(Trace, "Translating " << modelObject.briefDescription() << ".");
        translateAirflowNetworkDistributionNode(modelObject);
      }

      // Linkages
      std::vector<model::AirflowNetworkDistributionLinkage> links = model.getConcreteModelObjects<model::AirflowNetworkDistributionLinkage>();
      std::sort(links.begin(), links.end(), WorkspaceObjectNameLess());
      for (auto& modelObject : links) {
        LOG(Trace, "Translating " << modelObject.briefDescription() << ".");
        translateAirflowNetworkDistributionLinkage(modelObject);
      }

      // External Nodes
      std::vector<model::AirflowNetworkExternalNode> exts = model.getConcreteModelObjects<model::AirflowNetworkExternalNode>();
      std::sort(exts.begin(), exts.end(), WorkspaceObjectNameLess());
      for (auto& modelObject : exts) {
        LOG(Trace, "Translating " << modelObject.briefDescription() << ".");
        translateAirflowNetworkExternalNode(modelObject);
      }

      // Zone Exhaust Fan
      std::vector<model::AirflowNetworkZoneExhaustFan> zefs = model.getConcreteModelObjects<model::AirflowNetworkZoneExhaustFan>();
      std::sort(zefs.begin(), zefs.end(), WorkspaceObjectNameLess());
      for (auto& modelObject : zefs) {
        LOG(Trace, "Translating " << modelObject.briefDescription() << ".");
        translateAirflowNetworkZoneExhaustFan(modelObject);
      }

      // Fan
      std::vector<model::AirflowNetworkFan> fans = model.getConcreteModelObjects<model::AirflowNetworkFan>();
      std::sort(fans.begin(), fans.end(), WorkspaceObjectNameLess());
      for (auto& modelObject : fans) {
        LOG(Trace, "Translating " << modelObject.briefDescription() << ".");
        translateAirflowNetworkFan(modelObject);
      }

      // Duct
      std::vector<model::AirflowNetworkDuct> ducts = model.getConcreteModelObjects<model::AirflowNetworkDuct>();
      std::sort(ducts.begin(), ducts.end(), WorkspaceObjectNameLess());
      for (auto& modelObject : ducts) {
        LOG(Trace, "Translating " << modelObject.briefDescription() << ".");
        translateAirflowNetworkDuct(modelObject);
      }

      // Equivalent Duct
      std::vector<model::AirflowNetworkEquivalentDuct> equivds = model.getConcreteModelObjects<model::AirflowNetworkEquivalentDuct>();
      std::sort(equivds.begin(), equivds.end(), WorkspaceObjectNameLess());
      for (auto& modelObject : equivds) {
        LOG(Trace, "Translating " << modelObject.briefDescription() << ".");
        translateAirflowNetworkEquivalentDuct(modelObject);
      }

      // Leakage Ratio
      std::vector<model::AirflowNetworkLeakageRatio> lrs = model.getConcreteModelObjects<model::AirflowNetworkLeakageRatio>();
      std::sort(lrs.begin(), lrs.end(), WorkspaceObjectNameLess());
      for (auto& modelObject : lrs) {
        LOG(Trace, "Translating " << modelObject.briefDescription() << ".");
        translateAirflowNetworkLeakageRatio(modelObject);
      }

      // Constant Pressure Drops
      std::vector<model::AirflowNetworkConstantPressureDrop> constps = model.getConcreteModelObjects<model::AirflowNetworkConstantPressureDrop>();
      std::sort(constps.begin(), constps.end(), WorkspaceObjectNameLess());
      for (auto& modelObject : constps) {
        LOG(Trace, "Translating " << modelObject.briefDescription() << ".");
        translateAirflowNetworkConstantPressureDrop(modelObject);
      }

      // Outdoor Air Flow
      std::vector<model::AirflowNetworkOutdoorAirflow> oafs = model.getConcreteModelObjects<model::AirflowNetworkOutdoorAirflow>();
      std::sort(oafs.begin(), oafs.end(), WorkspaceObjectNameLess());
      for (auto& modelObject : oafs) {
        LOG(Trace, "Translating " << modelObject.briefDescription() << ".");
        translateAirflowNetworkOutdoorAirflow(modelObject);
      }

      // Duct VFs
      std::vector<model::AirflowNetworkDuctViewFactors> ductvfs = model.getConcreteModelObjects<model::AirflowNetworkDuctViewFactors>();
      std::sort(ductvfs.begin(), ductvfs.end(), WorkspaceObjectNameLess());
      for (auto& modelObject : ductvfs) {
        LOG(Trace, "Translating " << modelObject.briefDescription() << ".");
        translateAirflowNetworkDuctViewFactors(modelObject);
      }

      // Occupant Ventilation Control
      std::vector<model::AirflowNetworkOccupantVentilationControl> occvcs =
        model.getConcreteModelObjects<model::AirflowNetworkOccupantVentilationControl>();
      std::sort(occvcs.begin(), occvcs.end(), WorkspaceObjectNameLess());
      for (auto& modelObject : occvcs) {
        LOG(Trace, "Translating " << modelObject.briefDescription() << ".");
        translateAirflowNetworkOccupantVentilationControl(modelObject);
      }

    } else {

      unsigned int count = 0;

      // Zones
      std::vector<model::AirflowNetworkZone> zones = model.getConcreteModelObjects<model::AirflowNetworkZone>();
      count += zones.size();

      // Reference Crack Conditions
      std::vector<model::AirflowNetworkReferenceCrackConditions> refcracks =
        model.getConcreteModelObjects<model::AirflowNetworkReferenceCrackConditions>();
      count += refcracks.size();

      // Cracks
      std::vector<model::AirflowNetworkCrack> cracks = model.getConcreteModelObjects<model::AirflowNetworkCrack>();
      count += cracks.size();

      // Effective Leakage Area
      std::vector<model::AirflowNetworkEffectiveLeakageArea> elas = model.getConcreteModelObjects<model::AirflowNetworkEffectiveLeakageArea>();
      count += elas.size();

      // Simple Openings
      std::vector<model::AirflowNetworkSimpleOpening> simples = model.getConcreteModelObjects<model::AirflowNetworkSimpleOpening>();
      count += simples.size();

      // Detailed Openings
      std::vector<model::AirflowNetworkDetailedOpening> detaileds = model.getConcreteModelObjects<model::AirflowNetworkDetailedOpening>();
      count += detaileds.size();

      // Horizontal Openings
      std::vector<model::AirflowNetworkHorizontalOpening> horzs = model.getConcreteModelObjects<model::AirflowNetworkHorizontalOpening>();
      count += horzs.size();

      // Specified Flow Rate
      std::vector<model::AirflowNetworkSpecifiedFlowRate> sfrs = model.getConcreteModelObjects<model::AirflowNetworkSpecifiedFlowRate>();
      count += sfrs.size();

      // Surfaces
      std::vector<model::AirflowNetworkSurface> surfs = model.getConcreteModelObjects<model::AirflowNetworkSurface>();
      count += surfs.size();

      // Nodes
      std::vector<model::AirflowNetworkDistributionNode> nodes = model.getConcreteModelObjects<model::AirflowNetworkDistributionNode>();
      count += nodes.size();

      // Linkages
      std::vector<model::AirflowNetworkDistributionLinkage> links = model.getConcreteModelObjects<model::AirflowNetworkDistributionLinkage>();
      count += links.size();

      // External Nodes
      std::vector<model::AirflowNetworkExternalNode> exts = model.getConcreteModelObjects<model::AirflowNetworkExternalNode>();
      count += exts.size();

      // Zone Exhaust Fan
      std::vector<model::AirflowNetworkZoneExhaustFan> zefs = model.getConcreteModelObjects<model::AirflowNetworkZoneExhaustFan>();
      count += zefs.size();

      // Fan
      std::vector<model::AirflowNetworkFan> fans = model.getConcreteModelObjects<model::AirflowNetworkFan>();
      count += fans.size();

      // Duct
      std::vector<model::AirflowNetworkDuct> ducts = model.getConcreteModelObjects<model::AirflowNetworkDuct>();
      count += ducts.size();

      // Equivalent Duct
      std::vector<model::AirflowNetworkEquivalentDuct> equivds = model.getConcreteModelObjects<model::AirflowNetworkEquivalentDuct>();
      count += equivds.size();

      // Leakage Ratio
      std::vector<model::AirflowNetworkLeakageRatio> lrs = model.getConcreteModelObjects<model::AirflowNetworkLeakageRatio>();
      count += lrs.size();

      // Constant Pressure Drops
      std::vector<model::AirflowNetworkConstantPressureDrop> constps = model.getConcreteModelObjects<model::AirflowNetworkConstantPressureDrop>();
      count += constps.size();

      // Outdoor Air Flow
      std::vector<model::AirflowNetworkOutdoorAirflow> oafs = model.getConcreteModelObjects<model::AirflowNetworkOutdoorAirflow>();
      count += oafs.size();

      // Duct VFs
      std::vector<model::AirflowNetworkDuctViewFactors> ductvfs = model.getConcreteModelObjects<model::AirflowNetworkDuctViewFactors>();
      count += ductvfs.size();

      // Occupant Ventilation Control
      std::vector<model::AirflowNetworkOccupantVentilationControl> occvcs =
        model.getConcreteModelObjects<model::AirflowNetworkOccupantVentilationControl>();
      count += occvcs.size();

      if (count > 0) {
        LOG(Warn, "No AirflowNetworkSimulationControl found in model, skipping forward translation of " << count << " AirflowNetwork objects.");
      }
    }
  }

  void ForwardTranslator::reset() {
    m_idfObjects.clear();

    m_map.clear();

    m_anyNumberScheduleTypeLimits.reset();

    m_interiorPartitionSurfaceConstruction.reset();

    m_exteriorSurfaceConstruction.reset();

    m_constructionHandleToReversedConstructions.clear();

    m_logSink.setThreadId(std::this_thread::get_id());

    m_logSink.resetStringStream();
  }

  model::ConstructionBase ForwardTranslator::interiorPartitionSurfaceConstruction(model::Model& model) {
    if (m_interiorPartitionSurfaceConstruction) {
      return *m_interiorPartitionSurfaceConstruction;
    }

    StandardOpaqueMaterial material(model, "MediumSmooth", 0.0254, 0.16, 800, 1090);
    material.setThermalAbsorptance(0.4);
    material.setSolarAbsorptance(0.4);
    material.setVisibleAbsorptance(0.3);

    model::Construction construction(model);
    construction.setName("Interior Partition Surface Construction");
    construction.insertLayer(0, material);
    m_interiorPartitionSurfaceConstruction = construction;

    translateAndMapModelObject(material);
    translateAndMapModelObject(construction);

    return *m_interiorPartitionSurfaceConstruction;
  }

  model::ConstructionBase ForwardTranslator::exteriorSurfaceConstruction(model::Model& model) {
    if (m_exteriorSurfaceConstruction) {
      return *m_exteriorSurfaceConstruction;
    }

    StandardOpaqueMaterial material(model, "MediumSmooth", 0.1524, 0.49, 512, 880);
    material.setThermalAbsorptance(0.6);
    material.setSolarAbsorptance(0.6);
    material.setVisibleAbsorptance(0.5);

    model::Construction construction(model);
    construction.setName("Interior Partition Surface Construction");
    construction.insertLayer(0, material);
    m_exteriorSurfaceConstruction = construction;

    translateAndMapModelObject(material);
    translateAndMapModelObject(construction);

    return *m_exteriorSurfaceConstruction;
  }

  model::ConstructionBase ForwardTranslator::reverseConstruction(const model::ConstructionBase& construction) {
    auto it = m_constructionHandleToReversedConstructions.find(construction.handle());
    if (it != m_constructionHandleToReversedConstructions.end()) {
      return it->second;
    }

    if (!construction.optionalCast<model::LayeredConstruction>()) {
      m_constructionHandleToReversedConstructions.insert(std::make_pair(construction.handle(), construction));
      return construction;
    }

    auto layeredConstruction = construction.cast<model::LayeredConstruction>();

    if (layeredConstruction.isSymmetric()) {
      m_constructionHandleToReversedConstructions.insert(std::make_pair(construction.handle(), construction));
      return construction;
    }

    if (construction.optionalCast<model::Construction>()) {
      model::Construction reversed = construction.cast<model::Construction>().reverseConstruction();
      m_constructionHandleToReversedConstructions.insert(std::make_pair(construction.handle(), reversed));
      return std::move(reversed);
    }

    if (construction.optionalCast<model::ConstructionWithInternalSource>()) {
      model::ConstructionWithInternalSource reversed =
        construction.cast<model::ConstructionWithInternalSource>().reverseConstructionWithInternalSource();
      m_constructionHandleToReversedConstructions.insert(std::make_pair(construction.handle(), reversed));
      return std::move(reversed);
    }

    // DLM: this is not right, should make reverseConstruction a virtual method on LayeredConstruction so it will work for ConstructionWithInternalSource, etc
    LOG(Error, "Do not know how to reverse construction " << construction);

    model::MaterialVector layers = layeredConstruction.layers();
    std::reverse(layers.begin(), layers.end());

    model::Construction reversed = model::Construction(construction.model());
    reversed.setName(construction.name().get() + " Reversed");
    reversed.setLayers(layers);
    m_constructionHandleToReversedConstructions.insert(std::make_pair(construction.handle(), reversed));

    return std::move(reversed);
  }

  void ForwardTranslator::resolveMatchedSurfaceConstructionConflicts(model::Model& model) {
    // DLM: there is duplicate code in Surface_Impl::construction
    // if you update this code you must update that code

    std::set<Handle> processedSurfaces;

    model::SurfaceVector surfaces = model.getConcreteModelObjects<model::Surface>();
    for (auto& surface : surfaces) {

      if (processedSurfaces.find(surface.handle()) != processedSurfaces.end()) {
        continue;
      }

      model::OptionalSurface adjacentSurface = surface.adjacentSurface();
      if (!adjacentSurface) {
        processedSurfaces.insert(surface.handle());
        continue;
      }

      boost::optional<std::pair<model::ConstructionBase, int>> constructionWithSearchDistance = surface.constructionWithSearchDistance();
      boost::optional<std::pair<model::ConstructionBase, int>> adjacentConstructionWithSearchDistance =
        adjacentSurface->constructionWithSearchDistance();

      if (constructionWithSearchDistance && !adjacentConstructionWithSearchDistance) {

        model::ConstructionBase c1 = constructionWithSearchDistance->first;
        model::ConstructionBase c2 = reverseConstruction(c1);

        LOG(Info, "Surface '" << surface.nameString() << "' has a construction and '" << adjacentSurface->nameString() << "' does not, using '"
                              << surface.nameString() << "'\'s construction.");

        surface.setConstruction(c1);
        adjacentSurface->setConstruction(c2);
        processedSurfaces.insert(surface.handle());
        processedSurfaces.insert(adjacentSurface->handle());
        continue;
      }

      if (!constructionWithSearchDistance && adjacentConstructionWithSearchDistance) {

        model::ConstructionBase c2 = adjacentConstructionWithSearchDistance->first;
        model::ConstructionBase c1 = reverseConstruction(c2);

        LOG(Info, "Surface '" << adjacentSurface->nameString() << "' has a construction and '" << surface.nameString() << "' does not, using '"
                              << adjacentSurface->nameString() << "'\'s construction.");

        surface.setConstruction(c1);
        adjacentSurface->setConstruction(c2);
        processedSurfaces.insert(surface.handle());
        processedSurfaces.insert(adjacentSurface->handle());
        continue;
      }

      if (!constructionWithSearchDistance && !adjacentConstructionWithSearchDistance) {
        // no constructions, nothing to be done
        LOG(Error, "No construction for either surface '" << surface.nameString() << "', and '" << adjacentSurface->nameString() << "'");

        processedSurfaces.insert(surface.handle());
        processedSurfaces.insert(adjacentSurface->handle());
        continue;
      }

      // both surfaces return a construction

      if (constructionWithSearchDistance->first.handle() == adjacentConstructionWithSearchDistance->first.handle()) {

        // both surfaces have same construction

        model::ConstructionBase c1 = constructionWithSearchDistance->first;
        model::ConstructionBase c2 = reverseConstruction(c1);

        if (c1.handle() != c2.handle()) {
          LOG(Warn, "Both surfaces '" << surface.nameString() << "', and '" << adjacentSurface->nameString() << "' reference the same construction '"
                                      << c1.nameString() << "' but it is not symmetric, creating a reversed copy.");

          // DLM: use surface name to choose which surface gets the original copy, not a good way but at least repeatable
          if (surface.nameString() < adjacentSurface->nameString()) {
            surface.setConstruction(c1);
            adjacentSurface->setConstruction(c2);
          } else {
            surface.setConstruction(c2);
            adjacentSurface->setConstruction(c1);
          }
        }

        processedSurfaces.insert(surface.handle());
        processedSurfaces.insert(adjacentSurface->handle());
        continue;
      }

      // both surfaces return a construction and they are not the same

      if (constructionWithSearchDistance->second < adjacentConstructionWithSearchDistance->second) {
        // lower search distance to construction
        model::ConstructionBase c1 = constructionWithSearchDistance->first;
        model::ConstructionBase c2 = reverseConstruction(c1);

        LOG(Info, "Surfaces '" << surface.nameString() << "', and '" << adjacentSurface->nameString()
                               << "' reference different constructions, choosing '" << surface.nameString()
                               << "'\'s construction based on search distance.");

        surface.setConstruction(c1);
        adjacentSurface->setConstruction(c2);
        processedSurfaces.insert(surface.handle());
        processedSurfaces.insert(adjacentSurface->handle());
        continue;
      }

      if (constructionWithSearchDistance->second > adjacentConstructionWithSearchDistance->second) {
        // lower search distance to adjacent construction
        model::ConstructionBase c2 = adjacentConstructionWithSearchDistance->first;
        model::ConstructionBase c1 = reverseConstruction(c2);

        LOG(Info, "Surfaces '" << surface.nameString() << "', and '" << adjacentSurface->nameString()
                               << "' reference different constructions, choosing '" << adjacentSurface->nameString()
                               << "'\'s construction based on search distance.");

        surface.setConstruction(c1);
        adjacentSurface->setConstruction(c2);
        processedSurfaces.insert(surface.handle());
        processedSurfaces.insert(adjacentSurface->handle());
        continue;
      }

      // both surfaces return a construction, they are not the same, and both have same search distance

      if (constructionWithSearchDistance->first.optionalCast<model::LayeredConstruction>()
          && adjacentConstructionWithSearchDistance->first.optionalCast<model::LayeredConstruction>()) {
        if (constructionWithSearchDistance->first.cast<model::LayeredConstruction>().reverseEqualLayers(
              adjacentConstructionWithSearchDistance->first.cast<model::LayeredConstruction>())) {
          // these constructions are reverse equal
          surface.setConstruction(constructionWithSearchDistance->first);
          adjacentSurface->setConstruction(adjacentConstructionWithSearchDistance->first);
          processedSurfaces.insert(surface.handle());
          processedSurfaces.insert(adjacentSurface->handle());
          continue;
        }
      }

      // give up for now, we can add more later
      LOG(Error, "Could not resolve matched surface construction conflicts between surfaces '" << surface.nameString() << "', and '"
                                                                                               << adjacentSurface->nameString() << "'");
      surface.setConstruction(constructionWithSearchDistance->first);
      adjacentSurface->setConstruction(adjacentConstructionWithSearchDistance->first);
      processedSurfaces.insert(surface.handle());
      processedSurfaces.insert(adjacentSurface->handle());
    }
  }

  void ForwardTranslator::resolveMatchedSubSurfaceConstructionConflicts(model::Model& model) {
    // DLM: there is duplicate code in SubSurface_Impl::construction
    // if you update this code you must update that code

    std::set<Handle> processedSubSurfaces;

    model::SubSurfaceVector subSurfaces = model.getConcreteModelObjects<model::SubSurface>();
    for (auto& subSurface : subSurfaces) {

      if (processedSubSurfaces.find(subSurface.handle()) != processedSubSurfaces.end()) {
        continue;
      }

      model::OptionalSubSurface adjacentSubSurface = subSurface.adjacentSubSurface();
      if (!adjacentSubSurface) {
        processedSubSurfaces.insert(subSurface.handle());
        continue;
      }

      boost::optional<std::pair<model::ConstructionBase, int>> constructionWithSearchDistance = subSurface.constructionWithSearchDistance();
      boost::optional<std::pair<model::ConstructionBase, int>> adjacentConstructionWithSearchDistance =
        adjacentSubSurface->constructionWithSearchDistance();

      if (constructionWithSearchDistance && !adjacentConstructionWithSearchDistance) {

        model::ConstructionBase c1 = constructionWithSearchDistance->first;
        model::ConstructionBase c2 = reverseConstruction(c1);

        LOG(Info, "SubSurface '" << subSurface.nameString() << "' has a construction and '" << adjacentSubSurface->nameString()
                                 << "' does not, using '" << subSurface.nameString() << "'\'s construction.");

        subSurface.setConstruction(c1);
        adjacentSubSurface->setConstruction(c2);
        processedSubSurfaces.insert(subSurface.handle());
        processedSubSurfaces.insert(adjacentSubSurface->handle());
        continue;
      }

      if (!constructionWithSearchDistance && adjacentConstructionWithSearchDistance) {

        model::ConstructionBase c2 = adjacentConstructionWithSearchDistance->first;
        model::ConstructionBase c1 = reverseConstruction(c2);

        LOG(Info, "SubSurface '" << adjacentSubSurface->nameString() << "' has a construction and '" << subSurface.nameString()
                                 << "' does not, using '" << adjacentSubSurface->nameString() << "'\'s construction.");

        subSurface.setConstruction(c1);
        adjacentSubSurface->setConstruction(c2);
        processedSubSurfaces.insert(subSurface.handle());
        processedSubSurfaces.insert(adjacentSubSurface->handle());
        continue;
      }

      if (!constructionWithSearchDistance && !adjacentConstructionWithSearchDistance) {
        // no constructions, nothing to be done
        LOG(Error, "No construction for either sub surface '" << subSurface.nameString() << "', and '" << adjacentSubSurface->nameString() << "'");

        processedSubSurfaces.insert(subSurface.handle());
        processedSubSurfaces.insert(adjacentSubSurface->handle());
        continue;
      }

      // both surfaces return a construction

      if (constructionWithSearchDistance->first.handle() == adjacentConstructionWithSearchDistance->first.handle()) {

        // both surfaces have same construction

        model::ConstructionBase c1 = constructionWithSearchDistance->first;
        model::ConstructionBase c2 = reverseConstruction(c1);

        if (c1.handle() != c2.handle()) {
          LOG(Warn, "Both sub surfaces '" << subSurface.nameString() << "', and '" << adjacentSubSurface->nameString()
                                          << "' reference the same construction '" << c1.nameString()
                                          << "' but it is not symmetric, creating a reversed copy.");

          // DLM: use subSurface name to choose which surface gets the original copy, not a good way but at least repeatable
          if (subSurface.nameString() < adjacentSubSurface->nameString()) {
            subSurface.setConstruction(c1);
            adjacentSubSurface->setConstruction(c2);
          } else {
            subSurface.setConstruction(c2);
            adjacentSubSurface->setConstruction(c1);
          }
        }

        processedSubSurfaces.insert(subSurface.handle());
        processedSubSurfaces.insert(adjacentSubSurface->handle());
        continue;
      }

      // both surfaces return a construction and they are not the same

      if (constructionWithSearchDistance->second < adjacentConstructionWithSearchDistance->second) {
        // lower search distance to construction
        model::ConstructionBase c1 = constructionWithSearchDistance->first;
        model::ConstructionBase c2 = reverseConstruction(c1);

        LOG(Info, "SubSurfaces '" << subSurface.nameString() << "', and '" << adjacentSubSurface->nameString()
                                  << "' reference different constructions, choosing '" << subSurface.nameString()
                                  << "'\'s construction based on search distance.");

        subSurface.setConstruction(c1);
        adjacentSubSurface->setConstruction(c2);
        processedSubSurfaces.insert(subSurface.handle());
        processedSubSurfaces.insert(adjacentSubSurface->handle());
        continue;
      }

      if (constructionWithSearchDistance->second > adjacentConstructionWithSearchDistance->second) {
        // lower search distance to adjacent construction
        model::ConstructionBase c2 = adjacentConstructionWithSearchDistance->first;
        model::ConstructionBase c1 = reverseConstruction(c2);

        LOG(Info, "SubSurfaces '" << subSurface.nameString() << "', and '" << adjacentSubSurface->nameString()
                                  << "' reference different constructions, choosing '" << adjacentSubSurface->nameString()
                                  << "'\'s construction based on search distance.");

        subSurface.setConstruction(c1);
        adjacentSubSurface->setConstruction(c2);
        processedSubSurfaces.insert(subSurface.handle());
        processedSubSurfaces.insert(adjacentSubSurface->handle());
        continue;
      }

      // both surfaces return a construction, they are not the same, and both have same search distance

      if (constructionWithSearchDistance->first.optionalCast<model::LayeredConstruction>()
          && adjacentConstructionWithSearchDistance->first.optionalCast<model::LayeredConstruction>()) {
        if (constructionWithSearchDistance->first.cast<model::LayeredConstruction>().reverseEqualLayers(
              adjacentConstructionWithSearchDistance->first.cast<model::LayeredConstruction>())) {
          // these constructions are reverse equal
          subSurface.setConstruction(constructionWithSearchDistance->first);
          adjacentSubSurface->setConstruction(adjacentConstructionWithSearchDistance->first);
          processedSubSurfaces.insert(subSurface.handle());
          processedSubSurfaces.insert(adjacentSubSurface->handle());
          continue;
        }
      }

      // give up for now, we can add more later
      LOG(Error, "Could not resolve matched construction conflicts between sub surfaces '" << subSurface.nameString() << "', and '"
                                                                                           << adjacentSubSurface->nameString() << "'");
      subSurface.setConstruction(constructionWithSearchDistance->first);
      adjacentSubSurface->setConstruction(adjacentConstructionWithSearchDistance->first);
      processedSubSurfaces.insert(subSurface.handle());
      processedSubSurfaces.insert(adjacentSubSurface->handle());
    }
  }

  void ForwardTranslator::createStandardOutputRequests(const model::Model& model) {
    if (!m_forwardTranslatorOptions.excludeHTMLOutputReport()) {
      if (!model.getOptionalUniqueModelObject<model::OutputControlTableStyle>()) {
        IdfObject& tableStyle = m_idfObjects.emplace_back(IddObjectType::OutputControl_Table_Style);
        tableStyle.setString(OutputControl_Table_StyleFields::ColumnSeparator, "HTML");
        if (m_forwardTranslatorOptions.iPTabularOutput()) {
          tableStyle.setString(OutputControl_Table_StyleFields::UnitConversion, "InchPound");
        }
      }
    }

    if (!m_forwardTranslatorOptions.excludeVariableDictionary()) {
      IdfObject rddRequest(IddObjectType::Output_VariableDictionary);
      rddRequest.setString(Output_VariableDictionaryFields::KeyField, "IDF");
      rddRequest.setString(Output_VariableDictionaryFields::SortOption, "Unsorted");
      m_idfObjects.push_back(rddRequest);
    }

    if (!m_forwardTranslatorOptions.excludeSQliteOutputReport()) {
      if (!model.getOptionalUniqueModelObject<model::OutputSQLite>()) {
        IdfObject& sqliteOutput = m_idfObjects.emplace_back(IddObjectType::Output_SQLite);
        sqliteOutput.setString(Output_SQLiteFields::OptionType, "SimpleAndTabular");
      }
    }

    // ensure at least one life cycle cost exists to prevent crash in E+ 8
    if (!m_forwardTranslatorOptions.excludeLCCObjects()) {
      bool hasAtLeastOneCost = std::any_of(m_idfObjects.cbegin(), m_idfObjects.cend(), [](const auto& obj) {
        auto iddObjType = obj.iddObject().type();
        return (iddObjType == openstudio::IddObjectType::LifeCycleCost_NonrecurringCost)
               || (iddObjType == openstudio::IddObjectType::LifeCycleCost_RecurringCosts);
      });

      if (!hasAtLeastOneCost) {
        // add default cost
        auto& idfObject = m_idfObjects.emplace_back(openstudio::IddObjectType::LifeCycleCost_NonrecurringCost);
        idfObject.setString(LifeCycleCost_NonrecurringCostFields::Name, "Default Cost");
        idfObject.setString(LifeCycleCost_NonrecurringCostFields::Category, "Construction");
        idfObject.setDouble(LifeCycleCost_NonrecurringCostFields::Cost, 0.0);
        idfObject.setString(LifeCycleCost_NonrecurringCostFields::StartofCosts, "ServicePeriod");
      }
    }
  }

  IdfObject ForwardTranslator::createAndRegisterIdfObject(const IddObjectType& idfObjectType, const model::ModelObject& modelObject) {
    IdfObject idfObject(idfObjectType);
    m_idfObjects.push_back(idfObject);
    m_map.insert(std::make_pair(modelObject.handle(), idfObject));
    return idfObject;
  }

  IdfObject ForwardTranslator::createRegisterAndNameIdfObject(const IddObjectType& idfObjectType, const model::ModelObject& modelObject) {
    IdfObject idfObject = createAndRegisterIdfObject(idfObjectType, modelObject);
    if (OptionalString moName = modelObject.name()) {
      idfObject.setName(*moName);
    }
    return idfObject;
  }

  boost::optional<IdfFile> ForwardTranslator::findIdfFile(const std::string& path) {
    std::stringstream ss;
    ss << ::energyplus::embedded_files::getFileAsString(path);
    return IdfFile::load(ss, IddFileType::EnergyPlus);
  }

  bool ForwardTranslator::isHVACComponentWithinUnitary(const model::HVACComponent& hvacComponent) {
    return (hvacComponent.containingHVACComponent() || hvacComponent.containingZoneHVACComponent() || hvacComponent.containingStraightComponent());
  }

  void ForwardTranslator::createFluidPropertiesMap() {
    m_fluidPropertiesMap.insert(make_pair("R11", ":/Resources/R11_FluidPropertiesDataSet.idf"));
    m_fluidPropertiesMap.insert(make_pair("R12", ":/Resources/R12_FluidPropertiesDataSet.idf"));
    m_fluidPropertiesMap.insert(make_pair("R22", ":/Resources/R22_FluidPropertiesDataSet.idf"));
    m_fluidPropertiesMap.insert(make_pair("R123", ":/Resources/R123_FluidPropertiesDataSet.idf"));
    m_fluidPropertiesMap.insert(make_pair("R134a", ":/Resources/R134a_FluidPropertiesDataSet.idf"));
    m_fluidPropertiesMap.insert(make_pair("R404a", ":/Resources/R404a_FluidPropertiesDataSet.idf"));
    m_fluidPropertiesMap.insert(make_pair("R407a", ":/Resources/R407a_FluidPropertiesDataSet.idf"));
    m_fluidPropertiesMap.insert(make_pair("R410a", ":/Resources/R410a_FluidPropertiesDataSet.idf"));
    m_fluidPropertiesMap.insert(make_pair("NH3", ":/Resources/NH3_FluidPropertiesDataSet.idf"));
    m_fluidPropertiesMap.insert(make_pair("R507a", ":/Resources/R507a_FluidPropertiesDataSet.idf"));
    m_fluidPropertiesMap.insert(make_pair("R744", ":/Resources/R744_FluidPropertiesDataSet.idf"));
  }

  boost::optional<IdfObject> ForwardTranslator::createFluidProperties(const std::string& glycolType, int glycolConcentration) {

    if (!(istringEqual("PropyleneGlycol", glycolType) || istringEqual("EthyleneGlycol", glycolType))) {
      return boost::none;
    }

    std::stringstream sstm;
    sstm << glycolType << "_" << glycolConcentration;
    std::string glycolName = sstm.str();

    auto it = std::find_if(m_idfObjects.cbegin(), m_idfObjects.cend(), [&glycolName](const IdfObject& i) {
      return (i.iddObject().type().value() == openstudio::IddObjectType::FluidProperties_Name)
             && openstudio::istringEqual(i.getString(FluidProperties_NameFields::FluidName, true).get(), glycolName);
    });
    if (it != m_idfObjects.cend()) {
      return *it;
    }

    // TODO: JM 2019-03-22 I am not sure you need this one
    // But I temporarily removed the \reference FluidAndGlycolNames from FluidProperties_GlycolConcentration to avoid problems of having two objects of
    // the same reference group bearing the same name (FluidProperties:Name also has the same reference group)
    IdfObject& fluidPropGlyConcentration = m_idfObjects.emplace_back(openstudio::IddObjectType::FluidProperties_GlycolConcentration);
    fluidPropGlyConcentration.setName(glycolName);
    fluidPropGlyConcentration.setString(FluidProperties_GlycolConcentrationFields::GlycolType, glycolType);
    fluidPropGlyConcentration.setDouble(FluidProperties_GlycolConcentrationFields::GlycolConcentration, glycolConcentration * 0.01);

    // NOTE: do not do another emplace_back before returning
    IdfObject& fluidPropName = m_idfObjects.emplace_back(openstudio::IddObjectType::FluidProperties_Name);
    fluidPropName.setString(FluidProperties_NameFields::FluidName, glycolName);
    fluidPropName.setString(FluidProperties_NameFields::FluidType, "Glycol");

    return {fluidPropName};
  }

  boost::optional<IdfObject> ForwardTranslator::createFluidProperties(const std::string& fluidType) {

    boost::optional<IdfObject> idfObject;
    boost::optional<IdfFile> idfFile;

    auto it = std::find_if(m_idfObjects.cbegin(), m_idfObjects.cend(), [&fluidType](const IdfObject& i) {
      return (i.iddObject().type().value() == openstudio::IddObjectType::FluidProperties_Name)
             && openstudio::istringEqual(i.getString(FluidProperties_NameFields::FluidName, true).get(), fluidType);
    });
    if (it != m_idfObjects.cend()) {
      return *it;
    }
    auto objInMapIt = m_fluidPropertiesMap.find(fluidType);
    if (objInMapIt != m_fluidPropertiesMap.end()) {
      idfFile = findIdfFile(objInMapIt->second);
    } else {
      LOG(Warn, "Fluid Type not valid choice: '" << fluidType << "'");
      return boost::none;
    }

    if (idfFile) {
      std::vector<IdfObject> fluidObjects = idfFile->objects();

      for (auto& i : fluidObjects) {
        if (i.iddObject().type().value() == openstudio::IddObjectType::FluidProperties_Name) {
          idfObject = i;
        }
        m_idfObjects.push_back(i);
      }
    }

    return idfObject;
  }

  boost::optional<IdfObject> ForwardTranslator::createSimpleSchedule(const std::string& name,
                                                                     const std::vector<std::pair<openstudio::Time, double>>& defaultDay,
                                                                     const std::vector<std::pair<openstudio::Time, double>>& summerDesignDay,
                                                                     const std::vector<std::pair<openstudio::Time, double>>& winterDesignDay) {

    IdfObject idfObject(openstudio::IddObjectType::Schedule_Compact);

    idfObject.setName(name);

    StringVector values;
    values.push_back("Through: 12/31");
    IdfExtensibleGroup eg = idfObject.pushExtensibleGroup(values);
    OS_ASSERT(!eg.empty());

    if (!summerDesignDay.empty()) {
      bool hasEndTime = false;
      double endTimeValue = 0.0;
      values[0] = "For: SummerDesignDay";
      eg = idfObject.pushExtensibleGroup(values);
      OS_ASSERT(!eg.empty());

      for (const auto& [time, val] : summerDesignDay) {

        int minutes = time.minutes();
        int hours = time.hours();

        if (0 == minutes && 0 == hours) {
          hasEndTime = true;
          endTimeValue = val;
          continue;
        }
        values[0] =
          "Until: " + std::string(hours < 10 ? "0" : "") + std::to_string(hours) + std::string(minutes < 10 ? ":0" : ":") + std::to_string(minutes);
        eg = idfObject.pushExtensibleGroup(values);
        OS_ASSERT(!eg.empty());
        values[0] = "";
        eg = idfObject.pushExtensibleGroup(values);
        OS_ASSERT(!eg.empty());
        bool ok = eg.setDouble(0, val);
        OS_ASSERT(ok);
      }
      if (hasEndTime) {
        values[0] = "Until: 24:00";
        eg = idfObject.pushExtensibleGroup(values);
        OS_ASSERT(!eg.empty());
        values[0] = "";
        eg = idfObject.pushExtensibleGroup(values);
        OS_ASSERT(!eg.empty());
        bool ok = eg.setDouble(0, endTimeValue);
        OS_ASSERT(ok);
      } else {
        LOG(Error, "Summer Design Day must have a value for all 24 hours");
      }
    }

    if (!winterDesignDay.empty()) {
      bool hasEndTime = false;
      double endTimeValue = 0.0;
      values[0] = "For: WinterDesignDay";
      eg = idfObject.pushExtensibleGroup(values);
      OS_ASSERT(!eg.empty());

      for (const auto& [time, val] : winterDesignDay) {

        int minutes = time.minutes();
        int hours = time.hours();

        if (0 == minutes && 0 == hours) {
          hasEndTime = true;
          endTimeValue = val;
          continue;
        }
        values[0] =
          "Until: " + std::string(hours < 10 ? "0" : "") + std::to_string(hours) + std::string(minutes < 10 ? ":0" : ":") + std::to_string(minutes);
        eg = idfObject.pushExtensibleGroup(values);
        OS_ASSERT(!eg.empty());
        values[0] = "";
        eg = idfObject.pushExtensibleGroup(values);
        OS_ASSERT(!eg.empty());
        bool ok = eg.setDouble(0, val);
        OS_ASSERT(ok);
      }
      if (hasEndTime) {
        values[0] = "Until: 24:00";
        eg = idfObject.pushExtensibleGroup(values);
        OS_ASSERT(!eg.empty());
        values[0] = "";
        eg = idfObject.pushExtensibleGroup(values);
        OS_ASSERT(!eg.empty());
        bool ok = eg.setDouble(0, endTimeValue);
        OS_ASSERT(ok);
      } else {
        LOG(Error, "Winter Design Day must have a value for all 24 hours");
      }
    }

    if (!summerDesignDay.empty() || !winterDesignDay.empty()) {
      values[0] = "For: AllOtherDays";
      eg = idfObject.pushExtensibleGroup(values);
      OS_ASSERT(!eg.empty());
    } else {
      values[0] = "For: AllDays";
      eg = idfObject.pushExtensibleGroup(values);
      OS_ASSERT(!eg.empty());
    }

    bool hasEndTime = false;
    double endTimeValue = 0.0;
    for (const auto& [time, val] : defaultDay) {

      int minutes = time.minutes();
      int hours = time.hours();

      if (0 == minutes && 0 == hours) {
        hasEndTime = true;
        endTimeValue = val;
        continue;
      }
      values[0] =
        "Until: " + std::string(hours < 10 ? "0" : "") + std::to_string(hours) + std::string(minutes < 10 ? ":0" : ":") + std::to_string(minutes);
      eg = idfObject.pushExtensibleGroup(values);
      OS_ASSERT(!eg.empty());
      values[0] = "";
      eg = idfObject.pushExtensibleGroup(values);
      OS_ASSERT(!eg.empty());
      bool ok = eg.setDouble(0, val);
      OS_ASSERT(ok);
    }
    if (hasEndTime) {
      values[0] = "Until: 24:00";
      eg = idfObject.pushExtensibleGroup(values);
      OS_ASSERT(!eg.empty());
      values[0] = "";
      eg = idfObject.pushExtensibleGroup(values);
      OS_ASSERT(!eg.empty());
      bool ok = eg.setDouble(0, endTimeValue);
      OS_ASSERT(ok);
    } else {
      LOG(Error, "Default Day must have a value for all 24 hours");
    }

    m_idfObjects.push_back(idfObject);

    return idfObject;
  }

  void ForwardTranslator::fixSPMsForUnitarySystem(const model::HVACComponent& unitary, const std::string& fanInletNodeName,
                                                  const std::string& fanOutletNodeName) {
    if (auto airSystem = unitary.airLoopHVAC()) {
      auto supplyComponents = airSystem->supplyComponents(airSystem->supplyInletNode(), unitary);
      auto oaSystems = subsetCastVector<model::AirLoopHVACOutdoorAirSystem>(supplyComponents);
      if (!oaSystems.empty()) {
        auto reliefComponents = oaSystems.back().oaComponents();
        supplyComponents.insert(supplyComponents.end(), reliefComponents.begin(), reliefComponents.end());
      }
      auto upstreamNodes = subsetCastVector<model::Node>(supplyComponents);

      for (const auto& node : upstreamNodes) {
        auto spms = subsetCastVector<model::SetpointManagerMixedAir>(node.setpointManagers());
        for (auto& spm : spms) {
          auto pred = [&spm](IdfObject& idfObject) {
            if (idfObject.iddObject().type() == IddObjectType::SetpointManager_MixedAir) {
              auto idfName = idfObject.name();
              auto osName = spm.name();
              if (idfName && osName && (osName.get() == idfName.get())) {
                return true;
              }
            }
            return false;
          };
          auto spm_idf = std::find_if(m_idfObjects.begin(), m_idfObjects.end(), pred);
          if (spm_idf != m_idfObjects.end()) {
            auto result = spm_idf->getString(SetpointManager_MixedAirFields::FanInletNodeName);
            if (!result || result->empty()) {
              spm_idf->setString(SetpointManager_MixedAirFields::FanInletNodeName, fanInletNodeName);
            }
            result = spm_idf->getString(SetpointManager_MixedAirFields::FanOutletNodeName);
            if (!result || result->empty()) {
              spm_idf->setString(SetpointManager_MixedAirFields::FanOutletNodeName, fanOutletNodeName);
            }
          }
        }
      }
    }
  }

}  // namespace energyplus

}  // namespace openstudio
