/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "Model.hpp"
#include "Model_Impl.hpp"

#include "Component.hpp"
#include "ComponentWatcher_Impl.hpp"
#include "Connection.hpp"
#include "ModelObject.hpp"
#include "ModelObject_Impl.hpp"
#include "ResourceObject.hpp"
#include "ResourceObject_Impl.hpp"

// central list of all concrete ModelObject header files (_Impl and non-_Impl)
// needed here for ::createObject
#include "ConcreteModelObjects.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Version_FieldEnums.hxx>

#include "../osversion/VersionTranslator.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/ContainersMove.hpp"
#include "../utilities/core/Filesystem.hpp"
#include "../utilities/core/PathHelpers.hpp"

#include "../utilities/idd/IddEnums.hpp"
#include "../utilities/idd/IddObject_Impl.hpp"
#include "../utilities/idd/IddField_Impl.hpp"
#include "../utilities/idd/IddFile_Impl.hpp"
#include "../utilities/idf/Workspace_Impl.hpp"  // needed for serialization

#include "../utilities/idf/IdfFile.hpp"

#include "../utilities/math/FloatCompare.hpp"

#include "../utilities/plot/ProgressBar.hpp"

#include "../utilities/sql/SqlFile.hpp"

#include <boost/regex.hpp>

using openstudio::IddObjectType;
using openstudio::detail::WorkspaceObject_Impl;

using std::dynamic_pointer_cast;

namespace openstudio {
namespace model {

  namespace detail {

    // default constructor
    Model_Impl::Model_Impl() : Workspace_Impl(StrictnessLevel::Draft, IddFileType::OpenStudio) {
      // careful not to call anything that calls shared_from_this here, this is not yet constructed
    }

    Model_Impl::Model_Impl(const IdfFile& idfFile) : Workspace_Impl(idfFile, StrictnessLevel(StrictnessLevel::Draft)) {
      // careful not to call anything that calls shared_from_this here, this is not yet constructed
      if (iddFileType() != IddFileType::OpenStudio) {
        LOG_AND_THROW("Models must be constructed with the OpenStudio Idd as the underlying "
                      << "data schema. (Attempted construction from IdfFile with IddFileType " << idfFile.iddFileType().valueDescription() << ".)");
      }
    }

    Model_Impl::Model_Impl(const openstudio::detail::Workspace_Impl& workspace, bool keepHandles)
      : openstudio::detail::Workspace_Impl(workspace, keepHandles) {
      // careful not to call anything that calls shared_from_this here, this is not yet constructed
      if (iddFileType() != IddFileType::OpenStudio) {
        LOG_AND_THROW("Models must be constructed with the OpenStudio Idd as the underlying "
                      << "data schema. (Attempted construction from Workspace with IddFileType " << workspace.iddFileType().valueDescription()
                      << ".)");
      }
    }

    // copy constructor, used for clone
    Model_Impl::Model_Impl(const Model_Impl& other, bool keepHandles)
      : Workspace_Impl(other, keepHandles),
        m_sqlFile((other.m_sqlFile) ? (std::shared_ptr<SqlFile>(new SqlFile(*other.m_sqlFile))) : (other.m_sqlFile)),
        m_workflowJSON(WorkflowJSON(other.m_workflowJSON)) {
      // notice we are cloning the workflow and sqlfile too, if necessary
      // careful not to call anything that calls shared_from_this here, this is not yet constructed
    }

    // copy constructor used for cloneSubset
    Model_Impl::Model_Impl(const Model_Impl& other, const std::vector<Handle>& hs, bool keepHandles, StrictnessLevel level)
      : Workspace_Impl(other, hs, keepHandles, level),
        m_sqlFile((other.m_sqlFile) ? (std::shared_ptr<SqlFile>(new SqlFile(*other.m_sqlFile))) : (other.m_sqlFile)),
        m_workflowJSON(WorkflowJSON(other.m_workflowJSON)) {
      // notice we are cloning the workflow and sqlfile too, if necessary
    }
    Workspace Model_Impl::clone(bool keepHandles) const {
      // copy everything but objects
      std::shared_ptr<Model_Impl> cloneImpl(new Model_Impl(*this, keepHandles));
      // clone objects
      createAndAddClonedObjects(model().getImpl<Model_Impl>(), cloneImpl, keepHandles);
      cloneImpl->createComponentWatchers();
      // wrap impl and return
      Model result(cloneImpl);
      return result.cast<Workspace>();
    }

    Workspace Model_Impl::cloneSubset(const std::vector<Handle>& handles, bool keepHandles, StrictnessLevel level) const {
      // copy everything but objects
      std::shared_ptr<Model_Impl> cloneImpl(new Model_Impl(*this, handles, keepHandles, level));
      // clone objects
      createAndAddSubsetClonedObjects(model().getImpl<Model_Impl>(), cloneImpl, handles, keepHandles);
      // wrap impl and return
      Model result(cloneImpl);
      return result.cast<Model>();
    }

    void Model_Impl::swap(Workspace& other) {
      // Workspace::swap guarantees that other is a Model

      // swap Workspace-level data
      openstudio::detail::Workspace_Impl::swap(other);

      // swap Model-level data
      std::shared_ptr<Model_Impl> otherImpl = other.getImpl<detail::Model_Impl>();

      WorkflowJSON twf = m_workflowJSON;
      setWorkflowJSON(otherImpl->workflowJSON());
      otherImpl->setWorkflowJSON(twf);

      std::shared_ptr<SqlFile> tsf = m_sqlFile;
      m_sqlFile = otherImpl->m_sqlFile;
      otherImpl->m_sqlFile = tsf;

      ComponentWatcherVector tcw = m_componentWatchers;
      m_componentWatchers = otherImpl->m_componentWatchers;
      otherImpl->m_componentWatchers = tcw;

      clearCachedData();
      otherImpl->clearCachedData();
    }

    void Model_Impl::createComponentWatchers() {
      ComponentDataVector componentDataObjects = castVector<ComponentData>(getObjectsByType(ComponentData::iddObjectType()));
      if (!m_componentWatchers.empty()) {
        OS_ASSERT(m_componentWatchers.size() == componentDataObjects.size());
        return;
      }
      for (ComponentData& object : componentDataObjects) {
        mf_createComponentWatcher(object);
      }
    }

    // Overriding this from WorkspaceObject_Impl is how all objects in the model end up
    // as model objects
    std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> Model_Impl::createObject(const IdfObject& object, bool keepHandle) {
      auto result = modelObjectCreator.getNew(this, object, keepHandle);

      if (!result) {
        LOG(Warn, "Creating GenericModelObject for IddObjectType '" << object.iddObject().type().valueName() << "'.");
        result = std::shared_ptr<GenericModelObject_Impl>(new GenericModelObject_Impl(object, this, keepHandle));
      }

      return result;
    }

    std::shared_ptr<openstudio::detail::WorkspaceObject_Impl>
      Model_Impl::createObject(const std::shared_ptr<openstudio::detail::WorkspaceObject_Impl>& originalObjectImplPtr, bool keepHandle) {

      OS_ASSERT(originalObjectImplPtr);

      auto result = modelObjectCreator.getCopy(this, originalObjectImplPtr, keepHandle);

      if (!result) {
        LOG(Warn, "Creating GenericModelObject for IddObjectType '" << originalObjectImplPtr->iddObject().type().valueName() << "'.");
        if (dynamic_pointer_cast<GenericModelObject_Impl>(originalObjectImplPtr)) {
          result = std::shared_ptr<GenericModelObject_Impl>(
            new GenericModelObject_Impl(*dynamic_pointer_cast<GenericModelObject_Impl>(originalObjectImplPtr), this, keepHandle));
        } else {
          if (dynamic_pointer_cast<ModelObject_Impl>(originalObjectImplPtr)) {
            std::cout << "Please register copy constructors for IddObjectType '" << originalObjectImplPtr->iddObject().type().valueName() << "'."
                      << '\n';
            LOG_AND_THROW("Trying to copy a ModelObject, but the copy constructors are not "
                          << "registered for IddObjectType '" << originalObjectImplPtr->iddObject().type().valueName() << "'.");
          }
          result = std::shared_ptr<GenericModelObject_Impl>(new GenericModelObject_Impl(*originalObjectImplPtr, this, keepHandle));
        }
      }

      return result;
    }

    Model Model_Impl::model() const {
      // const cast looks pretty bad but is justified here as this operation does not
      // modify the model, this is similar to a copy constructor, don't abuse it though
      return {std::dynamic_pointer_cast<Model_Impl>(std::const_pointer_cast<openstudio::detail::Workspace_Impl>(this->shared_from_this()))};
    }

    bool Model_Impl::setIddFile(IddFileType iddFileType) {
      OS_ASSERT(iddFileType == IddFileType::OpenStudio);
      return false;
    }

    boost::optional<Building> Model_Impl::building() const {
      if (m_cachedBuilding) {
        return m_cachedBuilding;
      }

      boost::optional<Building> result = this->model().getOptionalUniqueModelObject<Building>();
      if (result) {
        m_cachedBuilding = result;
        result->getImpl<Building_Impl>()->Building_Impl::onRemoveFromWorkspace.connect<Model_Impl, &Model_Impl::clearCachedBuilding>(
          const_cast<openstudio::model::detail::Model_Impl*>(this));
      }

      return m_cachedBuilding;
    }

    boost::optional<FoundationKivaSettings> Model_Impl::foundationKivaSettings() const {
      if (m_cachedFoundationKivaSettings) {
        return m_cachedFoundationKivaSettings;
      }

      boost::optional<FoundationKivaSettings> result = this->model().getOptionalUniqueModelObject<FoundationKivaSettings>();
      if (result) {
        m_cachedFoundationKivaSettings = result;
        result->getImpl<FoundationKivaSettings_Impl>()
          ->FoundationKivaSettings_Impl::onRemoveFromWorkspace.connect<Model_Impl, &Model_Impl::clearCachedFoundationKivaSettings>(
            const_cast<openstudio::model::detail::Model_Impl*>(this));
      }

      return m_cachedFoundationKivaSettings;
    }

    boost::optional<OutputControlFiles> Model_Impl::outputControlFiles() const {
      if (m_cachedOutputControlFiles) {
        return m_cachedOutputControlFiles;
      }

      boost::optional<OutputControlFiles> result = this->model().getOptionalUniqueModelObject<OutputControlFiles>();
      if (result) {
        m_cachedOutputControlFiles = result;
        result->getImpl<OutputControlFiles_Impl>()
          ->OutputControlFiles_Impl::onRemoveFromWorkspace.connect<Model_Impl, &Model_Impl::clearCachedOutputControlFiles>(
            const_cast<openstudio::model::detail::Model_Impl*>(this));
      }

      return m_cachedOutputControlFiles;
    }

    boost::optional<OutputControlReportingTolerances> Model_Impl::outputControlReportingTolerances() const {
      if (m_cachedOutputControlReportingTolerances) {
        return m_cachedOutputControlReportingTolerances;
      }

      boost::optional<OutputControlReportingTolerances> result = this->model().getOptionalUniqueModelObject<OutputControlReportingTolerances>();
      if (result) {
        m_cachedOutputControlReportingTolerances = result;
        result->getImpl<OutputControlReportingTolerances_Impl>()
          ->OutputControlReportingTolerances_Impl::onRemoveFromWorkspace
          .connect<Model_Impl, &Model_Impl::clearCachedOutputControlReportingTolerances>(const_cast<openstudio::model::detail::Model_Impl*>(this));
      }

      return m_cachedOutputControlReportingTolerances;
    }

    boost::optional<OutputControlTableStyle> Model_Impl::outputControlTableStyle() const {
      if (m_cachedOutputControlTableStyle) {
        return m_cachedOutputControlTableStyle;
      }

      boost::optional<OutputControlTableStyle> result = this->model().getOptionalUniqueModelObject<OutputControlTableStyle>();
      if (result) {
        m_cachedOutputControlTableStyle = result;
        result->getImpl<OutputControlTableStyle_Impl>()
          .get()
          ->OutputControlTableStyle_Impl::onRemoveFromWorkspace.connect<Model_Impl, &Model_Impl::clearCachedOutputControlTableStyle>(
            const_cast<openstudio::model::detail::Model_Impl*>(this));
      }

      return m_cachedOutputControlTableStyle;
    }

    boost::optional<OutputControlTimestamp> Model_Impl::outputControlTimestamp() const {
      if (m_cachedOutputControlTimestamp) {
        return m_cachedOutputControlTimestamp;
      }

      boost::optional<OutputControlTimestamp> result = this->model().getOptionalUniqueModelObject<OutputControlTimestamp>();
      if (result) {
        m_cachedOutputControlTimestamp = result;
        result->getImpl<OutputControlTimestamp_Impl>()
          ->OutputControlTimestamp_Impl::onRemoveFromWorkspace.connect<Model_Impl, &Model_Impl::clearCachedOutputControlTimestamp>(
            const_cast<openstudio::model::detail::Model_Impl*>(this));
      }

      return m_cachedOutputControlTimestamp;
    }

    boost::optional<OutputDiagnostics> Model_Impl::outputDiagnostics() const {
      if (m_cachedOutputDiagnostics) {
        return m_cachedOutputDiagnostics;
      }

      boost::optional<OutputDiagnostics> result = this->model().getOptionalUniqueModelObject<OutputDiagnostics>();
      if (result) {
        m_cachedOutputDiagnostics = result;
        result->getImpl<OutputDiagnostics_Impl>()
          ->OutputDiagnostics_Impl::onRemoveFromWorkspace.connect<Model_Impl, &Model_Impl::clearCachedOutputDiagnostics>(
            const_cast<openstudio::model::detail::Model_Impl*>(this));
      }

      return m_cachedOutputDiagnostics;
    }

    boost::optional<OutputDebuggingData> Model_Impl::outputDebuggingData() const {
      if (m_cachedOutputDebuggingData) {
        return m_cachedOutputDebuggingData;
      }

      boost::optional<OutputDebuggingData> result = this->model().getOptionalUniqueModelObject<OutputDebuggingData>();
      if (result) {
        m_cachedOutputDebuggingData = result;
        result->getImpl<OutputDebuggingData_Impl>()
          ->OutputDebuggingData_Impl::onRemoveFromWorkspace.connect<Model_Impl, &Model_Impl::clearCachedOutputDebuggingData>(
            const_cast<openstudio::model::detail::Model_Impl*>(this));
      }

      return m_cachedOutputDebuggingData;
    }

    boost::optional<OutputJSON> Model_Impl::outputJSON() const {
      if (m_cachedOutputJSON) {
        return m_cachedOutputJSON;
      }

      boost::optional<OutputJSON> result = this->model().getOptionalUniqueModelObject<OutputJSON>();
      if (result) {
        m_cachedOutputJSON = result;
        result->getImpl<OutputJSON_Impl>()->OutputJSON_Impl::onRemoveFromWorkspace.connect<Model_Impl, &Model_Impl::clearCachedOutputJSON>(
          const_cast<openstudio::model::detail::Model_Impl*>(this));
      }

      return m_cachedOutputJSON;
    }

    boost::optional<OutputSQLite> Model_Impl::outputSQLite() const {
      if (m_cachedOutputSQLite) {
        return m_cachedOutputSQLite;
      }

      boost::optional<OutputSQLite> result = this->model().getOptionalUniqueModelObject<OutputSQLite>();
      if (result) {
        m_cachedOutputSQLite = result;
        result->getImpl<OutputSQLite_Impl>()
          .get()
          ->OutputSQLite_Impl::onRemoveFromWorkspace.connect<Model_Impl, &Model_Impl::clearCachedOutputSQLite>(
            const_cast<openstudio::model::detail::Model_Impl*>(this));
      }

      return m_cachedOutputSQLite;
    }

    boost::optional<OutputEnergyManagementSystem> Model_Impl::outputEnergyManagementSystem() const {
      if (m_cachedOutputEnergyManagementSystem) {
        return m_cachedOutputEnergyManagementSystem;
      }

      boost::optional<OutputEnergyManagementSystem> result = this->model().getOptionalUniqueModelObject<OutputEnergyManagementSystem>();
      if (result) {
        m_cachedOutputEnergyManagementSystem = result;
        result->getImpl<OutputEnergyManagementSystem_Impl>()
          ->OutputEnergyManagementSystem_Impl::onRemoveFromWorkspace.connect<Model_Impl, &Model_Impl::clearCachedOutputEnergyManagementSystem>(
            const_cast<openstudio::model::detail::Model_Impl*>(this));
      }

      return m_cachedOutputEnergyManagementSystem;
    }

    boost::optional<OutputTableSummaryReports> Model_Impl::outputTableSummaryReports() const {
      if (m_cachedOutputTableSummaryReports) {
        return m_cachedOutputTableSummaryReports;
      }

      boost::optional<OutputTableSummaryReports> result = this->model().getOptionalUniqueModelObject<OutputTableSummaryReports>();
      if (result) {
        m_cachedOutputTableSummaryReports = result;
        result->getImpl<OutputTableSummaryReports_Impl>()
          ->OutputTableSummaryReports_Impl::onRemoveFromWorkspace.connect<Model_Impl, &Model_Impl::clearCachedOutputTableSummaryReports>(
            const_cast<openstudio::model::detail::Model_Impl*>(this));
      }

      return m_cachedOutputTableSummaryReports;
    }

    boost::optional<OutputSchedules> Model_Impl::outputSchedules() const {
      if (m_cachedOutputSchedules) {
        return m_cachedOutputSchedules;
      }

      boost::optional<OutputSchedules> result = this->model().getOptionalUniqueModelObject<OutputSchedules>();
      if (result) {
        m_cachedOutputSchedules = result;
        result->getImpl<OutputSchedules_Impl>()
          .get()
          ->OutputSchedules_Impl::onRemoveFromWorkspace.connect<Model_Impl, &Model_Impl::clearCachedOutputSchedules>(
            const_cast<openstudio::model::detail::Model_Impl*>(this));
      }

      return m_cachedOutputSchedules;
    }

    boost::optional<OutputConstructions> Model_Impl::outputConstructions() const {
      if (m_cachedOutputConstructions) {
        return m_cachedOutputConstructions;
      }

      boost::optional<OutputConstructions> result = this->model().getOptionalUniqueModelObject<OutputConstructions>();
      if (result) {
        m_cachedOutputConstructions = result;
        result->getImpl<OutputConstructions_Impl>()
          .get()
          ->OutputConstructions_Impl::onRemoveFromWorkspace.connect<Model_Impl, &Model_Impl::clearCachedOutputConstructions>(
            const_cast<openstudio::model::detail::Model_Impl*>(this));
      }

      return m_cachedOutputConstructions;
    }

    boost::optional<PerformancePrecisionTradeoffs> Model_Impl::performancePrecisionTradeoffs() const {
      if (m_cachedPerformancePrecisionTradeoffs) {
        return m_cachedPerformancePrecisionTradeoffs;
      }

      boost::optional<PerformancePrecisionTradeoffs> result = this->model().getOptionalUniqueModelObject<PerformancePrecisionTradeoffs>();
      if (result) {
        m_cachedPerformancePrecisionTradeoffs = result;
        result->getImpl<PerformancePrecisionTradeoffs_Impl>()
          ->PerformancePrecisionTradeoffs_Impl::onRemoveFromWorkspace.connect<Model_Impl, &Model_Impl::clearCachedPerformancePrecisionTradeoffs>(
            const_cast<openstudio::model::detail::Model_Impl*>(this));
      }

      return m_cachedPerformancePrecisionTradeoffs;
    }

    boost::optional<LifeCycleCostParameters> Model_Impl::lifeCycleCostParameters() const {
      if (m_cachedLifeCycleCostParameters) {
        return m_cachedLifeCycleCostParameters;
      }

      boost::optional<LifeCycleCostParameters> result = this->model().getOptionalUniqueModelObject<LifeCycleCostParameters>();
      if (result) {
        m_cachedLifeCycleCostParameters = result;
        result->getImpl<LifeCycleCostParameters_Impl>()
          ->LifeCycleCostParameters_Impl::onRemoveFromWorkspace.connect<Model_Impl, &Model_Impl::clearCachedLifeCycleCostParameters>(
            const_cast<openstudio::model::detail::Model_Impl*>(this));
      }

      return m_cachedLifeCycleCostParameters;
    }

    boost::optional<SizingParameters> Model_Impl::sizingParameters() const {
      if (m_cachedSizingParameters) {
        return m_cachedSizingParameters;
      }

      boost::optional<SizingParameters> result = this->model().getOptionalUniqueModelObject<SizingParameters>();
      if (result) {
        m_cachedSizingParameters = result;
        result->getImpl<SizingParameters_Impl>()
          ->SizingParameters_Impl::onRemoveFromWorkspace.connect<Model_Impl, &Model_Impl::clearCachedSizingParameters>(
            const_cast<openstudio::model::detail::Model_Impl*>(this));
      }

      return m_cachedSizingParameters;
    }

    boost::optional<RadianceParameters> Model_Impl::radianceParameters() const {
      if (m_cachedRadianceParameters) {
        return m_cachedRadianceParameters;
      }

      boost::optional<RadianceParameters> result = this->model().getOptionalUniqueModelObject<RadianceParameters>();
      if (result) {
        m_cachedRadianceParameters = result;
        result->getImpl<RadianceParameters_Impl>()
          ->RadianceParameters_Impl::onRemoveFromWorkspace.connect<Model_Impl, &Model_Impl::clearCachedRadianceParameters>(
            const_cast<openstudio::model::detail::Model_Impl*>(this));
      }

      return m_cachedRadianceParameters;
    }

    boost::optional<RunPeriod> Model_Impl::runPeriod() const {
      if (m_cachedRunPeriod) {
        return m_cachedRunPeriod;
      }

      boost::optional<RunPeriod> result = this->model().getOptionalUniqueModelObject<RunPeriod>();
      if (result) {
        m_cachedRunPeriod = result;
        result->getImpl<RunPeriod_Impl>()->RunPeriod_Impl::onRemoveFromWorkspace.connect<Model_Impl, &Model_Impl::clearCachedRunPeriod>(
          const_cast<openstudio::model::detail::Model_Impl*>(this));
      }

      return m_cachedRunPeriod;
    }

    boost::optional<RunPeriodControlDaylightSavingTime> Model_Impl::runPeriodControlDaylightSavingTime() const {
      if (m_cachedRunPeriodControlDaylightSavingTime) {
        return m_cachedRunPeriodControlDaylightSavingTime;
      }

      boost::optional<RunPeriodControlDaylightSavingTime> result = this->model().getOptionalUniqueModelObject<RunPeriodControlDaylightSavingTime>();
      if (result) {
        m_cachedRunPeriodControlDaylightSavingTime = result;
        result->getImpl<RunPeriodControlDaylightSavingTime_Impl>()
          ->RunPeriodControlDaylightSavingTime_Impl::onRemoveFromWorkspace
          .connect<Model_Impl, &Model_Impl::clearCachedRunPeriodControlDaylightSavingTime>(const_cast<openstudio::model::detail::Model_Impl*>(this));
      }

      return m_cachedRunPeriodControlDaylightSavingTime;
    }

    boost::optional<YearDescription> Model_Impl::yearDescription() const {
      if (m_cachedYearDescription) {
        return m_cachedYearDescription;
      }

      boost::optional<YearDescription> result = this->model().getOptionalUniqueModelObject<YearDescription>();
      if (result) {
        m_cachedYearDescription = result;
        result->getImpl<YearDescription_Impl>()
          ->YearDescription_Impl::onRemoveFromWorkspace.connect<Model_Impl, &Model_Impl::clearCachedYearDescription>(
            const_cast<openstudio::model::detail::Model_Impl*>(this));
      }

      return m_cachedYearDescription;
    }

    boost::optional<Site> Model_Impl::site() const {
      if (m_cachedSite) {
        return m_cachedSite;
      }

      boost::optional<Site> result = this->model().getOptionalUniqueModelObject<Site>();
      if (result) {
        m_cachedSite = result;
        result->getImpl<Site_Impl>()->Site_Impl::onRemoveFromWorkspace.connect<Model_Impl, &Model_Impl::clearCachedSite>(
          const_cast<openstudio::model::detail::Model_Impl*>(this));
      }

      return m_cachedSite;
    }

    boost::optional<SiteGroundReflectance> Model_Impl::siteGroundReflectance() const {
      if (m_cachedSiteGroundReflectance) {
        return m_cachedSiteGroundReflectance;
      }

      boost::optional<SiteGroundReflectance> result = this->model().getOptionalUniqueModelObject<SiteGroundReflectance>();
      if (result) {
        m_cachedSiteGroundReflectance = result;
        result->getImpl<SiteGroundReflectance_Impl>()
          ->SiteGroundReflectance_Impl::onRemoveFromWorkspace.connect<Model_Impl, &Model_Impl::clearCachedSiteGroundReflectance>(
            const_cast<openstudio::model::detail::Model_Impl*>(this));
      }

      return m_cachedSiteGroundReflectance;
    }

    boost::optional<SiteWaterMainsTemperature> Model_Impl::siteWaterMainsTemperature() const {
      if (m_cachedSiteWaterMainsTemperature) {
        return m_cachedSiteWaterMainsTemperature;
      }

      boost::optional<SiteWaterMainsTemperature> result = this->model().getOptionalUniqueModelObject<SiteWaterMainsTemperature>();
      if (result) {
        m_cachedSiteWaterMainsTemperature = result;
        result->getImpl<SiteWaterMainsTemperature_Impl>()
          ->SiteWaterMainsTemperature_Impl::onRemoveFromWorkspace.connect<Model_Impl, &Model_Impl::clearCachedSiteWaterMainsTemperature>(
            const_cast<openstudio::model::detail::Model_Impl*>(this));
      }

      return m_cachedSiteWaterMainsTemperature;
    }

    boost::optional<SiteGroundTemperatureBuildingSurface> Model_Impl::siteGroundTemperatureBuildingSurface() const {
      if (m_cachedSiteGroundTemperatureBuildingSurface) {
        return m_cachedSiteGroundTemperatureBuildingSurface;
      }

      boost::optional<SiteGroundTemperatureBuildingSurface> result =
        this->model().getOptionalUniqueModelObject<SiteGroundTemperatureBuildingSurface>();
      if (result) {
        m_cachedSiteGroundTemperatureBuildingSurface = result;
        result->getImpl<SiteGroundTemperatureBuildingSurface_Impl>()
          ->SiteGroundTemperatureBuildingSurface_Impl::onRemoveFromWorkspace
          .connect<Model_Impl, &Model_Impl::clearCachedSiteGroundTemperatureBuildingSurface>(
            const_cast<openstudio::model::detail::Model_Impl*>(this));
      }

      return m_cachedSiteGroundTemperatureBuildingSurface;
    }

    boost::optional<SiteGroundTemperatureFCfactorMethod> Model_Impl::siteGroundTemperatureFCfactorMethod() const {
      if (m_cachedSiteGroundTemperatureFCfactorMethod) {
        return m_cachedSiteGroundTemperatureFCfactorMethod;
      }

      boost::optional<SiteGroundTemperatureFCfactorMethod> result = this->model().getOptionalUniqueModelObject<SiteGroundTemperatureFCfactorMethod>();
      if (result) {
        m_cachedSiteGroundTemperatureFCfactorMethod = result;
        result->getImpl<SiteGroundTemperatureFCfactorMethod_Impl>()
          ->SiteGroundTemperatureFCfactorMethod_Impl::onRemoveFromWorkspace
          .connect<Model_Impl, &Model_Impl::clearCachedSiteGroundTemperatureFCfactorMethod>(const_cast<openstudio::model::detail::Model_Impl*>(this));
      }

      return m_cachedSiteGroundTemperatureFCfactorMethod;
    }

    boost::optional<SiteGroundTemperatureDeep> Model_Impl::siteGroundTemperatureDeep() const {
      if (m_cachedSiteGroundTemperatureDeep) {
        return m_cachedSiteGroundTemperatureDeep;
      }

      boost::optional<SiteGroundTemperatureDeep> result = this->model().getOptionalUniqueModelObject<SiteGroundTemperatureDeep>();
      if (result) {
        m_cachedSiteGroundTemperatureDeep = result;
        result->getImpl<SiteGroundTemperatureDeep_Impl>()
          ->SiteGroundTemperatureDeep_Impl::onRemoveFromWorkspace.connect<Model_Impl, &Model_Impl::clearCachedSiteGroundTemperatureDeep>(
            const_cast<openstudio::model::detail::Model_Impl*>(this));
      }

      return m_cachedSiteGroundTemperatureDeep;
    }

    boost::optional<SiteGroundTemperatureShallow> Model_Impl::siteGroundTemperatureShallow() const {
      if (m_cachedSiteGroundTemperatureShallow) {
        return m_cachedSiteGroundTemperatureShallow;
      }

      boost::optional<SiteGroundTemperatureShallow> result = this->model().getOptionalUniqueModelObject<SiteGroundTemperatureShallow>();
      if (result) {
        m_cachedSiteGroundTemperatureShallow = result;
        result->getImpl<SiteGroundTemperatureShallow_Impl>()
          ->SiteGroundTemperatureShallow_Impl::onRemoveFromWorkspace.connect<Model_Impl, &Model_Impl::clearCachedSiteGroundTemperatureShallow>(
            const_cast<openstudio::model::detail::Model_Impl*>(this));
      }

      return m_cachedSiteGroundTemperatureShallow;
    }

    boost::optional<Facility> Model_Impl::facility() const {
      if (m_cachedFacility) {
        return m_cachedFacility;
      }

      boost::optional<Facility> result = this->model().getOptionalUniqueModelObject<Facility>();
      if (result) {
        m_cachedFacility = result;
        result->getImpl<Facility_Impl>()->Facility_Impl::onRemoveFromWorkspace.connect<Model_Impl, &Model_Impl::clearCachedFacility>(
          const_cast<openstudio::model::detail::Model_Impl*>(this));
      }

      return m_cachedFacility;
    }

    boost::optional<WeatherFile> Model_Impl::weatherFile() const {
      if (m_cachedWeatherFile) {
        return m_cachedWeatherFile;
      }

      boost::optional<WeatherFile> result = this->model().getOptionalUniqueModelObject<WeatherFile>();
      if (result) {
        m_cachedWeatherFile = result;
        result->getImpl<WeatherFile_Impl>()->WeatherFile_Impl::onRemoveFromWorkspace.connect<Model_Impl, &Model_Impl::clearCachedWeatherFile>(
          const_cast<openstudio::model::detail::Model_Impl*>(this));
      }

      return m_cachedWeatherFile;
    }

    boost::optional<SimulationControl> Model_Impl::simulationControl() const {
      if (m_cachedSimulationControl) {
        return m_cachedSimulationControl;
      }

      boost::optional<SimulationControl> result = this->model().getOptionalUniqueModelObject<SimulationControl>();
      if (result) {
        m_cachedSimulationControl = result;
        result->getImpl<SimulationControl_Impl>()
          ->SimulationControl_Impl::onRemoveFromWorkspace.connect<Model_Impl, &Model_Impl::clearCachedSimulationControl>(
            const_cast<openstudio::model::detail::Model_Impl*>(this));
      }

      return m_cachedSimulationControl;
    }

    boost::optional<LightingSimulationControl> Model_Impl::lightingSimulationControl() const {
      if (m_cachedLightingSimulationControl) {
        return m_cachedLightingSimulationControl;
      }

      boost::optional<LightingSimulationControl> result = this->model().getOptionalUniqueModelObject<LightingSimulationControl>();
      if (result) {
        m_cachedLightingSimulationControl = result;
        result->getImpl<LightingSimulationControl_Impl>()
          ->LightingSimulationControl_Impl::onRemoveFromWorkspace.connect<Model_Impl, &Model_Impl::clearCachedLightingSimulationControl>(
            const_cast<openstudio::model::detail::Model_Impl*>(this));
      }

      return m_cachedLightingSimulationControl;
    }

    boost::optional<AirflowNetworkSimulationControl> Model_Impl::airflowNetworkSimulationControl() const {
      if (m_cachedAirflowNetworkSimulationControl) {
        return m_cachedAirflowNetworkSimulationControl;
      }

      boost::optional<AirflowNetworkSimulationControl> result = this->model().getOptionalUniqueModelObject<AirflowNetworkSimulationControl>();
      if (result) {
        m_cachedAirflowNetworkSimulationControl = result;
        result->getImpl<AirflowNetworkSimulationControl_Impl>()
          ->AirflowNetworkSimulationControl_Impl::onRemoveFromWorkspace.connect<Model_Impl, &Model_Impl::clearCachedAirflowNetworkSimulationControl>(
            const_cast<openstudio::model::detail::Model_Impl*>(this));
      }

      return m_cachedAirflowNetworkSimulationControl;
    }

    boost::optional<InsideSurfaceConvectionAlgorithm> Model_Impl::insideSurfaceConvectionAlgorithm() const {
      if (m_cachedInsideSurfaceConvectionAlgorithm) {
        return m_cachedInsideSurfaceConvectionAlgorithm;
      }

      boost::optional<InsideSurfaceConvectionAlgorithm> result = this->model().getOptionalUniqueModelObject<InsideSurfaceConvectionAlgorithm>();
      if (result) {
        m_cachedInsideSurfaceConvectionAlgorithm = result;
        result->getImpl<InsideSurfaceConvectionAlgorithm_Impl>()
          ->InsideSurfaceConvectionAlgorithm_Impl::onRemoveFromWorkspace
          .connect<Model_Impl, &Model_Impl::clearCachedInsideSurfaceConvectionAlgorithm>(const_cast<openstudio::model::detail::Model_Impl*>(this));
      }

      return m_cachedInsideSurfaceConvectionAlgorithm;
    }

    boost::optional<OutsideSurfaceConvectionAlgorithm> Model_Impl::outsideSurfaceConvectionAlgorithm() const {
      if (m_cachedOutsideSurfaceConvectionAlgorithm) {
        return m_cachedOutsideSurfaceConvectionAlgorithm;
      }

      boost::optional<OutsideSurfaceConvectionAlgorithm> result = this->model().getOptionalUniqueModelObject<OutsideSurfaceConvectionAlgorithm>();
      if (result) {
        m_cachedOutsideSurfaceConvectionAlgorithm = result;
        result->getImpl<OutsideSurfaceConvectionAlgorithm_Impl>()
          ->OutsideSurfaceConvectionAlgorithm_Impl::onRemoveFromWorkspace
          .connect<Model_Impl, &Model_Impl::clearCachedOutsideSurfaceConvectionAlgorithm>(const_cast<openstudio::model::detail::Model_Impl*>(this));
      }

      return m_cachedOutsideSurfaceConvectionAlgorithm;
    }

