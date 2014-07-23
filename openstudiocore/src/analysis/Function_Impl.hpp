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

#ifndef ANALYSIS_FUNCTION_IMPL_HPP
#define ANALYSIS_FUNCTION_IMPL_HPP

#include "AnalysisAPI.hpp"
#include "AnalysisObject_Impl.hpp"

#include "Variable.hpp"

namespace openstudio {
namespace analysis {

class Function;
class DataPoint;

namespace detail {

  /** Function_Impl is a AnalysisObject_Impl that is the implementation class for Function.*/
  class ANALYSIS_API Function_Impl : public AnalysisObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    Function_Impl(const std::string& name,
                  const std::vector<Variable>& variables);

    /** Constructor provided for deserialization; not for general use. */
    Function_Impl(const UUID& uuid,
                  const UUID& versionUUID,
                  const std::string& name,
                  const std::string& displayName,
                  const std::string& description,
                  const std::vector<Variable>& variables);

    Function_Impl(const Function_Impl& other);

    virtual ~Function_Impl() {}

    //@}
    /** @name Getters */
    //@{

    std::vector<Variable> variables() const;

    virtual double getValue(const DataPoint& dataPoint) const = 0;

    //@}
    /** @name Not in Public Class */
    //@{

    bool doNotParent(const Variable& variable) const;

    static Function factoryFromVariant(const QVariant& variant, const VersionString& version);

    //@}
   protected:
    std::vector<Variable> m_variables;

   private:
    REGISTER_LOGGER("openstudio.analysis.Function");
  };

} // detail

} // model
} // openstudio

#endif // ANALYSIS_FUNCTION_IMPL_HPP

