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

#ifndef MODEL_DAYLIGHTINGDEVICESHELF_HPP
#define MODEL_DAYLIGHTINGDEVICESHELF_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

class SubSurface;
class InteriorPartitionSurface;
class ShadingSurface;

namespace detail {

  class DaylightingDeviceShelf_Impl;

} // detail

/** DaylightingDeviceShelf is a ModelObject that wraps the OpenStudio IDD object 'OS_DaylightingDevice_Shelf'. */
class MODEL_API DaylightingDeviceShelf : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit DaylightingDeviceShelf(const SubSurface& subSurface);

  virtual ~DaylightingDeviceShelf() {}

  //@}
  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  //@}
  /** @name Getters */
  //@{

  boost::optional<InteriorPartitionSurface> insideShelf() const;

  boost::optional<ShadingSurface> outsideShelf() const;

  boost::optional<double> viewFactortoOutsideShelf() const;

  //@}
  /** @name Setters */
  //@{

  bool setInsideShelf(const InteriorPartitionSurface& insideShelf);

  bool setOutsideShelf(const ShadingSurface& outsideShelf);

  bool setViewFactortoOutsideShelf(double viewFactortoOutsideShelf);

  void resetViewFactortoOutsideShelf();

  //@}

  /// Get the related sub surface.
  SubSurface subSurface() const;

 protected:
  /// @cond
  typedef detail::DaylightingDeviceShelf_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class openstudio::detail::IdfObject_Impl;

  explicit DaylightingDeviceShelf(std::shared_ptr<detail::DaylightingDeviceShelf_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.DaylightingDeviceShelf");
};

/** \relates DaylightingDeviceShelf*/
typedef boost::optional<DaylightingDeviceShelf> OptionalDaylightingDeviceShelf;

/** \relates DaylightingDeviceShelf*/
typedef std::vector<DaylightingDeviceShelf> DaylightingDeviceShelfVector;

} // model
} // openstudio

#endif // MODEL_DAYLIGHTINGDEVICESHELF_HPP

