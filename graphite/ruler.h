/* This file is part of the KDE project
   Copyright (C) 1998, 1999 Reginald Stadlbauer <reggie@kde.org>
                 2001 Werner Trobin <trobin@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#ifndef ruler_h
#define ruler_h

#include <qframe.h>
#include <qpixmap.h>
#include <graphiteglobal.h>

class QPopupMenu;

class Ruler : public QFrame {

    Q_OBJECT

public:
    Ruler(QWidget *parent, QWidget *canvas, Qt::Orientation orientation,
          const Graphite::PageLayout &layout, const double &zoomedRes);
    ~Ruler();

    void setUnit(const Graphite::Unit &unit);
    void setPageLayout(const Graphite::PageLayout &layout) { m_layout=layout; repaint(false); }

    void showMousePos(bool showMPos);
    // Not zoomed - real pixel coords!
    void setMousePos(int mx, int my);

    void setOffset(int dx, int dy) { m_dx=dx; m_dy=dy; repaint(false); }

    void setEditable(bool editable) { m_editable=editable; }
    bool editable() const { return m_editable; }

    void setZoomedRes(const double &zoomedRes);

signals:
    void pageBordersChanged(const Graphite::PageBorders &);
    void unitChanged(Graphite::Unit);
    void openPageLayoutDia();

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *);

    void drawContents(QPainter *p) { m_orientation==Qt::Horizontal ? drawHorizontal(p) : drawVertical(p); }

private slots:
    void rbPT() { setUnit(Graphite::Pt); emit unitChanged(Graphite::Pt); }
    void rbMM() { setUnit(Graphite::MM); emit unitChanged(Graphite::MM); }
    void rbINCH() { setUnit(Graphite::Inch); emit unitChanged(Graphite::Inch); }

private:
    Ruler(const Ruler &rhs);
    Ruler &operator=(const Ruler &rhs);
    enum Action { A_NONE, A_BR_LEFT, A_BR_RIGHT, A_BR_TOP, A_BR_BOTTOM };

    void drawHorizontal(QPainter *p);
    void drawVertical(QPainter *p);

    QWidget *m_canvas;
    Qt::Orientation m_orientation;
    Graphite::PageLayout m_layout;
    double m_zoomedRes;
    double m_1_zoomedRes; // 1/m_zoomedRes
    Graphite::Unit m_unit;
    int m_dx, m_dy;
    int m_MX, m_MY;
    int m_oldMX, m_oldMY;
    Ruler::Action m_action;
    unsigned short m_mousePressed : 1;
    unsigned short m_showMPos : 1;
    unsigned short m_haveToDelete : 1;
    unsigned short m_movingFirstBorder : 1;
    unsigned short m_movingSecondBorder : 1;
    unsigned short m_editable : 1;
    QPopupMenu *m_menu;
    int m_MMIndex, m_PTIndex, m_INCHIndex;

};

#endif
