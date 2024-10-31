/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILCOOLINGFOURPIPEBEAM_HPP
#define MODEL_COILCOOLINGFOURPIPEBEAM_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Node;
  class Curve;
  class AirTerminalSingleDuctConstantVolumeFourPipeBeam;

  namespace detail {

    class CoilCoolingFourPipeBeam_Impl;

  }  // namespace detail

  /** CoilCoolingFourPipeBeam is a StraightComponent that wraps the OpenStudio IDD object 'OS:Coil:Cooling:FourPipeBeam'. */
  class MODEL_API CoilCoolingFourPipeBeam : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilCoolingFourPipeBeam(const Model& model);

    virtual ~CoilCoolingFourPipeBeam() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilCoolingFourPipeBeam(const CoilCoolingFourPipeBeam& other) = default;
    CoilCoolingFourPipeBeam(CoilCoolingFourPipeBeam&& other) = default;
    CoilCoolingFourPipeBeam& operator=(const CoilCoolingFourPipeBeam&) = default;
    CoilCoolingFourPipeBeam& operator=(CoilCoolingFourPipeBeam&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    boost::optional<Node> chilledWaterInletNode() const;

    boost::optional<Node> chilledWaterOutletNode() const;

    double beamRatedCoolingCapacityperBeamLength() const;
    bool isBeamRatedCoolingCapacityperBeamLengthDefaulted() const;

    double beamRatedCoolingRoomAirChilledWaterTemperatureDifference() const;
    bool isBeamRatedCoolingRoomAirChilledWaterTemperatureDifferenceDefaulted() const;

    double beamRatedChilledWaterVolumeFlowRateperBeamLength() const;
    bool isBeamRatedChilledWaterVolumeFlowRateperBeamLengthDefaulted() const;

    Curve beamCoolingCapacityTemperatureDifferenceModificationFactorCurve() const;

    Curve beamCoolingCapacityAirFlowModificationFactorCurve() const;

    Curve beamCoolingCapacityChilledWaterFlowModificationFactorCurve() const;

    //@}
    /** @name Setters */
    //@{

    bool setBeamRatedCoolingCapacityperBeamLength(double beamRatedCoolingCapacityperBeamLength);
    void resetBeamRatedCoolingCapacityperBeamLength();

    bool setBeamRatedCoolingRoomAirChilledWaterTemperatureDifference(double beamRatedCoolingRoomAirChilledWaterTemperatureDifference);
    void resetBeamRatedCoolingRoomAirChilledWaterTemperatureDifference();

    bool setBeamRatedChilledWaterVolumeFlowRateperBeamLength(double beamRatedChilledWaterVolumeFlowRateperBeamLength);
    void resetBeamRatedChilledWaterVolumeFlowRateperBeamLength();

    bool setBeamCoolingCapacityTemperatureDifferenceModificationFactorCurve(const Curve& curve);

    bool setBeamCoolingCapacityAirFlowModificationFactorCurve(const Curve& curve);

    bool setBeamCoolingCapacityChilledWaterFlowModificationFactorCurve(const Curve& curve);

    // No reset functions provided for the three curves, because all curves are required if you connect the ATU FourPipeBeam to a hot water plant loop
    // And given that we added a subclass specifically for the heating portion of the ATU FourPipeBeam, they become required at all times

    //@}
    /** @name Other */
    //@{

    /* Convenience function to return the parent AirTerminal:SingleDuct:ConstantVolume:FourPipeBeam */
    boost::optional<AirTerminalSingleDuctConstantVolumeFourPipeBeam> airTerminalSingleDuctConstantVolumeFourPipeBeam() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilCoolingFourPipeBeam_Impl;

    explicit CoilCoolingFourPipeBeam(std::shared_ptr<detail::CoilCoolingFourPipeBeam_Impl> impl);

    friend class detail::CoilCoolingFourPipeBeam_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilCoolingFourPipeBeam");
  };

  /** \relates CoilCoolingFourPipeBeam*/
  using OptionalCoilCoolingFourPipeBeam = boost::optional<CoilCoolingFourPipeBeam>;

  /** \relates CoilCoolingFourPipeBeam*/
  using CoilCoolingFourPipeBeamVector = std::vector<CoilCoolingFourPipeBeam>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGFOURPIPEBEAM_HPP
