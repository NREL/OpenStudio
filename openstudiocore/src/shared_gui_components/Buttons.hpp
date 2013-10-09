/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

#ifndef OPENSTUDIO_BUTTONS_H
#define OPENSTUDIO_BUTTONS_H

#include <QPushButton>

#include <boost/smart_ptr.hpp>

class QPaintEvent;

class QPixmap;

class QTimer;

namespace openstudio {

class GrayButton : public QPushButton
{
  Q_OBJECT

  public:

  GrayButton(QWidget * parent = 0);
  virtual ~GrayButton() {}
};

class BlueButton : public QPushButton
{
  Q_OBJECT

  public:

  BlueButton(QWidget * parent = 0);
  virtual ~BlueButton() {}
};

class RemoveButton : public QPushButton
{
  Q_OBJECT
  
  public:

  RemoveButton(QWidget * parent = 0);
  virtual ~RemoveButton() {}
};

class SofterRemoveButton : public QPushButton
{
  Q_OBJECT
  
  public:

  SofterRemoveButton(QWidget * parent = 0);
  virtual ~SofterRemoveButton() {}
};


class AddScriptButton : public QPushButton
{
  Q_OBJECT
  
  public:

  AddScriptButton(QWidget * parent = 0);
  virtual ~AddScriptButton() {}
};

class DuplicateButton : public QPushButton
{
  Q_OBJECT

  public:

  DuplicateButton(QWidget * parent = 0);
  virtual ~DuplicateButton() {}
};

class UpButton : public QPushButton
{
  Q_OBJECT

  public:

  UpButton(QWidget * parent = 0);
  virtual ~UpButton() {}
};

class DownButton : public QPushButton
{
  Q_OBJECT

  public:

  DownButton(QWidget * parent = 0);
  virtual ~DownButton() {}
};

class MyMeasuresFolderButton : public QPushButton
{
  Q_OBJECT

  public:

  MyMeasuresFolderButton(QWidget * parent = 0);
  virtual ~MyMeasuresFolderButton() {}
};

class OpenDirectoryButton : public QPushButton
{
  Q_OBJECT

  public:

  OpenDirectoryButton(QWidget * parent = 0);
  virtual ~OpenDirectoryButton() {}
};

class CreateOneWithSelectedMeasuresButton : public GrayButton
{
  Q_OBJECT

  public:

  CreateOneWithSelectedMeasuresButton();
  virtual ~CreateOneWithSelectedMeasuresButton() {}
};

class CreateOneForEachSelectedMeasureButton : public GrayButton
{
  Q_OBJECT

  public:

  CreateOneForEachSelectedMeasureButton();
  virtual ~CreateOneForEachSelectedMeasureButton() {}
};

class CreateFromFileButton : public GrayButton
{
  Q_OBJECT

  public:

  CreateFromFileButton();
  virtual ~CreateFromFileButton() {}
};

class CloudLostConnectionButton : public QPushButton
{
  Q_OBJECT

  public:

  CloudLostConnectionButton(QWidget * parent = 0);
  virtual ~CloudLostConnectionButton() {}
};

class CloudOffButton : public QPushButton
{
  Q_OBJECT

  public:

  CloudOffButton(QWidget * parent = 0);
  virtual ~CloudOffButton() {}
};

class CloudOnButton : public QPushButton
{
  Q_OBJECT

  public:

  CloudOnButton(QWidget * parent = 0);
  virtual ~CloudOnButton() {}
};

class CloudStartingButton : public QPushButton
{
  Q_OBJECT

  public:

  CloudStartingButton(QWidget * parent = 0);
  virtual ~CloudStartingButton() {}

  public slots:

  void rotate();

  protected:

  void paintEvent ( QPaintEvent * event );

  private:

  QPixmap * m_background;

  QPixmap * m_arrow;

  float m_rotation;

  QTimer * m_timer;
};

class CloudStoppingButton : public QPushButton
{
  Q_OBJECT

  public:

  CloudStoppingButton(QWidget * parent = 0);
  virtual ~CloudStoppingButton() {}

  public slots:

  void rotate();

  protected:

  void paintEvent ( QPaintEvent * event );

  private:

  QPixmap * m_background;

  QPixmap * m_arrow;

  float m_rotation;

  QTimer * m_timer;

};

class PlayButton : public QPushButton
{
  Q_OBJECT

  public:

  enum Status { IDLE, STARTING, RUNNING, STOPPING, ERROR };

  PlayButton(QWidget * parent = 0);
  virtual ~PlayButton() {}

  Status status() const;
  void setStatus(const Status & status);

  private:

  Status m_status;
};

} // openstudio

#endif // OPENSTUDIO_BUTTONS_H

