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

#ifndef UTILITIES_CORE_ENUM_HPP
#define UTILITIES_CORE_ENUM_HPP

#include <set>
#include <map>
#include <vector>
#include "StaticInitializer.hpp"
#include <boost/preprocessor.hpp>
#include <boost/optional.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <stdexcept>

/** Base class for OPENSTUDIO_ENUMs. Comparison operators use the underlying (integer) value. */
template<typename Enum>
  class EnumBase : public ::StaticInitializer<Enum>
  {
    public:
      /** Default constructor. EnumBase default constructor never actually called; OPENSTUDIO_ENUM-
       *  generated classes have default constructors that set the value to the first item in the
       *  enumerated list. */
      EnumBase()
      {
        assert(!"This constructor should never get called");
      }

      /** Construct from t_value. Throws std::runtime_error if t_value is not in the domain. */
      EnumBase(int t_value)
        : m_value(lookupValue(t_value))
      {
      }

      /** Construct from t_value. Throws std::runtime_error if t_value does not match a name or 
       *  a description (case insensitive). */
      EnumBase(const std::string &t_value)
        : m_value(lookupValue(t_value))
      {
      }

      /** Returns the name associated with t_value, if it exists. Otherwise, throws 
       *  std::runtime_error. */
      static std::string valueName(int t_value) 
      { 
        const std::map<int, std::string> &m = getNames();
        auto itr = m.find(t_value);
        if (itr == m.end()) 
        { 
          throw std::runtime_error("Invalid domain"); 
        } 
        return itr->second;
      } 

      /** Returns the description associated with t_value, if it exists. Otherwise, throws 
       *  std::runtime_error. */
      static std::string valueDescription(int t_value) 
      {
        const std::map<int, std::string> &m = getDescriptions();
        auto itr = m.find(t_value);
        if (itr == m.end()) 
        { 
          return valueName(t_value); 
        } 
        return itr->second;
      } 

      /** Returns the set of all values in this enum's domain. */
      static const std::set<int> &getValues() 
      { 
        static std::set<int> values(buildValues()); 
        return values; 
      } 

      /** Returns this instance's current value (as an integer). */
      int value() const 
      {  
        return m_value;
      } 

      /** Returns the name associated with this instance's current value. */
      std::string valueName() const 
      { 
        return valueName(m_value); 
      }

      /** Returns the description associated with this instance's current value. */
      std::string valueDescription() const 
      { 
        return valueDescription(m_value); 
      } 

      /** Set this instance's value to t_value. Throws std::runtime_error if 
       *  t_value is not in the domain. */
      void setValue(int t_value)
      {
        m_value = lookupValue(t_value);
      }

      bool operator==(const Enum& other) const { return (m_value==other.m_value);} 
      bool operator!=(const Enum& other) const { return (m_value!=other.m_value);} 
      bool operator>(const Enum& other) const { return (m_value>other.m_value);} 
      bool operator>=(const Enum& other) const { return (m_value>=other.m_value);} 
      bool operator<(const Enum& other) const { return (m_value<other.m_value);} 
      bool operator<=(const Enum& other) const { return (m_value<=other.m_value);} 

      static void initialize() 
      { 
        getValues(); 
        getLookupMap(); 
        getNames();
        getDescriptions();
      } 

      /** Returns the (integer) value associated with t_name, as determined by case-insensitive
       *  comparison to the enumerated names and descriptions. */
      int lookupValue(std::string t_name) 
      { 
        boost::algorithm::to_upper(t_name); 
        const std::map<std::string, int> &names = getLookupMap(); 
        auto itr = names.find(t_name); 
        if (itr != names.end()) 
        { 
          return itr->second; 
        } 
        throw std::runtime_error("Unknown Value"); 
      } 

      /** Returns t_value if it is in the domain. Otherwise throws std::runtime_error. */
      int lookupValue(int t_value) 
      { 
        const std::set<int> &values = getValues(); 
        if (values.count(t_value) != 0) 
        { 
          return t_value; 
        } else { 
          throw std::runtime_error("Unknown value"); 
        } 
      } 

      static const std::map<std::string, int> &getLookupMap() 
      { 
        static const std::map<std::string, int> m = buildLookupMap(); 
        return m; 
      } 

      static const std::map<int, std::string> &getNames()
      {
        static const std::map<int, std::string> names = buildStrings(false);
        return names;
      }

      static const std::map<int, std::string> &getDescriptions()
      {
        static const std::map<int, std::string> descriptions = buildStrings(true);
        return descriptions;
      }

    protected:
      int m_value;

    private:

      static std::map<int, std::string> buildStrings(bool d)
      {
        std::vector<std::pair<std::string, int> > strings = Enum::buildStringVec(d);
        std::map<int, std::string> m;
        for (std::vector<std::pair<std::string, int> >::const_iterator itr = strings.begin();
            itr != strings.end();
            ++itr)
        {
          m[itr->second] = itr->first;
        }

        return m;
      }

      static std::map<std::string, int> buildLookupMap() 
      { 
        std::map<std::string, int> retval;

        const std::map<int, std::string> &m = getNames();
        const std::map<int, std::string> &d = getDescriptions();

        for (const auto & name : m)
        {
          retval[boost::algorithm::to_upper_copy(name.second)] = name.first;
        }

        for (const auto & description : d)
        {
          retval[boost::algorithm::to_upper_copy(description.second)] = description.first;
        }

        return retval;
      } 

      static std::set<int> buildValues()
      {
        const std::map<int, std::string> &names = getNames();

        std::set<int> retvals;

        for (const auto & name : names)
        {
          retvals.insert(name.first);
        }

        return retvals;
      }

  };



