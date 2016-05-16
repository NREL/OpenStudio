/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#include "WorkflowStepResult.hpp"
#include "WorkflowStepResult_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace detail {

  WorkflowStepResult_Impl::WorkflowStepResult_Impl()
    : m_stepResult(StepResult::Success)
  {}

  DateTime WorkflowStepResult_Impl::startedAt()
  {
    return m_startedAt;
  }

  DateTime WorkflowStepResult_Impl::completedAt()
  {
    return m_completedAt;
  }

  StepResult WorkflowStepResult_Impl::stepResult() const
  {
    return m_stepResult;
  }

  boost::optional<std::string> WorkflowStepResult_Impl::initialCondition() const
  {
    return m_initialCondition;
  }

  boost::optional<std::string> WorkflowStepResult_Impl::finalCondition() const
  {
    return m_finalCondition;
  }

  std::vector<std::string> WorkflowStepResult_Impl::stepErrors() const
  {
    return m_stepErrors;
  }

  std::vector<std::string> WorkflowStepResult_Impl::stepWarnings() const
  {
    return m_stepWarnings;
  }

  std::vector<std::string> WorkflowStepResult_Impl::stepInfo() const
  {
    return m_stepInfo;
  }

  std::vector<std::pair<std::string, Variant> > WorkflowStepResult_Impl::stepValues() const
  {
    return m_stepValues;
  }

  std::vector<openstudio::path> WorkflowStepResult_Impl::stepFiles() const
  {
    return m_stepFiles;
  }

  std::string WorkflowStepResult_Impl::stdOut() const
  {
    return m_stdOut;
  }

  std::string WorkflowStepResult_Impl::stdErr() const
  {
    return m_stdErr;
  }

} // detail

WorkflowStepResult::WorkflowStepResult()
  : m_impl(std::shared_ptr<detail::WorkflowStepResult_Impl>(new detail::WorkflowStepResult_Impl))
{
  OS_ASSERT(getImpl<detail::WorkflowStepResult_Impl>());
}

DateTime WorkflowStepResult::startedAt()
{
  return getImpl<detail::WorkflowStepResult_Impl>()->startedAt();
}

DateTime WorkflowStepResult::completedAt()
{
  return getImpl<detail::WorkflowStepResult_Impl>()->completedAt();
}

StepResult WorkflowStepResult::stepResult() const
{
  return getImpl<detail::WorkflowStepResult_Impl>()->stepResult();
}

boost::optional<std::string> WorkflowStepResult::initialCondition() const
{
  return getImpl<detail::WorkflowStepResult_Impl>()->initialCondition();
}

boost::optional<std::string> WorkflowStepResult::finalCondition() const
{
  return getImpl<detail::WorkflowStepResult_Impl>()->finalCondition();
}

std::vector<std::string> WorkflowStepResult::stepErrors() const
{
  return getImpl<detail::WorkflowStepResult_Impl>()->stepErrors();
}

std::vector<std::string> WorkflowStepResult::stepWarnings() const
{
  return getImpl<detail::WorkflowStepResult_Impl>()->stepWarnings();
}

std::vector<std::string> WorkflowStepResult::stepInfo() const
{
  return getImpl<detail::WorkflowStepResult_Impl>()->stepInfo();
}

std::vector<std::pair<std::string, Variant> > WorkflowStepResult::stepValues() const
{
  return getImpl<detail::WorkflowStepResult_Impl>()->stepValues();
}

std::vector<openstudio::path> WorkflowStepResult::stepFiles() const
{
  return getImpl<detail::WorkflowStepResult_Impl>()->stepFiles();
}

std::string WorkflowStepResult::stdOut() const
{
  return getImpl<detail::WorkflowStepResult_Impl>()->stdOut();
}

std::string WorkflowStepResult::stdErr() const
{
  return getImpl<detail::WorkflowStepResult_Impl>()->stdErr();
}


} // openstudio
