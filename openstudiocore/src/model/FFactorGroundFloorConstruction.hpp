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

#ifndef MODEL_FFACTORGROUNDFLOORCONSTRUCTION_HPP
#define MODEL_FFACTORGROUNDFLOORCONSTRUCTION_HPP

#include "ModelAPI.hpp"
#include "ConstructionBase.hpp"

namespace openstudio {
namespace model {

namespace detail{
  class FFactorGroundFloorConstruction_Impl;
}

/** FFactorGroundFloorConstruction is a ConstructionBase that wraps EnergyPlus IDD object
 *  'Construction:FfactorGroundFloor'. This is a simple, unlayered construction object that may
 *  be applied to a Surface (typically a ground floor). */
class MODEL_API FFactorGroundFloorConstruction : public ConstructionBase {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit FFactorGroundFloorConstruction(const Model& model,
    double fFactor = 0.1,
    double area = 0.1,
    double perimeterExposed = 0.1);

  virtual ~FFactorGroundFloorConstruction() {}

  /** Attribute Name: 'fFactor' */
  virtual double fFactor() const;
  virtual Quantity getFFactor(bool returnIP = false) const;
  virtual bool setFFactor(double fFactor);
  virtual bool setFFactor(const Quantity& fFactor);

  /** Attribute Name: 'area' */
  virtual double area() const;
  virtual Quantity getArea(bool returnIP = false) const;
  virtual bool setArea(double area);
  virtual bool setArea(const Quantity& area);

  /** Attribute Name: 'perimeterExposed' */
  virtual double perimeterExposed() const;
  virtual Quantity getPerimeterExposed(bool returnIP = false) const;
  virtual bool setPerimeterExposed(double perimeterExposed);
  virtual bool setPerimeterExposed(const Quantity& perimeterExposed);

  //@}
  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  //@}
 protected:
  /// @cond
  typedef detail::FFactorGroundFloorConstruction_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class detail::FFactorGroundFloorConstruction_Impl;

  explicit FFactorGroundFloorConstruction(
      std::shared_ptr<detail::FFactorGroundFloorConstruction_Impl> impl);

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.FFactorGroundFloorConstruction");
};

/** \relates FFactorGroundFloorConstruction */
typedef boost::optional<FFactorGroundFloorConstruction> OptionalFFactorGroundFloorConstruction;

/** \relates FFactorGroundFloorConstruction */
typedef std::vector<FFactorGroundFloorConstruction> FFactorGroundFloorConstructionVector;

} // model
} // openstudio

#endif // MODEL_FFACTORGROUNDFLOORCONSTRUCTION_HPP
