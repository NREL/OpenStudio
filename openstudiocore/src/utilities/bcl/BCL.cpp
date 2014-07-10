/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
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

#include "BCL.hpp"

#include "LocalBCL.hpp"
#include "RemoteBCL.hpp"
#include "BCLComponent.hpp"
#include "BCLMeasure.hpp"

#include "../core/Assert.hpp"
#include "../data/Attribute.hpp"

#include <QDomElement>

#include <boost/lexical_cast.hpp>

namespace openstudio{

  BCLFacet::BCLFacet(const QDomElement& facetElement)
  {
    QDomElement fieldElement = facetElement.firstChildElement("field");
    QDomElement labelElement = facetElement.firstChildElement("label");
    QDomElement itemElement = facetElement.firstChildElement("item");

    OS_ASSERT(!fieldElement.isNull());
    OS_ASSERT(!labelElement.isNull());
    OS_ASSERT(!itemElement.isNull());

    m_field = fieldElement.firstChild().nodeValue().toStdString();
  
    m_label = labelElement.firstChild().nodeValue().toStdString();

    while (!itemElement.isNull())
    {
      std::string value = itemElement.firstChildElement("value").firstChild().nodeValue().toStdString();
      uint count = itemElement.firstChildElement("count").firstChild().nodeValue().toUInt();
      m_items.push_back(std::make_pair(value, count));
      itemElement = itemElement.nextSiblingElement("item");
    }
  }

  std::string BCLFacet::field() const
  {
    return m_field;
  }

  std::string BCLFacet::label() const
  {
    return m_label;
  }

  std::vector<std::pair<std::string, uint> > BCLFacet::items() const
  {
    return m_items;
  }

  BCLTaxonomyTerm::BCLTaxonomyTerm(const QDomElement& taxonomyElement)
  {
    QDomElement nameElement = taxonomyElement.firstChildElement("name");
    QDomElement tidElement = taxonomyElement.firstChildElement("tid");
    QDomElement numResultsElement = taxonomyElement.firstChildElement("count");

    OS_ASSERT(!nameElement.isNull());
    OS_ASSERT(!tidElement.isNull());
    OS_ASSERT(!numResultsElement.isNull());

    m_name = nameElement.firstChild().nodeValue().toStdString();
  
    m_tid = tidElement.firstChild().nodeValue().toUInt();

    m_numResults = numResultsElement.firstChild().nodeValue().toUInt();
  }

  std::string BCLTaxonomyTerm::name() const
  {
    return m_name;
  }

  unsigned BCLTaxonomyTerm::tid() const
  {
    return m_tid;
  }

  unsigned BCLTaxonomyTerm::numResults() const
  {
    return m_numResults;
  }

  BCLMetaSearchResult::BCLMetaSearchResult(const QDomElement& resultElement)
  {
    QDomElement numResultsElement = resultElement.firstChildElement("result_count");

    OS_ASSERT(!numResultsElement.isNull());

    m_numResults = numResultsElement.firstChild().nodeValue().toUInt();

    QDomElement facetElement = resultElement.firstChildElement("facet");
    while (!facetElement.isNull())
    {
      QDomElement fieldElement = facetElement.firstChildElement("field");
      if (!fieldElement.isNull()){
        m_facets.push_back(BCLFacet(facetElement));
      }
      facetElement = facetElement.nextSiblingElement("facet");
    }

    QDomElement taxonomyTermElement = resultElement.firstChildElement("taxonomy_term");
    while (!taxonomyTermElement.isNull())
    {
      QDomElement tidElement = taxonomyTermElement.firstChildElement("tid");
      if (!tidElement.isNull()){
        m_taxonomyTerms.push_back(BCLTaxonomyTerm(taxonomyTermElement));
      }
      taxonomyTermElement = taxonomyTermElement.nextSiblingElement("taxonomy_term");
    }
  }

  unsigned BCLMetaSearchResult::numResults() const  
  {
    return m_numResults;
  }

  std::vector<BCLFacet> BCLMetaSearchResult::facets() const  
  {
    return m_facets;
  }

  std::vector<BCLTaxonomyTerm> BCLMetaSearchResult::taxonomyTerms() const  
  {
    return m_taxonomyTerms;
  }


