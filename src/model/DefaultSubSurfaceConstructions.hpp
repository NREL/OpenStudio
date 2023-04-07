/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef MODEL_DEFAULTSUBSURFACECONSTRUCTIONS_HPP
#define MODEL_DEFAULTSUBSURFACECONSTRUCTIONS_HPP

#include "ModelAPI.hpp"
#include "ResourceObject.hpp"

namespace openstudio {
namespace model {

  class ConstructionBase;

  namespace detail {

    class DefaultSubSurfaceConstructions_Impl;

  }  // namespace detail

  /** DefaultSubSurfaceConstructions is a ResourceObject that wraps the OpenStudio IDD object 'OS_DefaultSubSurfaceConstructions'. */
  class MODEL_API DefaultSubSurfaceConstructions : public ResourceObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit DefaultSubSurfaceConstructions(const Model& model);

    virtual ~DefaultSubSurfaceConstructions() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    DefaultSubSurfaceConstructions(const DefaultSubSurfaceConstructions& other) = default;
    DefaultSubSurfaceConstructions(DefaultSubSurfaceConstructions&& other) = default;
    DefaultSubSurfaceConstructions& operator=(const DefaultSubSurfaceConstructions&) = default;
    DefaultSubSurfaceConstructions& operator=(DefaultSubSurfaceConstructions&&) = default;

    //@}
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    //@}
    /** @name Getters */
    //@{

    boost::optional<ConstructionBase> fixedWindowConstruction() const;

    boost::optional<ConstructionBase> operableWindowConstruction() const;

    boost::optional<ConstructionBase> doorConstruction() const;

    boost::optional<ConstructionBase> glassDoorConstruction() const;

    boost::optional<ConstructionBase> overheadDoorConstruction() const;

    boost::optional<ConstructionBase> skylightConstruction() const;

    boost::optional<ConstructionBase> tubularDaylightDomeConstruction() const;

    boost::optional<ConstructionBase> tubularDaylightDiffuserConstruction() const;

    //@}
    /** @name Setters */
    //@{

    bool setFixedWindowConstruction(const ConstructionBase& construction);

    void resetFixedWindowConstruction();

    bool setOperableWindowConstruction(const ConstructionBase& construction);

    void resetOperableWindowConstruction();

    bool setDoorConstruction(const ConstructionBase& construction);

    void resetDoorConstruction();

    bool setGlassDoorConstruction(const ConstructionBase& construction);

    void resetGlassDoorConstruction();

    bool setOverheadDoorConstruction(const ConstructionBase& construction);

    void resetOverheadDoorConstruction();

    bool setSkylightConstruction(const ConstructionBase& construction);

    void resetSkylightConstruction();

    bool setTubularDaylightDomeConstruction(const ConstructionBase& construction);

    void resetTubularDaylightDomeConstruction();

    bool setTubularDaylightDiffuserConstruction(const ConstructionBase& construction);

    void resetTubularDaylightDiffuserConstruction();

    //@}

    /// Merge this object with other one, keep fields from this object if set otherwise set to value from other.
    void merge(const DefaultSubSurfaceConstructions& other);

   protected:
    /// @cond
    using ImplType = detail::DefaultSubSurfaceConstructions_Impl;

    friend class Model;
    friend class openstudio::IdfObject;

    explicit DefaultSubSurfaceConstructions(std::shared_ptr<detail::DefaultSubSurfaceConstructions_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.DefaultSubSurfaceConstructions");
  };

  /** \relates DefaultSubSurfaceConstructions*/
  using OptionalDefaultSubSurfaceConstructions = boost::optional<DefaultSubSurfaceConstructions>;

  /** \relates DefaultSubSurfaceConstructions*/
  using DefaultSubSurfaceConstructionsVector = std::vector<DefaultSubSurfaceConstructions>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_DEFAULTSUBSURFACECONSTRUCTIONS_HPP
