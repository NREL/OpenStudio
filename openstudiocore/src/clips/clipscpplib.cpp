#include <iostream>
#include <stdexcept>

#include "clipscpp.h"

using namespace CLIPS;
using std::vector;
using std::string;

#ifndef CLIPS_WIN32_DLL

#if CLIPS_COMPILED_AS_C_PLUS_PLUS
#include "clips.h"
#else
extern "C"
  {
   #include "clips.h"
  }
#endif

#else

#include <windows.h>
extern "C"
{
#include "setup.h"
#include "evaluatn.h"
#include "multifld.h"
#include "factmngr.h"
#include "inscom.h"
#include "CLIPSWin32.h"
}
#endif

/*##################*/
/* Static Functions */
/*##################*/

#ifndef CLIPS_WIN32_DLL
static int CLIPSCPPQuery(void *,char *);
static int CLIPSCPPPrint(void *,char *,char *);
static int CLIPSCPPGetc(void *,char *);
static int CLIPSCPPUngetc(void *,int,char *);
static int CLIPSCPPExit(void *,int);
#endif
static Value *ConvertSingleFieldValue(void *,int,void *);
static DataObject ConvertDataObject(void *,DATA_OBJECT *);

/*#####################*/
/* CLIPSCPPEnv Methods */
/*#####################*/

/***************/
/* CLIPSCPPEnv */
/***************/
CLIPSCPPEnv::CLIPSCPPEnv(void *t_userEnv) : theEnv(NULL), userEnv(t_userEnv)
  {
#ifndef CLIPS_WIN32_DLL
   theEnv = CreateEnvironment();

   SetEnvironmentContext(theEnv,this);
#else
   theEnv = __CreateEnvironment();
   /* TBD */
#endif
  }

/***************/
/* getUserEnv  */
/***************/
void *CLIPSCPPEnv::getUserEnv() const
{
  return userEnv;
}

/****************/
/* ~CLIPSCPPEnv */
/****************/
CLIPSCPPEnv::~CLIPSCPPEnv()
  {
#ifndef CLIPS_WIN32_DLL
   DestroyEnvironment(theEnv);
#else
   __DestroyEnvironment(theEnv);
#endif
  }

/*********/
/* Clear */
/*********/
void CLIPSCPPEnv::Clear()
  {
#ifndef CLIPS_WIN32_DLL
   EnvClear(theEnv);
#else
   __EnvClear(theEnv);
#endif
  }

/********/
/* Load */
/********/
int CLIPSCPPEnv::Load(
  char *theFile)
  {
#ifndef CLIPS_WIN32_DLL
   return EnvLoad(theEnv,theFile);
#else
   return __EnvLoad(theEnv,theFile);
#endif
  }

/*********/
/* Reset */
/*********/
void CLIPSCPPEnv::Reset()
  {
#ifndef CLIPS_WIN32_DLL
   EnvReset(theEnv);
#else
   __EnvReset(theEnv);
#endif
  }

/*******/
/* Run */
/*******/
long long CLIPSCPPEnv::Run(
  long long runLimit)
  {
#ifndef CLIPS_WIN32_DLL
   return EnvRun(theEnv,runLimit);
#else
   return __EnvRun(theEnv,runLimit);
#endif
  }

/*********/
/* Build */
/*********/
bool CLIPSCPPEnv::Build(
  char *buildString)
  {   
#ifndef CLIPS_WIN32_DLL
   if (EnvBuild(theEnv,buildString))
     { return true; }
   else
     { return false; }
#else
   if (__EnvBuild(theEnv,buildString))
     { return true; }
   else
     { return false; }
#endif
  }
  
/********/
/* Eval */
/********/
DataObject CLIPSCPPEnv::Eval(
  char *evalString)
  {
   int rc;
   DATA_OBJECT rv;
   
#ifndef CLIPS_WIN32_DLL
   rc = EnvEval(theEnv,evalString,&rv);
#else
   rc = __EnvEval(theEnv,evalString,&rv);
#endif

   if (rc == 0)
     {
      std::string excStr = "Eval: Invalid expression ";
      excStr.append(evalString);
      throw std::logic_error(excStr); 
     }
     
   return ConvertDataObject(theEnv,&rv);
  }
  
