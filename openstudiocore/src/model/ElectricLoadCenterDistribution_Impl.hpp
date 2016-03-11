/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#ifndef MODEL_ELECTRICLOADCENTERDISTRIBUTION_IMPL_HPP
#define MODEL_ELECTRICLOADCENTERDISTRIBUTION_IMPL_HPP

#include "ModelAPI.hpp"
#include "ParentObject_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;
class Generator;
class Inverter;
//class ElectricalStorage;
//class Transformer;
class ModelObjectList;

namespace detail {

  /** ElectricLoadCenterDistribution_Impl is a ParentObject_Impl that is the implementation class for ElectricLoadCenterDistribution.*/
  class MODEL_API ElectricLoadCenterDistribution_Impl : public ParentObject_Impl {

   public:
    /** @name Constructors and Destructors */
    //@{

    ElectricLoadCenterDistribution_Impl(const IdfObject& idfObject,
                                        Model_Impl* model,
                                        bool keepHandle);

    ElectricLoadCenterDistribution_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                        Model_Impl* model,
                                        bool keepHandle);

    ElectricLoadCenterDistribution_Impl(const ElectricLoadCenterDistribution_Impl& other,
                                        Model_Impl* model,
                                        bool keepHandle);

    virtual ~ElectricLoadCenterDistribution_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    /// returns all generators, inverters, transformers, and electrical storage
    virtual std::vector<ModelObject> children() const override;

    /// remove the object from the model's workspace
    virtual std::vector<openstudio::IdfObject> remove() override;

    /// get a vector of allowable children types
    virtual std::vector<IddObjectType> allowableChildTypes() const override;

    /// returns all generators, inverters, transformers, and electrical storage
    virtual ModelObject clone(Model model) const override;

    //@}
    /** @name Getters */
    //@{

    std::vector<Generator> generators() const;

    std::string generatorOperationSchemeType() const;

    bool isGeneratorOperationSchemeTypeDefaulted() const;

    //boost::optional<double> demandLimitSchemePurchasedElectricDemandLimit() const;

    //boost::optional<Schedule> trackScheduleSchemeSchedule() const;

    //boost::optional<std::string> trackMeterSchemeMeterName() const;

    std::string electricalBussType() const;

    bool isElectricalBussTypeDefaulted() const;

    boost::optional<Inverter> inverter() const;

    //boost::optional<ElectricalStorage> electricalStorage() const;

    //boost::optional<Transformer> transformer() const;

    //@}
    /** @name Setters */
    //@{

    bool addGenerator(const Generator& generator);

    bool removeGenerator(const Generator& generator);

    void resetGenerators();

    bool setGeneratorOperationSchemeType(const std::string& generatorOperationSchemeType);

    void resetGeneratorOperationSchemeType();

    //void setDemandLimitSchemePurchasedElectricDemandLimit(double demandLimitSchemePurchasedElectricDemandLimit);

    //void resetDemandLimitSchemePurchasedElectricDemandLimit();

    //bool setTrackScheduleSchemeSchedule(Schedule& schedule);

    //void resetTrackScheduleSchemeSchedule();

    //bool setTrackMeterSchemeMeterName(const std::string& trackMeterSchemeMeterName);

    //void resetTrackMeterSchemeMeterName();

    bool setElectricalBussType(const std::string& electricalBussType);

    void resetElectricalBussType();

    bool setInverter(const Inverter& inverter);

    void resetInverter();

    //bool setElectricalStorage(const ElectricalStorage& electricalStorage);

    //void resetElectricalStorage();

    //bool setTransformer(const Transformer& transformer);

    //void resetTransformerObject();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.ElectricLoadCenterDistribution");

    ModelObjectList generatorModelObjectList() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_ELECTRICLOADCENTERDISTRIBUTION_IMPL_HPP

