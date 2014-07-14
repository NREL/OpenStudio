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

#ifndef ANALYSIS_VARIABLE_IMPL_HPP
#define ANALYSIS_VARIABLE_IMPL_HPP

#include "AnalysisAPI.hpp"
#include "AnalysisObject_Impl.hpp"

#include <QVariant>

namespace openstudio {
namespace analysis {

class DataPoint;
class Variable;

namespace detail {

  /** Variable_Impl is a AnalysisObject_Impl that is the implementation class for
   *  Variable.*/
  class ANALYSIS_API Variable_Impl : public AnalysisObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    Variable_Impl(const std::string& name);

    /** Constructor provided for deserialization; not for general use. */
    Variable_Impl(const UUID& uuid,
                  const UUID& versionUUID,
                  const std::string& name,
                  const std::string& displayName,
                  const std::string& description);

    Variable_Impl(const Variable_Impl& other);

    virtual ~Variable_Impl() {}

    //@}
    /** @name Getters and Queries */
    //@{

    /** Get the variable value from a dataPoint. Throws an openstudio::Exception if the Variable
     *  cannot be evaluated for dataPoint. */
    virtual double getValue(const DataPoint& dataPoint) const = 0;

    //@}
    /** @name Absent or Protected in Public Class */
    //@{

    static Variable factoryFromVariant(const QVariant& variant, const VersionString& version);

    //@}
   private:
    REGISTER_LOGGER("openstudio.analysis.Variable");
  };

} // detail

} // model
} // openstudio

#endif // ANALYSIS_VARIABLE_IMPL_HPP
