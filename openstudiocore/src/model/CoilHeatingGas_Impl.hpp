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

#ifndef MODEL_COILHEATINGGAS_IMPL_HPP
#define MODEL_COILHEATINGGAS_IMPL_HPP

#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Curve;

namespace detail {

  class MODEL_API CoilHeatingGas_Impl : public StraightComponent_Impl {
    Q_OBJECT;

    Q_PROPERTY(double gasBurnerEfficiency READ gasBurnerEfficiency WRITE setGasBurnerEfficiency);
    Q_PROPERTY(double parasiticElectricLoad READ parasiticElectricLoad WRITE setParasiticElectricLoad);
    Q_PROPERTY(double parasiticGasLoad READ parasiticGasLoad WRITE setParasiticGasLoad);

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> availabilitySchedule READ availabilityScheduleAsModelObject WRITE setAvailabilityScheduleAsModelObject);

  Q_PROPERTY(double gasBurnerEfficiency READ gasBurnerEfficiency WRITE setGasBurnerEfficiency);
  Q_PROPERTY(boost::optional<double> nominalCapacity READ nominalCapacity WRITE setNominalCapacity);
  Q_PROPERTY(double parasiticElectricLoad READ parasiticElectricLoad WRITE setParasiticElectricLoad);
  Q_PROPERTY(double parasiticGasLoad READ parasiticGasLoad WRITE setParasiticGasLoad);
  public:
    /** @name Constructors and Destructors */
    //2{

    // constructor
    CoilHeatingGas_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    // construct from workspace
    CoilHeatingGas_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                        Model_Impl* model,
                        bool keepHandle);

    // copy constructor
    CoilHeatingGas_Impl(const CoilHeatingGas_Impl& other, Model_Impl* model, bool keepHandle);

    // virtual destructor
    virtual ~CoilHeatingGas_Impl(){}

    //@}
    /** @name Virtual Methods */
    //@{

    // Get all output variable names that could be associated with this object.
    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    virtual unsigned inletPort();

    virtual unsigned outletPort();

    virtual boost::optional<HVACComponent> containingHVACComponent() const;
    virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const;

    //@}
    /** @name Getters and Setters */
    //@{

    Schedule availabilitySchedule() const;

    bool setAvailabilitySchedule(Schedule& schedule);

    // Get GasBurnerEfficiency
    double gasBurnerEfficiency();

    // Set GasBurnerEfficiency
    void setGasBurnerEfficiency(double val);

    // Get ParasiticElectricLoad
    double parasiticElectricLoad();

    // Set parasiticElectricLoad
    void setParasiticElectricLoad(double val);

    // Get ParasiticGasLoad
    double parasiticGasLoad();

    // Set parasiticGasLoad
    void setParasiticGasLoad(double val);

    boost::optional<double> nominalCapacity() const;

    OSOptionalQuantity getNominalCapacity(bool returnIP=false) const;

    bool isNominalCapacityAutosized() const;

    void setNominalCapacity(boost::optional<double> nominalCapacity);

    bool setNominalCapacity(const OSOptionalQuantity& nominalCapacity);

    void resetNominalCapacity();

    void autosizeNominalCapacity();

    boost::optional<Curve> partLoadFractionCorrelationCurve() const;

    bool setPartLoadFractionCorrelationCurve( const Curve& curve );

    void resetPartLoadFractionCorrelationCurve();

    std::vector<ModelObject> children() const;

    ModelObject clone(Model model) const;

    bool addToNode(Node & node);

    //@}
  private:
    REGISTER_LOGGER("openstudio.model.CoilHeatingGas");

    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<Schedule> optionalAvailabilitySchedule() const;

    boost::optional<ModelObject> availabilityScheduleAsModelObject() const;

    bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_COILHEATINGGAS_IMPL_HPP
