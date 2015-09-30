/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.  
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
  
  m_id = UUID(idString);
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

  QDomText idText = doc.createTextNode(m_id.toString());
  idElement.appendChild(idText);

  return doc.toString();
}

QByteArray MeasureDragData::data()
{
  return toXml().toUtf8();
}

} // openstudio

