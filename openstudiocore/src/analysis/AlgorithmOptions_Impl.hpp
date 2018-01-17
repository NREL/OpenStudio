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

