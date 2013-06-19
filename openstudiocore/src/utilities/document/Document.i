#ifndef UTILITIES_DOCUMENT_DOCUMENT_I
#define UTILITIES_DOCUMENT_DOCUMENT_I

%ignore openstudio::detail;

%{
  #include <utilities/document/DocumentRegex.hpp>
  #include <utilities/document/HeadersAndFooters.hpp>

  #include <utilities/document/DocumentEnums.hpp>
  #include <utilities/document/DocumentElement.hpp>
  #include <utilities/document/DocumentElement_Impl.hpp>
  #include <utilities/document/SectionHeading.hpp>
  #include <utilities/document/SectionHeading_Impl.hpp>
  #include <utilities/document/SectionElement.hpp>
  #include <utilities/document/SectionElement_Impl.hpp>
  #include <utilities/document/Section.hpp>
  #include <utilities/document/Section_Impl.hpp>
  #include <utilities/document/Text.hpp>
  #include <utilities/document/Text_Impl.hpp>
  #include <utilities/document/Document.hpp>
    
  #include <utilities/document/TableElement.hpp>
  #include <utilities/document/Table.hpp>
  #include <utilities/document/Table_Impl.hpp>
%}

#ifdef SWIGRUBY
%alias openstudio::Document::push "<<";
%alias openstudio::Table::appendRow "<<";
#endif

%include <utilities/document/DocumentEnums.hpp>

// from DocumentRegex.hpp
namespace openstudio {
  std::string formatText(const std::string& str,DocumentFormat fmt);
  std::string formatUnderscore(const std::string& str);
  std::string formatSuperAndSubscripts(const std::string& str,DocumentFormat fmt);
}
%include <utilities/document/HeadersAndFooters.hpp>

%define SWIG_DOCUMENTELEMENT(_name)
  %template(_name##Vector) std::vector<openstudio::_name>;
  %template(Optional##_name) boost::optional<openstudio::_name>;
  
  %include <utilities/document/##_name##.hpp>
  
  %extend openstudio::DocumentElement {
    %template(to_##_name) optionalCast<openstudio::##_name>;
  }
%enddef

// only separate b/c of boost::optional<Table> problem.
%define SWIG_TABLE(_name)
  %template(_name##Vector) std::vector<openstudio::_name>;
  
  %include <utilities/document/##_name##.hpp>
  
  %extend openstudio::DocumentElement {
    %template(to_##_name) cast<openstudio::##_name>;
  }
%enddef

%define SWIG_ABSTRACT_DOCUMENTELEMENT(_name)
  %ignore std::vector<openstudio::_name>::vector(size_type);
  %ignore std::vector<openstudio::_name>::resize(size_type);
  SWIG_DOCUMENTELEMENT(_name);
%enddef

SWIG_ABSTRACT_DOCUMENTELEMENT(DocumentElement);
SWIG_DOCUMENTELEMENT(SectionHeading);
SWIG_ABSTRACT_DOCUMENTELEMENT(SectionElement);
SWIG_DOCUMENTELEMENT(Section);
SWIG_DOCUMENTELEMENT(Text);

%ignore openstudio::Table::operator[];

%template(TableRow) std::vector<openstudio::TableElement>;
%include <utilities/document/TableElement.hpp>

// Suppress warnings. In bindings, only std::vector<openstudio::TableElement> 
// version comes through. Should be okay (in Ruby at least) since the implicit 
// casts from all these types to TableElement should let these types be directly 
// pushed onto a TableRow. There is also version with two arguments for 
// streamlining the conversion of strings to TableElements.
%ignore openstudio::Table::appendRow(const std::vector<std::string>& row);
%ignore openstudio::Table::appendRow(const std::vector<double>& row);
%ignore openstudio::Table::appendRow(const std::vector<int>& row);
%ignore openstudio::Table::appendRow(const std::vector<bool>& row);

SWIG_TABLE(Table);

#if defined(SWIGRUBY) || defined(SWIGPYTHON)

namespace openstudio {
  %extend TableElement {
    std::string __str__() {
      std::ostringstream os;
      os << *self;
      return os.str();
    }
  }

  %extend Table {

    std::vector<openstudio::TableElement>& __getitem__(unsigned rowIndex)
    {
      return (*self)[rowIndex];
    }

  }
}
#endif

%template(DocumentVector) std::vector<openstudio::Document>;
%template(OptionalDocument) boost::optional<openstudio::Document>;
  
%include <utilities/document/DocumentEnums.hpp>
%include <utilities/document/Document.hpp>

#endif 
