/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "WorkflowStep.hpp"
#include "WorkflowStep_Impl.hpp"

#include "../core/Assert.hpp"


namespace openstudio{
namespace detail{

  WorkflowStep_Impl::WorkflowStep_Impl()
  {}

  WorkflowStep_Impl::~WorkflowStep_Impl()
  {}

  boost::optional<WorkflowStepResult> WorkflowStep_Impl::result() const
  {
    return m_result;
  }

  void WorkflowStep_Impl::setResult(const WorkflowStepResult& result)
  {
    m_result = result;
    onUpdate();
  }

  void WorkflowStep_Impl::resetResult()
  {
    m_result.reset();
    onUpdate();
  }

  void WorkflowStep_Impl::onUpdate()
  {
    this->onChange.nano_emit();
  }

  MeasureStep_Impl::MeasureStep_Impl(const std::string& measureDirName)
    : m_measureDirName(measureDirName)
  {}
  /*
  MeasureStep_Impl::MeasureStep_Impl(const Attribute& step)
  {
  boost::optional<Attribute> measureDirNameAttribute = step.findChildByName("measure_dir_name");
  if (measureDirNameAttribute && (measureDirNameAttribute->valueType().value() == AttributeValueType::String)){
  m_measureDirName = measureDirNameAttribute->valueAsString();
  } else{
  LOG_AND_THROW("Missing 'measure_dir_name' attribute");
  }

  boost::optional<Attribute> arguments = step.findChildByName("arguments");
  if (!arguments){
  LOG_AND_THROW("Missing 'arguments' attribute");
  } else if (arguments->valueType().value() != AttributeValueType::AttributeVector){
  LOG_AND_THROW("Attribute 'arguments' is not an AttributeVector");
  }

  for (const auto& argument : arguments->valueAsAttributeVector()){

  boost::optional<Attribute> name = argument.findChildByName("name");
  if (!name || name->valueType().value() != AttributeValueType::String){
  continue;
  }

  std::string nameString = name->valueAsString();
  boost::optional<Variant> variant;

  boost::optional<Attribute> value = argument.findChildByName("value");
  if (!value){
  continue;
  } else if (value->valueType().value() == AttributeValueType::Boolean){
  variant = Variant(value->valueAsBoolean());
  } else if (value->valueType().value() == AttributeValueType::Double){
  variant = Variant(value->valueAsDouble());
  } else if (value->valueType().value() == AttributeValueType::Integer){
  variant = Variant(value->valueAsInteger());
  } else if (value->valueType().value() == AttributeValueType::Unsigned){
  variant = Variant((int)value->valueAsUnsigned());
  } else if (value->valueType().value() == AttributeValueType::String){
  variant = Variant(value->valueAsString());
  }

  if (variant){
  m_arguments.insert(std::make_pair(nameString, *variant));
  }
  }
  }
  */

  std::string MeasureStep_Impl::string() const
  {
    Json::Value result;
    result["measure_dir_name"] = m_measureDirName;

    if (m_name){
      result["name"] = m_name.get();
    }

    if (m_description){
      result["description"] = m_description.get();
    }

    if (m_modelerDescription){
      result["modeler_description"] = m_modelerDescription.get();
    }

    Json::Value arguments(Json::objectValue);
    for (const auto& argument : m_arguments){
      std::string name = argument.first;
      Variant value = argument.second;

      if (value.variantType() == VariantType::String){
        arguments[name] = value.valueAsString();
      }else if (value.variantType() == VariantType::Double){
        arguments[name] = value.valueAsDouble();
      }else if (value.variantType() == VariantType::Integer){
        arguments[name] = value.valueAsInteger();
      }else if (value.variantType() == VariantType::Boolean){
        arguments[name] = value.valueAsBoolean();
      }

    }
    result["arguments"] = arguments;

    boost::optional<WorkflowStepResult> workflowStepResult = this->result();
    if (workflowStepResult){
      Json::Reader reader;
      Json::Value v;
      bool parsingSuccessful = reader.parse(workflowStepResult->string(), v);
      if (parsingSuccessful){
        result["result"] = v;
      }
    }

    Json::StyledWriter writer;
    return writer.write(result);
  }


  std::string MeasureStep_Impl::measureDirName() const
  {
    return m_measureDirName;
  }

  bool MeasureStep_Impl::setMeasureDirName(const std::string& measureDirName)
  {
    m_measureDirName = measureDirName;
    return true;
  }

