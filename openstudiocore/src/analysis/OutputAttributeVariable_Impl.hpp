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

#ifndef ANALYSIS_OUTPUTATTRIBUTEVARIABLE_IMPL_HPP
#define ANALYSIS_OUTPUTATTRIBUTEVARIABLE_IMPL_HPP

#include "AnalysisAPI.hpp"
#include "OutputVariable_Impl.hpp"

namespace openstudio {
namespace analysis {

class OutputAttributeVariable;

namespace detail {

  /** OutputAttributeVariable_Impl is an OutputVariable_Impl that is the implementation
   *  class for OutputAttributeVariable.*/
  class ANALYSIS_API OutputAttributeVariable_Impl : public OutputVariable_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    OutputAttributeVariable_Impl(const std::string& name,
                                 const std::string& attributeName);

    /** Constructor provided for deserialization; not for general use. */
    OutputAttributeVariable_Impl(const UUID& uuid,
                                 const UUID& versionUUID,
                                 const std::string& name,
                                 const std::string& displayName,
                                 const std::string& description,
                                 const std::string& attributeName);

    OutputAttributeVariable_Impl(const OutputAttributeVariable_Impl& other);

    virtual ~OutputAttributeVariable_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual AnalysisObject clone() const;

    /** Get the variable value from a dataPoint. Throws an openstudio::Exception if
     *  the Variable cannot be evaluated for dataPoint. */
    virtual double getValue(const DataPoint& dataPoint) const;

    //@}
    /** @name Getters */
    //@{

    std::string attributeName() const;

    //@}
    /** @name Setters */
    //@{

    void setAttributeName(const std::string& attributeName);

    //@}
    /** @name Protected in Public Class */
    //@{

    virtual QVariant toVariant() const;

    static OutputAttributeVariable fromVariant(const QVariant& variant, const VersionString& version);

    //@}
   private:
    REGISTER_LOGGER("openstudio.analysis.OutputAttributeVariable");

    std::string m_attributeName;
  };

} // detail

} // model
} // openstudio

#endif // ANALYSIS_OUTPUTATTRIBUTEVARIABLE_IMPL_HPP

