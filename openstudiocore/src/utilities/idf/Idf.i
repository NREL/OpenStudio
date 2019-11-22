#ifndef UTILITIES_IDF_I
#define UTILITIES_IDF_I

%{
  #include <utilities/idf/Handle.hpp>
  #include <utilities/idf/IdfExtensibleGroup.hpp>
  #include <utilities/idf/IdfObject.hpp>
  #include <utilities/idf/IdfObjectWatcher.hpp>
  #include <utilities/idf/IdfFile.hpp>
  #include <utilities/idf/ImfFile.hpp>
  #include <utilities/idf/Workspace.hpp>
  #include <utilities/idf/Workspace_Impl.hpp>
  #include <utilities/idf/WorkspaceWatcher.hpp>
  #include <utilities/idf/WorkspaceExtensibleGroup.hpp>
  #include <utilities/idf/WorkspaceObject.hpp>
  #include <utilities/idf/WorkspaceObjectOrder.hpp>
  #include <utilities/idf/WorkspaceObjectWatcher.hpp>
  #include <utilities/idf/ValidityEnums.hpp>
  #include <utilities/idf/ValidityReport.hpp>
  #include <utilities/idf/DataError.hpp>
  #include <utilities/idf/URLSearchPath.hpp>

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
  #include <utilities/units/OSOptionalQuantity.hpp>

  // to be ignored
  class QAction;
  class QDomNode;
  class QDomElement;
  class QDomDocument;
  class QNetworkAccessManager;
  class QThread;
  class QWidget;
  class QDialog;
  class QComboBox;
  class QMainWindow;
  namespace openstudio{
    class RemoteBCL;
    class UpdateManager;
    class PathWatcher;
  }
%}

// it seems that SWIG tries to create conversions of QObjects to these
%ignore QAction;
%ignore QDomNode;
%ignore QDomElement;
%ignore QDomDocument;
%ignore QNetworkAccessManager;
%ignore QThread;
%ignore QWidget;
%ignore QDialog;
%ignore QComboBox;
%ignore QMainWindow;
%ignore openstudio::PathWatcher;
%ignore openstudio::RemoteBCL;
%ignore openstudio::UpdateManager;

// no default constructors
%ignore std::vector<openstudio::IdfObject>::vector(size_type);
%ignore std::vector<openstudio::WorkspaceObject>::vector(size_type);
%ignore std::vector<openstudio::WorkspaceObjectOrder>::vector(size_type);
%ignore std::vector<openstudio::IdfExtensibleGroup>::vector(size_type);

%ignore std::vector<openstudio::IdfObject>::resize(size_type);
%ignore std::vector<openstudio::WorkspaceObject>::resize(size_type);
%ignore std::vector<openstudio::WorkspaceObjectOrder>::resize(size_type);
%ignore std::vector<openstudio::IdfExtensibleGroup>::resize(size_type);

// create an instantiation of the optional classes
%template(OptionalIdfObject) boost::optional<openstudio::IdfObject>;
%template(OptionalIdfFile) boost::optional<openstudio::IdfFile>;
%template(OptionalImfFile) boost::optional<openstudio::ImfFile>;
%template(OptionalWorkspace) boost::optional<openstudio::Workspace>;
%template(OptionalWorkspaceObject) boost::optional<openstudio::WorkspaceObject>;
%template(OptionalWorkspaceExtensibleGroup) boost::optional<openstudio::WorkspaceExtensibleGroup>;
%template(OptionalDataError) boost::optional<openstudio::DataError>;

// create an instantiation of the vector classes
%template(IdfObjectVector) std::vector<openstudio::IdfObject>;
%template(IdfExtensibleGroupVector) std::vector<openstudio::IdfExtensibleGroup>;
%template(WorkspaceObjectVector) std::vector<openstudio::WorkspaceObject>;
%template(WorkspaceObjectVectorVector) std::vector<std::vector<openstudio::WorkspaceObject> >;

// ignore detail namespace
%ignore openstudio::detail;

// ignore functions taking streams that were not previously already ignored on a global scale
%ignore openstudio::IdfFile::load(std::istream&);
%ignore openstudio::IdfFile::load(std::istream&, IddFileType);
%ignore openstudio::IdfFile::load(std::istream&, const IddFile&);

#if defined(SWIGRUBY)
  // add mixins
  %mixin openstudio::IdfObject "Comparable, Marshal";

  // alias methods

  // for Array.uniq
  %alias openstudio::IdfObject::operator== "eql?";
#endif

#if defined(SWIGCSHARP)
  %typemap(csclassmodifiers) openstudio::Workspace "public partial class"
  %typemap(csclassmodifiers) openstudio::WorkspaceObject "public partial class"
  %typemap(csclassmodifiers) openstudio::WorkspaceExtensibleGroup "public partial class"
  %typemap(csclassmodifiers) openstudio::IdfObject "public partial class"
  %typemap(csclassmodifiers) openstudio::IdfExtensibleGroup "public partial class"
