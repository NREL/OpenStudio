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

#ifndef ANALYSIS_MEASURE_IMPL_HPP
#define ANALYSIS_MEASURE_IMPL_HPP

#include "AnalysisAPI.hpp"
#include "AnalysisObject_Impl.hpp"

#include "../utilities/core/Path.hpp"

namespace openstudio {

class FileReferenceType;

namespace runmanager {
  class WorkItem;
}

namespace analysis {

class Measure;

namespace detail {

  /** Measure_Impl is a AnalysisObject_Impl that is the implementation class for Measure.*/
  class ANALYSIS_API Measure_Impl : public AnalysisObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Default constructor sets name to empty string. */
    Measure_Impl(bool isSelected);

    /** Constructor provided for deserialization; not for general use. */
    Measure_Impl(const UUID& uuid,
                 const UUID& versionUUID,
                 const std::string& name,
                 const std::string& displayName,
                 const std::string& description,
                 bool isSelected);

    Measure_Impl(const Measure_Impl& other);

    virtual ~Measure_Impl() {}

    //@}
    /** @name Getters and Queries */
    //@{

    bool isSelected() const;

    virtual boost::optional<FileReferenceType> inputFileType() const = 0;

    virtual boost::optional<FileReferenceType> outputFileType() const = 0;

    //@}
    /** @name Setters */
    //@{

    void setIsSelected(bool isSelected);

    //@}
    /** @name Actions */
    //@{

    virtual runmanager::WorkItem createWorkItem(
        const openstudio::path& rubyIncludeDirectory) const = 0;

    //@}
    /** @name Protected in Public Class */
    //@{

    virtual QVariant toVariant() const override;

    static Measure factoryFromVariant(const QVariant& variant, const VersionString& version);

    //@}
   protected:
    bool m_isSelected;
   private:
    REGISTER_LOGGER("openstudio.analysis.Measure");
  };

} // detail

} // model
} // openstudio

#endif // ANALYSIS_MEASURE_IMPL_HPP

