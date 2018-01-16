/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef MODEL_SIZINGPARAMETERS_HPP
#define MODEL_SIZINGPARAMETERS_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

class Quantity;
class OSOptionalQuantity;

namespace model {

namespace detail {

  class SizingParameters_Impl;

} // detail

/** SizingParameters is a ModelObject that wraps the OpenStudio IDD object 'OS:Sizing:Parameters'. */
class MODEL_API SizingParameters : public ModelObject {
 public:

  /** @name Constructors and Destructors */
  //@{

  virtual ~SizingParameters() {}

  //@}
  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  //@}
  /** @name Getters */
  //@{

  double heatingSizingFactor() const;

  Quantity getHeatingSizingFactor(bool returnIP=false) const;

  bool isHeatingSizingFactorDefaulted() const;

  double coolingSizingFactor() const;

  Quantity getCoolingSizingFactor(bool returnIP=false) const;

  bool isCoolingSizingFactorDefaulted() const;

  boost::optional<int> timestepsinAveragingWindow() const;

  //@}
  /** @name Setters */
  //@{

  bool setHeatingSizingFactor(double heatingSizingFactor);

  bool setHeatingSizingFactor(const Quantity& heatingSizingFactor);

  void resetHeatingSizingFactor();

  bool setCoolingSizingFactor(double coolingSizingFactor);

  bool setCoolingSizingFactor(const Quantity& coolingSizingFactor);

  void resetCoolingSizingFactor();

  bool setTimestepsinAveragingWindow(int timestepsinAveragingWindow);

  void resetTimestepsinAveragingWindow();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::SizingParameters_Impl ImplType;

  explicit SizingParameters(std::shared_ptr<detail::SizingParameters_Impl> impl);

  friend class detail::SizingParameters_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  explicit SizingParameters(Model& model);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.SizingParameters");
};

/** \relates SizingParameters*/
typedef boost::optional<SizingParameters> OptionalSizingParameters;

/** \relates SizingParameters*/
typedef std::vector<SizingParameters> SizingParametersVector;

} // model
} // openstudio

#endif // MODEL_SIZINGPARAMETERS_HPP

