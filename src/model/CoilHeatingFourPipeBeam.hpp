/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILHEATINGFOURPIPEBEAM_HPP
#define MODEL_COILHEATINGFOURPIPEBEAM_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Node;
  class Curve;
  class AirTerminalSingleDuctConstantVolumeFourPipeBeam;

  namespace detail {

    class CoilHeatingFourPipeBeam_Impl;

  }  // namespace detail

  /** CoilHeatingFourPipeBeam is a StraightComponent that wraps the OpenStudio IDD object 'OS:Coil:Heating:FourPipeBeam'. */
  class MODEL_API CoilHeatingFourPipeBeam : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilHeatingFourPipeBeam(const Model& model);

    virtual ~CoilHeatingFourPipeBeam() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilHeatingFourPipeBeam(const CoilHeatingFourPipeBeam& other) = default;
    CoilHeatingFourPipeBeam(CoilHeatingFourPipeBeam&& other) = default;
    CoilHeatingFourPipeBeam& operator=(const CoilHeatingFourPipeBeam&) = default;
    CoilHeatingFourPipeBeam& operator=(CoilHeatingFourPipeBeam&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    boost::optional<Node> hotWaterInletNode() const;

    boost::optional<Node> hotWaterOutletNode() const;

    double beamRatedHeatingCapacityperBeamLength() const;
    bool isBeamRatedHeatingCapacityperBeamLengthDefaulted() const;

    double beamRatedHeatingRoomAirHotWaterTemperatureDifference() const;
    bool isBeamRatedHeatingRoomAirHotWaterTemperatureDifferenceDefaulted() const;

    double beamRatedHotWaterVolumeFlowRateperBeamLength() const;
    bool isBeamRatedHotWaterVolumeFlowRateperBeamLengthDefaulted() const;

    Curve beamHeatingCapacityTemperatureDifferenceModificationFactorCurve() const;

    Curve beamHeatingCapacityAirFlowModificationFactorCurve() const;

    Curve beamHeatingCapacityHotWaterFlowModificationFactorCurve() const;

    //@}
    /** @name Setters */
    //@{

    bool setBeamRatedHeatingCapacityperBeamLength(double beamRatedHeatingCapacityperBeamLength);
    void resetBeamRatedHeatingCapacityperBeamLength();

    bool setBeamRatedHeatingRoomAirHotWaterTemperatureDifference(double beamRatedHeatingRoomAirHotWaterTemperatureDifference);
    void resetBeamRatedHeatingRoomAirHotWaterTemperatureDifference();

    bool setBeamRatedHotWaterVolumeFlowRateperBeamLength(double beamRatedHotWaterVolumeFlowRateperBeamLength);
    void resetBeamRatedHotWaterVolumeFlowRateperBeamLength();

    bool setBeamHeatingCapacityTemperatureDifferenceModificationFactorCurve(const Curve& curve);

    bool setBeamHeatingCapacityAirFlowModificationFactorCurve(const Curve& curve);

    bool setBeamHeatingCapacityHotWaterFlowModificationFactorCurve(const Curve& curve);

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
    using ImplType = detail::CoilHeatingFourPipeBeam_Impl;

    explicit CoilHeatingFourPipeBeam(std::shared_ptr<detail::CoilHeatingFourPipeBeam_Impl> impl);

    friend class detail::CoilHeatingFourPipeBeam_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilHeatingFourPipeBeam");
  };

  /** \relates CoilHeatingFourPipeBeam*/
  using OptionalCoilHeatingFourPipeBeam = boost::optional<CoilHeatingFourPipeBeam>;

  /** \relates CoilHeatingFourPipeBeam*/
  using CoilHeatingFourPipeBeamVector = std::vector<CoilHeatingFourPipeBeam>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGFOURPIPEBEAM_HPP
