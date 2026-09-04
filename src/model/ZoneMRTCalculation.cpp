/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ZoneMRTCalculation.hpp"
#include "ZoneMRTCalculation_Impl.hpp"
#include "People.hpp"
#include "People_Impl.hpp"
#include "PeopleDefinition.hpp"
#include "PeopleDefinition_Impl.hpp"
#include "Space.hpp"
#include "Space_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"

#include "ModelExtensibleGroup.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_ZoneMRTCalculation_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

#include <algorithm>

namespace openstudio {
namespace model {

  /*****************************************************************************************************************************************************
*                                                M R T   W E I G H T I N G   F A C T O R                                                            *
*****************************************************************************************************************************************************/
  MRTWeightingFactor::MRTWeightingFactor(const People& people, double mrtWeightingFactor)
    : m_people(people), m_mrtWeightingFactor(mrtWeightingFactor) {
    if (!((m_mrtWeightingFactor >= 0.0) && (m_mrtWeightingFactor <= 1.0))) {
      LOG_AND_THROW("Unable to create MRT weighting factor, factor of " << m_mrtWeightingFactor << " is not between 0 and 1");
    }
  }

  People MRTWeightingFactor::people() const {
    return m_people;
  }

  double MRTWeightingFactor::mrtWeightingFactor() const {
    return m_mrtWeightingFactor;
  }

  std::ostream& operator<<(std::ostream& out, const openstudio::model::MRTWeightingFactor& mrtWeightingFactor) {
    out << "(people='" << mrtWeightingFactor.people().nameString()
        << "', MRT weighting factor=" << mrtWeightingFactor.mrtWeightingFactor() << ")";
    return out;
  }

  /*****************************************************************************************************************************************************
*                                                        Z O N E   M R T   C A L C U L A T I O N                                                      *
*****************************************************************************************************************************************************/

  namespace detail {

    ZoneMRTCalculation_Impl::ZoneMRTCalculation_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ZoneMRTCalculation::iddObjectType());
    }

