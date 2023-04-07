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

#ifndef MODEL_SPACELOAD_HPP
#define MODEL_SPACELOAD_HPP

#include "ModelAPI.hpp"
#include "SpaceItem.hpp"

namespace openstudio {
namespace model {

  class SpaceType;

  namespace detail {
    class SpaceLoad_Impl;
  }

  /** SpaceLoad is an abstract class derived from SpaceItem.
 */
  class MODEL_API SpaceLoad : public SpaceItem
  {
   public:
    virtual ~SpaceLoad() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SpaceLoad(const SpaceLoad& other) = default;
    SpaceLoad(SpaceLoad&& other) = default;
    SpaceLoad& operator=(const SpaceLoad&) = default;
    SpaceLoad& operator=(SpaceLoad&&) = default;

    /// Returns the parent SpaceType.
    boost::optional<SpaceType> spaceType() const;

    /// Sets the parent SpaceType.
    bool setSpaceType(const SpaceType& spaceType);

    void resetSpaceType();

    /// Converts space load levels to absolute values rather than by per area, etc.
    /// Also hard sizes any costs associated with this load.
    /// Only possible if this space load is parented by a space, returns false if
    /// parented by a space type.
    bool hardSize();

    /// Directly applies default schedules to this space load if found.
    bool hardApplySchedules();

    /// Returns true if the load represents an absolute quantity that must be added when combining space types
    bool isAbsolute() const;

   protected:
    /** @name Constructors and Destructors */
    //@{

    /// Constructs a new SpaceLoad object in the model.
    SpaceLoad(IddObjectType type, const Model& model);

    //@}

    /// @cond

    using ImplType = detail::SpaceLoad_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class detail::SpaceLoad_Impl;

    explicit SpaceLoad(std::shared_ptr<detail::SpaceLoad_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.SpaceLoad");

    /// @endcond
  };

  /** \relates SpaceLoad */
  using OptionalSpaceLoad = boost::optional<SpaceLoad>;

  /** \relates SpaceLoad */
  using SpaceLoadVector = std::vector<SpaceLoad>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SPACELOAD_HPP
