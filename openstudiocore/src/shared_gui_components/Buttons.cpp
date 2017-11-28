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

#include "Buttons.hpp"
#include <QString>
#include <QPainter>
#include <QPaintEvent>
#include <QPixmap>
#include <QTimer>

namespace openstudio {

ZoomOutButton::ZoomOutButton(QWidget * parent)
  : QPushButton()
{
  setFlat(true);

  setFixedSize(20,20);

  QString style;
  style.append("QPushButton {"
               "  background-image:url(':images/zoom-out.png');"
               "  border:none;"
               "}");
  style.append("QPushButton:hover {"
               "  background-image:url(':images/zoom-out-over.png');"
               "}");
  style.append("QPushButton:pressed {"
               "  background-image:url(':images/zoom-out-press.png');"
               "}");

  setStyleSheet(style);
}

OrangeButton::OrangeButton(QWidget * parent)
  : QPushButton()
{
  setFlat(true);

  QString style;
  style.append("QPushButton { ");
  style.append("border-radius: 4px; ");
  style.append("border: 1px solid #949393; ");
  style.append("background: qlineargradient(x1:0,y1:0,x2:0,y2:1, ");
  style.append("                             stop: 0 #FDBC3B, "); 
  style.append("                             stop: 1 #F47920 ); ");
  style.append("height: 22px; ");
  style.append("font-size: 12px; ");
  style.append("margin: 0; ");
  style.append("padding-left: 10px; ");
  style.append("padding-right: 10px; ");
  style.append("} ");

  setStyleSheet(style);
}

GrayButton::GrayButton(QWidget * parent)
  : QPushButton()
{
  setFlat(true);

  QString style;
  style.append("QPushButton { ");
  style.append("border-radius: 4px; ");
  style.append("border: 1px solid #949393; ");
  style.append("background: qlineargradient(x1:0,y1:0,x2:0,y2:1, ");
  style.append("                             stop: 0 #F2F2F2, "); 
  style.append("                             stop: 1 #A5A5A5 ); ");
  style.append("height: 22px; ");
  style.append("font-size: 12px; ");
  style.append("margin: 0; ");
  style.append("padding-left: 10px; ");
  style.append("padding-right: 10px; ");
  style.append("} ");

  style.append("QPushButton:pressed { ");
  style.append("background: qlineargradient(x1:0,y1:0,x2:0,y2:1, ");
  style.append("                             stop: 0 #262626, "); 
  style.append("                             stop: 1 #535353 ); ");
  style.append("color: white; ");
  style.append("} ");

  style.append("QPushButton:checked { ");
  style.append("background: qlineargradient(x1:0,y1:0,x2:0,y2:1, ");
  style.append("                             stop: 0 #262626, "); 
  style.append("                             stop: 1 #535353 ); ");
  style.append("color: white; ");
  style.append("} ");

  setStyleSheet(style);
}

BlueButton::BlueButton(QWidget * parent)
  : QPushButton()
{
  setFlat(true);

  QString style;
  style.append("QPushButton { ");
  style.append("border-radius: 4px; ");
  style.append("border: 1px solid #093A07; ");
  style.append("background: qlineargradient(x1:0,y1:0,x2:0,y2:1, ");
  style.append("                             stop: 0 #C8DAF1, "); 
  style.append("                             stop: 1 #39679E ); ");
  style.append("height: 22px; ");
  style.append("font-size: 12px; ");
  style.append("margin: 0; ");
  style.append("padding-left: 10px; ");
  style.append("padding-right: 10px; ");
  style.append("} ");

  style.append("QPushButton:pressed { ");
  style.append("background: qlineargradient(x1:0,y1:0,x2:0,y2:1, ");
  style.append("                             stop: 0 #262626, "); 
  style.append("                             stop: 1 #535353 ); ");
  style.append("color: white; ");
  style.append("} ");

  style.append("QPushButton:checked { ");
  style.append("background: qlineargradient(x1:0,y1:0,x2:0,y2:1, ");
  style.append("                             stop: 0 #262626, "); 
  style.append("                             stop: 1 #535353 ); ");
  style.append("color: white; ");
  style.append("} ");

  setStyleSheet(style);
}

AddButton::AddButton(QWidget * parent)
  : QPushButton()
{
  setFlat(true);

  setFixedSize(24, 24);

  QString style;
  style.append("QPushButton {"
               "  background-image:url(':/images/add_off.png');"
               "  border:none;"
               "}");
  style.append("QPushButton:hover {"
               "  background-image:url(':/images/add_over.png');"
               "}");
  style.append("QPushButton:pressed {"
               "  background-image:url(':/images/add_press.png');"
               "}");

  setStyleSheet(style);
}

SofterAddButton::SofterAddButton(QWidget * parent)
  : QPushButton()
{
  setFlat(true);

  setFixedSize(16, 16);

  QString style;
  style.append("QPushButton {"
               "  background-image:url(':/shared_gui_components/images/add_softer_off.png');"
               "  border:none;"
               "}");
  style.append("QPushButton:hover {"
               "  background-image:url(':/shared_gui_components/images/add_softer_on.png');"
               "}");
  style.append("QPushButton:pressed {"
               "  background-image:url(':/shared_gui_components/images/add_softer_press.png');"
               "}");

  setStyleSheet(style);
}

RemoveButton::RemoveButton(QWidget * parent)
  : QPushButton()
{
  setFlat(true);

  setFixedSize(24, 24);

  QString style;
  style.append("QPushButton {"
               "  background-image:url(':/images/delete_off.png');"
               "  border:none;"
               "}");
  style.append("QPushButton:hover {"
               "  background-image:url(':/images/delete_over.png');"
               "}");
  style.append("QPushButton:pressed {"
               "  background-image:url(':/images/delete_press.png');"
               "}");

  setStyleSheet(style);
}

SofterRemoveButton::SofterRemoveButton(QWidget * parent)
  : QPushButton()
{
  setFlat(true);

  setFixedSize(16,16);

  QString style;
  style.append("QPushButton {"
               "  background-image:url(':/shared_gui_components/images/delete_softer.png');"
               "  border:none;"
               "}");
  style.append("QPushButton:hover {"
               "  background-image:url(':/shared_gui_components/images/delete_softer_over.png');"
               "}");
  style.append("QPushButton:pressed {"
               "  background-image:url(':/shared_gui_components/images/delete_softer_press.png');"
               "}");

  setStyleSheet(style);
}

AddScriptButton::AddScriptButton(QWidget * parent)
  : QPushButton()
{
  setFlat(true);

  setFixedSize(24,24);

  QString style;
  style.append("QPushButton {"
               "  background-image:url(':/shared_gui_components/images/add_script_icon.png');"
               "  border:none;"
               "}");
  style.append("QPushButton:hover {"
               "  background-image:url(':/shared_gui_components/images/add_script_icon_over.png');"
               "}");
  style.append("QPushButton:pressed {"
               "  background-image:url(':/shared_gui_components/images/add_script_icon_press.png');"
               "}");

  setStyleSheet(style);
}

DuplicateButton::DuplicateButton(QWidget * parent)
  : QPushButton()
{
  setFlat(true);

  setFixedSize(24,24);

  QString style;
  style.append("QPushButton {"
               "  background-image:url(':/shared_gui_components/images/duplicate_off.png');"
               "  border:none;"
               "}");
  style.append("QPushButton:hover {"
               "  background-image:url(':/shared_gui_components/images/duplicate_over.png');"
               "}");
  style.append("QPushButton:pressed {"
               "  background-image:url(':/shared_gui_components/images/duplicate_press.png');"
               "}");
  style.append("QPushButton:disabled {"
               "  background-image:url(':/shared_gui_components/images/duplicate_disabled.png');"
               "}");

  setStyleSheet(style);
}

SofterDuplicateButton::SofterDuplicateButton(QWidget * parent)
  : QPushButton()
{
  setFlat(true);

  setFixedSize(16,16);

  QString style;
  style.append("QPushButton {"
               "  background-image:url(':/shared_gui_components/images/duplicate_softer_off.png');"
               "  border:none;"
               "}");
  style.append("QPushButton:hover {"
               "  background-image:url(':/shared_gui_components/images/duplicate_softer_over.png');"
               "}");
  style.append("QPushButton:pressed {"
               "  background-image:url(':/shared_gui_components/images/duplicate_softer_press.png');"
               "}");
  style.append("QPushButton:disabled {"
               "  background-image:url(':/shared_gui_components/images/duplicate_softer_disabled.png');"
               "}");

  setStyleSheet(style);
}


UpButton::UpButton(QWidget * parent)
  : QPushButton(parent)
{
  setFlat(true);

  setFixedSize(22,24);

  QString style;

  style.append("QPushButton {"
               "  background-image:url(':/shared_gui_components/images/arrow_up.png');"
               "  border:none;"
               "}");
  style.append("QPushButton:hover {"
               "  background-image:url(':/shared_gui_components/images/arrow_up_over.png');"
               "}");
  style.append("QPushButton:pressed {"
               "  background-image:url(':/shared_gui_components/images/arrow_up_press.png');"
               "}");

  setStyleSheet(style);
}

DownButton::DownButton(QWidget * parent)
  : QPushButton(parent)
{
  setFlat(true);

  setFixedSize(22,24);

  QString style;

  style.append("QPushButton {"
               "  background-image:url(':/shared_gui_components/images/arrow_down.png');"
               "  border:none;"
               "}");
  style.append("QPushButton:hover {"
               "  background-image:url(':/shared_gui_components/images/arrow_down_over.png');"
               "}");
  style.append("QPushButton:pressed {"
               "  background-image:url(':/shared_gui_components/images/arrow_down_press.png');"
               "}");

  setStyleSheet(style);
}

MyMeasuresFolderButton::MyMeasuresFolderButton(QWidget * parent)
  : QPushButton(parent)
{
  setFlat(true);

  setFixedSize(31,24);

  QString style;

  style.append("QPushButton {"
               "  background-image:url(':/shared_gui_components/images/open_my_measures.png');"
               "  border:none;"
               "}");
  style.append("QPushButton:pressed {"
               "  background-image:url(':/shared_gui_components/images/open_my_measures_press.png');"
               "}");

  setStyleSheet(style);
}

OpenDirectoryButton::OpenDirectoryButton(QWidget * parent)
  : QPushButton(parent)
{
  setFlat(true);

  setFixedSize(31,24);

  QString style;

  style.append("QPushButton {"
               "  background-image:url(':/shared_gui_components/images/dir.png');"
               "  border:none;"
               "}");
  style.append("QPushButton:pressed {"
               "  background-image:url(':/shared_gui_components/images/dir_press.png');"
               "}");
  style.append("QPushButton:disabled {"
               "  background-image:url(':/shared_gui_components/images/dir_disabled.png');"
               "}");

  setStyleSheet(style);
}

CreateOneWithSelectedMeasuresButton::CreateOneWithSelectedMeasuresButton()
  : GrayButton()
{
  setText("Create One With Selected Measures");
}

CreateOneForEachSelectedMeasureButton::CreateOneForEachSelectedMeasureButton()
  : GrayButton()
{
  setText("Create One For Each Selected Measure");
}

CreateFromFileButton::CreateFromFileButton()
  : GrayButton()
{
  setText("Create From External File");
}

CloudLostConnectionButton::CloudLostConnectionButton(QWidget * parent)
  : QPushButton(parent)
{
  setFlat(true);

  setFixedSize(130,24);

  QString style;

  style.append("QPushButton {"
               "  background-image:url(':/shared_gui_components/images/cloud_lost_connection.png');"
               "  border:none;"
               "}");

  setStyleSheet(style);

}
CloudOffButton::CloudOffButton(QWidget * parent)
  : QPushButton(parent)
{
  setFlat(true);

  setFixedSize(130,24);

  QString style;

  style.append("QPushButton {"
               "  background-image:url(':/shared_gui_components/images/cloud_off.png');"
               "  border:none;"
               "}");
  style.append("QPushButton:disabled {"
               "  background-image:url(':/shared_gui_components/images/cloud_off_disabled.png');"
               "}");

  setStyleSheet(style);
}

CloudOnButton::CloudOnButton(QWidget * parent)
  : QPushButton(parent)
{
  setFlat(true);

  setFixedSize(130,24);

  QString style;

  style.append("QPushButton {"
               "  background-image:url(':/shared_gui_components/images/cloud_on.png');"
               "  border:none;"
               "}");
  style.append("QPushButton:disabled {"
               "  background-image:url(':/shared_gui_components/images/cloud_on_disabled.png');"
               "}");

  setStyleSheet(style);
}

CloudStartingButton::CloudStartingButton(QWidget * parent)
  : QPushButton(parent)
{
  setFlat(true);

  setFixedSize(130,24);

  QString style;

  style.append("QPushButton {"
               "  border:none;"
               "}");

  setStyleSheet(style);

  m_background = new QPixmap(":/shared_gui_components/images/cloud_starting.png");
  m_arrow = new QPixmap(":/shared_gui_components/images/cloud_arrow.png");

  m_rotation = 0.0;

  m_timer = new QTimer(this);
  connect(m_timer, &QTimer::timeout, this, &CloudStartingButton::rotate);
  m_timer->start(50);
}

void CloudStartingButton::rotate()
{
  m_rotation += 10.0;

  update();
}

void CloudStartingButton::paintEvent ( QPaintEvent * event )
{
  QPainter painter(this);

  double transX = m_arrow->width()/2.0;
  double transY = m_arrow->height()/2.0;

  QTransform t;
  t.translate(transX, transY);
  t.rotate(m_rotation);
  t.translate(-transX, -transY);

  QPixmap rotatedArrow = QPixmap(m_arrow->transformed(t, Qt::SmoothTransformation));

  double transX2 = rotatedArrow.width()/2.0;
  double transY2 = rotatedArrow.height()/2.0;

  painter.drawPixmap(0, 0, *m_background);
  painter.drawPixmap(12+transX-transX2, 5+transY-transY2, rotatedArrow);
}

CloudStoppingButton::CloudStoppingButton(QWidget * parent)
  : QPushButton(parent)
{
  setFlat(true);

  setFixedSize(130,24);

  QString style;

  style.append("QPushButton {"
               "  border:none;"
               "}");

  setStyleSheet(style);

  m_background = new QPixmap(":/shared_gui_components/images/cloud_stopping.png");
  m_arrow = new QPixmap(":/shared_gui_components/images/cloud_arrow_magenta.png");

  m_rotation = 0.0;

  m_timer = new QTimer(this);
  connect(m_timer, &QTimer::timeout, this, &CloudStoppingButton::rotate);
  m_timer->start(50);
}

void CloudStoppingButton::rotate()
{
  m_rotation += 10.0;

  update();
}

void CloudStoppingButton::paintEvent ( QPaintEvent * event )
{
  QPainter painter(this);

  double transX = m_arrow->width()/2.0;
  double transY = m_arrow->height()/2.0;

  QTransform t;
  t.translate(transX, transY);
  t.rotate(m_rotation);
  t.translate(-transX, -transY);

  QPixmap rotatedArrow = QPixmap(m_arrow->transformed(t, Qt::SmoothTransformation));

  double transX2 = rotatedArrow.width()/2.0;
  double transY2 = rotatedArrow.height()/2.0;

  painter.drawPixmap(0, 0, *m_background);
  painter.drawPixmap(12+transX-transX2, 5+transY-transY2, rotatedArrow);
}

PlayButton::PlayButton(QWidget * parent)
  : QPushButton(parent)
{
  setFlat(true);
  setFixedSize(35,35);

  setStatus(IDLE);
}

PlayButton::Status PlayButton::status() const
{
  return m_status;
}

void PlayButton::setStatus(const Status & status)
{
  QString style;

  switch (status)
  {
    case IDLE:
      style.append("QPushButton {"
                   "  background-image:url(':/shared_gui_components/images/run_simulation_button.png');"
                   "  border:none;"
                   "}");
      style.append("QPushButton:hover {"
                   "  background-image:url(':/shared_gui_components/images/run_simulation_over.png');"
                   "}");
      style.append("QPushButton:pressed {"
                   "  background-image:url(':/shared_gui_components/images/run_simulation_press.png');"
                   "}");
      setStyleSheet(style);
      setEnabled(true);
      break;
    case IDLEDISABLED:
      style.append("QPushButton {"
                   "  background-image:url(':/shared_gui_components/images/run_simulation_button_disabled.png');"
                   "  border:none;"
                   "}");
      setStyleSheet(style);
      setEnabled(false);
      break;
    case STARTING:
      style.clear();
      style.append("QPushButton {"
                   "  background-image:url(':/shared_gui_components/images/starting_simulation_button.png');"
                   "  border:none;"
                   "}");

      setStyleSheet(style);
      setEnabled(false);
      break;
    case RUNNING:
      style.clear();
      style.append("QPushButton {"
                   "  background-image:url(':/shared_gui_components/images/run_cancel.png');"
                   "  border:none;"
                   "}");
      style.append("QPushButton:hover {"
                   "  background-image:url(':/shared_gui_components/images/run_cancel_over.png');"
                   "}");
      style.append("QPushButton:pressed {"
                   "  background-image:url(':/shared_gui_components/images/run_cancel_press.png');"
                   "}");

      setStyleSheet(style);
      setEnabled(true);
      break;
    case STOPPING:
      style.clear();
      style.append("QPushButton {"
                   "  background-image:url(':/shared_gui_components/images/stopping_simulation_button.png');"
                   "  border:none;"
                   "}");

      setStyleSheet(style);
      setEnabled(false);
      break;
    case ERROR:
      style.clear();
      style.append("QPushButton {"
                   "  background-image:url(':/shared_gui_components/images/run_simulation_button.png');"
                   "  border:none;"
                   "}");
      style.append("QPushButton:hover {"
                   "  background-image:url(':/shared_gui_components/images/run_simulation_over.png');"
                   "}");
      style.append("QPushButton:pressed {"
                   "  background-image:url(':/shared_gui_components/images/run_simulation_press.png');"
                   "}");
      setStyleSheet(style);
      setEnabled(true);
      break;
  }

  m_status = status;
}


} // openstudio

