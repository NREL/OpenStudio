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

#ifndef ANALYSIS_DISCRETEPERTURBATION_IMPL_HPP
#define ANALYSIS_DISCRETEPERTURBATION_IMPL_HPP

#include <analysis/AnalysisAPI.hpp>
#include <analysis/AnalysisObject_Impl.hpp>

#include <utilities/core/Path.hpp>

namespace openstudio {

class FileReferenceType;

namespace runmanager {
  class WorkItem;
}

namespace analysis {

class DiscretePerturbation;

namespace detail {

  /** DiscretePerturbation_Impl is a AnalysisObject_Impl that is the implementation class for DiscretePerturbation.*/
  class ANALYSIS_API DiscretePerturbation_Impl : public AnalysisObject_Impl {
    Q_OBJECT;
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Default constructor sets name to empty string. */
    DiscretePerturbation_Impl(bool isSelected);

    /** Constructor provided for deserialization; not for general use. */
    DiscretePerturbation_Impl(const UUID& uuid,
                              const UUID& versionUUID,
                              const std::string& name,
                              const std::string& displayName,
                              const std::string& description,
                              bool isSelected);

    DiscretePerturbation_Impl(const DiscretePerturbation_Impl& other);

    virtual ~DiscretePerturbation_Impl() {}

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
   protected:
    bool m_isSelected;
   private:
    REGISTER_LOGGER("openstudio.analysis.DiscretePerturbation");
  };

} // detail

} // model
} // openstudio

#endif // ANALYSIS_DISCRETEPERTURBATION_IMPL_HPP

