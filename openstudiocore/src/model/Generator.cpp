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

#include "Generator.hpp"
#include "Generator_Impl.hpp"
#include "ParentObject.hpp"
#include "ParentObject_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "ElectricLoadCenterDistribution.hpp"
#include "ElectricLoadCenterDistribution_Impl.hpp"
#include "Schedule.hpp"
#include "ModelObjectList.hpp"
#include "ModelObjectList_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {

namespace model {

namespace detail {

Generator_Impl::Generator_Impl(IddObjectType type, Model_Impl* model)
  : ParentObject_Impl(type, model)
{
}

Generator_Impl::Generator_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
  : ParentObject_Impl(idfObject, model, keepHandle)
{ 
}

Generator_Impl::Generator_Impl(
    const openstudio::detail::WorkspaceObject_Impl& other, 
    Model_Impl* model, 
    bool keepHandle)
  : ParentObject_Impl(other, model, keepHandle)
{
}

Generator_Impl::Generator_Impl(const Generator_Impl& other, 
                               Model_Impl* model, 
                               bool keepHandles)
  : ParentObject_Impl(other, model, keepHandles)
{
}

std::vector<openstudio::IdfObject> Generator_Impl::remove()
{
  boost::optional<ElectricLoadCenterDistribution> elcd = this->electricLoadCenterDistribution();
  if (elcd){
    elcd->removeGenerator(getObject<Generator>());
  }
  return ParentObject_Impl::remove();
}

boost::optional<ElectricLoadCenterDistribution> Generator_Impl::electricLoadCenterDistribution() const
{
  boost::optional<ElectricLoadCenterDistribution> result;
  for (auto list : getObject<ModelObject>().getModelObjectSources<ModelObjectList>(ModelObjectList::iddObjectType())){
    auto elcds = list.getModelObjectSources<ElectricLoadCenterDistribution>(ElectricLoadCenterDistribution::iddObjectType());
    if (elcds.empty()){
      // error
    } else if (elcds.size() == 1u){
      return elcds[0];
    }else{
      // error
    }
  }
  return boost::none;
}

} // detail

Generator::Generator(IddObjectType type,const Model& model)
  : ParentObject(type,model)
{
  OS_ASSERT(getImpl<detail::Generator_Impl>());
}     

Generator::Generator(std::shared_ptr<detail::Generator_Impl> p)
  : ParentObject(p)
{}

std::string Generator::generatorObjectType() const
{
  return getImpl<detail::Generator_Impl>()->generatorObjectType();
}

boost::optional<double> Generator::ratedElectricPowerOutput() const
{
  return getImpl<detail::Generator_Impl>()->ratedElectricPowerOutput();
}

boost::optional<Schedule> Generator::availabilitySchedule() const
{
  return getImpl<detail::Generator_Impl>()->availabilitySchedule();
}

boost::optional<double> Generator::ratedThermalToElectricalPowerRatio() const
{
  return getImpl<detail::Generator_Impl>()->ratedThermalToElectricalPowerRatio();
}

boost::optional<ElectricLoadCenterDistribution> Generator::electricLoadCenterDistribution() const
{
  return getImpl<detail::Generator_Impl>()->electricLoadCenterDistribution();
}

} // model

} // openstudio

