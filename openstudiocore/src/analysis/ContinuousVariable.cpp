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

#include "ContinuousVariable.hpp"
#include "ContinuousVariable_Impl.hpp"

#include "DataPoint.hpp"

#include "../utilities/math/FloatCompare.hpp"

#include "../utilities/core/Containers.hpp"
#include "../utilities/core/Optional.hpp"

namespace openstudio {
namespace analysis {

namespace detail {

  ContinuousVariable_Impl::ContinuousVariable_Impl(const std::string& name)
    : InputVariable_Impl(name)
  {}

  ContinuousVariable_Impl::ContinuousVariable_Impl(const UUID& uuid,
                                                   const UUID& versionUUID,
                                                   const std::string& name,
                                                   const std::string& displayName,
                                                   const std::string& description,
                                                   const boost::optional<UncertaintyDescription>& udesc,
                                                   boost::optional<double> minimum,
                                                   boost::optional<double> maximum,
                                                   boost::optional<double> increment,
                                                   boost::optional<int> nSteps)
    : InputVariable_Impl(uuid,versionUUID,name,displayName,description,udesc),
      m_minimum(minimum),
      m_maximum(maximum),
      m_increment(increment),
      m_nSteps(nSteps)
  {}

  ContinuousVariable_Impl::ContinuousVariable_Impl(const ContinuousVariable_Impl &other)
    : InputVariable_Impl(other),
      m_minimum(other.minimum()),
      m_maximum(other.maximum()),
      m_increment(other.increment()),
      m_nSteps(other.nSteps())
  {}

  // GETTERS

  boost::optional<double> ContinuousVariable_Impl::minimum() const {
    return m_minimum;
  }

  boost::optional<double> ContinuousVariable_Impl::maximum() const {
    return m_maximum;
  }

  boost::optional<double> ContinuousVariable_Impl::increment() const {
    return m_increment;
  }

  boost::optional<int> ContinuousVariable_Impl::nSteps() const {
    return m_nSteps;
  }

  bool ContinuousVariable_Impl::isFeasible(double value) const {
    bool result = true;
    if (m_minimum) { result = result && greaterThanOrEqual(value,*m_minimum); }
    if (m_maximum) { result = result && lessThanOrEqual(value,*m_maximum); }
    return result;
  }

  std::vector<double> ContinuousVariable_Impl::incrementalValues() const {
    DoubleVector result;
    if (m_minimum && m_maximum && (m_increment || m_nSteps)) {
      // empty range
      if (*m_minimum > *m_maximum) { return result; }
      // degenerate range (single point)
      if (equal(*m_minimum,*m_maximum)) {
        result.push_back((*m_minimum + *m_maximum)/2.0);
        return result;
      }
      // otherwise get incremental values
      if (m_increment) {
        for (double value = *m_minimum; lessThanOrEqual(value,*m_maximum); value += *m_increment) {
          result.push_back(value);
        }
      }
      else {
        // m_nSteps
        double inc = (*m_maximum - *m_minimum)/static_cast<double>(*m_nSteps);
        for (double value = *m_minimum; lessThanOrEqual(value,*m_maximum); value += inc) {
          result.push_back(value);
        }
      }
    }
    return result;
  }

  void ContinuousVariable_Impl::setMinimum(double minimum) {
    m_minimum = minimum;
    onChange(AnalysisObject_Impl::Benign);
  }

  void ContinuousVariable_Impl::setMaximum(double maximum) {
    m_maximum = maximum;
    onChange(AnalysisObject_Impl::Benign);
  }

  void ContinuousVariable_Impl::setIncrement(double increment) {
    m_increment = increment;
    m_nSteps = boost::none;
    onChange(AnalysisObject_Impl::Benign);
  }

  void ContinuousVariable_Impl::setNSteps(int nSteps) {
    m_increment = boost::none;
    m_nSteps = nSteps;
    onChange(AnalysisObject_Impl::Benign);
  }

  void ContinuousVariable_Impl::clearMinimum() {
    m_minimum = boost::none;
    onChange(AnalysisObject_Impl::Benign);
  }

  void ContinuousVariable_Impl::clearMaximum() {
    m_maximum = boost::none;
    onChange(AnalysisObject_Impl::Benign);
  }

  void ContinuousVariable_Impl::clearIncrement() {
    m_increment = boost::none;
    onChange(AnalysisObject_Impl::Benign);
  }

