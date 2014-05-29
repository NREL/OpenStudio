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

#ifndef MODEL_SITEWATERMAINSTEMPERATURE_IMPL_HPP
#define MODEL_SITEWATERMAINSTEMPERATURE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"

namespace openstudio {
namespace model {

class Schedule;

namespace detail {

  /** SiteWaterMainsTemperature_Impl is a ModelObject_Impl that is the implementation class for SiteWaterMainsTemperature.*/
  class MODEL_API SiteWaterMainsTemperature_Impl : public ModelObject_Impl {
    Q_OBJECT;
    Q_PROPERTY(std::string calculationMethod READ calculationMethod WRITE setCalculationMethod);
    Q_PROPERTY(std::vector<std::string> calculationMethodValues READ calculationMethodValues);

    Q_PROPERTY(boost::optional<double> annualAverageOutdoorAirTemperature READ annualAverageOutdoorAirTemperature WRITE setAnnualAverageOutdoorAirTemperature RESET resetAnnualAverageOutdoorAirTemperature);
    Q_PROPERTY(openstudio::OSOptionalQuantity annualAverageOutdoorAirTemperature_SI READ annualAverageOutdoorAirTemperature_SI WRITE setAnnualAverageOutdoorAirTemperature RESET resetAnnualAverageOutdoorAirTemperature);
    Q_PROPERTY(openstudio::OSOptionalQuantity annualAverageOutdoorAirTemperature_IP READ annualAverageOutdoorAirTemperature_IP WRITE setAnnualAverageOutdoorAirTemperature RESET resetAnnualAverageOutdoorAirTemperature);

    Q_PROPERTY(boost::optional<double> maximumDifferenceInMonthlyAverageOutdoorAirTemperatures READ maximumDifferenceInMonthlyAverageOutdoorAirTemperatures WRITE setMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures RESET resetMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures);
    Q_PROPERTY(openstudio::OSOptionalQuantity maximumDifferenceInMonthlyAverageOutdoorAirTemperatures_SI READ maximumDifferenceInMonthlyAverageOutdoorAirTemperatures_SI WRITE setMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures RESET resetMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures);
    Q_PROPERTY(openstudio::OSOptionalQuantity maximumDifferenceInMonthlyAverageOutdoorAirTemperatures_IP READ maximumDifferenceInMonthlyAverageOutdoorAirTemperatures_IP WRITE setMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures RESET resetMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures);

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> temperatureSchedule READ temperatureScheduleAsModelObject WRITE setTemperatureScheduleAsModelObject RESET resetTemperatureSchedule);
   public:
    /** @name Constructors and Destructors */
    //@{

    SiteWaterMainsTemperature_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    SiteWaterMainsTemperature_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                   Model_Impl* model,
                                   bool keepHandle);

    SiteWaterMainsTemperature_Impl(const SiteWaterMainsTemperature_Impl& other,
                                   Model_Impl* model,
                                   bool keepHandle);

    virtual ~SiteWaterMainsTemperature_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual boost::optional<ParentObject> parent() const;

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    //@}
    /** @name Getters */
    //@{

    std::string calculationMethod() const;

    boost::optional<Schedule> temperatureSchedule() const;

    boost::optional<double> annualAverageOutdoorAirTemperature() const;

    OSOptionalQuantity getAnnualAverageOutdoorAirTemperature(bool returnIP=false) const;

    boost::optional<double> maximumDifferenceInMonthlyAverageOutdoorAirTemperatures() const;

    OSOptionalQuantity getMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures(bool returnIP=false) const;

    //@}
    /** @name Setters */
    //@{

    bool setCalculationMethod(std::string calculationMethod);

    bool setTemperatureSchedule(Schedule& schedule);

    void resetTemperatureSchedule();

    void setAnnualAverageOutdoorAirTemperature(boost::optional<double> annualAverageOutdoorAirTemperature);

    bool setAnnualAverageOutdoorAirTemperature(const OSOptionalQuantity& annualAverageOutdoorAirTemperature);

    void resetAnnualAverageOutdoorAirTemperature();

    bool setMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures(boost::optional<double> maximumDifferenceInMonthlyAverageOutdoorAirTemperatures);

    bool setMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures(const OSOptionalQuantity& maximumDifferenceInMonthlyAverageOutdoorAirTemperatures);

    void resetMaximumDifferenceInMonthlyAverageOutdoorAirTemperatures();

    //@}
   protected:
    virtual void populateValidityReport(ValidityReport& report,bool checkNames) const;
   private:
    REGISTER_LOGGER("openstudio.model.SiteWaterMainsTemperature");

    std::vector<std::string> calculationMethodValues() const;
    openstudio::OSOptionalQuantity annualAverageOutdoorAirTemperature_SI() const;
    openstudio::OSOptionalQuantity annualAverageOutdoorAirTemperature_IP() const;
    openstudio::OSOptionalQuantity maximumDifferenceInMonthlyAverageOutdoorAirTemperatures_SI() const;
    openstudio::OSOptionalQuantity maximumDifferenceInMonthlyAverageOutdoorAirTemperatures_IP() const;

    boost::optional<ModelObject> temperatureScheduleAsModelObject() const;

    bool setTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& temperatureSchedule);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_SITEWATERMAINSTEMPERATURE_IMPL_HPP

