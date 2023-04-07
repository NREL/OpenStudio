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

#ifndef MODEL_INTERIORPARTITIONSURFACEGROUP_HPP
#define MODEL_INTERIORPARTITIONSURFACEGROUP_HPP

#include "ModelAPI.hpp"
#include "PlanarSurfaceGroup.hpp"

namespace openstudio {

class Transformation;

namespace model {

  class Space;
  class InteriorPartitionSurface;

  namespace detail {

    class InteriorPartitionSurfaceGroup_Impl;

  }  // namespace detail

  /** InteriorPartitionSurfaceGroup is a PlanarSurfaceGroup that wraps the OpenStudio IDD object 'OS_InteriorPartitionSurfaceGroup'. */
  class MODEL_API InteriorPartitionSurfaceGroup : public PlanarSurfaceGroup
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit InteriorPartitionSurfaceGroup(const Model& model);

    virtual ~InteriorPartitionSurfaceGroup() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    InteriorPartitionSurfaceGroup(const InteriorPartitionSurfaceGroup& other) = default;
    InteriorPartitionSurfaceGroup(InteriorPartitionSurfaceGroup&& other) = default;
    InteriorPartitionSurfaceGroup& operator=(const InteriorPartitionSurfaceGroup&) = default;
    InteriorPartitionSurfaceGroup& operator=(InteriorPartitionSurfaceGroup&&) = default;

    //@}
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    //@}
    /** @name Getters */
    //@{

    int multiplier() const;

    bool isMultiplierDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setMultiplier(int multiplier);

    void resetMultiplier();

    //@}

    /// Returns the parent Space.
    boost::optional<Space> space() const;

    /// Sets the parent Space.
    bool setSpace(const Space& space);

    void resetSpace();

    /// Returns all \link InteriorPartitionSurface InteriorPartitionSurfaces \endlink in this group.
    std::vector<InteriorPartitionSurface> interiorPartitionSurfaces() const;

   protected:
    /// @cond
    using ImplType = detail::InteriorPartitionSurfaceGroup_Impl;

    friend class Model;
    friend class openstudio::IdfObject;

    explicit InteriorPartitionSurfaceGroup(std::shared_ptr<detail::InteriorPartitionSurfaceGroup_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.InteriorPartitionSurfaceGroup");
  };

  /** \relates InteriorPartitionSurfaceGroup*/
  using OptionalInteriorPartitionSurfaceGroup = boost::optional<InteriorPartitionSurfaceGroup>;

  /** \relates InteriorPartitionSurfaceGroup*/
  using InteriorPartitionSurfaceGroupVector = std::vector<InteriorPartitionSurfaceGroup>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_INTERIORPARTITIONSURFACEGROUP_HPP