/*********************/
/* ConvertDataObject */
/*********************/
static DataObject ConvertDataObject(
  void *theEnv,
  DATA_OBJECT *theDO)
  {
   DataObject tv;
   
   switch (GetpType(theDO))
     {
      case RVOID:
      case STRING:        
      case SYMBOL:
      case INSTANCE_NAME:
      case INTEGER:
      case FLOAT:
      case FACT_ADDRESS:
      case INSTANCE_ADDRESS:
        return DataObject(ConvertSingleFieldValue(theEnv,GetpType(theDO),GetpValue(theDO)));
     
      case MULTIFIELD:
        struct multifield *theList = (struct multifield *) DOPToPointer(theDO);
        size_t mfLength = GetpDOLength(theDO), i; 
        
        MultifieldValue *theMultifield = new MultifieldValue(mfLength);
        
        for (i = (unsigned) GetpDOBegin(theDO); i <= (unsigned) GetpDOEnd(theDO); i++)
         { theMultifield->add(ConvertSingleFieldValue(theEnv,GetMFType(theList,i),GetMFValue(theList,i))); }

        return DataObject(theMultifield);
     }
     
   return DataObject();
  }
  
/****************************/
/* ConvertSingleFieldValue: */
/****************************/
static Value *ConvertSingleFieldValue(
  void *theEnv,
  int type,
  void  *value)
  {
   switch(type)
     {
      case RVOID:
        return new VoidValue();

      case SYMBOL:
        return new SymbolValue(ValueToString(value));
        
      case STRING:
        return new StringValue(ValueToString(value));
        
      case INSTANCE_NAME:
        return new InstanceNameValue(ValueToString(value));

      case INTEGER:
        return new IntegerValue(ValueToLong(value));

      case FLOAT:
        return new FloatValue(ValueToDouble(value));

      case FACT_ADDRESS:
        return new FactAddressValue(theEnv,value);

      case INSTANCE_ADDRESS:
        return new InstanceAddressValue(theEnv,value);
     }

   return new VoidValue();
  }
  
#ifndef CLIPS_WIN32_DLL

/*********/
/* Watch */
/*********/
int CLIPSCPPEnv::Watch(
  char *item)
  {
   return EnvWatch(theEnv,item);
  }

/***********/
/* Unwatch */
/***********/
int CLIPSCPPEnv::Unwatch(
  char *item)
  {
   return EnvUnwatch(theEnv,item);
  }
  
/*************/
/* AddRouter */
/*************/
int CLIPSCPPEnv::AddRouter(
  char *routerName,
  int priority,
  CLIPSCPPRouter *router)
  {
   return EnvAddRouterWithContext(theEnv,routerName,priority,CLIPSCPPQuery,
                                  CLIPSCPPPrint,CLIPSCPPGetc,CLIPSCPPUngetc,
                                  CLIPSCPPExit,router);
  }
  
/*########################*/
/* CLIPSCPPRouter Methods */
/*########################*/

/*********/
/* Query */
/*********/
int CLIPSCPPRouter::Query(
  CLIPSCPPEnv *theCPPEnv,
  char *logicalName)
  { 
   return FALSE;
  }
  
/*********/
/* Print */
/*********/
int CLIPSCPPRouter::Print(
  CLIPSCPPEnv *theCPPEnv,
  char *logicalName,
  char *printString)
  {
   return FALSE;
  }
  
/********/
/* Getc */
/********/
int CLIPSCPPRouter::Getc(
  CLIPSCPPEnv *theCPPEnv,
  char *logicalName)
  {
   return -1;
  }
  
/**********/
/* Ungetc */
/**********/
int CLIPSCPPRouter::Ungetc(
  CLIPSCPPEnv *theCPPEnv,
  int character,
  char *logicalName)
  {
   return -1;
  }

/********/
/* Exit */
/********/
int CLIPSCPPRouter::Exit(
  CLIPSCPPEnv *theCPPEnv,
  int exitCode)
  {
   return FALSE;
  }

