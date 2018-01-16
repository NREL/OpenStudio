/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef OPENSTUDIO_REFRIGERATIONGRAPHICSITEMS_HPP
#define OPENSTUDIO_REFRIGERATIONGRAPHICSITEMS_HPP

#include <QGraphicsObject>
#include "OSItem.hpp"
#include "OSDropZone.hpp"
#include "../shared_gui_components/OSListController.hpp"
#include "../shared_gui_components/OSListView.hpp"
#include "../shared_gui_components/GraphicsItems.hpp"
#include "../utilities/idf/Handle.hpp"

class QGraphicsView;
class QPushButton;
class QLabel;

namespace openstudio {

class RefrigerationCondenserView;
class RefrigerationSubCoolerView;
class RefrigerationHeatReclaimView;
class RefrigerationCompressorView;
class RefrigerationSHXView;
class RefrigerationCasesView;
class RefrigerationCaseDetailView;
class RefrigerationCasesDropZoneView;
class RefrigerationSystemView;
class RefrigerationSecondaryView;
class RefrigerationCompressorDropZoneView;
class OSItemId;

class RefrigerationView : public QWidget
{
  Q_OBJECT;

  public:

  RefrigerationView();

  QWidget * header;

  QGraphicsView * graphicsView;

  QPushButton * zoomOutButton;

  QLabel * nameLabel;
};

class CaseViewExpandButton : public AbstractButtonItem
{
  Q_OBJECT;

  public:

  CaseViewExpandButton();

  virtual ~CaseViewExpandButton() {}

  QRectF boundingRect() const override;

  static QSize size();

  protected:

  void paint(QPainter *painter,
             const QStyleOptionGraphicsItem *option,
             QWidget *widget = nullptr) override;

  private:

  QPixmap m_openImage;

  QPixmap m_closeImage;
};

// A cell of the refrigeration system grid
// This is an item that contains a RefrigerationSystemItem plus some overlays
class RefrigerationSystemMiniView : public QGraphicsObject
{
  Q_OBJECT;

  public:

  RefrigerationSystemMiniView();

  virtual ~RefrigerationSystemMiniView() {}

  RefrigerationSystemView * refrigerationSystemView;

  RemoveButtonItem * removeButtonItem;

  ZoomInButtonItem * zoomInButtonItem;

  QRectF boundingRect() const override;

  void adjustLayout();

  static QSize cellSize();

  public slots:

  void setName( const QString & name);

  protected:

  void paint( QPainter *painter,
              const QStyleOptionGraphicsItem *option,
              QWidget *widget ) override;

  private:

  QRectF contentRect() const;

  QRectF headerRect() const;

  static int cellWidth();

  static int headerHeight();

  QString m_name;
};

class RefrigerationSystemDetailView : public QGraphicsObject
{
  Q_OBJECT;

  public:

  RefrigerationSystemDetailView();

  virtual ~RefrigerationSystemDetailView() {}

  RefrigerationSystemView * refrigerationSystemView;

  ZoomOutButtonItem * zoomOutButton;

  QRectF boundingRect() const override;

  protected:

  void paint( QPainter *painter,
              const QStyleOptionGraphicsItem *option,
              QWidget *widget ) override;

  private:

  // QRectF contentRect() const;

  // QRectF headerRect() const;

  // static int cellWidth();

  // static int headerHeight();

  // QString m_name;
};

class RefrigerationSystemDropZoneView : public OSDropZoneItem
{
  Q_OBJECT;

  public:

  QRectF boundingRect() const override;

  protected:

  void paint( QPainter *painter,
              const QStyleOptionGraphicsItem *option,
              QWidget *widget ) override;
};

class RefrigerationSystemView : public QGraphicsObject
{
  Q_OBJECT;

  public:

  RefrigerationSystemView();

  virtual ~RefrigerationSystemView() {}

  static const int verticalSpacing;

  static const int margin;

  static const double componentHeight;

  RefrigerationCondenserView * refrigerationCondenserView;

  RefrigerationSubCoolerView * refrigerationSubCoolerView;

  RefrigerationHeatReclaimView * refrigerationHeatReclaimView;

  RefrigerationCompressorView * refrigerationCompressorView;

  RefrigerationSHXView * refrigerationSHXView;

  RefrigerationCasesView * refrigerationCasesView;

  RefrigerationSecondaryView * refrigerationSecondaryView;

  void adjustLayout();

  // RefrigerationSystemItem is divided into three main colums.
  // These are the x coordinates of the left, center, and right center lines

  int leftXPos() const;

  int centerXPos() const;

  int rightXPos() const;

  QRectF boundingRect() const override;

  void setId(const OSItemId & id);

  signals:

  void inspectClicked(OSItemId id);

  public slots:

  void setCasesExpanded(bool expanded);

  protected:

  void paint( QPainter *painter,
              const QStyleOptionGraphicsItem *option,
              QWidget *widget = nullptr ) override;

