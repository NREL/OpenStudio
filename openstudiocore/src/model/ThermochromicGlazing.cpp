/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "ThermochromicGlazing.hpp"
#include "ThermochromicGlazing_Impl.hpp"
#include "ModelExtensibleGroup.hpp"

#include <utilities/idd/OS_WindowMaterial_GlazingGroup_Thermochromic_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"
#include "../utilities/math/FloatCompare.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ThermochromicGlazing_Impl::ThermochromicGlazing_Impl(const IdfObject& idfObject,
                                                       Model_Impl* model,
                                                       bool keepHandle)
    : Glazing_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ThermochromicGlazing::iddObjectType());
  }

  ThermochromicGlazing_Impl::ThermochromicGlazing_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                       Model_Impl* model,
                                                       bool keepHandle)
    : Glazing_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ThermochromicGlazing::iddObjectType());
  }

  ThermochromicGlazing_Impl::ThermochromicGlazing_Impl(const ThermochromicGlazing_Impl& other,
                                                       Model_Impl* model,
                                                       bool keepHandle)
    : Glazing_Impl(other,model,keepHandle)
  {}

  IddObjectType ThermochromicGlazing_Impl::iddObjectType() const {
    return ThermochromicGlazing::iddObjectType();
  }

  double ThermochromicGlazing_Impl::thickness() const {
    GlazingVector glazings = mf_glazings();
    double result = 0.0;         // running average
    unsigned n = 0;
    bool warned = false;
    for (const Glazing& glazing : glazings) {
      double glazingThickness = glazing.thickness();
      if (n == 0) {
        result = glazingThickness;
      }
      else {
        // keep running average
        if (!warned && ! equal(glazingThickness,result)) {
          LOG(Warn,"Thermochromic group '" << name().get() << "' contains glazings of different "
              << "thicknesses.");
          warned = true;
        }
        result = (result*static_cast<double>(n) + glazingThickness)/static_cast<double>(n + 1);
      }
      ++n;
    }
    return result;
  }

  double ThermochromicGlazing_Impl::thermalConductivity() const {
    LOG_AND_THROW("Thermal conductivity not yet supported for ThermochromicGlazings.");
  }

  double ThermochromicGlazing_Impl::thermalConductance() const {
    LOG_AND_THROW("Thermal conductance not yet supported for ThermochromicGlazings.");
  }

  double ThermochromicGlazing_Impl::thermalResistivity() const {
    LOG_AND_THROW("Thermal resistivity not yet supported for ThermochromicGlazings.");
  }

  double ThermochromicGlazing_Impl::thermalResistance() const {
    LOG_AND_THROW("Thermal resistance not yet supported for ThermochromicGlazings.");
  }

  double ThermochromicGlazing_Impl::thermalTransmittance() const {
    LOG_AND_THROW("Thermal transmittance not yet supported for ThermochromicGlazings.");
  }

  double ThermochromicGlazing_Impl::thermalAbsorptance() const {
    LOG_AND_THROW("Thermal absorptance not yet supported for ThermochromicGlazings.");
  }

  double ThermochromicGlazing_Impl::thermalReflectance() const {
    LOG_AND_THROW("Thermal reflectance not yet supported for ThermochromicGlazings.");
  }

  double ThermochromicGlazing_Impl::solarTransmittance() const {
    LOG_AND_THROW("Solar transmittance not yet supported for ThermochromicGlazings.");
  }

  double ThermochromicGlazing_Impl::solarAbsorptance() const {
    LOG_AND_THROW("Solar absorptance not yet supported for ThermochromicGlazings.");
  }

  double ThermochromicGlazing_Impl::solarReflectance() const {
    LOG_AND_THROW("Solar reflectance not yet supported for ThermochromicGlazings.");
  }

  OptionalDouble ThermochromicGlazing_Impl::getVisibleTransmittance() const {
    LOG_AND_THROW("Visible transmittance not yet supported for ThermochromicGlazings.");
  }

  double ThermochromicGlazing_Impl::visibleAbsorptance() const {
    LOG_AND_THROW("Visible absorptance not yet supported for ThermochromicGlazings.");
  }

  double ThermochromicGlazing_Impl::visibleReflectance() const {
    LOG_AND_THROW("Visible reflectance not yet supported for ThermochromicGlazings.");
  }

  const std::vector<std::string>& ThermochromicGlazing_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  double ThermochromicGlazing_Impl::opticalDataTemperature() const {
    boost::optional<double> value = getDouble(OS_WindowMaterial_GlazingGroup_ThermochromicExtensibleFields::OpticalDataTemperature,false); // no default
    //OS_ASSERT(value); TODO
    //return value.get(); TODO
    return 0;
  }

  bool ThermochromicGlazing_Impl::setThickness(double value) {
    GlazingVector glazings = mf_glazings();
    DoubleVector rollbackValues;
    for (unsigned i = 0,n = glazings.size(); i < n; ++ i) {
      rollbackValues.push_back(glazings[i].thickness());
      bool ok = glazings[i].setThickness(value);
      if (!ok) {
        // rollback previous values
        for (int j = i-1; j >= 0; --j) {
          glazings[j].setThickness(rollbackValues[j]);
        }
        return false;
      }
    }
    return true;
  }

  bool ThermochromicGlazing_Impl::setThermalConductivity(double value) {
    return false;
  }

  bool ThermochromicGlazing_Impl::setThermalConductance(double value) {
    return false;
  }

  bool ThermochromicGlazing_Impl::setThermalResistivity(double value) {
    return false;
  }

  bool ThermochromicGlazing_Impl::setThermalResistance(double value) {
    return false;
  }

  bool ThermochromicGlazing_Impl::setThermalTransmittance(double value) {
    return false;
  }

  bool ThermochromicGlazing_Impl::setThermalAbsorptance(double value) {
    return false;
  }

  bool ThermochromicGlazing_Impl::setThermalReflectance(double value) {
    return false;
  }

  bool ThermochromicGlazing_Impl::setSolarTransmittance(double value) {
    return false;
  }

  bool ThermochromicGlazing_Impl::setSolarAbsorptance(double value) {
    return false;
  }

  bool ThermochromicGlazing_Impl::setSolarReflectance(double value) {
    return false;
  }

  bool ThermochromicGlazing_Impl::setVisibleTransmittance(double value) {
    return false;
  }

  bool ThermochromicGlazing_Impl::setVisibleAbsorptance(double value) {
    return false;
  }

  bool ThermochromicGlazing_Impl::setVisibleReflectance(double value) {
    return false;
  }

  std::vector<Glazing> ThermochromicGlazing_Impl::mf_glazings() const {
    GlazingVector result;
    for (const IdfExtensibleGroup& idfGroup : extensibleGroups()) {
      ModelExtensibleGroup group = idfGroup.cast<ModelExtensibleGroup>();
      OptionalWorkspaceObject owo = group.getTarget(OS_WindowMaterial_GlazingGroup_ThermochromicExtensibleFields::WindowMaterialGlazingName);
      if (owo) {
        OptionalGlazing og = owo->optionalCast<Glazing>();
        OS_ASSERT(og);
        result.push_back(*og);
      }
    }
    return result;
  }

  void ThermochromicGlazing_Impl::setOpticalDataTemperature(double value) {
    bool result = setDouble(OS_WindowMaterial_GlazingGroup_ThermochromicExtensibleFields::OpticalDataTemperature,value);
    OS_ASSERT(result);
  }

} // detail

ThermochromicGlazing::ThermochromicGlazing(const Model& model,double opticalDataTemperature)
  : Glazing(ThermochromicGlazing::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ThermochromicGlazing_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  bool ok = true;
  // ok = setHandle();
  OS_ASSERT(ok);

  setOpticalDataTemperature(opticalDataTemperature);

}

IddObjectType ThermochromicGlazing::iddObjectType() {
  return IddObjectType(IddObjectType::OS_WindowMaterial_GlazingGroup_Thermochromic);
}

/// @cond
ThermochromicGlazing::ThermochromicGlazing(std::shared_ptr<detail::ThermochromicGlazing_Impl> impl)
  : Glazing(std::move(impl))
{}
/// @endcond

double ThermochromicGlazing::opticalDataTemperature() const {
  return getImpl<detail::ThermochromicGlazing_Impl>()->opticalDataTemperature();
}

void ThermochromicGlazing::setOpticalDataTemperature(double value) {
  return getImpl<detail::ThermochromicGlazing_Impl>()->setOpticalDataTemperature(value);
}

} // model
} // openstudio