#endif

/*####################*/
/* DataObject Methods */
/*####################*/

/**************/
/* DataObject */
/**************/
DataObject::DataObject() : theValue (new VoidValue())
  {
  }

/**************/
/* DataObject */
/**************/
DataObject::DataObject(Value * v) : theValue (v)
  { }

/**************/
/* DataObject */
/**************/
DataObject::DataObject(const DataObject& v) : theValue(NULL)
  { 
   theValue = v.theValue->clone();
  }
  
/***************/
/* ~DataObject */
/***************/
DataObject::~DataObject()
  { 
   delete theValue; 
  }

/**************/
/* Operator = */
/**************/
DataObject& DataObject::operator = (
  const DataObject& s)
  {
   if (this == &s) return *this;
   
   delete theValue;
   
   theValue = s.theValue->clone();

   return *this;
  }
  
/***************/
/* Operator << */
/***************/
std::ostream& CLIPS::operator<< (std::ostream& o, const DataObject& s)
  {
   s.print(o);
   return o;
  }

/***************/
/* Operator << */
/***************/
std::ostream& CLIPS::operator<< (std::ostream& o, const DataObject* s)
  {
   s->print(o);
   return o;
  }

/*********/
/* print */
/*********/
std::ostream& DataObject::print (std::ostream& o) const
  { return std::cout << theValue; }

/***************/
/* GetFactSlot */
/***************/
DataObject DataObject::GetFactSlot (char *slotName) const
  { return theValue->GetFactSlot(slotName); }

/*###############*/
/* Value Methods */
/*###############*/
    
/*********/
/* Value */
/*********/
Value::Value()
  {
  }

/**********/
/* ~Value */
/**********/
Value::~Value()
  {  
  }

/****************/
/* GetFactIndex */
/****************/
long long Value::GetFactIndex() const
  {  
   throw std::logic_error("GetFactIndex: DataObject is not type FACT ADDRESS");
  }

/***************/
/* GetFactSlot */
/***************/
DataObject Value::GetFactSlot(char *) const
  {  
   throw std::logic_error("GetFactSlot: DataObject is not type FACT ADDRESS");
  }

/**********************/
/* GetRawInstanceName */
/**********************/
char *Value::GetRawInstanceName() const
  {  
   throw std::logic_error("DataObject is not type INSTANCE ADDRESS");
  }
  
/***********/
/* Value = */
/***********/
/*
Value& Value::operator = (
  const Value& v)
  {
   return *this;
  }
*/  
/***************/
/* Operator << */
/***************/
std::ostream& CLIPS::operator<< (std::ostream& o, const Value& s)
  {
   s.print(o);
   return o;
  }

/***************/
/* Operator << */
/***************/
std::ostream& CLIPS::operator<< (std::ostream& o, const Value* s)
  {
   s->print(o);
   return o;
  }

/*###################*/
/* VoidValue Methods */
/*###################*/
    
/*************/
/* VoidValue */
/*************/
VoidValue::VoidValue()
  {
  }

/*************/
/* VoidValue */
/*************/
VoidValue::VoidValue(const VoidValue& v)
  { 
   this->operator=(v); 
  }
  
/**************/
/* ~VoidValue */
/**************/
VoidValue::~VoidValue()
  { 
  }

/***************/
/* VoidValue = */
/***************/
/*
VoidValue& VoidValue::operator = (
  const VoidValue& v)
  {
   return *this;
  }
*/
/*********/
/* print */
/*********/
std::ostream& VoidValue::print (std::ostream& o) const
  {
   return o << "<void>";
  }
  
/*********/
/* clone */
/*********/
VoidValue *VoidValue::clone() const
  { 
   return new VoidValue(*this); 
  }
  
/*#####################*/
/* StringValue Methods */
/*#####################*/
    
/***************/
/* StringValue */
/***************/
StringValue::StringValue()
  { 
  }

/***************/
/* StringValue */
/***************/
StringValue::StringValue(
  char *initialString)
  {
   theString.assign(initialString);
  }

