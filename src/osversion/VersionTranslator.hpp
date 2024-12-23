/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef OSVERSION_VERSIONTRANSLATOR_HPP
#define OSVERSION_VERSIONTRANSLATOR_HPP

#include "OSVersionAPI.hpp"

#include "../model/Schedule.hpp"

#include "../utilities/idf/IdfFile.hpp"
#include "../utilities/idd/IddFile.hpp"
#include "../utilities/idd/IddFileAndFactoryWrapper.hpp"

#include "../utilities/idf/IdfObject.hpp"
#include "../utilities/idd/IddObject.hpp"

#include "../utilities/core/Compare.hpp"
#include "../utilities/core/Logger.hpp"
#include "../utilities/core/StringStreamLogSink.hpp"

#include <boost/functional.hpp>

#include <map>
#include <istream>
#include <string>
#include <set>

namespace openstudio {
class ProgressBar;
namespace model {
  class Model;
  class Component;
}  // namespace model
namespace osversion {

  /** Helper class meant to replace std::pair<IdfObject, IdfObject> that maps objects that have been significantly refactor (old, new) */
  class OSVERSION_API RefactoredObjectData
  {
   public:
    RefactoredObjectData(const IdfObject& old, const IdfObject& newObject);
    RefactoredObjectData(IdfObject&& old, IdfObject&& newObject);

    IdfObject oldObject() const;
    IdfObject newObject() const;

   private:
    IdfObject m_oldObject;
    IdfObject m_newObject;
  };

  /** This class updates OpenStudio Models and Components to the latest version of OpenStudio. It
 *  must be maintained to keep everything working. The developer who is wrapping up the current
 *  release and starting the next one should:
 *
 *  <ol>
 *  <li> Run openstudio_osversion_tests.exe and verify that all the tests pass. </li>
 *  <li> Create the release branch. </li>
 *  <li> Copy the latest resources/osversion folder from the build directory to the source
 *       directory, add those files to the resources/CMakeLists.txt, and commit to trunk. </li>
 *  <li> Increment the OpenStudio version in CMakeLists.txt </li>
 *  <li> Increment the OpenStudio version in openstudiocore/resources/model/OpenStudio.idd (at the
 *       top and in the \\default of OS:Version's Version Identifier field). </li>
 *  <li> Register a trivial update method for the new version in the constructor. Further
 *       instructions are provided in the cpp code for VersionTranslator::VersionTranslator. </li>
 *  <li> Add the just-branched-for-release version number to m_startVersions, also in the
 *       constructor. </li>
 *  </ol>
 *
 *  To support the current iteration, developers should:
 *
 *  <ol>
 *  <li> Create a non-trivial update method upon the first IDD or other change that should result
 *       in data changes to models of the earlier vintages. </li>
 *  <li> Add to this non-trivial update method should any other such changes occur during the
 *       iteration. </li>
 *  <li> Feel free to just log warnings and errors if the desirable changes cannot be reliably
 *       completed at the data (IDF) level. Such messages could prompt the user to take specific
 *       actions in the OpenStudio Application once they have a nominally valid (updated) model. </li>
 *  </ol>
 *  */
  class OSVERSION_API VersionTranslator
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    VersionTranslator();

    //@}
    /** @name Actions
   *
   *  Update files on disk to the current version of OpenStudio. */
    //@{

    /** Returns a current-version OpenStudio Model, if possible. The file at pathToOldOsm must
   *  be an osm of version 0.7.0 or later. */
    boost::optional<model::Model> loadModel(const openstudio::path& pathToOldOsm, ProgressBar* progressBar = nullptr);

    /** \overload */
    boost::optional<model::Model> loadModel(std::istream& is, ProgressBar* progressBar = nullptr);

    /** Load a model from string. Not overloading loadModel so paths passed as strings in bindings will work.*/
    boost::optional<model::Model> loadModelFromString(const std::string& str, ProgressBar* progressBar = nullptr);

    /** Returns a current-version OpenStudio Component, if possible. The file at pathToOldOsc
   *  must be an osc of version 0.7.0 or later. */
    boost::optional<model::Component> loadComponent(const openstudio::path& pathToOldOsc, ProgressBar* progressBar = nullptr);

    /** \overload */
    boost::optional<model::Component> loadComponent(std::istream& is, ProgressBar* progressBar = nullptr);

