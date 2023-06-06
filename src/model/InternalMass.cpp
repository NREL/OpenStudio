/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "InternalMass.hpp"
#include "InternalMass_Impl.hpp"

#include "InternalMassDefinition.hpp"
#include "InternalMassDefinition_Impl.hpp"
#include "Space.hpp"
#include "Space_Impl.hpp"
#include "LifeCycleCost.hpp"
#include "SurfacePropertyConvectionCoefficients.hpp"
#include "SurfacePropertyConvectionCoefficients_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/OS_InternalMass_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    InternalMass_Impl::InternalMass_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : SpaceLoadInstance_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == InternalMass::iddObjectType());
    }

    InternalMass_Impl::InternalMass_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : SpaceLoadInstance_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == InternalMass::iddObjectType());
    }

    InternalMass_Impl::InternalMass_Impl(const InternalMass_Impl& other, Model_Impl* model, bool keepHandle)
      : SpaceLoadInstance_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& InternalMass_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      // Not sure
      return result;
    }

    IddObjectType InternalMass_Impl::iddObjectType() const {
      return InternalMass::iddObjectType();
    }

    bool InternalMass_Impl::hardSize() {
      boost::optional<Space> space = this->space();
      if (!space) {
        return false;
      }

      this->makeUnique();

      InternalMassDefinition internalMassDefinition = this->internalMassDefinition();
      for (LifeCycleCost& cost : internalMassDefinition.lifeCycleCosts()) {
        cost.convertToCostPerEach();
      }

      boost::optional<double> surfaceArea = internalMassDefinition.surfaceArea();
      if (surfaceArea) {
        return true;
      }

      boost::optional<double> surfaceAreaperSpaceFloorArea = internalMassDefinition.surfaceAreaperSpaceFloorArea();
      if (surfaceAreaperSpaceFloorArea) {
        return internalMassDefinition.setSurfaceArea(*surfaceAreaperSpaceFloorArea * space->floorArea());
      }

      boost::optional<double> surfaceAreaperPerson = internalMassDefinition.surfaceAreaperPerson();
      if (surfaceAreaperPerson) {
        return internalMassDefinition.setSurfaceArea(*surfaceAreaperPerson * space->numberOfPeople());
      }

      return false;
    }

    bool InternalMass_Impl::hardApplySchedules() {
      return true;
    }

    double InternalMass_Impl::multiplier() const {
      boost::optional<double> value = getDouble(OS_InternalMassFields::Multiplier, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool InternalMass_Impl::isMultiplierDefaulted() const {
      return isEmpty(OS_InternalMassFields::Multiplier);
    }

    bool InternalMass_Impl::isAbsolute() const {
      InternalMassDefinition definition = internalMassDefinition();
      if (definition.surfaceArea()) {
        return true;
      }
      return false;
    }

    bool InternalMass_Impl::setMultiplier(double multiplier) {
      bool result = setDouble(OS_InternalMassFields::Multiplier, multiplier);
      return result;
    }

    void InternalMass_Impl::resetMultiplier() {
      bool result = setString(OS_InternalMassFields::Multiplier, "");
      OS_ASSERT(result);
    }

    int InternalMass_Impl::spaceIndex() const {
      return OS_InternalMassFields::SpaceorSpaceTypeName;
    }

    int InternalMass_Impl::definitionIndex() const {
      return OS_InternalMassFields::InternalMassDefinitionName;
    }

    InternalMassDefinition InternalMass_Impl::internalMassDefinition() const {
      return this->definition().cast<InternalMassDefinition>();
    }

    bool InternalMass_Impl::setInternalMassDefinition(const InternalMassDefinition& definition) {
      return this->setPointer(this->definitionIndex(), definition.handle());
    }

    bool InternalMass_Impl::setDefinition(const SpaceLoadDefinition& definition) {
      bool result = false;
      boost::optional<InternalMassDefinition> internalMassDefinition = definition.optionalCast<InternalMassDefinition>();
      if (internalMassDefinition) {
        result = setInternalMassDefinition(*internalMassDefinition);
      }
      return result;
    }

    boost::optional<double> InternalMass_Impl::surfaceArea() const {
      OptionalDouble result = internalMassDefinition().surfaceArea();
      if (result) {
        return result.get() * multiplier();
      }
      return result;
    }

    boost::optional<double> InternalMass_Impl::surfaceAreaPerFloorArea() const {
      OptionalDouble result = internalMassDefinition().surfaceAreaperSpaceFloorArea();
      if (result) {
        return result.get() * multiplier();
      }
      return result;
    }

    boost::optional<double> InternalMass_Impl::surfaceAreaPerPerson() const {
      OptionalDouble result = internalMassDefinition().surfaceAreaperPerson();
      if (result) {
        return result.get() * multiplier();
      }
      return result;
    }

    double InternalMass_Impl::getSurfaceArea(double floorArea, double numPeople) const {
      return internalMassDefinition().getSurfaceArea(floorArea, numPeople) * multiplier();
    }

    double InternalMass_Impl::getSurfaceAreaPerFloorArea(double floorArea, double numPeople) const {
      return internalMassDefinition().getSurfaceAreaPerFloorArea(floorArea, numPeople) * multiplier();
    }

    double InternalMass_Impl::getSurfaceAreaPerPerson(double floorArea, double numPeople) const {
      return internalMassDefinition().getSurfaceAreaPerPerson(floorArea, numPeople) * multiplier();
    }

    boost::optional<ModelObject> InternalMass_Impl::internalMassDefinitionAsModelObject() const {
      OptionalModelObject result = internalMassDefinition();
      return result;
    }

    boost::optional<SurfacePropertyConvectionCoefficients> InternalMass_Impl::surfacePropertyConvectionCoefficients() const {
      std::vector<SurfacePropertyConvectionCoefficients> allspccs(model().getConcreteModelObjects<SurfacePropertyConvectionCoefficients>());
      std::vector<SurfacePropertyConvectionCoefficients> spccs;
      for (auto& spcc : allspccs) {
        OptionalInternalMass surface = spcc.surfaceAsInternalMass();
        if (surface) {
          if (surface->handle() == handle()) {
            spccs.push_back(spcc);
          }
        }
      }
      if (spccs.empty()) {
        return boost::none;
      } else if (spccs.size() == 1) {
        return spccs.at(0);
      } else {
        LOG(Error, "More than one SurfacePropertyConvectionCoefficients points to this InternalMass");
        return boost::none;
      }
    }

    bool InternalMass_Impl::setInternalMassDefinitionAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalInternalMassDefinition intermediate = modelObject->optionalCast<InternalMassDefinition>();
        if (intermediate) {
          return setInternalMassDefinition(*intermediate);
        }
      }
      return false;
    }

    ModelObject InternalMass_Impl::clone(Model model) const {
      auto clone = ModelObject_Impl::clone(model).cast<InternalMass>();

      auto coefficients = surfacePropertyConvectionCoefficients();
      if (coefficients) {
        auto coefficientsClone = coefficients->clone(model).cast<SurfacePropertyConvectionCoefficients>();
        coefficientsClone.setSurface(clone);
      }

      return std::move(clone);
    }

  }  // namespace detail

  InternalMass::InternalMass(const InternalMassDefinition& internalMassDefinition)
    : SpaceLoadInstance(InternalMass::iddObjectType(), internalMassDefinition) {
    OS_ASSERT(getImpl<detail::InternalMass_Impl>());

    bool test = this->setMultiplier(1.0);
    OS_ASSERT(test);
  }

  IddObjectType InternalMass::iddObjectType() {
    IddObjectType result(IddObjectType::OS_InternalMass);
    return result;
  }

  bool InternalMass::setMultiplier(double multiplier) {
    return getImpl<detail::InternalMass_Impl>()->setMultiplier(multiplier);
  }

  void InternalMass::resetMultiplier() {
    getImpl<detail::InternalMass_Impl>()->resetMultiplier();
  }

  InternalMassDefinition InternalMass::internalMassDefinition() const {
    return getImpl<detail::InternalMass_Impl>()->internalMassDefinition();
  }

  bool InternalMass::setInternalMassDefinition(const InternalMassDefinition& internalMassDefinition) {
    return getImpl<detail::InternalMass_Impl>()->setInternalMassDefinition(internalMassDefinition);
  }

  boost::optional<double> InternalMass::surfaceArea() const {
    return getImpl<detail::InternalMass_Impl>()->surfaceArea();
  }

  boost::optional<double> InternalMass::surfaceAreaPerFloorArea() const {
    return getImpl<detail::InternalMass_Impl>()->surfaceAreaPerFloorArea();
  }

  boost::optional<double> InternalMass::surfaceAreaPerPerson() const {
    return getImpl<detail::InternalMass_Impl>()->surfaceAreaPerPerson();
  }

  double InternalMass::getSurfaceArea(double floorArea, double numPeople) const {
    return getImpl<detail::InternalMass_Impl>()->getSurfaceArea(floorArea, numPeople);
  }

  double InternalMass::getSurfaceAreaPerFloorArea(double floorArea, double numPeople) const {
    return getImpl<detail::InternalMass_Impl>()->getSurfaceAreaPerFloorArea(floorArea, numPeople);
  }

  double InternalMass::getSurfaceAreaPerPerson(double floorArea, double numPeople) const {
    return getImpl<detail::InternalMass_Impl>()->getSurfaceAreaPerPerson(floorArea, numPeople);
  }

  boost::optional<SurfacePropertyConvectionCoefficients> InternalMass::surfacePropertyConvectionCoefficients() const {
    return getImpl<detail::InternalMass_Impl>()->surfacePropertyConvectionCoefficients();
  }

  /// @cond
  InternalMass::InternalMass(std::shared_ptr<detail::InternalMass_Impl> impl) : SpaceLoadInstance(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
