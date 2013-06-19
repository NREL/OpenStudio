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

#include <analysis/ModelRulesetPerturbation.hpp>
#include <analysis/ModelRulesetPerturbation_Impl.hpp>

#include <runmanager/lib/WorkItem.hpp>

#include <utilities/core/FileReference.hpp>

namespace openstudio {
namespace analysis {

namespace detail {

  ModelRulesetPerturbation_Impl::ModelRulesetPerturbation_Impl(
      const ruleset::ModelRuleset& ruleset,bool isSelected)
    : DiscretePerturbation_Impl(isSelected), m_ruleset(ruleset)
  {}

  /** Constructor provided for deserialization; not for general use. */
  ModelRulesetPerturbation_Impl::ModelRulesetPerturbation_Impl(
      const UUID& uuid,
      const UUID& versionUUID,
      const std::string& name,
      const std::string& displayName,
      const std::string& description,
      bool isSelected,
      const ruleset::ModelRuleset& ruleset)
    : DiscretePerturbation_Impl(uuid,versionUUID,name,displayName,description,isSelected),
      m_ruleset(ruleset)
  {}

  ModelRulesetPerturbation_Impl::ModelRulesetPerturbation_Impl(const ModelRulesetPerturbation_Impl &other)
    : DiscretePerturbation_Impl(other),
      m_ruleset(other.ruleset())
  {
    // TODO: Clone ruleset
  }

  AnalysisObject ModelRulesetPerturbation_Impl::clone() const {
    boost::shared_ptr<ModelRulesetPerturbation_Impl> impl(new ModelRulesetPerturbation_Impl(*this));
    return ModelRulesetPerturbation(impl);
  }

  boost::optional<FileReferenceType> ModelRulesetPerturbation_Impl::inputFileType() const {
    return FileReferenceType(FileReferenceType::OSM);
  }

  boost::optional<FileReferenceType> ModelRulesetPerturbation_Impl::outputFileType() const {
    return FileReferenceType(FileReferenceType::OSM);
  }

  runmanager::WorkItem ModelRulesetPerturbation_Impl::createWorkItem(
      const openstudio::path& rubyIncludeDirectory) const
  {
    return openstudio::runmanager::WorkItem(m_ruleset);
  }

  ruleset::ModelRuleset ModelRulesetPerturbation_Impl::ruleset() const
  {
    return m_ruleset;
  }

} // detail

ModelRulesetPerturbation::ModelRulesetPerturbation(const ruleset::ModelRuleset& ruleset,
                                                   bool isSelected)
  : DiscretePerturbation(boost::shared_ptr<detail::ModelRulesetPerturbation_Impl>(
        new detail::ModelRulesetPerturbation_Impl(ruleset,isSelected)))
{}

ModelRulesetPerturbation::ModelRulesetPerturbation(const UUID& uuid,
                                                   const UUID& versionUUID,
                                                   const std::string& name,
                                                   const std::string& displayName,
                                                   const std::string& description,
                                                   bool isSelected,
                                                   const ruleset::ModelRuleset& ruleset)
  : DiscretePerturbation(boost::shared_ptr<detail::ModelRulesetPerturbation_Impl>(
        new detail::ModelRulesetPerturbation_Impl(uuid,
                                                  versionUUID,
                                                  name,
                                                  displayName,
                                                  description,
                                                  isSelected,
                                                  ruleset)))
{}

ruleset::ModelRuleset ModelRulesetPerturbation::ruleset() const
{
  return getImpl<detail::ModelRulesetPerturbation_Impl>()->ruleset();
}

/// @cond
ModelRulesetPerturbation::ModelRulesetPerturbation(boost::shared_ptr<detail::ModelRulesetPerturbation_Impl> impl)
  : DiscretePerturbation(impl)
{}
/// @endcond


} // analysis
} // openstudio