    //@}
    /** @name Queries
   *
   *  Access warnings, errors, and other information about the last translation process. All this
   *  data is cleared by subsequent calls to loadModel or loadComponent. */
    //@{

    /** Get the version of the loaded file. Is 0.0.0 before loadModel or loadComponent is called.
   *  0.7.0 is the default if no Version object is found in the file. */
    VersionString originalVersion() const;

    /** Get warning messages generated by the last translation. */
    std::vector<LogMessage> warnings() const;

    /** Get error messages generated by the last translation. */
    std::vector<LogMessage> errors() const;

    /** Returns objects that were removed from the model because the object type or particular use
   *  has been deprecated. */
    std::vector<IdfObject> deprecatedObjects() const;

    /** Returns objects that were removed from the model because there is not a straightforward,
   *  unique way to upgrade them. */
    std::vector<IdfObject> untranslatedObjects() const;

    /** Returns objects that were added to the model to make it work properly in the latest
   *  version of OpenStudio. */
    std::vector<IdfObject> newObjects() const;

    /** Returns an (original object, new object) pair for objects that have been significantly
   *  refactored. */
    std::vector<RefactoredObjectData> refactoredObjects() const;

    /** Returns true if loading newer versions is allowable. Defaults to true. */
    bool allowNewerVersions() const;

    /** Set whether or not loading newer versions is allowed. */
    void setAllowNewerVersions(bool allowNewerVersions);

    //@}
   private:
    REGISTER_LOGGER("openstudio.osversion.VersionTranslator");

    using OSVersionUpdater = boost::function<std::string(VersionTranslator*, const IdfFile&, const IddFileAndFactoryWrapper&)>;
    std::map<VersionString, OSVersionUpdater> m_updateMethods;
    std::vector<VersionString> m_startVersions;

    VersionString m_originalVersion;
    bool m_allowNewerVersions;
    std::map<VersionString, IdfFile> m_map;
    StringStreamLogSink m_logSink;
    std::vector<IdfObject> m_deprecated, m_untranslated, m_new;
    std::vector<RefactoredObjectData> m_refactored;
    int m_nObjectsStart;
    int m_nObjectsFinalIdf;
    int m_nObjectsFinalModel;
    bool m_isComponent;
    std::vector<IdfObject> m_cbeccSizingObjects;

    boost::optional<model::Model> updateVersion(std::istream& is, bool isComponent, ProgressBar* progressBar = nullptr);

    void initializeMap(std::istream& is);

    IddFileAndFactoryWrapper getIddFile(const VersionString& version);

    void update(const VersionString& startVersion);

    /** Deletes handles from m_untranslated and m_deprecated, and adds handles from m_new */
    void updateComponentData(IdfFile& idfFile);

