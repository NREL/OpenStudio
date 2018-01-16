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

#include "RunOptions.hpp"
#include "RunOptions_Impl.hpp"

#include "../core/Assert.hpp"

#include <jsoncpp/json.h>

namespace openstudio{
namespace detail{

  RunOptions_Impl::RunOptions_Impl()
    : m_debug(false), m_preserveRunDir(false), m_cleanup(true)
  {}

  RunOptions_Impl::~RunOptions_Impl()
  {}

  void RunOptions_Impl::onUpdate()
  {
    this->onChange.nano_emit();
  }

  std::string RunOptions_Impl::string() const
  {
    Json::Value result;

    if (m_debug){
      result["debug"] = m_debug;
    }

    if (m_preserveRunDir){
      result["preserve_run_dir"] = m_preserveRunDir;
    }

    if (m_customOutputAdapter){
      Json::Value outputAdapter;
      outputAdapter["custom_file_name"] = m_customOutputAdapter->customFileName();
      outputAdapter["class_name"] = m_customOutputAdapter->className();

      Json::Reader reader;
      Json::Value options;

      bool parsingSuccessful = reader.parse(m_customOutputAdapter->options(), options);
      if (parsingSuccessful){
        outputAdapter["options"] = options;
      }

      result["output_adapter"] = outputAdapter;
    }

    Json::StyledWriter writer;
    return writer.write(result);
  }

  bool RunOptions_Impl::debug() const
  {
    return m_debug;
  }

  bool RunOptions_Impl::setDebug(bool debug)
  {
    m_debug = debug;
    onUpdate();
    return true;
  }

  void RunOptions_Impl::resetDebug()
  {
    m_debug = false;
    onUpdate();
  }

  bool RunOptions_Impl::preserveRunDir() const
  {
    return m_preserveRunDir;
  }

  bool RunOptions_Impl::setPreserveRunDir(bool preserve)
  {
    m_preserveRunDir = preserve;
    onUpdate();
    return true;
  }

  void RunOptions_Impl::resetPreserveRunDir()
  {
    m_preserveRunDir = false;
    onUpdate();
  }

  bool RunOptions_Impl::cleanup() const
  {
    return m_cleanup;
  }

  bool RunOptions_Impl::setCleanup(bool cleanup)
  {
    m_cleanup = cleanup;
    onUpdate();
    return true;
  }

  void RunOptions_Impl::resetCleanup()
  {
    m_cleanup = true;
    onUpdate();
  }

  boost::optional<CustomOutputAdapter> RunOptions_Impl::customOutputAdapter() const
  {
    return m_customOutputAdapter;
  }

  bool RunOptions_Impl::setCustomOutputAdapter(const CustomOutputAdapter& adapter)
  {
    m_customOutputAdapter = adapter;
    onUpdate();
    return true;
  }

  void RunOptions_Impl::resetCustomOutputAdapter()
  {
    m_customOutputAdapter.reset();
    onUpdate();
  }

} //detail

CustomOutputAdapter::CustomOutputAdapter(const std::string& customFileName, const std::string& className, const std::string& options)
  : m_customFileName(customFileName), m_className(className), m_options(options)
{}

std::string CustomOutputAdapter::customFileName() const
{
  return m_customFileName;
}

std::string CustomOutputAdapter::className() const
{
  return m_className;
}

std::string CustomOutputAdapter::options() const
{
  return m_options;
}

RunOptions::RunOptions()
  : m_impl(std::shared_ptr<detail::RunOptions_Impl>(new detail::RunOptions_Impl()))
{
  OS_ASSERT(getImpl<detail::RunOptions_Impl>());
}

RunOptions::~RunOptions()
{}

boost::optional<RunOptions> RunOptions::fromString(const std::string& s)
{
  boost::optional<RunOptions> result;

  Json::Reader reader;
  Json::Value value;
  bool parsingSuccessful = reader.parse(s, value);
  if (!parsingSuccessful){
    return result;
  }

  result = RunOptions();

  if (value.isMember("debug") && value["debug"].isBool()){
    result->setDebug(value["debug"].asBool());
  }

  if (value.isMember("preserve_run_dir") && value["preserve_run_dir"].isBool()){
    result->setPreserveRunDir(value["preserve_run_dir"].asBool());
  }

  if (value.isMember("output_adapter")){
    Json::Value outputAdapter = value["output_adapter"];
    if (outputAdapter.isMember("custom_file_name") && outputAdapter.isMember("class_name")){
      std::string customFileName = outputAdapter["custom_file_name"].asString();
      std::string className = outputAdapter["class_name"].asString();
      Json::Value options = outputAdapter["options"];

      Json::StyledWriter writer;
      std::string optionString = writer.write(options);

      CustomOutputAdapter coa(customFileName, className, optionString);
      result->setCustomOutputAdapter(coa);
    }
  }

  return result;
}

std::string RunOptions::string() const
{
  return getImpl<detail::RunOptions_Impl>()->string();
}

bool RunOptions::debug() const
{
  return getImpl<detail::RunOptions_Impl>()->debug();
}

bool RunOptions::setDebug(bool debug)
{
  return getImpl<detail::RunOptions_Impl>()->setDebug(debug);
}
void RunOptions::resetDebug()
{
  getImpl<detail::RunOptions_Impl>()->resetDebug();
}

bool RunOptions::preserveRunDir() const
{
  return getImpl<detail::RunOptions_Impl>()->preserveRunDir();
}

bool RunOptions::setPreserveRunDir(bool preserve)
{
  return getImpl<detail::RunOptions_Impl>()->setPreserveRunDir(preserve);
}

void RunOptions::resetPreserveRunDir()
{
  getImpl<detail::RunOptions_Impl>()->resetPreserveRunDir();
}

bool RunOptions::cleanup() const
{
  return getImpl<detail::RunOptions_Impl>()->cleanup();
}

bool RunOptions::setCleanup(bool cleanup)
{
  return getImpl<detail::RunOptions_Impl>()->setCleanup(cleanup);
}

void RunOptions::resetCleanup()
{
  getImpl<detail::RunOptions_Impl>()->resetCleanup();
}

boost::optional<CustomOutputAdapter> RunOptions::customOutputAdapter() const
{
  return getImpl<detail::RunOptions_Impl>()->customOutputAdapter();
}

bool RunOptions::setCustomOutputAdapter(const CustomOutputAdapter& adapter)
{
  return getImpl<detail::RunOptions_Impl>()->setCustomOutputAdapter(adapter);
}

void RunOptions::resetCustomOutputAdapter()
{
  getImpl<detail::RunOptions_Impl>()->resetCustomOutputAdapter();
}

std::ostream& operator<<(std::ostream& os, const RunOptions& runOptions)
{
  os << runOptions.string();
  return os;
}

} // openstudio
