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

#ifndef MODEL_MODEL_IMPL_HPP
#define MODEL_MODEL_IMPL_HPP

#include <model/ComponentWatcher.hpp>
#include <model/Building.hpp>
#include <model/LifeCycleCostParameters.hpp>
#include <model/RunPeriod.hpp>
#include <model/YearDescription.hpp>
#include <model/WeatherFile.hpp>

#include <utilities/idf/Workspace.hpp>
#include <utilities/idf/Workspace_Impl.hpp>
#include <utilities/idf/WorkspaceObject.hpp>
#include <utilities/idf/WorkspaceObject_Impl.hpp>

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

namespace detail {

  class ModelObject_Impl;

  /** Container for the OpenStudio Building Model hierarchy. */
  class MODEL_API Model_Impl : public openstudio::detail::Workspace_Impl {
    Q_OBJECT;
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
    virtual Workspace clone(bool keepHandles=false) const;

    /** Implementation of openstudio::detail::Workspace_Impl::cloneSubset for Model_Impl. The
     *  returned value may be cast to type Model. */
    virtual Workspace cloneSubset(const std::vector<Handle>& handles,
                                  bool keepHandles = false,
                                  StrictnessLevel level = StrictnessLevel::Draft) const;

    /** Swaps underlying data between this workspace and other. */
    virtual void swap(Workspace& other);

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

    /** Get the WeatherFile object if there is one, this implementation uses a cached reference to the WeatherFile
     *  object which can be significantly faster than calling getOptionalUniqueModelObject<WeatherFile>(). */
    boost::optional<WeatherFile> weatherFile() const;

    Schedule alwaysOnDiscreteSchedule() const;

    //@}
    /** @name Setters */
    //@{

    /** Override to return false. IddFileType is always equal to IddFileType::OpenStudio. */
    virtual bool setIddFile(IddFileType iddFileType);

    // Overriding this from WorkspaceObject_Impl is how all objects in the model end up
    // as model objects
    virtual boost::shared_ptr<openstudio::detail::WorkspaceObject_Impl> createObject(
        const IdfObject& object,
        bool keepHandle);

    // Helper function to start the process of adding a cloned object to the workspace.
    virtual boost::shared_ptr<openstudio::detail::WorkspaceObject_Impl> createObject(
        const boost::shared_ptr<openstudio::detail::WorkspaceObject_Impl>& originalObjectImplPtr,
        bool keepHandle);

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

   public slots :

    virtual void obsoleteComponentWatcher(const ComponentWatcher& watcher);

    virtual void reportInitialModelObjects();

   signals:

    void initialModelObject(openstudio::model::detail::ModelObject_Impl* modelObject, IddObjectType iddObjectType, const openstudio::UUID& handle);

    void initialReportComplete();

   private:
    // explicitly unimplemented copy constructor
    // ETH@20120116 This causes a build error on Windows since there is already a copy constructor
    // (with a defaulted second argument).
    // Model_Impl(const Model_Impl &);

    // explicitly unimplemented assignment operator
    Model_Impl &operator=(const Model_Impl &);


    REGISTER_LOGGER("openstudio.model.Model");

    // Make this a shared_ptr to avoid having to #include SqlFile.hpp in all Model objects
    boost::shared_ptr<openstudio::SqlFile> m_sqlFile;

    std::vector<ComponentWatcher> m_componentWatchers;

    void mf_createComponentWatcher(ComponentData& componentData);

  private:

    mutable boost::optional<Building> m_cachedBuilding;
    mutable boost::optional<LifeCycleCostParameters> m_cachedLifeCycleCostParameters;
    mutable boost::optional<RunPeriod> m_cachedRunPeriod;
    mutable boost::optional<YearDescription> m_cachedYearDescription;
    mutable boost::optional<WeatherFile> m_cachedWeatherFile;

  private slots:

    void clearCachedBuilding();
    void clearCachedLifeCycleCostParameters();
    void clearCachedRunPeriod();
    void clearCachedYearDescription();
    void clearCachedWeatherFile();

  };

} // detail

} // model
} // openstudio

#endif // MODEL_MODEL_IMPL_HPP
