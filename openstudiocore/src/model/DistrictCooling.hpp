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

#ifndef MODEL_DISTRICTCOOLING_HPP
#define MODEL_DISTRICTCOOLING_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {
class Quantity;
class OSOptionalQuantity;

namespace model {

namespace detail {

  class DistrictCooling_Impl;

} // detail

/** DistrictCooling is a StraightComponent that wraps the OpenStudio IDD object 'OS:DistrictCooling'. */

class MODEL_API DistrictCooling : public StraightComponent {

 public:

  /** @name Constructors and Destructors */
  //@{

  explicit DistrictCooling(const Model& model);

  virtual ~DistrictCooling() {}

  //@}

  //@}
  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  //@}
  /** @name Getters */
  //@{

  boost::optional<double> nominalCapacity() const;

  bool isNominalCapacityAutosized() const;

  //double nominalCapacity() const;

  //Quantity getNominalCapacity(bool returnIP=false) const;

  //@}
  /** @name Setters */
  //@{

  bool setNominalCapacity(double nominalCapacity);

  void autosizeNominalCapacity();

  //bool setNominalCapacity(double nominalCapacity);

  //bool setNominalCapacity(const Quantity& nominalCapacity);

  //@}
  /** @name Other */
  //@{

  //@}

  boost::optional<double> autosizedNominalCapacity() const ;



 protected:
  /// @cond
  typedef detail::DistrictCooling_Impl ImplType;

  explicit DistrictCooling(std::shared_ptr<detail::DistrictCooling_Impl> impl);

  friend class detail::DistrictCooling_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;

  /// @endcond

 private:

  REGISTER_LOGGER("openstudio.model.DistrictCooling");

};


/** \relates DistrictCooling*/
typedef boost::optional<DistrictCooling> OptionalDistrictCooling;

/** \relates DistrictCooling*/
typedef std::vector<DistrictCooling> DistrictCoolingVector;

} // model
} // openstudio

#endif // MODEL_DISTRICTCOOLING_HPP



