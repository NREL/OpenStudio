/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class MODEL_API Generator_Impl : public ModelObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    Generator_Impl(IddObjectType type, Model_Impl* model);

    Generator_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    Generator_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                   Model_Impl* model,
                   bool keepHandle);

    Generator_Impl(const StraightComponent_Impl& other, Model_Impl* model, bool keepHandles);

    virtual ~Generator_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual ModelObject clone(Model model) const override;

    virtual boost::optional<ParentObject> parent() const override;

    virtual std::vector<ModelObject> children() const override;

    //@}
    /** @name Getters */
    //@{

    // boost::optional<double> ratedElectricPowerOutput() const;
    
    // boost::optional<Schedule> availabilitySchedule() const;

    // boost::optional<double> ratedThermalToElectricalPowerRatio() const;

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