/***************/
/* StringValue */
/***************/
StringValue::StringValue( const StringValue& v)
  { 
   this->operator=(v); 
  }
  
/****************/
/* ~StringValue */
/****************/
StringValue::~StringValue()
  { 
  }

/*****************/
/* StringValue = */
/*****************/
/*
StringValue& StringValue::operator = (
  const StringValue& v)
  {
   if (this == &v) return *this;
   
   theString = v.theString; 

   return *this;
  }
*/
/*********/
/* print */
/*********/
std::ostream& StringValue::print (std::ostream& o) const
  { 
   return o << '\"' << theString.c_str() << '\"'; 
  }
  
/*********/
/* clone */
/*********/
StringValue *StringValue::clone() const
  {
   return new StringValue(*this); 
  }

/*#####################*/
/* SymbolValue Methods */
/*#####################*/

/***************/
/* SymbolValue */
/***************/
SymbolValue::SymbolValue()
  {
   /* theString = new string(""); */
  }

/***************/
/* SymbolValue */
/***************/
SymbolValue::SymbolValue(
  char *initialString)
  {
   theString.assign(initialString);
  }

/***************/
/* SymbolValue */
/***************/
SymbolValue::SymbolValue( const SymbolValue& v)
  { this->operator=(v); }

/****************/
/* ~SymbolValue */
/****************/
SymbolValue::~SymbolValue()
  {
   /* delete theString; */
  }

/*****************/
/* SymbolValue = */
/*****************/
/*
SymbolValue& SymbolValue::operator = (
  const SymbolValue& v)
  {
   if (this == &v) return *this;
   
   return *this;
  }
*/
/*********/
/* print */
/*********/
std::ostream& SymbolValue::print (std::ostream& o) const
  { return o << theString.c_str(); }
      
/*********/
/* clone */
/*********/
SymbolValue *SymbolValue::clone() const
  { return new SymbolValue(*this); }

/*###########################*/
/* InstanceNameValue Methods */
/*###########################*/

/*********************/
/* InstanceNameValue */
/*********************/
InstanceNameValue::InstanceNameValue()
  { }

/*********************/
/* InstanceNameValue */
/*********************/
InstanceNameValue::InstanceNameValue(
  char *initialString)
  {
   theString.assign(initialString);
  }

/*********************/
/* InstanceNameValue */
/*********************/
InstanceNameValue::InstanceNameValue( const InstanceNameValue& v)
  { this->operator=(v); }

/**********************/
/* ~InstanceNameValue */
/**********************/
InstanceNameValue::~InstanceNameValue()
  { }

/***********************/
/* InstanceNameValue = */
/***********************/
/*
InstanceNameValue& InstanceNameValue::operator = (
  const InstanceNameValue& v)
  {
   if (this == &v) return *this;
   
   return *this;
  }
*/  
/*********/
/* print */
/*********/
std::ostream& InstanceNameValue::print (std::ostream& o) const
  { return o << '[' << theString.c_str() << ']'; }
    
/*********/
/* clone */
/*********/
InstanceNameValue *InstanceNameValue::clone() const
  { return new InstanceNameValue(*this); }

/*######################*/
/* IntegerValue Methods */
/*######################*/

/****************/
/* IntegerValue */
/****************/
IntegerValue::IntegerValue() : theInteger (0)
  { }

/****************/
/* IntegerValue */
/****************/
IntegerValue::IntegerValue(
  long long initialValue) : theInteger(initialValue)
  {
  }

/****************/
/* IntegerValue */
/****************/
IntegerValue::IntegerValue( const IntegerValue& v)
  { this->operator=(v); }

/*****************/
/* ~IntegerValue */
/*****************/
IntegerValue::~IntegerValue()
  { }

/******************/
/* IntegerValue = */
/******************/
/*
IntegerValue& IntegerValue::operator = (
  const IntegerValue& v)
  {
   if (this == &v) return *this;
   
   return *this;
  }
*/  
/*********/
/* print */
/*********/
std::ostream& IntegerValue::print (std::ostream& o) const
  { return o << theInteger; }
    
