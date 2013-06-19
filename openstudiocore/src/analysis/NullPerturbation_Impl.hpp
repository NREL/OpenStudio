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

#ifndef ANALYSIS_NULLPERTURBATION_IMPL_HPP
#define ANALYSIS_NULLPERTURBATION_IMPL_HPP

#include <analysis/AnalysisAPI.hpp>
#include <analysis/DiscretePerturbation_Impl.hpp>

namespace openstudio {
namespace analysis {

class NullPerturbation;

namespace detail {

  /** NullPerturbation_Impl is a DiscretePerturbation_Impl that is the implementation class for NullPerturbation.*/
  class ANALYSIS_API NullPerturbation_Impl : public DiscretePerturbation_Impl {
    Q_OBJECT;
   public:
    /** @name Constructors and Destructors */
    //@{

    NullPerturbation_Impl(bool isSelected);

    /** Constructor provided for deserialization; not for general use. */
    NullPerturbation_Impl(const UUID& uuid,
                          const UUID& versionUUID,
                          const std::string& name,
                          const std::string& displayName,
                          const std::string& description,
                          bool isSelected);

    NullPerturbation_Impl(const NullPerturbation_Impl& other);

    virtual ~NullPerturbation_Impl() {}

    virtual AnalysisObject clone() const;

    //@}

    virtual boost::optional<FileReferenceType> inputFileType() const;

    virtual boost::optional<FileReferenceType> outputFileType() const;

    virtual runmanager::WorkItem createWorkItem(const openstudio::path& rubyIncludeDirectory) const;

   protected:
   private:
    REGISTER_LOGGER("openstudio.analysis.NullPerturbation");
  };

} // detail

} // model
} // openstudio

#endif // ANALYSIS_NULLPERTURBATION_IMPL_HPP

