/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

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

#include "EnumBase.hpp"

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
#define OPENSTUDIO_ENUM_BUILD_ARRAY_DESCRIPTION(_r, _value, _elem) BOOST_PP_STRINGIZE(_elem)

/**
 * Helper macro used by OPENSTUDIO_ENUM_BUILD_ARRAY to break down the parts to determine if a description exists.
 * Basic Result (if i is 1):
 * \code
 *   OPENSTUDIO_ENUM_BUILD_ARRAY_DESCRIPTION(_elem)
 * \endcode
 */
#define OPENSTUDIO_ENUM_BUILD_ARRAY_PART(_r, _value, _i, _elem) \
  BOOST_PP_EXPR_IF(BOOST_PP_EQUAL(_i, 1), OPENSTUDIO_ENUM_BUILD_ARRAY_DESCRIPTION(_r, _value, _elem))

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
  {_enum_name ::BOOST_PP_SEQ_ELEM(0, _elem) BOOST_PP_COMMA() BOOST_PP_STRINGIZE(BOOST_PP_SEQ_ELEM(0, _elem)) BOOST_PP_COMMA() "" BOOST_PP_SEQ_FOR_EACH_I(OPENSTUDIO_ENUM_BUILD_ARRAY_PART, 0, _elem)} BOOST_PP_COMMA()

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
  BOOST_PP_EXPR_IF(BOOST_PP_EQUAL(_i, 0), _elem)                    \
  BOOST_PP_EXPR_IF(BOOST_PP_EQUAL(_i, 2), = _elem)

/**
 * Helper macro used by OPENSTUDIO_ENUM. Iterates over the parts of an element
 * (Item)(Description)(Value) and calling OPENSTUDIO_ENUM_DOMAIN_ELEM_PART
 * on each element
 * Basic Result:
 *   OPENSTUDIO_ENUM_DOMAIN_ELEM_PART(_elem, 0) OPENSTUDIO_ENUM_DOMAIN_ELEM_PART(_elem, 2) ,
 */
#define OPENSTUDIO_ENUM_DOMAIN_ELEM(_r, _enum_name, _elem)                     \
  BOOST_PP_SEQ_FOR_EACH_I(OPENSTUDIO_ENUM_DOMAIN_ELEM_PART, _enum_name, _elem) \
  BOOST_PP_COMMA()

/** OPENSTUDIO_ENUM main implementation. The possible inputs are broken into 7 sets
 *  due to compiler limitations for how long a macro parameter may be and our use
 *  of extremely long enumerations. See OPENSTUDIO_ENUM comments for detailed usage.
 */
#define OPENSTUDIO_ENUM(_enum_name, _vals)                                                                 \
  class _enum_name : public ::EnumBase<_enum_name>                                                         \
  {                                                                                                        \
   public:                                                                                                 \
    enum domain                                                                                            \
    {                                                                                                      \
      BOOST_PP_SEQ_FOR_EACH(OPENSTUDIO_ENUM_DOMAIN_ELEM, _enum_name, _vals)                                \
    };                                                                                                     \
                                                                                                           \
    _enum_name() : EnumBase<_enum_name>(BOOST_PP_SEQ_ELEM(0, BOOST_PP_SEQ_ELEM(0, _vals))) {}              \
    _enum_name(const std::string& t_name) : EnumBase<_enum_name>(t_name) {}                                \
    _enum_name(int t_value) : EnumBase<_enum_name>(t_value) {}                                             \
    static std::string enumName() {                                                                        \
      return BOOST_PP_STRINGIZE(_enum_name);                                                               \
    }                                                                                                      \
    domain value() const {                                                                                 \
      return static_cast<domain>(EnumBase<_enum_name>::integer_value());                                   \
    }                                                                                                      \
                                                                                                           \
   private:                                                                                                \
    friend class EnumBase<_enum_name>;                                                                     \
    typedef std::pair<std::string, int> PT;                                                                \
    typedef std::vector<PT> VecType;                                                                       \
    static VecType buildStringVec(bool isd) {                                                              \
      struct evalue                                                                                        \
      {                                                                                                    \
        int value;                                                                                         \
        const char* name;                                                                                  \
        const char* description;                                                                           \
      };                                                                                                   \
      const evalue a[] = {BOOST_PP_SEQ_FOR_EACH(OPENSTUDIO_ENUM_BUILD_ARRAY, _enum_name, _vals){0, 0, 0}}; \
                                                                                                           \
      VecType v;                                                                                           \
      int i = 0;                                                                                           \
      while (!(a[i].value == 0 && a[i].name == 0 && a[i].description == 0)) {                              \
        if (isd) {                                                                                         \
          std::string description = a[i].description;                                                      \
          if (!description.empty()) {                                                                      \
            v.push_back(PT(description, a[i].value));                                                      \
          }                                                                                                \
        } else {                                                                                           \
          v.push_back(PT(a[i].name, a[i].value));                                                          \
        }                                                                                                  \
        ++i;                                                                                               \
      }                                                                                                    \
      return v;                                                                                            \
    }                                                                                                      \
  };                                                                                                       \
  inline std::ostream& operator<<(std::ostream& os, const _enum_name& e) {                                 \
    return os << e.valueName() << "(" << e.value() << ")";                                                 \
  }                                                                                                        \
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
 * std::cout << "List of possible values for class " << MyEnumeration::enumName() << ":" << '\n'
 * std::set<int> allValues = MyEnumeration::getValues();
 * for (std::set<int>::const_iterator it = allValues.begin(), itEnd = allValues.end();
 *      it != itEnd; ++it)
 * {
 *   MyEnumeration nextValue(*it);
 *   std::cout << "  " << nextValue << '\n';
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
 *     return static_cast<domain>(EnumBase<MyEnumerationComplex>::integer_value());
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
 * The implementation of OPENSTUDIO_ENUM is heavily dependent on the Boost Preprocessor library.
 * \sa http://www.boost.org/doc/libs/1_40_0/libs/preprocessor/doc/index.html
 */

#endif  // UTILITIES_CORE_ENUM_HPP