/*********/
/* clone */
/*********/
IntegerValue *IntegerValue::clone() const
  { return new IntegerValue(*this); }
  
/*####################*/
/* FloatValue Methods */
/*####################*/

/**************/
/* FloatValue */
/**************/
FloatValue::FloatValue() : theFloat (0.0)
  { }

/**************/
/* FloatValue */
/**************/
FloatValue::FloatValue(
  double initialValue) : theFloat(initialValue) /* TBD combine with prior using default? */
  {
  }

/**************/
/* FloatValue */
/**************/
FloatValue::FloatValue( const FloatValue& v)
  { this->operator=(v); }

/***************/
/* ~FloatValue */
/***************/
FloatValue::~FloatValue()
  { }

/****************/
/* FloatValue = */
/****************/
/*
FloatValue& FloatValue::operator = (
  const FloatValue& v)
  {
   if (this == &v) return *this;
   
   return *this;
  }
*/  
/*********/
/* print */
/*********/
std::ostream& FloatValue::print (std::ostream& o) const
  { return o << theFloat; }
    
/*********/
/* clone */
/*********/
FloatValue *FloatValue::clone() const
  { return new FloatValue(*this); }

/*##########################*/
/* FactAddressValue Methods */
/*##########################*/

/********************/
/* FactAddressValue */
/********************/
FactAddressValue::FactAddressValue(
  void *theEnv,void *theFact) : theEnvironment(theEnv), theFactAddress(theFact)
  {
#ifndef CLIPS_WIN32_DLL
   EnvIncrementFactCount(theEnvironment,theFact);
#else
   __EnvIncrementFactCount(theEnvironment,theFact);
#endif
  }

/********************/
/* FactAddressValue */
/********************/
FactAddressValue::FactAddressValue( const FactAddressValue& v) : theFactAddress(NULL)
  { 
   this->operator=(v); 
  }

/*********************/
/* ~FactAddressValue */
/*********************/
FactAddressValue::~FactAddressValue()
  {   
#ifndef CLIPS_WIN32_DLL
   EnvDecrementFactCount(theEnvironment,theFactAddress);
#else
   __EnvDecrementFactCount(theEnvironment,theFactAddress);
#endif
  }

/**********************/
/* FactAddressValue = */
/**********************/
FactAddressValue& FactAddressValue::operator = (
  const FactAddressValue& v)
  {
   if (this == &v) return *this;

   if (theFactAddress != NULL)
     { 
#ifndef CLIPS_WIN32_DLL
      EnvDecrementFactCount(theEnvironment,theFactAddress);
#else
      __EnvDecrementFactCount(theEnvironment,theFactAddress);
#endif
     }
        
   theEnvironment = v.theEnvironment;
   theFactAddress = v.theFactAddress;
     
#ifndef CLIPS_WIN32_DLL
   EnvIncrementFactCount(theEnvironment,theFactAddress);
#else
   __EnvIncrementFactCount(theEnvironment,theFactAddress);
#endif
   
   return *this;
  }

/****************/
/* GetFactIndex */
/****************/
long long FactAddressValue::GetFactIndex() const
  {  
#ifndef CLIPS_WIN32_DLL
   return EnvFactIndex(theEnvironment,theFactAddress);
#else
   return __EnvFactIndex(theEnvironment,theFactAddress);
#endif

  }

/*********/
/* print */
/*********/
std::ostream& FactAddressValue::print (std::ostream& o) const
  {  
   return o << "<Fact-" << GetFactIndex() << ">";
  }
    
/*********/
/* clone */
/*********/
FactAddressValue *FactAddressValue::clone() const
  { return new FactAddressValue(*this); }

/***************/
/* GetFactSlot */
/***************/
DataObject FactAddressValue::GetFactSlot(char *slotName) const
  {  
   DATA_OBJECT theDO;
   int rv;
   
#ifndef CLIPS_WIN32_DLL
   rv = EnvGetFactSlot(theEnvironment,theFactAddress,slotName,&theDO);
#else
   rv = __EnvGetFactSlot(theEnvironment,theFactAddress,slotName,&theDO);
#endif
   
   if (! rv)
      {
       std::string excStr = "GetFactSlot: Invalid slot name ";
       excStr.append(slotName);
       
       throw std::logic_error(excStr); 
      }

   return ConvertDataObject(theEnvironment,&theDO);
  }

