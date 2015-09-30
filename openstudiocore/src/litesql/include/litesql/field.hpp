/* LiteSQL 
 * 
 * The list of contributors at http://litesql.sf.net/ 
 * 
 * See LICENSE for copyright information. */

#ifndef litesql_field_hpp
#define litesql_field_hpp
#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include "litesql/string.hpp"

/** \file field.hpp
    contains FieldType- and Field-classes */
namespace litesql {
using namespace std;

/** holds field name, type and table information */
class In;
class Like;
class SelectQuery;

class FieldType {
    string _name, _type, _table;

protected:
    typedef vector< pair<string, string> > Values;
private:
    Values _values;
public:

	FieldType() {};
	
    FieldType(const string& n, 
              const string& t, 
              const string& tbl,
              const Values& vals = Values())
        : _name(n), _type(t), _table(tbl), _values(vals) {}
    string fullName() const { return  table() + "." + name(); }
    string name() const { return _name; }
    string type() const { return _type; }
    string table() const { return _table; }
    vector< pair<string, string> > values() { return _values; }
    /** syntactic sugar to Expr-API, Object::field_.in(set) */
    In in(const string& set) const;
    /** syntactic sugar to Expr-API, Object::field_.in(sel) */
    In in(const SelectQuery& sel) const;
    /** syntactic sugar to Expr-API, Object::field_.like(s) */
    Like like(const string& s);
    bool operator==(const FieldType & fd) const {
        return fd.fullName() == fullName();
    }
    bool operator!=(const FieldType & fd) const {
        return ! (*this == fd);
    }
};

/** convert function */
template <class From, class To>
To convert(From value);

/** store function */
template <class T>
std::string store(const T& value) {
    return litesql::toString(value);
}

template <class T>
T load(const std::string& value) {
    return convert<const std::string&, T>(value);
}
/** holds field value */
template <class T>
class Field {
    const FieldType * field; 
    bool _modified;
    T _value;
public:
    Field(const FieldType & f) : field(&f), _modified(true) {}
    string fullName() const { return field->fullName(); }   
    string name() const { return field->name(); }
    string type() const { return field->type(); }
    string table() const { return field->table(); }
    T value() const { return _value; }
    const FieldType & fieldType() const { return *field; } 
    bool modified() const { return _modified; }
    void setModified(bool state) { _modified = state; }
    const Field & operator=(const string& v) { 
        _value = convert<const string&, T>(v);
        _modified = true;
        return *this;
    }
    const Field & operator=(const T& v) {
        _value = v;
        _modified = true;
        return *this;
    }
    template <class T2>
    const Field & operator=(T2 v) { 
        _modified = true;
        _value = litesql::convert<T2, T>(v); 
        return *this;
    }
    template <class T2>
    bool operator==(const T2& v) const {
        return litesql::toString(_value) == litesql::toString(v);
    }
    template <class T2>
    bool operator!=(const T2& v) const { return !(*this == v); }
    

    operator string() const { return toString(value()); }

    operator T() const { return value(); }
};

template <>
class Field<string> {
    const FieldType * field; 
    bool _modified;
    string _value;
public:
    Field(const FieldType & f) : field(&f), _modified(true) {}
    string fullName() const { return field->fullName(); }   
    string name() const { return field->name(); }
    string type() const { return field->type(); }
    string table() const { return field->table(); }
    string value() const { return _value; }
    const FieldType & fieldType() const { return *field; } 
    bool modified() const { return _modified; }
    void setModified(bool state) { _modified = state; }
    const Field & operator=(string v) { 
        _value = v;
        _modified = true;
        return *this;
    }
    const Field& operator=(const char * v) {
        _value = v;
        _modified = true;
        return *this;
    }
    template <class T2>
    const Field & operator=(T2 v) { 
        _modified = true;
        _value = litesql::convert<T2, string>(v); 
        return *this;
    }
    template <class T2>
    bool operator==(const T2& v) const {
        return litesql::toString(_value) == litesql::toString(v);
    }
    template <class T2>
    bool operator!=(const T2& v) const { return !(*this == v); }

    operator string() const { return value(); }
};

typedef unsigned char u8_t;

class Blob {
public:
  Blob()                            : m_data(NULL),m_length(0)               {};
  Blob(const string & value) : m_data(NULL),m_length(0)
  {
    initWithHexString(value);
  };


  Blob(const Blob & b) : m_data(NULL)
  { 
    initWithData(b.m_data,b.m_length);
  };

  Blob(const void* data, size_t length) : m_data(NULL), m_length(0)
  {
    initWithData((u8_t*)data,length);
  };
  
  virtual ~Blob();
  const Blob& operator=(const Blob& v) {
    initWithData(v.m_data,v.m_length);
    return *this;
  }

  string toHex()            const ;
  size_t length()           const  { return m_length;      };
  bool   isNull()           const  { return m_data==NULL;  }; 
  u8_t   data(size_t index) const  { return m_data[index]; };
  void   data(const char* pszData);
private:
  u8_t* m_data;
  size_t m_length;

  void initWithData(const u8_t* data, size_t length);
  void initWithHexString(const string& hexString);
};

ostream& operator << (ostream& os, const Blob& blob);
template <>
Blob convert<const string&, Blob>(const string& value);
template <>
string convert<const Blob&, string>(const Blob& value);

template <>
class Field<Blob> {
    const FieldType * field; 
    bool _modified;
    Blob _value;
public:
    Field(const FieldType & f) : field(&f), _modified(true) {}
    string fullName() const { return field->fullName(); }   
    string name() const { return field->name(); }
    string type() const { return field->type(); }
    string table() const { return field->table(); }
    Blob value() const { return _value; }
    const FieldType & fieldType() const { return *field; } 
    bool modified() const { return _modified; }
    void setModified(bool state) { _modified = state; }
    const Field & operator=(const Blob& v) { 
        _value = v;
        _modified = true;
        return *this;
    }

const Field & operator=(int v) { 
        _value.data(NULL);
        _modified = true;
        return *this;
    }

/*
const Field& operator=(const char * v) {
        _value = v;
        _modified = true;
        return *this;
    }
    template <class T2>
    const Field & operator=(T2 v) { 
        _modified = true;
        _value = litesql::convert<T2, Blob>(v); 
        return *this;
    }
    template <class T2>
    bool operator==(const T2& v) const {
        return litesql::toString(_value) == litesql::toString(v);
    }
    template <class T2>
    bool operator!=(const T2& v) const { return !(*this == v); }
*/

    operator string() const { return _value.toHex(); }
};

template <class T>
std::string operator+(std::string a, litesql::Field<T> f) {
    return a + std::string(f);
}
template <class T>
std::string operator+(litesql::Field<T> f, std::string a) {
    return std::string(f) + a;    
}
template <class T>
std::ostream & operator << (std::ostream & os, const litesql::Field<T> & f) {
    return os << f.value();
}

}
#endif