    boost::optional<HeatBalanceAlgorithm> Model_Impl::heatBalanceAlgorithm() const {
      if (m_cachedHeatBalanceAlgorithm) {
        return m_cachedHeatBalanceAlgorithm;
      }

      boost::optional<HeatBalanceAlgorithm> result = this->model().getOptionalUniqueModelObject<HeatBalanceAlgorithm>();
      if (result) {
        m_cachedHeatBalanceAlgorithm = result;
        result->getImpl<HeatBalanceAlgorithm_Impl>()
          ->HeatBalanceAlgorithm_Impl::onRemoveFromWorkspace.connect<Model_Impl, &Model_Impl::clearCachedHeatBalanceAlgorithm>(
            const_cast<openstudio::model::detail::Model_Impl*>(this));
      }

      return m_cachedHeatBalanceAlgorithm;
    }

    boost::optional<ZoneAirContaminantBalance> Model_Impl::zoneAirContaminantBalance() const {
      if (m_cachedZoneAirContaminantBalance) {
        return m_cachedZoneAirContaminantBalance;
      }

      boost::optional<ZoneAirContaminantBalance> result = this->model().getOptionalUniqueModelObject<ZoneAirContaminantBalance>();
      if (result) {
        m_cachedZoneAirContaminantBalance = result;
        result->getImpl<ZoneAirContaminantBalance_Impl>()
          ->ZoneAirContaminantBalance_Impl::onRemoveFromWorkspace.connect<Model_Impl, &Model_Impl::clearCachedZoneAirContaminantBalance>(
            const_cast<openstudio::model::detail::Model_Impl*>(this));
      }

      return m_cachedZoneAirContaminantBalance;
    }

    boost::optional<ZoneAirHeatBalanceAlgorithm> Model_Impl::zoneAirHeatBalanceAlgorithm() const {
      if (m_cachedZoneAirHeatBalanceAlgorithm) {
        return m_cachedZoneAirHeatBalanceAlgorithm;
      }

      boost::optional<ZoneAirHeatBalanceAlgorithm> result = this->model().getOptionalUniqueModelObject<ZoneAirHeatBalanceAlgorithm>();
      if (result) {
        m_cachedZoneAirHeatBalanceAlgorithm = result;
        result->getImpl<ZoneAirHeatBalanceAlgorithm_Impl>()
          ->ZoneAirHeatBalanceAlgorithm_Impl::onRemoveFromWorkspace.connect<Model_Impl, &Model_Impl::clearCachedZoneAirHeatBalanceAlgorithm>(
            const_cast<openstudio::model::detail::Model_Impl*>(this));
      }

      return m_cachedZoneAirHeatBalanceAlgorithm;
    }

    boost::optional<ZoneAirMassFlowConservation> Model_Impl::zoneAirMassFlowConservation() const {
      if (m_cachedZoneAirMassFlowConservation) {
        return m_cachedZoneAirMassFlowConservation;
      }

      boost::optional<ZoneAirMassFlowConservation> result = this->model().getOptionalUniqueModelObject<ZoneAirMassFlowConservation>();
      if (result) {
        m_cachedZoneAirMassFlowConservation = result;
        result->getImpl<ZoneAirMassFlowConservation_Impl>()
          ->ZoneAirMassFlowConservation_Impl::onRemoveFromWorkspace.connect<Model_Impl, &Model_Impl::clearCachedZoneAirMassFlowConservation>(
            const_cast<openstudio::model::detail::Model_Impl*>(this));
      }

      return m_cachedZoneAirMassFlowConservation;
    }

    boost::optional<ZoneCapacitanceMultiplierResearchSpecial> Model_Impl::zoneCapacitanceMultiplierResearchSpecial() const {
      if (m_cachedZoneCapacitanceMultiplierResearchSpecial) {
        return m_cachedZoneCapacitanceMultiplierResearchSpecial;
      }

      boost::optional<ZoneCapacitanceMultiplierResearchSpecial> result =
        this->model().getOptionalUniqueModelObject<ZoneCapacitanceMultiplierResearchSpecial>();
      if (result) {
        m_cachedZoneCapacitanceMultiplierResearchSpecial = result;
        result->getImpl<ZoneCapacitanceMultiplierResearchSpecial_Impl>()
          ->ZoneCapacitanceMultiplierResearchSpecial_Impl::onRemoveFromWorkspace
          .connect<Model_Impl, &Model_Impl::clearCachedZoneCapacitanceMultiplierResearchSpecial>(
            const_cast<openstudio::model::detail::Model_Impl*>(this));
      }

      return m_cachedZoneCapacitanceMultiplierResearchSpecial;
    }

    boost::optional<ConvergenceLimits> Model_Impl::convergenceLimits() const {
      if (m_cachedConvergenceLimits) {
        return m_cachedConvergenceLimits;
      }

      boost::optional<ConvergenceLimits> result = this->model().getOptionalUniqueModelObject<ConvergenceLimits>();
      if (result) {
        m_cachedConvergenceLimits = result;
        result->getImpl<ConvergenceLimits_Impl>()
          ->ConvergenceLimits_Impl::onRemoveFromWorkspace.connect<Model_Impl, &Model_Impl::clearCachedConvergenceLimits>(
            const_cast<openstudio::model::detail::Model_Impl*>(this));
      }

      return m_cachedConvergenceLimits;
    }

    boost::optional<ShadowCalculation> Model_Impl::shadowCalculation() const {
      if (m_cachedShadowCalculation) {
        return m_cachedShadowCalculation;
      }

      boost::optional<ShadowCalculation> result = this->model().getOptionalUniqueModelObject<ShadowCalculation>();
      if (result) {
        m_cachedShadowCalculation = result;
        result->getImpl<ShadowCalculation_Impl>()
          ->ShadowCalculation_Impl::onRemoveFromWorkspace.connect<Model_Impl, &Model_Impl::clearCachedShadowCalculation>(
            const_cast<openstudio::model::detail::Model_Impl*>(this));
      }

      return m_cachedShadowCalculation;
    }

    boost::optional<Timestep> Model_Impl::timestep() const {
      if (m_cachedTimestep) {
        return m_cachedTimestep;
      }

      boost::optional<Timestep> result = this->model().getOptionalUniqueModelObject<Timestep>();
      if (result) {
        m_cachedTimestep = result;
        result->getImpl<Timestep_Impl>()->Timestep_Impl::onRemoveFromWorkspace.connect<Model_Impl, &Model_Impl::clearCachedTimestep>(
          const_cast<openstudio::model::detail::Model_Impl*>(this));
      }

      return m_cachedTimestep;
    }

    boost::optional<ClimateZones> Model_Impl::climateZones() const {
      if (m_cachedClimateZones) {
        return m_cachedClimateZones;
      }

      boost::optional<ClimateZones> result = this->model().getOptionalUniqueModelObject<ClimateZones>();
      if (result) {
        m_cachedClimateZones = result;
        result->getImpl<ClimateZones_Impl>()->ClimateZones_Impl::onRemoveFromWorkspace.connect<Model_Impl, &Model_Impl::clearCachedClimateZones>(
          const_cast<openstudio::model::detail::Model_Impl*>(this));
      }

      return m_cachedClimateZones;
    }

    boost::optional<EnvironmentalImpactFactors> Model_Impl::environmentalImpactFactors() const {
      if (m_cachedEnvironmentalImpactFactors) {
        return m_cachedEnvironmentalImpactFactors;
      }

      boost::optional<EnvironmentalImpactFactors> result = this->model().getOptionalUniqueModelObject<EnvironmentalImpactFactors>();
      if (result) {
        m_cachedEnvironmentalImpactFactors = result;
        result->getImpl<EnvironmentalImpactFactors_Impl>()
          ->EnvironmentalImpactFactors_Impl::onRemoveFromWorkspace.connect<Model_Impl, &Model_Impl::clearCachedEnvironmentalImpactFactors>(
            const_cast<openstudio::model::detail::Model_Impl*>(this));
      }

      return m_cachedEnvironmentalImpactFactors;
    }

    boost::optional<ExternalInterface> Model_Impl::externalInterface() const {
      if (m_cachedExternalInterface) {
        return m_cachedExternalInterface;
      }

      boost::optional<ExternalInterface> result = this->model().getOptionalUniqueModelObject<ExternalInterface>();
      if (result) {
        m_cachedExternalInterface = result;
        result->getImpl<ExternalInterface_Impl>()
          ->ExternalInterface_Impl::onRemoveFromWorkspace.connect<Model_Impl, &Model_Impl::clearCachedExternalInterface>(
            const_cast<openstudio::model::detail::Model_Impl*>(this));
      }

      return m_cachedExternalInterface;
    }

    boost::optional<PythonPluginSearchPaths> Model_Impl::pythonPluginSearchPaths() const {
      if (m_cachedPythonPluginSearchPaths) {
        return m_cachedPythonPluginSearchPaths;
      }

      boost::optional<PythonPluginSearchPaths> result = this->model().getOptionalUniqueModelObject<PythonPluginSearchPaths>();
      if (result) {
        m_cachedPythonPluginSearchPaths = result;
        result->getImpl<PythonPluginSearchPaths_Impl>()
          ->PythonPluginSearchPaths_Impl::onRemoveFromWorkspace.connect<Model_Impl, &Model_Impl::clearCachedPythonPluginSearchPaths>(
            const_cast<openstudio::model::detail::Model_Impl*>(this));
      }

      return m_cachedPythonPluginSearchPaths;
    }

    boost::optional<int> Model_Impl::calendarYear() const {
      if (!m_cachedYearDescription) {
        m_cachedYearDescription = this->model().getUniqueModelObject<YearDescription>();
      }
      OS_ASSERT(m_cachedYearDescription);
      return m_cachedYearDescription->calendarYear();
    }

    std::string Model_Impl::dayofWeekforStartDay() const {
      if (!m_cachedYearDescription) {
        m_cachedYearDescription = this->model().getUniqueModelObject<YearDescription>();
      }
      OS_ASSERT(m_cachedYearDescription);
      return m_cachedYearDescription->dayofWeekforStartDay();
    }

    bool Model_Impl::isDayofWeekforStartDayDefaulted() const {
      if (!m_cachedYearDescription) {
        m_cachedYearDescription = this->model().getUniqueModelObject<YearDescription>();
      }
      OS_ASSERT(m_cachedYearDescription);
      return m_cachedYearDescription->isDayofWeekforStartDayDefaulted();
    }

    bool Model_Impl::isLeapYear() const {
      if (!m_cachedYearDescription) {
        m_cachedYearDescription = this->model().getUniqueModelObject<YearDescription>();
      }
      OS_ASSERT(m_cachedYearDescription);
      return m_cachedYearDescription->isLeapYear();
    }

    bool Model_Impl::isIsLeapYearDefaulted() const {
      if (!m_cachedYearDescription) {
        m_cachedYearDescription = this->model().getUniqueModelObject<YearDescription>();
      }
      OS_ASSERT(m_cachedYearDescription);
      return m_cachedYearDescription->isIsLeapYearDefaulted();
    }

    bool Model_Impl::setCalendarYear(int calendarYear) {
      if (!m_cachedYearDescription) {
        m_cachedYearDescription = this->model().getUniqueModelObject<YearDescription>();
      }
      OS_ASSERT(m_cachedYearDescription);
      return m_cachedYearDescription->setCalendarYear(calendarYear);
    }

    void Model_Impl::resetCalendarYear() {
      if (!m_cachedYearDescription) {
        m_cachedYearDescription = this->model().getUniqueModelObject<YearDescription>();
      }
      OS_ASSERT(m_cachedYearDescription);
      m_cachedYearDescription->resetCalendarYear();
    }

    bool Model_Impl::setDayofWeekforStartDay(const std::string& dayofWeekforStartDay) {
      if (!m_cachedYearDescription) {
        m_cachedYearDescription = this->model().getUniqueModelObject<YearDescription>();
      }
      OS_ASSERT(m_cachedYearDescription);
      return m_cachedYearDescription->setDayofWeekforStartDay(dayofWeekforStartDay);
    }

    void Model_Impl::resetDayofWeekforStartDay() {
      if (!m_cachedYearDescription) {
        m_cachedYearDescription = this->model().getUniqueModelObject<YearDescription>();
      }
      OS_ASSERT(m_cachedYearDescription);
      m_cachedYearDescription->resetDayofWeekforStartDay();
    }

    bool Model_Impl::setIsLeapYear(bool isLeapYear) {
      if (!m_cachedYearDescription) {
        m_cachedYearDescription = this->model().getUniqueModelObject<YearDescription>();
      }
      OS_ASSERT(m_cachedYearDescription);
      return m_cachedYearDescription->setIsLeapYear(isLeapYear);
    }

    void Model_Impl::resetIsLeapYear() {
      if (!m_cachedYearDescription) {
        m_cachedYearDescription = this->model().getUniqueModelObject<YearDescription>();
      }
      OS_ASSERT(m_cachedYearDescription);
      m_cachedYearDescription->resetIsLeapYear();
    }

    int Model_Impl::assumedYear() {
      if (!m_cachedYearDescription) {
        m_cachedYearDescription = this->model().getUniqueModelObject<YearDescription>();
      }
      OS_ASSERT(m_cachedYearDescription);
      return m_cachedYearDescription->assumedYear();
    }

    openstudio::Date Model_Impl::makeDate(openstudio::MonthOfYear monthOfYear, unsigned dayOfMonth) {
      if (!m_cachedYearDescription) {
        m_cachedYearDescription = this->model().getUniqueModelObject<YearDescription>();
      }
      OS_ASSERT(m_cachedYearDescription);
      return m_cachedYearDescription->makeDate(monthOfYear, dayOfMonth);
    }

    openstudio::Date Model_Impl::makeDate(unsigned monthOfYear, unsigned dayOfMonth) {
      if (!m_cachedYearDescription) {
        m_cachedYearDescription = this->model().getUniqueModelObject<YearDescription>();
      }
      OS_ASSERT(m_cachedYearDescription);
      return m_cachedYearDescription->makeDate(monthOfYear, dayOfMonth);
    }

    openstudio::Date Model_Impl::makeDate(openstudio::NthDayOfWeekInMonth n, openstudio::DayOfWeek dayOfWeek, openstudio::MonthOfYear monthOfYear) {
      if (!m_cachedYearDescription) {
        m_cachedYearDescription = this->model().getUniqueModelObject<YearDescription>();
      }
      OS_ASSERT(m_cachedYearDescription);
      return m_cachedYearDescription->makeDate(n, dayOfWeek, monthOfYear);
    }

    openstudio::Date Model_Impl::makeDate(unsigned dayOfYear) {
      if (!m_cachedYearDescription) {
        m_cachedYearDescription = this->model().getUniqueModelObject<YearDescription>();
      }
      OS_ASSERT(m_cachedYearDescription);
      return m_cachedYearDescription->makeDate(dayOfYear);
    }

    Schedule Model_Impl::alwaysOffDiscreteSchedule() const {
      std::string alwaysOffName = this->alwaysOffDiscreteScheduleName();

      std::vector<ScheduleConstant> schedules = model().getConcreteModelObjects<ScheduleConstant>();

      for (const auto& schedule : schedules) {
        if (boost::optional<std::string> name = schedule.name()) {
          if (istringEqual(name.get(), alwaysOffName)) {
            // DLM: if a schedule matches the name but not these other properties we should reclaim the name or at least warn

            if (equal<double>(schedule.value(), 0.0)) {
              if (boost::optional<ScheduleTypeLimits> limits = schedule.scheduleTypeLimits()) {
                if (boost::optional<std::string> type = limits->numericType()) {
                  if (istringEqual(type.get(), "Discrete")) {
                    return schedule;
                  }
                }
              }
            }
          }
        }
      }

      ScheduleConstant schedule(model());

      schedule.setName(alwaysOffName);

      ScheduleTypeLimits limits(model());

      limits.setName("OnOff");

      limits.setNumericType("Discrete");

      limits.setUnitType("Availability");

      limits.setLowerLimitValue(0.0);

      limits.setUpperLimitValue(1.0);

      schedule.setScheduleTypeLimits(limits);

      schedule.setValue(0.0);

      return std::move(schedule);
    }

    std::string Model_Impl::alwaysOffDiscreteScheduleName() const {
      return "Always Off Discrete";
    }

    Schedule Model_Impl::alwaysOnDiscreteSchedule() const {
      std::string alwaysOnName = this->alwaysOnDiscreteScheduleName();

      std::vector<ScheduleConstant> schedules = model().getConcreteModelObjects<ScheduleConstant>();

      for (const auto& schedule : schedules) {
        if (boost::optional<std::string> name = schedule.name()) {
          if (istringEqual(name.get(), alwaysOnName)) {
            // DLM: if a schedule matches the name but not these other properties we should reclaim the name or at least warn

            if (equal<double>(schedule.value(), 1.0)) {
              if (boost::optional<ScheduleTypeLimits> limits = schedule.scheduleTypeLimits()) {
                if (boost::optional<std::string> type = limits->numericType()) {
                  if (istringEqual(type.get(), "Discrete")) {
                    return schedule;
                  }
                }
              }
            }
          }
        }
      }

      ScheduleConstant schedule(model());

      schedule.setName(alwaysOnName);

      ScheduleTypeLimits limits(model());

      limits.setName("OnOff");

      limits.setNumericType("Discrete");

      limits.setUnitType("Availability");

      limits.setLowerLimitValue(0.0);

      limits.setUpperLimitValue(1.0);

      schedule.setScheduleTypeLimits(limits);

      schedule.setValue(1.0);

      return std::move(schedule);
    }

    std::string Model_Impl::alwaysOnDiscreteScheduleName() const {
      return "Always On Discrete";
    }

    Schedule Model_Impl::alwaysOnContinuousSchedule() const {
      std::string alwaysOnName = this->alwaysOnContinuousScheduleName();

      std::vector<ScheduleConstant> schedules = model().getConcreteModelObjects<ScheduleConstant>();

      for (const auto& schedule : schedules) {
        if (boost::optional<std::string> name = schedule.name()) {
          if (istringEqual(name.get(), alwaysOnName)) {
            // DLM: if a schedule matches the name but not these other properties we should reclaim the name or at least warn

            if (equal<double>(schedule.value(), 1.0)) {
              if (boost::optional<ScheduleTypeLimits> limits = schedule.scheduleTypeLimits()) {
                if (boost::optional<std::string> type = limits->numericType()) {
                  if (istringEqual(type.get(), "Continuous")) {
                    return schedule;
                  }
                }
              }
            }
          }
        }
      }

      ScheduleConstant schedule(model());

      schedule.setName(alwaysOnName);

      ScheduleTypeLimits limits(model());

      limits.setName("Fractional");

      limits.setNumericType("Continuous");

      limits.setUnitType("");

      limits.setLowerLimitValue(0.0);

      limits.setUpperLimitValue(1.0);

      schedule.setScheduleTypeLimits(limits);

      schedule.setValue(1.0);

      return std::move(schedule);
    }

    std::string Model_Impl::alwaysOnContinuousScheduleName() const {
      return "Always On Continuous";
    }

    SpaceType Model_Impl::plenumSpaceType() const {
      std::string plenumSpaceTypeName = this->plenumSpaceTypeName();

      std::vector<SpaceType> spaceTypes = model().getConcreteModelObjects<SpaceType>();

      for (const auto& spaceType : spaceTypes) {
        if (boost::optional<std::string> name = spaceType.name()) {
          if (istringEqual(name.get(), plenumSpaceTypeName)) {
            return spaceType;
          }
        }
      }

      SpaceType spaceType(model());

      spaceType.setName(plenumSpaceTypeName);

      return spaceType;
    }

    std::string Model_Impl::plenumSpaceTypeName() const {
      return "Plenum Space Type";
    }

    Node Model_Impl::outdoorAirNode() const {
      std::string outdoorAirNodeName("Model Outdoor Air Node");

      std::vector<Node> nodes = model().getConcreteModelObjects<Node>();

      // Search for a node with the right name and not connected to any PlantLoop or AirLoopHVAC
      for (const auto& node : nodes) {
        if (boost::optional<std::string> name = node.name()) {
          if (istringEqual(name.get(), outdoorAirNodeName)) {
            if (!node.plantLoop() && !node.airLoopHVAC()) {
              return node;
            }
          }
        }
      }

      // Otherwise, create it
      Node node(model());
      node.setName(outdoorAirNodeName);

      return node;
    }

    WorkflowJSON Model_Impl::workflowJSON() const {
      return m_workflowJSON;
    }

    /// get the sql file
    boost::optional<openstudio::SqlFile> Model_Impl::sqlFile() const {
      if (m_sqlFile) {
        return boost::optional<openstudio::SqlFile>(*m_sqlFile);
      } else {
        return {};
      }
    }

    bool Model_Impl::setWorkflowJSON(const openstudio::WorkflowJSON& workflowJSON) {
      m_workflowJSON = workflowJSON;
      return true;
    }

    void Model_Impl::resetWorkflowJSON() {
      m_workflowJSON = WorkflowJSON();
    }

    /// set the sql file
    bool Model_Impl::setSqlFile(const openstudio::SqlFile& sqlFile) {
      bool result = true;
      m_sqlFile = std::shared_ptr<openstudio::SqlFile>(new openstudio::SqlFile(sqlFile));
      return result;
    }

    /// reset the sql file
    bool Model_Impl::resetSqlFile() {
      bool result = true;
      m_sqlFile.reset();
      return result;
    }

    boost::optional<ComponentData> Model_Impl::insertComponent(const Component& component) {
      ComponentData newComponentData = component.componentData();
      for (const ComponentWatcher& cw : m_componentWatchers) {
        ComponentData candidate = cw.componentData();
        if (candidate.uuid() == newComponentData.uuid()) {
          if (candidate.versionUUID() != newComponentData.versionUUID()) {
            LOG(Warn, "Component '" << newComponentData.nameString() << "' to be inserted into Model is "
                                    << "already in Model as determined by UUID comparison, but has a different version. "
                                    << "Returning the version already in Model; if you would like to use both versions, "
                                    << "please create a new UUID for one of the Components.");
          }
          return candidate;
        }
      }

      // Start by deleting existing Unique ModelObject, without touching children/resources by calling WorkspaceObject::remove
      for (const WorkspaceObject& componentObject : component.objects()) {
        auto compIddObject = componentObject.iddObject();
        if (compIddObject.properties().unique) {
          // Remove any existing objects (there should really be only one)
          for (WorkspaceObject& wo : model().getObjectsByType(compIddObject)) {
            LOG(Info, "Removing existing UniqueModelObject in the target model: " << wo.briefDescription());
            wo.remove();
          }
        }
      }

      WorkspaceObjectVector resultingObjects = model().addObjects(component.objects());
      if (resultingObjects.empty()) {
        return boost::none;
      }
      OS_ASSERT(resultingObjects.size() == component.numObjects());
      for (const WorkspaceObject& wo : resultingObjects) {
        OptionalComponentData ocd = wo.optionalCast<ComponentData>();
        if (ocd) {
          ComponentData componentDataObject = *ocd;
          mf_createComponentWatcher(componentDataObject);
          return componentDataObject;
        }
      }
      OS_ASSERT(false);
      return boost::none;
    }

    std::vector<openstudio::IdfObject> Model_Impl::purgeUnusedResourceObjects() {
      ResourceObjectVector resources = model().getModelObjects<ResourceObject>();
      IdfObjectVector removedObjects;
      for (ResourceObject& resource : resources) {
        // test for initialized first in case earlier .remove() got this one already
        if ((resource.initialized()) && (resource.nonResourceObjectUseCount(true) == 0)) {
          openstudio::detail::concat_helper(removedObjects, resource.remove());
        }
      }
      return removedObjects;
    }

    std::vector<openstudio::IdfObject> Model_Impl::purgeUnusedResourceObjects(IddObjectType iddObjectType) {
      IdfObjectVector removedObjects;
      for (const WorkspaceObject& workspaceObject : getObjectsByType(iddObjectType)) {
        boost::optional<ResourceObject> resource = workspaceObject.optionalCast<ResourceObject>();
        if (resource) {
          // test for initialized first in case earlier .remove() got this one already
          if ((resource->initialized()) && (resource->directUseCount(true) == 0)) {
            openstudio::detail::concat_helper(removedObjects, resource->remove());
          }
        }
      }
      return removedObjects;
    }

    void Model_Impl::connect(const Model& m, ModelObject sourceObject, unsigned sourcePort, ModelObject targetObject, unsigned targetPort) {
      disconnect(sourceObject, sourcePort);
      disconnect(targetObject, targetPort);

      Connection c(m);
      c.setSourceObject(sourceObject);
      c.setSourceObjectPort(sourcePort);
      c.setTargetObject(targetObject);
      c.setTargetObjectPort(targetPort);

      sourceObject.setPointer(sourcePort, c.handle());
      targetObject.setPointer(targetPort, c.handle());
    }

    void Model_Impl::disconnect(ModelObject object, unsigned port) {
      if (boost::optional<HVACComponent> hvacComponent = object.optionalCast<HVACComponent>()) {
        std::shared_ptr<HVACComponent_Impl> hvacComponentImpl;
        hvacComponentImpl = hvacComponent->getImpl<HVACComponent_Impl>();
        hvacComponentImpl->m_airLoopHVAC = boost::none;
        hvacComponentImpl->m_airLoopHVACOutdoorAirSystem = boost::none;
        hvacComponentImpl->m_plantLoop = boost::none;
      }

      if (boost::optional<WaterToWaterComponent> waterToWaterComponent = object.optionalCast<WaterToWaterComponent>()) {
        std::shared_ptr<WaterToWaterComponent_Impl> waterToWaterComponentImpl;
        waterToWaterComponentImpl = waterToWaterComponent->getImpl<WaterToWaterComponent_Impl>();
        waterToWaterComponentImpl->m_secondaryPlantLoop = boost::none;
        waterToWaterComponentImpl->m_tertiaryPlantLoop = boost::none;
      }

      Model m = model();
      OptionalModelObject optionalObject;
      OptionalUnsigned optionalPort;
      OptionalWorkspaceObject wo;

      // find a connection that exists at the given object and port
      if (boost::optional<Connection> connection = object.getModelObjectTarget<Connection>(port)) {
        boost::optional<ModelObject> sourceObject;
        boost::optional<ModelObject> targetObject;
        boost::optional<unsigned> sourcePort;
        boost::optional<unsigned> targetPort;

        sourceObject = connection->sourceObject();
        sourcePort = connection->sourceObjectPort();

        targetObject = connection->targetObject();
        targetPort = connection->targetObjectPort();

        // This resets the cache
        if (targetObject) {
          if (boost::optional<HVACComponent> hvacComponent = targetObject->optionalCast<HVACComponent>()) {
            std::shared_ptr<HVACComponent_Impl> hvacComponentImpl;
            hvacComponentImpl = hvacComponent->getImpl<HVACComponent_Impl>();
            hvacComponentImpl->m_airLoopHVAC = boost::none;
            hvacComponentImpl->m_airLoopHVACOutdoorAirSystem = boost::none;
            hvacComponentImpl->m_plantLoop = boost::none;
          }

          if (boost::optional<WaterToWaterComponent> waterToWaterComponent = targetObject->optionalCast<WaterToWaterComponent>()) {
            std::shared_ptr<WaterToWaterComponent_Impl> waterToWaterComponentImpl;
            waterToWaterComponentImpl = waterToWaterComponent->getImpl<WaterToWaterComponent_Impl>();
            waterToWaterComponentImpl->m_secondaryPlantLoop = boost::none;
            waterToWaterComponentImpl->m_tertiaryPlantLoop = boost::none;
          }
        }

        if (targetObject && targetPort) {
          if (boost::optional<PortList> portList = targetObject->optionalCast<PortList>()) {
            portList->getImpl<model::detail::PortList_Impl>()->removePort(targetPort.get());
          } else {
            targetObject->setString(targetPort.get(), "");
          }
        }

        // This resets the cache
        if (sourceObject) {
          if (boost::optional<HVACComponent> hvacComponent = sourceObject->optionalCast<HVACComponent>()) {
            std::shared_ptr<HVACComponent_Impl> hvacComponentImpl;
            hvacComponentImpl = hvacComponent->getImpl<HVACComponent_Impl>();
            hvacComponentImpl->m_airLoopHVAC = boost::none;
            hvacComponentImpl->m_airLoopHVACOutdoorAirSystem = boost::none;
            hvacComponentImpl->m_plantLoop = boost::none;
          }

          if (boost::optional<WaterToWaterComponent> waterToWaterComponent = sourceObject->optionalCast<WaterToWaterComponent>()) {
            std::shared_ptr<WaterToWaterComponent_Impl> waterToWaterComponentImpl;
            waterToWaterComponentImpl = waterToWaterComponent->getImpl<WaterToWaterComponent_Impl>();
            waterToWaterComponentImpl->m_secondaryPlantLoop = boost::none;
            waterToWaterComponentImpl->m_tertiaryPlantLoop = boost::none;
          }
        }

        if (sourceObject && sourcePort) {
          if (boost::optional<PortList> portList = sourceObject->optionalCast<PortList>()) {
            portList->getImpl<model::detail::PortList_Impl>()->removePort(sourcePort.get());
          } else {
            sourceObject->setString(sourcePort.get(), "");
          }
        }

        connection->remove();
      }
    }

    void Model_Impl::obsoleteComponentWatcher(const ComponentWatcher& watcher) {
      auto it = std::find(m_componentWatchers.begin(), m_componentWatchers.end(), watcher);
      OS_ASSERT(it != m_componentWatchers.end());
      m_componentWatchers.erase(it);
    }

    void Model_Impl::reportInitialModelObjects() {
      for (const WorkspaceObject& workspaceObject : this->objects()) {
        this->initialModelObject.nano_emit(workspaceObject.getImpl<detail::ModelObject_Impl>().get(), workspaceObject.iddObject().type(),
                                           workspaceObject.handle());
      }
      this->initialReportComplete.nano_emit();
    }

    void Model_Impl::mf_createComponentWatcher(ComponentData& componentData) {
      try {
        ComponentWatcher watcher(componentData);
        watcher.getImpl()->ComponentWatcher_Impl::obsolete.connect<Model_Impl, &Model_Impl::obsoleteComponentWatcher>(this);  // #HASHTAG Problem?
        m_componentWatchers.push_back(watcher);
      } catch (...) {
        componentData.remove();
        return;
      }
    }

    void Model_Impl::clearCachedData() {
      Handle dummy;
      clearCachedBuilding(dummy);
      clearCachedFoundationKivaSettings(dummy);
      clearCachedOutputControlFiles(dummy);
      clearCachedOutputControlReportingTolerances(dummy);
      clearCachedOutputControlTableStyle(dummy);
      clearCachedOutputControlTimestamp(dummy);
      clearCachedOutputDiagnostics(dummy);
      clearCachedOutputDebuggingData(dummy);
      clearCachedOutputJSON(dummy);
      clearCachedOutputSQLite(dummy);
      clearCachedOutputEnergyManagementSystem(dummy);
      clearCachedOutputTableSummaryReports(dummy);
      clearCachedOutputSchedules(dummy);
      clearCachedOutputConstructions(dummy);
      clearCachedPerformancePrecisionTradeoffs(dummy);
      clearCachedLifeCycleCostParameters(dummy);
      clearCachedSizingParameters(dummy);
      clearCachedRadianceParameters(dummy);
      clearCachedRunPeriod(dummy);
      clearCachedRunPeriodControlDaylightSavingTime(dummy);
      clearCachedYearDescription(dummy);
      clearCachedSite(dummy);
      clearCachedSiteGroundReflectance(dummy);
      clearCachedSiteWaterMainsTemperature(dummy);
      clearCachedSiteGroundTemperatureBuildingSurface(dummy);
      clearCachedSiteGroundTemperatureFCfactorMethod(dummy);
      clearCachedSiteGroundTemperatureDeep(dummy);
      clearCachedSiteGroundTemperatureShallow(dummy);
      clearCachedFacility(dummy);
      clearCachedWeatherFile(dummy);
      clearCachedSimulationControl(dummy);
      clearCachedLightingSimulationControl(dummy);
      clearCachedAirflowNetworkSimulationControl(dummy);
      clearCachedInsideSurfaceConvectionAlgorithm(dummy);
      clearCachedOutsideSurfaceConvectionAlgorithm(dummy);
      clearCachedHeatBalanceAlgorithm(dummy);
      clearCachedZoneAirContaminantBalance(dummy);
      clearCachedZoneAirHeatBalanceAlgorithm(dummy);
      clearCachedZoneAirMassFlowConservation(dummy);
      clearCachedZoneCapacitanceMultiplierResearchSpecial(dummy);
      clearCachedConvergenceLimits(dummy);
      clearCachedShadowCalculation(dummy);
      clearCachedTimestep(dummy);
      clearCachedClimateZones(dummy);
      clearCachedEnvironmentalImpactFactors(dummy);
      clearCachedExternalInterface(dummy);
      clearCachedPythonPluginSearchPaths(dummy);
    }

    void Model_Impl::clearCachedBuilding(const Handle&) {
      m_cachedBuilding.reset();
    }

    void Model_Impl::clearCachedFoundationKivaSettings(const Handle&) {
      m_cachedFoundationKivaSettings.reset();
    }

    void Model_Impl::clearCachedOutputControlFiles(const Handle&) {
      m_cachedOutputControlFiles.reset();
    }

    void Model_Impl::clearCachedOutputControlReportingTolerances(const Handle&) {
      m_cachedOutputControlReportingTolerances.reset();
    }

    void Model_Impl::clearCachedOutputControlTableStyle(const Handle&) {
      m_cachedOutputControlTableStyle.reset();
    }

    void Model_Impl::clearCachedOutputControlTimestamp(const Handle&) {
      m_cachedOutputControlTimestamp.reset();
    }

    void Model_Impl::clearCachedOutputDiagnostics(const Handle&) {
      m_cachedOutputDiagnostics.reset();
    }

    void Model_Impl::clearCachedOutputDebuggingData(const Handle&) {
      m_cachedOutputDebuggingData.reset();
    }

    void Model_Impl::clearCachedOutputJSON(const Handle&) {
      m_cachedOutputJSON.reset();
    }

    void Model_Impl::clearCachedOutputSQLite(const Handle&) {
      m_cachedOutputSQLite.reset();
    }

    void Model_Impl::clearCachedOutputEnergyManagementSystem(const Handle&) {
      m_cachedOutputEnergyManagementSystem.reset();
    }

    void Model_Impl::clearCachedOutputTableSummaryReports(const Handle&) {
      m_cachedOutputTableSummaryReports.reset();
    }

    void Model_Impl::clearCachedOutputSchedules(const Handle&) {
      m_cachedOutputSchedules.reset();
    }

    void Model_Impl::clearCachedOutputConstructions(const Handle&) {
      m_cachedOutputConstructions.reset();
    }

