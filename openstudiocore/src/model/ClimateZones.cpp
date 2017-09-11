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

#include "ClimateZones.hpp"
#include "ClimateZones_Impl.hpp"
#include "Model.hpp"

#include "Site.hpp"
#include "Site_Impl.hpp"

#include <utilities/idd/OS_ClimateZones_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/String.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ClimateZones_Impl::ClimateZones_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ClimateZones::iddObjectType());
  }

  ClimateZones_Impl::ClimateZones_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                       Model_Impl* model,
                                       bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ClimateZones::iddObjectType());
  }

  ClimateZones_Impl::ClimateZones_Impl(const ClimateZones_Impl& other,
                                       Model_Impl* model,
                                       bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  boost::optional<Site> ClimateZones_Impl::site() const {
    return model().getOptionalUniqueModelObject<Site>();
  }

  std::vector<ClimateZone> ClimateZones_Impl::climateZones() const {
    IdfExtensibleGroupVector egs = extensibleGroups();
    ClimateZoneVector result;
    for (const IdfExtensibleGroup& eg : egs) {
      result.push_back(eg.cast<ClimateZone>());
    }
    return result;
  }

  ClimateZone ClimateZones_Impl::getClimateZone(unsigned index) const {
    IdfExtensibleGroup eg = getExtensibleGroup(index);
    if (eg.empty()) {
      std::shared_ptr<ClimateZones_Impl> p;
      return ClimateZone(p,numFields());
    }
    return eg.cast<ClimateZone>();
  }

  ClimateZone ClimateZones_Impl::getClimateZone(const std::string& institution,unsigned year) const {
    std::shared_ptr<ClimateZones_Impl> p;
    ClimateZone result(p,numFields());
    ClimateZoneVector czs = climateZones();
    for (const ClimateZone& cz : czs) {
      if ( istringEqual(cz.institution(),institution) && (cz.year() == year) )
      {
        result = cz;
        break;
      }
    }
    return result;
  }

  std::vector<ClimateZone> ClimateZones_Impl::getClimateZones(const std::string& institution) const {
    ClimateZoneVector result;
    ClimateZoneVector czs = climateZones();
    for (const ClimateZone& cz : czs) {
      if ( istringEqual(cz.institution(),institution) ) {
        result.push_back(cz);
      }
    }
    return result;
  }
/*
  ClimateZone ClimateZones_Impl::activeClimateZone() const {
    std::string activeInstitution = getString(OS_ClimateZonesFields::ActiveInstitution,true).get();
    unsigned activeYear = getUnsigned(OS_ClimateZonesFields::ActiveYear,true).get();
    return getClimateZone(activeInstitution,activeYear);
  }

  std::string ClimateZones_Impl::activeClimateZoneValue() const {
    std::string result;
    ClimateZone cz = activeClimateZone();
    if (!cz.empty()) { result = cz.value(); }
    return result;
  }
*/
  boost::optional<ParentObject> ClimateZones_Impl::parent() const {
    OptionalParentObject result;
    OptionalSite oSite = site();
    if (oSite) { result = oSite->optionalCast<ParentObject>(); }
    return result;
  }

  const std::vector<std::string>& ClimateZones_Impl::outputVariableNames() const {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ClimateZones_Impl::iddObjectType() const {
    return ClimateZones::iddObjectType();
  }

  bool ClimateZones_Impl::clear() {
    clearExtensibleGroups();
    return (numExtensibleGroups() == 0u);
  }
/*
  ClimateZone ClimateZones_Impl::setActiveClimateZone(const std::string& institution) {
    std::shared_ptr<ClimateZones_Impl> p;
    ClimateZone result(p,numFields());

    ClimateZoneVector czs = getClimateZones(institution);
    if (czs.empty()) { result = appendClimateZone(institution); }
    else {
      if (czs.size() > 1) {
        LOG(Warn,"Multiple climate zones have institution '" << institution << "'. Setting the first "
            << "to be active.");
      }
      result = czs[0];
    }
    OS_ASSERT(!result.empty());
    bool ok = setString(OS_ClimateZonesFields::ActiveInstitution,result.institution());
    OS_ASSERT(ok);
    ok = setUnsigned(OS_ClimateZonesFields::ActiveYear,result.year());
    OS_ASSERT(ok);
    return result;
  }

  ClimateZone ClimateZones_Impl::setActiveClimateZone(const std::string& institution, unsigned year) {
    ClimateZone result = getClimateZone(institution,year);
    if (result.empty()) {
      result = appendClimateZone(institution,year,"");
    }
    bool ok = setString(OS_ClimateZonesFields::ActiveInstitution,result.institution());
    OS_ASSERT(ok);
    ok = setUnsigned(OS_ClimateZonesFields::ActiveYear,result.year());
    OS_ASSERT(ok);
    return result;
  }
*/
  ClimateZone ClimateZones_Impl::setClimateZone(const std::string& institution,
                                                const std::string& value)
  {
    std::shared_ptr<ClimateZones_Impl> p;
    ClimateZone result(p,numFields());

    ClimateZoneVector candidates = getClimateZones(institution);
    if (candidates.size() > 1) {
      LOG(Warn,"Cannot set climate zone value for institutuion '" << institution
          << "', because there are " << candidates.size() << " such listings, not 1.");
      return result;
    }
    if (candidates.size() == 0) {
      return appendClimateZone(institution,value);
    }
    bool ok = candidates[0].setValue(value);
    if (ok) { result = candidates[0]; }
    return result;
  }

  ClimateZone ClimateZones_Impl::setClimateZone(const std::string& institution,
                                                unsigned year,
                                                const std::string& value)
  {
    std::shared_ptr<ClimateZones_Impl> p;
    ClimateZone result(p,numFields());

    ClimateZone candidate = getClimateZone(institution,year);
    if (candidate.empty()) {
      return appendClimateZone(institution,
                               ClimateZones::getDefaultDocumentName(institution),
                               year,
                               value);
    }
    bool ok = candidate.setValue(value);
    if (ok) { result = candidate; }
    return result;
  }

  ClimateZone ClimateZones_Impl::appendClimateZone(const std::string& institution) {
    return appendClimateZone(institution,
                             ClimateZones::getDefaultDocumentName(institution),
                             ClimateZones::getDefaultYear(institution),
                             "");

  }

  ClimateZone ClimateZones_Impl::appendClimateZone(const std::string& institution,
                                                   const std::string& value)
  {
    return appendClimateZone(institution,
                             ClimateZones::getDefaultDocumentName(institution),
                             ClimateZones::getDefaultYear(institution),
                             value);
  }

  ClimateZone ClimateZones_Impl::appendClimateZone(const std::string& institution,
                                                   unsigned year,
                                                   const std::string& value)
  {
    return appendClimateZone(institution,
                             ClimateZones::getDefaultDocumentName(institution),
                             year,
                             value);
  }

  ClimateZone ClimateZones_Impl::appendClimateZone(const std::string& institution,
                                                   const std::string documentName,
                                                   unsigned year,
                                                   const std::string& value)
  {
    StringVector values;
    values.push_back(institution);
    values.push_back(documentName);
    std::stringstream ss;
    ss << year;
    values.push_back(ss.str());
    values.push_back(value);
    IdfExtensibleGroup eg = pushExtensibleGroup(values);
    if (eg.empty()) {
      std::shared_ptr<ClimateZones_Impl> p;
      return ClimateZone(p,numFields());
    }
    return eg.cast<ClimateZone>();
  }

  bool ClimateZones_Impl::setParent(ParentObject& newParent) {
    if ( newParent.optionalCast<Site>() && (newParent.model() == model()) ){
      return true;
    }
    return false;
  }

  unsigned ClimateZones_Impl::numClimateZones() const {
    return numExtensibleGroups();
  }

} // detail


// ClimateZone

std::string ClimateZone::institution() const {
  OS_ASSERT(!empty());
  std::string result;
  OptionalString os = getString(OS_ClimateZonesExtensibleFields::ClimateZoneInstitutionName,true);
  if (os) { result = *os; }
  return result;
}

std::string ClimateZone::documentName() const {
  OS_ASSERT(!empty());
  std::string result;
  OptionalString os = getString(OS_ClimateZonesExtensibleFields::ClimateZoneDocumentName,true);
  if (os) { result = *os; }
  return result;
}

unsigned ClimateZone::year() const {
  OS_ASSERT(!empty());
  OptionalUnsigned ou = getUnsigned(OS_ClimateZonesExtensibleFields::ClimateZoneDocumentYear,true);
  if (!ou) {
    LOG_AND_THROW("Climate zone year is not yet set for the " << groupIndex() << "th ClimateZone.");
  }
  return *ou;
}

std::string ClimateZone::value() const {
  OS_ASSERT(!empty());
  std::string result;
  OptionalString os = getString(OS_ClimateZonesExtensibleFields::ClimateZoneValue,true);
  if (os) { result = *os; }
  return result;
}

bool ClimateZone::setValue(const std::string& value) {
  return setString(OS_ClimateZonesExtensibleFields::ClimateZoneValue,value);
}

bool ClimateZone::setType(const std::string& institution,
                          const std::string& documentName,
                          unsigned year)
{
  bool result = setString(OS_ClimateZonesExtensibleFields::ClimateZoneInstitutionName,institution);
  result = result && setString(OS_ClimateZonesExtensibleFields::ClimateZoneDocumentName,documentName);
  result = result && setUnsigned(OS_ClimateZonesExtensibleFields::ClimateZoneDocumentYear,year);

  return result;
}

bool ClimateZone::setTypeAndValue(const std::string& institution,
                                  const std::string& documentName,
                                  unsigned year,
                                  const std::string& value)
{
  bool result = setType(institution,documentName,year);
  result = result && setString(OS_ClimateZonesExtensibleFields::ClimateZoneValue,value);

  return result;
}

ClimateZone::ClimateZone(std::shared_ptr<detail::ClimateZones_Impl> impl,unsigned index)
  : ModelExtensibleGroup(impl,index)
{}


// ClimateZones

std::string ClimateZones::ashraeInstitutionName() {
  static std::string result("ASHRAE");
  return result;
}

std::string ClimateZones::ashraeDocumentName() {
  static std::string result("ANSI/ASHRAE Standard 169");
  return result;
}

unsigned ClimateZones::ashraeDefaultYear() {
  static unsigned result(2006);
  return result;
}

std::string ClimateZones::cecInstitutionName() {
  static std::string result("CEC");
  return result;
}

std::string ClimateZones::cecDocumentName() {
  static std::string result("California Climate Zone Descriptions");
  return result;
}

unsigned ClimateZones::cecDefaultYear() {
  static unsigned result(1995);
  return result;
}

std::string ClimateZones::getDefaultDocumentName(const std::string& institution) {
  std::string result;
  if (istringEqual(institution,ClimateZones::ashraeInstitutionName())) {
    result = ClimateZones::ashraeDocumentName();
  }
  if (istringEqual(institution,ClimateZones::cecInstitutionName())) {
    result = ClimateZones::cecDocumentName();
  }
  return result;
}

unsigned ClimateZones::getDefaultYear(const std::string& institution) {
  unsigned result(0);
  if (istringEqual(institution,ClimateZones::ashraeInstitutionName())) {
    result = ClimateZones::ashraeDefaultYear();
  }
  if (istringEqual(institution,ClimateZones::cecInstitutionName())) {
    result = ClimateZones::cecDefaultYear();
  }
  return result;
}

std::vector<std::string> ClimateZones::validClimateZoneValues(const std::string& institution,
                                                              unsigned year)
{
  StringVector result;
  if (istringEqual(institution,ashraeInstitutionName())) {
    if (year == 2006) {
      result.push_back("1A");
      result.push_back("1B");
      result.push_back("2A");
      result.push_back("2B");
      result.push_back("3A");
      result.push_back("3B");
      result.push_back("3C");
      result.push_back("4A");
      result.push_back("4B");
      result.push_back("4C");
      result.push_back("5A");
      result.push_back("5B");
      result.push_back("5C");
      result.push_back("6A");
      result.push_back("6B");
      result.push_back("7");
      result.push_back("8");
    }
  }

  if (istringEqual(institution,cecInstitutionName())) {
    result.push_back("1");
    result.push_back("2");
    result.push_back("3");
    result.push_back("4");
    result.push_back("5");
    result.push_back("6");
    result.push_back("7");
    result.push_back("8");
    result.push_back("9");
    result.push_back("10");
    result.push_back("11");
    result.push_back("12");
    result.push_back("13");
    result.push_back("14");
    result.push_back("15");
    result.push_back("16");
  }

  return result;
}

IddObjectType ClimateZones::iddObjectType() {
  IddObjectType result(IddObjectType::OS_ClimateZones);
  return result;
}

boost::optional<Site> ClimateZones::site() const {
  return getImpl<detail::ClimateZones_Impl>()->site();
}

std::vector<ClimateZone> ClimateZones::climateZones() const {
  return getImpl<detail::ClimateZones_Impl>()->climateZones();
}

ClimateZone ClimateZones::getClimateZone(unsigned index) const {
  return getImpl<detail::ClimateZones_Impl>()->getClimateZone(index);
}

ClimateZone ClimateZones::getClimateZone(const std::string& institution,unsigned year) const {
  return getImpl<detail::ClimateZones_Impl>()->getClimateZone(institution,year);
}

std::vector<ClimateZone> ClimateZones::getClimateZones(const std::string& institution) const {
  return getImpl<detail::ClimateZones_Impl>()->getClimateZones(institution);
}
/*
ClimateZone ClimateZones::activeClimateZone() const {
  return getImpl<detail::ClimateZones_Impl>()->activeClimateZone();
}

std::string ClimateZones::activeClimateZoneValue() const {
  return getImpl<detail::ClimateZones_Impl>()->activeClimateZoneValue();
}
*/
bool ClimateZones::clear() {
  return getImpl<detail::ClimateZones_Impl>()->clear();
}
/*
ClimateZone ClimateZones::setActiveClimateZone(const std::string& institution) {
  return getImpl<detail::ClimateZones_Impl>()->setActiveClimateZone(institution);
}

ClimateZone ClimateZones::setActiveClimateZone(const std::string& institution,unsigned year) {
  return getImpl<detail::ClimateZones_Impl>()->setActiveClimateZone(institution,year);
}
*/
ClimateZone ClimateZones::setClimateZone(const std::string& institution,
                                         const std::string& value)
{
  return getImpl<detail::ClimateZones_Impl>()->setClimateZone(institution,value);
}

ClimateZone ClimateZones::setClimateZone(const std::string& institution,
                                         unsigned year,
                                         const std::string& value)
{
  return getImpl<detail::ClimateZones_Impl>()->setClimateZone(institution,year,value);
}

ClimateZone ClimateZones::appendClimateZone(const std::string& institution) {
  return getImpl<detail::ClimateZones_Impl>()->appendClimateZone(institution);
}

ClimateZone ClimateZones::appendClimateZone(const std::string& institution,
                                            const std::string& value)
{
  return getImpl<detail::ClimateZones_Impl>()->appendClimateZone(institution,value);
}

ClimateZone ClimateZones::appendClimateZone(const std::string& institution,
                                            unsigned year,
                                            const std::string& value)
{
  return getImpl<detail::ClimateZones_Impl>()->appendClimateZone(institution,year,value);
}

ClimateZone ClimateZones::appendClimateZone(const std::string& institution,
                                            const std::string documentName,
                                            unsigned year,
                                            const std::string& value)
{
  return getImpl<detail::ClimateZones_Impl>()->appendClimateZone(institution,documentName,year,value);
}

unsigned ClimateZones::numClimateZones() const {
  return getImpl<detail::ClimateZones_Impl>()->numClimateZones();
}

/// @cond
ClimateZones::ClimateZones(Model& model)
  : ModelObject(ClimateZones::iddObjectType(),model)
{
  // add empty climate zone to define default institution
  pushExtensibleGroup(StringVector());
  // Programming note: This line of code cannot be in the _Impl constructor because
  // pushExtensibleGroup constructs an IdfExtensibleGroup from the _Impl.
}

ClimateZones::ClimateZones(std::shared_ptr<detail::ClimateZones_Impl> impl)
  : ModelObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio
