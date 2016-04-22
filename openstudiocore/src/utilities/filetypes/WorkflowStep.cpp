/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#include "WorkflowStep.hpp"

#include "../core/Assert.hpp"

#include <boost/optional.hpp>

#include <string>

namespace openstudio{

WorkflowStep::WorkflowStep(const std::string& measureDirName)
  : m_measureDirName(measureDirName)
{}

WorkflowStep::WorkflowStep(const Attribute& step)
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

std::string WorkflowStep::measureDirName() const
{
  return m_measureDirName;
}

std::map<std::string, Variant> WorkflowStep::arguments() const
{
  return m_arguments;
}

boost::optional<Variant> WorkflowStep::getArgument(const std::string& name) const
{
  auto it = m_arguments.find(name);
  if (it != m_arguments.end()){
    return it->second;
  }
  return boost::none;
}

void WorkflowStep::setArgument(const std::string& name, const Variant& value)
{
  m_arguments.insert(std::make_pair(name, value));
}

void WorkflowStep::setArgument(const std::string& name, bool value)
{
  setArgument(name, Variant(value));
}

void WorkflowStep::setArgument(const std::string& name, double value)
{
  setArgument(name, Variant(value));
}

void WorkflowStep::setArgument(const std::string& name, int value)
{
  setArgument(name, Variant(value));
}

void WorkflowStep::setArgument(const std::string& name, const std::string& value)
{
  setArgument(name, Variant(value));
}

void WorkflowStep::removeArgument(const std::string& name)
{
  m_arguments.erase(name);
}

void WorkflowStep::clearArguments()
{
  m_arguments.clear();
}

} // openstudio
