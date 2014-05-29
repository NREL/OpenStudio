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

#include "NullMeasure.hpp"
#include "NullMeasure_Impl.hpp"

#include "../runmanager/lib/WorkItem.hpp"

#include "../utilities/core/FileReference.hpp"

namespace openstudio {
namespace analysis {

namespace detail {

  NullMeasure_Impl::NullMeasure_Impl(bool isSelected)
    : Measure_Impl(isSelected)
  {}

  NullMeasure_Impl::NullMeasure_Impl(const UUID& uuid,
                                     const UUID& versionUUID,
                                     const std::string& name,
                                     const std::string& displayName,
                                     const std::string& description,
                                     bool isSelected)
    : Measure_Impl(uuid,versionUUID,name,displayName,description,isSelected)
  {}

  NullMeasure_Impl::NullMeasure_Impl(const NullMeasure_Impl &other)
    : Measure_Impl(other)
  {}

  AnalysisObject NullMeasure_Impl::clone() const {
    std::shared_ptr<NullMeasure_Impl> impl(new NullMeasure_Impl(*this));
    return NullMeasure(impl);
  }

  boost::optional<FileReferenceType> NullMeasure_Impl::inputFileType() const {
    return boost::none;
  }

  boost::optional<FileReferenceType> NullMeasure_Impl::outputFileType() const {
    return boost::none;
  }

  runmanager::WorkItem NullMeasure_Impl::createWorkItem(
      const openstudio::path& rubyIncludeDirectory) const
  {
    return runmanager::WorkItem(runmanager::JobType::Null);
  }

  QVariant NullMeasure_Impl::toVariant() const {
    QVariantMap nullMeasureData = Measure_Impl::toVariant().toMap();

    nullMeasureData["measure_type"] = QString("NullMeasure");

    return QVariant(nullMeasureData);
  }

  NullMeasure NullMeasure_Impl::fromVariant(const QVariant& variant, const VersionString& version) {
    QVariantMap map = variant.toMap();

    return NullMeasure(toUUID(map["uuid"].toString().toStdString()),
                       toUUID(map["version_uuid"].toString().toStdString()),
                       map.contains("name") ? map["name"].toString().toStdString() : std::string(),
                       map.contains("display_name") ? map["display_name"].toString().toStdString() : std::string(),
                       map.contains("description") ? map["description"].toString().toStdString() : std::string(),
                       map["is_selected"].toBool());
  }

} // detail

NullMeasure::NullMeasure(bool isSelected)
  : Measure(std::shared_ptr<detail::NullMeasure_Impl>(
        new detail::NullMeasure_Impl(isSelected)))
{}

NullMeasure::NullMeasure(const UUID& uuid,
                         const UUID& versionUUID,
                         const std::string& name,
                         const std::string& displayName,
                         const std::string& description,
                         bool isSelected)
  : Measure(std::shared_ptr<detail::NullMeasure_Impl>(
        new detail::NullMeasure_Impl(uuid,
                                     versionUUID,
                                     name,
                                     displayName,
                                     description,
                                     isSelected)))
{}

/// @cond
NullMeasure::NullMeasure(std::shared_ptr<detail::NullMeasure_Impl> impl)
  : Measure(impl)
{}
/// @endcond

} // analysis
} // openstudio

