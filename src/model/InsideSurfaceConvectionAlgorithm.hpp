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

#ifndef MODEL_INSIDESURFACECONVECTIONALGORITHM_HPP
#define MODEL_INSIDESURFACECONVECTIONALGORITHM_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {
    class InsideSurfaceConvectionAlgorithm_Impl;
  }  // namespace detail

  /** InsideSurfaceConvectionAlgorithm derives from ModelObject and is an interface to the OpenStudio IDD object named "SurfaceConvectionAlgorithm:Inside".
 *
 *  InsideSurfaceConvectionAlgorithm is a unique object that specifies the global model used for surface convection at the inside face of all the
 *  heat transfer surfaces in the model.  This global algorithm may be overridden for specific Surface, SurfaceList, or Zone objects.
 *  InsideSurfaceConvectionAlgorithm does not have a public constructor because it is a unique ModelObject.
 *  To get the InsideSurfaceConvectionAlgorithm object for a Model or create one if it does not yet exist use model.getUniqueObject<InsideSurfaceConvectionAlgorithm>().
 *  To get the InsideSurfaceConvectionAlgorithm object for a Model but not create one if it does not yet exist use model.getOptionalUniqueObject<InsideSurfaceConvectionAlgorithm>().
 */
  class MODEL_API InsideSurfaceConvectionAlgorithm : public ModelObject
  {
   public:
    virtual ~InsideSurfaceConvectionAlgorithm() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    InsideSurfaceConvectionAlgorithm(const InsideSurfaceConvectionAlgorithm& other) = default;
    InsideSurfaceConvectionAlgorithm(InsideSurfaceConvectionAlgorithm&& other) = default;
    InsideSurfaceConvectionAlgorithm& operator=(const InsideSurfaceConvectionAlgorithm&) = default;
    InsideSurfaceConvectionAlgorithm& operator=(InsideSurfaceConvectionAlgorithm&&) = default;

    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    static std::vector<std::string> validAlgorithmValues();

    //@}
    /** @name Getters */
    //@{

    std::string algorithm() const;

    bool isAlgorithmDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setAlgorithm(const std::string& algorithm);

    void resetAlgorithm();

    //@}

   protected:
    /** @name Constructors and Destructors */
    //@{

    /// Constructs a new InsideSurfaceConvectionAlgorithm object in the model.
    explicit InsideSurfaceConvectionAlgorithm(const Model& model);

    //@}

    /// @cond

    using ImplType = detail::InsideSurfaceConvectionAlgorithm_Impl;

    friend class Model;
    friend class openstudio::IdfObject;

    // constructor
    explicit InsideSurfaceConvectionAlgorithm(std::shared_ptr<detail::InsideSurfaceConvectionAlgorithm_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.InsideSurfaceConvectionAlgorithm");

    /// @endcond
  };

  /** \relates InsideSurfaceConvectionAlgorithm */
  using OptionalInsideSurfaceConvectionAlgorithm = boost::optional<InsideSurfaceConvectionAlgorithm>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_INSIDESURFACECONVECTIONALGORITHM_HPP