    std::string defaultUpdate(const IdfFile& idf, const IddFileAndFactoryWrapper& targetIdd);
    std::string update_0_7_1_to_0_7_2(const IdfFile& idf_0_7_1, const IddFileAndFactoryWrapper& idd_0_7_2);
    std::string update_0_7_2_to_0_7_3(const IdfFile& idf_0_7_2, const IddFileAndFactoryWrapper& idd_0_7_3);
    std::string update_0_7_3_to_0_7_4(const IdfFile& idf_0_7_3, const IddFileAndFactoryWrapper& idd_0_7_4);
    std::string update_0_9_1_to_0_9_2(const IdfFile& idf_0_9_1, const IddFileAndFactoryWrapper& idd_0_9_2);
    std::string update_0_9_5_to_0_9_6(const IdfFile& idf_0_9_5, const IddFileAndFactoryWrapper& idd_0_9_6);
    std::string update_0_9_6_to_0_10_0(const IdfFile& idf_0_9_6, const IddFileAndFactoryWrapper& idd_0_10_0);
    std::string update_0_11_0_to_0_11_1(const IdfFile& idf_0_11_0, const IddFileAndFactoryWrapper& idd_0_11_1);
    std::string update_0_11_1_to_0_11_2(const IdfFile& idf_0_11_1, const IddFileAndFactoryWrapper& idd_0_11_2);
    std::string update_0_11_4_to_0_11_5(const IdfFile& idf_0_11_4, const IddFileAndFactoryWrapper& idd_0_11_5);
    std::string update_0_11_5_to_0_11_6(const IdfFile& idf_0_11_5, const IddFileAndFactoryWrapper& idd_0_11_6);
    std::string update_1_0_1_to_1_0_2(const IdfFile& idf_1_0_1, const IddFileAndFactoryWrapper& idd_1_0_2);
    std::string update_1_0_2_to_1_0_3(const IdfFile& idf_1_0_2, const IddFileAndFactoryWrapper& idd_1_0_3);
    std::string update_1_2_2_to_1_2_3(const IdfFile& idf_1_2_2, const IddFileAndFactoryWrapper& idd_1_2_3);
    std::string update_1_3_4_to_1_3_5(const IdfFile& idf_1_3_4, const IddFileAndFactoryWrapper& idd_1_3_5);
    std::string update_1_5_3_to_1_5_4(const IdfFile& idf_1_5_3, const IddFileAndFactoryWrapper& idd_1_5_4);
    std::string update_1_7_1_to_1_7_2(const IdfFile& idf_1_7_1, const IddFileAndFactoryWrapper& idd_1_7_2);
    std::string update_1_7_4_to_1_7_5(const IdfFile& idf_1_7_4, const IddFileAndFactoryWrapper& idd_1_7_5);
    std::string update_1_8_3_to_1_8_4(const IdfFile& idf_1_8_3, const IddFileAndFactoryWrapper& idd_1_8_4);
    std::string update_1_8_4_to_1_8_5(const IdfFile& idf_1_8_4, const IddFileAndFactoryWrapper& idd_1_8_5);
    std::string update_1_8_5_to_1_9_0(const IdfFile& idf_1_8_5, const IddFileAndFactoryWrapper& idd_1_9_0);
    std::string update_1_9_2_to_1_9_3(const IdfFile& idf_1_9_2, const IddFileAndFactoryWrapper& idd_1_9_3);
    std::string update_1_9_4_to_1_9_5(const IdfFile& idf_1_9_4, const IddFileAndFactoryWrapper& idd_1_9_5);
    std::string update_1_9_5_to_1_10_0(const IdfFile& idf_1_9_5, const IddFileAndFactoryWrapper& idd_1_10_0);
    std::string update_1_10_1_to_1_10_2(const IdfFile& idf_1_10_1, const IddFileAndFactoryWrapper& idd_1_10_2);
    std::string update_1_10_5_to_1_10_6(const IdfFile& idf_1_10_5, const IddFileAndFactoryWrapper& idd_1_10_6);
    std::string update_1_11_3_to_1_11_4(const IdfFile& idf_1_11_3, const IddFileAndFactoryWrapper& idd_1_11_4);
    std::string update_1_11_4_to_1_11_5(const IdfFile& idf_1_11_4, const IddFileAndFactoryWrapper& idd_1_11_5);
    std::string update_1_12_0_to_1_12_1(const IdfFile& idf_1_12_0, const IddFileAndFactoryWrapper& idd_1_12_1);
    std::string update_1_12_3_to_1_12_4(const IdfFile& idf_1_12_3, const IddFileAndFactoryWrapper& idd_1_12_4);
    std::string update_2_1_0_to_2_1_1(const IdfFile& idf_2_1_0, const IddFileAndFactoryWrapper& idd_2_1_1);
    std::string update_2_1_1_to_2_1_2(const IdfFile& idf_2_1_1, const IddFileAndFactoryWrapper& idd_2_1_2);
    std::string update_2_3_0_to_2_3_1(const IdfFile& idf_2_3_0, const IddFileAndFactoryWrapper& idd_2_3_1);
    std::string update_2_4_1_to_2_4_2(const IdfFile& idf_2_4_1, const IddFileAndFactoryWrapper& idd_2_4_2);
    std::string update_2_4_3_to_2_5_0(const IdfFile& idf_2_4_3, const IddFileAndFactoryWrapper& idd_2_5_0);
    std::string update_2_6_0_to_2_6_1(const IdfFile& idf_2_6_0, const IddFileAndFactoryWrapper& idd_2_6_1);
    std::string update_2_6_1_to_2_6_2(const IdfFile& idf_2_6_1, const IddFileAndFactoryWrapper& idd_2_6_2);
    std::string update_2_6_2_to_2_7_0(const IdfFile& idf_2_6_2, const IddFileAndFactoryWrapper& idd_2_7_0);
    std::string update_2_7_0_to_2_7_1(const IdfFile& idf_2_7_0, const IddFileAndFactoryWrapper& idd_2_7_1);
    std::string update_2_7_1_to_2_7_2(const IdfFile& idf_2_7_1, const IddFileAndFactoryWrapper& idd_2_7_2);
    std::string update_2_8_1_to_2_9_0(const IdfFile& idf_2_8_1, const IddFileAndFactoryWrapper& idd_2_9_0);
    std::string update_2_9_0_to_2_9_1(const IdfFile& idf_2_9_0, const IddFileAndFactoryWrapper& idd_2_9_1);
    std::string update_2_9_1_to_3_0_0(const IdfFile& idf_2_9_1, const IddFileAndFactoryWrapper& idd_3_0_0);
    std::string update_3_0_0_to_3_0_1(const IdfFile& idf_3_0_0, const IddFileAndFactoryWrapper& idd_3_0_1);
    std::string update_3_0_1_to_3_1_0(const IdfFile& idf_3_0_1, const IddFileAndFactoryWrapper& idd_3_1_0);
    std::string update_3_1_0_to_3_2_0(const IdfFile& idf_3_1_0, const IddFileAndFactoryWrapper& idd_3_2_0);
    std::string update_3_2_0_to_3_2_1(const IdfFile& idf_3_2_0, const IddFileAndFactoryWrapper& idd_3_2_1);
    std::string update_3_2_1_to_3_3_0(const IdfFile& idf_3_2_1, const IddFileAndFactoryWrapper& idd_3_3_0);
    std::string update_3_3_0_to_3_4_0(const IdfFile& idf_3_3_0, const IddFileAndFactoryWrapper& idd_3_4_0);
    std::string update_3_4_0_to_3_5_0(const IdfFile& idf_3_4_0, const IddFileAndFactoryWrapper& idd_3_5_0);
    std::string update_3_5_0_to_3_5_1(const IdfFile& idf_3_5_0, const IddFileAndFactoryWrapper& idd_3_5_1);
    std::string update_3_5_1_to_3_6_0(const IdfFile& idf_3_5_1, const IddFileAndFactoryWrapper& idd_3_6_0);
    std::string update_3_6_1_to_3_7_0(const IdfFile& idf_3_6_1, const IddFileAndFactoryWrapper& idd_3_7_0);
    std::string update_3_7_0_to_3_8_0(const IdfFile& idf_3_7_0, const IddFileAndFactoryWrapper& idd_3_8_0);
    std::string update_3_8_0_to_3_9_0(const IdfFile& idf_3_8_0, const IddFileAndFactoryWrapper& idd_3_9_0);
    std::string update_3_9_0_to_3_9_1(const IdfFile& idf_3_9_0, const IddFileAndFactoryWrapper& idd_3_9_1);