  boost::optional<std::string> MeasureStep_Impl::name() const
  {
    return m_name;
  }

  bool MeasureStep_Impl::setName(const std::string& name)
  {
    m_name = name;
    onUpdate();
    return true;
  }

  void MeasureStep_Impl::resetName()
  {
    m_name.reset();
    onUpdate();
  }

  boost::optional<std::string> MeasureStep_Impl::description() const
  {
    return m_description;
  }

  bool MeasureStep_Impl::setDescription(const std::string& description)
  {
    m_description = description;
    onUpdate();
    return true;
  }

  void MeasureStep_Impl::resetDescription()
  {
    m_description.reset();
    onUpdate();
  }

  boost::optional<std::string> MeasureStep_Impl::modelerDescription() const
  {
    return m_modelerDescription;
  }

  bool MeasureStep_Impl::setModelerDescription(const std::string& modelerDescription)
  {
    m_modelerDescription = modelerDescription;
    onUpdate();
    return true;
  }

  void MeasureStep_Impl::resetModelerDescription()
  {
    m_modelerDescription.reset();
    onUpdate();
  }

  std::map<std::string, Variant> MeasureStep_Impl::arguments() const
  {
    return m_arguments;
  }

  boost::optional<Variant> MeasureStep_Impl::getArgument(const std::string& name) const
  {
    auto it = m_arguments.find(name);
    if (it != m_arguments.end()){
      return it->second;
    }
    return boost::none;
  }

  void MeasureStep_Impl::setArgument(const std::string& name, const Variant& value)
  {
    m_arguments[name] = value;
    onUpdate();
  }

  void MeasureStep_Impl::setArgument(const std::string& name, bool value)
  {
    setArgument(name, Variant(value));
  }

  void MeasureStep_Impl::setArgument(const std::string& name, double value)
  {
    setArgument(name, Variant(value));
  }

  void MeasureStep_Impl::setArgument(const std::string& name, int value)
  {
    setArgument(name, Variant(value));
  }

  void MeasureStep_Impl::setArgument(const std::string& name, const std::string& value)
  {
    setArgument(name, Variant(value));
  }

  void MeasureStep_Impl::removeArgument(const std::string& name)
  {
    m_arguments.erase(name);
    onUpdate();
  }

