/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

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
  class MODEL_API SpaceInfiltrationDesignFlowRate_Impl : public SpaceLoad_Impl {
    Q_OBJECT;
    Q_PROPERTY(std::string designFlowRateCalculationMethod READ designFlowRateCalculationMethod);
    Q_PROPERTY(boost::optional<double> designFlowRate READ designFlowRate WRITE setDesignFlowRate);
    Q_PROPERTY(boost::optional<double> flowperSpaceFloorArea READ flowperSpaceFloorArea WRITE setFlowperSpaceFloorArea);
    Q_PROPERTY(boost::optional<double> flowperExteriorSurfaceArea READ flowperExteriorSurfaceArea WRITE setFlowperExteriorSurfaceArea);
    Q_PROPERTY(boost::optional<double> flowperExteriorWallArea READ flowperExteriorWallArea WRITE setFlowperExteriorWallArea);
    Q_PROPERTY(boost::optional<double> airChangesperHour READ airChangesperHour WRITE setAirChangesperHour);
    Q_PROPERTY(double constantTermCoefficient READ constantTermCoefficient WRITE setConstantTermCoefficient RESET resetConstantTermCoefficient);
    Q_PROPERTY(bool isConstantTermCoefficientDefaulted READ isConstantTermCoefficientDefaulted);
    Q_PROPERTY(double temperatureTermCoefficient READ temperatureTermCoefficient WRITE setTemperatureTermCoefficient RESET resetTemperatureTermCoefficient);
    Q_PROPERTY(bool isTemperatureTermCoefficientDefaulted READ isTemperatureTermCoefficientDefaulted);
    Q_PROPERTY(double velocityTermCoefficient READ velocityTermCoefficient WRITE setVelocityTermCoefficient RESET resetVelocityTermCoefficient);
    Q_PROPERTY(bool isVelocityTermCoefficientDefaulted READ isVelocityTermCoefficientDefaulted);
    Q_PROPERTY(double velocitySquaredTermCoefficient READ velocitySquaredTermCoefficient WRITE setVelocitySquaredTermCoefficient RESET resetVelocitySquaredTermCoefficient);
    Q_PROPERTY(bool isVelocitySquaredTermCoefficientDefaulted READ isVelocitySquaredTermCoefficientDefaulted);

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> schedule READ scheduleAsModelObject WRITE setScheduleAsModelObject RESET resetSchedule);
   public:
    /** @name Constructors and Destructors */
    //@{

    SpaceInfiltrationDesignFlowRate_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    SpaceInfiltrationDesignFlowRate_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                         Model_Impl* model,
                                         bool keepHandle);

    SpaceInfiltrationDesignFlowRate_Impl(const SpaceInfiltrationDesignFlowRate_Impl& other,
                                         Model_Impl* model,
                                         bool keepHandle);

    virtual ~SpaceInfiltrationDesignFlowRate_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;
    
    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    virtual bool hardSize();

    virtual bool hardApplySchedules();

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
    double getDesignFlowRate(double floorArea,
                             double exteriorSurfaceArea,
                             double exteriorWallArea,
                             double airVolume) const;

    /** Returns the design flow per space floor area (m^3/m^2*s) based on the
     *  floorArea (m^2), exteriorSurfaceArea (m^2), exteriorWallArea (m^2), and
     *  airVolume (m^3). */
    double getFlowPerSpaceFloorArea(double floorArea,
                                    double exteriorSurfaceArea,
                                    double exteriorWallArea,
                                    double airVolume) const;

    /** Returns the design flow per exterior surface area (m^3/m^2*s) based on the
     *  floorArea (m^2), exteriorSurfaceArea (m^2), exteriorWallArea (m^2), and
     *  airVolume (m^3). */
    double getFlowPerExteriorSurfaceArea(double floorArea,
                                         double exteriorSurfaceArea,
                                         double exteriorWallArea,
                                         double airVolume) const;

    /** Returns the design flow per exterior wall area (m^3/m^2*s) based on the
     *  floorArea (m^2), exteriorSurfaceArea (m^2), exteriorWallArea (m^2), and
     *  airVolume (m^3). */
    double getFlowPerExteriorWallArea(double floorArea,
                                      double exteriorSurfaceArea,
                                      double exteriorWallArea,
                                      double airVolume) const;

    /** Returns the design air changes per hour (1/h) based on the floorArea (m^2),
     *  exteriorSurfaceArea (m^2), exteriorWallArea (m^2), and airVolume (m^3). */
    double getAirChangesPerHour(double floorArea,
                                double exteriorSurfaceArea,
                                double exteriorWallArea,
                                double airVolume) const;

    //@}
   protected:

    // index of the space name
    virtual int spaceIndex() const;

   private:
    REGISTER_LOGGER("openstudio.model.SpaceInfiltrationDesignFlowRate");

    boost::optional<ModelObject> scheduleAsModelObject() const;

    bool setScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_SPACEINFILTRATIONDESIGNFLOWRATE_IMPL_HPP

