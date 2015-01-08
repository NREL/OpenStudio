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

#ifndef OPENSTUDIO_RENDERINGCOLORWIDGET_HPP
#define OPENSTUDIO_RENDERINGCOLORWIDGET_HPP

#include "../shared_gui_components/FieldMethodTypedefs.hpp"

#include "../model/RenderingColor.hpp"

#include <QWidget>

class QPushButton;

namespace openstudio {

  class RenderingColorWidget2 : public QWidget
  {
    Q_OBJECT

  public:

    RenderingColorWidget2(QWidget* parent = 0);

    virtual ~RenderingColorWidget2() {}

    virtual void bind(model::ModelObject & modelObject,
      OptionalModelObjectGetter get,
      ModelObjectSetter set);

    virtual void unbind();

  private slots:

    void clear();

    void refresh();

    void renderColorButtonClicked();

    void getRenderingColor();

  private:

    void setRenderingColor();

    boost::optional<OptionalModelObjectGetter> m_get;
    boost::optional<ModelObjectSetter> m_set;
    boost::optional<model::ModelObject> m_modelObject;
    QPushButton * m_renderColorButton;
    boost::optional<model::RenderingColor> m_renderingColor;
  };

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