/**
 * Helper macro used by OPENSTUDIO_ENUM_BUILD_ARRAY_PART to build the description portion of the array
 * of value / string mappings
 * Basic Result:
 * \code
 *   "String" 
 * \endcode
 *
 * "String" will be the description of the element if it exists
 */
#define OPENSTUDIO_ENUM_BUILD_ARRAY_DESCRIPTION(_r, _value, _elem) \
  BOOST_PP_STRINGIZE(_elem)

/**
 * Helper macro used by OPENSTUDIO_ENUM_BUILD_ARRAY to break down the parts to determine if a description exists.
 * Basic Result (if i is 1):
 * \code
 *   OPENSTUDIO_ENUM_BUILD_ARRAY_DESCRIPTION(_elem)
 * \endcode
 */
#define OPENSTUDIO_ENUM_BUILD_ARRAY_PART(_r, _value, _i, _elem) \
  BOOST_PP_EXPR_IF(BOOST_PP_EQUAL(_i, 1), OPENSTUDIO_ENUM_BUILD_ARRAY_DESCRIPTION(_r, _value, _elem) )

/**
 * Helper macro used by OPENSTUDIO_ENUM to convert a string value into an enumeration domain value.
 * The "" empty string is used in case no description exists. The C compiler has no problem with a string
 * constant like "" "Description" it just concatenates the two strings.
 * 
 * Basic Result:
 * \code
 *   { _enum_name::BOOST_PP_SEQ_ELEM(0, _elem), BOOST_PP_STRINGIZE(BOOST_PP_SEQ_ELEM(0, _elem)), "" OPENSTUDIO_ENUM_BUILD_ARRAY_PART(0, _elem, 1)  },
  OPENSTUDIO_ENUM_BUILD_STRINGS_PART(_enum_name :: BOOST_PP_SEQ_ELEM(0, _elem), _elem, 1)
 * \endcode
 */
#define OPENSTUDIO_ENUM_BUILD_ARRAY(_r, _enum_name, _elem) \
    { _enum_name :: BOOST_PP_SEQ_ELEM(0, _elem) BOOST_PP_COMMA() BOOST_PP_STRINGIZE(BOOST_PP_SEQ_ELEM(0, _elem)) BOOST_PP_COMMA() ""  BOOST_PP_SEQ_FOR_EACH_I(OPENSTUDIO_ENUM_BUILD_ARRAY_PART, 0, _elem)  } BOOST_PP_COMMA()

