/* This file is part of the KDE project
   Copyright (C) 2001, The Karbon Developers
   Copyright (C) 2002, The Karbon Developers
*/

#ifndef __KARBON_PART_H__
#define __KARBON_PART_H__

#include <qptrlist.h>
#include <koDocument.h>

#include "vdocument.h"
#include "vlayer.h"
#include "vcolor.h"

class QRect;

class VCommand;
class VCommandHistory;
class DCOPObject;

namespace Karbon
{
	const short m_copyOffset = 10;
}

class KarbonPart : public KoDocument
{
	Q_OBJECT
public:
	KarbonPart( QWidget* parentWidget = 0, const char* widgetName = 0,
		QObject* parent = 0, const char* name = 0, bool singleViewMode = false );
	virtual ~KarbonPart();

	virtual void paintContent( QPainter& painter, const QRect& rect,
		bool transparent = false, double zoomX = 1.0, double zoomY = 1.0 );

	virtual bool initDoc();

	virtual bool loadXML( QIODevice*, const QDomDocument& document );
	virtual QDomDocument saveXML();

    virtual DCOPObject* dcopObject();

	// access document:
	VDocument& document() { return m_doc; }

	// inserting, deleting objects:
	void insertObject( const VObject* object );	// insert a new vobject:

	// insert a command into the undo/redo-history:
	void addCommand( VCommand* cmd, bool repaint = false );

	// remove all vobjects which are marked "deleted" and clear command-history:
	void purgeHistory();

	// layers:
	const QPtrList<VLayer>& layers() const { return m_doc.layers(); }	// r/o access.
	// TODO: still needed?
	VLayer* activeLayer() const { return m_activeLayer; }	// active layer.

	// move up/down within layer
	void moveSelectionToTop();
	void moveSelectionToBottom();
	void moveSelectionDown();
	void moveSelectionUp();

	void setDefaultStrokeColor( const VColor &color ) { m_defaultStrokeColor = color; }
	void setDefaultFillColor( const VColor &color ) { m_defaultFillColor = color; }
	void applyDefaultColors( VObject & ) const;

	bool showStatusBar() const { return m_bShowStatusBar; }
	void setShowStatusBar( bool b );
	void reorganizeGUI();

	void setUndoRedoLimit( int undos );

	void initConfig();
	int maxRecentFiles() const { return m_maxRecentFiles; }

public slots:
	void repaintAllViews( bool erase = false );
	void slotDocumentRestored();
	void slotCommandExecuted();

protected:
	virtual KoView* createViewInstance( QWidget* parent, const char* name );

private:
	VDocument m_doc;
	VLayer* m_activeLayer;				// the active/current layer.

	VCommandHistory* m_commandHistory;	// everybody loves undo/redo.

	VColor m_defaultStrokeColor;
	VColor m_defaultFillColor;

	bool m_bShowStatusBar;
	int m_maxRecentFiles;
	DCOPObject *dcop;
};

#endif
