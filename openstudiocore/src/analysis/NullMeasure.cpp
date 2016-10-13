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

