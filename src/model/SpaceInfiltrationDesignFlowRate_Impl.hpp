/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SPACEINFILTRATIONDESIGNFLOWRATE_IMPL_HPP
#define MODEL_SPACEINFILTRATIONDESIGNFLOWRATE_IMPL_HPP

#include "ModelAPI.hpp"
#include "SpaceLoad_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class SpaceInfiltrationDesignFlowRate;

  namespace detail {

    /** SpaceInfiltrationDesignFlowRate_Impl is a SpaceLoad_Impl that is the implementation class for SpaceInfiltrationDesignFlowRate.*/
    class MODEL_API SpaceInfiltrationDesignFlowRate_Impl : public SpaceLoad_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      SpaceInfiltrationDesignFlowRate_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SpaceInfiltrationDesignFlowRate_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SpaceInfiltrationDesignFlowRate_Impl(const SpaceInfiltrationDesignFlowRate_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~SpaceInfiltrationDesignFlowRate_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual bool hardSize() override;

      virtual bool hardApplySchedules() override;

      virtual bool isAbsolute() const override;

      //@}
      /** @name Getters */
      //@{

      /** Returns the (fractional) infiltration schedule.  If this object does not
     *  specify a schedule this function will search the hierarchy. */
      boost::optional<Schedule> schedule() const;

      /** Returns true if this object does not specify a schedule directly. */
      bool isScheduleDefaulted() const;

      std::string designFlowRateCalculationMethod() const;

      boost::optional<double> designFlowRate() const;

      boost::optional<double> flowperSpaceFloorArea() const;

      boost::optional<double> flowperExteriorSurfaceArea() const;

      boost::optional<double> flowperExteriorWallArea() const;

      boost::optional<double> airChangesperHour() const;

      double constantTermCoefficient() const;

      bool isConstantTermCoefficientDefaulted() const;

      double temperatureTermCoefficient() const;

      bool isTemperatureTermCoefficientDefaulted() const;

      double velocityTermCoefficient() const;

      bool isVelocityTermCoefficientDefaulted() const;

      double velocitySquaredTermCoefficient() const;

      bool isVelocitySquaredTermCoefficientDefaulted() const;

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

      //@}
      /** @name Setters */
      //@{

      /** Sets the (fractional) Schedule. */
      bool setSchedule(Schedule& schedule);

      /** Resets the (fractional) Schedule. */
      void resetSchedule();

      bool setDesignFlowRate(boost::optional<double> designFlowRate);

      bool setFlowperSpaceFloorArea(boost::optional<double> flowperSpaceFloorArea);

      bool setFlowperExteriorSurfaceArea(boost::optional<double> flowperExteriorSurfaceArea);

      bool setFlowperExteriorWallArea(boost::optional<double> flowperExteriorWallArea);

      bool setAirChangesperHour(boost::optional<double> airChangesperHour);

      bool setConstantTermCoefficient(double constantTermCoefficient);

      void resetConstantTermCoefficient();

      bool setTemperatureTermCoefficient(double temperatureTermCoefficient);

      void resetTemperatureTermCoefficient();

      bool setVelocityTermCoefficient(double velocityTermCoefficient);

      void resetVelocityTermCoefficient();

      bool setVelocitySquaredTermCoefficient(double velocitySquaredTermCoefficient);

      void resetVelocitySquaredTermCoefficient();

      //@}
      /** @name Other */
      //@{

      /** Returns the design flow rate (m^3/s) based on the floorArea (m^2),
     *  exteriorSurfaceArea (m^2), exteriorWallArea (m^2), and airVolume (m^3). */
      double getDesignFlowRate(double floorArea, double exteriorSurfaceArea, double exteriorWallArea, double airVolume) const;

      /** Returns the design flow per space floor area (m^3/m^2*s) based on the
     *  floorArea (m^2), exteriorSurfaceArea (m^2), exteriorWallArea (m^2), and
     *  airVolume (m^3). */
      double getFlowPerSpaceFloorArea(double floorArea, double exteriorSurfaceArea, double exteriorWallArea, double airVolume) const;

      /** Returns the design flow per exterior surface area (m^3/m^2*s) based on the
     *  floorArea (m^2), exteriorSurfaceArea (m^2), exteriorWallArea (m^2), and
     *  airVolume (m^3). */
      double getFlowPerExteriorSurfaceArea(double floorArea, double exteriorSurfaceArea, double exteriorWallArea, double airVolume) const;

      /** Returns the design flow per exterior wall area (m^3/m^2*s) based on the
     *  floorArea (m^2), exteriorSurfaceArea (m^2), exteriorWallArea (m^2), and
     *  airVolume (m^3). */
      double getFlowPerExteriorWallArea(double floorArea, double exteriorSurfaceArea, double exteriorWallArea, double airVolume) const;

      /** Returns the design air changes per hour (1/h) based on the floorArea (m^2),
     *  exteriorSurfaceArea (m^2), exteriorWallArea (m^2), and airVolume (m^3). */
      double getAirChangesPerHour(double floorArea, double exteriorSurfaceArea, double exteriorWallArea, double airVolume) const;

      //@}
     protected:
      // index of the space name
      virtual int spaceIndex() const override;

     private:
      REGISTER_LOGGER("openstudio.model.SpaceInfiltrationDesignFlowRate");

      boost::optional<ModelObject> scheduleAsModelObject() const;

      bool setScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SPACEINFILTRATIONDESIGNFLOWRATE_IMPL_HPP