    void Model_Impl::clearCachedPerformancePrecisionTradeoffs(const Handle&) {
      m_cachedPerformancePrecisionTradeoffs.reset();
    }

    void Model_Impl::clearCachedLifeCycleCostParameters(const Handle& handle) {
      m_cachedLifeCycleCostParameters.reset();
    }

    void Model_Impl::clearCachedSizingParameters(const Handle&) {
      m_cachedSizingParameters.reset();
    }

    void Model_Impl::clearCachedRadianceParameters(const Handle&) {
      m_cachedRadianceParameters.reset();
    }

    void Model_Impl::clearCachedRunPeriod(const Handle& handle) {
      m_cachedRunPeriod.reset();
    }

    void Model_Impl::clearCachedRunPeriodControlDaylightSavingTime(const Handle&) {
      m_cachedRunPeriodControlDaylightSavingTime.reset();
    }

    void Model_Impl::clearCachedYearDescription(const Handle& handle) {
      m_cachedYearDescription.reset();
    }

    void Model_Impl::clearCachedSite(const Handle& handle) {
      m_cachedSite.reset();
    }

    void Model_Impl::clearCachedSiteGroundReflectance(const Handle&) {
      m_cachedSiteGroundReflectance.reset();
    }

    void Model_Impl::clearCachedSiteWaterMainsTemperature(const Handle& handle) {
      m_cachedSiteWaterMainsTemperature.reset();
    }

    void Model_Impl::clearCachedSiteGroundTemperatureBuildingSurface(const Handle& handle) {
      m_cachedSiteGroundTemperatureBuildingSurface.reset();
    }

    void Model_Impl::clearCachedSiteGroundTemperatureFCfactorMethod(const Handle&) {
      m_cachedSiteGroundTemperatureFCfactorMethod.reset();
    }

    void Model_Impl::clearCachedSiteGroundTemperatureDeep(const Handle& handle) {
      m_cachedSiteGroundTemperatureDeep.reset();
    }

    void Model_Impl::clearCachedSiteGroundTemperatureShallow(const Handle&) {
      m_cachedSiteGroundTemperatureShallow.reset();
    }

    void Model_Impl::clearCachedFacility(const Handle& handle) {
      m_cachedFacility.reset();
    }

    void Model_Impl::clearCachedWeatherFile(const Handle& handle) {
      m_cachedWeatherFile.reset();
    }

    void Model_Impl::clearCachedSimulationControl(const Handle& handle) {
      m_cachedSimulationControl.reset();
    }

    void Model_Impl::clearCachedLightingSimulationControl(const Handle&) {
      m_cachedLightingSimulationControl.reset();
    }

    void Model_Impl::clearCachedAirflowNetworkSimulationControl(const Handle&) {
      m_cachedAirflowNetworkSimulationControl.reset();
    }

    void Model_Impl::clearCachedInsideSurfaceConvectionAlgorithm(const Handle& handle) {
      m_cachedInsideSurfaceConvectionAlgorithm.reset();
    }

    void Model_Impl::clearCachedOutsideSurfaceConvectionAlgorithm(const Handle& handle) {
      m_cachedOutsideSurfaceConvectionAlgorithm.reset();
    }

    void Model_Impl::clearCachedHeatBalanceAlgorithm(const Handle& handle) {
      m_cachedHeatBalanceAlgorithm.reset();
    }

    void Model_Impl::clearCachedZoneAirContaminantBalance(const Handle& handle) {
      m_cachedZoneAirContaminantBalance.reset();
    }

    void Model_Impl::clearCachedZoneAirHeatBalanceAlgorithm(const Handle& handle) {
      m_cachedZoneAirHeatBalanceAlgorithm.reset();
    }

    void Model_Impl::clearCachedZoneAirMassFlowConservation(const Handle&) {
      m_cachedZoneAirMassFlowConservation.reset();
    }

    void Model_Impl::clearCachedZoneCapacitanceMultiplierResearchSpecial(const Handle&) {
      m_cachedZoneCapacitanceMultiplierResearchSpecial.reset();
    }

    void Model_Impl::clearCachedConvergenceLimits(const Handle& handle) {
      m_cachedConvergenceLimits.reset();
    }

    void Model_Impl::clearCachedShadowCalculation(const Handle& handle) {
      m_cachedShadowCalculation.reset();
    }

    void Model_Impl::clearCachedTimestep(const Handle&) {
      m_cachedTimestep.reset();
    }

    void Model_Impl::clearCachedClimateZones(const Handle&) {
      m_cachedClimateZones.reset();
    }

    void Model_Impl::clearCachedEnvironmentalImpactFactors(const Handle&) {
      m_cachedEnvironmentalImpactFactors.reset();
    }

    void Model_Impl::clearCachedExternalInterface(const Handle&) {
      m_cachedExternalInterface.reset();
    }

    void Model_Impl::clearCachedPythonPluginSearchPaths(const Handle&) {
      m_cachedPythonPluginSearchPaths.reset();
    }

    void Model_Impl::autosize() {
      for (auto& optModelObj : objects()) {
        if (auto modelObj = optModelObj.optionalCast<HVACComponent>()) {  // HVACComponent
          modelObj->autosize();
        } else if (auto modelObj = optModelObj.optionalCast<Loop>()) {  // Loop
          modelObj->autosize();
        } else if (auto modelObj = optModelObj.optionalCast<SizingZone>()) {  // SizingZone
          modelObj->autosize();
        } else if (auto modelObj = optModelObj.optionalCast<SizingSystem>()) {  // SizingSystem
          modelObj->autosize();
        } else if (auto modelObj = optModelObj.optionalCast<ControllerOutdoorAir>()) {  // ControllerOutdoorAir
          modelObj->autosize();
        } else if (auto modelObj = optModelObj.optionalCast<ChillerHeaterPerformanceElectricEIR>()) {  // ChillerHeaterPerformanceElectricEIR
          modelObj->autosize();
        } else if (auto modelObj = optModelObj.optionalCast<CoilCoolingDXCurveFitOperatingMode>()) {  // CoilCoolingDXCurveFitOperatingMode
          modelObj->autosize();
        } else if (auto modelObj = optModelObj.optionalCast<CoilCoolingDXCurveFitSpeed>()) {  // CoilCoolingDXCurveFitSpeed
          modelObj->autosize();
        } else if (auto modelObj = optModelObj.optionalCast<CoilCoolingDXMultiSpeedStageData>()) {  // CoilCoolingDXMultiSpeedStageData
          modelObj->autosize();
        } else if (auto modelObj = optModelObj.optionalCast<CoilPerformanceDXCooling>()) {  // CoilPerformanceDXCooling
          modelObj->autosize();
        } else if (auto modelObj = optModelObj.optionalCast<CoilHeatingDXMultiSpeedStageData>()) {  // CoilHeatingDXMultiSpeedStageData
          modelObj->autosize();
        } else if (auto modelObj = optModelObj.optionalCast<CoilHeatingGasMultiStageStageData>()) {  // CoilHeatingGasMultiStageStageData
          modelObj->autosize();
        } else if (auto modelObj = optModelObj.optionalCast<CoilHeatingElectricMultiStageStageData>()) {  // CoilHeatingElectricMultiStageStageData
          modelObj->autosize();
        } else if (auto modelObj =
                     optModelObj.optionalCast<
                       HeatExchangerDesiccantBalancedFlowPerformanceDataType1>()) {  // HeatExchangerDesiccantBalancedFlowPerformanceDataType1
          modelObj->autosize();
        }
      }
    }

    void Model_Impl::applySizingValues() {
      for (auto& optModelObj : objects()) {
        if (auto modelObj = optModelObj.optionalCast<HVACComponent>()) {  // HVACComponent
          modelObj->applySizingValues();
        } else if (auto modelObj = optModelObj.optionalCast<Loop>()) {  // Loop
          modelObj->applySizingValues();
        } else if (auto modelObj = optModelObj.optionalCast<SizingZone>()) {  // SizingZone
          modelObj->applySizingValues();
        } else if (auto modelObj = optModelObj.optionalCast<SizingSystem>()) {  // SizingSystem
          modelObj->applySizingValues();
        } else if (auto modelObj = optModelObj.optionalCast<ControllerOutdoorAir>()) {  // ControllerOutdoorAir
          modelObj->applySizingValues();
        } else if (auto modelObj = optModelObj.optionalCast<ChillerHeaterPerformanceElectricEIR>()) {  // ChillerHeaterPerformanceElectricEIR
          modelObj->applySizingValues();
        } else if (auto modelObj = optModelObj.optionalCast<CoilCoolingDXCurveFitOperatingMode>()) {  // CoilCoolingDXCurveFitOperatingMode
          modelObj->applySizingValues();
        } else if (auto modelObj = optModelObj.optionalCast<CoilCoolingDXCurveFitSpeed>()) {  // CoilCoolingDXCurveFitSpeed
          modelObj->applySizingValues();
        } else if (auto modelObj = optModelObj.optionalCast<CoilCoolingDXMultiSpeedStageData>()) {  // CoilCoolingDXMultiSpeedStageData
          modelObj->applySizingValues();
        } else if (auto modelObj = optModelObj.optionalCast<CoilPerformanceDXCooling>()) {  // CoilPerformanceDXCooling
          modelObj->applySizingValues();
        } else if (auto modelObj = optModelObj.optionalCast<CoilHeatingDXMultiSpeedStageData>()) {  // CoilHeatingDXMultiSpeedStageData
          modelObj->applySizingValues();
        } else if (auto modelObj = optModelObj.optionalCast<CoilHeatingGasMultiStageStageData>()) {  // CoilHeatingGasMultiStageStageData
          modelObj->applySizingValues();
        } else if (auto modelObj = optModelObj.optionalCast<CoilHeatingElectricMultiStageStageData>()) {  // CoilHeatingElectricMultiStageStageData
          modelObj->applySizingValues();
        } else if (auto modelObj =
                     optModelObj.optionalCast<
                       HeatExchangerDesiccantBalancedFlowPerformanceDataType1>()) {  // HeatExchangerDesiccantBalancedFlowPerformanceDataType1
          modelObj->applySizingValues();
        }
      }
    }

  }  // namespace detail

  Model::Model() : Workspace(std::shared_ptr<detail::Model_Impl>(new detail::Model_Impl())) {
    this->addVersionObject();
  }

  Model::Model(const openstudio::IdfFile& idfFile) : Workspace(std::shared_ptr<detail::Model_Impl>(new detail::Model_Impl(idfFile))) {
    // construct WorkspaceObject_ImplPtrs
    openstudio::detail::WorkspaceObject_ImplPtrVector objectImplPtrs;
    if (OptionalIdfObject vo = idfFile.versionObject()) {
      objectImplPtrs.push_back(getImpl<detail::Model_Impl>()->createObject(*vo, true));
    }
    for (const IdfObject& idfObject : idfFile.objects()) {
      objectImplPtrs.push_back(getImpl<detail::Model_Impl>()->createObject(idfObject, true));
      LOG(Trace, "idfObject: " << toString(idfObject.handle()));
      LOG(Trace, "objectImplPtr: " << toString(objectImplPtrs.back()->handle()));
    }
    // add Object_ImplPtrs to Workspace_Impl
    getImpl<detail::Model_Impl>()->addObjects(objectImplPtrs);
    // watch loaded components
    getImpl<detail::Model_Impl>()->createComponentWatchers();
  }

  Model::Model(const openstudio::Workspace& workspace)
    : Workspace(std::shared_ptr<detail::Model_Impl>(new detail::Model_Impl(*(workspace.getImpl<openstudio::detail::Workspace_Impl>()), true))) {
    // construct WorkspaceObject_ImplPtrs
    openstudio::detail::WorkspaceObject_ImplPtrVector newObjectImplPtrs;
    HandleMap oldNewHandleMap;
    if (OptionalWorkspaceObject vo = workspace.versionObject()) {
      newObjectImplPtrs.push_back(getImpl<detail::Model_Impl>()->createObject(vo->getImpl<openstudio::detail::WorkspaceObject_Impl>(), true));
    }
    for (const WorkspaceObject& object : workspace.getImpl<openstudio::detail::Workspace_Impl>()->objects()) {
      newObjectImplPtrs.push_back(getImpl<detail::Model_Impl>()->createObject(object.getImpl<openstudio::detail::WorkspaceObject_Impl>(), true));
    }
    // add Object_ImplPtrs to clone's Workspace_Impl
    getImpl<detail::Model_Impl>()->addClones(newObjectImplPtrs, oldNewHandleMap, true);
    // watch loaded components
    getImpl<detail::Model_Impl>()->createComponentWatchers();
  }

  boost::optional<Model> Model::load(const path& osmPath) {

    if (!openstudio::filesystem::is_regular_file(osmPath)) {
      LOG(Warn, "Path is not a valid file: " << osmPath);
      return boost::none;
    }
    openstudio::osversion::VersionTranslator vt;
    boost::optional<openstudio::model::Model> model_ = vt.loadModel(osmPath);
    if (!model_) {
      LOG(Warn, "Failed to load model at " << osmPath);
      return boost::none;
    }
    // Load the workflow.osw in the model's companion folder
    const openstudio::path workflowJSONPath = getCompanionFolder(osmPath) / toPath("workflow.osw");
    if (exists(workflowJSONPath)) {
      if (boost::optional<WorkflowJSON> workflowJSON_ = WorkflowJSON::load(workflowJSONPath)) {
        model_->setWorkflowJSON(*workflowJSON_);
      }
    }

    return model_;
  }

  boost::optional<Model> Model::load(const path& osmPath, const path& workflowJSONPath) {
    OptionalModel result = load(osmPath);
    if (result) {
      boost::optional<WorkflowJSON> workflowJSON = WorkflowJSON::load(workflowJSONPath);
      if (workflowJSON) {
        result->setWorkflowJSON(*workflowJSON);
      } else {
        result.reset();
      }
    }
    return result;
  }

  Model::Model(std::shared_ptr<detail::Model_Impl> impl) : Workspace(std::move(impl)) {}

  boost::optional<Building> Model::building() const {
    return getImpl<detail::Model_Impl>()->building();
  }

  boost::optional<FoundationKivaSettings> Model::foundationKivaSettings() const {
    return getImpl<detail::Model_Impl>()->foundationKivaSettings();
  }

  boost::optional<OutputControlFiles> Model::outputControlFiles() const {
    return getImpl<detail::Model_Impl>()->outputControlFiles();
  }

  boost::optional<OutputControlReportingTolerances> Model::outputControlReportingTolerances() const {
    return getImpl<detail::Model_Impl>()->outputControlReportingTolerances();
  }

  boost::optional<OutputControlTableStyle> Model::outputControlTableStyle() const {
    return getImpl<detail::Model_Impl>()->outputControlTableStyle();
  }

  boost::optional<OutputControlTimestamp> Model::outputControlTimestamp() const {
    return getImpl<detail::Model_Impl>()->outputControlTimestamp();
  }

  boost::optional<OutputDiagnostics> Model::outputDiagnostics() const {
    return getImpl<detail::Model_Impl>()->outputDiagnostics();
  }

  boost::optional<OutputDebuggingData> Model::outputDebuggingData() const {
    return getImpl<detail::Model_Impl>()->outputDebuggingData();
  }

  boost::optional<OutputJSON> Model::outputJSON() const {
    return getImpl<detail::Model_Impl>()->outputJSON();
  }

  boost::optional<OutputSQLite> Model::outputSQLite() const {
    return getImpl<detail::Model_Impl>()->outputSQLite();
  }

  boost::optional<OutputEnergyManagementSystem> Model::outputEnergyManagementSystem() const {
    return getImpl<detail::Model_Impl>()->outputEnergyManagementSystem();
  }

  boost::optional<OutputTableSummaryReports> Model::outputTableSummaryReports() const {
    return getImpl<detail::Model_Impl>()->outputTableSummaryReports();
  }

  boost::optional<OutputSchedules> Model::outputSchedules() const {
    return getImpl<detail::Model_Impl>()->outputSchedules();
  }

  boost::optional<OutputConstructions> Model::outputConstructions() const {
    return getImpl<detail::Model_Impl>()->outputConstructions();
  }

  boost::optional<PerformancePrecisionTradeoffs> Model::performancePrecisionTradeoffs() const {
    return getImpl<detail::Model_Impl>()->performancePrecisionTradeoffs();
  }

  boost::optional<LifeCycleCostParameters> Model::lifeCycleCostParameters() const {
    return getImpl<detail::Model_Impl>()->lifeCycleCostParameters();
  }

  boost::optional<SizingParameters> Model::sizingParameters() const {
    return getImpl<detail::Model_Impl>()->sizingParameters();
  }

  boost::optional<RadianceParameters> Model::radianceParameters() const {
    return getImpl<detail::Model_Impl>()->radianceParameters();
  }

  boost::optional<RunPeriod> Model::runPeriod() const {
    return getImpl<detail::Model_Impl>()->runPeriod();
  }

  boost::optional<RunPeriodControlDaylightSavingTime> Model::runPeriodControlDaylightSavingTime() const {
    return getImpl<detail::Model_Impl>()->runPeriodControlDaylightSavingTime();
  }

  boost::optional<YearDescription> Model::yearDescription() const {
    return getImpl<detail::Model_Impl>()->yearDescription();
  }

  boost::optional<Site> Model::site() const {
    return getImpl<detail::Model_Impl>()->site();
  }

  boost::optional<SiteGroundReflectance> Model::siteGroundReflectance() const {
    return getImpl<detail::Model_Impl>()->siteGroundReflectance();
  }

  boost::optional<SiteWaterMainsTemperature> Model::siteWaterMainsTemperature() const {
    return getImpl<detail::Model_Impl>()->siteWaterMainsTemperature();
  }

  boost::optional<SiteGroundTemperatureBuildingSurface> Model::siteGroundTemperatureBuildingSurface() const {
    return getImpl<detail::Model_Impl>()->siteGroundTemperatureBuildingSurface();
  }

  boost::optional<SiteGroundTemperatureFCfactorMethod> Model::siteGroundTemperatureFCfactorMethod() const {
    return getImpl<detail::Model_Impl>()->siteGroundTemperatureFCfactorMethod();
  }

  boost::optional<SiteGroundTemperatureDeep> Model::siteGroundTemperatureDeep() const {
    return getImpl<detail::Model_Impl>()->siteGroundTemperatureDeep();
  }

  boost::optional<SiteGroundTemperatureShallow> Model::siteGroundTemperatureShallow() const {
    return getImpl<detail::Model_Impl>()->siteGroundTemperatureShallow();
  }

  boost::optional<Facility> Model::facility() const {
    return getImpl<detail::Model_Impl>()->facility();
  }

  boost::optional<WeatherFile> Model::weatherFile() const {
    return getImpl<detail::Model_Impl>()->weatherFile();
  }

  boost::optional<SimulationControl> Model::simulationControl() const {
    return getImpl<detail::Model_Impl>()->simulationControl();
  }

  boost::optional<LightingSimulationControl> Model::lightingSimulationControl() const {
    return getImpl<detail::Model_Impl>()->lightingSimulationControl();
  }

  boost::optional<AirflowNetworkSimulationControl> Model::airflowNetworkSimulationControl() const {
    return getImpl<detail::Model_Impl>()->airflowNetworkSimulationControl();
  }

  boost::optional<InsideSurfaceConvectionAlgorithm> Model::insideSurfaceConvectionAlgorithm() const {
    return getImpl<detail::Model_Impl>()->insideSurfaceConvectionAlgorithm();
  }

  boost::optional<OutsideSurfaceConvectionAlgorithm> Model::outsideSurfaceConvectionAlgorithm() const {
    return getImpl<detail::Model_Impl>()->outsideSurfaceConvectionAlgorithm();
  }

  boost::optional<HeatBalanceAlgorithm> Model::heatBalanceAlgorithm() const {
    return getImpl<detail::Model_Impl>()->heatBalanceAlgorithm();
  }

  boost::optional<ZoneAirContaminantBalance> Model::zoneAirContaminantBalance() const {
    return getImpl<detail::Model_Impl>()->zoneAirContaminantBalance();
  }

  boost::optional<ZoneAirHeatBalanceAlgorithm> Model::zoneAirHeatBalanceAlgorithm() const {
    return getImpl<detail::Model_Impl>()->zoneAirHeatBalanceAlgorithm();
  }

  boost::optional<ZoneAirMassFlowConservation> Model::zoneAirMassFlowConservation() const {
    return getImpl<detail::Model_Impl>()->zoneAirMassFlowConservation();
  }

  boost::optional<ZoneCapacitanceMultiplierResearchSpecial> Model::zoneCapacitanceMultiplierResearchSpecial() const {
    return getImpl<detail::Model_Impl>()->zoneCapacitanceMultiplierResearchSpecial();
  }

  boost::optional<ConvergenceLimits> Model::convergenceLimits() const {
    return getImpl<detail::Model_Impl>()->convergenceLimits();
  }

  boost::optional<ShadowCalculation> Model::shadowCalculation() const {
    return getImpl<detail::Model_Impl>()->shadowCalculation();
  }

  boost::optional<Timestep> Model::timestep() const {
    return getImpl<detail::Model_Impl>()->timestep();
  }

  boost::optional<ClimateZones> Model::climateZones() const {
    return getImpl<detail::Model_Impl>()->climateZones();
  }

  boost::optional<EnvironmentalImpactFactors> Model::environmentalImpactFactors() const {
    return getImpl<detail::Model_Impl>()->environmentalImpactFactors();
  }

  boost::optional<ExternalInterface> Model::externalInterface() const {
    return getImpl<detail::Model_Impl>()->externalInterface();
  }

  boost::optional<PythonPluginSearchPaths> Model::pythonPluginSearchPaths() const {
    return getImpl<detail::Model_Impl>()->pythonPluginSearchPaths();
  }

  boost::optional<int> Model::calendarYear() const {
    return getImpl<detail::Model_Impl>()->calendarYear();
  }

  std::string Model::dayofWeekforStartDay() const {
    return getImpl<detail::Model_Impl>()->dayofWeekforStartDay();
  }

  bool Model::isDayofWeekforStartDayDefaulted() const {
    return getImpl<detail::Model_Impl>()->isDayofWeekforStartDayDefaulted();
  }

  bool Model::isLeapYear() const {
    return getImpl<detail::Model_Impl>()->isLeapYear();
  }

  bool Model::isIsLeapYearDefaulted() const {
    return getImpl<detail::Model_Impl>()->isIsLeapYearDefaulted();
  }

  bool Model::setCalendarYear(int calendarYear) {
    return getImpl<detail::Model_Impl>()->setCalendarYear(calendarYear);
  }

  void Model::resetCalendarYear() {
    getImpl<detail::Model_Impl>()->resetCalendarYear();
  }

  bool Model::setDayofWeekforStartDay(const std::string& dayofWeekforStartDay) {
    return getImpl<detail::Model_Impl>()->setDayofWeekforStartDay(dayofWeekforStartDay);
  }

  void Model::resetDayofWeekforStartDay() {
    getImpl<detail::Model_Impl>()->resetDayofWeekforStartDay();
  }

  bool Model::setIsLeapYear(bool isLeapYear) {
    return getImpl<detail::Model_Impl>()->setIsLeapYear(isLeapYear);
  }

  void Model::resetIsLeapYear() {
    getImpl<detail::Model_Impl>()->resetIsLeapYear();
  }

  int Model::assumedYear() {
    return getImpl<detail::Model_Impl>()->assumedYear();
  }

  openstudio::Date Model::makeDate(openstudio::MonthOfYear monthOfYear, unsigned dayOfMonth) {
    return getImpl<detail::Model_Impl>()->makeDate(monthOfYear, dayOfMonth);
  }

  openstudio::Date Model::makeDate(unsigned monthOfYear, unsigned dayOfMonth) {
    return getImpl<detail::Model_Impl>()->makeDate(monthOfYear, dayOfMonth);
  }

  openstudio::Date Model::makeDate(openstudio::NthDayOfWeekInMonth n, openstudio::DayOfWeek dayOfWeek, openstudio::MonthOfYear monthOfYear) {
    return getImpl<detail::Model_Impl>()->makeDate(n, dayOfWeek, monthOfYear);
  }

  openstudio::Date Model::makeDate(unsigned dayOfYear) {
    return getImpl<detail::Model_Impl>()->makeDate(dayOfYear);
  }

  Schedule Model::alwaysOffDiscreteSchedule() const {
    return getImpl<detail::Model_Impl>()->alwaysOffDiscreteSchedule();
  }

  std::string Model::alwaysOffDiscreteScheduleName() const {
    return getImpl<detail::Model_Impl>()->alwaysOffDiscreteScheduleName();
  }

  Schedule Model::alwaysOnDiscreteSchedule() const {
    return getImpl<detail::Model_Impl>()->alwaysOnDiscreteSchedule();
  }

  std::string Model::alwaysOnDiscreteScheduleName() const {
    return getImpl<detail::Model_Impl>()->alwaysOnDiscreteScheduleName();
  }

  Schedule Model::alwaysOnContinuousSchedule() const {
    return getImpl<detail::Model_Impl>()->alwaysOnContinuousSchedule();
  }

  std::string Model::alwaysOnContinuousScheduleName() const {
    return getImpl<detail::Model_Impl>()->alwaysOnContinuousScheduleName();
  }

  Node Model::outdoorAirNode() const {
    return getImpl<detail::Model_Impl>()->outdoorAirNode();
  }

  SpaceType Model::plenumSpaceType() const {
    return getImpl<detail::Model_Impl>()->plenumSpaceType();
  }

  std::string Model::plenumSpaceTypeName() const {
    return getImpl<detail::Model_Impl>()->plenumSpaceTypeName();
  }

  openstudio::WorkflowJSON Model::workflowJSON() const {
    return getImpl<detail::Model_Impl>()->workflowJSON();
  }

  bool Model::setWorkflowJSON(const openstudio::WorkflowJSON& setWorkflowJSON) {
    return getImpl<detail::Model_Impl>()->setWorkflowJSON(setWorkflowJSON);
  }

  void Model::resetWorkflowJSON() {
    return getImpl<detail::Model_Impl>()->resetWorkflowJSON();
  }

  openstudio::OptionalSqlFile Model::sqlFile() const {
    return getImpl<detail::Model_Impl>()->sqlFile();
  }

  bool Model::setSqlFile(const openstudio::SqlFile& sqlFile) {
    return getImpl<detail::Model_Impl>()->setSqlFile(sqlFile);
  }

  bool Model::resetSqlFile() {
    return getImpl<detail::Model_Impl>()->resetSqlFile();
  }

  bool Model::operator==(const Model& other) const {
    return (getImpl<detail::Model_Impl>() == other.getImpl<detail::Model_Impl>());
  }

  std::vector<ModelObject> Model::modelObjects(bool sorted) const {
    return castVector<ModelObject>(this->objects(sorted));
  }

  boost::optional<ComponentData> Model::insertComponent(const Component& component) {
    return getImpl<detail::Model_Impl>()->insertComponent(component);
  }

  std::vector<openstudio::IdfObject> Model::purgeUnusedResourceObjects() {
    return getImpl<detail::Model_Impl>()->purgeUnusedResourceObjects();
  }

  std::vector<openstudio::IdfObject> Model::purgeUnusedResourceObjects(IddObjectType iddObjectType) {
    return getImpl<detail::Model_Impl>()->purgeUnusedResourceObjects(iddObjectType);
  }

  void Model::addVersionObject() {
    getUniqueModelObject<Version>();
  }

  void Model::connect(ModelObject sourceObject, unsigned sourcePort, ModelObject targetObject, unsigned targetPort) const {
    getImpl<detail::Model_Impl>()->connect(*this, sourceObject, sourcePort, targetObject, targetPort);
  }

  void Model::disconnect(ModelObject object, unsigned port) {
    getImpl<detail::Model_Impl>()->disconnect(object, port);
  }

  detail::Model_Impl* Model::rawImpl() const {
    return getImpl<detail::Model_Impl>().get();
  }

  bool compareInputAndOutput(const ModelObject& object, const std::string& attributeName, double inputResult, double outputResult, double tol,
                             LogLevel logLevel) {
    if (!openstudio::equal(inputResult, outputResult, tol)) {
      LOG_FREE(logLevel, "openstudio.model.Model",
               "The " << attributeName << " values determined for " << object.briefDescription()
                      << " using input and output data differ by a (relative) error "
                      << "greater than " << tol << ". The value calculated from input data was " << inputResult
                      << ", whereas the value calculated from output data was " << outputResult << ".");
      return false;
    }
    return true;
  }

  Model exampleModel() {
    Model model;
    addExampleModelObjects(model);
    return model;
  }

  void addExampleSchedules(Model& model);

  void addExampleConstructions(Model& model);

