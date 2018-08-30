/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "ExternalFile.hpp"
#include "ExternalFile_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_External_File_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

#include <unordered_map>

namespace openstudio {
namespace model {

namespace detail {

  ExternalFile_Impl::ExternalFile_Impl(const IdfObject& idfObject,
                                       Model_Impl* model,
                                       bool keepHandle)
    : ResourceObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ExternalFile::iddObjectType());
  }

  ExternalFile_Impl::ExternalFile_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                       Model_Impl* model,
                                       bool keepHandle)
    : ResourceObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ExternalFile::iddObjectType());
  }

  ExternalFile_Impl::ExternalFile_Impl(const ExternalFile_Impl& other,
                                       Model_Impl* model,
                                       bool keepHandle)
    : ResourceObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ExternalFile_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ExternalFile_Impl::iddObjectType() const {
    return ExternalFile::iddObjectType();
  }

  std::string ExternalFile_Impl::fileName() const {
    boost::optional<std::string> value = getString(OS_External_FileFields::FileName,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<std::string> ExternalFile_Impl::columnSeparator() const {
    return getString(OS_External_FileFields::ColumnSeparator,true);
  }

  char ExternalFile_Impl::columnSeparatorChar() const {
    static std::unordered_map<std::string, char> lookup({ { "Comma", ',' }, { "Tab", '\t' }, { "Fixed", ' ' }, { "Semicolon", ';' } });
    boost::optional<std::string> value = getString(OS_External_FileFields::ColumnSeparator, true);
    OS_ASSERT(value);
    auto it = lookup.find(value.get());
    if (it == std::end(lookup)) {
      // Invalid separator
      return '\0';
    }
    return it->second;
  }

  bool ExternalFile_Impl::isColumnSeparatorDefaulted() const {
    return isEmpty(OS_External_FileFields::ColumnSeparator);
  }

  bool ExternalFile_Impl::setFileName(const std::string& fileName) {
    bool result = setString(OS_External_FileFields::FileName, fileName);
    OS_ASSERT(result);
    return result;
  }

  bool ExternalFile_Impl::setColumnSeparator(const std::string& columnSeparator) {
    bool result = setString(OS_External_FileFields::ColumnSeparator, columnSeparator);
    return result;
  }

  void ExternalFile_Impl::resetColumnSeparator() {
    bool result = setString(OS_External_FileFields::ColumnSeparator, "");
    OS_ASSERT(result);
  }

} // detail

ExternalFile::ExternalFile(const Model& model, const std::string &filename)
  : ResourceObject(ExternalFile::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ExternalFile_Impl>());

  std::ifstream ifs;
  ifs.open(filename, std::ifstream::in);
  if (!ifs.is_open()) {
    // Failed to open file
    LOG_AND_THROW("Failed to open file \"" << filename << "\" for " << this->briefDescription());
  }
  ifs.close();

  bool ok;
  ok = setFileName(filename);
  OS_ASSERT(ok);

  // Moving the file etc. should go here
}

IddObjectType ExternalFile::iddObjectType() {
  return IddObjectType(IddObjectType::OS_External_File);
}

std::vector<std::string> ExternalFile::columnSeparatorValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_External_FileFields::ColumnSeparator);
}

std::string ExternalFile::fileName() const {
  return getImpl<detail::ExternalFile_Impl>()->fileName();
}

boost::optional<std::string> ExternalFile::columnSeparator() const {
  return getImpl<detail::ExternalFile_Impl>()->columnSeparator();
}

bool ExternalFile::isColumnSeparatorDefaulted() const {
  return getImpl<detail::ExternalFile_Impl>()->isColumnSeparatorDefaulted();
}

bool ExternalFile::setFileName(const std::string& fileName) {
  return getImpl<detail::ExternalFile_Impl>()->setFileName(fileName);
}

bool ExternalFile::setColumnSeparator(const std::string& columnSeparator) {
  return getImpl<detail::ExternalFile_Impl>()->setColumnSeparator(columnSeparator);
}

void ExternalFile::resetColumnSeparator() {
  getImpl<detail::ExternalFile_Impl>()->resetColumnSeparator();
}

//bool ExternalFile::isValid() {
//  return getImpl<detail::ExternalFile_Impl>()->isValid();
//}

/// @cond
ExternalFile::ExternalFile(std::shared_ptr<detail::ExternalFile_Impl> impl)
  : ResourceObject(impl)
{}
/// @endcond

} // model
} // openstudio

