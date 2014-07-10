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

#ifndef MODEL_OUTPUTVARIABLE_IMPL_HPP
#define MODEL_OUTPUTVARIABLE_IMPL_HPP

#include "ModelObject_Impl.hpp"

namespace openstudio {

class ReportingFrequency;

namespace model {

class Schedule;

namespace detail {

  class MODEL_API OutputVariable_Impl : public ModelObject_Impl {
    Q_OBJECT;

    Q_PROPERTY(std::string keyValue READ keyValue WRITE setKeyValue RESET resetKeyValue);
    Q_PROPERTY(bool isKeyValueDefaulted READ isKeyValueDefaulted);

    Q_PROPERTY(std::string variableName READ variableName WRITE setVariableName);

    Q_PROPERTY(std::string reportingFrequency READ reportingFrequency WRITE setReportingFrequency RESET resetReportingFrequency);
    Q_PROPERTY(bool isReportingFrequencyDefaulted READ isReportingFrequencyDefaulted);
    Q_PROPERTY(std::vector<std::string> reportingFrequencyValues READ reportingFrequencyValues);

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> schedule READ scheduleAsModelObject WRITE setScheduleAsModelObject RESET resetSchedule);
   public:
    /** @name Constructors and Destructors */
    //@{

    // constructor
    OutputVariable_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    // construct from workspace
    OutputVariable_Impl(const openstudio::detail::WorkspaceObject_Impl& other, 
                        Model_Impl* model, 
                        bool keepHandle);

    // copy constructor
    OutputVariable_Impl(const OutputVariable_Impl& other, Model_Impl* model, bool keepHandle);

    // virtual destructor
    virtual ~OutputVariable_Impl(){}

    //@}
    /** @name Virtual Methods */
    //@{

    // Get all output variable names that could be associated with this object.
    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    //@}
    /** @name Getters */
    //@{

    std::string keyValue() const;

    bool isKeyValueDefaulted() const;

    std::string variableName() const;

    std::string reportingFrequency() const;

    bool isReportingFrequencyDefaulted() const;

    std::vector<std::string> reportingFrequencyValues() const;

    boost::optional<Schedule> schedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setKeyValue(const std::string& keyValue);

    void resetKeyValue();

    bool setVariableName(const std::string& variableName);

    bool setReportingFrequency(const std::string& reportingFrequency);

    void resetReportingFrequency();

    bool setSchedule(Schedule& schedule);

    void resetSchedule();

    //@}
   private:

    REGISTER_LOGGER("openstudio.model.OutputVariable");

    boost::optional<ModelObject> scheduleAsModelObject() const;

    bool setScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_OUTPUTVARIABLE_IMPL_HPP
