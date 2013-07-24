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

#include <analysis/Measure.hpp>
#include <analysis/Measure_Impl.hpp>

#include <utilities/core/FileReference.hpp>

#include <runmanager/lib/WorkItem.hpp>

namespace openstudio {
namespace analysis {

namespace detail {

  Measure_Impl::Measure_Impl(bool isSelected)
    : AnalysisObject_Impl(std::string()), m_isSelected(isSelected)
  {}

  Measure_Impl::Measure_Impl(const UUID& uuid,
                             const UUID& versionUUID,
                             const std::string& name,
                             const std::string& displayName,
                             const std::string& description,
                             bool isSelected)
    : AnalysisObject_Impl(uuid,versionUUID,name,displayName,description),
      m_isSelected(isSelected)
  {}

  Measure_Impl::Measure_Impl(const Measure_Impl &other)
    : AnalysisObject_Impl(other),
      m_isSelected(other.isSelected())
  {}

  bool Measure_Impl::isSelected() const {
    return m_isSelected;
  }

  void Measure_Impl::setIsSelected(bool isSelected) {
    m_isSelected = isSelected;
    onChange(AnalysisObject_Impl::Benign);
  }

} // detail

bool Measure::isSelected() const {
  return getImpl<detail::Measure_Impl>()->isSelected();
}

boost::optional<FileReferenceType> Measure::inputFileType() const {
  return getImpl<detail::Measure_Impl>()->inputFileType();
}

boost::optional<FileReferenceType> Measure::outputFileType() const {
  return getImpl<detail::Measure_Impl>()->outputFileType();
}

void Measure::setIsSelected(bool isSelected) {
  getImpl<detail::Measure_Impl>()->setIsSelected(isSelected);
}

runmanager::WorkItem Measure::createWorkItem(
    const openstudio::path& rubyIncludeDirectory) const
{
  return getImpl<detail::Measure_Impl>()->createWorkItem(rubyIncludeDirectory);
}

/// @cond
Measure::Measure(boost::shared_ptr<detail::Measure_Impl> impl)
  : AnalysisObject(impl)
{}
/// @endcond

} // analysis
} // openstudio

