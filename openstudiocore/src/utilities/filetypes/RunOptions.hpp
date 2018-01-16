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

#ifndef UTILITIES_FILETYPES_RUNOPTIONS_HPP
#define UTILITIES_FILETYPES_RUNOPTIONS_HPP

#include "../UtilitiesAPI.hpp"

#include "../core/Logger.hpp"
#include "../data/Variant.hpp"
#include "../data/Attribute.hpp"

#include <jsoncpp/json.h>

namespace openstudio{
namespace detail{
  class WorkflowJSON_Impl;
  class RunOptions_Impl;
}

class UTILITIES_API CustomOutputAdapter {
public:
  CustomOutputAdapter(const std::string& customFileName, const std::string& className, const std::string& options);
  std::string customFileName() const;
  std::string className() const;
  std::string options() const;
private:
  std::string m_customFileName;
  std::string m_className;
  std::string m_options;
};

/** Base class for defining a run options for a OpenStudio Workflow. */
class UTILITIES_API RunOptions
{
public:

  RunOptions();

  virtual ~RunOptions();

  /// Construct from JSON formatted string
  static boost::optional<RunOptions> fromString(const std::string& s);

  /// Serialize to JSON formatted string
  std::string string() const;

  bool debug() const;
  bool setDebug(bool debug);
  void resetDebug();

  bool preserveRunDir() const;
  bool setPreserveRunDir(bool preserve);
  void resetPreserveRunDir();

  bool cleanup() const;
  bool setCleanup(bool cleanup);
  void resetCleanup();

  boost::optional<CustomOutputAdapter> customOutputAdapter() const;
  bool setCustomOutputAdapter(const CustomOutputAdapter& adapter);
  void resetCustomOutputAdapter();

protected:

  // get the impl
  template<typename T>
  std::shared_ptr<T> getImpl() const {
    return std::dynamic_pointer_cast<T>(m_impl);
  }

  friend class detail::WorkflowJSON_Impl;

private:

  // configure logging
  REGISTER_LOGGER("openstudio.RunOptions");

  // pointer to implementation
  std::shared_ptr<detail::RunOptions_Impl> m_impl;

};

UTILITIES_API std::ostream& operator<<(std::ostream& os, const RunOptions& runOptions);

} // openstudio

#endif //UTILITIES_FILETYPES_RUNOPTIONS_HPP
