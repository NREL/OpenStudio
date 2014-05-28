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

#ifndef ANALYSIS_NULLMEASURE_IMPL_HPP
#define ANALYSIS_NULLMEASURE_IMPL_HPP

#include "AnalysisAPI.hpp"
#include "Measure_Impl.hpp"

namespace openstudio {
namespace analysis {

class NullMeasure;

namespace detail {

  /** NullMeasure_Impl is a Measure_Impl that is the implementation class for NullMeasure.*/
  class ANALYSIS_API NullMeasure_Impl : public Measure_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    NullMeasure_Impl(bool isSelected);

    /** Constructor provided for deserialization; not for general use. */
    NullMeasure_Impl(const UUID& uuid,
                          const UUID& versionUUID,
                          const std::string& name,
                          const std::string& displayName,
                          const std::string& description,
                          bool isSelected);

    NullMeasure_Impl(const NullMeasure_Impl& other);

    virtual ~NullMeasure_Impl() {}

    virtual AnalysisObject clone() const;

    //@}
    /** @name Virtual Methods */
    //@{

    virtual boost::optional<FileReferenceType> inputFileType() const;

    virtual boost::optional<FileReferenceType> outputFileType() const;

    virtual runmanager::WorkItem createWorkItem(const openstudio::path& rubyIncludeDirectory) const;

    //@}
    /** @name Protected in Public Class */
    //@{

    virtual QVariant toVariant() const;

    static NullMeasure fromVariant(const QVariant& variant, const VersionString& version);

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.analysis.NullMeasure");
  };

} // detail

} // model
} // openstudio

#endif // ANALYSIS_NULLMEASURE_IMPL_HPP

