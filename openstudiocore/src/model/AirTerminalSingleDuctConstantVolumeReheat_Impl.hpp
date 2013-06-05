/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#ifndef MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMEREHEAT_IMPL_HPP
#define MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMEREHEAT_IMPL_HPP

#include <model/ModelAPI.hpp>
#include <model/StraightComponent_Impl.hpp>

#include <utilities/units/Quantity.hpp>
#include <utilities/units/OSOptionalQuantity.hpp>

namespace openstudio {
namespace model {

// TODO: Check the following class names against object getters and setters.
class Schedule;
class Connection;
class Connection;
class HeatingCoilName;

namespace detail {

  /** AirTerminalSingleDuctConstantVolumeReheat_Impl is a StraightComponent_Impl that is the implementation class for AirTerminalSingleDuctConstantVolumeReheat.*/
  class MODEL_API AirTerminalSingleDuctConstantVolumeReheat_Impl : public StraightComponent_Impl {
    Q_OBJECT;

    Q_PROPERTY(boost::optional<double> maximumAirFlowRate READ maximumAirFlowRate WRITE setMaximumAirFlowRate RESET resetMaximumAirFlowRate);
    Q_PROPERTY(openstudio::OSOptionalQuantity maximumAirFlowRate_SI READ maximumAirFlowRate_SI WRITE setMaximumAirFlowRate RESET resetMaximumAirFlowRate);
    Q_PROPERTY(openstudio::OSOptionalQuantity maximumAirFlowRate_IP READ maximumAirFlowRate_IP WRITE setMaximumAirFlowRate RESET resetMaximumAirFlowRate);
    Q_PROPERTY(bool isMaximumAirFlowRateAutosized READ isMaximumAirFlowRateAutosized);

    Q_PROPERTY(boost::optional<double> maximumHotWaterorSteamFlowRate READ maximumHotWaterorSteamFlowRate WRITE setMaximumHotWaterorSteamFlowRate RESET resetMaximumHotWaterorSteamFlowRate);
    Q_PROPERTY(openstudio::OSOptionalQuantity maximumHotWaterorSteamFlowRate_SI READ maximumHotWaterorSteamFlowRate_SI WRITE setMaximumHotWaterorSteamFlowRate RESET resetMaximumHotWaterorSteamFlowRate);
    Q_PROPERTY(openstudio::OSOptionalQuantity maximumHotWaterorSteamFlowRate_IP READ maximumHotWaterorSteamFlowRate_IP WRITE setMaximumHotWaterorSteamFlowRate RESET resetMaximumHotWaterorSteamFlowRate);
    Q_PROPERTY(bool isMaximumHotWaterorSteamFlowRateAutosized READ isMaximumHotWaterorSteamFlowRateAutosized);

    Q_PROPERTY(double minimumHotWaterorSteamFlowRate READ minimumHotWaterorSteamFlowRate WRITE setMinimumHotWaterorSteamFlowRate RESET resetMinimumHotWaterorSteamFlowRate);
    Q_PROPERTY(openstudio::Quantity minimumHotWaterorSteamFlowRate_SI READ minimumHotWaterorSteamFlowRate_SI WRITE setMinimumHotWaterorSteamFlowRate RESET resetMinimumHotWaterorSteamFlowRate);
    Q_PROPERTY(openstudio::Quantity minimumHotWaterorSteamFlowRate_IP READ minimumHotWaterorSteamFlowRate_IP WRITE setMinimumHotWaterorSteamFlowRate RESET resetMinimumHotWaterorSteamFlowRate);
    Q_PROPERTY(bool isMinimumHotWaterorSteamFlowRateDefaulted READ isMinimumHotWaterorSteamFlowRateDefaulted);

    Q_PROPERTY(double convergenceTolerance READ convergenceTolerance WRITE setConvergenceTolerance RESET resetConvergenceTolerance);
    Q_PROPERTY(openstudio::Quantity convergenceTolerance_SI READ convergenceTolerance_SI WRITE setConvergenceTolerance RESET resetConvergenceTolerance);
    Q_PROPERTY(openstudio::Quantity convergenceTolerance_IP READ convergenceTolerance_IP WRITE setConvergenceTolerance RESET resetConvergenceTolerance);
    Q_PROPERTY(bool isConvergenceToleranceDefaulted READ isConvergenceToleranceDefaulted);

    Q_PROPERTY(double maximumReheatAirTemperature READ maximumReheatAirTemperature WRITE setMaximumReheatAirTemperature RESET resetMaximumReheatAirTemperature);
    Q_PROPERTY(openstudio::Quantity maximumReheatAirTemperature_SI READ maximumReheatAirTemperature_SI WRITE setMaximumReheatAirTemperature RESET resetMaximumReheatAirTemperature);
    Q_PROPERTY(openstudio::Quantity maximumReheatAirTemperature_IP READ maximumReheatAirTemperature_IP WRITE setMaximumReheatAirTemperature RESET resetMaximumReheatAirTemperature);
    Q_PROPERTY(bool isMaximumReheatAirTemperatureDefaulted READ isMaximumReheatAirTemperatureDefaulted);

