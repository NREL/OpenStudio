/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef UTILITIES_BCL_BCL_HPP
#define UTILITIES_BCL_BCL_HPP

#include "../UtilitiesAPI.hpp"
#include "BCLComponent.hpp"
#include "BCLMeasure.hpp"
#include "../core/Path.hpp"
#include "../core/Logger.hpp"
#include "../data/Attribute.hpp"

#include <QObject>

class QDomElement;
class QNetworkReply;

namespace openstudio{

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
    boost::optional<VersionString> minCompatibleVersion() const;
    boost::optional<VersionString> maxCompatibleVersion() const;
    std::string filename() const;
    std::string url() const;
    std::string filetype() const;
    std::string usageType() const;
    std::string checksum() const;

  private:
    std::string m_softwareProgram;
    std::string m_identifier;
    boost::optional<VersionString> m_minCompatibleVersion;
    boost::optional<VersionString> m_maxCompatibleVersion;
    std::string m_filename;
    std::string m_url;
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

    //@}
  private:

    REGISTER_LOGGER("openstudio.BCL");

    // no body on purpose, do not want this generated
    BCL(const BCL& other);

  protected:
    /// Default constructor
    BCL(QObject * parent = nullptr);

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