    IdfObject updateUrlField_0_7_1_to_0_7_2(const IdfObject& object, unsigned index);

    struct InterobjectIssueInformation
    {
      VersionString endVersion;
      InterobjectIssueInformation(const VersionString& t_endVersion) : endVersion(t_endVersion) {}
      virtual ~InterobjectIssueInformation() = default;
    };

    std::vector<std::shared_ptr<InterobjectIssueInformation>> fixInterobjectIssuesStage1(model::Model& model, const VersionString& startVersion);

    void fixInterobjectIssuesStage2(model::Model& model, std::vector<std::shared_ptr<InterobjectIssueInformation>>& stage1Information);

    struct InterobjectIssueInformation_0_8_3_to_0_8_4 : public InterobjectIssueInformation
    {
      std::vector<model::Schedule> schedules;
      std::vector<std::vector<model::ModelObject>> users;
      std::vector<std::vector<std::vector<unsigned>>> indices;
      std::vector<std::vector<std::vector<model::ScheduleTypeKey>>> keys;
      std::vector<std::vector<IdfObject>> componentDataObjects;
      std::vector<IdfObject> originalUsers;
      std::set<model::ModelObject> refactoredUsers;

      InterobjectIssueInformation_0_8_3_to_0_8_4() : InterobjectIssueInformation(VersionString("0.8.4")) {}
      virtual ~InterobjectIssueInformation_0_8_3_to_0_8_4() = default;
    };

    std::shared_ptr<InterobjectIssueInformation> fixInterobjectIssuesStage1_0_8_3_to_0_8_4(model::Model& model);

    void fixInterobjectIssuesStage2_0_8_3_to_0_8_4(model::Model& model, std::shared_ptr<InterobjectIssueInformation>& info);
  };

}  // namespace osversion
}  // namespace openstudio

#endif  // OSVERSION_VERSIONTRANSLATOR_HPP
