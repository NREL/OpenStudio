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

#ifndef SHAREDGUICOMPONENTS_BUTTONS_HPP
#define SHAREDGUICOMPONENTS_BUTTONS_HPP

#include <QPushButton>

#include <boost/smart_ptr.hpp>

class QPaintEvent;

class QPixmap;

class QTimer;

namespace openstudio {

class ZoomOutButton : public QPushButton
{
  Q_OBJECT

  public:

  ZoomOutButton(QWidget * parent = nullptr);
  virtual ~ZoomOutButton() {}
};

class OrangeButton : public QPushButton
{
  Q_OBJECT

  public:

  OrangeButton(QWidget * parent = nullptr);
  virtual ~OrangeButton() {}
};

class GrayButton : public QPushButton
{
  Q_OBJECT

  public:

  GrayButton(QWidget * parent = nullptr);
  virtual ~GrayButton() {}
};

class BlueButton : public QPushButton
{
  Q_OBJECT

  public:

  BlueButton(QWidget * parent = nullptr);
  virtual ~BlueButton() {}
};

class AddButton : public QPushButton
{
  Q_OBJECT

public:

  AddButton(QWidget * parent = nullptr);
  virtual ~AddButton() {}
};

class SofterAddButton : public QPushButton
{
  Q_OBJECT

public:

  SofterAddButton(QWidget * parent = nullptr);
  virtual ~SofterAddButton() {}
};

class RemoveButton : public QPushButton
{
  Q_OBJECT
  
  public:

  RemoveButton(QWidget * parent = nullptr);
  virtual ~RemoveButton() {}
};

class SofterRemoveButton : public QPushButton
{
  Q_OBJECT
  
  public:

  SofterRemoveButton(QWidget * parent = nullptr);
  virtual ~SofterRemoveButton() {}
};


class AddScriptButton : public QPushButton
{
  Q_OBJECT
  
  public:

  AddScriptButton(QWidget * parent = nullptr);
  virtual ~AddScriptButton() {}
};

class DuplicateButton : public QPushButton
{
  Q_OBJECT

  public:

  DuplicateButton(QWidget * parent = nullptr);
  virtual ~DuplicateButton() {}
};

class SofterDuplicateButton : public QPushButton
{
  Q_OBJECT
 
 public:
  
  SofterDuplicateButton(QWidget * parent = nullptr);
  virtual ~SofterDuplicateButton() {}
};

class UpButton : public QPushButton
{
  Q_OBJECT

  public:

  UpButton(QWidget * parent = nullptr);
  virtual ~UpButton() {}
};

class DownButton : public QPushButton
{
  Q_OBJECT

  public:

  DownButton(QWidget * parent = nullptr);
  virtual ~DownButton() {}
};

class MyMeasuresFolderButton : public QPushButton
{
  Q_OBJECT

  public:

  MyMeasuresFolderButton(QWidget * parent = nullptr);
  virtual ~MyMeasuresFolderButton() {}
};

class OpenDirectoryButton : public QPushButton
{
  Q_OBJECT

  public:

  OpenDirectoryButton(QWidget * parent = nullptr);
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

  CloudLostConnectionButton(QWidget * parent = nullptr);
  virtual ~CloudLostConnectionButton() {}
};

class CloudOffButton : public QPushButton
{
  Q_OBJECT

  public:

  CloudOffButton(QWidget * parent = nullptr);
  virtual ~CloudOffButton() {}
};

class CloudOnButton : public QPushButton
{
  Q_OBJECT

  public:

  CloudOnButton(QWidget * parent = nullptr);
  virtual ~CloudOnButton() {}
};

class CloudStartingButton : public QPushButton
{
  Q_OBJECT

  public:

  CloudStartingButton(QWidget * parent = nullptr);
  virtual ~CloudStartingButton() {}

  public slots:

  void rotate();

  protected:

  void paintEvent ( QPaintEvent * event ) override;

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

  CloudStoppingButton(QWidget * parent = nullptr);
  virtual ~CloudStoppingButton() {}

  public slots:

  void rotate();

  protected:

  void paintEvent ( QPaintEvent * event ) override;

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

  enum Status { IDLE, IDLEDISABLED, STARTING, RUNNING, STOPPING, ERROR };

  PlayButton(QWidget * parent = nullptr);
  virtual ~PlayButton() {}

  Status status() const;
  void setStatus(const Status & status);

  private:

  Status m_status;
};

} // openstudio

#endif // SHAREDGUICOMPONENTS_BUTTONS_HPP

