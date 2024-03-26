/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "BCL.hpp"

#include "LocalBCL.hpp"
#include "RemoteBCL.hpp"

#include "../core/Assert.hpp"
#include "../core/StringHelpers.hpp"
#include "../time/DateTime.hpp"

#include <pugixml.hpp>

namespace openstudio {

BCLFacet::BCLFacet(const pugi::xml_node& facetElement) {
  auto fieldElement = facetElement.child("field");
  auto labelElement = facetElement.child("label");
  auto itemElement = facetElement.child("item");

  OS_ASSERT(fieldElement);
  OS_ASSERT(labelElement);
  OS_ASSERT(itemElement);

  m_field = fieldElement.text().as_string();

  m_label = labelElement.text().as_string();

  while (itemElement) {
    std::string value = itemElement.child("value").text().as_string();
    unsigned count = itemElement.child("count").text().as_uint();
    m_items.push_back(std::make_pair(value, count));
    itemElement = itemElement.next_sibling("item");
  }
}

std::string BCLFacet::field() const {
  return m_field;
}

std::string BCLFacet::label() const {
  return m_label;
}

std::vector<std::pair<std::string, unsigned>> BCLFacet::items() const {
  return m_items;
}

BCLTaxonomyTerm::BCLTaxonomyTerm(const pugi::xml_node& taxonomyElement) {
  auto nameElement = taxonomyElement.child("name");
  auto tidElement = taxonomyElement.child("tid");
  auto numResultsElement = taxonomyElement.child("count");

  OS_ASSERT(nameElement);
  OS_ASSERT(tidElement);
  OS_ASSERT(numResultsElement);

  m_name = nameElement.text().as_string();

  m_tid = tidElement.text().as_uint();

  m_numResults = numResultsElement.text().as_uint();
}

std::string BCLTaxonomyTerm::name() const {
  return m_name;
}

unsigned BCLTaxonomyTerm::tid() const {
  return m_tid;
}

unsigned BCLTaxonomyTerm::numResults() const {
  return m_numResults;
}

BCLMetaSearchResult::BCLMetaSearchResult(const pugi::xml_node& resultElement) {
  auto numResultsElement = resultElement.child("result_count");

  OS_ASSERT(numResultsElement);

  m_numResults = numResultsElement.text().as_uint();

  auto facetElement = resultElement.child("facet");
  while (facetElement) {
    auto fieldElement = facetElement.child("field");
    if (fieldElement) {
      m_facets.push_back(BCLFacet(facetElement));
    }
    facetElement = facetElement.next_sibling("facet");
  }

  auto taxonomyTermElement = resultElement.child("taxonomy_term");
  while (taxonomyTermElement) {
    auto tidElement = taxonomyTermElement.child("tid");
    if (tidElement) {
      m_taxonomyTerms.push_back(BCLTaxonomyTerm(taxonomyTermElement));
    }
    taxonomyTermElement = taxonomyTermElement.next_sibling("taxonomy_term");
  }
}

unsigned BCLMetaSearchResult::numResults() const {
  return m_numResults;
}

std::vector<BCLFacet> BCLMetaSearchResult::facets() const {
  return m_facets;
}

std::vector<BCLTaxonomyTerm> BCLMetaSearchResult::taxonomyTerms() const {
  return m_taxonomyTerms;
}

BCLFile::BCLFile(const pugi::xml_node& fileElement) {
  auto version = fileElement.child("version");
  auto softwareProgramElement = version.child("software_program");
  auto identifierElement = version.child("identifier");
  auto minCompatibleElement = version.child("min_compatible");
  auto maxCompatibleElement = version.child("max_compatible");
  auto filenameElement = fileElement.child("filename");
  auto urlElement = fileElement.child("url");
  auto filetypeElement = fileElement.child("filetype");
  auto usageTypeElement = fileElement.child("usage_type");
  auto checksumElement = fileElement.child("checksum");

  m_softwareProgram = softwareProgramElement.text().as_string();
  m_identifier = identifierElement.text().as_string();

  // Avoids gettings lots of 'Could not parse '' as a version string'
  auto parseVersionIfExistsAndNotEmpty = [](const pugi::xml_node& vElement) -> boost::optional<VersionString> {
    if (!vElement) {
      return boost::none;
    }
    const std::string vstr = vElement.text().as_string();
    if (vstr.empty()) {
      return boost::none;
    }
    boost::optional<VersionString> result;

    try {
      result = VersionString(vstr);
    } catch (const std::exception&) {
    }
    return result;
  };

  if (!minCompatibleElement) {
    m_minCompatibleVersion = parseVersionIfExistsAndNotEmpty(identifierElement);
  } else {
    m_minCompatibleVersion = parseVersionIfExistsAndNotEmpty(minCompatibleElement);
  }

  m_maxCompatibleVersion = parseVersionIfExistsAndNotEmpty(maxCompatibleElement);

  m_filename = filenameElement.text().as_string();
  m_url = urlElement.text().as_string();
  m_filetype = filetypeElement.text().as_string();
  m_usageType = usageTypeElement.text().as_string();
  m_checksum = checksumElement.text().as_string();
}

std::string BCLFile::softwareProgram() const {
  return m_softwareProgram;
}

std::string BCLFile::identifier() const {
  return m_identifier;
}

boost::optional<VersionString> BCLFile::minCompatibleVersion() const {
  return m_minCompatibleVersion;
}

boost::optional<VersionString> BCLFile::maxCompatibleVersion() const {
  return m_maxCompatibleVersion;
}

std::string BCLFile::filename() const {
  return m_filename;
}

std::string BCLFile::url() const {
  return m_url;
}

std::string BCLFile::filetype() const {
  return m_filetype;
}

std::string BCLFile::usageType() const {
  return m_usageType;
}

std::string BCLFile::checksum() const {
  return m_checksum;
}

BCLProvenance::BCLProvenance(const pugi::xml_node& provenanceElement) {
  auto authorElement = provenanceElement.child("author");
  auto datetimeElement = provenanceElement.child("datetime");
  auto commentElement = provenanceElement.child("comment");

  m_author = authorElement.text().as_string();
  m_datetime = datetimeElement.text().as_string();
  m_comment = commentElement.text().as_string();
}

std::string BCLProvenance::author() const {
  return m_author;
}

std::string BCLProvenance::datetime() const {
  return m_datetime;
}

std::string BCLProvenance::comment() const {
  return m_comment;
}

BCLCost::BCLCost(const pugi::xml_node& costElement) {
  auto instanceNameElement = costElement.child("instance_name");
  auto costTypeElement = costElement.child("cost_type");
  auto categoryElement = costElement.child("category");
  auto valueElement = costElement.child("value");
  auto unitsElement = costElement.child("units");
  auto intervalElement = costElement.child("interval");
  auto intervalUnitsElement = costElement.child("interval_units");
  auto yearElement = costElement.child("year");
  auto locationElement = costElement.child("location");
  auto currencyElement = costElement.child("currency");
  auto sourceElement = costElement.child("source");
  auto referenceComponentNameElement = costElement.child("reference_component_name");
  auto referenceComponentIdElement = costElement.child("reference_component_id");

  m_instanceName = instanceNameElement.text().as_string();
  m_costType = costTypeElement.text().as_string();
  m_category = categoryElement.text().as_string();
  m_value = valueElement.text().as_double();

  m_units = unitsElement.text().as_string();
  m_interval = intervalElement.text().as_string();
  m_intervalUnits = intervalUnitsElement.text().as_string();

  m_year = yearElement.text().as_uint(0);

  m_location = locationElement.text().as_string();
  m_currency = currencyElement.text().as_string();
  m_source = sourceElement.text().as_string();
  m_referenceComponentName = referenceComponentNameElement.text().as_string();
  m_referenceComponentId = referenceComponentIdElement.text().as_string();
}

std::string BCLCost::instanceName() const {
  return m_instanceName;
}

std::string BCLCost::costType() const {
  return m_costType;
}

std::string BCLCost::category() const {
  return m_category;
}

double BCLCost::value() const {
  return m_value;
}

std::string BCLCost::units() const {
  return m_units;
}

std::string BCLCost::interval() const {
  return m_interval;
}

std::string BCLCost::intervalUnits() const {
  return m_intervalUnits;
}

unsigned BCLCost::year() const {
  return m_year;
}

std::string BCLCost::location() const {
  return m_location;
}

std::string BCLCost::currency() const {
  return m_currency;
}

std::string BCLCost::source() const {
  return m_source;
}

std::string BCLCost::referenceComponentName() const {
  return m_referenceComponentName;
}

std::string BCLCost::referenceComponentId() const {
  return m_referenceComponentId;
}

BCLSearchResult::BCLSearchResult(const pugi::xml_node& componentElement) : m_componentType{componentElement.name()} {

  auto uidElement = componentElement.child("uuid");
  auto versionIdElement = componentElement.child("vuuid");
  auto nameElement = componentElement.child("name");
  auto descriptionElement = componentElement.child("description");
  auto modelerDescriptionElement = componentElement.child("modeler_description");
  auto fidelityLevelElement = componentElement.child("fidelity_level");
  auto provenancesElement = componentElement.child("provenances");
  auto tagsElement = componentElement.child("tags");
  auto attributesElement = componentElement.child("attributes");
  auto filesElement = componentElement.child("files");
  auto costsElement = componentElement.child("costs");

  OS_ASSERT(nameElement);
  OS_ASSERT(uidElement);
  OS_ASSERT(versionIdElement);

  std::string name = nameElement.text().as_string();
  std::replace(name.begin(), name.end(), '_', ' ');
  auto index = name.find("  ", 0);
  while (index != std::string::npos) {
    name.replace(index, 2, " ");
    ++index;
    index = name.find("  ", index);
  }
  //QString name = nameElement.firstChild().nodeValue().replace('_', ' ');
  //while (name.indexOf("  ") != -1) {
  //  name = name.replace("  ", " ");
  //}
  name[0] = std::toupper(name[0]);  // Why is this needed? Won't this be an issue for unicode, etc.?
  m_name = name;

  if (uidElement) {
    std::string txt = uidElement.text().as_string();
    if (txt.length() == 36) {
      m_uid = txt;
    }
  }

  if (versionIdElement) {
    std::string txt = versionIdElement.text().as_string();
    if (txt.length() == 36) {
      m_versionId = txt;
    }
  }

  m_description = descriptionElement.text().as_string();
  m_modelerDescription = modelerDescriptionElement.text().as_string();
  m_fidelityLevel = fidelityLevelElement.text().as_string();

  auto provenanceElement = provenancesElement.child("provenance");
  while (provenanceElement) {
    if (provenanceElement.first_child() != nullptr) {
      m_provenances.emplace_back(provenanceElement);
    } else {
      break;
    }
    provenanceElement = provenanceElement.next_sibling("provenance");
  }
  auto provenanceRequiredElement = provenancesElement.child("provenance_required");
  if (provenanceRequiredElement) {
    std::string required = provenanceRequiredElement.text().as_string();
    m_provenanceRequired = (required == "true");
  }

  auto tagElement = tagsElement.child("tag");
  while (tagElement) {
    m_tags.push_back(tagElement.text().as_string());
    tagElement = tagElement.next_sibling("tag");
  }

  auto attributeElement = attributesElement.child("attribute");
  while (attributeElement) {
    if (attributeElement.first_child() != nullptr) {
      std::string att_name = attributeElement.child("name").text().as_string();
      std::string value = attributeElement.child("value").text().as_string();
      std::string datatype = attributeElement.child("datatype").text().as_string();

      // Units are optional
      std::string units = attributeElement.child("units").text().as_string();

      const auto doubleValue = openstudio::string_conversions::to_no_throw<double>(value);
      const auto intValue = openstudio::string_conversions::to_no_throw<int>(value);

      if (datatype == "float" && doubleValue) {
        if (units.empty()) {
          Attribute attr(att_name, *doubleValue);
          m_attributes.push_back(attr);
        } else {
          Attribute attr(att_name, *doubleValue, units);
          m_attributes.push_back(attr);
        }
      } else if (datatype == "int" && intValue) {
        if (units.empty()) {
          Attribute attr(att_name, *intValue);
          m_attributes.push_back(attr);
        } else {
          Attribute attr(att_name, *intValue, units);
          m_attributes.push_back(attr);
        }
      }
      // Assume string
      else {
        if (units.empty()) {
          Attribute attr(att_name, value);
          m_attributes.push_back(attr);
        } else {
          Attribute attr(att_name, value, units);
          m_attributes.push_back(attr);
        }
        //LOG(Error, "Error: Unrecognized attribute datatype \"" << datatype << "\"");
      }
    } else {
      break;
    }
    attributeElement = attributeElement.next_sibling("attribute");
  }

  auto fileElement = filesElement.child("file");
  while (fileElement) {
    if (fileElement.first_child() != nullptr) {
      m_files.emplace_back(fileElement);  // BCLFile
    } else {
      break;
    }
    fileElement = fileElement.next_sibling("file");
  }

  if (m_componentType == "component") {
    auto costElement = costsElement.child("cost");
    while (costElement) {
      if (costElement.first_child() != nullptr) {
        m_costs.emplace_back(costElement);  // BCLCost
      } else {
        break;
      }
      costElement = costElement.next_sibling("cost");
    }
  }

  if (auto orgElement = componentElement.child("org")) {
    m_org = orgElement.text().as_string();
  }
  if (auto repoElement = componentElement.child("repo")) {
    m_repo = repoElement.text().as_string();
  }
  if (auto release_tagElement = componentElement.child("release_tag")) {
    m_releaseTag = release_tagElement.text().as_string();
  }
  if (auto versionModifiedElement = componentElement.child("version_modified")) {
    const std::string versionModified = versionModifiedElement.text().as_string();
    if (!versionModified.empty()) {
      // fromXsdDateTime forwards to fromISO8601 and handles both formats
      if (auto dt_ = openstudio::DateTime::fromXsdDateTime(versionModified)) {
        m_versionModified = *dt_;
      }
    }
  }
}

std::string BCLSearchResult::uid() const {
  return m_uid;
}

std::string BCLSearchResult::versionId() const {
  return m_versionId;
}

std::string BCLSearchResult::name() const {
  return m_name;
}

std::string BCLSearchResult::description() const {
  return m_description;
}

std::string BCLSearchResult::modelerDescription() const {
  return m_modelerDescription;
}

std::string BCLSearchResult::fidelityLevel() const {
  return m_fidelityLevel;
}

std::string BCLSearchResult::componentType() const {
  return m_componentType;
}

bool BCLSearchResult::provenanceRequired() const {
  return m_provenanceRequired;
}

std::vector<BCLProvenance> BCLSearchResult::provenances() const {
  return m_provenances;
}

std::vector<std::string> BCLSearchResult::tags() const {
  return m_tags;
}

std::vector<Attribute> BCLSearchResult::attributes() const {
  return m_attributes;
}

std::vector<BCLFile> BCLSearchResult::files() const {
  return m_files;
}

std::vector<BCLCost> BCLSearchResult::costs() const {
  return m_costs;
}

std::string BCLSearchResult::org() const {
  return m_org;
}
std::string BCLSearchResult::repo() const {
  return m_repo;
}
std::string BCLSearchResult::releaseTag() const {
  return m_releaseTag;
}
boost::optional<openstudio::DateTime> BCLSearchResult::versionModified() const {
  return m_versionModified;
}

BCL::BCL() = default;

boost::optional<BCLComponent> getComponent(const std::string& uid, const std::string& versionId) {
  OptionalBCLComponent localComponent = LocalBCL::instance().getComponent(uid, versionId);

  // if versionId specified, done if localComponent exists
  if (!versionId.empty() && localComponent) {
    return localComponent;
  }

  // versionId.empty() or !localComponent
  RemoteBCL remoteBCL;
  OptionalBCLComponent remoteComponent = remoteBCL.getComponent(uid, versionId);
  if (remoteComponent) {
    // RemoteBCL class handles updating the LocalBCL
    localComponent = LocalBCL::instance().getComponent(uid, versionId);
    OS_ASSERT(localComponent);
    OS_ASSERT(localComponent.get() == remoteComponent.get());
  }

  return localComponent;
}

boost::optional<BCLMeasure> getMeasure(const std::string& uid, const std::string& versionId) {
  OptionalBCLMeasure localMeasure = LocalBCL::instance().getMeasure(uid, versionId);

  // if versionId specified, done if localMeasure exists
  if (!versionId.empty() && localMeasure) {
    return localMeasure;
  }

  // versionId.empty() or !localMeasure
  RemoteBCL remoteBCL;
  OptionalBCLMeasure remoteMeasure = remoteBCL.getMeasure(uid, versionId);
  if (remoteMeasure) {
    // RemoteBCL class handles updating the LocalBCL
    localMeasure = LocalBCL::instance().getMeasure(uid, versionId);
    OS_ASSERT(localMeasure);
    OS_ASSERT(localMeasure.get() == remoteMeasure.get());
  }

  return localMeasure;
}

}  // namespace openstudio