#endif

#if defined(SWIGJAVA)
  %ignore openstudio::Workspace::load;
#endif

%include <utilities/idf/Handle.hpp>
%include <utilities/idf/ValidityEnums.hpp>
%include <utilities/idf/DataError.hpp>
%include <utilities/idf/ValidityReport.hpp>
%include <utilities/idf/IdfObject.hpp>

%template(QUrlPathPair) std::pair<QUrl, openstudio::path>;
%template(QUrlPathPairVector) std::vector<std::pair<QUrl, openstudio::path> >;

// template non-member function for extracting handles from objects
%template(getIdfObjectHandles) openstudio::getHandles<openstudio::IdfObject>;
%template(getWorkspaceObjectHandles) openstudio::getHandles<openstudio::WorkspaceObject>;

%template(HandleSet) std::set<openstudio::UUID>;
%template(HandleMap) std::map<openstudio::UUID,openstudio::UUID>;
//%ignore openstudio::applyHandleMap;

%ignore std::vector<openstudio::URLSearchPath>::vector(size_type);
%ignore std::vector<openstudio::URLSearchPath>::resize(size_type);
%template(URLSearchPathVector) std::vector<openstudio::URLSearchPath>;
%template(OptionalURLSearchPath) boost::optional<openstudio::URLSearchPath>;
%include <utilities/idf/URLSearchPath.hpp>

%include <utilities/idf/IdfExtensibleGroup.hpp>
%include <utilities/idf/ImfFile.hpp>
%include <utilities/idf/IdfFile.hpp>
%include <utilities/idf/ObjectOrderBase.hpp>
%include <utilities/idf/WorkspaceObjectOrder.hpp>
%include <utilities/idf/WorkspaceExtensibleGroup.hpp>
%include <utilities/idf/WorkspaceObject.hpp>
%include <utilities/idf/Workspace.hpp>

%feature("director") IdfObjectWatcher;
%include <utilities/idf/IdfObjectWatcher.hpp>

%feature("director") WorkspaceObjectWatcher;
%include <utilities/idf/WorkspaceObjectWatcher.hpp>

%feature("director") WorkspaceWatcher;
%include <utilities/idf/WorkspaceWatcher.hpp>

%extend openstudio::IdfObject{
  std::string __str__() const {
    std::ostringstream os;
    os << *self;
    return os.str();
  }

  #if defined(SWIGRUBY)

  // for Comparable mixin
  int __cmp__(const openstudio::IdfObject& other) const {
    if ((*self) == other){
      return 0;
    }

    if (self->iddObject() == other.iddObject()){
      // sort by name
      openstudio::IdfObjectNameLess tester;
      if (tester.operator()(*self, other)){
        return -1;
      }else{
        return 1;
      }
    }

    // sort by type
    openstudio::IdfObjectTypeLess tester;
    if (tester.operator()(*self, other)){
      return -1;
    }
    return 1;
  }

  // for Array.uniq
  int __hash__() const {
    std::string uuid(openstudio::toString(self->handle()));
    return qChecksum(uuid.c_str(), uuid.size());
  }

  // for Marshal mixin
  std::string _dump(int) const {
    std::ostringstream os;
    os << *self;
    return os.str();
  }

  // for Marshal mixin
  static openstudio::IdfObject _load(const std::string& text) {
    boost::optional<openstudio::IdfObject> result = openstudio::IdfObject::load(text);
    if (!result){
      throw openstudio::Exception("Cannot load IdfObject from string '" + text + "'");
    }
    return result.get();
  }

  #endif

};

%extend openstudio::IdfFile{
  std::string __str__() {
    std::ostringstream os;
    os << *self;
    return os.str();
  }
  static boost::optional<openstudio::IdfFile> load(const std::string& s, const openstudio::IddFileType& iddFileType){
    std::stringstream ss;
    ss << s;
    return openstudio::IdfFile::load(ss, iddFileType);
  }
};

%extend openstudio::ImfFile{
  std::string __str__() {
    std::ostringstream os;
    os << *self;
    return os.str();
  }
};

// DLM: must override this method from IdfObject as there is a different
// operator<< for WorkspaceObject than for IdfObject
%extend openstudio::WorkspaceObject{
  std::string __str__() {
    std::ostringstream os;
    os << *self;
    return os.str();
  }
};

%extend openstudio::Workspace{
  std::string __str__() {
    std::ostringstream os;
    os << *self;
    return os.str();
  }
};

%extend openstudio::IdfExtensibleGroup {
  %template(to_WorkspaceExtensibleGroup) optionalCast<openstudio::WorkspaceExtensibleGroup>;
}

%extend openstudio::DataError {
  std::string __str__() {
    std::ostringstream os;
    os << *self;
    return os.str();
  }
};

%extend openstudio::ValidityReport {
  std::string __str__() {
    std::ostringstream os;
    os << *self;
    return os.str();
  }
};

#endif // UTILITIES_IDF_I