  void addExampleModelObjects(Model& model) {
    std::vector<Surface> searchResults;

    // add Version
    auto version = model.getUniqueModelObject<Version>();

    // add SimulationControl
    auto simulationControl = model.getUniqueModelObject<SimulationControl>();
    simulationControl.setDoZoneSizingCalculation(true);
    simulationControl.setDoSystemSizingCalculation(true);
    simulationControl.setRunSimulationforSizingPeriods(false);
    simulationControl.setRunSimulationforWeatherFileRunPeriods(true);

    // add Timestep
    auto timestep = model.getUniqueModelObject<Timestep>();

    // add RunPeriod
    auto runPeriod = model.getUniqueModelObject<RunPeriod>();
    runPeriod.setBeginMonth(1);
    runPeriod.setBeginDayOfMonth(1);
    runPeriod.setEndMonth(12);
    runPeriod.setEndDayOfMonth(31);
    runPeriod.setUseWeatherFileHolidays(false);
    runPeriod.setUseWeatherFileDaylightSavings(false);
    runPeriod.setApplyWeekendHolidayRule(false);
    runPeriod.setUseWeatherFileRainInd(true);
    runPeriod.setUseWeatherFileSnowInd(true);
    runPeriod.setNumTimePeriodRepeats(1);

    // add SurfaceConvectionAlgorithmInside
    auto insideSurfaceConvectionAlgorithm = model.getUniqueModelObject<InsideSurfaceConvectionAlgorithm>();

    // add SurfaceConvectionAlgorithmOutside
    auto outsideSurfaceConvectionAlgorithm = model.getUniqueModelObject<OutsideSurfaceConvectionAlgorithm>();

    // add HeatBalanceAlgorithm
    auto heatBalanceAlgorithm = model.getUniqueModelObject<HeatBalanceAlgorithm>();

    // add ZoneAirHeatBalanceAlgorithm
    auto zoneAirHeatBalanceAlgorithm = model.getUniqueModelObject<ZoneAirHeatBalanceAlgorithm>();

    // add ConvergenceLimits
    auto convergenceLimits = model.getUniqueModelObject<ConvergenceLimits>();

    // add ShadowCalculation
    auto shadowCalculation = model.getUniqueModelObject<ShadowCalculation>();

    // add Site
    Site site = model.getUniqueModelObject<Site>();
    site.setLatitude(41.77);
    site.setLongitude(-87.75);
    site.setTimeZone(-6.0);
    site.setElevation(190.0);

    // add SiteGroundTemperatureBuildingSurface
    auto groundTemp = model.getUniqueModelObject<SiteGroundTemperatureBuildingSurface>();
    groundTemp.setJanuaryGroundTemperature(19.527);
    groundTemp.setFebruaryGroundTemperature(19.502);
    groundTemp.setMarchGroundTemperature(19.536);
    groundTemp.setAprilGroundTemperature(19.598);
    groundTemp.setMayGroundTemperature(20.002);
    groundTemp.setJuneGroundTemperature(21.640);
    groundTemp.setJulyGroundTemperature(22.225);
    groundTemp.setAugustGroundTemperature(22.375);
    groundTemp.setSeptemberGroundTemperature(21.449);
    groundTemp.setOctoberGroundTemperature(20.121);
    groundTemp.setNovemberGroundTemperature(19.802);
    groundTemp.setDecemberGroundTemperature(19.633);

    // add SiteGroundTemperatureDeep
    auto groundTempDeep = model.getUniqueModelObject<SiteGroundTemperatureDeep>();
    groundTempDeep.setJanuaryDeepGroundTemperature(19.527);
    groundTempDeep.setFebruaryDeepGroundTemperature(19.502);
    groundTempDeep.setMarchDeepGroundTemperature(19.536);
    groundTempDeep.setAprilDeepGroundTemperature(19.598);
    groundTempDeep.setMayDeepGroundTemperature(20.002);
    groundTempDeep.setJuneDeepGroundTemperature(21.640);
    groundTempDeep.setJulyDeepGroundTemperature(22.225);
    groundTempDeep.setAugustDeepGroundTemperature(22.375);
    groundTempDeep.setSeptemberDeepGroundTemperature(21.449);
    groundTempDeep.setOctoberDeepGroundTemperature(20.121);
    groundTempDeep.setNovemberDeepGroundTemperature(19.802);
    groundTempDeep.setDecemberDeepGroundTemperature(19.633);

    // add SiteWaterMainsTemperature
    auto waterTemp = model.getUniqueModelObject<SiteWaterMainsTemperature>();
    waterTemp.setAnnualAverageOutdoorAirTemperature(9.69);
    waterTemp.setMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures(28.10);

    // add DesignDay
    DesignDay designDay1(model);
    designDay1.setMaximumDryBulbTemperature(-20.6);
    designDay1.setDailyDryBulbTemperatureRange(0.0);
    designDay1.setWetBulbOrDewPointAtMaximumDryBulb(-20.6);
    designDay1.setBarometricPressure(99063.0);
    designDay1.setWindSpeed(4.9);
    designDay1.setWindDirection(270);
    designDay1.setSkyClearness(0.0);
    designDay1.setRainIndicator(false);
    designDay1.setSnowIndicator(false);
    designDay1.setDayOfMonth(21);
    designDay1.setMonth(1);
    designDay1.setDayType("WinterDesignDay");
    designDay1.setDaylightSavingTimeIndicator(false);
    designDay1.setHumidityConditionType("WetBulb");

    DesignDay designDay2(model);
    designDay2.setMaximumDryBulbTemperature(33.2);
    designDay2.setDailyDryBulbTemperatureRange(10.7);
    designDay2.setWetBulbOrDewPointAtMaximumDryBulb(23.8);
    designDay2.setBarometricPressure(99063.0);
    designDay2.setWindSpeed(5.3);
    designDay2.setWindDirection(230);
    designDay2.setSkyClearness(1.0);
    designDay2.setRainIndicator(false);
    designDay2.setSnowIndicator(false);
    designDay2.setDayOfMonth(21);
    designDay2.setMonth(7);
    designDay2.setDayType("SummerDesignDay");
    designDay2.setDaylightSavingTimeIndicator(false);
    designDay2.setHumidityConditionType("WetBulb");

    // add schedules
    addExampleSchedules(model);
    OS_ASSERT(!model.getConcreteModelObjects<DefaultScheduleSet>().empty());
    DefaultScheduleSet defaultScheduleSet = model.getConcreteModelObjects<DefaultScheduleSet>()[0];

    // add constructions
    addExampleConstructions(model);
    OS_ASSERT(!model.getConcreteModelObjects<DefaultConstructionSet>().empty());
    DefaultConstructionSet defaultConstructionSet = model.getConcreteModelObjects<DefaultConstructionSet>()[0];

    // add a space type
    SpaceType spaceType(model);

    // add some lights to the space type
    LightsDefinition lightsDefinition(model);
    lightsDefinition.setWattsperSpaceFloorArea(10.0);
    Lights lights(lightsDefinition);
    lights.setSpaceType(spaceType);

    // add some electric equipment to the space type
    ElectricEquipmentDefinition electricEquipmentDefinition(model);
    electricEquipmentDefinition.setWattsperSpaceFloorArea(5.0);
    ElectricEquipment electricEquipment(electricEquipmentDefinition);
    electricEquipment.setSpaceType(spaceType);

    // add some people to the space type
    PeopleDefinition peopleDefinition(model);
    peopleDefinition.setPeopleperSpaceFloorArea(0.05);
    People people(peopleDefinition);
    people.setSpaceType(spaceType);

    // create the facility
    auto facility = model.getUniqueModelObject<Facility>();

    // create the building
    auto building = model.getUniqueModelObject<Building>();
    building.setSpaceType(spaceType);
    building.setDefaultConstructionSet(defaultConstructionSet);
    building.setDefaultScheduleSet(defaultScheduleSet);

    // create a thermal zone
    ThermalZone thermalZone(model);

    ThermostatSetpointDualSetpoint thermostat(model);
    thermalZone.setThermostatSetpointDualSetpoint(thermostat);

    Schedule heatingSchedule = model.getModelObjectByName<Schedule>("Medium Office Heating Setpoint Schedule").get();
    Schedule coolingSchedule = model.getModelObjectByName<Schedule>("Medium Office Cooling Setpoint Schedule").get();

    thermostat.setHeatingSchedule(heatingSchedule);
    thermostat.setCoolingSchedule(coolingSchedule);

    // create a building story
    BuildingStory buildingStory(model);
    buildingStory.setNominalZCoordinate(0);
    buildingStory.setNominalFloortoFloorHeight(3);

    // create spaces from floor print
    // GG 10/26/21 - Change the spac egeneration to use fromFloorPrint instead of using Clone
    // don't know the underlying workins of Clone but each time this method was called in a different
    // session the order and type of surfaces was different, sometimes a surface was a roof and other
    // times a wall. To test for deterministic gbxml export I need to be sure that the model used is
    // idemntical every single time it this method is run. If these chanegs are aproblem I can refactor
    // so both Clone and fromFloorProint could be used
    //
    //            y (=North)
    //  Site      ▲
    //  Shading   │                  building height = 3m
    //   ║      20├────────┬────────┐
    //   ║        │        │        │
    //   ║        │        │        │
    //   ║        │ Space 3│ Space 4│
    //   ║        │        │        │
    //   ║      10├────────┼────────┤
    //   ║        │        │        │
    //   ║        │        │        ├
    //   ║        │ Space 1│ Space 2│◄─── window + space shading
    //   ║        │        │        ├
    //   ║        └──┬─┬───┴────────┴──────► x
    //           0    ▲    10       20
    //                └─ door+building shading

    std::vector<Point3d> floorPrint{
      {0, 0, 0},
      {0, 10, 0},
      {10, 10, 0},
      {10, 0, 0},
    };

    // make spaces
    boost::optional<Space> space1 = Space::fromFloorPrint(floorPrint, 3, model);
    OS_ASSERT(space1);
    space1->setThermalZone(thermalZone);
    space1->setBuildingStory(buildingStory);

    auto space2 = space1->clone(model).cast<Space>();
    space2.setXOrigin(10);

    auto space3 = space1->clone(model).cast<Space>();
    space3.setYOrigin(10);

    auto space4 = space1->clone(model).cast<Space>();
    space4.setXOrigin(10);
    space4.setYOrigin(10);

    // add a door to south wall of space1
    std::vector<Point3d> doorPoints{
      {2, 0, 2},
      {2, 0, 0},
      {4, 0, 0},
      {4, 0, 2},
    };

    // find south wall
    searchResults = space1->findSurfaces(180.0, 180.0, 90.0, 90.0);
    OS_ASSERT(!searchResults.empty());

    // add door
    SubSurface door(doorPoints, model);
    door.setSurface(searchResults[0]);

    // add a window to east wall of space2
    std::vector<Point3d> windowPoints{
      {10, 2, 2},
      {10, 2, 1},
      {10, 8, 1},
      {10, 8, 2},
    };

    // find east wall
    searchResults = space2.findSurfaces(90.0, 90.0, 90.0, 90.0);
    OS_ASSERT(!searchResults.empty());

    // add window
    SubSurface window(windowPoints, model);
    window.setSurface(searchResults[0]);

    // add overhang to the window
    bool test = window.addOverhangByProjectionFactor(0.5, 0.1).has_value();
    OS_ASSERT(test);

    // add daylighting control point to center of space2
    DaylightingControl daylightingControl(model);
    daylightingControl.setSpace(space2);
    daylightingControl.setPosition(Point3d(5, 5, 1.1));

    // hook daylighting control up to zone
    test = thermalZone.setPrimaryDaylightingControl(daylightingControl);
    OS_ASSERT(test);
    thermalZone.setFractionofZoneControlledbyPrimaryDaylightingControl(0.25);

    // add illuminance map to space2
    IlluminanceMap illuminanceMap(model);
    illuminanceMap.setSpace(space2);
    illuminanceMap.setOriginXCoordinate(1);
    illuminanceMap.setXLength(8);
    illuminanceMap.setOriginYCoordinate(1);
    illuminanceMap.setYLength(8);
    illuminanceMap.setOriginZCoordinate(1.1);

    // hook illuminanceMap up to zone
    test = thermalZone.setIlluminanceMap(illuminanceMap);
    OS_ASSERT(test);

    // add a glare sensor to center of space2
    GlareSensor glareSensor(model);
    glareSensor.setSpace(space2);
    glareSensor.setPosition(Point3d(5, 5, 1.1));

    // add a desk to space 2
    InteriorPartitionSurfaceGroup deskGroup(model);
    deskGroup.setSpace(space2);

    std::vector<Point3d> deskPoints{
      {5, 8, 1},
      {5, 6, 1},
      {8, 6, 1},
      {8, 8, 1},
    };
    InteriorPartitionSurface desk(deskPoints, model);
    desk.setInteriorPartitionSurfaceGroup(deskGroup);

    // add a printer to space4
    ElectricEquipmentDefinition printerDefinition(model);
    printerDefinition.setName("Printer Definition");
    printerDefinition.setDesignLevel(200.0);
    ElectricEquipment printer(printerDefinition);
    printer.setName("Printer");
    printer.setSpace(space4);

    // add a building shading device
    ShadingSurfaceGroup canopyGroup(model);
    canopyGroup.setShadingSurfaceType("Building");

    std::vector<Point3d> canopyPoints{
      {2, 0, 2},
      {2, -1, 2},
      {4, -1, 2},
      {4, 0, 2},
    };
    ShadingSurface canopy(canopyPoints, model);
    canopy.setShadingSurfaceGroup(canopyGroup);

    // add a neighboring building
    ShadingSurfaceGroup neighboringBuildingGroup(model);
    neighboringBuildingGroup.setShadingSurfaceType("Site");

    std::vector<Point3d> neighboringBuildingPoints{
      {-30, 0, 20},
      {-30, 0, 0},
      {-30, 20, 0},
      {-30, 20, 20},
    };
    ShadingSurface neighboringBuilding(neighboringBuildingPoints, model);
    neighboringBuilding.setShadingSurfaceGroup(neighboringBuildingGroup);

    // match surfaces
    std::vector<Space> spaces = model.getConcreteModelObjects<Space>();
    matchSurfaces(spaces);

    // Add an air loop
    ScheduleCompact alwaysOnSchedule(model);
    alwaysOnSchedule.setName("ALWAYS_ON");
    alwaysOnSchedule.setString(3, "Through: 12/31");
    alwaysOnSchedule.setString(4, "For: AllDays");
    alwaysOnSchedule.setString(5, "Until: 24:00");
    alwaysOnSchedule.setString(6, "1");

    FanConstantVolume fan(model, alwaysOnSchedule);
    fan.setName("Standard Fan");
    CoilHeatingGas coilHeatingGas(model, alwaysOnSchedule);

    CurveBiquadratic coolingCurveFofTemp(model);
    coolingCurveFofTemp.setCoefficient1Constant(0.42415);
    coolingCurveFofTemp.setCoefficient2x(0.04426);
    coolingCurveFofTemp.setCoefficient3xPOW2(-0.00042);
    coolingCurveFofTemp.setCoefficient4y(0.00333);
    coolingCurveFofTemp.setCoefficient5yPOW2(-0.00008);
    coolingCurveFofTemp.setCoefficient6xTIMESY(-0.00021);
    coolingCurveFofTemp.setMinimumValueofx(17.0);
    coolingCurveFofTemp.setMaximumValueofx(22.0);
    coolingCurveFofTemp.setMinimumValueofy(13.0);
    coolingCurveFofTemp.setMaximumValueofy(46.0);
    coolingCurveFofTemp.setMinimumCurveOutput(-1000);
    coolingCurveFofTemp.setMaximumCurveOutput(1000);

    CurveQuadratic coolingCurveFofFlow(model);
    coolingCurveFofFlow.setCoefficient1Constant(0.77136);
    coolingCurveFofFlow.setCoefficient2x(0.34053);
    coolingCurveFofFlow.setCoefficient3xPOW2(-0.11088);
    coolingCurveFofFlow.setMinimumValueofx(0.75918);
    coolingCurveFofFlow.setMaximumValueofx(1.13877);
    coolingCurveFofFlow.setMinimumCurveOutput(-1000);
    coolingCurveFofFlow.setMaximumCurveOutput(1000);

    CurveBiquadratic energyInputRatioFofTemp(model);
    energyInputRatioFofTemp.setCoefficient1Constant(1.23649);
    energyInputRatioFofTemp.setCoefficient2x(-0.02431);
    energyInputRatioFofTemp.setCoefficient3xPOW2(0.00057);
    energyInputRatioFofTemp.setCoefficient4y(-0.01434);
    energyInputRatioFofTemp.setCoefficient5yPOW2(0.00063);
    energyInputRatioFofTemp.setCoefficient6xTIMESY(-0.00038);
    energyInputRatioFofTemp.setMinimumValueofx(17.0);
    energyInputRatioFofTemp.setMaximumValueofx(22.0);
    energyInputRatioFofTemp.setMaximumValueofy(13.0);
    energyInputRatioFofTemp.setMaximumValueofy(46.0);
    energyInputRatioFofTemp.setMinimumCurveOutput(-1000);
    energyInputRatioFofTemp.setMaximumCurveOutput(1000);

    CurveQuadratic energyInputRatioFofFlow(model);
    energyInputRatioFofFlow.setCoefficient1Constant(1.20550);
    energyInputRatioFofFlow.setCoefficient2x(-0.32953);
    energyInputRatioFofFlow.setCoefficient3xPOW2(0.12308);
    energyInputRatioFofFlow.setMinimumValueofx(0.75918);
    energyInputRatioFofFlow.setMaximumValueofx(1.13877);
    energyInputRatioFofFlow.setMinimumCurveOutput(-1000);
    energyInputRatioFofFlow.setMaximumCurveOutput(1000);

    CurveQuadratic partLoadFraction(model);
    partLoadFraction.setCoefficient1Constant(0.77100);
    partLoadFraction.setCoefficient2x(0.22900);
    partLoadFraction.setCoefficient3xPOW2(0.0);
    partLoadFraction.setMinimumValueofx(0.0);
    partLoadFraction.setMaximumValueofx(1.0);
    partLoadFraction.setMinimumCurveOutput(0.71);
    partLoadFraction.setMaximumCurveOutput(1.0);

    CoilCoolingDXSingleSpeed coilCooling(model, alwaysOnSchedule, coolingCurveFofTemp, coolingCurveFofFlow, energyInputRatioFofTemp,
                                         energyInputRatioFofFlow, partLoadFraction);
    EvaporativeCoolerDirectResearchSpecial evaporativeCoolerDirectResearchSpecial(model, alwaysOnSchedule);

    AirTerminalSingleDuctConstantVolumeNoReheat airTerminalSingleDuctConstantVolumeNoReheat(model, alwaysOnSchedule);

    ControllerOutdoorAir controller(model);

    AirLoopHVACOutdoorAirSystem outdoorAirSystem(model, controller);

    AirLoopHVAC airLoopHVAC(model);

    airLoopHVAC.addBranchForZone(thermalZone, airTerminalSingleDuctConstantVolumeNoReheat);

    Node supplyInletNode = airLoopHVAC.supplyInletNode();
    Node supplyOutletNode = airLoopHVAC.supplyOutletNode();

    fan.addToNode(supplyInletNode);

    coilHeatingGas.addToNode(supplyInletNode);

    coilCooling.addToNode(supplyInletNode);

    outdoorAirSystem.addToNode(supplyInletNode);

    Node mixedAirNode = outdoorAirSystem.mixedAirModelObject()->cast<Node>();
    Node coolCoilOutletNode = coilCooling.outletModelObject()->cast<Node>();
    Node heatingCoilOutletNode = coilHeatingGas.outletModelObject()->cast<Node>();
    Node oaNode = outdoorAirSystem.outdoorAirModelObject()->cast<Node>();

    evaporativeCoolerDirectResearchSpecial.addToNode(oaNode);

    Node evapOutletNode = evaporativeCoolerDirectResearchSpecial.outletModelObject()->cast<Node>();

    SetpointManagerMixedAir setpointMMA1(model);
    SetpointManagerMixedAir setpointMMA2(model);
    SetpointManagerMixedAir setpointMMA3(model);
    SetpointManagerMixedAir setpointMMA4(model);
    SetpointManagerSingleZoneReheat setpointMSZR(model);

    setpointMSZR.addToNode(supplyOutletNode);
    setpointMMA1.addToNode(mixedAirNode);
    setpointMMA2.addToNode(coolCoilOutletNode);
    setpointMMA3.addToNode(heatingCoilOutletNode);
    setpointMMA4.addToNode(evapOutletNode);

    // add some example variables
    for (const std::string& variableName : thermalZone.outputVariableNames()) {
      OutputVariable(variableName, model);
      break;
    }

    // add some example variables
    for (const Surface& surface : model.getConcreteModelObjects<Surface>()) {
      for (const std::string& variableName : surface.outputVariableNames()) {
        OutputVariable(variableName, model);
        break;
      }
      break;
    }

    // add some meters
    OutputMeter electricityMeter(model);
    electricityMeter.setFuelType(FuelType(FuelType::Electricity));
    electricityMeter.setReportingFrequency(ReportingFrequency(ReportingFrequency::Hourly).valueName());
    electricityMeter.setInstallLocationType(InstallLocationType(InstallLocationType::Facility));

    OutputMeter gasMeter(model);
    gasMeter.setFuelType(FuelType(FuelType::Gas));
    gasMeter.setReportingFrequency(ReportingFrequency(ReportingFrequency::Hourly).valueName());
    gasMeter.setInstallLocationType(InstallLocationType(InstallLocationType::Facility));

    OutputMeter propaneMeter(model);
    propaneMeter.setFuelType(FuelType(FuelType::Propane));
    propaneMeter.setReportingFrequency(ReportingFrequency(ReportingFrequency::Hourly).valueName());
    propaneMeter.setInstallLocationType(InstallLocationType(InstallLocationType::Facility));
  }

  void addExampleSchedules(Model& model) {

    // nice to pre-define Temperature limits because default limits do not provide bounds
    ScheduleTypeLimits temperatureLimits(model);
    temperatureLimits.setName("Temperature");
    temperatureLimits.setLowerLimitValue(-60.0);
    temperatureLimits.setUpperLimitValue(200.0);
    temperatureLimits.setNumericType("CONTINUOUS");
    temperatureLimits.setUnitType("Temperature");

    DefaultScheduleSet defaultScheduleSet(model);
    defaultScheduleSet.setName("Default Schedules");

    ScheduleRuleset numberOfPeopleSchedule(model);
    numberOfPeopleSchedule.setName("Medium Office Number of People Schedule");
    numberOfPeopleSchedule.defaultDaySchedule().setName("Medium Office Number of People All Other Days Schedule");
    ScheduleDay numberOfPeopleSummerDesignDay(model);
    numberOfPeopleSummerDesignDay.setName("Medium Office Number of People Summer Design Day Schedule");
    numberOfPeopleSummerDesignDay.clearValues();
    numberOfPeopleSummerDesignDay.addValue(Time(0, 6, 0, 0), 0.0);
    numberOfPeopleSummerDesignDay.addValue(Time(0, 22, 0, 0), 1.0);
    numberOfPeopleSummerDesignDay.addValue(Time(0, 24, 0, 0), 0.05);
    numberOfPeopleSchedule.setSummerDesignDaySchedule(numberOfPeopleSummerDesignDay);
    ScheduleRule numberOfPeopleWeekdaysRule(numberOfPeopleSchedule);
    numberOfPeopleWeekdaysRule.setName("Medium Office Number of People Schedule Weekdays Rule");
    numberOfPeopleWeekdaysRule.setApplyMonday(true);
    numberOfPeopleWeekdaysRule.setApplyTuesday(true);
    numberOfPeopleWeekdaysRule.setApplyWednesday(true);
    numberOfPeopleWeekdaysRule.setApplyThursday(true);
    numberOfPeopleWeekdaysRule.setApplyFriday(true);
    numberOfPeopleWeekdaysRule.daySchedule().setName("Medium Office Number of People Weekdays Schedule");
    numberOfPeopleWeekdaysRule.daySchedule().clearValues();
    numberOfPeopleWeekdaysRule.daySchedule().addValue(Time(0, 6, 0, 0), 0.0);
    numberOfPeopleWeekdaysRule.daySchedule().addValue(Time(0, 7, 0, 0), 0.1);
    numberOfPeopleWeekdaysRule.daySchedule().addValue(Time(0, 8, 0, 0), 0.2);
    numberOfPeopleWeekdaysRule.daySchedule().addValue(Time(0, 12, 0, 0), 0.95);
    numberOfPeopleWeekdaysRule.daySchedule().addValue(Time(0, 13, 0, 0), 0.5);
    numberOfPeopleWeekdaysRule.daySchedule().addValue(Time(0, 17, 0, 0), 0.95);
    numberOfPeopleWeekdaysRule.daySchedule().addValue(Time(0, 18, 0, 0), 0.7);
    numberOfPeopleWeekdaysRule.daySchedule().addValue(Time(0, 20, 0, 0), 0.4);
    numberOfPeopleWeekdaysRule.daySchedule().addValue(Time(0, 22, 0, 0), 0.1);
    numberOfPeopleWeekdaysRule.daySchedule().addValue(Time(0, 24, 0, 0), 0.05);
    ScheduleRule numberOfPeopleSaturdayRule(numberOfPeopleSchedule);
    numberOfPeopleSaturdayRule.setName("Medium Office Number of People Schedule Saturday Rule");
    numberOfPeopleSaturdayRule.setApplySaturday(true);
    numberOfPeopleSaturdayRule.daySchedule().setName("Medium Office Number of People Saturday Schedule");
    numberOfPeopleSaturdayRule.daySchedule().clearValues();
    numberOfPeopleSaturdayRule.daySchedule().addValue(Time(0, 6, 0, 0), 0.0);
    numberOfPeopleSaturdayRule.daySchedule().addValue(Time(0, 8, 0, 0), 0.1);
    numberOfPeopleSaturdayRule.daySchedule().addValue(Time(0, 14, 0, 0), 0.5);
    numberOfPeopleSaturdayRule.daySchedule().addValue(Time(0, 17, 0, 0), 0.1);
    numberOfPeopleSaturdayRule.daySchedule().addValue(Time(0, 24, 0, 0), 0.0);

    defaultScheduleSet.setNumberofPeopleSchedule(numberOfPeopleSchedule);

    ScheduleRuleset peopleActivityLevelSchedule(model);
    peopleActivityLevelSchedule.setName("Medium Office People Activity Level Schedule");
    peopleActivityLevelSchedule.defaultDaySchedule().setName("Medium Office People Activity Level All Days Schedule");
    peopleActivityLevelSchedule.defaultDaySchedule().clearValues();
    peopleActivityLevelSchedule.defaultDaySchedule().addValue(Time(0, 24, 0, 0), 120.0);

    defaultScheduleSet.setPeopleActivityLevelSchedule(peopleActivityLevelSchedule);

    ScheduleRuleset lightingSchedule(model);
    lightingSchedule.setName("Medium Office Lighting Schedule");
    lightingSchedule.defaultDaySchedule().setName("Medium Office Lighting All Other Days Schedule");
    lightingSchedule.defaultDaySchedule().clearValues();
    lightingSchedule.defaultDaySchedule().addValue(Time(0, 24, 0, 0), 0.05);
    ScheduleDay lightingSummerDesignDay(model);
    lightingSummerDesignDay.setName("Medium Office Lighting Summer Design Day Schedule");
    lightingSummerDesignDay.clearValues();
    lightingSummerDesignDay.addValue(Time(0, 6, 0, 0), 1.0);
    lightingSchedule.setSummerDesignDaySchedule(lightingSummerDesignDay);
    ScheduleDay lightingWinterDesignDay(model);
    lightingWinterDesignDay.setName("Medium Office Lighting Winter Design Day Schedule");
    lightingSchedule.setWinterDesignDaySchedule(lightingWinterDesignDay);
    ScheduleRule lightingWeekdaysRule(lightingSchedule);
    lightingWeekdaysRule.setName("Medium Office Lighting Schedule Weekdays Rule");
    lightingWeekdaysRule.setApplyMonday(true);
    lightingWeekdaysRule.setApplyTuesday(true);
    lightingWeekdaysRule.setApplyWednesday(true);
    lightingWeekdaysRule.setApplyThursday(true);
    lightingWeekdaysRule.setApplyFriday(true);
    lightingWeekdaysRule.daySchedule().setName("Medium Office Lighting Weekdays Schedule");
    lightingWeekdaysRule.daySchedule().clearValues();
    lightingWeekdaysRule.daySchedule().addValue(Time(0, 5, 0, 0), 0.05);
    lightingWeekdaysRule.daySchedule().addValue(Time(0, 7, 0, 0), 0.1);
    lightingWeekdaysRule.daySchedule().addValue(Time(0, 8, 0, 0), 0.3);
    lightingWeekdaysRule.daySchedule().addValue(Time(0, 17, 0, 0), 0.9);
    lightingWeekdaysRule.daySchedule().addValue(Time(0, 18, 0, 0), 0.7);
    lightingWeekdaysRule.daySchedule().addValue(Time(0, 20, 0, 0), 0.5);
    lightingWeekdaysRule.daySchedule().addValue(Time(0, 22, 0, 0), 0.3);
    lightingWeekdaysRule.daySchedule().addValue(Time(0, 23, 0, 0), 0.1);
    lightingWeekdaysRule.daySchedule().addValue(Time(0, 24, 0, 0), 0.05);
    ScheduleRule lightingSaturdayRule(lightingSchedule);
    lightingSaturdayRule.setName("Medium Office Lighting Schedule Saturday Rule");
    lightingSaturdayRule.setApplySaturday(true);
    lightingSaturdayRule.daySchedule().setName("Medium Office Lighting Saturday Schedule");
    lightingSaturdayRule.daySchedule().clearValues();
    lightingSaturdayRule.daySchedule().addValue(Time(0, 6, 0, 0), 0.05);
    lightingSaturdayRule.daySchedule().addValue(Time(0, 8, 0, 0), 0.1);
    lightingSaturdayRule.daySchedule().addValue(Time(0, 14, 0, 0), 0.5);
    lightingSaturdayRule.daySchedule().addValue(Time(0, 17, 0, 0), 0.15);
    lightingSaturdayRule.daySchedule().addValue(Time(0, 24, 0, 0), 0.05);

    defaultScheduleSet.setLightingSchedule(lightingSchedule);

    ScheduleRuleset electricEquipmentSchedule(model);
    electricEquipmentSchedule.setName("Medium Office Electric Equipment Schedule");
    electricEquipmentSchedule.defaultDaySchedule().setName("Medium Office Electric Equipment All Other Days Schedule");
    electricEquipmentSchedule.defaultDaySchedule().clearValues();
    electricEquipmentSchedule.defaultDaySchedule().addValue(Time(0, 24, 0, 0), 0.3);
    ScheduleDay electricEquipmentSummerDesignDay(model);
    electricEquipmentSummerDesignDay.setName("Medium Office Electric Equipment Summer Design Day Schedule");
    electricEquipmentSummerDesignDay.clearValues();
    electricEquipmentSummerDesignDay.addValue(Time(0, 6, 0, 0), 1.0);
    electricEquipmentSchedule.setSummerDesignDaySchedule(electricEquipmentSummerDesignDay);
    ScheduleDay electricEquipmentWinterDesignDay(model);
    electricEquipmentWinterDesignDay.setName("Medium Office Electric Equipment Winter Design Day Schedule");
    electricEquipmentSchedule.setWinterDesignDaySchedule(electricEquipmentWinterDesignDay);
    ScheduleRule electricEquipmentWeekdaysRule(electricEquipmentSchedule);
    electricEquipmentWeekdaysRule.setName("Medium Office Electric Equipment Schedule Weekdays Rule");
    electricEquipmentWeekdaysRule.setApplyMonday(true);
    electricEquipmentWeekdaysRule.setApplyTuesday(true);
    electricEquipmentWeekdaysRule.setApplyWednesday(true);
    electricEquipmentWeekdaysRule.setApplyThursday(true);
    electricEquipmentWeekdaysRule.setApplyFriday(true);
    electricEquipmentWeekdaysRule.daySchedule().setName("Medium Office Electric Equipment Weekdays Schedule");
    electricEquipmentWeekdaysRule.daySchedule().clearValues();
    electricEquipmentWeekdaysRule.daySchedule().addValue(Time(0, 8, 0, 0), 0.4);
    electricEquipmentWeekdaysRule.daySchedule().addValue(Time(0, 12, 0, 0), 0.9);
    electricEquipmentWeekdaysRule.daySchedule().addValue(Time(0, 13, 0, 0), 0.8);
    electricEquipmentWeekdaysRule.daySchedule().addValue(Time(0, 17, 0, 0), 0.9);
    electricEquipmentWeekdaysRule.daySchedule().addValue(Time(0, 18, 0, 0), 0.8);
    electricEquipmentWeekdaysRule.daySchedule().addValue(Time(0, 20, 0, 0), 0.6);
    electricEquipmentWeekdaysRule.daySchedule().addValue(Time(0, 22, 0, 0), 0.5);
    electricEquipmentWeekdaysRule.daySchedule().addValue(Time(0, 24, 0, 0), 0.4);
    ScheduleRule electricEquipmentSaturdayRule(electricEquipmentSchedule);
    electricEquipmentSaturdayRule.setName("Medium Office Electric Equipment Schedule Saturday Rule");
    electricEquipmentSaturdayRule.setApplySaturday(true);
    electricEquipmentSaturdayRule.daySchedule().setName("Medium Office Electric Equipment Saturday Schedule");
    electricEquipmentSaturdayRule.daySchedule().clearValues();
    electricEquipmentSaturdayRule.daySchedule().addValue(Time(0, 6, 0, 0), 0.3);
    electricEquipmentSaturdayRule.daySchedule().addValue(Time(0, 8, 0, 0), 0.4);
    electricEquipmentSaturdayRule.daySchedule().addValue(Time(0, 14, 0, 0), 0.5);
    electricEquipmentSaturdayRule.daySchedule().addValue(Time(0, 17, 0, 0), 0.35);
    electricEquipmentSaturdayRule.daySchedule().addValue(Time(0, 24, 0, 0), 0.3);

    defaultScheduleSet.setElectricEquipmentSchedule(electricEquipmentSchedule);

    ScheduleRuleset gasEquipmentSchedule(model);
    gasEquipmentSchedule.setName("Medium Office Gas Equipment Schedule");
    gasEquipmentSchedule.defaultDaySchedule().setName("Medium Office Gas Equipment All Other Days Schedule");
    gasEquipmentSchedule.defaultDaySchedule().clearValues();
    gasEquipmentSchedule.defaultDaySchedule().addValue(Time(0, 24, 0, 0), 0.3);
    ScheduleDay gasEquipmentSummerDesignDay(model);
    gasEquipmentSummerDesignDay.setName("Medium Office Gas Equipment Summer Design Day Schedule");
    gasEquipmentSummerDesignDay.clearValues();
    gasEquipmentSummerDesignDay.addValue(Time(0, 6, 0, 0), 1.0);
    gasEquipmentSchedule.setSummerDesignDaySchedule(gasEquipmentSummerDesignDay);
    ScheduleDay gasEquipmentWinterDesignDay(model);
    gasEquipmentWinterDesignDay.setName("Medium Office Gas Equipment Winter Design Day Schedule");
    gasEquipmentSchedule.setWinterDesignDaySchedule(gasEquipmentWinterDesignDay);
    ScheduleRule gasEquipmentWeekdaysRule(gasEquipmentSchedule);
    gasEquipmentWeekdaysRule.setName("Medium Office Gas Equipment Schedule Weekdays Rule");
    gasEquipmentWeekdaysRule.setApplyMonday(true);
    gasEquipmentWeekdaysRule.setApplyTuesday(true);
    gasEquipmentWeekdaysRule.setApplyWednesday(true);
    gasEquipmentWeekdaysRule.setApplyThursday(true);
    gasEquipmentWeekdaysRule.setApplyFriday(true);
    gasEquipmentWeekdaysRule.daySchedule().setName("Medium Office Gas Equipment Weekdays Schedule");
    gasEquipmentWeekdaysRule.daySchedule().clearValues();
    gasEquipmentWeekdaysRule.daySchedule().addValue(Time(0, 8, 0, 0), 0.4);
    gasEquipmentWeekdaysRule.daySchedule().addValue(Time(0, 12, 0, 0), 0.9);
    gasEquipmentWeekdaysRule.daySchedule().addValue(Time(0, 13, 0, 0), 0.8);
    gasEquipmentWeekdaysRule.daySchedule().addValue(Time(0, 17, 0, 0), 0.9);
    gasEquipmentWeekdaysRule.daySchedule().addValue(Time(0, 18, 0, 0), 0.8);
    gasEquipmentWeekdaysRule.daySchedule().addValue(Time(0, 20, 0, 0), 0.6);
    gasEquipmentWeekdaysRule.daySchedule().addValue(Time(0, 22, 0, 0), 0.5);
    gasEquipmentWeekdaysRule.daySchedule().addValue(Time(0, 24, 0, 0), 0.4);
    ScheduleRule gasEquipmentSaturdayRule(gasEquipmentSchedule);
    gasEquipmentSaturdayRule.setName("Medium Office Gas Equipment Schedule Saturday Rule");
    gasEquipmentSaturdayRule.setApplySaturday(true);
    gasEquipmentSaturdayRule.daySchedule().setName("Medium Office Gas Equipment Saturday Schedule");
    gasEquipmentSaturdayRule.daySchedule().clearValues();
    gasEquipmentSaturdayRule.daySchedule().addValue(Time(0, 6, 0, 0), 0.3);
    gasEquipmentSaturdayRule.daySchedule().addValue(Time(0, 8, 0, 0), 0.4);
    gasEquipmentSaturdayRule.daySchedule().addValue(Time(0, 14, 0, 0), 0.5);
    gasEquipmentSaturdayRule.daySchedule().addValue(Time(0, 17, 0, 0), 0.35);
    gasEquipmentSaturdayRule.daySchedule().addValue(Time(0, 24, 0, 0), 0.3);

    defaultScheduleSet.setGasEquipmentSchedule(gasEquipmentSchedule);

    ScheduleRuleset hotWaterEquipmentSchedule(model);
    hotWaterEquipmentSchedule.setName("Medium Office Hot Water Equipment Schedule");
    hotWaterEquipmentSchedule.defaultDaySchedule().setName("Medium Office Hot Water Equipment Default Schedule");
    hotWaterEquipmentSchedule.defaultDaySchedule().clearValues();
    hotWaterEquipmentSchedule.defaultDaySchedule().addValue(Time(0, 5, 0, 0), 0.04);
    hotWaterEquipmentSchedule.defaultDaySchedule().addValue(Time(0, 6, 0, 0), 0.07);
    hotWaterEquipmentSchedule.defaultDaySchedule().addValue(Time(0, 11, 0, 0), 0.04);
    hotWaterEquipmentSchedule.defaultDaySchedule().addValue(Time(0, 13, 0, 0), 0.06);
    hotWaterEquipmentSchedule.defaultDaySchedule().addValue(Time(0, 14, 0, 0), 0.09);
    hotWaterEquipmentSchedule.defaultDaySchedule().addValue(Time(0, 15, 0, 0), 0.06);
    hotWaterEquipmentSchedule.defaultDaySchedule().addValue(Time(0, 21, 0, 0), 0.04);
    hotWaterEquipmentSchedule.defaultDaySchedule().addValue(Time(0, 22, 0, 0), 0.07);
    hotWaterEquipmentSchedule.defaultDaySchedule().addValue(Time(0, 24, 0, 0), 0.04);
    ScheduleDay hotWaterEquipmentSummerDesignDay(model);
    hotWaterEquipmentSummerDesignDay.setName("Medium Office Hot Water Equipment Summer Design Day Schedule");
    hotWaterEquipmentSummerDesignDay.clearValues();
    hotWaterEquipmentSummerDesignDay.addValue(Time(0, 5, 0, 0), 0.05);
    hotWaterEquipmentSummerDesignDay.addValue(Time(0, 6, 0, 0), 0.08);
    hotWaterEquipmentSummerDesignDay.addValue(Time(0, 7, 0, 0), 0.07);
    hotWaterEquipmentSummerDesignDay.addValue(Time(0, 8, 0, 0), 0.19);
    hotWaterEquipmentSummerDesignDay.addValue(Time(0, 9, 0, 0), 0.35);
    hotWaterEquipmentSummerDesignDay.addValue(Time(0, 10, 0, 0), 0.38);
    hotWaterEquipmentSummerDesignDay.addValue(Time(0, 11, 0, 0), 0.39);
    hotWaterEquipmentSummerDesignDay.addValue(Time(0, 12, 0, 0), 0.47);
    hotWaterEquipmentSummerDesignDay.addValue(Time(0, 13, 0, 0), 0.57);
    hotWaterEquipmentSummerDesignDay.addValue(Time(0, 14, 0, 0), 0.54);
    hotWaterEquipmentSummerDesignDay.addValue(Time(0, 15, 0, 0), 0.34);
    hotWaterEquipmentSummerDesignDay.addValue(Time(0, 16, 0, 0), 0.33);
    hotWaterEquipmentSummerDesignDay.addValue(Time(0, 17, 0, 0), 0.44);
    hotWaterEquipmentSummerDesignDay.addValue(Time(0, 18, 0, 0), 0.26);
    hotWaterEquipmentSummerDesignDay.addValue(Time(0, 19, 0, 0), 0.21);
    hotWaterEquipmentSummerDesignDay.addValue(Time(0, 20, 0, 0), 0.15);
    hotWaterEquipmentSummerDesignDay.addValue(Time(0, 21, 0, 0), 0.17);
    hotWaterEquipmentSummerDesignDay.addValue(Time(0, 22, 0, 0), 0.08);
    hotWaterEquipmentSummerDesignDay.addValue(Time(0, 24, 0, 0), 0.05);
    hotWaterEquipmentSchedule.setSummerDesignDaySchedule(hotWaterEquipmentSummerDesignDay);
    ScheduleRule hotWaterEquipmentWeekdaysRule(hotWaterEquipmentSchedule, hotWaterEquipmentSummerDesignDay);
    hotWaterEquipmentWeekdaysRule.setName("Medium Office Hot Water Schedule Weekdays Rule");
    hotWaterEquipmentWeekdaysRule.setApplyMonday(true);
    hotWaterEquipmentWeekdaysRule.setApplyTuesday(true);
    hotWaterEquipmentWeekdaysRule.setApplyWednesday(true);
    hotWaterEquipmentWeekdaysRule.setApplyThursday(true);
    hotWaterEquipmentWeekdaysRule.setApplyFriday(true);
    hotWaterEquipmentWeekdaysRule.daySchedule().setName("Medium Office Hot Water Equipment Weekdays Schedule");
    ScheduleDay hotWaterEquipmentWinterDesignDay(model);
    hotWaterEquipmentWinterDesignDay.setName("Medium Office Hot Water Equipment Winter Design Day Schedule");
    hotWaterEquipmentWinterDesignDay.clearValues();
    hotWaterEquipmentWinterDesignDay.addValue(Time(0, 5, 0, 0), 0.05);
    hotWaterEquipmentWinterDesignDay.addValue(Time(0, 6, 0, 0), 0.08);
    hotWaterEquipmentWinterDesignDay.addValue(Time(0, 7, 0, 0), 0.07);
    hotWaterEquipmentWinterDesignDay.addValue(Time(0, 8, 0, 0), 0.11);
    hotWaterEquipmentWinterDesignDay.addValue(Time(0, 9, 0, 0), 0.15);
    hotWaterEquipmentWinterDesignDay.addValue(Time(0, 10, 0, 0), 0.21);
    hotWaterEquipmentWinterDesignDay.addValue(Time(0, 11, 0, 0), 0.19);
    hotWaterEquipmentWinterDesignDay.addValue(Time(0, 12, 0, 0), 0.23);
    hotWaterEquipmentWinterDesignDay.addValue(Time(0, 13, 0, 0), 0.20);
    hotWaterEquipmentWinterDesignDay.addValue(Time(0, 14, 0, 0), 0.19);
    hotWaterEquipmentWinterDesignDay.addValue(Time(0, 15, 0, 0), 0.15);
    hotWaterEquipmentWinterDesignDay.addValue(Time(0, 16, 0, 0), 0.13);
    hotWaterEquipmentWinterDesignDay.addValue(Time(0, 17, 0, 0), 0.14);
    hotWaterEquipmentWinterDesignDay.addValue(Time(0, 21, 0, 0), 0.07);
    hotWaterEquipmentWinterDesignDay.addValue(Time(0, 22, 0, 0), 0.09);
    hotWaterEquipmentWinterDesignDay.addValue(Time(0, 24, 0, 0), 0.05);
    hotWaterEquipmentSchedule.setWinterDesignDaySchedule(hotWaterEquipmentWinterDesignDay);
    ScheduleRule hotWaterEquipmentSaturdayRule(hotWaterEquipmentSchedule, hotWaterEquipmentWinterDesignDay);
    hotWaterEquipmentSaturdayRule.setName("Medium Office Hot Water Schedule Saturday Rule");
    hotWaterEquipmentSaturdayRule.setApplySaturday(true);
    hotWaterEquipmentSaturdayRule.daySchedule().setName("Medium Office Hot Water Equipment Saturday Schedule");

    defaultScheduleSet.setHotWaterEquipmentSchedule(hotWaterEquipmentSchedule);

    ScheduleRuleset infiltrationSchedule(model);
    infiltrationSchedule.setName("Medium Office Infiltration Schedule");
    infiltrationSchedule.defaultDaySchedule().setName("Medium Office Infiltration Default Schedule");
    infiltrationSchedule.defaultDaySchedule().clearValues();
    infiltrationSchedule.defaultDaySchedule().addValue(Time(0, 24, 0, 0), 1.0);
    ScheduleDay infiltrationSummerDesignDay(model);
    infiltrationSummerDesignDay.setName("Medium Office Infiltration Summer Design Day Schedule");
    infiltrationSummerDesignDay.clearValues();
    infiltrationSummerDesignDay.addValue(Time(0, 6, 0, 0), 1.0);
    infiltrationSummerDesignDay.addValue(Time(0, 22, 0, 0), 0.25);
    infiltrationSummerDesignDay.addValue(Time(0, 24, 0, 0), 1.0);
    infiltrationSchedule.setSummerDesignDaySchedule(infiltrationSummerDesignDay);
    ScheduleRule infiltrationWeekdaysRule(infiltrationSchedule, infiltrationSummerDesignDay);
    infiltrationWeekdaysRule.setName("Medium Office Infiltration Schedule Weekdays Rule");
    infiltrationWeekdaysRule.setApplyMonday(true);
    infiltrationWeekdaysRule.setApplyTuesday(true);
    infiltrationWeekdaysRule.setApplyWednesday(true);
    infiltrationWeekdaysRule.setApplyThursday(true);
    infiltrationWeekdaysRule.setApplyFriday(true);
    infiltrationWeekdaysRule.daySchedule().setName("Medium Office Infiltration Weekdays Schedule");
    ScheduleDay infiltrationWinterDesignDay(model);
    infiltrationWinterDesignDay.setName("Medium Office Infiltration Winter Design Day Schedule");
    infiltrationWinterDesignDay.clearValues();
    infiltrationWinterDesignDay.addValue(Time(0, 6, 0, 0), 1.0);
    infiltrationWinterDesignDay.addValue(Time(0, 18, 0, 0), 0.25);
    infiltrationWinterDesignDay.addValue(Time(0, 24, 0, 0), 1.0);
    infiltrationSchedule.setWinterDesignDaySchedule(infiltrationWinterDesignDay);
    ScheduleRule infiltrationSaturdayRule(infiltrationSchedule, infiltrationWinterDesignDay);
    infiltrationSaturdayRule.setName("Medium Office Infiltration Schedule Saturday Rule");
    infiltrationSaturdayRule.setApplySaturday(true);
    infiltrationSaturdayRule.daySchedule().setName("Medium Office Infiltration Saturday Schedule");

    defaultScheduleSet.setInfiltrationSchedule(infiltrationSchedule);

    ScheduleRuleset coolingSetpointSchedule(model);
    coolingSetpointSchedule.setName("Medium Office Cooling Setpoint Schedule");
    coolingSetpointSchedule.setScheduleTypeLimits(temperatureLimits);
    coolingSetpointSchedule.defaultDaySchedule().setName("Medium Office Cooling Setpoint All Other Days Schedule");
    coolingSetpointSchedule.defaultDaySchedule().clearValues();
    coolingSetpointSchedule.defaultDaySchedule().addValue(Time(0, 24, 0, 0), 26.7);
    ScheduleDay coolingSetpointSummerDesignDay(model);
    coolingSetpointSummerDesignDay.setName("Medium Office Cooling Setpoint Summer Design Day Schedule");
    coolingSetpointSummerDesignDay.setScheduleTypeLimits(temperatureLimits);
    coolingSetpointSummerDesignDay.clearValues();
    coolingSetpointSummerDesignDay.addValue(Time(0, 6, 0, 0), 26.7);
    coolingSetpointSummerDesignDay.addValue(Time(0, 22, 0, 0), 24.0);
    coolingSetpointSummerDesignDay.addValue(Time(0, 24, 0, 0), 26.7);
    coolingSetpointSchedule.setSummerDesignDaySchedule(coolingSetpointSummerDesignDay);
    ScheduleRule coolingSetpointWeekdaysRule(coolingSetpointSchedule, coolingSetpointSummerDesignDay);
    coolingSetpointWeekdaysRule.setName("Medium Office Cooling Setpoint Schedule Weekdays Rule");
    coolingSetpointWeekdaysRule.setApplyMonday(true);
    coolingSetpointWeekdaysRule.setApplyTuesday(true);
    coolingSetpointWeekdaysRule.setApplyWednesday(true);
    coolingSetpointWeekdaysRule.setApplyThursday(true);
    coolingSetpointWeekdaysRule.setApplyFriday(true);
    coolingSetpointWeekdaysRule.daySchedule().setName("Medium Office Cooling Setpoint Weekdays Schedule");
    ScheduleRule coolingSetpointSaturdayRule(coolingSetpointSchedule);
    coolingSetpointSaturdayRule.setName("Medium Office Cooling Setpoint Schedule Saturday Rule");
    coolingSetpointSaturdayRule.setApplySaturday(true);
    coolingSetpointSaturdayRule.daySchedule().setName("Medium Office Cooling Setpoint Saturday Schedule");
    coolingSetpointSaturdayRule.daySchedule().clearValues();
    coolingSetpointSaturdayRule.daySchedule().addValue(Time(0, 6, 0, 0), 26.7);
    coolingSetpointSaturdayRule.daySchedule().addValue(Time(0, 18, 0, 0), 24.0);
    coolingSetpointSaturdayRule.daySchedule().addValue(Time(0, 24, 0, 0), 26.7);

    ScheduleRuleset heatingSetpointSchedule(model);
    heatingSetpointSchedule.setName("Medium Office Heating Setpoint Schedule");
    heatingSetpointSchedule.setScheduleTypeLimits(temperatureLimits);
    heatingSetpointSchedule.defaultDaySchedule().setName("Medium Office Heating Setpoint All Other Days Schedule");
    heatingSetpointSchedule.defaultDaySchedule().clearValues();
    heatingSetpointSchedule.defaultDaySchedule().addValue(Time(0, 24, 0, 0), 15.6);
    ScheduleDay heatingSetpointWinterDesignDay(model);
    heatingSetpointWinterDesignDay.setName("Medium Office Heating Setpoint Winter Design Day Schedule");
    heatingSetpointWinterDesignDay.setScheduleTypeLimits(temperatureLimits);
    heatingSetpointWinterDesignDay.clearValues();
    heatingSetpointWinterDesignDay.addValue(Time(0, 24, 0, 0), 21.0);
    heatingSetpointSchedule.setWinterDesignDaySchedule(heatingSetpointWinterDesignDay);
    ScheduleRule heatingSetpointWeekdaysRule(heatingSetpointSchedule);
    heatingSetpointWeekdaysRule.setName("Medium Office Heating Setpoint Schedule Weekdays Rule");
    heatingSetpointWeekdaysRule.setApplyMonday(true);
    heatingSetpointWeekdaysRule.setApplyTuesday(true);
    heatingSetpointWeekdaysRule.setApplyWednesday(true);
    heatingSetpointWeekdaysRule.setApplyThursday(true);
    heatingSetpointWeekdaysRule.setApplyFriday(true);
    heatingSetpointWeekdaysRule.daySchedule().setName("Medium Office Heating Setpoint Weekdays Schedule");
    heatingSetpointWeekdaysRule.daySchedule().clearValues();
    heatingSetpointWeekdaysRule.daySchedule().addValue(Time(0, 6, 0, 0), 15.6);
    heatingSetpointWeekdaysRule.daySchedule().addValue(Time(0, 22, 0, 0), 21.0);
    heatingSetpointWeekdaysRule.daySchedule().addValue(Time(0, 24, 0, 0), 15.6);
    ScheduleRule heatingSetpointSaturdayRule(heatingSetpointSchedule);
    heatingSetpointSaturdayRule.setName("Medium Office Heating Setpoint Schedule Saturday Rule");
    heatingSetpointSaturdayRule.setApplySaturday(true);
    heatingSetpointSaturdayRule.daySchedule().setName("Medium Office Heating Setpoint Saturday Schedule");
    heatingSetpointSaturdayRule.daySchedule().clearValues();
    heatingSetpointSaturdayRule.daySchedule().addValue(Time(0, 6, 0, 0), 15.6);
    heatingSetpointSaturdayRule.daySchedule().addValue(Time(0, 18, 0, 0), 21.0);
    heatingSetpointSaturdayRule.daySchedule().addValue(Time(0, 24, 0, 0), 15.6);
  }

