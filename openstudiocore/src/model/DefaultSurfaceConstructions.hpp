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

#ifndef MODEL_DEFAULTSURFACECONSTRUCTIONS_HPP
#define MODEL_DEFAULTSURFACECONSTRUCTIONS_HPP

#include "ModelAPI.hpp"
#include "ResourceObject.hpp"

namespace openstudio {
namespace model {

class ConstructionBase;

namespace detail {

  class DefaultSurfaceConstructions_Impl;

} // detail

/** DefaultSurfaceConstructions is a ResourceObject that wraps the OpenStudio IDD object 'OS_DefaultSurfaceConstructions'. */
class MODEL_API DefaultSurfaceConstructions : public ResourceObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit DefaultSurfaceConstructions(const Model& model);

  virtual ~DefaultSurfaceConstructions() {}

  //@}
  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  //@}
  /** @name Getters */
  //@{

  boost::optional<ConstructionBase> floorConstruction() const;

  boost::optional<ConstructionBase> wallConstruction() const;

  boost::optional<ConstructionBase> roofCeilingConstruction() const;

  //@}
  /** @name Setters */
  //@{

  bool setFloorConstruction(const ConstructionBase& construction);

  void resetFloorConstruction();

  bool setWallConstruction(const ConstructionBase& construction);

  void resetWallConstruction();

  bool setRoofCeilingConstruction(const ConstructionBase& construction);

  void resetRoofCeilingConstruction();

  //@}

  /// Merge this object with other one, keep fields from this object if set otherwise set to value from other.
  void merge(const DefaultSurfaceConstructions& other);

 protected:
  /// @cond
  typedef detail::DefaultSurfaceConstructions_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;

  explicit DefaultSurfaceConstructions(std::shared_ptr<detail::DefaultSurfaceConstructions_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.DefaultSurfaceConstructions");
};

/** \relates DefaultSurfaceConstructions*/
typedef boost::optional<DefaultSurfaceConstructions> OptionalDefaultSurfaceConstructions;

/** \relates DefaultSurfaceConstructions*/
typedef std::vector<DefaultSurfaceConstructions> DefaultSurfaceConstructionsVector;

} // model
} // openstudio

#endif // MODEL_DEFAULTSURFACECONSTRUCTIONS_HPP