/**
 * Helper macro used by OPENSTUDIO_ENUM_DOMAIN_ELEM
 * Extracts the first element of a list (Item)(Description)(Value) (that is: Item)
 * verbatim. Extracts the third element (if provided) as: = Value.
 * Basic Result:
 * If i == 0
 * \code
 *   Item
 * \endcode
 * If i == 1
 * \code
 *   = Value
 * \endcode
 *
 * It takes 1 or two calls to piece the parts together. See OPENSTUDIO_ENUM_DOMAIN_ELEM for the
 * macro that calls this macro.
 * \sa OPENSTUDIO_ENUM_DOMAIN_ELEM 
 */
#define OPENSTUDIO_ENUM_DOMAIN_ELEM_PART(_r, _enum_name, _i, _elem) \
    BOOST_PP_EXPR_IF(BOOST_PP_EQUAL(_i, 0), _elem) \
    BOOST_PP_EXPR_IF(BOOST_PP_EQUAL(_i, 2), = _elem)

/**
 * Helper macro used by OPENSTUDIO_ENUM. Iterates over the parts of an element
 * (Item)(Description)(Value) and calling OPENSTUDIO_ENUM_DOMAIN_ELEM_PART
 * on each element
 * Basic Result:
 *   OPENSTUDIO_ENUM_DOMAIN_ELEM_PART(_elem, 0) OPENSTUDIO_ENUM_DOMAIN_ELEM_PART(_elem, 2) , 
 */
#define OPENSTUDIO_ENUM_DOMAIN_ELEM(_r, _enum_name, _elem) \
  BOOST_PP_SEQ_FOR_EACH_I(OPENSTUDIO_ENUM_DOMAIN_ELEM_PART, _enum_name, _elem) \
  BOOST_PP_COMMA() 


/** OPENSTUDIO_ENUM main implementation. The possible inputs are broken into 7 sets 
 *  due to compiler limitations for how long a macro parameter may be and our use
 *  of extremely long enumerations. See OPENSTUDIO_ENUM comments for detailed usage.
 */
#define OPENSTUDIO_ENUM7(_enum_name, _vals, _vals2, _vals3, _vals4, _vals5, _vals6, _vals7) \
class _enum_name : public EnumBase<_enum_name> \
  { \
  public: \
    enum domain { \
       BOOST_PP_SEQ_FOR_EACH(OPENSTUDIO_ENUM_DOMAIN_ELEM, _enum_name, _vals) \
       BOOST_PP_SEQ_FOR_EACH(OPENSTUDIO_ENUM_DOMAIN_ELEM, _enum_name, _vals2) \
       BOOST_PP_SEQ_FOR_EACH(OPENSTUDIO_ENUM_DOMAIN_ELEM, _enum_name, _vals3) \
       BOOST_PP_SEQ_FOR_EACH(OPENSTUDIO_ENUM_DOMAIN_ELEM, _enum_name, _vals4) \
       BOOST_PP_SEQ_FOR_EACH(OPENSTUDIO_ENUM_DOMAIN_ELEM, _enum_name, _vals5) \
       BOOST_PP_SEQ_FOR_EACH(OPENSTUDIO_ENUM_DOMAIN_ELEM, _enum_name, _vals6) \
       BOOST_PP_SEQ_FOR_EACH(OPENSTUDIO_ENUM_DOMAIN_ELEM, _enum_name, _vals7) \
    }; \
    \
    _enum_name() \
      : EnumBase<_enum_name>(BOOST_PP_SEQ_ELEM(0, BOOST_PP_SEQ_ELEM(0, _vals))) \
    { } \
    _enum_name(const std::string &t_name) \
      : EnumBase<_enum_name>(t_name) \
    { } \
    _enum_name(int t_value) \
      : EnumBase<_enum_name>(t_value) \
    { } \
    static std::string enumName() \
    { \
       return BOOST_PP_STRINGIZE(_enum_name); \
    } \
    domain value() const \
    { \
      return static_cast<domain>(EnumBase<_enum_name>::value()); \
    } \
  private: \
    \
    friend class EnumBase<_enum_name>; \
    typedef std::pair<std::string, int> PT; \
    typedef std::vector<PT> VecType; \
    static VecType buildStringVec(bool isd) \
    { \
      struct evalue { \
        int value; \
        const char *name; \
        const char *description; \
      }; \
      const evalue a[] = { \
      BOOST_PP_SEQ_FOR_EACH(OPENSTUDIO_ENUM_BUILD_ARRAY, _enum_name, _vals) \
      BOOST_PP_SEQ_FOR_EACH(OPENSTUDIO_ENUM_BUILD_ARRAY, _enum_name, _vals2) \
      BOOST_PP_SEQ_FOR_EACH(OPENSTUDIO_ENUM_BUILD_ARRAY, _enum_name, _vals3) \
      BOOST_PP_SEQ_FOR_EACH(OPENSTUDIO_ENUM_BUILD_ARRAY, _enum_name, _vals4) \
      BOOST_PP_SEQ_FOR_EACH(OPENSTUDIO_ENUM_BUILD_ARRAY, _enum_name, _vals5) \
      BOOST_PP_SEQ_FOR_EACH(OPENSTUDIO_ENUM_BUILD_ARRAY, _enum_name, _vals6) \
      BOOST_PP_SEQ_FOR_EACH(OPENSTUDIO_ENUM_BUILD_ARRAY, _enum_name, _vals7) \
        {0,0,0} }; \
      \
      VecType v; \
      int i = 0; \
      while (!(a[i].value == 0 && a[i].name == 0 && a[i].description == 0)) \
      { \
        if (isd) { \
          std::string description = a[i].description; \
          if (!description.empty()) { \
            v.push_back(PT(description, a[i].value)); \
          } \
        } else { \
          v.push_back(PT(a[i].name, a[i].value)); \
        } \
        ++i; \
      } \
      return v; \
    } \
  }; \
  inline std::ostream &operator<<(std::ostream &os, const _enum_name &e) { return os << e.valueName() << "(" << e.value() << ")";} \
  typedef boost::optional<_enum_name> Optional##_enum_name; 

