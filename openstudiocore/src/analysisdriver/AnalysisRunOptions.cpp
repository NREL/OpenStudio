/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "AnalysisRunOptions.hpp"

namespace openstudio {
namespace analysisdriver {

AnalysisRunOptions::AnalysisRunOptions(const openstudio::path& workingDirectory,
                                       const openstudio::path& rubyIncludeDirectory,
                                       const openstudio::path& dakotaExePath)
  : m_workingDirectory(workingDirectory),
    m_queuePausingBehavior(QueuePausingBehavior::NoPause),
    m_jobCleanUpBehavior(JobCleanUpBehavior::standard),
    m_rubyIncludeDirectory(rubyIncludeDirectory),
    m_force(false),
    m_firstN(4),
    m_dakotaExePath(dakotaExePath),
    m_dakotaFileSave(true)
{}

openstudio::path AnalysisRunOptions::workingDirectory() const {
  return m_workingDirectory;
}

openstudio::path AnalysisRunOptions::rubyIncludeDirectory() const {
  return m_rubyIncludeDirectory;
}

QueuePausingBehavior AnalysisRunOptions::queuePausingBehavior() const {
  return m_queuePausingBehavior;
}

JobCleanUpBehavior AnalysisRunOptions::jobCleanUpBehavior() const {
  return m_jobCleanUpBehavior;
}

boost::optional<int> AnalysisRunOptions::queueSize() const {
  return m_queueSize;
}

std::vector<openstudio::URLSearchPath> AnalysisRunOptions::urlSearchPaths() const {
  return m_urlSearchPaths;
}

bool AnalysisRunOptions::force() const {
  return m_force;
}

int AnalysisRunOptions::firstN() const {
  return m_firstN;
}

boost::optional<runmanager::Tools> AnalysisRunOptions::runManagerTools() const {
  return m_tools;
}

openstudio::path AnalysisRunOptions::dakotaExePath() const {
  return m_dakotaExePath;
}

bool AnalysisRunOptions::dakotaFileSave() const {
  return m_dakotaFileSave;
}

void AnalysisRunOptions::setRubyIncludeDirectory(const openstudio::path& includeDir) {
  m_rubyIncludeDirectory = includeDir;
}

void AnalysisRunOptions::setQueuePausingBehavior(const QueuePausingBehavior& queueBehavior) {
  m_queuePausingBehavior = queueBehavior;
}

void AnalysisRunOptions::setJobCleanUpBehavior(const JobCleanUpBehavior& jobCleanUpBehavior) {
  m_jobCleanUpBehavior = jobCleanUpBehavior;
}

void AnalysisRunOptions::setQueueSize(int queueSize) {
  m_queueSize = queueSize;
}

void AnalysisRunOptions::clearQueueSize() {
  m_queueSize.reset();
}

void AnalysisRunOptions::setUrlSearchPaths(
    const std::vector<openstudio::URLSearchPath>& searchPaths)
{
  m_urlSearchPaths = searchPaths;
}

void AnalysisRunOptions::appendUrlSearchPath(const openstudio::URLSearchPath& searchPath) {
  m_urlSearchPaths.push_back(searchPath);
}

void AnalysisRunOptions::clearUrlSearchPaths() {
  m_urlSearchPaths.clear();
}

void AnalysisRunOptions::setForce(bool force) {
  m_force = force;
}

void AnalysisRunOptions::setFirstN(int firstN) {
  m_firstN = firstN;
}

void AnalysisRunOptions::setRunManagerTools(const runmanager::Tools& tools) {
  m_tools = tools;
}

void AnalysisRunOptions::clearRunManagerTools() {
  m_tools.reset();
}

void AnalysisRunOptions::setDakotaExePath(const openstudio::path& dakotaPath) {
  m_dakotaExePath = dakotaPath;
}

void AnalysisRunOptions::setDakotaFileSave(bool value) {
  m_dakotaFileSave = value;
}

} // analysisdriver
} // openstudio