  void addExampleConstructions(Model& model) {

    DefaultConstructionSet defaultConstructions(model);
    defaultConstructions.setName("Default Constructions");
    DefaultSurfaceConstructions exteriorSurfaceConstructions(model);
    exteriorSurfaceConstructions.setName("Exterior Surface Constructions");
    defaultConstructions.setDefaultExteriorSurfaceConstructions(exteriorSurfaceConstructions);
    DefaultSurfaceConstructions interiorSurfaceConstructions(model);
    interiorSurfaceConstructions.setName("Interior Surface Constructions");
    defaultConstructions.setDefaultInteriorSurfaceConstructions(interiorSurfaceConstructions);
    DefaultSurfaceConstructions groundContactSurfaceConstructions(model);
    groundContactSurfaceConstructions.setName("Ground Contact Surface Constructions");
    defaultConstructions.setDefaultGroundContactSurfaceConstructions(groundContactSurfaceConstructions);
    DefaultSubSurfaceConstructions exteriorSubSurfaceConstructions(model);
    exteriorSubSurfaceConstructions.setName("Exterior SubSurface Constructions");
    defaultConstructions.setDefaultExteriorSubSurfaceConstructions(exteriorSubSurfaceConstructions);
    DefaultSubSurfaceConstructions interiorSubSurfaceConstructions(model);
    interiorSubSurfaceConstructions.setName("Interior SubSurface Constructions");
    defaultConstructions.setDefaultInteriorSubSurfaceConstructions(interiorSubSurfaceConstructions);

    OpaqueMaterialVector opaqueMaterials;

    // Exterior Wall

    StandardOpaqueMaterial m01_100mm_brick(model);
    m01_100mm_brick.setName("M01 100mm brick");
    m01_100mm_brick.setRoughness("MediumRough");
    m01_100mm_brick.setThickness(0.1016);
    m01_100mm_brick.setThermalConductivity(0.89);
    m01_100mm_brick.setDensity(1920.0);
    m01_100mm_brick.setSpecificHeat(790.0);

    opaqueMaterials.push_back(m01_100mm_brick);

    StandardOpaqueMaterial m15_200mm_heavyweight_concrete(model);
    m15_200mm_heavyweight_concrete.setName("M15 200mm heavyweight concrete");
    m15_200mm_heavyweight_concrete.setRoughness("MediumRough");
    m15_200mm_heavyweight_concrete.setThickness(0.2032);
    m15_200mm_heavyweight_concrete.setThermalConductivity(1.95);
    m15_200mm_heavyweight_concrete.setDensity(2240.0);
    m15_200mm_heavyweight_concrete.setSpecificHeat(900.0);

    opaqueMaterials.push_back(m15_200mm_heavyweight_concrete);

    StandardOpaqueMaterial i02_50mm_insulation_board(model);
    i02_50mm_insulation_board.setName("I02 50mm insulation board");
    i02_50mm_insulation_board.setRoughness("MediumRough");
    i02_50mm_insulation_board.setThickness(0.0508);
    i02_50mm_insulation_board.setThermalConductivity(0.03);
    i02_50mm_insulation_board.setDensity(43.0);
    i02_50mm_insulation_board.setSpecificHeat(1210.0);

    opaqueMaterials.push_back(i02_50mm_insulation_board);

    AirGap f04_wall_air_space_resistance(model);
    f04_wall_air_space_resistance.setName("F04 Wall air space resistance");
    f04_wall_air_space_resistance.setThermalResistance(0.15);

    opaqueMaterials.push_back(f04_wall_air_space_resistance);

    StandardOpaqueMaterial g01a_19mm_gypsum_board(model);
    g01a_19mm_gypsum_board.setName("G01a 19mm gypsum board");
    g01a_19mm_gypsum_board.setRoughness("MediumSmooth");
    g01a_19mm_gypsum_board.setThickness(0.019);
    g01a_19mm_gypsum_board.setThermalConductivity(0.16);
    g01a_19mm_gypsum_board.setDensity(800.0);
    g01a_19mm_gypsum_board.setSpecificHeat(1090.0);

    opaqueMaterials.push_back(g01a_19mm_gypsum_board);

    Construction exteriorWall(opaqueMaterials);
    exteriorWall.setName("Exterior Wall");
    exteriorWall.setInsulation(i02_50mm_insulation_board);
    exteriorSurfaceConstructions.setWallConstruction(exteriorWall);
    opaqueMaterials.clear();

    // Exterior Roof

    StandardOpaqueMaterial m11_100mm_lightweight_concrete(model);
    m11_100mm_lightweight_concrete.setName("M11 100mm lightweight concrete");
    m11_100mm_lightweight_concrete.setRoughness("MediumRough");
    m11_100mm_lightweight_concrete.setThickness(0.1016);
    m11_100mm_lightweight_concrete.setThermalConductivity(0.53);
    m11_100mm_lightweight_concrete.setDensity(1280.0);
    m11_100mm_lightweight_concrete.setSpecificHeat(840.0);

    opaqueMaterials.push_back(m11_100mm_lightweight_concrete);

    AirGap f05_ceiling_air_space_resistance(model);
    f05_ceiling_air_space_resistance.setName("F05 Ceiling air space resistance");
    f05_ceiling_air_space_resistance.setThermalResistance(0.18);

    opaqueMaterials.push_back(f05_ceiling_air_space_resistance);

    StandardOpaqueMaterial f16_acoustic_tile(model);
    f16_acoustic_tile.setName("F16 Acoustic tile");
    f16_acoustic_tile.setRoughness("MediumSmooth");
    f16_acoustic_tile.setThickness(0.0191);
    f16_acoustic_tile.setThermalConductivity(0.06);
    f16_acoustic_tile.setDensity(368.0);
    f16_acoustic_tile.setSpecificHeat(590.0);

    opaqueMaterials.push_back(f16_acoustic_tile);

    Construction exteriorRoof(opaqueMaterials);
    exteriorRoof.setName("Exterior Roof");
    exteriorSurfaceConstructions.setRoofCeilingConstruction(exteriorRoof);
    opaqueMaterials.clear();

    // Interior Floor

    opaqueMaterials.push_back(f16_acoustic_tile);
    opaqueMaterials.push_back(f05_ceiling_air_space_resistance);
    opaqueMaterials.push_back(m11_100mm_lightweight_concrete);

    Construction interiorFloor(opaqueMaterials);
    interiorFloor.setName("Interior Floor");
    interiorSurfaceConstructions.setFloorConstruction(interiorFloor);
    opaqueMaterials.clear();

    // Air Wall

    ConstructionAirBoundary airWall(model);
    airWall.setName("Air Wall");
    interiorSurfaceConstructions.setWallConstruction(airWall);

    // Interior Ceiling

    opaqueMaterials.push_back(m11_100mm_lightweight_concrete);
    opaqueMaterials.push_back(f05_ceiling_air_space_resistance);
    opaqueMaterials.push_back(f16_acoustic_tile);

    Construction interiorCeiling(opaqueMaterials);
    interiorCeiling.setName("Interior Ceiling");
    interiorSurfaceConstructions.setRoofCeilingConstruction(interiorCeiling);
    opaqueMaterials.clear();

    // Slab

    StandardOpaqueMaterial mat_cc05_8_hw_concrete(model);
    mat_cc05_8_hw_concrete.setName("MAT-CC05 8 HW CONCRETE");
    mat_cc05_8_hw_concrete.setRoughness("Rough");
    mat_cc05_8_hw_concrete.setThickness(0.2032);
    mat_cc05_8_hw_concrete.setThermalConductivity(1.3110);
    mat_cc05_8_hw_concrete.setDensity(2240.0);
    mat_cc05_8_hw_concrete.setSpecificHeat(836.8);
    mat_cc05_8_hw_concrete.setThermalAbsorptance(0.9);
    mat_cc05_8_hw_concrete.setSolarAbsorptance(0.7);
    mat_cc05_8_hw_concrete.setVisibleAbsorptance(0.7);

    opaqueMaterials.push_back(mat_cc05_8_hw_concrete);

    MasslessOpaqueMaterial cp02_carpet_pad(model);
    cp02_carpet_pad.setName("CP02 CARPET PAD");
    cp02_carpet_pad.setRoughness("VeryRough");
    cp02_carpet_pad.setThermalResistance(0.2165);
    cp02_carpet_pad.setThermalAbsorptance(0.9);
    cp02_carpet_pad.setSolarAbsorptance(0.7);
    cp02_carpet_pad.setVisibleAbsorptance(0.8);

    opaqueMaterials.push_back(cp02_carpet_pad);

    Construction slab(opaqueMaterials);
    slab.setName("Slab");
    groundContactSurfaceConstructions.setFloorConstruction(slab);
    opaqueMaterials.clear();

    FenestrationMaterialVector fenestrationMaterials;

    // Exterior Window

    SimpleGlazing simple_glazing(model);
    simple_glazing.setName("Simple Glazing");
    simple_glazing.setUFactor(3.23646);
    simple_glazing.setSolarHeatGainCoefficient(0.39);
    simple_glazing.setVisibleTransmittance(0.6);

    StandardGlazing clear_3mm(model);
    clear_3mm.setName("Clear 3mm");
    clear_3mm.setOpticalDataType("SpectralAverage");
    clear_3mm.setThickness(0.003);
    clear_3mm.setSolarTransmittance(0.837);
    clear_3mm.setFrontSideSolarReflectanceatNormalIncidence(0.075);
    clear_3mm.setBackSideSolarReflectanceatNormalIncidence(0.075);
    clear_3mm.setVisibleTransmittance(0.898);
    clear_3mm.setFrontSideVisibleReflectanceatNormalIncidence(0.081);
    clear_3mm.setBackSideVisibleReflectanceatNormalIncidence(0.081);
    clear_3mm.setInfraredTransmittance(0.0);
    clear_3mm.setFrontSideInfraredHemisphericalEmissivity(0.084);
    clear_3mm.setBackSideInfraredHemisphericalEmissivity(0.084);
    clear_3mm.setThermalConductivity(0.9);

    Gas air_13mm(model);
    air_13mm.setName("Air 13mm");
    air_13mm.setGasType("Air");
    air_13mm.setThickness(0.0127);

    //fenestrationMaterials.push_back(clear_3mm);
    //fenestrationMaterials.push_back(air_13mm);
    //fenestrationMaterials.push_back(clear_3mm);

    // DLM: use simple glazing so we can know window properties without requiring E+ run
    fenestrationMaterials.push_back(simple_glazing);

    Construction exteriorWindow(fenestrationMaterials);
    exteriorWindow.setName("Exterior Window");
    exteriorSubSurfaceConstructions.setFixedWindowConstruction(exteriorWindow);
    exteriorSubSurfaceConstructions.setOperableWindowConstruction(exteriorWindow);
    exteriorSubSurfaceConstructions.setGlassDoorConstruction(exteriorWindow);
    exteriorSubSurfaceConstructions.setSkylightConstruction(exteriorWindow);
    exteriorSubSurfaceConstructions.setTubularDaylightDomeConstruction(exteriorWindow);
    exteriorSubSurfaceConstructions.setTubularDaylightDiffuserConstruction(exteriorWindow);
    fenestrationMaterials.clear();

    // Exterior Door

    StandardOpaqueMaterial f08_metal_surface(model);
    f08_metal_surface.setName("F08 Metal surface");
    f08_metal_surface.setRoughness("Smooth");
    f08_metal_surface.setThickness(0.0008);
    f08_metal_surface.setThermalConductivity(45.28);
    f08_metal_surface.setDensity(7824.0);
    f08_metal_surface.setSpecificHeat(500.0);

    opaqueMaterials.push_back(f08_metal_surface);

    StandardOpaqueMaterial i01_25mm_insulation_board(model);
    i01_25mm_insulation_board.setName("I02 25mm insulation board");
    i01_25mm_insulation_board.setRoughness("MediumRough");
    i01_25mm_insulation_board.setThickness(0.0254);
    i01_25mm_insulation_board.setThermalConductivity(0.03);
    i01_25mm_insulation_board.setDensity(43.0);
    i01_25mm_insulation_board.setSpecificHeat(1210.0);

    opaqueMaterials.push_back(i01_25mm_insulation_board);

    Construction exteriorDoor(opaqueMaterials);
    exteriorDoor.setName("Exterior Door");
    exteriorDoor.setInsulation(i01_25mm_insulation_board);
    exteriorSubSurfaceConstructions.setDoorConstruction(exteriorDoor);
    exteriorSubSurfaceConstructions.setOverheadDoorConstruction(exteriorDoor);
    opaqueMaterials.clear();

    // Interior Window

    //fenestrationMaterials.push_back(clear_3mm);

    // DLM: use simple glazing so we can know window properties without requiring E+ run
    fenestrationMaterials.push_back(simple_glazing);

    Construction interiorWindow(fenestrationMaterials);
    interiorWindow.setName("Interior Window");
    interiorSubSurfaceConstructions.setFixedWindowConstruction(interiorWindow);
    interiorSubSurfaceConstructions.setOperableWindowConstruction(interiorWindow);
    interiorSubSurfaceConstructions.setGlassDoorConstruction(interiorWindow);
    interiorSubSurfaceConstructions.setSkylightConstruction(interiorWindow);
    interiorSubSurfaceConstructions.setTubularDaylightDomeConstruction(interiorWindow);
    interiorSubSurfaceConstructions.setTubularDaylightDiffuserConstruction(interiorWindow);
    fenestrationMaterials.clear();

    // Interior Door

    StandardOpaqueMaterial g05_25mm_wood(model);
    g05_25mm_wood.setName("G05 25mm wood");
    g05_25mm_wood.setRoughness("MediumSmooth");
    g05_25mm_wood.setThickness(0.0254);
    g05_25mm_wood.setThermalConductivity(0.15);
    g05_25mm_wood.setDensity(608.0);
    g05_25mm_wood.setSpecificHeat(1630.0);

    opaqueMaterials.push_back(g05_25mm_wood);

    Construction interiorDoor(opaqueMaterials);
    interiorDoor.setName("Interior Door");
    interiorSubSurfaceConstructions.setDoorConstruction(interiorDoor);
    interiorSubSurfaceConstructions.setOverheadDoorConstruction(interiorDoor);
    opaqueMaterials.clear();

    // Interior Partition

    opaqueMaterials.push_back(g05_25mm_wood);

    Construction interiorPartition(opaqueMaterials);
    interiorPartition.setName("Interior Partition");
    defaultConstructions.setInteriorPartitionConstruction(interiorPartition);
    opaqueMaterials.clear();
  }

  void Model::autosize() {
    return getImpl<detail::Model_Impl>()->autosize();
  }

  void Model::applySizingValues() {
    return getImpl<detail::Model_Impl>()->applySizingValues();
  }

  // Template specializations for getUniqueModelObject to use caching
  template <>
  Building Model::getUniqueModelObject<Building>() {
    if (boost::optional<Building> _b = building()) {
      return _b.get();
    } else {
      return Building(*this);
    }
  }

  template <>
  FoundationKivaSettings Model::getUniqueModelObject<FoundationKivaSettings>() {
    if (boost::optional<FoundationKivaSettings> _b = foundationKivaSettings()) {
      return _b.get();
    } else {
      return FoundationKivaSettings(*this);
    }
  }

  template <>
  OutputControlFiles Model::getUniqueModelObject<OutputControlFiles>() {
    if (boost::optional<OutputControlFiles> _b = outputControlFiles()) {
      return _b.get();
    } else {
      return OutputControlFiles(*this);
    }
  }

  template <>
  OutputControlReportingTolerances Model::getUniqueModelObject<OutputControlReportingTolerances>() {
    if (boost::optional<OutputControlReportingTolerances> _b = outputControlReportingTolerances()) {
      return _b.get();
    } else {
      return OutputControlReportingTolerances(*this);
    }
  }

  template <>
  OutputControlTableStyle Model::getUniqueModelObject<OutputControlTableStyle>() {
    if (boost::optional<OutputControlTableStyle> _b = outputControlTableStyle()) {
      return _b.get();
    } else {
      return OutputControlTableStyle(*this);
    }
  }

  template <>
  OutputControlTimestamp Model::getUniqueModelObject<OutputControlTimestamp>() {
    if (boost::optional<OutputControlTimestamp> _b = outputControlTimestamp()) {
      return _b.get();
    } else {
      return OutputControlTimestamp(*this);
    }
  }

  template <>
  OutputDiagnostics Model::getUniqueModelObject<OutputDiagnostics>() {
    if (boost::optional<OutputDiagnostics> _b = outputDiagnostics()) {
      return _b.get();
    } else {
      return OutputDiagnostics(*this);
    }
  }

  template <>
  OutputDebuggingData Model::getUniqueModelObject<OutputDebuggingData>() {
    if (boost::optional<OutputDebuggingData> _b = outputDebuggingData()) {
      return _b.get();
    } else {
      return OutputDebuggingData(*this);
    }
  }

  template <>
  OutputJSON Model::getUniqueModelObject<OutputJSON>() {
    if (boost::optional<OutputJSON> _b = outputJSON()) {
      return _b.get();
    } else {
      return OutputJSON(*this);
    }
  }

  template <>
  OutputSQLite Model::getUniqueModelObject<OutputSQLite>() {
    if (boost::optional<OutputSQLite> _b = outputSQLite()) {
      return _b.get();
    } else {
      return OutputSQLite(*this);
    }
  }

  template <>
  OutputEnergyManagementSystem Model::getUniqueModelObject<OutputEnergyManagementSystem>() {
    if (boost::optional<OutputEnergyManagementSystem> _b = outputEnergyManagementSystem()) {
      return _b.get();
    } else {
      return OutputEnergyManagementSystem(*this);
    }
  }

  template <>
  OutputTableSummaryReports Model::getUniqueModelObject<OutputTableSummaryReports>() {
    if (boost::optional<OutputTableSummaryReports> _b = outputTableSummaryReports()) {
      return _b.get();
    } else {
      return OutputTableSummaryReports(*this);
    }
  }

  template <>
  OutputSchedules Model::getUniqueModelObject<OutputSchedules>() {
    if (boost::optional<OutputSchedules> _b = outputSchedules()) {
      return _b.get();
    } else {
      return OutputSchedules(*this);
    }
  }

  template <>
  OutputConstructions Model::getUniqueModelObject<OutputConstructions>() {
    if (boost::optional<OutputConstructions> _b = outputConstructions()) {
      return _b.get();
    } else {
      return OutputConstructions(*this);
    }
  }

  template <>
  PerformancePrecisionTradeoffs Model::getUniqueModelObject<PerformancePrecisionTradeoffs>() {
    if (boost::optional<PerformancePrecisionTradeoffs> _p = performancePrecisionTradeoffs()) {
      return _p.get();
    } else {
      return PerformancePrecisionTradeoffs(*this);
    }
  }

  template <>
  LifeCycleCostParameters Model::getUniqueModelObject<LifeCycleCostParameters>() {
    if (boost::optional<LifeCycleCostParameters> _l = lifeCycleCostParameters()) {
      return _l.get();
    } else {
      return LifeCycleCostParameters(*this);
    }
  }

  template <>
  SizingParameters Model::getUniqueModelObject<SizingParameters>() {
    if (boost::optional<SizingParameters> _b = sizingParameters()) {
      return _b.get();
    } else {
      return SizingParameters(*this);
    }
  }

  template <>
  RadianceParameters Model::getUniqueModelObject<RadianceParameters>() {
    if (boost::optional<RadianceParameters> _b = radianceParameters()) {
      return _b.get();
    } else {
      return RadianceParameters(*this);
    }
  }

  template <>
  RunPeriod Model::getUniqueModelObject<RunPeriod>() {
    if (boost::optional<RunPeriod> _r = runPeriod()) {
      return _r.get();
    } else {
      return RunPeriod(*this);
    }
  }

  template <>
  RunPeriodControlDaylightSavingTime Model::getUniqueModelObject<RunPeriodControlDaylightSavingTime>() {
    if (boost::optional<RunPeriodControlDaylightSavingTime> _b = runPeriodControlDaylightSavingTime()) {
      return _b.get();
    } else {
      return RunPeriodControlDaylightSavingTime(*this);
    }
  }

  template <>
  YearDescription Model::getUniqueModelObject<YearDescription>() {
    if (boost::optional<YearDescription> _yd = yearDescription()) {
      return _yd.get();
    } else {
      return YearDescription(*this);
    }
  }

  template <>
  Site Model::getUniqueModelObject<Site>() {
    if (boost::optional<Site> _s = site()) {
      return _s.get();
    } else {
      return Site(*this);
    }
  }

  template <>
  SiteGroundReflectance Model::getUniqueModelObject<SiteGroundReflectance>() {
    if (boost::optional<SiteGroundReflectance> _b = siteGroundReflectance()) {
      return _b.get();
    } else {
      return SiteGroundReflectance(*this);
    }
  }

  template <>
  SiteWaterMainsTemperature Model::getUniqueModelObject<SiteWaterMainsTemperature>() {
    if (boost::optional<SiteWaterMainsTemperature> _swmt = siteWaterMainsTemperature()) {
      return _swmt.get();
    } else {
      return SiteWaterMainsTemperature(*this);
    }
  }

  template <>
  SiteGroundTemperatureBuildingSurface Model::getUniqueModelObject<SiteGroundTemperatureBuildingSurface>() {
    if (boost::optional<SiteGroundTemperatureBuildingSurface> _sgtbs = siteGroundTemperatureBuildingSurface()) {
      return _sgtbs.get();
    } else {
      return SiteGroundTemperatureBuildingSurface(*this);
    }
  }