  void MeasureStep_Impl::clearArguments()
  {
    m_arguments.clear();
    onUpdate();
  }

} //detail

WorkflowStep::WorkflowStep(std::shared_ptr<detail::WorkflowStep_Impl> impl)
  : m_impl(impl)
{
  OS_ASSERT(getImpl<detail::WorkflowStep_Impl>());
}

WorkflowStep::~WorkflowStep()
{}

boost::optional<WorkflowStep> WorkflowStep::fromString(const std::string& s)
{
  boost::optional<WorkflowStep> result;

  Json::Reader reader;
  Json::Value value;
  bool parsingSuccessful = reader.parse(s, value);
  if (!parsingSuccessful){
    return result;
  }

  if (value.isMember("measure_dir_name")){
    Json::Value measureDirName = value["measure_dir_name"];

    MeasureStep measureStep(measureDirName.asString());
    result = measureStep;

    if (value.isMember("name")){
      Json::Value name = value["name"];
      measureStep.setName(name.asString());
    }

    if (value.isMember("description")){
      Json::Value description = value["description"];
      measureStep.setDescription(description.asString());
    }

    if (value.isMember("modeler_description")){
      Json::Value modelerDescription = value["modeler_description"];
      measureStep.setModelerDescription(modelerDescription.asString());
    }

    Json::Value arguments = value["arguments"];
    for (const auto& name : arguments.getMemberNames()){
      Json::Value value = arguments[name];

      if (value.isBool()){
        measureStep.setArgument(name, value.asBool());
      }else if (value.isIntegral()){
        measureStep.setArgument(name, value.asInt());
      }else if (value.isDouble()){
        measureStep.setArgument(name, value.asDouble());
      }else{
        measureStep.setArgument(name, value.asString());
      }
    }
  }

  if (value.isMember("result")){
    Json::StyledWriter writer;
    Json::Value v = value["result"];
    std::string s = writer.write(v);
    boost::optional<WorkflowStepResult> workflowStepResult = WorkflowStepResult::fromString(s);
    if (workflowStepResult){
      result->setResult(*workflowStepResult);
    }
  }

  return result;
}

std::string WorkflowStep::string() const
{
  return getImpl<detail::WorkflowStep_Impl>()->string();
}

boost::optional<WorkflowStepResult> WorkflowStep::result() const
{
  return getImpl<detail::WorkflowStep_Impl>()->result();
}

void WorkflowStep::setResult(const WorkflowStepResult& result)
{
  getImpl<detail::WorkflowStep_Impl>()->setResult(result);
}

void WorkflowStep::resetResult()
{
  getImpl<detail::WorkflowStep_Impl>()->resetResult();
}

bool WorkflowStep::operator==(const WorkflowStep& other) const
{
  return m_impl == other.m_impl;
}

MeasureStep::MeasureStep(const std::string& measureDirName)
  : WorkflowStep(std::shared_ptr<detail::MeasureStep_Impl>(new detail::MeasureStep_Impl(measureDirName)))
{
  OS_ASSERT(getImpl<detail::MeasureStep_Impl>());
}

MeasureStep::MeasureStep(std::shared_ptr<detail::MeasureStep_Impl> impl)
  : WorkflowStep(impl)
{
  OS_ASSERT(getImpl<detail::MeasureStep_Impl>());
}

std::string MeasureStep::measureDirName() const
{
  return getImpl<detail::MeasureStep_Impl>()->measureDirName();
}

bool MeasureStep::setMeasureDirName(const std::string& measureDirName)
{
  return getImpl<detail::MeasureStep_Impl>()->setMeasureDirName(measureDirName);
}

boost::optional<std::string> MeasureStep::name() const
{
  return getImpl<detail::MeasureStep_Impl>()->name();
}

bool MeasureStep::setName(const std::string& name)
{
  return getImpl<detail::MeasureStep_Impl>()->setName(name);
}

void MeasureStep::resetName()
{
  getImpl<detail::MeasureStep_Impl>()->resetName();
}

boost::optional<std::string> MeasureStep::description() const
{
  return getImpl<detail::MeasureStep_Impl>()->description();
}

bool MeasureStep::setDescription(const std::string& description)
{
  return getImpl<detail::MeasureStep_Impl>()->setDescription(description);
}

void MeasureStep::resetDescription()
{
  getImpl<detail::MeasureStep_Impl>()->resetDescription();
}

boost::optional<std::string> MeasureStep::modelerDescription() const
{
  return getImpl<detail::MeasureStep_Impl>()->modelerDescription();
}

bool MeasureStep::setModelerDescription(const std::string& modelerDescription)
{
  return getImpl<detail::MeasureStep_Impl>()->setModelerDescription(modelerDescription);
}

void MeasureStep::resetModelerDescription()
{
  getImpl<detail::MeasureStep_Impl>()->resetModelerDescription();
}

std::map<std::string, Variant> MeasureStep::arguments() const
{
  return getImpl<detail::MeasureStep_Impl>()->arguments();
}

boost::optional<Variant> MeasureStep::getArgument(const std::string& name) const
{
  return getImpl<detail::MeasureStep_Impl>()->getArgument(name);
}

void MeasureStep::setArgument(const std::string& name, const Variant& value)
{
  getImpl<detail::MeasureStep_Impl>()->setArgument(name, value);
}

void MeasureStep::setArgument(const std::string& name, bool value)
{
  getImpl<detail::MeasureStep_Impl>()->setArgument(name, value);
}

void MeasureStep::setArgument(const std::string& name, double value)
{
  getImpl<detail::MeasureStep_Impl>()->setArgument(name, value);
}

void MeasureStep::setArgument(const std::string& name, int value)
{
  getImpl<detail::MeasureStep_Impl>()->setArgument(name, value);
}

void MeasureStep::setArgument(const std::string& name, const char* value)
{
  getImpl<detail::MeasureStep_Impl>()->setArgument(name, std::string(value));
}

void MeasureStep::setArgument(const std::string& name, const std::string& value)
{
  getImpl<detail::MeasureStep_Impl>()->setArgument(name, value);
}

void MeasureStep::removeArgument(const std::string& name)
{
  getImpl<detail::MeasureStep_Impl>()->removeArgument(name);
}

void MeasureStep::clearArguments()
{
  getImpl<detail::MeasureStep_Impl>()->clearArguments();
}

std::ostream& operator<<(std::ostream& os, const WorkflowStep& workflowStep)
{
  os << workflowStep.string();
  return os;
}

} // openstudio
