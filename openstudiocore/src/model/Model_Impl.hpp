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

#ifndef MODEL_MODEL_IMPL_HPP
#define MODEL_MODEL_IMPL_HPP

#include "ComponentWatcher.hpp"
#include "Building.hpp"
#include "LifeCycleCostParameters.hpp"
#include "RunPeriod.hpp"
#include "YearDescription.hpp"
#include "WeatherFile.hpp"

#include "../nano/nano_signal_slot.hpp" // Signal-Slot replacement

#include "../utilities/idf/Workspace.hpp"
#include "../utilities/idf/Workspace_Impl.hpp"
#include "../utilities/idf/WorkspaceObject.hpp"
#include "../utilities/idf/WorkspaceObject_Impl.hpp"
#include "../utilities/filetypes/WorkflowJSON.hpp"

#include <boost/optional.hpp>

#include <vector>

namespace openstudio {

class SqlFile;
class IdfFile;
class Workspace;
class ProgressBar;

namespace detail{
  class WorkspaceObject_Impl;
}

namespace model {

// forward declaration
class Model;
class ModelObject;
class Component;
class ComponentData;
class Schedule;
class SpaceType;

namespace detail {

  class ModelObject_Impl;

  /** Container for the OpenStudio Building Model hierarchy. */
  class MODEL_API Model_Impl : public openstudio::detail::Workspace_Impl {
    
   public:
    /** @name Constructors and Destructors */
    //@{

    // default constructor
    Model_Impl();

    // construct from IdfFile
    Model_Impl(const IdfFile& idfFile);

    // construct from Workspace
    Model_Impl(const openstudio::detail::Workspace_Impl& workspace, bool keepHandles=false);

    // copy constructor, used for clone
    Model_Impl(const Model_Impl& other, bool keepHandles=false);

    /** Subset copy constructor makes unconnected copy of all data, only keeping information about
     *  objects corresponding to handles. */
    Model_Impl(const Model_Impl& other,
               const std::vector<Handle>& hs,
               bool keepHandles=false,
               StrictnessLevel level = StrictnessLevel::Draft);

    /** Implementation of openstudio::detail::Workspace_Impl::clone for Model_Impl. The returned
     *  value may be cast to type Model. */
    virtual Workspace clone(bool keepHandles=false) const override;

    /** Implementation of openstudio::detail::Workspace_Impl::cloneSubset for Model_Impl. The
     *  returned value may be cast to type Model. */
    virtual Workspace cloneSubset(const std::vector<Handle>& handles,
                                  bool keepHandles = false,
                                  StrictnessLevel level = StrictnessLevel::Draft) const override;

    /** Swaps underlying data between this workspace and other. */
    virtual void swap(Workspace& other) override;

    virtual ~Model_Impl() {}

    /** Creates ComponentWatchers for each ComponentData object. Should be called as part of
     *  construction from IdfFile or Workspace. */
    void createComponentWatchers();

    //@}
    /** @name Type Casting */
    //@{

    Model model() const;

    //@}
    /** @name Getters */
    //@{

    /// Get the WorkflowJSON
    WorkflowJSON workflowJSON() const;

    /// Get the sql file
    boost::optional<openstudio::SqlFile> sqlFile() const;

    /** Get the Building object if there is one, this implementation uses a cached reference to the Building
     *  object which can be significantly faster than calling getOptionalUniqueModelObject<Building>(). */
    boost::optional<Building> building() const;

    /** Get the LifeCycleCostParameters object if there is one, this implementation uses a cached reference to the LifeCycleCostParameters
     *  object which can be significantly faster than calling getOptionalUniqueModelObject<LifeCycleCostParameters>(). */
    boost::optional<LifeCycleCostParameters> lifeCycleCostParameters() const;
    
    /** Get the RunPeriod object if there is one, this implementation uses a cached reference to the RunPeriod
     *  object which can be significantly faster than calling getOptionalUniqueModelObject<RunPeriod>(). */
    boost::optional<RunPeriod> runPeriod() const;

    /** Get the YearDescription object if there is one, this implementation uses a cached reference to the YearDescription
     *  object which can be significantly faster than calling getOptionalUniqueModelObject<YearDescription>(). */
    boost::optional<YearDescription> yearDescription() const;
      
    /** Get or create the YearDescription object if there is one, then call method from YearDescription. */
    // DLM: this is due to issues exporting the model::YearDescription object because of name conflict with utilities::YearDescription.
    boost::optional<int> calendarYear() const;
    std::string dayofWeekforStartDay() const;
    bool isDayofWeekforStartDayDefaulted() const;
    bool isLeapYear() const;
    bool isIsLeapYearDefaulted() const;
    void setCalendarYear(int calendarYear);
    void resetCalendarYear();
    bool setDayofWeekforStartDay(std::string dayofWeekforStartDay);
    void resetDayofWeekforStartDay();
    bool setIsLeapYear(bool isLeapYear);
    void resetIsLeapYear();
    int assumedYear();
    openstudio::Date makeDate(openstudio::MonthOfYear monthOfYear, unsigned dayOfMonth);
    openstudio::Date makeDate(unsigned monthOfYear, unsigned dayOfMonth);
    openstudio::Date makeDate(openstudio::NthDayOfWeekInMonth n, openstudio::DayOfWeek dayOfWeek, openstudio::MonthOfYear monthOfYear);
    openstudio::Date makeDate(unsigned dayOfYear);

