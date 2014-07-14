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

#ifndef OPENSTUDIO_RENDERINGCOLORWIDGET_HPP
#define OPENSTUDIO_RENDERINGCOLORWIDGET_HPP

#include "../model/RenderingColor.hpp"

#include <QWidget>

class QPushButton;

namespace openstudio {

class RenderingColorWidget : public QWidget
{
  Q_OBJECT

public:

  RenderingColorWidget(QWidget* parent = 0);

  virtual ~RenderingColorWidget() {}

  virtual void attach(const openstudio::model::RenderingColor& renderingColor);

  virtual void detach();

private slots:

  void clear();

  void refresh();

  void renderColorButtonClicked();

private:
  
  QWidget* m_renderColorWidget;
  QPushButton* m_renderColorButton;
  boost::optional<openstudio::model::RenderingColor> m_renderingColor;
};

} // openstudio

#endif // OPENSTUDIO_RENDERINGCOLORWIDGET_HPP

