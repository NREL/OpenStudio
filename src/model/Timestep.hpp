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

#ifndef MODEL_TIMESTEP_HPP
#define MODEL_TIMESTEP_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {
    class Timestep_Impl;
  }  // namespace detail

  /** Timestep derives from ModelObject and is an interface to the OpenStudio IDD object named "Timestep".
 *
 *  Timestep allows the user to set the zone timestep for EnergyPlus simulations.
 *  Timestep does not have a public constructor because it is a unique ModelObject.
 *  To get the Timestep object for a Model or create one if it does not yet exist use model.getUniqueObject<Timestep>().
 *  To get the Timestep object for a Model but not create one if it does not yet exist use model.getOptionalUniqueObject<Timestep>().
 */
  class MODEL_API Timestep : public ModelObject
  {
   public:
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    virtual ~Timestep() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    Timestep(const Timestep& other) = default;
    Timestep(Timestep&& other) = default;
    Timestep& operator=(const Timestep&) = default;
    Timestep& operator=(Timestep&&) = default;

    //@}
    /** @name Getters */
    //@{

    int numberOfTimestepsPerHour() const;

    bool isNumberOfTimestepsPerHourDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setNumberOfTimestepsPerHour(int numberOfTimestepsPerHour);

    void resetNumberOfTimestepsPerHour();

    //@}

   protected:
    /** @name Constructors and Destructors */
    //@{

    /// Constructs a new Timestep object in the model.
    explicit Timestep(const Model& model);

    //@}

    /// @cond
    using ImplType = detail::Timestep_Impl;

    friend class Model;

    friend class openstudio::IdfObject;

    // constructor
    explicit Timestep(std::shared_ptr<detail::Timestep_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.Timestep");

    /// @endcond
  };

  /** \relates Timestep */
  using OptionalTimestep = boost::optional<Timestep>;

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_TIMESTEP_HPP