  template <>
  SiteGroundTemperatureFCfactorMethod Model::getUniqueModelObject<SiteGroundTemperatureFCfactorMethod>() {
    if (boost::optional<SiteGroundTemperatureFCfactorMethod> _b = siteGroundTemperatureFCfactorMethod()) {
      return _b.get();
    } else {
      return SiteGroundTemperatureFCfactorMethod(*this);
    }
  }

  template <>
  SiteGroundTemperatureDeep Model::getUniqueModelObject<SiteGroundTemperatureDeep>() {
    if (boost::optional<SiteGroundTemperatureDeep> _sgtd = siteGroundTemperatureDeep()) {
      return _sgtd.get();
    } else {
      return SiteGroundTemperatureDeep(*this);
    }
  }

  template <>
  SiteGroundTemperatureShallow Model::getUniqueModelObject<SiteGroundTemperatureShallow>() {
    if (boost::optional<SiteGroundTemperatureShallow> _b = siteGroundTemperatureShallow()) {
      return _b.get();
    } else {
      return SiteGroundTemperatureShallow(*this);
    }
  }

  template <>
  Facility Model::getUniqueModelObject<Facility>() {
    if (boost::optional<Facility> _f = facility()) {
      return _f.get();
    } else {
      return Facility(*this);
    }
  }

  template <>
  WeatherFile Model::getUniqueModelObject<WeatherFile>() {
    if (boost::optional<WeatherFile> _w = weatherFile()) {
      return _w.get();
    } else {
      return WeatherFile(*this);
    }
  }

  template <>
  SimulationControl Model::getUniqueModelObject<SimulationControl>() {
    if (boost::optional<SimulationControl> _sc = simulationControl()) {
      return _sc.get();
    } else {
      return SimulationControl(*this);
    }
  }

  template <>
  LightingSimulationControl Model::getUniqueModelObject<LightingSimulationControl>() {
    if (boost::optional<LightingSimulationControl> _b = lightingSimulationControl()) {
      return _b.get();
    } else {
      return LightingSimulationControl(*this);
    }
  }

  template <>
  AirflowNetworkSimulationControl Model::getUniqueModelObject<AirflowNetworkSimulationControl>() {
    if (boost::optional<AirflowNetworkSimulationControl> _b = airflowNetworkSimulationControl()) {
      return _b.get();
    } else {
      return AirflowNetworkSimulationControl(*this);
    }
  }

  template <>
  InsideSurfaceConvectionAlgorithm Model::getUniqueModelObject<InsideSurfaceConvectionAlgorithm>() {
    if (boost::optional<InsideSurfaceConvectionAlgorithm> _isca = insideSurfaceConvectionAlgorithm()) {
      return _isca.get();
    } else {
      return InsideSurfaceConvectionAlgorithm(*this);
    }
  }

  template <>
  OutsideSurfaceConvectionAlgorithm Model::getUniqueModelObject<OutsideSurfaceConvectionAlgorithm>() {
    if (boost::optional<OutsideSurfaceConvectionAlgorithm> _osca = outsideSurfaceConvectionAlgorithm()) {
      return _osca.get();
    } else {
      return OutsideSurfaceConvectionAlgorithm(*this);
    }
  }

  template <>
  HeatBalanceAlgorithm Model::getUniqueModelObject<HeatBalanceAlgorithm>() {
    if (boost::optional<HeatBalanceAlgorithm> _hba = heatBalanceAlgorithm()) {
      return _hba.get();
    } else {
      return HeatBalanceAlgorithm(*this);
    }
  }

  template <>
  ZoneAirContaminantBalance Model::getUniqueModelObject<ZoneAirContaminantBalance>() {
    if (boost::optional<ZoneAirContaminantBalance> _zahba = zoneAirContaminantBalance()) {
      return _zahba.get();
    } else {
      return ZoneAirContaminantBalance(*this);
    }
  }

  template <>
  ZoneAirHeatBalanceAlgorithm Model::getUniqueModelObject<ZoneAirHeatBalanceAlgorithm>() {
    if (boost::optional<ZoneAirHeatBalanceAlgorithm> _zahba = zoneAirHeatBalanceAlgorithm()) {
      return _zahba.get();
    } else {
      return ZoneAirHeatBalanceAlgorithm(*this);
    }
  }

  template <>
  ZoneAirMassFlowConservation Model::getUniqueModelObject<ZoneAirMassFlowConservation>() {
    if (boost::optional<ZoneAirMassFlowConservation> _b = zoneAirMassFlowConservation()) {
      return _b.get();
    } else {
      return ZoneAirMassFlowConservation(*this);
    }
  }

  template <>
  ZoneCapacitanceMultiplierResearchSpecial Model::getUniqueModelObject<ZoneCapacitanceMultiplierResearchSpecial>() {
    if (boost::optional<ZoneCapacitanceMultiplierResearchSpecial> _b = zoneCapacitanceMultiplierResearchSpecial()) {
      return _b.get();
    } else {
      return ZoneCapacitanceMultiplierResearchSpecial(*this);
    }
  }

  template <>
  ConvergenceLimits Model::getUniqueModelObject<ConvergenceLimits>() {
    if (boost::optional<ConvergenceLimits> _cl = convergenceLimits()) {
      return _cl.get();
    } else {
      return ConvergenceLimits(*this);
    }
  }

  template <>
  ShadowCalculation Model::getUniqueModelObject<ShadowCalculation>() {
    if (boost::optional<ShadowCalculation> _sc = shadowCalculation()) {
      return _sc.get();
    } else {
      return ShadowCalculation(*this);
    }
  }

  template <>
  Timestep Model::getUniqueModelObject<Timestep>() {
    if (boost::optional<Timestep> _b = timestep()) {
      return _b.get();
    } else {
      return Timestep(*this);
    }
  }

  template <>
  ClimateZones Model::getUniqueModelObject<ClimateZones>() {
    if (boost::optional<ClimateZones> _b = climateZones()) {
      return _b.get();
    } else {
      return ClimateZones(*this);
    }
  }

  template <>
  EnvironmentalImpactFactors Model::getUniqueModelObject<EnvironmentalImpactFactors>() {
    if (boost::optional<EnvironmentalImpactFactors> _b = environmentalImpactFactors()) {
      return _b.get();
    } else {
      return EnvironmentalImpactFactors(*this);
    }
  }

  template <>
  ExternalInterface Model::getUniqueModelObject<ExternalInterface>() {
    if (boost::optional<ExternalInterface> _b = externalInterface()) {
      return _b.get();
    } else {
      return ExternalInterface(*this);
    }
  }

  template <>
  PythonPluginSearchPaths Model::getUniqueModelObject<PythonPluginSearchPaths>() {
    if (boost::optional<PythonPluginSearchPaths> _b = pythonPluginSearchPaths()) {
      return _b.get();
    } else {
      return PythonPluginSearchPaths(*this);
    }
  }

  std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> detail::Model_Impl::ModelObjectCreator::getNew(Model_Impl* model, const IdfObject& obj,
                                                                                                           bool keepHandle) const {
    auto typeToCreate = obj.iddObject().type();
    std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> result;
    auto it = m_newMap.find(typeToCreate);
    if (it != m_newMap.end()) {
      result = it->second(model, obj, keepHandle);
    }
    return result;
  }

  std::shared_ptr<openstudio::detail::WorkspaceObject_Impl>
    detail::Model_Impl::ModelObjectCreator::getCopy(Model_Impl* model, const std::shared_ptr<openstudio::detail::WorkspaceObject_Impl>& obj,
                                                    bool keepHandle) const {
    auto typeToCreate = obj->iddObject().type();
    std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> result;
    auto it = m_copyMap.find(typeToCreate);
    if (it != m_copyMap.end()) {
      result = it->second(model, obj, keepHandle);
    }
    return result;
  }

