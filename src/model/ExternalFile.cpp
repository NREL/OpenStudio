/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ExternalFile.hpp"
#include "ExternalFile_Impl.hpp"

#include "Model.hpp"
#include "ScheduleFile.hpp"
#include "PythonPluginInstance.hpp"
#include "PythonPluginInstance_Impl.hpp"
#include "ChillerElectricASHRAE205.hpp"
#include "ChillerElectricASHRAE205_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_External_File_FieldEnums.hxx>

#include "../utilities/filetypes/WorkflowJSON.hpp"
#include "../utilities/core/PathHelpers.hpp"
#include "../utilities/core/Checksum.hpp"
#include "../utilities/core/Assert.hpp"

#include <unordered_map>
#include "ScheduleFile_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ExternalFile_Impl::ExternalFile_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ResourceObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ExternalFile::iddObjectType());
    }

    ExternalFile_Impl::ExternalFile_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ResourceObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ExternalFile::iddObjectType());
    }

    ExternalFile_Impl::ExternalFile_Impl(const ExternalFile_Impl& other, Model_Impl* model, bool keepHandle)
      : ResourceObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& ExternalFile_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      if (result.empty()) {
      }
      return result;
    }

    IddObjectType ExternalFile_Impl::iddObjectType() const {
      return ExternalFile::iddObjectType();
    }

    ModelObject ExternalFile_Impl::clone(Model model) const {
      boost::optional<ExternalFile> externalFile = ExternalFile::getExternalFile(model, toString(this->filePath()));
      if (!externalFile) {
        LOG_AND_THROW("Could not clone " << this->briefDescription());
      }
      return *externalFile;
    }

    std::vector<IdfObject> ExternalFile_Impl::remove() {
      if (!fileName().empty()) {
        path p = filePath();
        if (exists(p)) {
          try {
            boost::filesystem::remove(p);
          } catch (std::exception&) {
            LOG(Warn, "Could not remove file \"" << p << "\"");
          }
        }
      }

      std::vector<openstudio::IdfObject> tmp;

      // ScheduleFile
      std::vector<ScheduleFile> sfs = scheduleFiles();
      for (auto& scheduleFile : sfs) {
        std::vector<openstudio::IdfObject> tmp2 = scheduleFile.remove();
        tmp.insert(tmp.end(), tmp2.begin(), tmp2.end());
      }

      // PythonPluginInstance
      std::vector<PythonPluginInstance> ppis = pythonPluginInstances();
      for (auto& pythonPluginInstance : ppis) {
        std::vector<openstudio::IdfObject> tmp2 = pythonPluginInstance.remove();
        tmp.insert(tmp.end(), tmp2.begin(), tmp2.end());
      }

      // ChillerElectricASHRAE205
      std::vector<ChillerElectricASHRAE205> chs = chillerElectricASHRAE205s();
      for (auto& ch : chs) {
        std::vector<openstudio::IdfObject> tmp2 = ch.remove();
        tmp.insert(tmp.end(), tmp2.begin(), tmp2.end());
      }

      std::vector<openstudio::IdfObject> idfObjects = ModelObject_Impl::remove();
      idfObjects.insert(idfObjects.end(), tmp.begin(), tmp.end());

      return idfObjects;
    }

    std::string ExternalFile_Impl::fileName() const {
      boost::optional<std::string> value = getString(OS_External_FileFields::FileName, true);
      OS_ASSERT(value);
      return value.get();
    }

    path ExternalFile_Impl::filePath() const {
      path result;
      path fname = toPath(fileName());
      if (openstudio::filesystem::is_regular_file(fname)) {
        return fname;
      }
      std::vector<path> absoluteFilePaths = this->model().workflowJSON().absoluteFilePaths();
      if (absoluteFilePaths.empty()) {
        result = this->model().workflowJSON().absoluteRootDir() / fname;
      } else {
        // Loop to find the file in all possible directories in order of preference
        // (eg goes to hardcoded paths, then potentially generated_files, then files, etc.)
        for (const openstudio::path& dirpath : absoluteFilePaths) {
          path p = dirpath / fname;
          if (openstudio::filesystem::is_regular_file(p)) {
            result = p;
            break;
          }
        }
      }
      return result;
    }

    //boost::optional<std::string> ExternalFile_Impl::columnSeparator() const {
    //  return getString(OS_External_FileFields::ColumnSeparator,true);
    //}

    //char ExternalFile_Impl::columnSeparatorChar() const {
    //  static std::unordered_map<std::string, char> lookup({ { "Comma", ',' }, { "Tab", '\t' }, { "Fixed", ' ' }, { "Semicolon", ';' } });
    //  boost::optional<std::string> value = getString(OS_External_FileFields::ColumnSeparator, true);
    //  OS_ASSERT(value);
    //  auto it = lookup.find(value.get());
    //  if (it == std::end(lookup)) {
    //    // Invalid separator
    //    return '\0';
    //  }
    //  return it->second;
    //}

    //bool ExternalFile_Impl::isColumnSeparatorDefaulted() const {
    //  return isEmpty(OS_External_FileFields::ColumnSeparator);
    //}

    bool ExternalFile_Impl::setFileName(const std::string& fileName) {
      bool result = setString(OS_External_FileFields::FileName, fileName);
      OS_ASSERT(result);
      return result;
    }

    //bool ExternalFile_Impl::setColumnSeparator(const std::string& columnSeparator) {
    //  bool result = setString(OS_External_FileFields::ColumnSeparator, columnSeparator);
    //  return result;
    //}

    //void ExternalFile_Impl::resetColumnSeparator() {
    //  bool result = setString(OS_External_FileFields::ColumnSeparator, "");
    //  OS_ASSERT(result);
    //}

    std::vector<ScheduleFile> ExternalFile_Impl::scheduleFiles() const {
      std::vector<ScheduleFile> result = getObject<ExternalFile>().getModelObjectSources<ScheduleFile>();
      return result;
    }

    std::vector<PythonPluginInstance> ExternalFile_Impl::pythonPluginInstances() const {
      std::vector<PythonPluginInstance> result = getObject<ExternalFile>().getModelObjectSources<PythonPluginInstance>();
      return result;
    }

    std::vector<ChillerElectricASHRAE205> ExternalFile_Impl::chillerElectricASHRAE205s() const {
      std::vector<ChillerElectricASHRAE205> result = getObject<ExternalFile>().getModelObjectSources<ChillerElectricASHRAE205>();
      return result;
    }

  }  // namespace detail

  boost::optional<ExternalFile> ExternalFile::getExternalFile(const Model& model, const std::string& filename, bool copyFile) {
    path p = toPath(filename);
    if (!p.has_filename()) {
      return boost::none;
    }
    std::string s;
    if (copyFile) {
      s = toString(p.filename());
    } else {
      s = filename;
    }

    // this factory method will give us better control if we start doing things like copying files
    for (const auto& externalFile : model.getConcreteModelObjects<ExternalFile>()) {

      // handle Windows filename case insensitivity?
      if (externalFile.fileName() == s) {
        return externalFile;
      }
    }

    try {
      return ExternalFile(model, filename, copyFile);
    } catch (std::exception&) {
    }
    return boost::none;
  }

  ExternalFile::ExternalFile(const Model& model, const std::string& filename, bool copyFile) : ResourceObject(ExternalFile::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ExternalFile_Impl>());

    WorkflowJSON workflow = model.workflowJSON();

    // we expect all strings to be UTF-8 encoded
    path p = toPath(filename);
    if (!exists(p)) {
      boost::optional<path> op = workflow.findFile(filename);
      if (!op) {
        this->remove();
        LOG_AND_THROW("Cannot find file \"" << filename << "\" for " << this->briefDescription());
      }
      p = op.get();
    }
    OS_ASSERT(exists(p));

    bool ok;
    if (copyFile) {
      path destDir;
      std::vector<path> absoluteFilePaths = workflow.absoluteFilePaths();
      if (absoluteFilePaths.empty()) {
        destDir = workflow.absoluteRootDir();
      } else {
        destDir = absoluteFilePaths[0];
      }
      path dest = destDir / p.filename();

      if (exists(dest)) {
        if (checksum(p) != checksum(dest)) {
          this->remove();
          LOG_AND_THROW("File \"" << p.filename() << "\" already exists in \"" << destDir << "\"");
        }
      } else {

        try {
          makeParentFolder(dest, path(), true);
        } catch (std::exception&) {
          this->remove();
          LOG_AND_THROW("Failed to created parent folder at \"" << dest << "\"");
        }
        try {
          boost::filesystem::copy(p, dest);
        } catch (std::exception&) {
          this->remove();
          LOG_AND_THROW("Failed to copy file from \"" << p << "\" to \"" << dest << "\"");
        }
      }
      OS_ASSERT(exists(dest));

      ok = setFileName(toString(dest.filename()));
      OS_ASSERT(ok);
    } else {
      ok = setFileName(filename);
      OS_ASSERT(ok);
    }
  }

  IddObjectType ExternalFile::iddObjectType() {
    return {IddObjectType::OS_External_File};
  }

  std::vector<std::string> ExternalFile::columnSeparatorValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_External_FileFields::ColumnSeparator);
  }

  std::string ExternalFile::fileName() const {
    return getImpl<detail::ExternalFile_Impl>()->fileName();
  }

  path ExternalFile::filePath() const {
    return getImpl<detail::ExternalFile_Impl>()->filePath();
  }

  //boost::optional<std::string> ExternalFile::columnSeparator() const {
  //  return getImpl<detail::ExternalFile_Impl>()->columnSeparator();
  //}

  //bool ExternalFile::isColumnSeparatorDefaulted() const {
  //  return getImpl<detail::ExternalFile_Impl>()->isColumnSeparatorDefaulted();
  //}

  bool ExternalFile::setFileName(const std::string& fileName) {
    return getImpl<detail::ExternalFile_Impl>()->setFileName(fileName);
  }

  //bool ExternalFile::setColumnSeparator(const std::string& columnSeparator) {
  //  return getImpl<detail::ExternalFile_Impl>()->setColumnSeparator(columnSeparator);
  //}

  //void ExternalFile::resetColumnSeparator() {
  //  getImpl<detail::ExternalFile_Impl>()->resetColumnSeparator();
  //}

  //bool ExternalFile::isValid() {
  //  return getImpl<detail::ExternalFile_Impl>()->isValid();
  //}

  std::vector<ScheduleFile> ExternalFile::scheduleFiles() const {
    return getImpl<detail::ExternalFile_Impl>()->scheduleFiles();
  }

  std::vector<PythonPluginInstance> ExternalFile::pythonPluginInstances() const {
    return getImpl<detail::ExternalFile_Impl>()->pythonPluginInstances();
  }

  std::vector<ChillerElectricASHRAE205> ExternalFile::chillerElectricASHRAE205s() const {
    return getImpl<detail::ExternalFile_Impl>()->chillerElectricASHRAE205s();
  }

  /// @cond
  ExternalFile::ExternalFile(std::shared_ptr<detail::ExternalFile_Impl> impl) : ResourceObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
