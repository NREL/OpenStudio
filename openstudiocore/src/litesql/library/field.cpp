/* LiteSQL - Syntactic sugar for Expr-API
 * 
 * The list of contributors at http://litesql.sf.net/ 
 * 
 * See LICENSE for copyright information. */
#include <cstdlib>
#include "compatibility.hpp"
#include "litesql.hpp"
#include "litesql/field.hpp"

namespace litesql {
using namespace std;
In FieldType::in(const string& set) const {
    return In(*this, set);
}
In FieldType::in(const SelectQuery& sel) const {
    return In(*this, sel);
}
Like FieldType::like(const string& s) {
    return Like(*this, s);
}
template <> 
string convert<int, string>(int value) {
    return toString(value);
}
template <>
string convert<float, string>(float value) {
    return toString(value);
}
template <> 
string convert<const int&, string>(const int& value) {
    return toString(value);
}
template <>
string convert<const float&, string>(const float& value) {
    return toString(value);
}
template <>
string convert<const bool&, string>(const bool& value) {
    return toString(value);
}

template <>
int convert<const string&, int>(const string& value) {
    return strtol(value.c_str(), NULL, 10);
}
template <>
bool convert<int, bool>(int value) {
    return value!=0;
}
template <> 
float convert<int, float>(int value) {
    return (float)value;
}
template <> 
float convert<double, float>(double value) {
    return (float)value;
}
template <>
bool convert<const string&, bool>(const string& value) {
    return convert<const string&, int>(value);
}
template <>
long long convert<const string&, long long>(const string& value) {
    return strtoll(value.c_str(), NULL, 10);
}
template <>
float convert<const string&, float>(const string& value) {
    return strtof(value.c_str(), NULL);
}
template <>
string convert<const string&, string>(const string& value) {
    return value;
}



const char hexDigits[] = "0123456789abcdef";

string Blob::toHex(void) const
{
  string result;
  if (!m_data) 
  {
    result ="NULL";  
  }
  else
  {
    result.reserve(m_length);
    for (size_t i = 0; i < m_length;i++)
    {
      result.push_back( hexDigits[(m_data[i]&0xf0) >>4]);
      result.push_back( hexDigits[m_data[i]&0x0f]);
    }
  }
  return result;
}

int hex(char c)
{
  switch(c)
  {
    case '0':
      return 0;
    case '1':
      return 1;
    case '2':
      return 2;
    case '3':
      return 3;
    case '4':
      return 4;
    case '5':
      return 5;
    case '6':
      return 6;
    case '7':
      return 7;
    case '8':
      return 8;
    case '9':
      return 9;
    case 'a':
    case 'A':
      return 0xa;
    case 'b':
    case 'B':
      return 0xb;
    case 'c':
    case 'C':
      return 0xc;
    case 'd':
    case 'D':
      return 0xd;
    case 'e':
    case 'E':
      return 0xe;
    case 'f':
    case 'F':
      return 0xf;
    default:
      throw("invalid digit");
  }
}

Blob::~Blob()
{ 
  if (m_data!=NULL) 
    free(m_data); 
}

void Blob::initWithHexString(const string& hexString)
{
  if ("NULL"==hexString)
  {
    m_data = NULL;
    m_length = 0;
  }
  else
  {
    m_length = hexString.size()/2;
    m_data = (u8_t*) malloc(m_length);
    for (size_t i = 0; i < m_length;i++)
    {
      m_data[i] = (hex(hexString[2*i])<<4) | hex(hexString[2*i+1]);
    }
  }
}

void   Blob::data(const char* pszData) 
{
  if (m_data!=NULL)
  {
    free(m_data);
  }
  initWithData( (u8_t*)pszData,
		  pszData!=NULL ? strlen(pszData)+1 : 0 );
}


void Blob::initWithData(const u8_t* data, size_t length)
{
  if (data!=m_data)
  {
    if (m_data!=NULL)
    {
      free(m_data);
    }

    if (data==NULL)
    {
      m_data = NULL;
      m_length = 0;
    }
    else
    {
      m_data = (u8_t*)malloc(length); 
      memcpy(m_data,data,length);
      m_length = length;
    }
  }
}

ostream& operator << (ostream& os, const Blob& blob)
{
  return os << convert<const Blob&, string>(blob);
}


template <>
Blob convert<const string&, Blob>(const string& value) 
{
  if ("NULL"==value)
    return Blob(NULL,0);
  else
    return Blob(value);
}

template <>
string convert<const Blob&, string>(const Blob& value)
{
  if (value.isNull())
  {
    return "NULL";
  }
  else 
  {
    string hexVal;
    hexVal.append("X'");
    hexVal.append(value.toHex());
    hexVal.append("'");
    return hexVal;
  }
}


}
