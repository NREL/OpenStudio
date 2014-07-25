#ifndef UTILITIES_CSHARP_LANGUAGESPECIFIC_I
#define UTILITIES_CSHARP_LANGUAGESPECIFIC_I

%include <wchar.i>
%include <cwstring.i>
%include <std_wstring.i>

%rename(Path) openstudio::path; // For consistancy with other class names and to avoid conflicts
%rename(ZeroException) Exception;
%rename(ReportingFrequencyEnum) ReportingFrequency;
%ignore openstudio::SqlFile::dataDictionary();
%rename(SolarSystemModel) SolarSystem;

//Simple support for std::set

%{
#include<set>
%}

namespace std {

    template <typename Key, typename Compare = std::less<Key> >
    class set {
      public:
        set();
        void erase(const Key &);
        size_t count(const Key& );
        bool empty() const;
        void clear();
        size_t size() const;
        size_t max_size() const;

        %extend {
            bool insert(const Key& t) {
              return self->insert(t).second;
            }

            bool contains(const Key& t) {
              return self->find(t) != self->end();
            }

            std::vector<Key> asVector() {
              return std::vector<Key>(self->begin(), self->end());
            }
        }

    };
}

// Provide some basic operator renaming for C# to make sure that the
// functions are at least available.
// Operator overloading does exist in C#, but it is not automatically supported
// like it is for some other languages using "magic names"
// http://www.swig.org/Doc1.3/Ruby.html#Ruby_nn54
// For each individual implementation that you want to support the operator overload
// for C# you will need to implement the operator overload using the 
// %typemap(cscode) directive. See:
// http://swig.svn.sourceforge.net/viewvc/swig/trunk/Examples/test-suite/operator_overload.i?revision=10885&view=markup
%rename(Equal) operator =;
%rename(PlusEqual) operator +=;
%rename(MinusEqual) operator -=;
%rename(MultiplyEqual) operator *=;
%rename(DivideEqual) operator /=;
%rename(PercentEqual) operator %=;
%rename(Plus) operator +;
%rename(Minus) operator -;
%rename(Multiply) operator *;
%rename(Divide) operator /;
%rename(Percent) operator %;
%rename(Not) operator !;
%rename(IndexIntoConst) operator[](unsigned idx) const;
%rename(IndexInto) operator[](unsigned idx);
%rename(Functor) operator ();
%rename(EqualEqual) operator ==;
%rename(NotEqual) operator !=;
%rename(LessThan) operator <;
%rename(LessThanEqual) operator <=;
%rename(GreaterThan) operator >;
%rename(GreaterThanEqual) operator >=;
%rename(LeftShift) operator <<;
%rename(RightShift) operator >>;
%rename(And) operator &&;
%rename(Or) operator ||;
%rename(PlusPlusPrefix) operator++();
%rename(PlusPlusPostfix) operator++(int);
%rename(MinusMinusPrefix) operator--();
%rename(MinusMinusPostfix) operator--(int);



%include <std_shared_ptr.i>
%include <boost_shared_ptr.i>

#endif 