/*##############################*/
/* InstanceAddressValue Methods */
/*##############################*/

/************************/
/* InstanceAddressValue */
/************************/
InstanceAddressValue::InstanceAddressValue(
  void *theEnv,void *theInstance) : theEnvironment(theEnv), theInstanceAddress(theInstance)
  {
#ifndef CLIPS_WIN32_DLL
   EnvIncrementInstanceCount(theEnvironment,theInstance);
#else
   __EnvIncrementInstanceCount(theEnvironment,theInstance);
#endif
  }

/************************/
/* InstanceAddressValue */
/************************/
InstanceAddressValue::InstanceAddressValue( const InstanceAddressValue& v) : theInstanceAddress(NULL)
  { 
   this->operator=(v); 
  }

/*************************/
/* ~InstanceAddressValue */
/*************************/
InstanceAddressValue::~InstanceAddressValue()
  {   
#ifndef CLIPS_WIN32_DLL
   EnvDecrementInstanceCount(theEnvironment,theInstanceAddress);
#else
   __EnvDecrementInstanceCount(theEnvironment,theInstanceAddress);
#endif
  }

/**************************/
/* InstanceAddressValue = */
/**************************/
InstanceAddressValue& InstanceAddressValue::operator = (
  const InstanceAddressValue& v)
  {
   if (this == &v) return *this;

   if (theInstanceAddress != NULL)
     { 
#ifndef CLIPS_WIN32_DLL
      EnvDecrementInstanceCount(theEnvironment,theInstanceAddress);
#else
      __EnvDecrementInstanceCount(theEnvironment,theInstanceAddress);
#endif

     }
        
   theEnvironment = v.theEnvironment;
   theInstanceAddress = v.theInstanceAddress;
     
#ifndef CLIPS_WIN32_DLL
   EnvIncrementInstanceCount(theEnvironment,theInstanceAddress);
#else
   __EnvIncrementInstanceCount(theEnvironment,theInstanceAddress);
#endif
   
   return *this;
  }

/**********************/
/* GetRawInstanceName */
/**********************/
char *InstanceAddressValue::GetRawInstanceName() const
  {  
#ifndef CLIPS_WIN32_DLL
   return EnvGetInstanceName(theEnvironment,theInstanceAddress);
#else
   return __EnvGetInstanceName(theEnvironment,theInstanceAddress);
#endif
  }

/*********/
/* print */
/*********/
std::ostream& InstanceAddressValue::print (std::ostream& o) const
  {  
   return o << "<Instance-" << GetRawInstanceName() << ">";
  }
    
/*********/
/* clone */
/*********/
InstanceAddressValue *InstanceAddressValue::clone() const
  { return new InstanceAddressValue(*this); }
  
/*#########################*/
/* MultifieldValue Methods */
/*#########################*/

/*******************/
/* MultifieldValue */
/*******************/
MultifieldValue::MultifieldValue()
  { 
   /* std::cout << "MultifieldValue::MultifieldValue()" << std::endl; */
  }

/*******************/
/* MultifieldValue */
/*******************/
MultifieldValue::MultifieldValue(size_t size)
  {
   /* std::cout << "MultifieldValue::MultifieldValue(" << size << ")" << std::endl; */
   theMultifield.reserve(size);
  }

/*******************/
/* MultifieldValue */
/*******************/
MultifieldValue::MultifieldValue( const MultifieldValue& v)
  { 
   /* std::cout << "MultifieldValue::MultifieldValue(MultifieldValue)" << std::endl; */
   this->operator=(v);
  }

/********************/
/* ~MultifieldValue */
/********************/
MultifieldValue::~MultifieldValue()
  { 
   size_t i;
   
   for (i = 0; i < theMultifield.size(); i++)
     { delete theMultifield[i]; }
  }

