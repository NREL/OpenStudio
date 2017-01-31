/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "MeasureDragData.hpp"
#include <QDomDocument>
#include <QDomElement>
#include <QDomText>

namespace openstudio {

QString MeasureDragData::mimeType() { return "MeasureDragData"; }

QString MeasureDragData::mimeType(openstudio::MeasureType type)
{
  return mimeType() + openstudio::toQString(":" + type.valueName());
}

MeasureDragData::MeasureDragData(const QByteArray & data)
  : QObject()
{
  QString stringData(data);
  QDomDocument doc;
  doc.setContent(stringData);

  QDomElement measureDragDataElement = doc.documentElement();

  // Source Enum

  QDomElement sourceEnumElement = measureDragDataElement.firstChildElement("Source");
  QString sourceEnumString = sourceEnumElement.text();

  if( sourceEnumString == "BCL" )
  {
    m_sourceEnum = LocalLibrary::BCL;
  }
  else if( sourceEnumString == "USER" )
  {
    m_sourceEnum = LocalLibrary::USER;
  }

  // ID

  QDomElement idElement = measureDragDataElement.firstChildElement("ID");
  QString idString = idElement.text();
  
  m_id = toUUID(idString);
}

MeasureDragData::MeasureDragData(const UUID & id)
  : QObject(),
    m_id(id)
{
}

LocalLibrary::LibrarySource MeasureDragData::source()
{
  return m_sourceEnum;
}

UUID MeasureDragData::id()
{
  return m_id;
}

QString MeasureDragData::toXml()
{
  QDomDocument doc;
  QDomElement measureDragDataElement = doc.createElement("MeasureDragData");
  doc.appendChild(measureDragDataElement);

  // Source Enum

  //QDomElement sourceEnumElement = doc.createElement("Source");
  //measureDragDataElement.appendChild(sourceEnumElement);

  //if( m_sourceEnum == LocalLibraryController::BCL )
  //{
  //  QDomText text = doc.createTextNode("BCL");
  //  sourceEnumElement.appendChild(text);
  //}
  //else if ( m_sourceEnum == LocalLibraryController::USER )
  //{
  //  QDomText text = doc.createTextNode("USER");
  //  sourceEnumElement.appendChild(text);
  //}

  // ID

  QDomElement idElement = doc.createElement("ID");
  measureDragDataElement.appendChild(idElement);

  QDomText idText = doc.createTextNode(toQString(m_id));
  idElement.appendChild(idText);

  return doc.toString();
}

QByteArray MeasureDragData::data()
{
  return toXml().toUtf8();
}

} // openstudio

