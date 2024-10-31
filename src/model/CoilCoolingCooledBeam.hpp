/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

    virtual ~CoilCoolingCooledBeam() override = default;
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