  void mousePressEvent(QGraphicsSceneMouseEvent * event) override;

  void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) override;

  bool m_mouseDown;

  private:

  OSItemId m_id;
};

class RefrigerationCondenserView : public RefrigerationSystemDropZoneView
{
  Q_OBJECT;

  public:

  RefrigerationCondenserView();

  virtual ~RefrigerationCondenserView() {}

  RemoveButtonItem * removeButtonItem;

  QRectF boundingRect() const override;

  void setCondenserId(const OSItemId & condenserId);

  void setCondenserName(const QString & name);

  void setIcon(const QPixmap & pixmap);

  static QSizeF size();

  signals:

  void removeClicked(OSItemId id);

  void inspectClicked(OSItemId id);

  protected:

  void paint( QPainter *painter,
              const QStyleOptionGraphicsItem *option,
              QWidget *widget = nullptr ) override;

  void mousePressEvent(QGraphicsSceneMouseEvent * event) override;

  void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) override;

  bool m_mouseDown;

  private slots:

  void onRemoveButtonClicked();

  private:

  OSItemId m_id;

  QString m_name;

  QPixmap m_pixmap;
};

class RefrigerationCompressorDetailView : public QGraphicsObject
{
  Q_OBJECT;

  public:

  RefrigerationCompressorDetailView();

  virtual ~RefrigerationCompressorDetailView() {}

  RemoveButtonItem * removeButtonItem;

  static QSizeF size();

  QRectF boundingRect() const override;

  void setLabel(const QString & label);

  void setId(const OSItemId & id);

  signals:

  void removeClicked(OSItemId id);

  void inspectClicked(OSItemId id);

  protected:

  void paint( QPainter *painter,
              const QStyleOptionGraphicsItem *option,
              QWidget *widget = nullptr ) override;

  void mousePressEvent(QGraphicsSceneMouseEvent * event) override;

  void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) override;

  bool m_mouseDown;

  private slots:

  void onRemoveButtonClicked();

  private:

  QString m_label;

  OSItemId m_id;

  QPixmap m_pixmap;
};

class RefrigerationCompressorDropZoneView : public RefrigerationSystemDropZoneView
{
  Q_OBJECT;

  public:

  RefrigerationCompressorDropZoneView();

  virtual ~RefrigerationCompressorDropZoneView() {}

  QRectF boundingRect() const override;

  protected:

  void paint( QPainter *painter,
              const QStyleOptionGraphicsItem *option,
              QWidget *widget = nullptr ) override;
};

class RefrigerationCompressorView : public QGraphicsObject
{
  Q_OBJECT;

  public:

  RefrigerationCompressorView();

  virtual ~RefrigerationCompressorView() {}

  QRectF boundingRect() const override;

  RefrigerationCompressorDropZoneView * refrigerationCompressorDropZoneView;

  void insertCompressorDetailView(int index, QGraphicsObject * object);

  void removeAllCompressorDetailViews();

  void adjustLayout();

  static double height();

  protected:

  void paint( QPainter *painter,
              const QStyleOptionGraphicsItem *option,
              QWidget *widget = nullptr ) override;

  private:

  std::vector<QGraphicsObject *> m_compressorDetailViews;
};

class RefrigerationCasesDropZoneView : public RefrigerationSystemDropZoneView
{
  Q_OBJECT;

  public:

  RefrigerationCasesDropZoneView();

  virtual ~RefrigerationCasesDropZoneView() {}

  static QSizeF size();

  QRectF boundingRect() const override;

  protected:

  void paint( QPainter *painter,
              const QStyleOptionGraphicsItem *option,
              QWidget *widget = nullptr ) override;
};

class RefrigerationCaseDetailView : public QGraphicsObject
{
  Q_OBJECT;

  public:

  RefrigerationCaseDetailView();

  virtual ~RefrigerationCaseDetailView() {}

  RemoveButtonItem * removeButtonItem;

  static QSizeF size();

  static QRectF nameRect();

  static QRectF iconRect();

  QRectF boundingRect() const override;

  void setName(const QString & name);

  void setId(const OSItemId & id);

  signals:

  void removeClicked(OSItemId id);

  void inspectClicked(OSItemId id);

  protected:

  void paint( QPainter *painter,
              const QStyleOptionGraphicsItem *option,
              QWidget *widget = nullptr ) override;

  void mousePressEvent(QGraphicsSceneMouseEvent * event) override;

  void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) override;

  bool m_mouseDown;

  private slots:

  void onRemoveButtonClicked();

  private:

  QString m_name;

  OSItemId m_id;

  QPixmap m_displayCasesPixmap;

  QPixmap m_walkinPixmap;
};

class RefrigerationCasesView : public QGraphicsObject
{
  Q_OBJECT;

  public:

  RefrigerationCasesView();

  virtual ~RefrigerationCasesView() {}

  QRectF boundingRect() const override;

  RefrigerationCasesDropZoneView * refrigerationCasesDropZoneView;

  CaseViewExpandButton * expandButton;

