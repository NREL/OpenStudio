#ifndef UTILITIES_V8_LANGUAGESPECIFIC_I
#define UTILITIES_V8_LANGUAGESPECIFIC_I

#ifdef BUILD_NODE_MODULE
//%include <node.i>
#endif

%header %{

// The ruby SWIG wrappers have lots of unused variables which makes GCC rather chatty.
// This cleans it up.
#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

%}

namespace std {
    template<class T>
    class set {
    };
}

// boost::shared_ptr wrapper, the current macros in shared_ptr.i are not defined for  v8
// this has to be missing a lot but seems to work
%{
  #include <boost/smart_ptr.hpp>
%}
namespace boost {

    template <class T>
    class shared_ptr {
    };


};

// std::shared_ptr wrapper, the current macros in shared_ptr.i are not defined for v8
// minimal implementation to get it compiling.
namespace std {

    template <class T>
    class shared_ptr {
    };
}
 
// Provide some basic operator renaming for V8 to make sure that the
// functions are at least available.
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
%rename(Equals) operator ==;
%rename(NotEqual) operator !=;
%rename(LessThan) operator <;
%rename(LessThanEqual) operator <=;
%rename(GreaterThan) operator >;
%rename(GreaterThanEqual) operator >=;
%rename(LeftShift) operator <<;
%rename(RightShift) operator >>;
%rename(PlusPlusPrefix) operator++();
%rename(PlusPlusPostfix) operator++(int);
%rename(MinusMinusPrefix) operator--();
%rename(MinusMinusPostfix) operator--(int);




#endif // UTILITIES_V8_LANGUAGESPECIFIC_I
