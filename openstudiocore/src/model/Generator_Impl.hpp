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

#ifndef MODEL_GENERATOR_IMPL_HPP
#define MODEL_GENERATOR_IMPL_HPP

#include "ParentObject_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class MODEL_API Generator_Impl : public ParentObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    Generator_Impl(IddObjectType type, Model_Impl* model);

    Generator_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    Generator_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                   Model_Impl* model,
                   bool keepHandle);

    Generator_Impl(const Generator_Impl& other, Model_Impl* model, bool keepHandles);

    virtual ~Generator_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    /// remove this object from any ElectricLoadCenterDistribution objects then remove from file
    virtual std::vector<openstudio::IdfObject> remove() override;
    
    virtual std::string generatorObjectType() const = 0;

    virtual boost::optional<double> ratedElectricPowerOutput() const = 0;

    virtual boost::optional<Schedule> availabilitySchedule() const = 0;

    virtual boost::optional<double> ratedThermalToElectricalPowerRatio() const = 0;

    //@}
    /** @name Getters */
    //@{

    boost::optional<ElectricLoadCenterDistribution> electricLoadCenterDistribution() const;

    //@}
    /** @name Setters */
    //@{


    //@}
    /** @name Other */
    //@{
 
    //@}
   private:

    REGISTER_LOGGER("openstudio.model.Generator");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_GENERATOR_IMPL_HPP
