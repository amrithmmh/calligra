/* This file is part of the KDE project

   Copyright (C) 2006 Thorsten Zachmann <zachmann@kde.org>
   Copyright (C) 2006-2007 Thomas Zander <zander@kde.org>

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
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
*/

#ifndef SHAPEMOVESTRATEGY_H
#define SHAPEMOVESTRATEGY_H

#include <KoInteractionStrategy.h>

#include <QPointF>
#include <QList>
#include <QVector>

class KoCanvasBase;
class KoToolBase;
class KoShape;

/**
 * Implements the Move action on an object or selected objects.
 */
class ShapeMoveStrategy : public KoInteractionStrategy
{
public:
    /**
     * Constructor that starts to move the objects.
     * @param tool the parent tool which controls this strategy
     * @param canvas the canvas interface which will supply things like a selection object
     * @param clicked the initial point that the user depressed (in pt).
     */
    ShapeMoveStrategy(KoToolBase *tool, const QPointF &clicked);
    ~ShapeMoveStrategy() override {}

    void handleMouseMove(const QPointF &mouseLocation, Qt::KeyboardModifiers modifiers) override;
    KUndo2Command* createCommand() override;
    void finishInteraction(Qt::KeyboardModifiers modifiers) override;
    void paint( QPainter &painter, const KoViewConverter &converter) override;
    void handleCustomEvent( KoPointerEvent * event ) override;
private:
    void moveSelection();
    QVector<QPointF> m_previousPositions;
    QVector<QPointF> m_newPositions;
    QVector<QPointF> m_previousOffsets;
    QVector<QPointF> m_newOffsets;
    QPointF m_start, m_diff, m_initialSelectionPosition, m_initialOffset;
    QList<KoShape*> m_selectedShapes;
    KoCanvasBase *m_canvas;
    bool m_firstMove;
};

#endif
