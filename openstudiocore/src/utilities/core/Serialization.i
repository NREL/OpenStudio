#ifndef UTILITIES_CORE_SERIALIZATION_I
#define UTILITIES_CORE_SERIALIZATION_I

%{
  #include <utilities/core/Serialization.hpp>
%}

#define MAKE_PTR_SERIALIZABLE(__T__) \
virtual std::ostream& toText(std::ostream& os) const; \
virtual std::string toText() const; \
virtual std::ostream& toXml(std::ostream& os) const; \
virtual std::string toXml() const; \
virtual std::ostream& toBinary(std::ostream& os) const; \
virtual std::string toBinary() const; \
static Ptr fromText(std::istream& is); \
static Ptr fromText(const std::string& str); \
static Ptr fromXml(std::istream& is); \
static Ptr fromXml(const std::string& str); \
static Ptr fromBinary(std::istream& is); \
static Ptr fromBinary(const std::string& str);

#define MAKE_POD_SERIALIZABLE(__T__) \
virtual std::ostream& toText(std::ostream& os) const; \
virtual std::string toText() const; \
virtual std::ostream& toXml(std::ostream& os) const; \
virtual std::string toXml() const; \
virtual std::ostream& toBinary(std::ostream& os) const; \
virtual std::string toBinary() const; \
static __T__ fromText(std::istream& is); \
static __T__ fromText(const std::string& str); \
static __T__ fromXml(std::istream& is); \
static __T__ fromXml(const std::string& str); \
static __T__ fromBinary(std::istream& is); \
static __T__ fromBinary(const std::string& str); 

#endif //UTILITIES_CORE_SERIALIZATION_I