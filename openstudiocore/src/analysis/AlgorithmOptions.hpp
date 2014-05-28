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

#ifndef ANALYSIS_ALGORITHMOPTIONS_HPP
#define ANALYSIS_ALGORITHMOPTIONS_HPP

#include "AnalysisAPI.hpp"

#include "../utilities/core/Logger.hpp"

#include <boost/optional.hpp>

#include <vector>

namespace openstudio {

class Attribute;

namespace analysis {

class Algorithm;

namespace detail {

  class AlgorithmOptions_Impl;
  class Algorithm_Impl;

} // detail

/** AlgorithmOptions defines the basic options that all \link Algorithm Algorithms \endlink are 
 *  assumed to have, that is, a maximum number of iterations and a maximum number of simulations.
 *  All Algorithm classes are constructed with an AlgorithmOptions object (often of a derived 
 *  type). \relates Algorithm
 */
class ANALYSIS_API AlgorithmOptions {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** By default, maxIter and maxSims are set to boost::none (no restriction). */
  AlgorithmOptions();

  virtual ~AlgorithmOptions() {}

  AlgorithmOptions clone() const;

  //@}
  /** @name Getters */
  //@{

  boost::optional<int> maxIter() const;

  boost::optional<int> maxSims() const;

  /** Getter provided for serialization; not for general use. */
  std::vector<Attribute> options() const;

  //@}
  /** @name Setters */
  //@{

  void setMaxIter(int maxIter);

  void setMaxSims(int maxSims);

  void clearMaxIter();

  void clearMaxSims();

  //@}
  /** @name Type Casting */
  //@{

  /** Get the impl pointer */
  template<typename T>
  std::shared_ptr<T> getImpl() const {
    return std::dynamic_pointer_cast<T>(m_impl);
  }

  /** Cast to type T. Throws std::bad_cast if object is not a T. */
  template<typename T>
  T cast() const {
    std::shared_ptr<typename T::ImplType> impl = this->getImpl<typename T::ImplType>();
    if (!impl) {
      throw(std::bad_cast());
    }
    return T(impl);
  }

  /** Cast to boost::optional<T>. Return value is boost::none (evaluates to false) 
   *  if object is not a T. */
  template<typename T>
  boost::optional<T> optionalCast() const{
    boost::optional<T> result;
    std::shared_ptr<typename T::ImplType> impl = this->getImpl<typename T::ImplType>();
    if (impl){
      result = T(impl);
    }
    return result;
  }

  //@}
 protected:
  /// @cond
  typedef detail::AlgorithmOptions_Impl ImplType;

  explicit AlgorithmOptions(std::shared_ptr<detail::AlgorithmOptions_Impl> impl);

  friend class detail::AlgorithmOptions_Impl;
  /// @endcond

  friend class Algorithm;

  void setAlgorithm(const Algorithm& algorithm);

  friend class detail::Algorithm_Impl;

  QVariant toVariant() const;
 private:
  std::shared_ptr<detail::AlgorithmOptions_Impl> m_impl;

  REGISTER_LOGGER("openstudio.analysis.AlgorithmOptions");
};

/** \relates AlgorithmOptions*/
typedef boost::optional<AlgorithmOptions> OptionalAlgorithmOptions;

/** \relates AlgorithmOptions*/
typedef std::vector<AlgorithmOptions> AlgorithmOptionsVector;

} // analysis
} // openstudio

#endif // ANALYSIS_ALGORITHMOPTIONS_HPP

