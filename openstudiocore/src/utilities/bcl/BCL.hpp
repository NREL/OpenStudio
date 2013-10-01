/**********************************************************************
* Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

#ifndef UTILITIES_BCL_BCL_HPP
#define UTILITIES_BCL_BCL_HPP

#include <utilities/UtilitiesAPI.hpp>
#include <utilities/bcl/BCLComponent.hpp>
#include <utilities/bcl/BCLMeasure.hpp>
#include <utilities/core/Path.hpp>
#include <utilities/core/Logger.hpp>
#include <utilities/data/Attribute.hpp>

#include <boost/shared_ptr.hpp>

#include <QObject> 

class QDomElement;
class QFile;
class QNetworkReply;

namespace openstudio{

  class OnDemandGenerator;

  /// This gives information about items from a BCL facet from a meta search result.
  /// A facet is an optional search filter which may be applied to narrow down search results.
  class UTILITIES_API BCLFacet {
  public:
    BCLFacet(const QDomElement& facetElement);

    /// Field name of the facet to use during meta searches,
    /// e.g. 'construction_type'
    std::string field() const;

    /// Human readable name of the facet,
    /// e.g. 'Construction Type'
    std::string label() const;

    /// Possible values for the facet as well as the number of search results after applying each value, 
    /// e.g. [['steel framed', 30], ['metal building', 21], ...]
    std::vector<std::pair<std::string, uint> > items() const;

  private:
    std::string m_field;
    std::string m_label;
    std::vector<std::pair<std::string, uint> > m_items;
  };

  /// This gives information about a taxonomy term from the taxonomy.
  class UTILITIES_API BCLTaxonomyTerm {
  public:
    BCLTaxonomyTerm(const QDomElement& taxonomyElement);

    std::string name() const;
    unsigned tid() const;
    unsigned numResults() const;

  private:
    std::string m_name;
    unsigned m_tid;
    unsigned m_numResults;
  };

  /// This gives information about a single file in a BCL search result.
  class UTILITIES_API BCLFile {
  public:
    BCLFile(const QDomElement& fileElement);

    std::string softwareProgram() const;
    std::string identifier() const;
    std::string filename() const;
    std::string filetype() const;
    std::string usageType() const;
    std::string checksum() const;

  private:
    std::string m_softwareProgram;
    std::string m_identifier;
    std::string m_filename;
    std::string m_filetype;
    std::string m_usageType;
    std::string m_checksum;
  };

  /// This gives information about the provenance of a single file in a BCL search result.
  class UTILITIES_API BCLProvenance {
  public:
    BCLProvenance(const QDomElement& provenanceElement);

    std::string author() const;
    std::string datetime() const;
    std::string comment() const;

  private:
    std::string m_author;
    std::string m_datetime;
    std::string m_comment;
  };

  /// This gives information about a cost in a BCL search result.
  class UTILITIES_API BCLCost {
  public:
    BCLCost(const QDomElement& costElement);

    std::string instanceName() const;
    std::string costType() const;
    std::string category() const;
    double value() const;
    std::string units() const;
    std::string interval() const;
    std::string intervalUnits() const;
    unsigned year() const;
    std::string location() const;
    std::string currency() const;
    std::string source() const;
    std::string referenceComponentName() const;
    std::string referenceComponentId() const;

  private:
    std::string m_instanceName;
    std::string m_costType;
    std::string m_category;
    double m_value;
    std::string m_units;
    std::string m_interval;
    std::string m_intervalUnits;
    unsigned m_year;
    std::string m_location;
    std::string m_currency;
    std::string m_source;
    std::string m_referenceComponentName;
    std::string m_referenceComponentId;
  };

  /// This gives information about a meta search result.
  /// A meta search is used to determine the number and types of results of a search
  /// before performing a full search.
  class UTILITIES_API BCLMetaSearchResult {
  public:

    BCLMetaSearchResult(const QDomElement& resultElement);

    unsigned numResults() const;
    std::vector<BCLFacet> facets() const;
    std::vector<BCLTaxonomyTerm> taxonomyTerms() const;

  private:
    REGISTER_LOGGER("openstudio.BCLMetaSearchResult");

    unsigned m_numResults;
    std::vector<BCLFacet> m_facets;
    std::vector<BCLTaxonomyTerm> m_taxonomyTerms;
  };

  /// This gives information about an item in a BCL search result.
  class UTILITIES_API BCLSearchResult {
  public:
    BCLSearchResult(const QDomElement& componentElement);

    std::string name() const;
    std::string uid() const;
    std::string versionId() const;
    std::string description() const;
    std::string modelerDescription() const;
    std::string fidelityLevel() const;
    std::string componentType() const;
    bool provenanceRequired() const;
    std::vector<BCLProvenance> provenances() const;
    std::vector<std::string> tags() const;
    std::vector<Attribute> attributes() const;
    std::vector<BCLFile> files() const;
    std::vector<BCLCost> costs() const;

  private:
    REGISTER_LOGGER("openstudio.BCLSearchResult");

    std::string m_name;
    std::string m_uid;
    std::string m_versionId;
    std::string m_description;
    std::string m_modelerDescription;
    std::string m_fidelityLevel;
    std::string m_componentType;
    bool m_provenanceRequired;
    std::vector<BCLProvenance> m_provenances;
    std::vector<std::string> m_tags;
    std::vector<Attribute> m_attributes;
    std::vector<BCLFile> m_files;
    std::vector<BCLCost> m_costs;
  };

  /// This is a generic interface that can be used for searching either the local or remote bcl.
  class UTILITIES_API BCL : public QObject {

    Q_OBJECT

  public:

    /** @name Constructor */
    //@{

    //@}
    /** @name Destructor */
    //@{

    /// Virtual destructor
    virtual ~BCL();

    //@}
    /** @name Inherited members */
    //@{

    // todo: add the following methods to this interface
    // unsigned resultsPerQuery() const;
    // bool setResultsPerQuery(const int numResults);
    // unsigned numResultPages(unsigned numResults) const;

    /// Get the component by uid
    virtual boost::optional<BCLComponent> getComponent(const std::string& uid, const std::string& versionId = "") const = 0;

    /// Get the measure by uid
    virtual boost::optional<BCLMeasure> getMeasure(const std::string& uid, const std::string& versionId = "") const = 0;

    /// Searches the library for an on demand generated component matching this generator.
    /// This generator should have values for all arguments set.
    virtual boost::optional<BCLComponent> getOnDemandComponent(const OnDemandGenerator& generator) const = 0;

    //@}
  private:

    REGISTER_LOGGER("openstudio.BCL");

    // no body on purpose, do not want this generated
    BCL(const BCL& other);

  protected:
    /// Default constructor
    BCL(QObject * parent = NULL);

  };

  /** Uses LocalBCL and RemoteBCL to get a component from the BCL. Will update LocalBCL with the
   *  downloaded component if that is appropriate. Ultimately returns a BCLComponent from the
   *  LocalBCL (existing or just downloaded). */
  UTILITIES_API boost::optional<BCLComponent> getComponent(const std::string& uid,
                                                           const std::string& versionId = "");

  /** Uses LocalBCL and RemoteBCL to get a measure from the BCL. Will update LocalBCL with the
   *  downloaded measure if that is appropriate. Ultimately returns a BCLMeasure from the
   *  LocalBCL (existing or just downloaded). */
  UTILITIES_API boost::optional<BCLMeasure> getMeasure(const std::string& uid,
                                                       const std::string& versionId = "");

} // openstudio

#endif // UTILITIES_BCL_BCL_HPP