  void ContinuousVariable_Impl::clearNSteps() {
    m_nSteps = boost::none;
    onChange(AnalysisObject_Impl::Benign);
  }

  boost::optional<double> ContinuousVariable_Impl::truncate(double value) const {
    if (isFeasible(value)) { return value; }
    double lTrunc(value), rTrunc(value);
    if (m_minimum && (value < *m_minimum)) { lTrunc = *m_minimum; }
    if (m_maximum && (value > *m_maximum)) { rTrunc = *m_maximum; }
    OptionalDouble result;
    if (isFeasible(lTrunc)) { result = lTrunc; }
    if (isFeasible(rTrunc)) {
      if (result) { result = (lTrunc + rTrunc)/2.0; }
      else { result = rTrunc; }
    }
    return result;
  }

  QVariant ContinuousVariable_Impl::toVariant() const {
    QVariantMap variableData = InputVariable_Impl::toVariant().toMap();

    if (minimum()) {
      variableData["minimum"] = minimum().get();
    }

    if (maximum()) {
      variableData["maximum"] = maximum().get();
    }

    if (increment()) {
      variableData["increment"] = increment().get();
    }

    if (nSteps()) {
      variableData["n_steps"] = nSteps().get();
    }

    return QVariant(variableData);
  }

  QVariant ContinuousVariable_Impl::toServerFormulationVariant() const {
    QVariantMap map;

    map["uuid"] = toQString(removeBraces(uuid()));
    map["version_uuid"] = toQString(removeBraces(uuid()));
    map["name"] = toQString(name());
    map["display_name"] = toQString(displayName());
    map["type"] = QString("Double"); // could be Continuous instead
    if (minimum()) {
      map["minimum"] = minimum().get();
    }
    if (maximum()) {
      map["maximum"] = maximum().get();
    }
    // there is no initial_value yet

    return QVariant(map);
  }

} // detail

boost::optional<double> ContinuousVariable::minimum() const {
  return getImpl<detail::ContinuousVariable_Impl>()->minimum();
}

boost::optional<double> ContinuousVariable::maximum() const {
  return getImpl<detail::ContinuousVariable_Impl>()->maximum();
}

boost::optional<double> ContinuousVariable::increment() const {
  return getImpl<detail::ContinuousVariable_Impl>()->increment();
}

boost::optional<int> ContinuousVariable::nSteps() const {
  return getImpl<detail::ContinuousVariable_Impl>()->nSteps();
}

bool ContinuousVariable::isFeasible(double value) const {
  return getImpl<detail::ContinuousVariable_Impl>()->isFeasible(value);
}

std::vector<double> ContinuousVariable::incrementalValues() const {
  return getImpl<detail::ContinuousVariable_Impl>()->incrementalValues();
}

void ContinuousVariable::setMinimum(double minimum) {
  getImpl<detail::ContinuousVariable_Impl>()->setMinimum(minimum);
}

void ContinuousVariable::setMaximum(double maximum) {
  getImpl<detail::ContinuousVariable_Impl>()->setMaximum(maximum);
}

void ContinuousVariable::setIncrement(double increment) {
  getImpl<detail::ContinuousVariable_Impl>()->setIncrement(increment);
}

void ContinuousVariable::setNSteps(int nSteps) {
  getImpl<detail::ContinuousVariable_Impl>()->setNSteps(nSteps);
}

void ContinuousVariable::clearMinimum() {
  getImpl<detail::ContinuousVariable_Impl>()->clearMinimum();
}

void ContinuousVariable::clearMaximum() {
  getImpl<detail::ContinuousVariable_Impl>()->clearMaximum();
}

void ContinuousVariable::clearIncrement() {
  getImpl<detail::ContinuousVariable_Impl>()->clearIncrement();
}

void ContinuousVariable::clearNSteps() {
  getImpl<detail::ContinuousVariable_Impl>()->clearNSteps();
}

boost::optional<double> ContinuousVariable::truncate(double value) const {
  return getImpl<detail::ContinuousVariable_Impl>()->truncate(value);
}

/// @cond
ContinuousVariable::ContinuousVariable(std::shared_ptr<detail::ContinuousVariable_Impl> impl)
  : InputVariable(impl)
{}
/// @endcond

} // analysis
} // openstudio
