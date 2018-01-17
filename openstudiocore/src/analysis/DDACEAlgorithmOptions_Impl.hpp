/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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

    virtual AlgorithmOptions clone() const override;

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

    virtual QVariant toVariant() const override;

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

