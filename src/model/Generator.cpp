/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

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

    Generator_Impl::Generator_Impl(IddObjectType type, Model_Impl* model) : ParentObject_Impl(type, model) {}

    Generator_Impl::Generator_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ParentObject_Impl(idfObject, model, keepHandle) {}

    Generator_Impl::Generator_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ParentObject_Impl(other, model, keepHandle) {}

    Generator_Impl::Generator_Impl(const Generator_Impl& other, Model_Impl* model, bool keepHandles) : ParentObject_Impl(other, model, keepHandles) {}

    std::vector<openstudio::IdfObject> Generator_Impl::remove() {
      boost::optional<ElectricLoadCenterDistribution> elcd = this->electricLoadCenterDistribution();
      if (elcd) {
        elcd->removeGenerator(getObject<Generator>());
      }
      return ParentObject_Impl::remove();
    }

    boost::optional<ElectricLoadCenterDistribution> Generator_Impl::electricLoadCenterDistribution() const {
      boost::optional<ElectricLoadCenterDistribution> result;
      for (const auto& list : getObject<ModelObject>().getModelObjectSources<ModelObjectList>(ModelObjectList::iddObjectType())) {
        auto elcds = list.getModelObjectSources<ElectricLoadCenterDistribution>(ElectricLoadCenterDistribution::iddObjectType());
        if (elcds.empty()) {
          // error
        } else if (elcds.size() == 1u) {
          return elcds[0];
        } else {
          // error
        }
      }
      return boost::none;
    }
    /*
std::vector<EMSActuatorNames> Generator_Impl::emsActuatorNames() const {
  std::vector<EMSActuatorNames> actuators{{"On-Site Generator Control", "Requested Power"}};
  return actuators;
}

std::vector<std::string> Generator_Impl::emsInternalVariableNames() const {
  std::vector<std::string> types{"Generator Nominal Maximum Power",
                                 "Generator Nominal Thermal To Electric Ratio"};
  return types;
} */
  }  // namespace detail

  Generator::Generator(IddObjectType type, const Model& model) : ParentObject(type, model) {
    OS_ASSERT(getImpl<detail::Generator_Impl>());
  }

  Generator::Generator(std::shared_ptr<detail::Generator_Impl> p) : ParentObject(std::move(p)) {}

  std::string Generator::generatorObjectType() const {
    return getImpl<detail::Generator_Impl>()->generatorObjectType();
  }

  boost::optional<double> Generator::ratedElectricPowerOutput() const {
    return getImpl<detail::Generator_Impl>()->ratedElectricPowerOutput();
  }

  boost::optional<Schedule> Generator::availabilitySchedule() const {
    return getImpl<detail::Generator_Impl>()->availabilitySchedule();
  }

  boost::optional<double> Generator::ratedThermaltoElectricalPowerRatio() const {
    return getImpl<detail::Generator_Impl>()->ratedThermaltoElectricalPowerRatio();
  }

  boost::optional<ElectricLoadCenterDistribution> Generator::electricLoadCenterDistribution() const {
    return getImpl<detail::Generator_Impl>()->electricLoadCenterDistribution();
  }

}  // namespace model

}  // namespace openstudio
