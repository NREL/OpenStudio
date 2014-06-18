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

#include "OSResult.hpp"

#include "../utilities/core/Assert.hpp"

#include <boost/filesystem.hpp>

#include <QFile>
#include <QDomDocument>
#include <QDomElement>

namespace openstudio {
namespace ruleset {

OSResult::OSResult()
  : m_value(OSResultValue::Success)
{}

OSResult::OSResult(const QDomElement& element) {
  if (element.isNull()) {
    LOG_AND_THROW("Cannot construct OSResult from null QDomElement.");
  }
  if (element.tagName() != QString::fromStdString("OSResult")) {
    LOG_AND_THROW("Cannot construct OSResult from element named " << toString(element.tagName())
                  << " rather than OSResult.");
  }

  QDomElement valueElement = element.firstChildElement(QString::fromStdString("Value"));
  QDomElement errorsElement = element.firstChildElement(QString::fromStdString("Errors"));
  QDomElement warningsElement = element.firstChildElement(QString::fromStdString("Warnings"));
  QDomElement infoElement = element.firstChildElement(QString::fromStdString("Info"));
  QDomElement initialConditionElement = element.firstChildElement(QString::fromStdString("InitialCondition"));
  QDomElement finalConditionElement = element.firstChildElement(QString::fromStdString("FinalCondition"));
  QDomElement attributesElement = element.firstChildElement(QString::fromStdString("Attributes"));

  if (valueElement.isNull()) {
    LOG_AND_THROW("valueElement is null.");
  }

  m_value = OSResultValue(valueElement.firstChild().nodeValue().toStdString());

  if (!errorsElement.isNull()) {
    QDomNodeList childNodes = errorsElement.childNodes();
    for (int i = 0; i < childNodes.count(); ++i) {
      QDomElement childElement = childNodes.at(i).toElement();
      m_errors.push_back(logMessageFromXML(Error,childElement));
    }
  }

  if (!warningsElement.isNull()) {
    QDomNodeList childNodes = warningsElement.childNodes();
    for (int i = 0; i < childNodes.count(); ++i) {
      QDomElement childElement = childNodes.at(i).toElement();
      m_warnings.push_back(logMessageFromXML(Warn,childElement));
    }
  }

  if (!infoElement.isNull()) {
    QDomNodeList childNodes = infoElement.childNodes();
    for (int i = 0; i < childNodes.count(); ++i) {
      QDomElement childElement = childNodes.at(i).toElement();
      m_info.push_back(logMessageFromXML(Info,childElement));
    }
  }

  if (!initialConditionElement.isNull()) {
    QDomNodeList childNodes = initialConditionElement.childNodes();
    int n = childNodes.count();
    OS_ASSERT(n < 2);
    if (n > 0) {
      QDomElement childElement = childNodes.at(0).toElement();
      m_initialCondition = logMessageFromXML(Info,childElement);
    }
  }

  if (!finalConditionElement.isNull()) {
    QDomNodeList childNodes = finalConditionElement.childNodes();
    int n = childNodes.count();
    OS_ASSERT(n < 2);
    if (n > 0) {
      QDomElement childElement = childNodes.at(0).toElement();
      m_finalCondition = logMessageFromXML(Info,childElement);
    }
  }

  if (!attributesElement.isNull()) {
    QDomNodeList childNodes = attributesElement.childNodes();
    for (int i = 0; i < childNodes.count(); ++i) {
      QDomElement childElement = childNodes.at(i).toElement();
      m_attributes.push_back(Attribute(childElement));
    }
  }
}

OSResultValue OSResult::value() const {
  return m_value;
}

std::vector<LogMessage> OSResult::errors() const {
  return m_errors;
}

std::vector<LogMessage> OSResult::warnings() const {
  return m_warnings;
}

std::vector<LogMessage> OSResult::info() const {
  return m_info;
}

boost::optional<LogMessage> OSResult::initialCondition() const {
  return m_initialCondition;
}

boost::optional<LogMessage> OSResult::finalCondition() const {
  return m_finalCondition;
}

std::vector<Attribute> OSResult::attributes() const {
  return m_attributes;
}

void OSResult::setValue(const OSResultValue& value) {
  m_value = value;
}

void OSResult::addError(const std::string& channel, const std::string& message) {
  m_errors.push_back(LogMessage(Error,channel,message));
}

void OSResult::addWarning(const std::string& channel, const std::string& message) {
  m_warnings.push_back(LogMessage(Warn,channel,message));
}

void OSResult::addInfo(const std::string& channel, const std::string& message) {
  m_info.push_back(LogMessage(Info,channel,message));
}

void OSResult::setInitialCondition(const std::string& channel, const std::string& message) {
  m_initialCondition = LogMessage(Info,channel,message);
}

void OSResult::setFinalCondition(const std::string& channel, const std::string& message) {
  m_finalCondition = LogMessage(Info,channel,message);
}

void OSResult::appendAttribute(const Attribute& attribute) {
  m_attributes.push_back(attribute);
}

boost::optional<OSResult> OSResult::load(const openstudio::path& p) {
  OptionalOSResult result;
  if (boost::filesystem::exists(p)) {
    try {
      // load xml file
      QFile file(toQString(p));
      file.open(QFile::ReadOnly);
      QDomDocument qDomDocument;
      qDomDocument.setContent(&file);
      file.close();

      // de-serialize
      result = OSResult(qDomDocument.documentElement());
    }
    catch (std::exception& e) {
      LOG(Error,"Could not load OSResult from " << toString(p) << ", because "
          << e.what() << ".");
    }
    catch (...) {
      LOG(Error,"Could not load OSResult from " << toString(p) << ".");
    }
  }
  else {
    LOG(Error,"Cannot load OSResult from " << toString(p)
        << ", because that file does not exist.");
  }

  return result;
}

bool OSResult::save(const openstudio::path& p, bool overwrite) const {
  bool result(false);
  if (overwrite || !boost::filesystem::exists(p)) {
    try {
      QFile file(toQString(p));
      file.open(QFile::WriteOnly);
      QTextStream out(&file);
      toXml().save(out, 2);
      file.close();
      result = true;
    }
    catch (std::exception& e) {
      LOG(Error,"Could not save OSResult to " << toString(p) << ", because "
          << e.what());
    }
    catch (...) {
      LOG(Error,"Could not save OSResult to " << toString(p) << ".");
    }
  }
  else {
    LOG(Error,"Could not save OSResult because " << toString(p) <<
        " exists and overwrite == false.");
  }

  return result;
}

LogMessage OSResult::logMessageFromXML(LogLevel level, const QDomElement& element) const {
  if (element.isNull()) {
    LOG_AND_THROW("Cannot construct LogMessage from null QDomElement.");
  }
  if (element.tagName() != QString::fromStdString("LogMessage")) {
    LOG_AND_THROW("Cannot construct LogMessage from element named " << toString(element.tagName())
                  << " rather than LogMessage.");
  }

  QDomElement channelElement = element.firstChildElement(QString::fromStdString("Channel"));
  QDomElement messageElement = element.firstChildElement(QString::fromStdString("Message"));

  if (channelElement.isNull()) {
    LOG_AND_THROW("channelElement is null.");
  }
  if (messageElement.isNull()) {
    LOG_AND_THROW("messageElement is null.");
  }

  return LogMessage(level,
                    channelElement.firstChild().nodeValue().toStdString(),
                    messageElement.firstChild().nodeValue().toStdString());

}

QDomDocument OSResult::toXml() const {
  QDomDocument doc;
  QDomElement element = doc.createElement(QString("OSResult"));

  QDomElement childElement;
  QDomText text;

  childElement = doc.createElement(QString("Value"));
  text = doc.createTextNode(toQString(m_value.valueName()));
  childElement.appendChild(text);
  element.appendChild(childElement);

  if (!m_errors.empty()) {
    childElement = doc.createElement(QString("Errors"));
    logMessagesToXml(doc,childElement,m_errors);
    element.appendChild(childElement);
  }

  if (!m_warnings.empty()) {
    childElement = doc.createElement(QString("Warnings"));
    logMessagesToXml(doc,childElement,m_warnings);
    element.appendChild(childElement);
  }

  if (!m_info.empty()) {
    childElement = doc.createElement(QString("Info"));
    logMessagesToXml(doc,childElement,m_info);
    element.appendChild(childElement);
  }

  if (m_initialCondition) {
    childElement = doc.createElement(QString("InitialCondition"));
    logMessagesToXml(doc,childElement,std::vector<LogMessage>(1u,*m_initialCondition));
    element.appendChild(childElement);
  }

  if (m_finalCondition) {
    childElement = doc.createElement(QString("FinalCondition"));
    logMessagesToXml(doc,childElement,std::vector<LogMessage>(1u,*m_finalCondition));
    element.appendChild(childElement);
  }

  if (!m_attributes.empty()) {
    childElement = doc.createElement(QString("Attributes"));
    for (const Attribute& attribute : m_attributes) {
      childElement.appendChild(attribute.toXml().documentElement());
    }
    element.appendChild(childElement);
  }

  doc.appendChild(element);
  return doc;
}

void OSResult::logMessagesToXml(QDomDocument& doc,
                                QDomElement& element,
                                const std::vector<LogMessage>& logMessages) const
{
  QDomElement childElement, grandchildElement;
  QDomText text;

  for (const LogMessage& msg : logMessages) {
    childElement = doc.createElement(QString("LogMessage"));

    grandchildElement = doc.createElement(QString("Channel"));
    text = doc.createTextNode(toQString(msg.logChannel()));
    grandchildElement.appendChild(text);
    childElement.appendChild(grandchildElement);

    grandchildElement = doc.createElement(QString("Message"));
    text = doc.createTextNode(toQString(msg.logMessage()));
    grandchildElement.appendChild(text);
    childElement.appendChild(grandchildElement);

    element.appendChild(childElement);
  }
}

} // ruleset
} // openstudio
