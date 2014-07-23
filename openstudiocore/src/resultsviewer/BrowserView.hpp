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

#ifndef RESULTSVIEWER_BROWSERVIEW_HPP
#define RESULTSVIEWER_BROWSERVIEW_HPP

#include <QTextBrowser>

namespace resultsviewer{

/**
BrowserView is a ui browser widget for the EnergyPlus html output table.
*/
class BrowserView : public QTextBrowser
{
  Q_OBJECT

public:

  BrowserView( QWidget* parent=nullptr);

  const QString& filename() {return m_filename;}
  void setFilename(const QString& filename);
  const QString& alias() {return m_alias;}
  void setAlias(const QString& alias);
  void updateAlias(const QString& alias, const QString& filename);

private:
  // send float or dock signal
  void mouseDoubleClickEvent(QMouseEvent *evt);
  QString m_filename;
  QString m_alias;
  void closeEvent(QCloseEvent *evt);
  void leaveEvent(QEvent *evt);

signals: 
  void signalFloatOrDockMe(resultsviewer::BrowserView *browser);
  // closing in resultsviewer - remove from collection
  void signalClose(resultsviewer::BrowserView *browser);

};


}; // resultsviewer namespace

#endif // RESULTSVIEWER_BROWSERVIEW_HPP