  void setNumberOfDisplayCases(int number);

  void setNumberOfWalkinCases(int number);

  void insertCaseDetailView(int index, QGraphicsObject * object);

  void removeAllCaseDetailViews();

  void adjustLayout();

  static QRectF displayCasesRect();

  static QRectF walkinCasesRect();

  static QRectF summaryRect();

  public slots:

  void setExpanded(bool exapanded);

  protected:

  void paint( QPainter *painter,
              const QStyleOptionGraphicsItem *option,
              QWidget *widget = nullptr ) override;

  private:

  QPointF casePos(int index) const;

  int m_numberOfDisplayCases;

  int m_numberOfWalkinCases;

  QSizeF size() const;

  bool m_expanded;

  std::vector<QGraphicsObject *> m_caseDetailViews;

  QPixmap m_displayCasesPixmap;

  QPixmap m_walkinPixmap;
};

class RefrigerationSubCoolerView : public RefrigerationSystemDropZoneView
{
  Q_OBJECT;

  public:

  RefrigerationSubCoolerView();

  virtual ~RefrigerationSubCoolerView() {}

  RemoveButtonItem * removeButtonItem;

  QRectF boundingRect() const override;

  void setId(const OSItemId & id);

  void setName(const QString & name);

  static QSizeF size();

  signals:

  void removeClicked(OSItemId id);

  void inspectClicked(OSItemId id);

  protected:

  void paint( QPainter *painter,
              const QStyleOptionGraphicsItem *option,
              QWidget *widget = nullptr ) override;

  void mousePressEvent(QGraphicsSceneMouseEvent * event) override;

  void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) override;

  bool m_mouseDown;

  private slots:

  void onRemoveButtonClicked();

  private:

  OSItemId m_id;

  QString m_name;

  QPixmap m_pixmap;
};

class RefrigerationHeatReclaimView : public RefrigerationSystemDropZoneView
{
  Q_OBJECT;

  public:

  QRectF boundingRect() const override;

  protected:

  void paint( QPainter *painter,
              const QStyleOptionGraphicsItem *option,
              QWidget *widget = nullptr ) override;
};

class RefrigerationSHXView : public RefrigerationSystemDropZoneView
{
  Q_OBJECT;

  public:

  RefrigerationSHXView();

  virtual ~RefrigerationSHXView() {}

  RemoveButtonItem * removeButtonItem;

  QRectF boundingRect() const override;

  void setId(const OSItemId & id);

  void setName(const QString & name);

  static QSizeF size();

  signals:

  void removeClicked(OSItemId id);

  void inspectClicked(OSItemId id);

  protected:

  void paint( QPainter *painter,
              const QStyleOptionGraphicsItem *option,
              QWidget *widget = nullptr ) override;

  void mousePressEvent(QGraphicsSceneMouseEvent * event) override;

  void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) override;

  bool m_mouseDown;

  private slots:

  void onRemoveButtonClicked();

  private:

  OSItemId m_id;

  QString m_name;

  QPixmap m_pixmap;
};

class SecondaryDropZoneView : public RefrigerationSystemDropZoneView
{
  Q_OBJECT;

  public:

  QRectF boundingRect() const override;

  protected:

  void paint( QPainter *painter,
              const QStyleOptionGraphicsItem *option,
              QWidget *widget ) override;
};

class SecondaryDetailView : public QGraphicsObject
{
  Q_OBJECT;

  public:

  SecondaryDetailView();

  virtual ~SecondaryDetailView() {}

  ZoomInButtonItem * zoomInButtonItem;
  RemoveButtonItem * removeButtonItem;

  QRectF boundingRect() const override;
  QRectF nameRect();

  void setName(const QString & name);
  void setHandle(const Handle & handle);

  static double width();
  static double height();

  signals:

  void zoomInOnSystemClicked(const Handle & handle);
  void removeClicked(const Handle & handle);

  protected:

  void paint( QPainter *painter,
              const QStyleOptionGraphicsItem *option,
              QWidget *widget ) override;

  private slots:

  void onZoomButtonClicked();
  void onRemoveButtonClicked();

  private:

  QString m_name;
  Handle m_handle;
};

class RefrigerationSecondaryView : public QGraphicsObject
{
  Q_OBJECT;

  public:

  RefrigerationSecondaryView();

  virtual ~RefrigerationSecondaryView() {}

  QRectF boundingRect() const override;

  SecondaryDropZoneView * secondaryDropZoneView;

  void insertSecondaryDetailView(int index, QGraphicsObject * object);

  void removeAllSecondaryDetailViews();

  void adjustLayout();

  protected:

  void paint( QPainter *painter,
              const QStyleOptionGraphicsItem *option,
              QWidget *widget = nullptr ) override;

  private:

  std::vector<QGraphicsObject *> m_secondaryDetailViews;
  int m_height;
};

} // openstudio

#endif // OPENSTUDIO_REFRIGERATIONGRAPHICSITEMS_HPP