  detail::Model_Impl::ModelObjectCreator::ModelObjectCreator() {
#define REGISTER_CONSTRUCTOR(_className)                                                                                           \
  m_newMap[_className::iddObjectType()] = [](openstudio::model::detail::Model_Impl* m, const IdfObject& object, bool keepHandle) { \
    return std::make_shared<_className##_Impl>(object, m, keepHandle);                                                             \
  };

    REGISTER_CONSTRUCTOR(AdditionalProperties);
    REGISTER_CONSTRUCTOR(AirConditionerVariableRefrigerantFlow);
    REGISTER_CONSTRUCTOR(AirConditionerVariableRefrigerantFlowFluidTemperatureControl);
    REGISTER_CONSTRUCTOR(AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR);
    REGISTER_CONSTRUCTOR(AirflowNetworkConstantPressureDrop);
    REGISTER_CONSTRUCTOR(AirflowNetworkCrack);
    REGISTER_CONSTRUCTOR(AirflowNetworkDetailedOpening);
    REGISTER_CONSTRUCTOR(AirflowNetworkDistributionLinkage);
    REGISTER_CONSTRUCTOR(AirflowNetworkDistributionNode);
    REGISTER_CONSTRUCTOR(AirflowNetworkDuct);
    REGISTER_CONSTRUCTOR(AirflowNetworkDuctViewFactors);
    REGISTER_CONSTRUCTOR(AirflowNetworkEffectiveLeakageArea);
    REGISTER_CONSTRUCTOR(AirflowNetworkEquivalentDuct);
    REGISTER_CONSTRUCTOR(AirflowNetworkExternalNode);
    REGISTER_CONSTRUCTOR(AirflowNetworkFan);
    REGISTER_CONSTRUCTOR(AirflowNetworkHorizontalOpening);
    REGISTER_CONSTRUCTOR(AirflowNetworkLeakageRatio);
    REGISTER_CONSTRUCTOR(AirflowNetworkOccupantVentilationControl);
    REGISTER_CONSTRUCTOR(AirflowNetworkOutdoorAirflow);
    REGISTER_CONSTRUCTOR(AirflowNetworkReferenceCrackConditions);
    REGISTER_CONSTRUCTOR(AirflowNetworkSimpleOpening);
    REGISTER_CONSTRUCTOR(AirflowNetworkSimulationControl);
    REGISTER_CONSTRUCTOR(AirflowNetworkSpecifiedFlowRate);
    REGISTER_CONSTRUCTOR(AirflowNetworkSurface);
    REGISTER_CONSTRUCTOR(AirflowNetworkZone);
    REGISTER_CONSTRUCTOR(AirflowNetworkZoneExhaustFan);
    REGISTER_CONSTRUCTOR(AirGap);
    REGISTER_CONSTRUCTOR(AirLoopHVAC);
    REGISTER_CONSTRUCTOR(AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass);
    REGISTER_CONSTRUCTOR(AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed);
    REGISTER_CONSTRUCTOR(AirLoopHVACUnitaryHeatPumpAirToAir);
    REGISTER_CONSTRUCTOR(AirLoopHVACOutdoorAirSystem);
    REGISTER_CONSTRUCTOR(AirLoopHVACDedicatedOutdoorAirSystem);
    REGISTER_CONSTRUCTOR(AirLoopHVACReturnPlenum);
    REGISTER_CONSTRUCTOR(AirLoopHVACSupplyPlenum);
    REGISTER_CONSTRUCTOR(AirLoopHVACUnitarySystem);
    REGISTER_CONSTRUCTOR(AirLoopHVACZoneMixer);
    REGISTER_CONSTRUCTOR(AirLoopHVACZoneSplitter);
    REGISTER_CONSTRUCTOR(AirTerminalDualDuctConstantVolume);
    REGISTER_CONSTRUCTOR(AirTerminalDualDuctVAV);
    REGISTER_CONSTRUCTOR(AirTerminalDualDuctVAVOutdoorAir);
    REGISTER_CONSTRUCTOR(AirTerminalSingleDuctInletSideMixer);
    REGISTER_CONSTRUCTOR(AirTerminalSingleDuctConstantVolumeCooledBeam);
    REGISTER_CONSTRUCTOR(AirTerminalSingleDuctConstantVolumeFourPipeInduction);
    REGISTER_CONSTRUCTOR(AirTerminalSingleDuctConstantVolumeFourPipeBeam);
    REGISTER_CONSTRUCTOR(AirTerminalSingleDuctConstantVolumeReheat);
    REGISTER_CONSTRUCTOR(AirTerminalSingleDuctParallelPIUReheat);
    REGISTER_CONSTRUCTOR(AirTerminalSingleDuctSeriesPIUReheat);
    REGISTER_CONSTRUCTOR(AirTerminalSingleDuctConstantVolumeNoReheat);
    REGISTER_CONSTRUCTOR(AirTerminalSingleDuctVAVReheat);
    REGISTER_CONSTRUCTOR(AirTerminalSingleDuctVAVNoReheat);
    REGISTER_CONSTRUCTOR(AirTerminalSingleDuctVAVHeatAndCoolNoReheat);
    REGISTER_CONSTRUCTOR(AirTerminalSingleDuctVAVHeatAndCoolReheat);
    REGISTER_CONSTRUCTOR(AvailabilityManagerAssignmentList);
    REGISTER_CONSTRUCTOR(AvailabilityManagerNightCycle);
    REGISTER_CONSTRUCTOR(AvailabilityManagerOptimumStart);
    REGISTER_CONSTRUCTOR(AvailabilityManagerHybridVentilation);
    REGISTER_CONSTRUCTOR(AvailabilityManagerDifferentialThermostat);
    REGISTER_CONSTRUCTOR(AvailabilityManagerNightVentilation);
    REGISTER_CONSTRUCTOR(AvailabilityManagerHighTemperatureTurnOn);
    REGISTER_CONSTRUCTOR(AvailabilityManagerHighTemperatureTurnOff);
    REGISTER_CONSTRUCTOR(AvailabilityManagerLowTemperatureTurnOn);
    REGISTER_CONSTRUCTOR(AvailabilityManagerLowTemperatureTurnOff);
    REGISTER_CONSTRUCTOR(AvailabilityManagerScheduled);
    REGISTER_CONSTRUCTOR(AvailabilityManagerScheduledOn);
    REGISTER_CONSTRUCTOR(AvailabilityManagerScheduledOff);
    REGISTER_CONSTRUCTOR(Blind);
    REGISTER_CONSTRUCTOR(BoilerHotWater);
    REGISTER_CONSTRUCTOR(BoilerSteam);
    REGISTER_CONSTRUCTOR(Building);
    REGISTER_CONSTRUCTOR(BuildingStory);
    REGISTER_CONSTRUCTOR(BuildingUnit);
    REGISTER_CONSTRUCTOR(CentralHeatPumpSystem);
    REGISTER_CONSTRUCTOR(CentralHeatPumpSystemModule);
    REGISTER_CONSTRUCTOR(CFactorUndergroundWallConstruction);
    REGISTER_CONSTRUCTOR(ChillerAbsorption);
    REGISTER_CONSTRUCTOR(ChillerAbsorptionIndirect);
    REGISTER_CONSTRUCTOR(ChillerElectricASHRAE205);
    REGISTER_CONSTRUCTOR(ChillerElectricEIR);
    REGISTER_CONSTRUCTOR(ChillerElectricReformulatedEIR);
    REGISTER_CONSTRUCTOR(ChillerHeaterPerformanceElectricEIR);
    REGISTER_CONSTRUCTOR(ClimateZones);
    REGISTER_CONSTRUCTOR(CoilCoolingCooledBeam);
    REGISTER_CONSTRUCTOR(CoilCoolingDX);
    REGISTER_CONSTRUCTOR(CoilCoolingDXCurveFitPerformance);
    REGISTER_CONSTRUCTOR(CoilCoolingDXCurveFitOperatingMode);
    REGISTER_CONSTRUCTOR(CoilCoolingDXCurveFitSpeed);
    REGISTER_CONSTRUCTOR(CoilCoolingDXMultiSpeed);
    REGISTER_CONSTRUCTOR(CoilCoolingDXMultiSpeedStageData);
    REGISTER_CONSTRUCTOR(CoilCoolingDXSingleSpeed);
    REGISTER_CONSTRUCTOR(CoilCoolingDXSingleSpeedThermalStorage);
    REGISTER_CONSTRUCTOR(CoilCoolingDXTwoSpeed);
    REGISTER_CONSTRUCTOR(CoilCoolingDXTwoStageWithHumidityControlMode);
    REGISTER_CONSTRUCTOR(CoilCoolingDXVariableRefrigerantFlow);
    REGISTER_CONSTRUCTOR(CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl);
    REGISTER_CONSTRUCTOR(CoilCoolingLowTempRadiantConstFlow);
    REGISTER_CONSTRUCTOR(CoilCoolingLowTempRadiantVarFlow);
    REGISTER_CONSTRUCTOR(CoilCoolingDXVariableSpeed);
    REGISTER_CONSTRUCTOR(CoilCoolingDXVariableSpeedSpeedData);
    REGISTER_CONSTRUCTOR(CoilCoolingFourPipeBeam);
    REGISTER_CONSTRUCTOR(CoilCoolingWater);
    REGISTER_CONSTRUCTOR(CoilCoolingWaterToAirHeatPumpEquationFit);
    REGISTER_CONSTRUCTOR(CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit);
    REGISTER_CONSTRUCTOR(CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData);
    REGISTER_CONSTRUCTOR(CoilCoolingWaterPanelRadiant);
    REGISTER_CONSTRUCTOR(CoilHeatingDesuperheater);
    REGISTER_CONSTRUCTOR(CoilHeatingDXMultiSpeed);
    REGISTER_CONSTRUCTOR(CoilHeatingDXMultiSpeedStageData);
    REGISTER_CONSTRUCTOR(CoilHeatingDXSingleSpeed);
    REGISTER_CONSTRUCTOR(CoilHeatingDXVariableRefrigerantFlow);
    REGISTER_CONSTRUCTOR(CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl);
    REGISTER_CONSTRUCTOR(CoilHeatingElectric);
    REGISTER_CONSTRUCTOR(CoilHeatingElectricMultiStage);
    REGISTER_CONSTRUCTOR(CoilHeatingElectricMultiStageStageData);
    REGISTER_CONSTRUCTOR(CoilHeatingFourPipeBeam);
    REGISTER_CONSTRUCTOR(CoilHeatingGas);
    REGISTER_CONSTRUCTOR(CoilHeatingGasMultiStage);
    REGISTER_CONSTRUCTOR(CoilHeatingGasMultiStageStageData);
    REGISTER_CONSTRUCTOR(CoilHeatingLowTempRadiantConstFlow);
    REGISTER_CONSTRUCTOR(CoilHeatingLowTempRadiantVarFlow);
    REGISTER_CONSTRUCTOR(CoilHeatingDXVariableSpeed);
    REGISTER_CONSTRUCTOR(CoilHeatingDXVariableSpeedSpeedData);
    REGISTER_CONSTRUCTOR(CoilHeatingWater);
    REGISTER_CONSTRUCTOR(CoilHeatingWaterToAirHeatPumpEquationFit);
    REGISTER_CONSTRUCTOR(CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit);
    REGISTER_CONSTRUCTOR(CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData);
    REGISTER_CONSTRUCTOR(CoilHeatingWaterBaseboard);
    REGISTER_CONSTRUCTOR(CoilHeatingWaterBaseboardRadiant);
    REGISTER_CONSTRUCTOR(CoilPerformanceDXCooling);
    REGISTER_CONSTRUCTOR(CoilSystemCoolingWaterHeatExchangerAssisted);
    REGISTER_CONSTRUCTOR(CoilSystemCoolingDXHeatExchangerAssisted);
    REGISTER_CONSTRUCTOR(CoilSystemIntegratedHeatPumpAirSource);
    REGISTER_CONSTRUCTOR(CoilUserDefined);
    REGISTER_CONSTRUCTOR(CoilWaterHeatingAirToWaterHeatPump);
    REGISTER_CONSTRUCTOR(CoilWaterHeatingAirToWaterHeatPumpVariableSpeed);
    REGISTER_CONSTRUCTOR(CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData);
    REGISTER_CONSTRUCTOR(CoilWaterHeatingAirToWaterHeatPumpWrapped);
    REGISTER_CONSTRUCTOR(CoilWaterHeatingDesuperheater);
    REGISTER_CONSTRUCTOR(ComponentCostAdjustments);
    REGISTER_CONSTRUCTOR(ComponentData);
    REGISTER_CONSTRUCTOR(Connection);
    REGISTER_CONSTRUCTOR(ConnectorMixer);
    REGISTER_CONSTRUCTOR(ConnectorSplitter);
    REGISTER_CONSTRUCTOR(Construction);
    REGISTER_CONSTRUCTOR(ConstructionAirBoundary);
    REGISTER_CONSTRUCTOR(ConstructionWithInternalSource);
    REGISTER_CONSTRUCTOR(ControllerMechanicalVentilation);
    REGISTER_CONSTRUCTOR(ControllerOutdoorAir);
    REGISTER_CONSTRUCTOR(ControllerWaterCoil);
    REGISTER_CONSTRUCTOR(ConvergenceLimits);
    REGISTER_CONSTRUCTOR(CoolingTowerPerformanceCoolTools);
    REGISTER_CONSTRUCTOR(CoolingTowerPerformanceYorkCalc);
    REGISTER_CONSTRUCTOR(CoolingTowerSingleSpeed);
    REGISTER_CONSTRUCTOR(CoolingTowerTwoSpeed);
    REGISTER_CONSTRUCTOR(CoolingTowerVariableSpeed);
    REGISTER_CONSTRUCTOR(CurrencyType);
    REGISTER_CONSTRUCTOR(CurveBicubic);
    REGISTER_CONSTRUCTOR(CurveBiquadratic);
    REGISTER_CONSTRUCTOR(CurveCubic);
    REGISTER_CONSTRUCTOR(CurveDoubleExponentialDecay);
    REGISTER_CONSTRUCTOR(CurveExponent);
    REGISTER_CONSTRUCTOR(CurveExponentialDecay);
    REGISTER_CONSTRUCTOR(CurveExponentialSkewNormal);
    REGISTER_CONSTRUCTOR(CurveFanPressureRise);
    REGISTER_CONSTRUCTOR(CurveFunctionalPressureDrop);
    REGISTER_CONSTRUCTOR(CurveLinear);
    REGISTER_CONSTRUCTOR(CurveQuadLinear);
    REGISTER_CONSTRUCTOR(CurveQuintLinear);
    REGISTER_CONSTRUCTOR(CurveQuadratic);
    REGISTER_CONSTRUCTOR(CurveQuadraticLinear);
    REGISTER_CONSTRUCTOR(CurveQuartic);
    REGISTER_CONSTRUCTOR(CurveRectangularHyperbola1);
    REGISTER_CONSTRUCTOR(CurveRectangularHyperbola2);
    REGISTER_CONSTRUCTOR(CurveSigmoid);
    REGISTER_CONSTRUCTOR(CurveTriquadratic);
    REGISTER_CONSTRUCTOR(DaylightingControl);
    REGISTER_CONSTRUCTOR(DaylightingDeviceShelf);
    REGISTER_CONSTRUCTOR(DaylightingDeviceTubular);
    REGISTER_CONSTRUCTOR(DaylightingDeviceLightWell);
    REGISTER_CONSTRUCTOR(DaylightRedirectionDevice);
    REGISTER_CONSTRUCTOR(DefaultConstructionSet);
    REGISTER_CONSTRUCTOR(DefaultScheduleSet);
    REGISTER_CONSTRUCTOR(DefaultSubSurfaceConstructions);
    REGISTER_CONSTRUCTOR(DefaultSurfaceConstructions);
    REGISTER_CONSTRUCTOR(DesignDay);
    REGISTER_CONSTRUCTOR(DesignSpecificationOutdoorAir);
    REGISTER_CONSTRUCTOR(DesignSpecificationZoneAirDistribution);
    REGISTER_CONSTRUCTOR(DistrictCooling);
    REGISTER_CONSTRUCTOR(DistrictHeatingWater);
    REGISTER_CONSTRUCTOR(DistrictHeatingSteam);
    REGISTER_CONSTRUCTOR(Duct);
    REGISTER_CONSTRUCTOR(ElectricEquipment);
    REGISTER_CONSTRUCTOR(ElectricEquipmentDefinition);
    REGISTER_CONSTRUCTOR(ElectricEquipmentITEAirCooled);
    REGISTER_CONSTRUCTOR(ElectricEquipmentITEAirCooledDefinition);
    REGISTER_CONSTRUCTOR(ElectricLoadCenterDistribution);
    REGISTER_CONSTRUCTOR(ElectricLoadCenterInverterLookUpTable);
    REGISTER_CONSTRUCTOR(ElectricLoadCenterInverterSimple);
    REGISTER_CONSTRUCTOR(ElectricLoadCenterInverterPVWatts);
    REGISTER_CONSTRUCTOR(ElectricLoadCenterStorageSimple);
    REGISTER_CONSTRUCTOR(ElectricLoadCenterStorageConverter);
    REGISTER_CONSTRUCTOR(ElectricLoadCenterStorageLiIonNMCBattery);
    REGISTER_CONSTRUCTOR(ElectricLoadCenterTransformer);
    REGISTER_CONSTRUCTOR(EnergyManagementSystemActuator);
    REGISTER_CONSTRUCTOR(EnergyManagementSystemConstructionIndexVariable);
    REGISTER_CONSTRUCTOR(EnergyManagementSystemCurveOrTableIndexVariable);
    REGISTER_CONSTRUCTOR(EnergyManagementSystemGlobalVariable);
    REGISTER_CONSTRUCTOR(EnergyManagementSystemInternalVariable);
    REGISTER_CONSTRUCTOR(EnergyManagementSystemMeteredOutputVariable);
    REGISTER_CONSTRUCTOR(EnergyManagementSystemOutputVariable);
    REGISTER_CONSTRUCTOR(EnergyManagementSystemProgram);
    REGISTER_CONSTRUCTOR(EnergyManagementSystemProgramCallingManager);
    REGISTER_CONSTRUCTOR(EnergyManagementSystemSensor);
    REGISTER_CONSTRUCTOR(EnergyManagementSystemSubroutine);
    REGISTER_CONSTRUCTOR(EnergyManagementSystemTrendVariable);
    REGISTER_CONSTRUCTOR(EvaporativeCoolerDirectResearchSpecial);
    REGISTER_CONSTRUCTOR(EvaporativeCoolerIndirectResearchSpecial);
    REGISTER_CONSTRUCTOR(EvaporativeFluidCoolerSingleSpeed);
    REGISTER_CONSTRUCTOR(EvaporativeFluidCoolerTwoSpeed);
    REGISTER_CONSTRUCTOR(ExteriorLights);
    REGISTER_CONSTRUCTOR(ExteriorLightsDefinition);
    REGISTER_CONSTRUCTOR(ExteriorFuelEquipment);
    REGISTER_CONSTRUCTOR(ExteriorFuelEquipmentDefinition);
    REGISTER_CONSTRUCTOR(ExteriorWaterEquipment);
    REGISTER_CONSTRUCTOR(ExteriorWaterEquipmentDefinition);
    REGISTER_CONSTRUCTOR(ExternalFile);
    REGISTER_CONSTRUCTOR(ExternalInterface);
    REGISTER_CONSTRUCTOR(ExternalInterfaceActuator);
    REGISTER_CONSTRUCTOR(ExternalInterfaceFunctionalMockupUnitExportFromVariable);
    REGISTER_CONSTRUCTOR(ExternalInterfaceFunctionalMockupUnitExportToActuator);
    REGISTER_CONSTRUCTOR(ExternalInterfaceFunctionalMockupUnitExportToSchedule);
    REGISTER_CONSTRUCTOR(ExternalInterfaceFunctionalMockupUnitExportToVariable);
    REGISTER_CONSTRUCTOR(ExternalInterfaceFunctionalMockupUnitImport);
    REGISTER_CONSTRUCTOR(ExternalInterfaceFunctionalMockupUnitImportFromVariable);
    REGISTER_CONSTRUCTOR(ExternalInterfaceFunctionalMockupUnitImportToActuator);
    REGISTER_CONSTRUCTOR(ExternalInterfaceFunctionalMockupUnitImportToSchedule);
    REGISTER_CONSTRUCTOR(ExternalInterfaceFunctionalMockupUnitImportToVariable);
    REGISTER_CONSTRUCTOR(ExternalInterfaceSchedule);
    REGISTER_CONSTRUCTOR(ExternalInterfaceVariable);
    REGISTER_CONSTRUCTOR(Facility);
    REGISTER_CONSTRUCTOR(FanComponentModel);
    REGISTER_CONSTRUCTOR(FanConstantVolume);
    REGISTER_CONSTRUCTOR(FanOnOff);
    REGISTER_CONSTRUCTOR(FanSystemModel);
    REGISTER_CONSTRUCTOR(FanVariableVolume);
    REGISTER_CONSTRUCTOR(FanZoneExhaust);
    REGISTER_CONSTRUCTOR(FFactorGroundFloorConstruction);
    REGISTER_CONSTRUCTOR(FluidCoolerSingleSpeed);
    REGISTER_CONSTRUCTOR(FluidCoolerTwoSpeed);
    REGISTER_CONSTRUCTOR(FoundationKiva);
    REGISTER_CONSTRUCTOR(FoundationKivaSettings);
    REGISTER_CONSTRUCTOR(Gas);
    REGISTER_CONSTRUCTOR(GasEquipment);
    REGISTER_CONSTRUCTOR(GasEquipmentDefinition);
    REGISTER_CONSTRUCTOR(GasMixture);
    REGISTER_CONSTRUCTOR(GeneratorFuelCell);
    REGISTER_CONSTRUCTOR(GeneratorFuelCellAirSupply);
    REGISTER_CONSTRUCTOR(GeneratorFuelCellAuxiliaryHeater);
    REGISTER_CONSTRUCTOR(GeneratorFuelCellElectricalStorage);
    REGISTER_CONSTRUCTOR(GeneratorFuelCellExhaustGasToWaterHeatExchanger);
    REGISTER_CONSTRUCTOR(GeneratorFuelCellInverter);
    REGISTER_CONSTRUCTOR(GeneratorFuelCellPowerModule);
    REGISTER_CONSTRUCTOR(GeneratorFuelCellStackCooler);
    REGISTER_CONSTRUCTOR(GeneratorFuelCellWaterSupply);
    REGISTER_CONSTRUCTOR(GeneratorFuelSupply);
    REGISTER_CONSTRUCTOR(GeneratorMicroTurbine);
    REGISTER_CONSTRUCTOR(GeneratorMicroTurbineHeatRecovery);
    REGISTER_CONSTRUCTOR(GeneratorPhotovoltaic);
    REGISTER_CONSTRUCTOR(GeneratorPVWatts);
    REGISTER_CONSTRUCTOR(GeneratorWindTurbine);
    REGISTER_CONSTRUCTOR(GlareSensor);
    REGISTER_CONSTRUCTOR(GroundHeatExchangerHorizontalTrench);
    REGISTER_CONSTRUCTOR(GroundHeatExchangerVertical);
    REGISTER_CONSTRUCTOR(HeaderedPumpsConstantSpeed);
    REGISTER_CONSTRUCTOR(HeaderedPumpsVariableSpeed);
    REGISTER_CONSTRUCTOR(HeatBalanceAlgorithm);
    REGISTER_CONSTRUCTOR(HeatExchangerAirToAirSensibleAndLatent);
    REGISTER_CONSTRUCTOR(HeatExchangerDesiccantBalancedFlow);
    REGISTER_CONSTRUCTOR(HeatExchangerDesiccantBalancedFlowPerformanceDataType1);
    REGISTER_CONSTRUCTOR(HeatExchangerFluidToFluid);
    REGISTER_CONSTRUCTOR(HeatPumpAirToWaterFuelFiredHeating);
    REGISTER_CONSTRUCTOR(HeatPumpAirToWaterFuelFiredCooling);
    REGISTER_CONSTRUCTOR(HeatPumpWaterToWaterEquationFitCooling);
    REGISTER_CONSTRUCTOR(HeatPumpWaterToWaterEquationFitHeating);
    REGISTER_CONSTRUCTOR(HeatPumpPlantLoopEIRCooling);
    REGISTER_CONSTRUCTOR(HeatPumpPlantLoopEIRHeating);
    REGISTER_CONSTRUCTOR(HotWaterEquipment);
    REGISTER_CONSTRUCTOR(HotWaterEquipmentDefinition);
    REGISTER_CONSTRUCTOR(HumidifierSteamElectric);
    REGISTER_CONSTRUCTOR(HumidifierSteamGas);
    REGISTER_CONSTRUCTOR(IlluminanceMap);
    REGISTER_CONSTRUCTOR(InfraredTransparentMaterial);
    REGISTER_CONSTRUCTOR(InsideSurfaceConvectionAlgorithm);
    REGISTER_CONSTRUCTOR(InteriorPartitionSurface);
    REGISTER_CONSTRUCTOR(InteriorPartitionSurfaceGroup);
    REGISTER_CONSTRUCTOR(InternalMass);
    REGISTER_CONSTRUCTOR(InternalMassDefinition);
    REGISTER_CONSTRUCTOR(LoadProfilePlant);
    REGISTER_CONSTRUCTOR(LifeCycleCost);
    REGISTER_CONSTRUCTOR(LifeCycleCostParameters);
    REGISTER_CONSTRUCTOR(LifeCycleCostUsePriceEscalation);
    REGISTER_CONSTRUCTOR(LightingDesignDay);
    REGISTER_CONSTRUCTOR(LightingSimulationControl);
    REGISTER_CONSTRUCTOR(LightingSimulationZone);
    REGISTER_CONSTRUCTOR(Lights);
    REGISTER_CONSTRUCTOR(LightsDefinition);
    REGISTER_CONSTRUCTOR(LoadingIndex);
    REGISTER_CONSTRUCTOR(Luminaire);
    REGISTER_CONSTRUCTOR(LuminaireDefinition);
    REGISTER_CONSTRUCTOR(MaterialPropertyGlazingSpectralData);
    REGISTER_CONSTRUCTOR(MaterialPropertyMoisturePenetrationDepthSettings);
    REGISTER_CONSTRUCTOR(MaterialPropertyPhaseChange);
    REGISTER_CONSTRUCTOR(MaterialPropertyPhaseChangeHysteresis);
    REGISTER_CONSTRUCTOR(MasslessOpaqueMaterial);
    REGISTER_CONSTRUCTOR(MeterCustom);
    REGISTER_CONSTRUCTOR(MeterCustomDecrement);
    REGISTER_CONSTRUCTOR(ModelObjectList);
    REGISTER_CONSTRUCTOR(Node);
    REGISTER_CONSTRUCTOR(OtherEquipment);
    REGISTER_CONSTRUCTOR(OtherEquipmentDefinition);
    REGISTER_CONSTRUCTOR(OutputControlFiles);
    REGISTER_CONSTRUCTOR(OutputControlReportingTolerances);
    REGISTER_CONSTRUCTOR(OutputControlTableStyle);
    REGISTER_CONSTRUCTOR(OutputControlTimestamp);
    REGISTER_CONSTRUCTOR(OutputDebuggingData);
    REGISTER_CONSTRUCTOR(OutputDiagnostics);
    REGISTER_CONSTRUCTOR(OutputEnergyManagementSystem);
    REGISTER_CONSTRUCTOR(OutputJSON);
    REGISTER_CONSTRUCTOR(OutputSQLite);
    REGISTER_CONSTRUCTOR(OutputEnvironmentalImpactFactors);
    REGISTER_CONSTRUCTOR(OutputSchedules);
    REGISTER_CONSTRUCTOR(OutputConstructions);
    REGISTER_CONSTRUCTOR(EnvironmentalImpactFactors);
    REGISTER_CONSTRUCTOR(FuelFactors);
    REGISTER_CONSTRUCTOR(OutputMeter);
    REGISTER_CONSTRUCTOR(OutputVariable);
    REGISTER_CONSTRUCTOR(OutputTableSummaryReports);
    REGISTER_CONSTRUCTOR(OutsideSurfaceConvectionAlgorithm);
    REGISTER_CONSTRUCTOR(People);
    REGISTER_CONSTRUCTOR(PeopleDefinition);
    REGISTER_CONSTRUCTOR(PerformancePrecisionTradeoffs);
    REGISTER_CONSTRUCTOR(PhotovoltaicPerformanceEquivalentOneDiode);
    REGISTER_CONSTRUCTOR(PhotovoltaicPerformanceSimple);
    REGISTER_CONSTRUCTOR(PhotovoltaicPerformanceSandia);
    REGISTER_CONSTRUCTOR(PipeAdiabatic);
    REGISTER_CONSTRUCTOR(PipeIndoor);
    REGISTER_CONSTRUCTOR(PipeOutdoor);
    REGISTER_CONSTRUCTOR(PlantComponentTemperatureSource);
    REGISTER_CONSTRUCTOR(PlantComponentUserDefined);
    REGISTER_CONSTRUCTOR(PlantLoop);
    REGISTER_CONSTRUCTOR(PlantEquipmentOperationCoolingLoad);
    REGISTER_CONSTRUCTOR(PlantEquipmentOperationHeatingLoad);
    REGISTER_CONSTRUCTOR(PlantEquipmentOperationOutdoorDryBulb);
    REGISTER_CONSTRUCTOR(PlantEquipmentOperationOutdoorWetBulb);
    REGISTER_CONSTRUCTOR(PlantEquipmentOperationOutdoorDewpoint);
    REGISTER_CONSTRUCTOR(PlantEquipmentOperationOutdoorRelativeHumidity);
    REGISTER_CONSTRUCTOR(PlantEquipmentOperationOutdoorDryBulbDifference);
    REGISTER_CONSTRUCTOR(PlantEquipmentOperationOutdoorWetBulbDifference);
    REGISTER_CONSTRUCTOR(PlantEquipmentOperationOutdoorDewpointDifference);
    REGISTER_CONSTRUCTOR(PortList);
    REGISTER_CONSTRUCTOR(ProgramControl);
    REGISTER_CONSTRUCTOR(PumpConstantSpeed);
    REGISTER_CONSTRUCTOR(PumpVariableSpeed);
    REGISTER_CONSTRUCTOR(PythonPluginInstance);
    REGISTER_CONSTRUCTOR(PythonPluginVariable);
    REGISTER_CONSTRUCTOR(PythonPluginTrendVariable);
    REGISTER_CONSTRUCTOR(PythonPluginOutputVariable);
    REGISTER_CONSTRUCTOR(PythonPluginSearchPaths);
    REGISTER_CONSTRUCTOR(RadianceParameters);
    REGISTER_CONSTRUCTOR(RefractionExtinctionGlazing);
    REGISTER_CONSTRUCTOR(RefrigerationAirChiller);
    REGISTER_CONSTRUCTOR(RefrigerationCase);
    REGISTER_CONSTRUCTOR(RefrigerationCompressor);
    REGISTER_CONSTRUCTOR(RefrigerationCompressorRack);
    REGISTER_CONSTRUCTOR(RefrigerationCondenserAirCooled);
    REGISTER_CONSTRUCTOR(RefrigerationCondenserCascade);
    REGISTER_CONSTRUCTOR(RefrigerationCondenserEvaporativeCooled);
    REGISTER_CONSTRUCTOR(RefrigerationCondenserWaterCooled);
    REGISTER_CONSTRUCTOR(RefrigerationDefrostCycleParameters);
    REGISTER_CONSTRUCTOR(RefrigerationGasCoolerAirCooled);
    REGISTER_CONSTRUCTOR(RefrigerationSubcoolerMechanical);
    REGISTER_CONSTRUCTOR(RefrigerationSubcoolerLiquidSuction);
    REGISTER_CONSTRUCTOR(RefrigerationSecondarySystem);
    REGISTER_CONSTRUCTOR(RefrigerationSystem);
    REGISTER_CONSTRUCTOR(RefrigerationTranscriticalSystem);
    REGISTER_CONSTRUCTOR(RefrigerationWalkIn);
    REGISTER_CONSTRUCTOR(RefrigerationWalkInZoneBoundary);
    REGISTER_CONSTRUCTOR(RenderingColor);
    REGISTER_CONSTRUCTOR(RoofVegetation);
    REGISTER_CONSTRUCTOR(RunPeriod);
    REGISTER_CONSTRUCTOR(RunPeriodControlDaylightSavingTime);
    REGISTER_CONSTRUCTOR(RunPeriodControlSpecialDays);
    REGISTER_CONSTRUCTOR(ScheduleCompact);
    REGISTER_CONSTRUCTOR(ScheduleConstant);
    REGISTER_CONSTRUCTOR(ScheduleDay);
    REGISTER_CONSTRUCTOR(ScheduleFile);
    REGISTER_CONSTRUCTOR(ScheduleFixedInterval);
    REGISTER_CONSTRUCTOR(ScheduleTypeLimits);
    REGISTER_CONSTRUCTOR(ScheduleVariableInterval);
    REGISTER_CONSTRUCTOR(ScheduleRule);
    REGISTER_CONSTRUCTOR(ScheduleRuleset);
    REGISTER_CONSTRUCTOR(ScheduleWeek);
    REGISTER_CONSTRUCTOR(ScheduleYear);
    REGISTER_CONSTRUCTOR(Screen);
    REGISTER_CONSTRUCTOR(SetpointManagerColdest);
    REGISTER_CONSTRUCTOR(SetpointManagerFollowGroundTemperature);
    REGISTER_CONSTRUCTOR(SetpointManagerFollowOutdoorAirTemperature);
    REGISTER_CONSTRUCTOR(SetpointManagerFollowSystemNodeTemperature);
    REGISTER_CONSTRUCTOR(SetpointManagerMixedAir);
    REGISTER_CONSTRUCTOR(SetpointManagerMultiZoneCoolingAverage);
    REGISTER_CONSTRUCTOR(SetpointManagerMultiZoneHeatingAverage);
    REGISTER_CONSTRUCTOR(SetpointManagerMultiZoneHumidityMaximum);
    REGISTER_CONSTRUCTOR(SetpointManagerMultiZoneHumidityMinimum);
    REGISTER_CONSTRUCTOR(SetpointManagerMultiZoneMaximumHumidityAverage);
    REGISTER_CONSTRUCTOR(SetpointManagerMultiZoneMinimumHumidityAverage);
    REGISTER_CONSTRUCTOR(SetpointManagerOutdoorAirPretreat);
    REGISTER_CONSTRUCTOR(SetpointManagerOutdoorAirReset);
    REGISTER_CONSTRUCTOR(SetpointManagerScheduled);
    REGISTER_CONSTRUCTOR(SetpointManagerScheduledDualSetpoint);
    REGISTER_CONSTRUCTOR(SetpointManagerSingleZoneCooling);
    REGISTER_CONSTRUCTOR(SetpointManagerSingleZoneHeating);
    REGISTER_CONSTRUCTOR(SetpointManagerSingleZoneHumidityMaximum);
    REGISTER_CONSTRUCTOR(SetpointManagerSingleZoneHumidityMinimum);
    REGISTER_CONSTRUCTOR(SetpointManagerSingleZoneOneStageCooling);
    REGISTER_CONSTRUCTOR(SetpointManagerSingleZoneOneStageHeating);
    REGISTER_CONSTRUCTOR(SetpointManagerSingleZoneReheat);
    REGISTER_CONSTRUCTOR(SetpointManagerWarmest);
    REGISTER_CONSTRUCTOR(SetpointManagerWarmestTemperatureFlow);
    REGISTER_CONSTRUCTOR(SetpointManagerSystemNodeResetHumidity);
    REGISTER_CONSTRUCTOR(SetpointManagerSystemNodeResetTemperature);
    REGISTER_CONSTRUCTOR(Shade);
    REGISTER_CONSTRUCTOR(ShadingControl);
    REGISTER_CONSTRUCTOR(ShadingSurface);
    REGISTER_CONSTRUCTOR(ShadingSurfaceGroup);
    REGISTER_CONSTRUCTOR(ShadowCalculation);
    REGISTER_CONSTRUCTOR(SimpleGlazing);
    REGISTER_CONSTRUCTOR(SimulationControl);
    REGISTER_CONSTRUCTOR(Site);
    REGISTER_CONSTRUCTOR(SiteGroundReflectance);
    REGISTER_CONSTRUCTOR(SiteGroundTemperatureBuildingSurface);
    REGISTER_CONSTRUCTOR(SiteGroundTemperatureDeep);
    REGISTER_CONSTRUCTOR(SiteGroundTemperatureShallow);
    REGISTER_CONSTRUCTOR(SiteGroundTemperatureFCfactorMethod);
    REGISTER_CONSTRUCTOR(SiteGroundTemperatureUndisturbedKusudaAchenbach);
    REGISTER_CONSTRUCTOR(SiteGroundTemperatureUndisturbedXing);
    REGISTER_CONSTRUCTOR(SiteWaterMainsTemperature);
    REGISTER_CONSTRUCTOR(SizingParameters);
    REGISTER_CONSTRUCTOR(SizingPlant);
    REGISTER_CONSTRUCTOR(SizingSystem);
    REGISTER_CONSTRUCTOR(SizingZone);
    REGISTER_CONSTRUCTOR(SkyTemperature);
    REGISTER_CONSTRUCTOR(SolarCollectorFlatPlatePhotovoltaicThermal);
    REGISTER_CONSTRUCTOR(SolarCollectorFlatPlateWater);
    REGISTER_CONSTRUCTOR(SolarCollectorIntegralCollectorStorage);
    REGISTER_CONSTRUCTOR(SolarCollectorPerformanceFlatPlate);
    REGISTER_CONSTRUCTOR(SolarCollectorPerformanceIntegralCollectorStorage);
    REGISTER_CONSTRUCTOR(SolarCollectorPerformancePhotovoltaicThermalBIPVT);
    REGISTER_CONSTRUCTOR(SolarCollectorPerformancePhotovoltaicThermalSimple);
    REGISTER_CONSTRUCTOR(Space);
    REGISTER_CONSTRUCTOR(SpaceInfiltrationDesignFlowRate);
    REGISTER_CONSTRUCTOR(SpaceInfiltrationEffectiveLeakageArea);
    REGISTER_CONSTRUCTOR(SpaceInfiltrationFlowCoefficient);
    REGISTER_CONSTRUCTOR(SpaceType);
    REGISTER_CONSTRUCTOR(StandardGlazing);
    REGISTER_CONSTRUCTOR(StandardOpaqueMaterial);
    REGISTER_CONSTRUCTOR(StandardsInformationConstruction);
    REGISTER_CONSTRUCTOR(StandardsInformationMaterial);
    REGISTER_CONSTRUCTOR(SteamEquipment);
    REGISTER_CONSTRUCTOR(SteamEquipmentDefinition);
    REGISTER_CONSTRUCTOR(SubSurface);
    REGISTER_CONSTRUCTOR(Surface);
    REGISTER_CONSTRUCTOR(SurfaceControlMovableInsulation);
    REGISTER_CONSTRUCTOR(SurfacePropertyConvectionCoefficients);
    REGISTER_CONSTRUCTOR(SurfacePropertyConvectionCoefficientsMultipleSurface);
    REGISTER_CONSTRUCTOR(SurfacePropertyExposedFoundationPerimeter);
    REGISTER_CONSTRUCTOR(SurfacePropertyLocalEnvironment);
    REGISTER_CONSTRUCTOR(SurfacePropertyOtherSideCoefficients);
    REGISTER_CONSTRUCTOR(SurfacePropertyOtherSideConditionsModel);
    REGISTER_CONSTRUCTOR(SurfacePropertySurroundingSurfaces);
    REGISTER_CONSTRUCTOR(SurfacePropertyGroundSurfaces);
    REGISTER_CONSTRUCTOR(SurfacePropertyIncidentSolarMultiplier);
    REGISTER_CONSTRUCTOR(SwimmingPoolIndoor);
#if defined(_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable : 4996)
#elif (defined(__GNUC__))
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
    REGISTER_CONSTRUCTOR(TableMultiVariableLookup);
#if defined(_MSC_VER)
#  pragma warning(pop)
#elif (defined(__GNUC__))
#  pragma GCC diagnostic pop
#endif
    REGISTER_CONSTRUCTOR(TableLookup);
    REGISTER_CONSTRUCTOR(TableIndependentVariable);
    REGISTER_CONSTRUCTOR(TemperingValve);
    REGISTER_CONSTRUCTOR(ThermochromicGlazing);
    REGISTER_CONSTRUCTOR(ThermostatSetpointDualSetpoint);
    REGISTER_CONSTRUCTOR(ThermalZone);
    REGISTER_CONSTRUCTOR(ThermalStorageChilledWaterStratified);
    REGISTER_CONSTRUCTOR(ThermalStorageIceDetailed);
    REGISTER_CONSTRUCTOR(Timestep);
    REGISTER_CONSTRUCTOR(UnitarySystemPerformanceMultispeed);
    REGISTER_CONSTRUCTOR(UtilityBill);

    // TODO: once UtilityCost objects are wrapped
    // REGISTER_CONSTRUCTOR(UtilityCost_Charge_Block);
    // REGISTER_CONSTRUCTOR(UtilityCost_Charge_Simple);
    // REGISTER_CONSTRUCTOR(UtilityCost_Computation);
    // REGISTER_CONSTRUCTOR(UtilityCost_Qualify);
    // REGISTER_CONSTRUCTOR(UtilityCost_Ratchet);
    // REGISTER_CONSTRUCTOR(UtilityCost_Tariff);
    // REGISTER_CONSTRUCTOR(UtilityCost_Variable);

    REGISTER_CONSTRUCTOR(Version);
    REGISTER_CONSTRUCTOR(WaterHeaterMixed);
    REGISTER_CONSTRUCTOR(WaterHeaterHeatPump);
    REGISTER_CONSTRUCTOR(WaterHeaterHeatPumpWrappedCondenser);
    REGISTER_CONSTRUCTOR(WaterHeaterSizing);
    REGISTER_CONSTRUCTOR(WaterHeaterStratified);
    REGISTER_CONSTRUCTOR(WaterUseConnections);
    REGISTER_CONSTRUCTOR(WaterUseEquipment);
    REGISTER_CONSTRUCTOR(WaterUseEquipmentDefinition);
    REGISTER_CONSTRUCTOR(WeatherFile);
    REGISTER_CONSTRUCTOR(WeatherFileConditionType);
    REGISTER_CONSTRUCTOR(WeatherFileDays);
    REGISTER_CONSTRUCTOR(WindowDataFile);
    REGISTER_CONSTRUCTOR(WindowPropertyFrameAndDivider);
    REGISTER_CONSTRUCTOR(YearDescription);
    REGISTER_CONSTRUCTOR(ZoneAirContaminantBalance);
    REGISTER_CONSTRUCTOR(ZoneAirHeatBalanceAlgorithm);
    REGISTER_CONSTRUCTOR(ZoneAirMassFlowConservation);
    REGISTER_CONSTRUCTOR(ZoneCapacitanceMultiplierResearchSpecial);
    REGISTER_CONSTRUCTOR(ZoneControlContaminantController);
    REGISTER_CONSTRUCTOR(ZoneControlHumidistat);
    REGISTER_CONSTRUCTOR(ZoneControlThermostatStagedDualSetpoint);
    REGISTER_CONSTRUCTOR(ZoneHVACEquipmentList);
    REGISTER_CONSTRUCTOR(ZoneHVACEvaporativeCoolerUnit);
    REGISTER_CONSTRUCTOR(ZoneHVACBaseboardConvectiveElectric);
    REGISTER_CONSTRUCTOR(ZoneHVACBaseboardConvectiveWater);
    REGISTER_CONSTRUCTOR(ZoneHVACBaseboardRadiantConvectiveElectric);
    REGISTER_CONSTRUCTOR(ZoneHVACBaseboardRadiantConvectiveWater);
    REGISTER_CONSTRUCTOR(ZoneHVACCoolingPanelRadiantConvectiveWater);
    REGISTER_CONSTRUCTOR(ZoneHVACIdealLoadsAirSystem);
    REGISTER_CONSTRUCTOR(ZoneHVACFourPipeFanCoil);
    REGISTER_CONSTRUCTOR(ZoneHVACHighTemperatureRadiant);
    REGISTER_CONSTRUCTOR(ZoneHVACLowTemperatureRadiantElectric);
    REGISTER_CONSTRUCTOR(ZoneHVACLowTempRadiantConstFlow);
    REGISTER_CONSTRUCTOR(ZoneHVACLowTempRadiantVarFlow);
    REGISTER_CONSTRUCTOR(ZoneHVACPackagedTerminalHeatPump);
    REGISTER_CONSTRUCTOR(ZoneHVACPackagedTerminalAirConditioner);
    REGISTER_CONSTRUCTOR(ZoneHVACTerminalUnitVariableRefrigerantFlow);
    REGISTER_CONSTRUCTOR(ZoneHVACWaterToAirHeatPump);
    REGISTER_CONSTRUCTOR(ZoneHVACUnitHeater);
    REGISTER_CONSTRUCTOR(ZoneHVACDehumidifierDX);
    REGISTER_CONSTRUCTOR(ZoneHVACEnergyRecoveryVentilatorController);
    REGISTER_CONSTRUCTOR(ZoneHVACEnergyRecoveryVentilator);
    REGISTER_CONSTRUCTOR(ZoneHVACUnitVentilator);
    REGISTER_CONSTRUCTOR(ZoneMixing);
    REGISTER_CONSTRUCTOR(ZonePropertyUserViewFactorsBySurfaceName);
    REGISTER_CONSTRUCTOR(ZoneVentilationDesignFlowRate);
    REGISTER_CONSTRUCTOR(ZoneVentilationWindandStackOpenArea);

#define REGISTER_COPYCONSTRUCTORS(_className)                                                                                          \
  m_copyMap[_className::iddObjectType()] = [](openstudio::model::detail::Model_Impl* m,                                                \
                                              const std::shared_ptr<openstudio::detail::WorkspaceObject_Impl>& ptr, bool keepHandle) { \
    if (dynamic_pointer_cast<_className##_Impl>(ptr)) {                                                                                \
      return std::make_shared<_className##_Impl>(*dynamic_pointer_cast<_className##_Impl>(ptr), m, keepHandle);                        \
    } else {                                                                                                                           \
      OS_ASSERT(!dynamic_pointer_cast<openstudio::model::detail::ModelObject_Impl>(ptr));                                              \
      return std::make_shared<_className##_Impl>(*ptr, m, keepHandle);                                                                 \
    }                                                                                                                                  \
  };
    REGISTER_COPYCONSTRUCTORS(AdditionalProperties);
    REGISTER_COPYCONSTRUCTORS(AirConditionerVariableRefrigerantFlow);
    REGISTER_COPYCONSTRUCTORS(AirConditionerVariableRefrigerantFlowFluidTemperatureControl);
    REGISTER_COPYCONSTRUCTORS(AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR);
    REGISTER_COPYCONSTRUCTORS(AirflowNetworkConstantPressureDrop);
    REGISTER_COPYCONSTRUCTORS(AirflowNetworkCrack);
    REGISTER_COPYCONSTRUCTORS(AirflowNetworkDetailedOpening);
    REGISTER_COPYCONSTRUCTORS(AirflowNetworkDistributionLinkage);
    REGISTER_COPYCONSTRUCTORS(AirflowNetworkDistributionNode);
    REGISTER_COPYCONSTRUCTORS(AirflowNetworkDuct);
    REGISTER_COPYCONSTRUCTORS(AirflowNetworkDuctViewFactors);
    REGISTER_COPYCONSTRUCTORS(AirflowNetworkEffectiveLeakageArea);
    REGISTER_COPYCONSTRUCTORS(AirflowNetworkEquivalentDuct);
    REGISTER_COPYCONSTRUCTORS(AirflowNetworkExternalNode);
    REGISTER_COPYCONSTRUCTORS(AirflowNetworkFan);
    REGISTER_COPYCONSTRUCTORS(AirflowNetworkHorizontalOpening);
    REGISTER_COPYCONSTRUCTORS(AirflowNetworkLeakageRatio);
    REGISTER_COPYCONSTRUCTORS(AirflowNetworkOccupantVentilationControl);
    REGISTER_COPYCONSTRUCTORS(AirflowNetworkOutdoorAirflow);
    REGISTER_COPYCONSTRUCTORS(AirflowNetworkReferenceCrackConditions);
    REGISTER_COPYCONSTRUCTORS(AirflowNetworkSimpleOpening);
    REGISTER_COPYCONSTRUCTORS(AirflowNetworkSimulationControl);
    REGISTER_COPYCONSTRUCTORS(AirflowNetworkSpecifiedFlowRate);
    REGISTER_COPYCONSTRUCTORS(AirflowNetworkSurface);
    REGISTER_COPYCONSTRUCTORS(AirflowNetworkZone);
    REGISTER_COPYCONSTRUCTORS(AirflowNetworkZoneExhaustFan);
    REGISTER_COPYCONSTRUCTORS(AirGap);
    REGISTER_COPYCONSTRUCTORS(AirLoopHVAC);
    REGISTER_COPYCONSTRUCTORS(AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass);
    REGISTER_COPYCONSTRUCTORS(AirLoopHVACUnitaryHeatPumpAirToAir);
    REGISTER_COPYCONSTRUCTORS(AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed);
    REGISTER_COPYCONSTRUCTORS(AirLoopHVACOutdoorAirSystem);
    REGISTER_COPYCONSTRUCTORS(AirLoopHVACDedicatedOutdoorAirSystem);
    REGISTER_COPYCONSTRUCTORS(AirLoopHVACReturnPlenum);
    REGISTER_COPYCONSTRUCTORS(AirLoopHVACSupplyPlenum);
    REGISTER_COPYCONSTRUCTORS(AirLoopHVACUnitarySystem);
    REGISTER_COPYCONSTRUCTORS(AirLoopHVACZoneMixer);
    REGISTER_COPYCONSTRUCTORS(AirLoopHVACZoneSplitter);
    REGISTER_COPYCONSTRUCTORS(AirTerminalDualDuctConstantVolume);
    REGISTER_COPYCONSTRUCTORS(AirTerminalDualDuctVAV);
    REGISTER_COPYCONSTRUCTORS(AirTerminalDualDuctVAVOutdoorAir);
    REGISTER_COPYCONSTRUCTORS(AirTerminalSingleDuctInletSideMixer);
    REGISTER_COPYCONSTRUCTORS(AirTerminalSingleDuctConstantVolumeCooledBeam);
    REGISTER_COPYCONSTRUCTORS(AirTerminalSingleDuctConstantVolumeFourPipeInduction);
    REGISTER_COPYCONSTRUCTORS(AirTerminalSingleDuctConstantVolumeFourPipeBeam);
    REGISTER_COPYCONSTRUCTORS(AirTerminalSingleDuctConstantVolumeReheat);
    REGISTER_COPYCONSTRUCTORS(AirTerminalSingleDuctParallelPIUReheat);
    REGISTER_COPYCONSTRUCTORS(AirTerminalSingleDuctSeriesPIUReheat);
    REGISTER_COPYCONSTRUCTORS(AirTerminalSingleDuctConstantVolumeNoReheat);
    REGISTER_COPYCONSTRUCTORS(AirTerminalSingleDuctVAVReheat);
    REGISTER_COPYCONSTRUCTORS(AirTerminalSingleDuctVAVNoReheat);
    REGISTER_COPYCONSTRUCTORS(AirTerminalSingleDuctVAVHeatAndCoolNoReheat);
    REGISTER_COPYCONSTRUCTORS(AirTerminalSingleDuctVAVHeatAndCoolReheat);
    REGISTER_COPYCONSTRUCTORS(AvailabilityManagerAssignmentList);
    REGISTER_COPYCONSTRUCTORS(AvailabilityManagerNightCycle);
    REGISTER_COPYCONSTRUCTORS(AvailabilityManagerOptimumStart);
    REGISTER_COPYCONSTRUCTORS(AvailabilityManagerHybridVentilation);
    REGISTER_COPYCONSTRUCTORS(AvailabilityManagerDifferentialThermostat);
    REGISTER_COPYCONSTRUCTORS(AvailabilityManagerNightVentilation);
    REGISTER_COPYCONSTRUCTORS(AvailabilityManagerHighTemperatureTurnOn);
    REGISTER_COPYCONSTRUCTORS(AvailabilityManagerHighTemperatureTurnOff);
    REGISTER_COPYCONSTRUCTORS(AvailabilityManagerLowTemperatureTurnOn);
    REGISTER_COPYCONSTRUCTORS(AvailabilityManagerLowTemperatureTurnOff);
    REGISTER_COPYCONSTRUCTORS(AvailabilityManagerScheduled);
    REGISTER_COPYCONSTRUCTORS(AvailabilityManagerScheduledOn);
    REGISTER_COPYCONSTRUCTORS(AvailabilityManagerScheduledOff);
    REGISTER_COPYCONSTRUCTORS(Blind);
    REGISTER_COPYCONSTRUCTORS(BoilerHotWater);
    REGISTER_COPYCONSTRUCTORS(BoilerSteam);
    REGISTER_COPYCONSTRUCTORS(Building);
    REGISTER_COPYCONSTRUCTORS(BuildingStory);
    REGISTER_COPYCONSTRUCTORS(BuildingUnit);
    REGISTER_COPYCONSTRUCTORS(CentralHeatPumpSystem);
    REGISTER_COPYCONSTRUCTORS(CentralHeatPumpSystemModule);
    REGISTER_COPYCONSTRUCTORS(CFactorUndergroundWallConstruction);
    REGISTER_COPYCONSTRUCTORS(ClimateZones);
    REGISTER_COPYCONSTRUCTORS(ChillerAbsorption);
    REGISTER_COPYCONSTRUCTORS(ChillerAbsorptionIndirect);
    REGISTER_COPYCONSTRUCTORS(ChillerElectricASHRAE205);
    REGISTER_COPYCONSTRUCTORS(ChillerElectricEIR);
    REGISTER_COPYCONSTRUCTORS(ChillerElectricReformulatedEIR);
    REGISTER_COPYCONSTRUCTORS(ChillerHeaterPerformanceElectricEIR);
    REGISTER_COPYCONSTRUCTORS(CoilCoolingCooledBeam);
    REGISTER_COPYCONSTRUCTORS(CoilCoolingDX);
    REGISTER_COPYCONSTRUCTORS(CoilCoolingDXCurveFitPerformance);
    REGISTER_COPYCONSTRUCTORS(CoilCoolingDXCurveFitOperatingMode);
    REGISTER_COPYCONSTRUCTORS(CoilCoolingDXCurveFitSpeed);
    REGISTER_COPYCONSTRUCTORS(CoilCoolingDXMultiSpeed);
    REGISTER_COPYCONSTRUCTORS(CoilCoolingDXMultiSpeedStageData);
    REGISTER_COPYCONSTRUCTORS(CoilCoolingDXSingleSpeed);
    REGISTER_COPYCONSTRUCTORS(CoilCoolingDXSingleSpeedThermalStorage);
    REGISTER_COPYCONSTRUCTORS(CoilCoolingDXTwoSpeed);
    REGISTER_COPYCONSTRUCTORS(CoilCoolingDXTwoStageWithHumidityControlMode);
    REGISTER_COPYCONSTRUCTORS(CoilCoolingDXVariableRefrigerantFlow);
    REGISTER_COPYCONSTRUCTORS(CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl);
    REGISTER_COPYCONSTRUCTORS(CoilCoolingLowTempRadiantConstFlow);
    REGISTER_COPYCONSTRUCTORS(CoilCoolingLowTempRadiantVarFlow);
    REGISTER_COPYCONSTRUCTORS(CoilCoolingDXVariableSpeed);
    REGISTER_COPYCONSTRUCTORS(CoilCoolingDXVariableSpeedSpeedData);
    REGISTER_COPYCONSTRUCTORS(CoilCoolingFourPipeBeam);
    REGISTER_COPYCONSTRUCTORS(CoilCoolingWater);
    REGISTER_COPYCONSTRUCTORS(CoilCoolingWaterToAirHeatPumpEquationFit);
    REGISTER_COPYCONSTRUCTORS(CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit);
    REGISTER_COPYCONSTRUCTORS(CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData);
    REGISTER_COPYCONSTRUCTORS(CoilCoolingWaterPanelRadiant);
    REGISTER_COPYCONSTRUCTORS(CoilHeatingDesuperheater);
    REGISTER_COPYCONSTRUCTORS(CoilHeatingDXMultiSpeed);
    REGISTER_COPYCONSTRUCTORS(CoilHeatingDXMultiSpeedStageData);
    REGISTER_COPYCONSTRUCTORS(CoilHeatingDXSingleSpeed);
    REGISTER_COPYCONSTRUCTORS(CoilHeatingDXVariableRefrigerantFlow);
    REGISTER_COPYCONSTRUCTORS(CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl);
    REGISTER_COPYCONSTRUCTORS(CoilHeatingElectric);
    REGISTER_COPYCONSTRUCTORS(CoilHeatingElectricMultiStage);
    REGISTER_COPYCONSTRUCTORS(CoilHeatingElectricMultiStageStageData);
    REGISTER_COPYCONSTRUCTORS(CoilHeatingFourPipeBeam);
    REGISTER_COPYCONSTRUCTORS(CoilHeatingGas);
    REGISTER_COPYCONSTRUCTORS(CoilHeatingGasMultiStage);
    REGISTER_COPYCONSTRUCTORS(CoilHeatingGasMultiStageStageData);
    REGISTER_COPYCONSTRUCTORS(CoilHeatingLowTempRadiantConstFlow);
    REGISTER_COPYCONSTRUCTORS(CoilHeatingLowTempRadiantVarFlow);
    REGISTER_COPYCONSTRUCTORS(CoilHeatingDXVariableSpeed);
    REGISTER_COPYCONSTRUCTORS(CoilHeatingDXVariableSpeedSpeedData);
    REGISTER_COPYCONSTRUCTORS(CoilHeatingWater);
    REGISTER_COPYCONSTRUCTORS(CoilHeatingWaterToAirHeatPumpEquationFit);
    REGISTER_COPYCONSTRUCTORS(CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit);
    REGISTER_COPYCONSTRUCTORS(CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData);
    REGISTER_COPYCONSTRUCTORS(CoilHeatingWaterBaseboard);
    REGISTER_COPYCONSTRUCTORS(CoilHeatingWaterBaseboardRadiant);
    REGISTER_COPYCONSTRUCTORS(CoilPerformanceDXCooling);
    REGISTER_COPYCONSTRUCTORS(CoilSystemCoolingWaterHeatExchangerAssisted);
    REGISTER_COPYCONSTRUCTORS(CoilSystemCoolingDXHeatExchangerAssisted);
    REGISTER_COPYCONSTRUCTORS(CoilSystemIntegratedHeatPumpAirSource);
    REGISTER_COPYCONSTRUCTORS(CoilUserDefined);
    REGISTER_COPYCONSTRUCTORS(CoilWaterHeatingAirToWaterHeatPump);
    REGISTER_COPYCONSTRUCTORS(CoilWaterHeatingAirToWaterHeatPumpVariableSpeed);
    REGISTER_COPYCONSTRUCTORS(CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData);
    REGISTER_COPYCONSTRUCTORS(CoilWaterHeatingAirToWaterHeatPumpWrapped);
    REGISTER_COPYCONSTRUCTORS(CoilWaterHeatingDesuperheater);
    REGISTER_COPYCONSTRUCTORS(ComponentCostAdjustments);
    REGISTER_COPYCONSTRUCTORS(ComponentData);
    REGISTER_COPYCONSTRUCTORS(Connection);
    REGISTER_COPYCONSTRUCTORS(ConnectorMixer);
    REGISTER_COPYCONSTRUCTORS(ConnectorSplitter);
    REGISTER_COPYCONSTRUCTORS(Construction);
    REGISTER_COPYCONSTRUCTORS(ConstructionAirBoundary);
    REGISTER_COPYCONSTRUCTORS(ConstructionWithInternalSource);
    REGISTER_COPYCONSTRUCTORS(ControllerMechanicalVentilation);
    REGISTER_COPYCONSTRUCTORS(ControllerOutdoorAir);
    REGISTER_COPYCONSTRUCTORS(ControllerWaterCoil);
    REGISTER_COPYCONSTRUCTORS(ConvergenceLimits);
    REGISTER_COPYCONSTRUCTORS(CoolingTowerPerformanceCoolTools);
    REGISTER_COPYCONSTRUCTORS(CoolingTowerPerformanceYorkCalc);
    REGISTER_COPYCONSTRUCTORS(CoolingTowerSingleSpeed);
    REGISTER_COPYCONSTRUCTORS(CoolingTowerTwoSpeed);
    REGISTER_COPYCONSTRUCTORS(CoolingTowerVariableSpeed);
    REGISTER_COPYCONSTRUCTORS(CurrencyType);
    REGISTER_COPYCONSTRUCTORS(CurveBicubic);
    REGISTER_COPYCONSTRUCTORS(CurveBiquadratic);
    REGISTER_COPYCONSTRUCTORS(CurveCubic);
    REGISTER_COPYCONSTRUCTORS(CurveDoubleExponentialDecay);
    REGISTER_COPYCONSTRUCTORS(CurveExponent);
    REGISTER_COPYCONSTRUCTORS(CurveExponentialDecay);
    REGISTER_COPYCONSTRUCTORS(CurveExponentialSkewNormal);
    REGISTER_COPYCONSTRUCTORS(CurveFanPressureRise);
    REGISTER_COPYCONSTRUCTORS(CurveFunctionalPressureDrop);
    REGISTER_COPYCONSTRUCTORS(CurveLinear);
    REGISTER_COPYCONSTRUCTORS(CurveQuadLinear);
    REGISTER_COPYCONSTRUCTORS(CurveQuintLinear);
    REGISTER_COPYCONSTRUCTORS(CurveQuadratic);
    REGISTER_COPYCONSTRUCTORS(CurveQuadraticLinear);
    REGISTER_COPYCONSTRUCTORS(CurveQuartic);
    REGISTER_COPYCONSTRUCTORS(CurveRectangularHyperbola1);
    REGISTER_COPYCONSTRUCTORS(CurveRectangularHyperbola2);
    REGISTER_COPYCONSTRUCTORS(CurveSigmoid);
    REGISTER_COPYCONSTRUCTORS(CurveTriquadratic);
    REGISTER_COPYCONSTRUCTORS(DaylightingControl);
    REGISTER_COPYCONSTRUCTORS(DaylightingDeviceShelf);
    REGISTER_COPYCONSTRUCTORS(DaylightingDeviceTubular);
    REGISTER_COPYCONSTRUCTORS(DaylightingDeviceLightWell);
    REGISTER_COPYCONSTRUCTORS(DaylightRedirectionDevice);
    REGISTER_COPYCONSTRUCTORS(DefaultConstructionSet);
    REGISTER_COPYCONSTRUCTORS(DefaultScheduleSet);
    REGISTER_COPYCONSTRUCTORS(DefaultSubSurfaceConstructions);
    REGISTER_COPYCONSTRUCTORS(DefaultSurfaceConstructions);
    REGISTER_COPYCONSTRUCTORS(DesignDay);
    REGISTER_COPYCONSTRUCTORS(DesignSpecificationOutdoorAir);
    REGISTER_COPYCONSTRUCTORS(DesignSpecificationZoneAirDistribution);
    REGISTER_COPYCONSTRUCTORS(DistrictCooling);
    REGISTER_COPYCONSTRUCTORS(DistrictHeatingWater);
    REGISTER_COPYCONSTRUCTORS(DistrictHeatingSteam);
    REGISTER_COPYCONSTRUCTORS(Duct);
    REGISTER_COPYCONSTRUCTORS(ElectricEquipment);
    REGISTER_COPYCONSTRUCTORS(ElectricEquipmentDefinition);
    REGISTER_COPYCONSTRUCTORS(ElectricEquipmentITEAirCooled);
    REGISTER_COPYCONSTRUCTORS(ElectricEquipmentITEAirCooledDefinition);
    REGISTER_COPYCONSTRUCTORS(ElectricLoadCenterDistribution);
    REGISTER_COPYCONSTRUCTORS(ElectricLoadCenterInverterLookUpTable);
    REGISTER_COPYCONSTRUCTORS(ElectricLoadCenterInverterSimple);
    REGISTER_COPYCONSTRUCTORS(ElectricLoadCenterInverterPVWatts);
    REGISTER_COPYCONSTRUCTORS(ElectricLoadCenterStorageSimple);
    REGISTER_COPYCONSTRUCTORS(ElectricLoadCenterStorageConverter);
    REGISTER_COPYCONSTRUCTORS(ElectricLoadCenterStorageLiIonNMCBattery);
    REGISTER_COPYCONSTRUCTORS(ElectricLoadCenterTransformer);
    REGISTER_COPYCONSTRUCTORS(EnergyManagementSystemActuator);
    REGISTER_COPYCONSTRUCTORS(EnergyManagementSystemConstructionIndexVariable);
    REGISTER_COPYCONSTRUCTORS(EnergyManagementSystemCurveOrTableIndexVariable);
    REGISTER_COPYCONSTRUCTORS(EnergyManagementSystemGlobalVariable);
    REGISTER_COPYCONSTRUCTORS(EnergyManagementSystemInternalVariable);
    REGISTER_COPYCONSTRUCTORS(EnergyManagementSystemMeteredOutputVariable);
    REGISTER_COPYCONSTRUCTORS(EnergyManagementSystemOutputVariable);
    REGISTER_COPYCONSTRUCTORS(EnergyManagementSystemProgram);
    REGISTER_COPYCONSTRUCTORS(EnergyManagementSystemProgramCallingManager);
    REGISTER_COPYCONSTRUCTORS(EnergyManagementSystemSensor);
    REGISTER_COPYCONSTRUCTORS(EnergyManagementSystemSubroutine);
    REGISTER_COPYCONSTRUCTORS(EnergyManagementSystemTrendVariable);
    REGISTER_COPYCONSTRUCTORS(EvaporativeCoolerDirectResearchSpecial);
    REGISTER_COPYCONSTRUCTORS(EvaporativeCoolerIndirectResearchSpecial);
    REGISTER_COPYCONSTRUCTORS(EvaporativeFluidCoolerSingleSpeed);
    REGISTER_COPYCONSTRUCTORS(EvaporativeFluidCoolerTwoSpeed);
    REGISTER_COPYCONSTRUCTORS(ExteriorLights);
    REGISTER_COPYCONSTRUCTORS(ExteriorLightsDefinition);
    REGISTER_COPYCONSTRUCTORS(ExteriorFuelEquipment);
    REGISTER_COPYCONSTRUCTORS(ExteriorFuelEquipmentDefinition);
    REGISTER_COPYCONSTRUCTORS(ExteriorWaterEquipment);
    REGISTER_COPYCONSTRUCTORS(ExteriorWaterEquipmentDefinition);
    REGISTER_COPYCONSTRUCTORS(ExternalFile);
    REGISTER_COPYCONSTRUCTORS(ExternalInterface);
    REGISTER_COPYCONSTRUCTORS(ExternalInterfaceActuator);
    REGISTER_COPYCONSTRUCTORS(ExternalInterfaceFunctionalMockupUnitExportFromVariable);
    REGISTER_COPYCONSTRUCTORS(ExternalInterfaceFunctionalMockupUnitExportToActuator);
    REGISTER_COPYCONSTRUCTORS(ExternalInterfaceFunctionalMockupUnitExportToSchedule);
    REGISTER_COPYCONSTRUCTORS(ExternalInterfaceFunctionalMockupUnitExportToVariable);
    REGISTER_COPYCONSTRUCTORS(ExternalInterfaceFunctionalMockupUnitImport);
    REGISTER_COPYCONSTRUCTORS(ExternalInterfaceFunctionalMockupUnitImportFromVariable);
    REGISTER_COPYCONSTRUCTORS(ExternalInterfaceFunctionalMockupUnitImportToActuator);
    REGISTER_COPYCONSTRUCTORS(ExternalInterfaceFunctionalMockupUnitImportToSchedule);
    REGISTER_COPYCONSTRUCTORS(ExternalInterfaceFunctionalMockupUnitImportToVariable);
    REGISTER_COPYCONSTRUCTORS(ExternalInterfaceSchedule);
    REGISTER_COPYCONSTRUCTORS(ExternalInterfaceVariable);
    REGISTER_COPYCONSTRUCTORS(Facility);
    REGISTER_COPYCONSTRUCTORS(FanComponentModel);
    REGISTER_COPYCONSTRUCTORS(FanConstantVolume);
    REGISTER_COPYCONSTRUCTORS(FanOnOff);
    REGISTER_COPYCONSTRUCTORS(FanSystemModel);
    REGISTER_COPYCONSTRUCTORS(FanVariableVolume);
    REGISTER_COPYCONSTRUCTORS(FanZoneExhaust);
    REGISTER_COPYCONSTRUCTORS(FFactorGroundFloorConstruction);
    REGISTER_COPYCONSTRUCTORS(FluidCoolerSingleSpeed);
    REGISTER_COPYCONSTRUCTORS(FluidCoolerTwoSpeed);
    REGISTER_COPYCONSTRUCTORS(FoundationKiva);
    REGISTER_COPYCONSTRUCTORS(FoundationKivaSettings);
    REGISTER_COPYCONSTRUCTORS(Gas);
    REGISTER_COPYCONSTRUCTORS(GasEquipment);
    REGISTER_COPYCONSTRUCTORS(GasEquipmentDefinition);
    REGISTER_COPYCONSTRUCTORS(GasMixture);
    REGISTER_COPYCONSTRUCTORS(GeneratorFuelCell);
    REGISTER_COPYCONSTRUCTORS(GeneratorFuelCellAirSupply);
    REGISTER_COPYCONSTRUCTORS(GeneratorFuelCellAuxiliaryHeater);
    REGISTER_COPYCONSTRUCTORS(GeneratorFuelCellElectricalStorage);
    REGISTER_COPYCONSTRUCTORS(GeneratorFuelCellExhaustGasToWaterHeatExchanger);
    REGISTER_COPYCONSTRUCTORS(GeneratorFuelCellInverter);
    REGISTER_COPYCONSTRUCTORS(GeneratorFuelCellPowerModule);
    REGISTER_COPYCONSTRUCTORS(GeneratorFuelCellStackCooler);
    REGISTER_COPYCONSTRUCTORS(GeneratorFuelCellWaterSupply);
    REGISTER_COPYCONSTRUCTORS(GeneratorFuelSupply);
    REGISTER_COPYCONSTRUCTORS(GeneratorMicroTurbine);
    REGISTER_COPYCONSTRUCTORS(GeneratorMicroTurbineHeatRecovery);
    REGISTER_COPYCONSTRUCTORS(GeneratorPhotovoltaic);
    REGISTER_COPYCONSTRUCTORS(GeneratorPVWatts);
    REGISTER_COPYCONSTRUCTORS(GeneratorWindTurbine);
    REGISTER_COPYCONSTRUCTORS(GlareSensor);
    REGISTER_COPYCONSTRUCTORS(GroundHeatExchangerHorizontalTrench);
    REGISTER_COPYCONSTRUCTORS(GroundHeatExchangerVertical);
    REGISTER_COPYCONSTRUCTORS(HeaderedPumpsConstantSpeed);
    REGISTER_COPYCONSTRUCTORS(HeaderedPumpsVariableSpeed);
    REGISTER_COPYCONSTRUCTORS(HeatBalanceAlgorithm);
    REGISTER_COPYCONSTRUCTORS(HeatExchangerAirToAirSensibleAndLatent);
    REGISTER_COPYCONSTRUCTORS(HeatExchangerDesiccantBalancedFlow);
    REGISTER_COPYCONSTRUCTORS(HeatExchangerDesiccantBalancedFlowPerformanceDataType1);
    REGISTER_COPYCONSTRUCTORS(HeatExchangerFluidToFluid);
    REGISTER_COPYCONSTRUCTORS(HeatPumpAirToWaterFuelFiredHeating);
    REGISTER_COPYCONSTRUCTORS(HeatPumpAirToWaterFuelFiredCooling);
    REGISTER_COPYCONSTRUCTORS(HeatPumpWaterToWaterEquationFitCooling);
    REGISTER_COPYCONSTRUCTORS(HeatPumpWaterToWaterEquationFitHeating);
    REGISTER_COPYCONSTRUCTORS(HeatPumpPlantLoopEIRCooling);
    REGISTER_COPYCONSTRUCTORS(HeatPumpPlantLoopEIRHeating);
    REGISTER_COPYCONSTRUCTORS(HotWaterEquipment);
    REGISTER_COPYCONSTRUCTORS(HotWaterEquipmentDefinition);
    REGISTER_COPYCONSTRUCTORS(HumidifierSteamElectric);
    REGISTER_COPYCONSTRUCTORS(HumidifierSteamGas);
    REGISTER_COPYCONSTRUCTORS(InfraredTransparentMaterial);
    REGISTER_COPYCONSTRUCTORS(InsideSurfaceConvectionAlgorithm);
    REGISTER_COPYCONSTRUCTORS(InteriorPartitionSurface);
    REGISTER_COPYCONSTRUCTORS(InteriorPartitionSurfaceGroup);
    REGISTER_COPYCONSTRUCTORS(InternalMass);
    REGISTER_COPYCONSTRUCTORS(InternalMassDefinition);
    REGISTER_COPYCONSTRUCTORS(IlluminanceMap);
    REGISTER_COPYCONSTRUCTORS(LoadProfilePlant);
    REGISTER_COPYCONSTRUCTORS(LifeCycleCost);
    REGISTER_COPYCONSTRUCTORS(LifeCycleCostParameters);
    REGISTER_COPYCONSTRUCTORS(LifeCycleCostUsePriceEscalation);
    REGISTER_COPYCONSTRUCTORS(LightingDesignDay);
    REGISTER_COPYCONSTRUCTORS(LightingSimulationControl);
    REGISTER_COPYCONSTRUCTORS(LightingSimulationZone);
    REGISTER_COPYCONSTRUCTORS(Lights);
    REGISTER_COPYCONSTRUCTORS(LightsDefinition);
    REGISTER_COPYCONSTRUCTORS(LoadingIndex);
    REGISTER_COPYCONSTRUCTORS(Luminaire);
    REGISTER_COPYCONSTRUCTORS(LuminaireDefinition);
    REGISTER_COPYCONSTRUCTORS(MaterialPropertyGlazingSpectralData);
    REGISTER_COPYCONSTRUCTORS(MaterialPropertyMoisturePenetrationDepthSettings);
    REGISTER_COPYCONSTRUCTORS(MaterialPropertyPhaseChange);
    REGISTER_COPYCONSTRUCTORS(MaterialPropertyPhaseChangeHysteresis);
    REGISTER_COPYCONSTRUCTORS(MasslessOpaqueMaterial);
    REGISTER_COPYCONSTRUCTORS(MeterCustom);
    REGISTER_COPYCONSTRUCTORS(MeterCustomDecrement);
    REGISTER_COPYCONSTRUCTORS(ModelObjectList);
    REGISTER_COPYCONSTRUCTORS(Node);
    REGISTER_COPYCONSTRUCTORS(OtherEquipment);
    REGISTER_COPYCONSTRUCTORS(OtherEquipmentDefinition);
    REGISTER_COPYCONSTRUCTORS(OutputControlFiles);
    REGISTER_COPYCONSTRUCTORS(OutputControlReportingTolerances);
    REGISTER_COPYCONSTRUCTORS(OutputControlTableStyle);
    REGISTER_COPYCONSTRUCTORS(OutputControlTimestamp);
    REGISTER_COPYCONSTRUCTORS(OutputDebuggingData);
    REGISTER_COPYCONSTRUCTORS(OutputDiagnostics);
    REGISTER_COPYCONSTRUCTORS(OutputEnergyManagementSystem);
    REGISTER_COPYCONSTRUCTORS(OutputJSON);
    REGISTER_COPYCONSTRUCTORS(OutputSQLite);
    REGISTER_COPYCONSTRUCTORS(OutputEnvironmentalImpactFactors);
    REGISTER_COPYCONSTRUCTORS(OutputSchedules);
    REGISTER_COPYCONSTRUCTORS(OutputConstructions);
    REGISTER_COPYCONSTRUCTORS(EnvironmentalImpactFactors);
    REGISTER_COPYCONSTRUCTORS(FuelFactors);
    REGISTER_COPYCONSTRUCTORS(OutputMeter);
    REGISTER_COPYCONSTRUCTORS(OutputVariable);
    REGISTER_COPYCONSTRUCTORS(OutputTableSummaryReports);
    REGISTER_COPYCONSTRUCTORS(OutsideSurfaceConvectionAlgorithm);
    REGISTER_COPYCONSTRUCTORS(People);
    REGISTER_COPYCONSTRUCTORS(PeopleDefinition);
    REGISTER_COPYCONSTRUCTORS(PerformancePrecisionTradeoffs);
    REGISTER_COPYCONSTRUCTORS(PhotovoltaicPerformanceEquivalentOneDiode);
    REGISTER_COPYCONSTRUCTORS(PhotovoltaicPerformanceSimple);
    REGISTER_COPYCONSTRUCTORS(PhotovoltaicPerformanceSandia);
    REGISTER_COPYCONSTRUCTORS(PipeAdiabatic);
    REGISTER_COPYCONSTRUCTORS(PipeIndoor);
    REGISTER_COPYCONSTRUCTORS(PipeOutdoor);
    REGISTER_COPYCONSTRUCTORS(PlantComponentTemperatureSource);
    REGISTER_COPYCONSTRUCTORS(PlantComponentUserDefined);
    REGISTER_COPYCONSTRUCTORS(PlantLoop);
    REGISTER_COPYCONSTRUCTORS(PlantEquipmentOperationCoolingLoad);
    REGISTER_COPYCONSTRUCTORS(PlantEquipmentOperationHeatingLoad);
    REGISTER_COPYCONSTRUCTORS(PlantEquipmentOperationOutdoorDryBulb);
    REGISTER_COPYCONSTRUCTORS(PlantEquipmentOperationOutdoorWetBulb);
    REGISTER_COPYCONSTRUCTORS(PlantEquipmentOperationOutdoorDewpoint);
    REGISTER_COPYCONSTRUCTORS(PlantEquipmentOperationOutdoorRelativeHumidity);
    REGISTER_COPYCONSTRUCTORS(PlantEquipmentOperationOutdoorDryBulbDifference);
    REGISTER_COPYCONSTRUCTORS(PlantEquipmentOperationOutdoorWetBulbDifference);
    REGISTER_COPYCONSTRUCTORS(PlantEquipmentOperationOutdoorDewpointDifference);
    REGISTER_COPYCONSTRUCTORS(PortList);
    REGISTER_COPYCONSTRUCTORS(ProgramControl);
    REGISTER_COPYCONSTRUCTORS(PumpConstantSpeed);
    REGISTER_COPYCONSTRUCTORS(PumpVariableSpeed);
    REGISTER_COPYCONSTRUCTORS(PythonPluginInstance);
    REGISTER_COPYCONSTRUCTORS(PythonPluginVariable);
    REGISTER_COPYCONSTRUCTORS(PythonPluginTrendVariable);
    REGISTER_COPYCONSTRUCTORS(PythonPluginOutputVariable);
    REGISTER_COPYCONSTRUCTORS(PythonPluginSearchPaths);
    REGISTER_COPYCONSTRUCTORS(RadianceParameters);
    REGISTER_COPYCONSTRUCTORS(RefractionExtinctionGlazing);
    REGISTER_COPYCONSTRUCTORS(RefrigerationAirChiller);
    REGISTER_COPYCONSTRUCTORS(RefrigerationCase);
    REGISTER_COPYCONSTRUCTORS(RefrigerationCompressor);
    REGISTER_COPYCONSTRUCTORS(RefrigerationCompressorRack);
    REGISTER_COPYCONSTRUCTORS(RefrigerationCondenserAirCooled);
    REGISTER_COPYCONSTRUCTORS(RefrigerationCondenserCascade);
    REGISTER_COPYCONSTRUCTORS(RefrigerationCondenserEvaporativeCooled);
    REGISTER_COPYCONSTRUCTORS(RefrigerationCondenserWaterCooled);
    REGISTER_COPYCONSTRUCTORS(RefrigerationDefrostCycleParameters);
    REGISTER_COPYCONSTRUCTORS(RefrigerationGasCoolerAirCooled);
    REGISTER_COPYCONSTRUCTORS(RefrigerationSubcoolerMechanical);
    REGISTER_COPYCONSTRUCTORS(RefrigerationSubcoolerLiquidSuction);
    REGISTER_COPYCONSTRUCTORS(RefrigerationSecondarySystem);
    REGISTER_COPYCONSTRUCTORS(RefrigerationSystem);
    REGISTER_COPYCONSTRUCTORS(RefrigerationTranscriticalSystem);
    REGISTER_COPYCONSTRUCTORS(RefrigerationWalkIn);
    REGISTER_COPYCONSTRUCTORS(RefrigerationWalkInZoneBoundary);
    REGISTER_COPYCONSTRUCTORS(RenderingColor);
    REGISTER_COPYCONSTRUCTORS(RoofVegetation);
    REGISTER_COPYCONSTRUCTORS(RunPeriod);
    REGISTER_COPYCONSTRUCTORS(RunPeriodControlDaylightSavingTime);
    REGISTER_COPYCONSTRUCTORS(RunPeriodControlSpecialDays);
    REGISTER_COPYCONSTRUCTORS(ScheduleCompact);
    REGISTER_COPYCONSTRUCTORS(ScheduleConstant);
    REGISTER_COPYCONSTRUCTORS(ScheduleDay);
    REGISTER_COPYCONSTRUCTORS(ScheduleFixedInterval);
    REGISTER_COPYCONSTRUCTORS(ScheduleFile);
    REGISTER_COPYCONSTRUCTORS(ScheduleTypeLimits);
    REGISTER_COPYCONSTRUCTORS(ScheduleVariableInterval);
    REGISTER_COPYCONSTRUCTORS(ScheduleRule);
    REGISTER_COPYCONSTRUCTORS(ScheduleRuleset);
    REGISTER_COPYCONSTRUCTORS(ScheduleWeek);
    REGISTER_COPYCONSTRUCTORS(ScheduleYear);
    REGISTER_COPYCONSTRUCTORS(Screen);
    REGISTER_COPYCONSTRUCTORS(SetpointManagerColdest);
    REGISTER_COPYCONSTRUCTORS(SetpointManagerFollowGroundTemperature);
    REGISTER_COPYCONSTRUCTORS(SetpointManagerFollowOutdoorAirTemperature);
    REGISTER_COPYCONSTRUCTORS(SetpointManagerFollowSystemNodeTemperature);
    REGISTER_COPYCONSTRUCTORS(SetpointManagerMixedAir);
    REGISTER_COPYCONSTRUCTORS(SetpointManagerMultiZoneCoolingAverage);
    REGISTER_COPYCONSTRUCTORS(SetpointManagerMultiZoneHeatingAverage);
    REGISTER_COPYCONSTRUCTORS(SetpointManagerMultiZoneHumidityMaximum);
    REGISTER_COPYCONSTRUCTORS(SetpointManagerMultiZoneHumidityMinimum);
    REGISTER_COPYCONSTRUCTORS(SetpointManagerMultiZoneMaximumHumidityAverage);
    REGISTER_COPYCONSTRUCTORS(SetpointManagerMultiZoneMinimumHumidityAverage);
    REGISTER_COPYCONSTRUCTORS(SetpointManagerOutdoorAirPretreat);
    REGISTER_COPYCONSTRUCTORS(SetpointManagerOutdoorAirReset);
    REGISTER_COPYCONSTRUCTORS(SetpointManagerScheduled);
    REGISTER_COPYCONSTRUCTORS(SetpointManagerScheduledDualSetpoint);
    REGISTER_COPYCONSTRUCTORS(SetpointManagerSingleZoneCooling);
    REGISTER_COPYCONSTRUCTORS(SetpointManagerSingleZoneHeating);
    REGISTER_COPYCONSTRUCTORS(SetpointManagerSingleZoneHumidityMaximum);
    REGISTER_COPYCONSTRUCTORS(SetpointManagerSingleZoneHumidityMinimum);
    REGISTER_COPYCONSTRUCTORS(SetpointManagerSingleZoneOneStageCooling);
    REGISTER_COPYCONSTRUCTORS(SetpointManagerSingleZoneOneStageHeating);
    REGISTER_COPYCONSTRUCTORS(SetpointManagerSingleZoneReheat);
    REGISTER_COPYCONSTRUCTORS(SetpointManagerWarmest);
    REGISTER_COPYCONSTRUCTORS(SetpointManagerWarmestTemperatureFlow);
    REGISTER_COPYCONSTRUCTORS(SetpointManagerSystemNodeResetHumidity);
    REGISTER_COPYCONSTRUCTORS(SetpointManagerSystemNodeResetTemperature);
    REGISTER_COPYCONSTRUCTORS(Shade);
    REGISTER_COPYCONSTRUCTORS(ShadingControl);
    REGISTER_COPYCONSTRUCTORS(ShadingSurface);
    REGISTER_COPYCONSTRUCTORS(ShadingSurfaceGroup);
    REGISTER_COPYCONSTRUCTORS(ShadowCalculation);
    REGISTER_COPYCONSTRUCTORS(SimpleGlazing);
    REGISTER_COPYCONSTRUCTORS(SimulationControl);
    REGISTER_COPYCONSTRUCTORS(Site);
    REGISTER_COPYCONSTRUCTORS(SiteGroundReflectance);
    REGISTER_COPYCONSTRUCTORS(SiteGroundTemperatureBuildingSurface);
    REGISTER_COPYCONSTRUCTORS(SiteGroundTemperatureDeep);
    REGISTER_COPYCONSTRUCTORS(SiteGroundTemperatureShallow);
    REGISTER_COPYCONSTRUCTORS(SiteGroundTemperatureFCfactorMethod);
    REGISTER_COPYCONSTRUCTORS(SiteGroundTemperatureUndisturbedKusudaAchenbach);
    REGISTER_COPYCONSTRUCTORS(SiteGroundTemperatureUndisturbedXing);
    REGISTER_COPYCONSTRUCTORS(SiteWaterMainsTemperature);
    REGISTER_COPYCONSTRUCTORS(SizingParameters);
    REGISTER_COPYCONSTRUCTORS(SizingPlant);
    REGISTER_COPYCONSTRUCTORS(SizingSystem);
    REGISTER_COPYCONSTRUCTORS(SizingZone);
    REGISTER_COPYCONSTRUCTORS(SkyTemperature);
    REGISTER_COPYCONSTRUCTORS(SolarCollectorFlatPlatePhotovoltaicThermal);
    REGISTER_COPYCONSTRUCTORS(SolarCollectorFlatPlateWater);
    REGISTER_COPYCONSTRUCTORS(SolarCollectorIntegralCollectorStorage);
    REGISTER_COPYCONSTRUCTORS(SolarCollectorPerformanceFlatPlate);
    REGISTER_COPYCONSTRUCTORS(SolarCollectorPerformanceIntegralCollectorStorage);
    REGISTER_COPYCONSTRUCTORS(SolarCollectorPerformancePhotovoltaicThermalBIPVT);
    REGISTER_COPYCONSTRUCTORS(SolarCollectorPerformancePhotovoltaicThermalSimple);
    REGISTER_COPYCONSTRUCTORS(Space);
    REGISTER_COPYCONSTRUCTORS(SpaceInfiltrationDesignFlowRate);
    REGISTER_COPYCONSTRUCTORS(SpaceInfiltrationEffectiveLeakageArea);
    REGISTER_COPYCONSTRUCTORS(SpaceInfiltrationFlowCoefficient);
    REGISTER_COPYCONSTRUCTORS(SpaceType);
    REGISTER_COPYCONSTRUCTORS(StandardGlazing);
    REGISTER_COPYCONSTRUCTORS(StandardOpaqueMaterial);
    REGISTER_COPYCONSTRUCTORS(StandardsInformationConstruction);
    REGISTER_COPYCONSTRUCTORS(StandardsInformationMaterial);
    REGISTER_COPYCONSTRUCTORS(SteamEquipment);
    REGISTER_COPYCONSTRUCTORS(SteamEquipmentDefinition);
    REGISTER_COPYCONSTRUCTORS(SubSurface);
    REGISTER_COPYCONSTRUCTORS(Surface);
    REGISTER_COPYCONSTRUCTORS(SurfaceControlMovableInsulation);
    REGISTER_COPYCONSTRUCTORS(SurfacePropertyConvectionCoefficients);
    REGISTER_COPYCONSTRUCTORS(SurfacePropertyConvectionCoefficientsMultipleSurface);
    REGISTER_COPYCONSTRUCTORS(SurfacePropertyExposedFoundationPerimeter);
    REGISTER_COPYCONSTRUCTORS(SurfacePropertyLocalEnvironment);
    REGISTER_COPYCONSTRUCTORS(SurfacePropertyOtherSideCoefficients);
    REGISTER_COPYCONSTRUCTORS(SurfacePropertyOtherSideConditionsModel);
    REGISTER_COPYCONSTRUCTORS(SurfacePropertySurroundingSurfaces);
    REGISTER_COPYCONSTRUCTORS(SurfacePropertyGroundSurfaces);
    REGISTER_COPYCONSTRUCTORS(SurfacePropertyIncidentSolarMultiplier);
    REGISTER_COPYCONSTRUCTORS(SwimmingPoolIndoor);
#if defined(_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable : 4996)
#elif (defined(__GNUC__))
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
    REGISTER_COPYCONSTRUCTORS(TableMultiVariableLookup);
#if defined(_MSC_VER)
#  pragma warning(pop)
#elif (defined(__GNUC__))
#  pragma GCC diagnostic pop
#endif
    REGISTER_COPYCONSTRUCTORS(TableLookup);
    REGISTER_COPYCONSTRUCTORS(TableIndependentVariable);
    REGISTER_COPYCONSTRUCTORS(TemperingValve);
    REGISTER_COPYCONSTRUCTORS(ThermochromicGlazing);
    REGISTER_COPYCONSTRUCTORS(ThermostatSetpointDualSetpoint);
    REGISTER_COPYCONSTRUCTORS(ThermalZone);
    REGISTER_COPYCONSTRUCTORS(ThermalStorageChilledWaterStratified);
    REGISTER_COPYCONSTRUCTORS(ThermalStorageIceDetailed);
    REGISTER_COPYCONSTRUCTORS(Timestep);
    REGISTER_COPYCONSTRUCTORS(UnitarySystemPerformanceMultispeed);
    REGISTER_COPYCONSTRUCTORS(UtilityBill);

    // TODO: once UtilityCost objects are wrapped
    // REGISTER_COPYCONSTRUCTORS(UtilityCost_Charge_Block);
    // REGISTER_COPYCONSTRUCTORS(UtilityCost_Charge_Simple);
    // REGISTER_COPYCONSTRUCTORS(UtilityCost_Computation);
    // REGISTER_COPYCONSTRUCTORS(UtilityCost_Qualify);
    // REGISTER_COPYCONSTRUCTORS(UtilityCost_Ratchet);
    // REGISTER_COPYCONSTRUCTORS(UtilityCost_Tariff);
    // REGISTER_COPYCONSTRUCTORS(UtilityCost_Variable);

    REGISTER_COPYCONSTRUCTORS(Version);
    REGISTER_COPYCONSTRUCTORS(WaterHeaterMixed);
    REGISTER_COPYCONSTRUCTORS(WaterHeaterHeatPump);
    REGISTER_COPYCONSTRUCTORS(WaterHeaterHeatPumpWrappedCondenser);
    REGISTER_COPYCONSTRUCTORS(WaterHeaterSizing);
    REGISTER_COPYCONSTRUCTORS(WaterHeaterStratified);
    REGISTER_COPYCONSTRUCTORS(WaterUseConnections);
    REGISTER_COPYCONSTRUCTORS(WaterUseEquipment);
    REGISTER_COPYCONSTRUCTORS(WaterUseEquipmentDefinition);
    REGISTER_COPYCONSTRUCTORS(WeatherFile);
    REGISTER_COPYCONSTRUCTORS(WeatherFileConditionType);
    REGISTER_COPYCONSTRUCTORS(WeatherFileDays);
    REGISTER_COPYCONSTRUCTORS(WindowDataFile);
    REGISTER_COPYCONSTRUCTORS(WindowPropertyFrameAndDivider);
    REGISTER_COPYCONSTRUCTORS(YearDescription);
    REGISTER_COPYCONSTRUCTORS(ZoneAirContaminantBalance);
    REGISTER_COPYCONSTRUCTORS(ZoneAirHeatBalanceAlgorithm);
    REGISTER_COPYCONSTRUCTORS(ZoneAirMassFlowConservation);
    REGISTER_COPYCONSTRUCTORS(ZoneCapacitanceMultiplierResearchSpecial);
    REGISTER_COPYCONSTRUCTORS(ZoneControlContaminantController);
    REGISTER_COPYCONSTRUCTORS(ZoneControlHumidistat);
    REGISTER_COPYCONSTRUCTORS(ZoneControlThermostatStagedDualSetpoint);
    REGISTER_COPYCONSTRUCTORS(ZoneHVACEquipmentList);
    REGISTER_COPYCONSTRUCTORS(ZoneHVACEvaporativeCoolerUnit);
    REGISTER_COPYCONSTRUCTORS(ZoneHVACBaseboardConvectiveElectric);
    REGISTER_COPYCONSTRUCTORS(ZoneHVACBaseboardConvectiveWater);
    REGISTER_COPYCONSTRUCTORS(ZoneHVACBaseboardRadiantConvectiveElectric);
    REGISTER_COPYCONSTRUCTORS(ZoneHVACBaseboardRadiantConvectiveWater);
    REGISTER_COPYCONSTRUCTORS(ZoneHVACCoolingPanelRadiantConvectiveWater);
    REGISTER_COPYCONSTRUCTORS(ZoneHVACIdealLoadsAirSystem);
    REGISTER_COPYCONSTRUCTORS(ZoneHVACFourPipeFanCoil);
    REGISTER_COPYCONSTRUCTORS(ZoneHVACHighTemperatureRadiant);
    REGISTER_COPYCONSTRUCTORS(ZoneHVACLowTemperatureRadiantElectric);
    REGISTER_COPYCONSTRUCTORS(ZoneHVACLowTempRadiantConstFlow);
    REGISTER_COPYCONSTRUCTORS(ZoneHVACLowTempRadiantVarFlow);
    REGISTER_COPYCONSTRUCTORS(ZoneHVACPackagedTerminalHeatPump);
    REGISTER_COPYCONSTRUCTORS(ZoneHVACPackagedTerminalAirConditioner);
    REGISTER_COPYCONSTRUCTORS(ZoneHVACTerminalUnitVariableRefrigerantFlow);
    REGISTER_COPYCONSTRUCTORS(ZoneHVACWaterToAirHeatPump);
    REGISTER_COPYCONSTRUCTORS(ZoneHVACUnitHeater);
    REGISTER_COPYCONSTRUCTORS(ZoneHVACDehumidifierDX);
    REGISTER_COPYCONSTRUCTORS(ZoneHVACEnergyRecoveryVentilatorController);
    REGISTER_COPYCONSTRUCTORS(ZoneHVACEnergyRecoveryVentilator);
    REGISTER_COPYCONSTRUCTORS(ZoneHVACUnitVentilator);
    REGISTER_COPYCONSTRUCTORS(ZoneMixing);
    REGISTER_COPYCONSTRUCTORS(ZonePropertyUserViewFactorsBySurfaceName);
    REGISTER_COPYCONSTRUCTORS(ZoneVentilationDesignFlowRate);
    REGISTER_COPYCONSTRUCTORS(ZoneVentilationWindandStackOpenArea);
  }

  const detail::Model_Impl::ModelObjectCreator detail::Model_Impl::modelObjectCreator;

}  // namespace model
}  // namespace openstudio
