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

#ifndef MODEL_COILHEATINGGASMULTISTAGE_IMPL_HPP
#define MODEL_COILHEATINGGASMULTISTAGE_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;
class Curve;
class CoilHeatingGasMultiStageStageData;

namespace detail {

  /** CoilHeatingGasMultiStage_Impl is a StraightComponent_Impl that is the implementation class for CoilHeatingGasMultiStage.*/
  class MODEL_API CoilHeatingGasMultiStage_Impl : public StraightComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    CoilHeatingGasMultiStage_Impl(const IdfObject& idfObject,
                                  Model_Impl* model,
                                  bool keepHandle);

    CoilHeatingGasMultiStage_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                  Model_Impl* model,
                                  bool keepHandle);

    CoilHeatingGasMultiStage_Impl(const CoilHeatingGasMultiStage_Impl& other,
                                  Model_Impl* model,
                                  bool keepHandle);

    virtual ~CoilHeatingGasMultiStage_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    //@}
    /** @name Getters */
    //@{

    boost::optional<Schedule> availabilitySchedule() const;

    boost::optional<Curve> partLoadFractionCorrelationCurve() const;

    boost::optional<double> parasiticGasLoad() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setPartLoadFractionCorrelationCurve(const boost::optional<Curve>& curve);

    void resetPartLoadFractionCorrelationCurve();

    void setParasiticGasLoad(boost::optional<double> parasiticGasLoad);

    void resetParasiticGasLoad();

    //@}
    /** @name Other */
    //@{

    unsigned inletPort();

    unsigned outletPort();

    ModelObject clone(Model model) const;

    std::vector<ModelObject> children() const;

    std::vector<CoilHeatingGasMultiStageStageData> stages() const;

    void addStage(CoilHeatingGasMultiStageStageData& stage);

    boost::optional<HVACComponent> containingHVACComponent() const;

    bool addToNode(Node & node);

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.CoilHeatingGasMultiStage");

  };

} // detail

} // model
} // openstudio

#endif // MODEL_COILHEATINGGASMULTISTAGE_IMPL_HPP

