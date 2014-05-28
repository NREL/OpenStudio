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

#ifndef ANALYSIS_ALGORITHMOPTIONS_IMPL_HPP
#define ANALYSIS_ALGORITHMOPTIONS_IMPL_HPP

#include "AnalysisAPI.hpp"
#include "Algorithm.hpp"

#include "../utilities/data/Attribute.hpp"

namespace openstudio {
namespace analysis {

namespace detail {

  /** AlgorithmOptions_Impl is the implementation class for AlgorithmOptions.*/
  class ANALYSIS_API AlgorithmOptions_Impl : public std::enable_shared_from_this<AlgorithmOptions_Impl> {
   public:
    /** @name Constructors and Destructors */
    //@{

    AlgorithmOptions_Impl();

    /** Constructor provided for deserialization; not for general use. */
    explicit AlgorithmOptions_Impl(const std::vector<Attribute>& options);

    AlgorithmOptions_Impl(const AlgorithmOptions_Impl& other);

    virtual ~AlgorithmOptions_Impl() {}

    virtual AlgorithmOptions clone() const;

    //@}
    /** @name Getters */
    //@{

    boost::optional<int> maxIter() const;

    boost::optional<int> maxSims() const;

    /** Getter provided for serialization; not for general use. */
    std::vector<Attribute> options() const;

    // only available in _Impl
    boost::optional<Attribute> getOption(const std::string& name) const;

    //@}
    /** @name Setters */
    //@{

    void setMaxIter(int maxIter);

    void setMaxSims(int maxSims);

    void clearMaxIter();

    void clearMaxSims();

    // only available in _Impl
    void saveOption(const Attribute& attribute);

    // only available in _Impl
    void clearOption(const std::string& name);

    //@}
    /** @name Type Casting */
    //@{

    /** Get a public object that wraps this impl.*/
    template<typename T>
    T getPublicObject() const {
      T result(std::dynamic_pointer_cast<typename T::ImplType>(
                 std::const_pointer_cast<AlgorithmOptions_Impl>(shared_from_this())));
      return result;
    }

    //@}
    /** @name Absent or Protected in Public Class */
    //@{

    boost::optional<Algorithm> algorithm() const;

    void setAlgorithm(const Algorithm& algorithm);

    virtual QVariant toVariant() const;

    //@}
   protected:
    std::vector<Attribute> m_options;

    boost::optional<Algorithm> m_algorithm; // link back to parent Algorithm
   private:
    REGISTER_LOGGER("openstudio.analysis.AlgorithmOptions");
  };

} // detail

} // analysis
} // openstudio

#endif // ANALYSIS_ALGORITHMOPTIONS_IMPL_HPP

