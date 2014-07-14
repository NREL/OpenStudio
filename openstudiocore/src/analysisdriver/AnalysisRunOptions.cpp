/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

