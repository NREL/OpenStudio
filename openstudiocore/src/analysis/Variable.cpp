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

#include "Variable.hpp"
#include "Variable_Impl.hpp"

#include "MeasureGroup.hpp"
#include "MeasureGroup_Impl.hpp"
#include "OutputAttributeVariable.hpp"
#include "OutputAttributeVariable_Impl.hpp"
#include "RubyContinuousVariable.hpp"
#include "RubyContinuousVariable_Impl.hpp"

namespace openstudio {
namespace analysis {

namespace detail {

  Variable_Impl::Variable_Impl(const std::string& name)
    : AnalysisObject_Impl(name)
  {}

  Variable_Impl::Variable_Impl(const UUID& uuid,
                               const UUID& versionUUID,
                               const std::string& name,
                               const std::string& displayName,
                               const std::string& description)
    : AnalysisObject_Impl(uuid,versionUUID,name,displayName,description)
  {}

  Variable_Impl::Variable_Impl(const Variable_Impl &other)
    : AnalysisObject_Impl(other)
  {}

  Variable Variable_Impl::factoryFromVariant(const QVariant& variant, const VersionString& version) {
    QVariantMap map = variant.toMap();

    std::string variableType;
    if (map.contains("variable_type")) {
      variableType = map["variable_type"].toString().toStdString();
    }
    else if (map.contains("workflow_step_type")) {
      variableType = map["workflow_step_type"].toString().toStdString();
    }
    else {
      LOG_AND_THROW("Unable to find Variable in expected location.");
    }

    if (variableType == "MeasureGroup") {
      return MeasureGroup_Impl::fromVariant(variant,version);
    }
    if (variableType == "OutputAttributeVariable") {
      return OutputAttributeVariable_Impl::fromVariant(variant,version);
    }
    if (variableType == "RubyContinuousVariable") {
      return RubyContinuousVariable_Impl::fromVariant(variant,version);
    }

    LOG_AND_THROW("Unexpected variable_type or workflow_step_type " << variableType << ".");
    return OptionalVariable().get();
  }

} // detail

double Variable::getValue(const DataPoint& dataPoint) const {
  return getImpl<detail::Variable_Impl>()->getValue(dataPoint);
}

/// @cond
Variable::Variable(std::shared_ptr<detail::Variable_Impl> impl)
  : AnalysisObject(impl)
{}
/// @endcond

} // analysis
} // openstudio

