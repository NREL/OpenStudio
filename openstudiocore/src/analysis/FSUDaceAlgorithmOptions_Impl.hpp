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

#ifndef ANALYSIS_FSUDACEALGORITHMOPTIONS_IMPL_HPP
#define ANALYSIS_FSUDACEALGORITHMOPTIONS_IMPL_HPP

#include "AnalysisAPI.hpp"
#include "DakotaAlgorithmOptions_Impl.hpp"

#include "FSUDaceAlgorithmOptions.hpp"

namespace openstudio {
namespace analysis {

class Problem;

namespace detail {

  /** FSUDaceAlgorithmOptions_Impl is a DakotaAlgorithmOptions_Impl that is the implementation 
   *  class for FSUDaceAlgorithmOptions. */ 
  class ANALYSIS_API FSUDaceAlgorithmOptions_Impl : public DakotaAlgorithmOptions_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit FSUDaceAlgorithmOptions_Impl(const FSUDaceAlgorithmType& algorithmType);

    /** Constructor provided for deserialization; not for general use. */
    FSUDaceAlgorithmOptions_Impl(const FSUDaceAlgorithmType& algorithmType,
                                 const boost::optional<FSUDaceCvtTrialType>& trialType,
                                 const std::vector<Attribute>& options);

    virtual ~FSUDaceAlgorithmOptions_Impl() {}

    virtual AlgorithmOptions clone() const override;

    //@}
    /** @name Getters */
    //@{

    FSUDaceAlgorithmType algorithmType() const;

    /** Returns the trial type if it exists, evaluates to false otherwise. Only applies to
     *  FSUDaceAlgorithmType::cvt. */
    boost::optional<FSUDaceCvtTrialType> trialType() const;

    /** Returns the user-specified number of samples to be used. Applies to all
     *  \link FSUDaceAlgorithmType FSUDaceAlgorithmTypes \endlink. */
    int samples() const;

    /** Returns the user-specified starting sequence number. Only applicable to 
     *  FSUDaceAlgorithmType::halton and FSUDaceAlgorithmType::hammersley. */
    std::vector<int> sequenceStart() const;

    /** Returns the user-specified sequence leap number. Only applicable to 
     *  FSUDaceAlgorithmType::halton and FSUDaceAlgorithmType::hammersley. */
    std::vector<int> sequenceLeap() const;

    /** Returns whether or not the user-specified fixed_sequence option is active, if set;
     *  the default value is false. Only applicable to FSUDaceAlgorithmType::halton and 
     *  FSUDaceAlgorithmType::hammersley. */
    bool fixedSequence() const;

    /** Returns whether or not the user-specified latinize option is active, if set;
     *  the default value is false. Applies to all \link FSUDaceAlgorithmType 
     *  FSUDaceAlgorithmTypes \endlink. */
    bool latinize() const;

    /** Returns whether or not the user-specified variance_based_decomp option is active, if set;
     *  the default value is false. Applies to all \link FSUDaceAlgorithmType 
     *  FSUDaceAlgorithmTypes \endlink. */
    bool varianceBasedDecomp() const;

    /** Returns whether or not the user-specified quality_metrics option is active, if set;
     *  the default value is false. Applies to all \link FSUDaceAlgorithmType 
     *  FSUDaceAlgorithmTypes \endlink. */
    bool qualityMetrics() const;

    /** Returns the explict pseudo-random number generator seed if it exists, evaluates to false 
     *  otherwise. Only applies to FSUDaceAlgorithmType::cvt. */
    boost::optional<int> seed() const;

    /** Returns the number of trials specified if it exists, evaluates to false 
     *  otherwise. Only applies to FSUDaceAlgorithmType::cvt. */
    boost::optional<int> numTrials() const;

    /** Returns whether or not the user-specified fixed_seed option is active, if set;
     *  the default value is false. Only applies to FSUDaceAlgorithmType::cvt. */
    bool fixedSeed() const;

    //@}
    /** @name Setters */
    //@{

    /** The number of samples must be greater than zero. Applies to all \link FSUDaceAlgorithmType 
     *  FSUDaceAlgorithmTypes \endlink. */
    bool setSamples(int value);

    /** To set, there must be one value for every variable, and all values must be nonnegative. 
     *  Only applicable to FSUDaceAlgorithmType::halton and FSUDaceAlgorithmType::hammersley. 
     *  DAKOTA provides defaults if left unspecified. */
    bool setSequenceStart(const std::vector<int>& value);
  
    void clearSequenceStart();  

    /** To set, There must be one value for every variable, and all values must be greater than zero. 
     *  Only applicable to FSUDaceAlgorithmType::halton and FSUDaceAlgorithmType::hammersley. 
     *  DAKOTA provides defaults if left unspecified. */
    bool setSequenceLeap(const std::vector<int>& value);
  
    void clearSequenceLeap();  

    /** Places the string "fixed_sequence" in the .in file if true, otherwise prints nothing.
     *  Only applicable to FSUDaceAlgorithmType::halton and FSUDaceAlgorithmType::hammersley. */
    void setFixedSequence(bool value);

    /** Places the string "latinize" in the .in file if true, otherwise prints nothing.
     *  Applies to all \link FSUDaceAlgorithmType FSUDaceAlgorithmTypes \endlink.*/
    void setLatinize(bool value);
  
    /** Places the string "variance_based_decomp" in the .in file if true, otherwise prints nothing.
     *  Applies to all \link FSUDaceAlgorithmType FSUDaceAlgorithmTypes \endlink. This is a 
     *  computationally intestisive option; it requires the evaluation of n*(m+2) samples, where n 
     *  is the number of samples specified and m is the number of variables. */
    void setVarianceBasedDecomp(bool value);

    /** Places the string "quality_metrics" in the .in file if true, otherwise prints nothing.
     *  Applies to all \link FSUDaceAlgorithmType FSUDaceAlgorithmTypes \endlink. */
    void setQualityMetrics(bool value);

    /** Seed value must be greater than zero. Only applies to FSUDaceAlgorithmType::cvt. */
    bool setSeed(int value);
  
    void clearSeed();  

    /** Number of Trials must be greater than zero and is typically very large (>10000). Only 
     *  applies to FSUDaceAlgorithmType::cvt. */
    bool setNumTrials(int value);
  
    void clearNumTrials();  

    /** Places the string "fixed_seed" in the .in file if true, otherwise prints nothing.
     *  Only applies to FSUDaceAlgorithmType::cvt. */
    void setFixedSeed(bool value);

    /** Specifies the CVT trial type. Only applies to FSUDaceAlgorithmType::cvt. Defaults to random
     *  if left unspecified. */
    void setTrialType(FSUDaceCvtTrialType value);
  
    void clearTrialType();  

    //@}
    /** @name Absent or Protected in Public Class */
    //@{

    virtual QVariant toVariant() const override;

    static FSUDaceAlgorithmOptions fromVariant(const QVariant& variant, const VersionString& version);

    //@}
   protected:
    FSUDaceAlgorithmType m_algorithmType;
    boost::optional<FSUDaceCvtTrialType> m_trialType;

   private:
    REGISTER_LOGGER("openstudio.analysis.FSUDaceAlgorithmOptions");
  };

} // detail

} // analysis
} // openstudio

#endif // ANALYSIS_FSUDACEALGORITHMOPTIONS_IMPL_HPP

