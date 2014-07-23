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

#ifndef ANALYSIS_PSUADEDACEALGORITHMOPTIONS_IMPL_HPP
#define ANALYSIS_PSUADEDACEALGORITHMOPTIONS_IMPL_HPP

#include "AnalysisAPI.hpp"
#include "DakotaAlgorithmOptions_Impl.hpp"

#include "PSUADEDaceAlgorithmOptions.hpp"

namespace openstudio {
namespace analysis {

class Problem;

namespace detail {

  /** PSUADEDaceAlgorithmOptions_Impl is a DakotaAlgorithmOptions_Impl that is the implementation 
   *  class for PSUADEDaceAlgorithmOptions.*/
  class ANALYSIS_API PSUADEDaceAlgorithmOptions_Impl : public DakotaAlgorithmOptions_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    PSUADEDaceAlgorithmOptions_Impl();

    /** Constructor provided for deserialization; not for general use. */
    explicit PSUADEDaceAlgorithmOptions_Impl(const std::vector<Attribute>& options);

    virtual ~PSUADEDaceAlgorithmOptions_Impl() {}

    virtual AlgorithmOptions clone() const;

    //@}

    /** @name Getters and Queries */
    //@{

    /** Returns the explict pseudo-random number generator seed if it exists, evaluates to false 
     *  otherwise. */
    boost::optional<int> seed() const;

    /** Returns the user-specified number of samples to be used if it exists, evaluates to false
     *  otherwise. */
    boost::optional<int> samples() const;

    /** Returns the user-specified number of partitions to be used if it exists, evaluates to false
     *  otherwise. */
    boost::optional<int> partitions() const;

    //@}
    /** @name Setters */
    //@{

    /** Seed value must be greater than zero. */
    bool setSeed(int value);
    
    void clearSeed();    

    /** The number of samples must be greater than zero. This value should be an integer multiple of
     *  (number of continuous design variables + 1), but if will automatically adjusted by Dakota if
     *  misspecified.*/
    bool setSamples(int value);
    
    void clearSamples();    

    /** The number of partitions must be greater than zero. This value should be odd, though Dakota
     *  will automatically correct the value if it is not odd. */
    bool setPartitions(int value);

    void clearPartitions();

    //@}
    /** @name Absent or Protected in Public Class */
    //@{

    static PSUADEDaceAlgorithmOptions fromVariant(const QVariant& variant, const VersionString& version);

    //@}
   private:
    REGISTER_LOGGER("openstudio.analysis.PSUADEDaceAlgorithmOptions");
  };

} // detail

} // analysis
} // openstudio

#endif // ANALYSIS_PSUADEDACEALGORITHMOPTIONS_IMPL_HPP

