/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "ExternalFile.hpp"
#include "ExternalFile_Impl.hpp"

#include "Model.hpp"
#include "ScheduleFile.hpp"

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

    ExternalFile_Impl::~ExternalFile_Impl() {}

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
      std::vector<ScheduleFile> sfs = scheduleFiles();
      for (auto& scheduleFile : sfs) {
        std::vector<openstudio::IdfObject> tmp2 = scheduleFile.remove();
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
      std::vector<path> absoluteFilePaths = this->model().workflowJSON().absoluteFilePaths();
      if (absoluteFilePaths.empty()) {
        result = this->model().workflowJSON().absoluteRootDir() / fname;
      } else {
        // Loop to find the file in all possible directories in order of preference
        // (eg goes to hardcoded paths, then potentially generated_files, then files, etc.)
        for (const openstudio::path& dirpath : absoluteFilePaths) {
          path p = dirpath / fname;
          if (openstudio::filesystem::exists(p) || openstudio::filesystem::is_regular_file(p)) {
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

  }  // namespace detail

  boost::optional<ExternalFile> ExternalFile::getExternalFile(const Model& model, const std::string& filename) {
    path p = toPath(filename);
    if (!p.has_filename()) {
      return boost::none;
    }
    std::string s = toString(p.filename());

    // this factory method will give us better control if we start doing things like copying files
    for (const auto& externalFile : model.getConcreteModelObjects<ExternalFile>()) {

      // handle Windows filename case insensitivity?
      if (externalFile.fileName() == s) {
        return externalFile;
      }
    }

    try {
      return ExternalFile(model, filename);
    } catch (std::exception&) {
    }
    return boost::none;
  }

  ExternalFile::ExternalFile(const Model& model, const std::string& filename) : ResourceObject(ExternalFile::iddObjectType(), model) {
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
        boost::filesystem::copy(p, dest);
      } catch (std::exception&) {
        this->remove();
        LOG_AND_THROW("Failed to copy file from \"" << p << "\" to \"" << dest << "\"");
      }
    }
    OS_ASSERT(exists(dest));

    bool ok;
    ok = setFileName(toString(dest.filename()));
    OS_ASSERT(ok);
  }

  IddObjectType ExternalFile::iddObjectType() {
    return IddObjectType(IddObjectType::OS_External_File);
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

  /// @cond
  ExternalFile::ExternalFile(std::shared_ptr<detail::ExternalFile_Impl> impl) : ResourceObject(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