/*******/
/* add */
/*******/
void MultifieldValue::add(
  Value *theValue)
  {
   theMultifield.push_back(theValue);
  }
  
/*********************/
/* MultifieldValue = */
/*********************/
MultifieldValue& MultifieldValue::operator = (
  const MultifieldValue& v)
  {
   size_t i;
   Value *theValue;

   if (this == &v) return *this;

   for (i = 0; i < theMultifield.size(); i++)
     { delete theMultifield[i]; }
      
   theMultifield = v.theMultifield; 
   theMultifield.reserve(v.theMultifield.capacity());

   for (i = 0; i < v.theMultifield.size(); i++)
     {  
      theValue = v.theMultifield[i];
      theMultifield[i] = theValue->clone(); 
     }
   
   return *this;
  }

/*********/
/* print */
/*********/
std::ostream& MultifieldValue::print (std::ostream& o) const
  {
   size_t i;
   bool first = true;
   
   o << "("; 
   
   for (i = 0; i < theMultifield.size(); i++)
     { 
      if (first)
        { o << theMultifield[i]; }
      else
        { o << " " << theMultifield[i]; }
      first = false;
     }

   return o << ")";    
  }
    
/*********/
/* clone */
/*********/
MultifieldValue *MultifieldValue::clone() const
  { 
   return new MultifieldValue(*this); 
  }

#ifndef CLIPS_WIN32_DLL

/*#########################*/
/* Static Router Functions */
/*#########################*/
  
/*****************/
/* CLIPSCPPQuery */
/*****************/
static int CLIPSCPPQuery(
  void *theEnv,
  char *logicalName)
  { 
   CLIPSCPPRouter *theRouter = (CLIPSCPPRouter *) GetEnvironmentRouterContext(theEnv);
   CLIPSCPPEnv *theCPPEnv = (CLIPSCPPEnv *) GetEnvironmentContext(theEnv);
   
   return(theRouter->Query(theCPPEnv,logicalName));
  }

/*****************/
/* CLIPSCPPPrint */
/*****************/
static int CLIPSCPPPrint(
  void *theEnv,
  char *logicalName,
  char *printString)
  { 
   CLIPSCPPRouter *theRouter = (CLIPSCPPRouter *) GetEnvironmentRouterContext(theEnv);
   CLIPSCPPEnv *theCPPEnv = (CLIPSCPPEnv *) GetEnvironmentContext(theEnv);
   
   return(theRouter->Print(theCPPEnv,logicalName,printString));
  }

/*****************/
/* CLIPSCPPGetc */
/*****************/
static int CLIPSCPPGetc(
  void *theEnv,
  char *logicalName)
  { 
   CLIPSCPPRouter *theRouter = (CLIPSCPPRouter *) GetEnvironmentRouterContext(theEnv);
   CLIPSCPPEnv *theCPPEnv = (CLIPSCPPEnv *) GetEnvironmentContext(theEnv);
   
   return(theRouter->Getc(theCPPEnv,logicalName));
  }
  
/*****************/
/* CLIPSCPPUngetc */
/*****************/
static int CLIPSCPPUngetc(
  void *theEnv,
  int character,
  char *logicalName)
  { 
   CLIPSCPPRouter *theRouter = (CLIPSCPPRouter *) GetEnvironmentRouterContext(theEnv);
   CLIPSCPPEnv *theCPPEnv = (CLIPSCPPEnv *) GetEnvironmentContext(theEnv);
   
   return(theRouter->Ungetc(theCPPEnv,character,logicalName));
  }
  
/*****************/
/* CLIPSCPPExit */
/*****************/
static int CLIPSCPPExit(
  void *theEnv,
  int exitCode)
  { 
   CLIPSCPPRouter *theRouter = (CLIPSCPPRouter *) GetEnvironmentRouterContext(theEnv);
   CLIPSCPPEnv *theCPPEnv = (CLIPSCPPEnv *) GetEnvironmentContext(theEnv);
   
   return(theRouter->Exit(theCPPEnv,exitCode));
  }

#endif
