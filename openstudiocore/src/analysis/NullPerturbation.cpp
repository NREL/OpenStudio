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

#include <analysis/NullPerturbation.hpp>
#include <analysis/NullPerturbation_Impl.hpp>

#include <runmanager/lib/WorkItem.hpp>

#include <utilities/core/FileReference.hpp>

namespace openstudio {
namespace analysis {

namespace detail {

  NullPerturbation_Impl::NullPerturbation_Impl(bool isSelected)
    : DiscretePerturbation_Impl(isSelected)
  {}

  NullPerturbation_Impl::NullPerturbation_Impl(const UUID& uuid,
                                               const UUID& versionUUID,
                                               const std::string& name,
                                               const std::string& displayName,
                                               const std::string& description,
                                               bool isSelected)
    : DiscretePerturbation_Impl(uuid,versionUUID,name,displayName,description,isSelected)
  {}

  NullPerturbation_Impl::NullPerturbation_Impl(const NullPerturbation_Impl &other)
    : DiscretePerturbation_Impl(other)
  {}

  AnalysisObject NullPerturbation_Impl::clone() const {
    boost::shared_ptr<NullPerturbation_Impl> impl(new NullPerturbation_Impl(*this));
    return NullPerturbation(impl);
  }

  boost::optional<FileReferenceType> NullPerturbation_Impl::inputFileType() const {
    return boost::none;
  }

  boost::optional<FileReferenceType> NullPerturbation_Impl::outputFileType() const {
    return boost::none;
  }

  runmanager::WorkItem NullPerturbation_Impl::createWorkItem(
      const openstudio::path& rubyIncludeDirectory) const
  {
    return runmanager::WorkItem(runmanager::JobType::Null);
  }

} // detail

NullPerturbation::NullPerturbation(bool isSelected)
  : DiscretePerturbation(boost::shared_ptr<detail::NullPerturbation_Impl>(
        new detail::NullPerturbation_Impl(isSelected)))
{}

NullPerturbation::NullPerturbation(const UUID& uuid,
                                   const UUID& versionUUID,
                                   const std::string& name,
                                   const std::string& displayName,
                                   const std::string& description,
                                   bool isSelected)
  : DiscretePerturbation(boost::shared_ptr<detail::NullPerturbation_Impl>(
        new detail::NullPerturbation_Impl(uuid,
                                          versionUUID,
                                          name,
                                          displayName,
                                          description,
                                          isSelected)))
{}

/// @cond
NullPerturbation::NullPerturbation(boost::shared_ptr<detail::NullPerturbation_Impl> impl)
  : DiscretePerturbation(impl)
{}
/// @endcond

} // analysis
} // openstudio