    /** Get the WeatherFile object if there is one, this implementation uses a cached reference to the WeatherFile
     *  object which can be significantly faster than calling getOptionalUniqueModelObject<WeatherFile>(). */
    boost::optional<WeatherFile> weatherFile() const;

    Schedule alwaysOnDiscreteSchedule() const;

    std::string alwaysOnDiscreteScheduleName() const;

    Schedule alwaysOffDiscreteSchedule() const;

    std::string alwaysOffDiscreteScheduleName() const;

    Schedule alwaysOnContinuousSchedule() const;

    std::string alwaysOnContinuousScheduleName() const;

    SpaceType plenumSpaceType() const;

    std::string plenumSpaceTypeName() const;

    //@}
    /** @name Setters */
    //@{

    /** Override to return false. IddFileType is always equal to IddFileType::OpenStudio. */
    virtual bool setIddFile(IddFileType iddFileType);

    // Overriding this from WorkspaceObject_Impl is how all objects in the model end up
    // as model objects
    virtual std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> createObject(
        const IdfObject& object,
        bool keepHandle) override;

    // Helper function to start the process of adding a cloned object to the workspace.
    virtual std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> createObject(
        const std::shared_ptr<openstudio::detail::WorkspaceObject_Impl>& originalObjectImplPtr,
        bool keepHandle) override;

    /// Set the WorkflowJSON
    bool setWorkflowJSON(const WorkflowJSON& workflowJSON);

    void resetWorkflowJSON();

    /// set the sql file
    virtual bool setSqlFile(const openstudio::SqlFile& sqlFile);

    /// reset the sql file
    bool resetSqlFile();

    /// equality test
    bool operator==(const Model& other) const;

    /** Inserts Component into Model and returns the primary object, if possible. */
    virtual boost::optional<ComponentData> insertComponent(const Component& component);

    /** Removes all \link ResourceObject ResourceObjects\endlink with
     *  nonResourceObjectUseCount() == 0. All objects removed in the course of the purge
     *  are returned to support undos. Note that ResourceObjects may have children that
     *  are not ResourceObjects, and these may be removed as well. */
    virtual std::vector<openstudio::IdfObject> purgeUnusedResourceObjects();

    /** Removes all \link ResourceObject ResourceObjects\endlink of given IddObjectType with
     *  directUseCount() == 0. All objects removed in the course of the purge
     *  are returned to support undos. Note that ResourceObjects may have children that
     *  are not ResourceObjects, and these may be removed as well. */
    virtual std::vector<openstudio::IdfObject> purgeUnusedResourceObjects(IddObjectType iddObjectType);

    void connect(const Model& model,
                 ModelObject sourceObject,
                 unsigned sourcePort,
                 ModelObject targetObject,
                 unsigned targetPort);

    void disconnect(ModelObject object, unsigned port);

    //@}
    /** @name Nano Signals */
    //@{

    Nano::Signal<void(openstudio::model::detail::ModelObject_Impl *, IddObjectType, openstudio::UUID)> initialModelObject;

    Nano::Signal<void()> initialReportComplete;

   //@}

   // public slots :

    virtual void obsoleteComponentWatcher(const ComponentWatcher& watcher);

    virtual void reportInitialModelObjects();

   

   private:
    // explicitly unimplemented copy constructor
    // ETH@20120116 This causes a build error on Windows since there is already a copy constructor
    // (with a defaulted second argument).
    // Model_Impl(const Model_Impl &);

    // explicitly unimplemented assignment operator
    Model_Impl &operator=(const Model_Impl &);


    REGISTER_LOGGER("openstudio.model.Model");

    // Make this a shared_ptr to avoid having to #include SqlFile.hpp in all Model objects
    std::shared_ptr<openstudio::SqlFile> m_sqlFile;

    std::vector<ComponentWatcher> m_componentWatchers;

    void mf_createComponentWatcher(ComponentData& componentData);

    WorkflowJSON m_workflowJSON;

  private:

    mutable boost::optional<Building> m_cachedBuilding;
    mutable boost::optional<LifeCycleCostParameters> m_cachedLifeCycleCostParameters;
    mutable boost::optional<RunPeriod> m_cachedRunPeriod;
    mutable boost::optional<YearDescription> m_cachedYearDescription;
    mutable boost::optional<WeatherFile> m_cachedWeatherFile;

  // private slots:
    void clearCachedData();
    void clearCachedBuilding(const Handle& handle);
    void clearCachedLifeCycleCostParameters(const Handle& handle);
    void clearCachedRunPeriod(const Handle& handle);
    void clearCachedYearDescription(const Handle& handle);
    void clearCachedWeatherFile(const Handle& handle);

  };

} // detail

} // model
} // openstudio

#endif // MODEL_MODEL_IMPL_HPP
