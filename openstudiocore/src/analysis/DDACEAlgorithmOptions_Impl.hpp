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

#ifndef ANALYSIS_DDACEALGORITHMOPTIONS_IMPL_HPP
#define ANALYSIS_DDACEALGORITHMOPTIONS_IMPL_HPP

#include "AnalysisAPI.hpp"
#include "DakotaAlgorithmOptions_Impl.hpp"

#include "DDACEAlgorithmOptions.hpp"

namespace openstudio {
namespace analysis {

class Problem;

namespace detail {

  /** DDACEAlgorithmOptions_Impl is a DakotaAlgorithmOptions_Impl that is the 
   *  implementation class for DDACEAlgorithmOptions.*/
  class ANALYSIS_API DDACEAlgorithmOptions_Impl : public DakotaAlgorithmOptions_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{
    
    explicit DDACEAlgorithmOptions_Impl(const DDACEAlgorithmType& algorithmType);

    /** Constructor provided for deserialization; not for general use. */
    DDACEAlgorithmOptions_Impl(const DDACEAlgorithmType& algorithmType,
                               const std::vector<Attribute>& options);

    virtual ~DDACEAlgorithmOptions_Impl() {}

    virtual AlgorithmOptions clone() const;

    //@}
    /** @name Getters */
    //@{

    /** Returns the type of DDACE sampling algorithm to be applied. */
    DDACEAlgorithmType algorithmType() const;

    /** Returns the explict pseudo-random number generator seed if it exists, evaluates to false 
     *  otherwise. */
    boost::optional<int> seed() const;

    /** Returns the user-specified number of samples to be used, if set; evaluates to false 
     *  otherwise. */
    boost::optional<int> samples() const;

    /** Returns the user-specified number of symbols to be used, if set; evaluates to false
     *  otherwise. Symbols is used by some of the algorithms to set the structure and number of
     *  samples, and can generally be calculated by DAKOTA if left unspecified. */
    boost::optional<int> symbols() const;

    //@}
    /** @name Setters */
    //@{

    void setSeed(int value);
  
    void clearSeed();  

    bool setSamples(int value);
  
    void clearSamples();  

    bool setSymbols(int value);
  
    void clearSymbols();

    //@}
    /** @name Other */
    //@{
  
    // ETH@20120921 This text is a little unclear. Would be good to look at/add to the test cases to
    // figure out the true behavior.
    /** Equivalent to setSamples( numGridPartitions^(problem.numVariables()) ) and
     *  setSymbols(numGridPartitions). The grid sampling algorithm requires that samples be set, and
     *  determines the actual number of samples by taking numGridPartitions =
     *  ceil( samples ^ (1 / problem.numVariables()) ). */
    bool setSamplesForGrid(int numGridPartitions,const Problem& problem);

    /** Checks that symbols is prime or 4, sets symbols, and sets samples = multiplier * symbols^2.
     *  For use with Orthogonal Array and Orthogonal Array - Latin Hypercube. If dakota crashes after
     *  using this method, try calling clearSymbols afterwards and then look at the dakota stdout
     *  file to see what values dakota decides on for symbols and samples. Have had success with (2,1)
     *  and (2,2), but dakota crashed with (2,3). In general, the allowable samples and symbols 
     *  combinations seem to be more restricted than what the DAKOTA documentation indicates. */
    bool setSamplesAndSymbolsForOrthogonalArray(int symbols, int multiplier);
  
    //@}  
    /** @name Absent or Protected in Public Class */
    //@{

    virtual QVariant toVariant() const;

    static DDACEAlgorithmOptions fromVariant(const QVariant& variant, const VersionString& version);

    //@}
   protected:
    DDACEAlgorithmType m_algorithmType;
   private:
    REGISTER_LOGGER("openstudio.analysis.DDACEAlgorithmOptions");
  };

} // detail

} // analysis
} // openstudio

#endif // ANALYSIS_DDACEALGORITHMOPTIONS_IMPL_HPP

