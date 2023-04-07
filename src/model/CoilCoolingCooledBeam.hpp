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

#ifndef MODEL_COILCOOLINGCOOLEDBEAM_HPP
#define MODEL_COILCOOLINGCOOLEDBEAM_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class CoilCoolingCooledBeam_Impl;

  }  // namespace detail

  /** CoilCoolingCooledBeam is a StraightComponent that wraps the OpenStudio IDD object 'OS:Coil:Cooling:CooledBeam'. */
  class MODEL_API CoilCoolingCooledBeam : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilCoolingCooledBeam(const Model& model);

    virtual ~CoilCoolingCooledBeam() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilCoolingCooledBeam(const CoilCoolingCooledBeam& other) = default;
    CoilCoolingCooledBeam(CoilCoolingCooledBeam&& other) = default;
    CoilCoolingCooledBeam& operator=(const CoilCoolingCooledBeam&) = default;
    CoilCoolingCooledBeam& operator=(CoilCoolingCooledBeam&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    //unsigned inletPort() const;

    //unsigned outletPort() const;

    double coilSurfaceAreaperCoilLength() const;

    bool isCoilSurfaceAreaperCoilLengthDefaulted() const;

    double modelParametera() const;

    bool isModelParameteraDefaulted() const;

    double modelParametern1() const;

    bool isModelParametern1Defaulted() const;

    double modelParametern2() const;

    bool isModelParametern2Defaulted() const;

    double modelParametern3() const;

    bool isModelParametern3Defaulted() const;

    double modelParametera0() const;

    bool isModelParametera0Defaulted() const;

    double modelParameterK1() const;

    bool isModelParameterK1Defaulted() const;

    double modelParametern() const;

    bool isModelParameternDefaulted() const;

    double leavingPipeInsideDiameter() const;

    bool isLeavingPipeInsideDiameterDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setCoilSurfaceAreaperCoilLength(double coilSurfaceAreaperCoilLength);

    void resetCoilSurfaceAreaperCoilLength();

    bool setModelParametera(double modelParametera);

    void resetModelParametera();

    bool setModelParametern1(double modelParametern1);

    void resetModelParametern1();

    bool setModelParametern2(double modelParametern2);

    void resetModelParametern2();

    bool setModelParametern3(double modelParametern3);

    void resetModelParametern3();

    bool setModelParametera0(double modelParametera0);

    void resetModelParametera0();

    bool setModelParameterK1(double modelParameterK1);

    void resetModelParameterK1();

    bool setModelParametern(double modelParametern);

    void resetModelParametern();

    bool setLeavingPipeInsideDiameter(double leavingPipeInsideDiameter);

    void resetLeavingPipeInsideDiameter();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilCoolingCooledBeam_Impl;

    explicit CoilCoolingCooledBeam(std::shared_ptr<detail::CoilCoolingCooledBeam_Impl> impl);

    friend class detail::CoilCoolingCooledBeam_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilCoolingCooledBeam");
  };

  /** \relates CoilCoolingCooledBeam*/
  using OptionalCoilCoolingCooledBeam = boost::optional<CoilCoolingCooledBeam>;

  /** \relates CoilCoolingCooledBeam*/
  using CoilCoolingCooledBeamVector = std::vector<CoilCoolingCooledBeam>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGCOOLEDBEAM_HPP