/**
 * OPENSTUDIO_ENUM provides a mechanism for defining strongly typed enumeration classes with built-in 
 * string conversion. The first argument to the macro is _enum_name, which becomes the name of a class
 * deriving from \link EnumBase EnumBase<_enum_name> \endlink (whose public member functions are 
 * directly available to the generated class). The second (and final) macro argument is a list of 
 * element names. Each item in that list is itself a list. The simplest example is a list of 
 * elements that only have one piece of information provided:
 * \code
 * OPENSTUDIO_ENUM(MyEnumeration,
 *   ((Item1))
 *   ((Item2))
 *   ((Item3))
 * );
 * \endcode   
 *
 * This example defines a class, MyEnumeration, containing a C++ enum that naturally provides named 
 * integer constants corresponding to each element in the list, for example
 * \code
 * assert(MyEnumeration::Item1 == 0)
 * assert(MyEnumeration::Item2 == 1)
 * assert(MyEnumeration::item3 == 1)
 * \endcode
 * The base class EnumBase<MyEnumeration> provides methods for converting between these integers 
 * and their associated strings, and provides class introspection:
 * \code
 * MyEnumeration myEnumValue("item1");
 * assert(myEnumValue.value() == 0);
 * assert(myEnumValue.valueName() == "Item1");
 * assert(myEnumValue.valueDescription() == "Item1");
 *
 * std::cout << "List of possible values for class " << MyEnumeration::enumName() << ":" << std::endl
 * std::set<int> allValues = MyEnumeration::getValues();
 * for (std::set<int>::const_iterator it = allValues.begin(), itEnd = allValues.end(); 
 *      it != itEnd; ++it) 
 * {
 *   MyEnumeration nextValue(*it);
 *   std::cout << "  " << nextValue << std::endl;
 * }
 * \endcode
 * 
 * To specify non-default description strings and integer values, provide an expanded list for 
 * each enumeration element: 
 * \code
 * OPENSTUDIO_ENUM(MyEnumerationComplex,
 *   ((Item1)(First Item)(-1))
 *   ((Item2)(Second Item))
 *   ((Item3))
 *   ((Item4)(Fourth Item)(100))
 * );
 * \endcode
 * (As shown in the MyEnumeration example, if a description is not provided, the description 
 * string defaults to the element's name.)
 *
 * Note that some compilers do not allow you to leave a blank item in a list, so, if you want to provide
 * a value, you must also provide a description
 * \code
 *   OPENSTUDIO_ENUM(MyEnumeration,
 *     ((Item1)()(-1)) // Don't do this, it won't compile on some systems
 *   );
 * \endcode
 *
 * If a value is not provided, normal compiler rules for how enumeration values are assigned apply. They start
 * at 0, and automatically increment by one for each value not explicitly defined. See your compiler
 * documentation for more details. 
 *
 * Taking our earlier example, "MyEnumerationComplex", the compiler generates the following code,
 * which defines a class in the current namespace called MyEnumerationComplex, with the structure:
 * \code
 * class MyEnumerationComplex : public EnumBase<MyEnumerationComplex> {
 *  public: 
 *   enum domain
 *   {
 *     Item1 = -1 , Item2 , Item3 , Item4 = 100 ,
 *   };
 *
 *   MyEnumerationComplex()
 *    : EnumBase<MyEnumerationComplex>(Item1)
 *   {
 *   }
 *
 *   MyEnumerationComplex(const std::string &t_name)
 *    : EnumBase<MyEnumerationComplex>(t_name)
 *   {
 *   }
 *
 *   MyEnumerationComplex(int t_value)
 *    : EnumBase<MyEnumerationComplex>(t_value)
 *   {
 *   }
 *
 *   static std::string enumName()
 *   {
 *     return "MyEnumerationComplex";
 *   }
 *
 *   domain value() const
 *   {
 *     return static_cast<domain>(EnumBase<MyEnumerationComplex>::value());
 *   }
 *
 *  private:
 *   friend class EnumBase<MyEnumerationComplex>;
 *   typedef std::pair<std::string, int> PT;
 *   typedef std::vector<PT> VecType;
 * 
 *   static VecType buildStringVec(bool isd)
 *   {
 *     struct evalue
 *     {
 *       int value; const char *name; const char *description;
 *     };
 *     const evalue a[] =
 *     {
 *       { MyEnumerationComplex :: Item1 , "Item1" , "" "First Item" },
 *       { MyEnumerationComplex :: Item2 , "Item2" , "" "Second Item" },
 *       { MyEnumerationComplex :: Item3 , "Item3" , "" },
 *       { MyEnumerationComplex :: Item4 , "Item4" , "" "Fourth Item"},
 *       { 0,0,0 }
 *     };
 *
 *     VecType v;
 *     int i = 0;
 *     while (!(a[i].value == 0 && a[i].name == 0 && a[i].description == 0))
 *     {
 *       if (isd)
 *       {
 *         std::string description = a[i].description; 
 *         if (!description.empty())
 *         {
 *           v.push_back(PT(description, a[i].value));
 *         }
 *       } else {
 *         v.push_back(PT(a[i].name, a[i].value));
 *       }
 *       ++i;
 *     }
 *     return v;
 *   }
 *   friend class boost::serialization::access;
 *
 *   template<class Archive> void serialize(Archive & ar, const unsigned int version)
 *   {
 *     ar & boost::serialization::make_nvp("value", m_value);
 *   }
 * };
 *
 * inline std::ostream &operator<<(std::ostream &os, const MyEnumerationComplex &e)
 * {
 *   return os << e.valueName() << "(" << e.value() << ")";
 * }
 *
 * typedef boost::optional<MyEnumerationComplex> OptionalMyEnumerationComplex;
 * \endcode
 * 
 * If the number of elements you want to pass to OPENSTUDIO_ENUM becomes too long (The compiler will let you know)
 * use OPENSTUDIO_ENUM7 instead, which takes 7 sets of elements, allowing for extremely large enumerations.
 *
 * The implementation of OPENSTUDIO_ENUM is heavily dependent on the Boost Preprocessor library.
 * \sa http://www.boost.org/doc/libs/1_40_0/libs/preprocessor/doc/index.html
 */
#define OPENSTUDIO_ENUM(_enum_name, _vals) OPENSTUDIO_ENUM7(_enum_name, _vals, , , , , , )






#endif // UTILITIES_CORE_ENUM_HPP