    ZoneMRTCalculation_Impl::ZoneMRTCalculation_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ZoneMRTCalculation::iddObjectType());
    }

    ZoneMRTCalculation_Impl::ZoneMRTCalculation_Impl(const ZoneMRTCalculation_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& ZoneMRTCalculation_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType ZoneMRTCalculation_Impl::iddObjectType() const {
      return ZoneMRTCalculation::iddObjectType();
    }

    ModelObject ZoneMRTCalculation_Impl::clone(Model /*model*/) const {
      LOG_AND_THROW("Cloning isn't allowed for ZoneMRTCalculation in order to guarantee that every ZoneMRTCalculation has a thermal zone, and "
                    "that a thermal zone must have only one ZoneMRTCalculation.");
    }

    ThermalZone ZoneMRTCalculation_Impl::thermalZone() const {
      boost::optional<ThermalZone> thermalZone =
        getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_ZoneMRTCalculationFields::ThermalZoneName);
      OS_ASSERT(thermalZone);
      return thermalZone.get();
    }

    unsigned int ZoneMRTCalculation_Impl::numberofMRTWeightingFactors() const {
      return numExtensibleGroups();
    }

    boost::optional<unsigned> ZoneMRTCalculation_Impl::mrtWeightingFactorIndex(const People& people) const {
      boost::optional<unsigned> result;

      auto egs = castVector<WorkspaceExtensibleGroup>(extensibleGroups());
      auto peopleHandle = toString(people.handle());
      auto it = std::find_if(egs.begin(), egs.end(), [&](const WorkspaceExtensibleGroup& eg) {
        return eg.getField(OS_ZoneMRTCalculationExtensibleFields::PeopleName).get() == peopleHandle;
      });

      if (it != egs.end()) {
        result = std::distance(egs.begin(), it);
      }

      return result;
    }

    boost::optional<People> ZoneMRTCalculation_Impl::getPeople(unsigned groupIndex) const {
      boost::optional<People> result;

      if (groupIndex >= numberofMRTWeightingFactors()) {
        LOG(Error, "Asked to get People with index " << groupIndex << ", but " << briefDescription() << " has just " << numberofMRTWeightingFactors()
                                                     << " MRT weighting factors.");
        return result;
      }

      auto group = getExtensibleGroup(groupIndex).cast<ModelExtensibleGroup>();
      result = group.getModelObjectTarget<People>(OS_ZoneMRTCalculationExtensibleFields::PeopleName);

      if (!result) {
        LOG(Error, "Could not retrieve People Name for extensible group " << group.groupIndex() << ".");
      }

      return result;
    }

    boost::optional<double> ZoneMRTCalculation_Impl::getMRTWeightingFactorValue(unsigned groupIndex) const {
      boost::optional<double> result;

      if (groupIndex >= numberofMRTWeightingFactors()) {
        LOG(Error, "Asked to get MRT Weighting Factor with index " << groupIndex << ", but " << briefDescription() << " has just "
                                                                   << numberofMRTWeightingFactors() << " MRT weighting factors.");
        return result;
      }

      auto group = getExtensibleGroup(groupIndex).cast<ModelExtensibleGroup>();
      result = group.getDouble(OS_ZoneMRTCalculationExtensibleFields::MRTWeightingFactor);

      if (!result) {
        LOG(Error, "Could not retrieve MRT Weighting Factor for extensible group " << group.groupIndex() << ".");
      }

      return result;
    }

    boost::optional<MRTWeightingFactor> ZoneMRTCalculation_Impl::getMRTWeightingFactor(unsigned groupIndex) const {
      boost::optional<MRTWeightingFactor> result;

      boost::optional<People> people = getPeople(groupIndex);
      boost::optional<double> value = getMRTWeightingFactorValue(groupIndex);

      if (people && value) {
        result = MRTWeightingFactor(people.get(), value.get());
      }

      return result;
    }

    std::vector<MRTWeightingFactor> ZoneMRTCalculation_Impl::mrtWeightingFactors() const {
      std::vector<MRTWeightingFactor> result;

      for (unsigned i = 0; i < numberofMRTWeightingFactors(); ++i) {
        boost::optional<MRTWeightingFactor> mrtWeightingFactor = getMRTWeightingFactor(i);
        if (mrtWeightingFactor) {
          result.push_back(mrtWeightingFactor.get());
        } else {
          LOG(Error, briefDescription() << " has an invalid MRTWeightingFactor group at index " << i);
        }
      }

      return result;
    }

    bool ZoneMRTCalculation_Impl::addMRTWeightingFactor(const MRTWeightingFactor& mrtWeightingFactor) {
      bool result = false;

      People people = mrtWeightingFactor.people();

      if (people.model() != model()) {
        LOG(Error, "Cannot add " << people.briefDescription() << " to " << briefDescription() << " because they are in different models.");
        return result;
      }

      boost::optional<Space> space = people.space();
      if (!space) {
        LOG(Error, "Cannot add " << people.briefDescription() << " to " << briefDescription() << " because it is not directly assigned to a Space.");
        return result;
      }

      boost::optional<ThermalZone> thermalZone = space->thermalZone();
      if (!thermalZone) {
        LOG(Error, "Cannot add " << people.briefDescription() << " to " << briefDescription() << " because it is not assigned to any ThermalZone.");
      } else if (thermalZone->handle() != this->thermalZone().handle()) {
        LOG(Error, "Cannot add " << people.briefDescription() << " to " << briefDescription() << " because it is assigned to ThermalZone '" << thermalZone->nameString() << "' instead of '" << this->thermalZone().nameString() << "'.");
        return result;
      }

      if (people.peopleDefinition().numThermalComfortModelTypes() == 0) {
        LOG(Error, "Cannot add " << people.briefDescription() << " to " << briefDescription()
                                 << " because it does not select at least one Thermal Comfort Model Type.");
        return result;
      }

      boost::optional<unsigned> existingIndex = mrtWeightingFactorIndex(people);
      if (existingIndex) {
        boost::optional<MRTWeightingFactor> existingMRTWeightingFactor = getMRTWeightingFactor(existingIndex.get());
        OS_ASSERT(existingMRTWeightingFactor);
        LOG(Warn, "For " << briefDescription() << ", MRTWeightingFactor already exists, will be modified in place from "
                         << existingMRTWeightingFactor.get() << " to " << mrtWeightingFactor << ".");
      }

      double sum = mrtWeightingFactor.mrtWeightingFactor();
      for (unsigned i = 0; i < numberofMRTWeightingFactors(); ++i) {
        if (existingIndex && (i == existingIndex.get())) {
          continue;
        }

        boost::optional<double> existingValue = getMRTWeightingFactorValue(i);
        if (!existingValue || !((*existingValue >= 0.0) && (*existingValue <= 1.0))) {
          LOG(Error,
              "Cannot add " << people.briefDescription() << " to " << briefDescription() << " because an existing MRT Weighting Factor is invalid.");
          return result;
        }

        sum += existingValue.get();
      }

      if (sum > 1.0) {
        LOG(Error, "Cannot add " << people.briefDescription() << " to " << briefDescription() << " because the MRT Weighting Factors would sum to "
                                 << sum << ", which is greater than 1.");
        return result;
      }

      std::vector<std::string> temp;
      ModelExtensibleGroup group = (existingIndex ? getExtensibleGroup(existingIndex.get()).cast<ModelExtensibleGroup>()
                                                  : pushExtensibleGroup(temp, false).cast<ModelExtensibleGroup>());

      bool peopleSet = group.setPointer(OS_ZoneMRTCalculationExtensibleFields::PeopleName, people.handle(), false);
      if (!peopleSet) {
        LOG(Error, "Unable to add MRT Weighting Factor which has an incompatible People object to " << briefDescription());
        OS_ASSERT(false);
      }

      bool factorSet = group.setDouble(OS_ZoneMRTCalculationExtensibleFields::MRTWeightingFactor, mrtWeightingFactor.mrtWeightingFactor());

      if (peopleSet && factorSet) {
        result = true;
      } else {
        if (!existingIndex) {
          getObject<ModelObject>().eraseExtensibleGroup(group.groupIndex());
        }
      }

      return result;
    }

    bool ZoneMRTCalculation_Impl::addMRTWeightingFactor(const People& people, double mrtWeightingFactor) {
      MRTWeightingFactor weightingFactor(people, mrtWeightingFactor);
      return addMRTWeightingFactor(weightingFactor);
    }

    bool ZoneMRTCalculation_Impl::addMRTWeightingFactors(const std::vector<MRTWeightingFactor>& mrtWeightingFactors) {
      bool result = true;

      for (const MRTWeightingFactor& mrtWeightingFactor : mrtWeightingFactors) {
        bool thisResult = addMRTWeightingFactor(mrtWeightingFactor);
        if (!thisResult) {
          result = false;
          LOG(Error, "Could not add mrtWeightingFactor " << mrtWeightingFactor << " to " << briefDescription() << ". Continuing with others.");
        }
      }

      return result;
    }

    bool ZoneMRTCalculation_Impl::removeMRTWeightingFactor(unsigned groupIndex) {
      bool result = false;

      if (groupIndex < numberofMRTWeightingFactors()) {
        getObject<ModelObject>().eraseExtensibleGroup(groupIndex);
        result = true;
      }

      return result;
    }

    void ZoneMRTCalculation_Impl::removeAllMRTWeightingFactors() {
      getObject<ModelObject>().clearExtensibleGroups();
    }

  }  // namespace detail

  ZoneMRTCalculation::ZoneMRTCalculation(const ThermalZone& thermalZone) : ModelObject(ZoneMRTCalculation::iddObjectType(), thermalZone.model()) {
    std::vector<ZoneMRTCalculation> existingObjects = thermalZone.getModelObjectSources<ZoneMRTCalculation>(ZoneMRTCalculation::iddObjectType());
    if (!existingObjects.empty()) {
      remove();
      LOG_AND_THROW(thermalZone.briefDescription() << " already has a ZoneMRTCalculation, cannot create a new one.");
    }
    OS_ASSERT(getImpl<detail::ZoneMRTCalculation_Impl>());

    bool ok = setPointer(OS_ZoneMRTCalculationFields::ThermalZoneName, thermalZone.handle());
    OS_ASSERT(ok);
  }

  IddObjectType ZoneMRTCalculation::iddObjectType() {
    return {IddObjectType::OS_ZoneMRTCalculation};
  }

  ThermalZone ZoneMRTCalculation::thermalZone() const {
    return getImpl<detail::ZoneMRTCalculation_Impl>()->thermalZone();
  }

  std::vector<MRTWeightingFactor> ZoneMRTCalculation::mrtWeightingFactors() const {
    return getImpl<detail::ZoneMRTCalculation_Impl>()->mrtWeightingFactors();
  }

  unsigned int ZoneMRTCalculation::numberofMRTWeightingFactors() const {
    return getImpl<detail::ZoneMRTCalculation_Impl>()->numberofMRTWeightingFactors();
  }

  boost::optional<unsigned> ZoneMRTCalculation::mrtWeightingFactorIndex(const People& people) const {
    return getImpl<detail::ZoneMRTCalculation_Impl>()->mrtWeightingFactorIndex(people);
  }

  boost::optional<MRTWeightingFactor> ZoneMRTCalculation::getMRTWeightingFactor(unsigned groupIndex) const {
    return getImpl<detail::ZoneMRTCalculation_Impl>()->getMRTWeightingFactor(groupIndex);
  }

  bool ZoneMRTCalculation::addMRTWeightingFactor(const MRTWeightingFactor& mrtWeightingFactor) {
    return getImpl<detail::ZoneMRTCalculation_Impl>()->addMRTWeightingFactor(mrtWeightingFactor);
  }

  bool ZoneMRTCalculation::addMRTWeightingFactor(const People& people, double mrtWeightingFactor) {
    return getImpl<detail::ZoneMRTCalculation_Impl>()->addMRTWeightingFactor(people, mrtWeightingFactor);
  }

  bool ZoneMRTCalculation::addMRTWeightingFactors(const std::vector<MRTWeightingFactor>& mrtWeightingFactors) {
    return getImpl<detail::ZoneMRTCalculation_Impl>()->addMRTWeightingFactors(mrtWeightingFactors);
  }

  void ZoneMRTCalculation::removeMRTWeightingFactor(int groupIndex) {
    getImpl<detail::ZoneMRTCalculation_Impl>()->removeMRTWeightingFactor(groupIndex);
  }

  void ZoneMRTCalculation::removeAllMRTWeightingFactors() {
    getImpl<detail::ZoneMRTCalculation_Impl>()->removeAllMRTWeightingFactors();
  }

  /// @cond
  ZoneMRTCalculation::ZoneMRTCalculation(std::shared_ptr<detail::ZoneMRTCalculation_Impl> impl) : ModelObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
