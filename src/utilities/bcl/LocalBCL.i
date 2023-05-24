#ifndef UTILITIES_BCL_LOCALBCL_I
#define UTILITIES_BCL_LOCALBCL_I

%include <utilities/core/CommonInclude.i>
%import <utilities/core/CommonImport.i>

%{
  #include <utilities/bcl/BCLEnums.hpp>
  #include <utilities/bcl/BCL.hpp>
  #include <utilities/bcl/BCLComponent.hpp>
  #include <utilities/bcl/BCLFileReference.hpp>
  #include <utilities/bcl/BCLMeasureArgument.hpp>
  #include <utilities/bcl/BCLMeasureOutput.hpp>
  #include <utilities/bcl/BCLMeasure.hpp>
  #include <utilities/bcl/BCLXML.hpp>
  #include <utilities/bcl/LocalBCL.hpp>
  #include <utilities/bcl/RemoteBCL.hpp>
  #include <utilities/core/FileReference.hpp>
  #include <utilities/plot/ProgressBar.hpp>

  // TODO: this shouldn't be there really... But I give up refactoring for now
  #include <utilities/units/Quantity.hpp>
  #include <utilities/units/Unit.hpp>
  #include <utilities/units/BTUUnit.hpp>
  #include <utilities/units/CelsiusUnit.hpp>
  #include <utilities/units/CFMUnit.hpp>
  #include <utilities/units/FahrenheitUnit.hpp>
  #include <utilities/units/GPDUnit.hpp>
  #include <utilities/units/IPUnit.hpp>
  #include <utilities/units/Misc1Unit.hpp>
  #include <utilities/units/MPHUnit.hpp>
  #include <utilities/units/SIUnit.hpp>
  #include <utilities/units/ThermUnit.hpp>
  #include <utilities/units/WhUnit.hpp>
%}

%ignore componentDownloaded;
%ignore measureDownloaded;

%ignore std::vector<openstudio::RemoteQueryResponse>::vector(size_type);
%ignore std::vector<openstudio::RemoteQueryResponse>::resize(size_type);
%template(RemoteQueryResponseVector) std::vector<openstudio::RemoteQueryResponse>;

%ignore std::vector<openstudio::BCLSearchResult>::vector(size_type);
%ignore std::vector<openstudio::BCLSearchResult>::resize(size_type);
%template(BCLSearchResultVector) std::vector<openstudio::BCLSearchResult>;
%template(OptionalBCLSearchResult) boost::optional<openstudio::BCLSearchResult>;

%ignore std::vector<openstudio::BCLMetaSearchResult>::vector(size_type);
%ignore std::vector<openstudio::BCLMetaSearchResult>::resize(size_type);
%template(BCLMetaSearchResultVector) std::vector<openstudio::BCLMetaSearchResult>;
%template(OptionalBCLMetaSearchResult) boost::optional<openstudio::BCLMetaSearchResult>;

%ignore std::vector<openstudio::BCLFacet>::vector(size_type);
%ignore std::vector<openstudio::BCLFacet>::resize(size_type);
%template(BCLFacetVector) std::vector<openstudio::BCLFacet>;

%ignore std::vector<openstudio::BCLTaxonomyTerm>::vector(size_type);
%ignore std::vector<openstudio::BCLTaxonomyTerm>::resize(size_type);
%template(BCLTaxonomyTermVector) std::vector<openstudio::BCLTaxonomyTerm>;

%ignore std::vector<openstudio::BCLFile>::vector(size_type);
%ignore std::vector<openstudio::BCLFile>::resize(size_type);
%template(BCLFileVector) std::vector<openstudio::BCLFile>;

%ignore std::vector<openstudio::BCLProvenance>::vector(size_type);
%ignore std::vector<openstudio::BCLProvenance>::resize(size_type);
%template(BCLProvenanceVector) std::vector<openstudio::BCLProvenance>;

%ignore std::vector<openstudio::BCLCost>::vector(size_type);
%ignore std::vector<openstudio::BCLCost>::resize(size_type);
%template(BCLCostVector) std::vector<openstudio::BCLCost>;

%ignore std::vector<openstudio::BCLFileReference>::vector(size_type);
%ignore std::vector<openstudio::BCLFileReference>::resize(size_type);
%template(OptionalBCLFileReference) boost::optional<openstudio::BCLFileReference>;
%template(BCLFileReferenceVector) std::vector<openstudio::BCLFileReference>;

%template(OptionalBCLComponent) boost::optional<openstudio::BCLComponent>;
%template(BCLComponentVector) std::vector<openstudio::BCLComponent>;

%ignore std::vector<openstudio::BCLMeasureArgument>::vector(size_type);
%ignore std::vector<openstudio::BCLMeasureArgument>::resize(size_type);
%template(OptionalBCLMeasureArgument) boost::optional<openstudio::BCLMeasureArgument>;
%template(BCLMeasureArgumentVector) std::vector<openstudio::BCLMeasureArgument>;

%ignore std::vector<openstudio::BCLMeasureOutput>::vector(size_type);
%ignore std::vector<openstudio::BCLMeasureOutput>::resize(size_type);
%template(OptionalBCLMeasureOutput)boost::optional<openstudio::BCLMeasureOutput>;
%template(BCLMeasureOutputVector)std::vector<openstudio::BCLMeasureOutput>;

%ignore std::vector<openstudio::BCLMeasure>::vector(size_type);
%ignore std::vector<openstudio::BCLMeasure>::resize(size_type);
%template(OptionalBCLMeasure) boost::optional<openstudio::BCLMeasure>;
%template(BCLMeasureVector) std::vector<openstudio::BCLMeasure>;

%ignore std::vector<openstudio::BCLXML>::vector(size_type);
%ignore std::vector<openstudio::BCLXML>::resize(size_type);
%template(BCLXMLVector)std::vector<openstudio::BCLXML>;
%template(OptionalBCLXML)boost::optional<openstudio::BCLXML>;

%template(PugixmldocPtr) std::shared_ptr<pugi::xml_document>;

%template(OptionalMeasureType) boost::optional<openstudio::MeasureType>;
%template(OptionalMeasureLanguage) boost::optional<openstudio::MeasureLanguage>;

%include <utilities/bcl/BCLEnums.hpp>
%include <utilities/bcl/BCLFileReference.hpp>
%include <utilities/bcl/BCLComponent.hpp>
%include <utilities/bcl/BCLMeasureArgument.hpp>
%include <utilities/bcl/BCLMeasureOutput.hpp>
%include <utilities/bcl/BCLMeasure.hpp>
%include <utilities/bcl/BCLXML.hpp>
%include <utilities/bcl/BCL.hpp>
%include <utilities/bcl/RemoteBCL.hpp>
%include <utilities/bcl/LocalBCL.hpp>

%extend openstudio::BCLXML {
  std::string __str__() {
    std::ostringstream os;
    os << *self;
    return os.str();
  }
};

#endif // UTILITIES_BCL_LOCALBCL_I