  BCLFile::BCLFile(const QDomElement& fileElement)
  {
    QDomElement softwareProgramElement = fileElement.firstChildElement("version").firstChildElement("software_program");
    QDomElement identifierElement = fileElement.firstChildElement("version").firstChildElement("identifier");
    QDomElement filenameElement = fileElement.firstChildElement("filename");
    QDomElement urlElement = fileElement.firstChildElement("url");
    QDomElement filetypeElement = fileElement.firstChildElement("filetype");
    QDomElement usageTypeElement = fileElement.firstChildElement("usage_type");
    QDomElement checksumElement = fileElement.firstChildElement("checksum");

    m_softwareProgram = softwareProgramElement.firstChild().nodeValue().toStdString();
    m_identifier = identifierElement.firstChild().nodeValue().toStdString();
    m_filename = filenameElement.firstChild().nodeValue().toStdString();
    m_url = urlElement.firstChild().nodeValue().toStdString();
    m_filetype = filetypeElement.firstChild().nodeValue().toStdString();
    m_usageType = usageTypeElement.firstChild().nodeValue().toStdString();
    m_checksum = checksumElement.firstChild().nodeValue().toStdString();
  }
  
  std::string BCLFile::softwareProgram() const
  {
    return m_softwareProgram;
  }

  std::string BCLFile::identifier() const
  {
    return m_identifier;
  }

  std::string BCLFile::filename() const
  {
    return m_filename;
  }

  std::string BCLFile::url() const
  {
    return m_url;
  }

  std::string BCLFile::filetype() const
  {
    return m_filetype;
  }

  std::string BCLFile::usageType() const
  {
    return m_usageType;
  }

  std::string BCLFile::checksum() const
  {
    return m_checksum;
  }


  BCLProvenance::BCLProvenance(const QDomElement& provenanceElement)
  {
    QDomElement authorElement = provenanceElement.firstChildElement("author");
    QDomElement datetimeElement = provenanceElement.firstChildElement("datetime");
    QDomElement commentElement = provenanceElement.firstChildElement("comment");

    m_author = authorElement.firstChild().nodeValue().toStdString();
    m_datetime = datetimeElement.firstChild().nodeValue().toStdString();
    m_comment = commentElement.firstChild().nodeValue().toStdString();
  }
  
  std::string BCLProvenance::author() const
  {
    return m_author;
  }

  std::string BCLProvenance::datetime() const
  {
    return m_datetime;
  }

  std::string BCLProvenance::comment() const
  {
    return m_comment;
  }


  BCLCost::BCLCost(const QDomElement& costElement)
  {
    QDomElement instanceNameElement = costElement.firstChildElement("instance_name");
    QDomElement costTypeElement = costElement.firstChildElement("cost_type");
    QDomElement categoryElement = costElement.firstChildElement("category");
    QDomElement valueElement = costElement.firstChildElement("value");
    QDomElement unitsElement = costElement.firstChildElement("units");
    QDomElement intervalElement = costElement.firstChildElement("interval");
    QDomElement intervalUnitsElement = costElement.firstChildElement("interval_units");
    QDomElement yearElement = costElement.firstChildElement("year");
    QDomElement locationElement = costElement.firstChildElement("location");
    QDomElement currencyElement = costElement.firstChildElement("currency");
    QDomElement sourceElement = costElement.firstChildElement("source");
    QDomElement referenceComponentNameElement = costElement.firstChildElement("reference_component_name");
    QDomElement referenceComponentIdElement = costElement.firstChildElement("reference_component_id");

    m_instanceName = instanceNameElement.firstChild().nodeValue().toStdString();
    m_costType = costTypeElement.firstChild().nodeValue().toStdString();
    m_category = categoryElement.firstChild().nodeValue().toStdString();
    m_value = valueElement.firstChild().nodeValue().toDouble();
    if (!unitsElement.isNull()){
      m_units = unitsElement.firstChild().nodeValue().toStdString();
    }
    if (!intervalElement.isNull()){
      m_interval = intervalElement.firstChild().nodeValue().toStdString();
    }
    if (!intervalUnitsElement.isNull()){
      m_intervalUnits = intervalUnitsElement.firstChild().nodeValue().toStdString();
    }
    if (!yearElement.isNull()){
      m_year = yearElement.firstChild().nodeValue().toUInt();
    }else{
      m_year = 0;
    }
    if (!locationElement.isNull()){
      m_location = locationElement.firstChild().nodeValue().toStdString();
    }
    if (!currencyElement.isNull()){
      m_currency = currencyElement.firstChild().nodeValue().toStdString();
    }
    if (!sourceElement.isNull()){
      m_source = sourceElement.firstChild().nodeValue().toStdString();
    }
    if (!referenceComponentNameElement.isNull()){
      m_referenceComponentName = referenceComponentNameElement.firstChild().nodeValue().toStdString();
    }
    if (!referenceComponentIdElement.isNull()){
      m_referenceComponentId = referenceComponentIdElement.firstChild().nodeValue().toStdString();
    }
  }

  std::string BCLCost::instanceName() const  
  {
    return m_instanceName;
  }

  std::string BCLCost::costType() const  
  {
    return m_costType;
  }

