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

#ifndef MODEL_GENERATORPVWATTS_HPP
#define MODEL_GENERATORPVWATTS_HPP

#include <model/ModelAPI.hpp>
#include "Generator.hpp"

#include "PlanarSurface.hpp"
#include "PlanarSurface_Impl.hpp"

namespace openstudio {
namespace model {

class PlanarSurface;

namespace detail {

  class GeneratorPVWatts_Impl;

} // detail

/** GeneratorPVWatts is a Generator that wraps the OpenStudio IDD object 'OS:Generator:PVWatts'. */
class MODEL_API GeneratorPVWatts : public Generator {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit GeneratorPVWatts(const Model& model, double dcSystemCapacity);

  explicit GeneratorPVWatts(const Model& model, const PlanarSurface& surface, double dcSystemCapacity);

  virtual ~GeneratorPVWatts() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  std::string pvWattsVersion() const;

  double dcSystemCapacity() const;

  std::string moduleType() const;

  bool isModuleTypeDefaulted() const;

  std::string arrayType() const;

  bool isArrayTypeDefaulted() const;

  double systemLosses() const;

  bool isSystemLossesDefaulted() const;

  double tiltAngle() const;

  bool isTiltAngleDefaulted() const;

  double azimuthAngle() const;

  bool isAzimuthAngleDefaulted() const;

  boost::optional<PlanarSurface> surface() const;

  double groundCoverageRatio() const;

  bool isGroundCoverageRatioDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  bool setDCSystemCapacity(double dcSystemCapacity);

  bool setModuleType(std::string moduleType);

  void resetModuleType();

  bool setArrayType(std::string arrayType);

  void resetArrayType();

  bool setSystemLosses(double systemLosses);

  void resetSystemLosses();

  bool setTiltAngle(double tiltAngle);

  void resetTiltAngle();

  bool setAzimuthAngle(double azimuthAngle);

  void resetAzimuthAngle();

  bool setSurface(const PlanarSurface& surface);

  void resetSurface();

  bool setGroundCoverageRatio(double groundCoverageRatio);

  void resetGroundCoverageRatio();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond

   typedef detail::GeneratorPVWatts_Impl ImplType;

   explicit GeneratorPVWatts(std::shared_ptr<detail::GeneratorPVWatts_Impl> impl);

   friend class detail::GeneratorPVWatts_Impl;
   friend class Model;
   friend class IdfObject;
   friend class openstudio::detail::IdfObject_Impl;

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.GeneratorPVWatts");
};

/** \relates GeneratorPVWatts*/
typedef boost::optional<GeneratorPVWatts> OptionalGeneratorPVWatts;

/** \relates GeneratorPVWatts*/
typedef std::vector<GeneratorPVWatts> GeneratorPVWattsVector;

} // model
} // openstudio

#endif // MODEL_GENERATORPVWATTS_HPP
