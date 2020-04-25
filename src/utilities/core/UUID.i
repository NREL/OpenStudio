#ifndef UTILITIES_CORE_UUID_I
#define UTILITIES_CORE_UUID_I

// DLM@200921221: reinclude the ruby undef stuff, ruby define clashes with accept in Qt
%include <utilities/core/CommonInclude.i>
%include <utilities/core/CommonImport.i>

%{
  #include <utilities/core/UUID.hpp>
%}

namespace openstudio{

  #ifdef SWIGRUBY
    %rename("nil?") isNull();
  #endif

  class UUID {
  public:
    bool isNull() const;
    ~UUID();

  protected:
     UUID();
  };

  UUID toUUID(const std::string& str);

  UUID createUUID();

  std::string removeBraces(const UUID& uuid);

  %extend UUID{

    static UUID create(){
      return createUUID();
    }

    std::string __str__() const{
      return openstudio::toString(*self);
    }

    bool operator!= ( const UUID & other ) const {
      return *self != other;
    }

    bool operator< ( const UUID & other ) const {
      return *self < other;
    }

    bool operator== ( const UUID & other ) const {
      return *self == other;
    }

    bool operator> ( const UUID & other ) const {
      return *self > other;
    }
  };

} // openstudio

%template(OptionalUUID) boost::optional<openstudio::UUID>;
%template(UUIDVector) std::vector<openstudio::UUID>;
%template(OptionalUUIDVector) boost::optional<std::vector<openstudio::UUID> >;

#endif //UTILITIES_CORE_UUID_I
