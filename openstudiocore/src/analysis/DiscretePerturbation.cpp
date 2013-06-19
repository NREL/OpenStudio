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

#include <analysis/DiscretePerturbation.hpp>
#include <analysis/DiscretePerturbation_Impl.hpp>

#include <utilities/core/FileReference.hpp>

#include <runmanager/lib/WorkItem.hpp>

namespace openstudio {
namespace analysis {

namespace detail {

  DiscretePerturbation_Impl::DiscretePerturbation_Impl(bool isSelected)
    : AnalysisObject_Impl(std::string()), m_isSelected(isSelected)
  {}

  DiscretePerturbation_Impl::DiscretePerturbation_Impl(const UUID& uuid,
                                                       const UUID& versionUUID,
                                                       const std::string& name,
                                                       const std::string& displayName,
                                                       const std::string& description,
                                                       bool isSelected)
    : AnalysisObject_Impl(uuid,versionUUID,name,displayName,description),
      m_isSelected(isSelected)
  {}

  DiscretePerturbation_Impl::DiscretePerturbation_Impl(const DiscretePerturbation_Impl &other)
    : AnalysisObject_Impl(other),
      m_isSelected(other.isSelected())
  {}

  bool DiscretePerturbation_Impl::isSelected() const {
    return m_isSelected;
  }

  void DiscretePerturbation_Impl::setIsSelected(bool isSelected) {
    m_isSelected = isSelected;
    onChange(AnalysisObject_Impl::Benign);
  }

} // detail

bool DiscretePerturbation::isSelected() const {
  return getImpl<detail::DiscretePerturbation_Impl>()->isSelected();
}

boost::optional<FileReferenceType> DiscretePerturbation::inputFileType() const {
  return getImpl<detail::DiscretePerturbation_Impl>()->inputFileType();
}

boost::optional<FileReferenceType> DiscretePerturbation::outputFileType() const {
  return getImpl<detail::DiscretePerturbation_Impl>()->outputFileType();
}

void DiscretePerturbation::setIsSelected(bool isSelected) {
  getImpl<detail::DiscretePerturbation_Impl>()->setIsSelected(isSelected);
}

runmanager::WorkItem DiscretePerturbation::createWorkItem(
    const openstudio::path& rubyIncludeDirectory) const
{
  return getImpl<detail::DiscretePerturbation_Impl>()->createWorkItem(rubyIncludeDirectory);
}

/// @cond
DiscretePerturbation::DiscretePerturbation(boost::shared_ptr<detail::DiscretePerturbation_Impl> impl)
  : AnalysisObject(impl)
{}
/// @endcond

} // analysis
} // openstudio