  std::string BCLCost::category() const
  {
    return m_category;
  }

  double BCLCost::value() const
  {
    return m_value;
  }

  std::string BCLCost::units() const
  {
    return m_units;
  }

  std::string BCLCost::interval() const
  {
    return m_interval;
  }

  std::string BCLCost::intervalUnits() const
  {
    return m_intervalUnits;
  }

  unsigned BCLCost::year() const
  {
    return m_year;
  }

  std::string BCLCost::location() const
  {
    return m_location;
  }

  std::string BCLCost::currency() const
  {
    return m_currency;
  }

  std::string BCLCost::source() const
  {
    return m_source;
  }

  std::string BCLCost::referenceComponentName() const
  {
    return m_referenceComponentName;
  }

  std::string BCLCost::referenceComponentId() const
  {
    return m_referenceComponentId;
  }


  BCLSearchResult::BCLSearchResult(const QDomElement& componentElement)
  {
    m_componentType = componentElement.tagName().toStdString();

    QDomElement uidElement = componentElement.firstChildElement("uuid");
    QDomElement versionIdElement = componentElement.firstChildElement("vuuid");
    QDomElement nameElement = componentElement.firstChildElement("name");
    QDomElement descriptionElement = componentElement.firstChildElement("description");
    QDomElement modelerDescriptionElement = componentElement.firstChildElement("modeler_description");
    QDomElement fidelityLevelElement = componentElement.firstChildElement("fidelity_level");
    QDomElement provenancesElement = componentElement.firstChildElement("provenances");
    QDomElement tagsElement = componentElement.firstChildElement("tags");
    QDomElement attributesElement = componentElement.firstChildElement("attributes");
    QDomElement filesElement = componentElement.firstChildElement("files");
    QDomElement costsElement = componentElement.firstChildElement("costs");

    OS_ASSERT(!nameElement.isNull());
    OS_ASSERT(!uidElement.isNull());
    OS_ASSERT(!versionIdElement.isNull());
    
    QString name = nameElement.firstChild().nodeValue().replace('_', ' ');
    while (name.indexOf("  ") != -1) {
      name = name.replace("  ", " ");
    }
    name[0] = name[0].toUpper();
    m_name = name.toStdString();

    if (!uidElement.isNull() && uidElement.firstChild().nodeValue().length() == 36)
    {
      m_uid = uidElement.firstChild().nodeValue().toStdString();
    }

    if (!versionIdElement.isNull() && versionIdElement.firstChild().nodeValue().length() == 36)
    {
      m_versionId = versionIdElement.firstChild().nodeValue().toStdString();
    }

    if (!descriptionElement.isNull())
    {
      m_description = descriptionElement.firstChild().nodeValue().toStdString();
    }

    if (!modelerDescriptionElement.isNull())
    {
      m_modelerDescription = modelerDescriptionElement.firstChild().nodeValue().toStdString();
    }

    if (!fidelityLevelElement.isNull())
    {
      m_fidelityLevel= fidelityLevelElement.firstChild().nodeValue().toStdString();
    }

    QDomElement provenanceElement = provenancesElement.firstChildElement("provenance");
    while (!provenanceElement.isNull())
    {
      if (provenanceElement.hasChildNodes())
      {
        m_provenances.push_back(BCLProvenance(provenanceElement));
      }
      else
      {
        break;
      }
      provenanceElement = provenanceElement.nextSiblingElement("provenance");
    }
    QDomElement provenanceRequiredElement = provenancesElement.firstChildElement("provenance_required");
    if (!provenanceRequiredElement.isNull())
    {
      std::string required = provenanceRequiredElement.firstChild().nodeValue().toStdString();
      m_provenanceRequired = (required == "true") ? true : false;
    }

    QDomElement tagElement = tagsElement.firstChildElement("tag");
    while (!tagElement.isNull())
    {
      m_tags.push_back(tagElement.firstChild().nodeValue().toStdString());
      tagElement = tagElement.nextSiblingElement("tag");
    }

    QDomElement attributeElement = attributesElement.firstChildElement("attribute");
    while (!attributeElement.isNull())
    {
      if (attributeElement.hasChildNodes())
      {
        std::string name = attributeElement.firstChildElement("name").firstChild()
          .nodeValue().toStdString();
        std::string value = attributeElement.firstChildElement("value").firstChild()
          .nodeValue().toStdString();
        //std::string datatype = attributeElement.firstChildElement("datatype").firstChild()
        //  .nodeValue().toStdString();

        // Units are optional
        std::string units = attributeElement.firstChildElement("units").firstChild()
          .nodeValue().toStdString();
        
        /*if (datatype == "float")
        {
          if (units.empty())
          {
            Attribute attr(name, boost::lexical_cast<double>(value));
            m_attributes.push_back(attr);
          }
          else
          {
            Attribute attr(name, boost::lexical_cast<double>(value), units);
            m_attributes.push_back(attr);
          }
        }
        else if (datatype == "int")
        {
          if (units.empty())
          {
            Attribute attr(name, boost::lexical_cast<int>(value));
            m_attributes.push_back(attr);
          }
          else
          {
            Attribute attr(name, boost::lexical_cast<int>(value), units);
            m_attributes.push_back(attr);
          }
        }
        // Assume string
        else
        {*/
        if (units.empty())
        {
          Attribute attr(name, value);
          m_attributes.push_back(attr);
        }
        else
        {
          Attribute attr(name, value, units);
          m_attributes.push_back(attr);
        }
          //LOG(Error, "Error: Unrecognized attribute datatype \"" << datatype << "\"");
        //}
      }
      else
      {
        break;
      }
      attributeElement = attributeElement.nextSiblingElement("attribute");
    }

    QDomElement fileElement = filesElement.firstChildElement("file");
    while (!fileElement.isNull())
    {
      if (fileElement.hasChildNodes())
      {
        m_files.push_back(BCLFile(fileElement));
      }
      else
      {
        break;
      }
      fileElement = fileElement.nextSiblingElement("file");
    }

    if (m_componentType == "component")
    {
      QDomElement costElement = costsElement.firstChildElement("cost");
      while (!costElement.isNull())
      {
        if (costElement.hasChildNodes())
        {
          m_costs.push_back(BCLCost(costElement));
        }
        else
        {
          break;
        }
        costElement = costElement.nextSiblingElement("cost");
      }
    }
  }