    // TODO: Add relationships for objects related to this one, but not pointed to by the underlying data.
    //       Such relationships can be generated by the GenerateRelationships.rb script.
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> availabilitySchedule READ availabilityScheduleAsModelObject WRITE setAvailabilityScheduleAsModelObject);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> airOutletNode READ airOutletNodeAsModelObject WRITE setAirOutletNodeAsModelObject);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> airInletNode READ airInletNodeAsModelObject WRITE setAirInletNodeAsModelObject);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> reheatCoil READ reheatCoilAsModelObject WRITE setReheatCoilAsModelObject RESET resetReheatCoil);
   public:
    /** @name Constructors and Destructors */
    //@{

    AirTerminalSingleDuctConstantVolumeReheat_Impl(const IdfObject& idfObject,
                                                   Model_Impl* model,
                                                   bool keepHandle);

    AirTerminalSingleDuctConstantVolumeReheat_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                   Model_Impl* model,
                                                   bool keepHandle);

    AirTerminalSingleDuctConstantVolumeReheat_Impl(const AirTerminalSingleDuctConstantVolumeReheat_Impl& other,
                                                   Model_Impl* model,
                                                   bool keepHandle);

    virtual ~AirTerminalSingleDuctConstantVolumeReheat_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    virtual unsigned inletPort();

    virtual unsigned outletPort();

    virtual bool addToNode(Node & node);

    virtual std::vector<ModelObject> children() const;

    virtual std::vector<openstudio::IdfObject> remove();

    virtual bool isRemovable() const;

    virtual ModelObject clone(Model model) const;

    //@}
    /** @name Getters */
    //@{

    // TODO: Check return type. From object lists, some candidates are: Schedule.
    Schedule availabilitySchedule() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    Connection airOutletNode() const;

    // TODO: Check return type. From object lists, some candidates are: Connection.
    Connection airInletNode() const;

    boost::optional<double> maximumAirFlowRate() const;

    OSOptionalQuantity getMaximumAirFlowRate(bool returnIP=false) const;

    bool isMaximumAirFlowRateAutosized() const;

    // TODO: Check return type. From object lists, some candidates are: HeatingCoilName.
    HVACComponent reheatCoil() const;

    boost::optional<double> maximumHotWaterorSteamFlowRate() const;

    OSOptionalQuantity getMaximumHotWaterorSteamFlowRate(bool returnIP=false) const;

    bool isMaximumHotWaterorSteamFlowRateAutosized() const;

    double minimumHotWaterorSteamFlowRate() const;

    Quantity getMinimumHotWaterorSteamFlowRate(bool returnIP=false) const;

    bool isMinimumHotWaterorSteamFlowRateDefaulted() const;

    double convergenceTolerance() const;

    Quantity getConvergenceTolerance(bool returnIP=false) const;

    bool isConvergenceToleranceDefaulted() const;

    double maximumReheatAirTemperature() const;

    Quantity getMaximumReheatAirTemperature(bool returnIP=false) const;

    bool isMaximumReheatAirTemperatureDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    // TODO: Check argument type. From object lists, some candidates are: Schedule.
    bool setAvailabilitySchedule(Schedule& schedule);

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setAirOutletNode(const Connection& connection);

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setAirInletNode(const Connection& connection);

    bool setMaximumAirFlowRate(boost::optional<double> maximumAirFlowRate);

    bool setMaximumAirFlowRate(const OSOptionalQuantity& maximumAirFlowRate);

    void resetMaximumAirFlowRate();

    void autosizeMaximumAirFlowRate();

    // TODO: Check argument type. From object lists, some candidates are: HeatingCoilName.
    bool setReheatCoil(const HVACComponent& heatingCoilName);

    void resetReheatCoil();

    bool setMaximumHotWaterorSteamFlowRate(boost::optional<double> maximumHotWaterorSteamFlowRate);

    bool setMaximumHotWaterorSteamFlowRate(const OSOptionalQuantity& maximumHotWaterorSteamFlowRate);

    void resetMaximumHotWaterorSteamFlowRate();

    void autosizeMaximumHotWaterorSteamFlowRate();

    bool setMinimumHotWaterorSteamFlowRate(double minimumHotWaterorSteamFlowRate);

    bool setMinimumHotWaterorSteamFlowRate(const Quantity& minimumHotWaterorSteamFlowRate);

    void resetMinimumHotWaterorSteamFlowRate();

    bool setConvergenceTolerance(double convergenceTolerance);

    bool setConvergenceTolerance(const Quantity& convergenceTolerance);

    void resetConvergenceTolerance();

    bool setMaximumReheatAirTemperature(double maximumReheatAirTemperature);

    bool setMaximumReheatAirTemperature(const Quantity& maximumReheatAirTemperature);

    void resetMaximumReheatAirTemperature();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.AirTerminalSingleDuctConstantVolumeReheat");

    // TODO: Check the return types of these methods.
    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<HVACComponent> optionalReheatCoil() const;
    boost::optional<Schedule> optionalAvailabilitySchedule() const;
    boost::optional<Connection> optionalAirOutletNode() const;
    boost::optional<Connection> optionalAirInletNode() const;

    openstudio::OSOptionalQuantity maximumAirFlowRate_SI() const;
    openstudio::OSOptionalQuantity maximumAirFlowRate_IP() const;
    openstudio::OSOptionalQuantity maximumHotWaterorSteamFlowRate_SI() const;
    openstudio::OSOptionalQuantity maximumHotWaterorSteamFlowRate_IP() const;
    openstudio::Quantity minimumHotWaterorSteamFlowRate_SI() const;
    openstudio::Quantity minimumHotWaterorSteamFlowRate_IP() const;
    openstudio::Quantity convergenceTolerance_SI() const;
    openstudio::Quantity convergenceTolerance_IP() const;
    openstudio::Quantity maximumReheatAirTemperature_SI() const;
    openstudio::Quantity maximumReheatAirTemperature_IP() const;

    boost::optional<ModelObject> availabilityScheduleAsModelObject() const;
    boost::optional<ModelObject> airOutletNodeAsModelObject() const;
    boost::optional<ModelObject> airInletNodeAsModelObject() const;
    boost::optional<ModelObject> reheatCoilAsModelObject() const;

    bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setAirOutletNodeAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setAirInletNodeAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setReheatCoilAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMEREHEAT_IMPL_HPP