  std::string BCLSearchResult::uid() const
  {
    return m_uid;
  }

  std::string BCLSearchResult::versionId() const
  {
    return m_versionId;
  }

  std::string BCLSearchResult::name() const
  {
    return m_name;
  }

  std::string BCLSearchResult::description() const
  {
    return m_description;
  }

  std::string BCLSearchResult::modelerDescription() const
  {
    return m_modelerDescription;
  }

  std::string BCLSearchResult::fidelityLevel() const
  {
    return m_fidelityLevel;
  }

  std::string BCLSearchResult::componentType() const
  {
    return m_componentType;
  }

  bool BCLSearchResult::provenanceRequired() const
  {
    return m_provenanceRequired;
  }
  
  std::vector<BCLProvenance> BCLSearchResult::provenances() const
  {
    return m_provenances;
  }

  std::vector<std::string> BCLSearchResult::tags() const
  {
    return m_tags;
  }

  std::vector<Attribute> BCLSearchResult::attributes() const
  {
    return m_attributes;
  }

  std::vector<BCLFile> BCLSearchResult::files() const
  {
    return m_files;
  }

  std::vector<BCLCost> BCLSearchResult::costs() const
  {
    return m_costs;
  }

  BCL::BCL(QObject * parent):
    QObject(parent)
  {
  }

  BCL::~BCL()
  {
  }


  boost::optional<BCLComponent> getComponent(const std::string& uid,
                                             const std::string& versionId)
  {
    OptionalBCLComponent localComponent = LocalBCL::instance().getComponent(uid,versionId);

    // if versionId specified, done if localComponent exists
    if (!versionId.empty() && localComponent) {
      return localComponent;
    }

    // versionId.empty() or !localComponent
    RemoteBCL remoteBCL;
    OptionalBCLComponent remoteComponent = remoteBCL.getComponent(uid,versionId);
    if (remoteComponent) {
      // RemoteBCL class handles updating the LocalBCL
      localComponent = LocalBCL::instance().getComponent(uid,versionId);
      OS_ASSERT(localComponent);
      OS_ASSERT(localComponent.get() == remoteComponent.get());
    }

    return localComponent;
  }

  boost::optional<BCLMeasure> getMeasure(const std::string& uid,
                                         const std::string& versionId)
  {
    OptionalBCLMeasure localMeasure = LocalBCL::instance().getMeasure(uid,versionId);

    // if versionId specified, done if localMeasure exists
    if (!versionId.empty() && localMeasure) {
      return localMeasure;
    }

    // versionId.empty() or !localMeasure
    RemoteBCL remoteBCL;
    OptionalBCLMeasure remoteMeasure = remoteBCL.getMeasure(uid,versionId);
    if (remoteMeasure) {
      // RemoteBCL class handles updating the LocalBCL
      localMeasure = LocalBCL::instance().getMeasure(uid,versionId);
      OS_ASSERT(localMeasure);
      OS_ASSERT(localMeasure.get() == remoteMeasure.get());
    }

    return localMeasure;
  }

} // openstudio
