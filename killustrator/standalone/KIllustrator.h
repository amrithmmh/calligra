/* -*- C++ -*-

  $Id$

  This file is part of KIllustrator.
  Copyright (C) 1998 Kai-Uwe Sattler (kus@iti.cs.uni-magdeburg.de)

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU Library General Public License as
  published by  
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU Library General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/

#ifndef KIllustrator_h_
#define KIllustrator_h_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <ktopwidget.h>
#include <ktoolbar.h>
#include <kstatusbar.h>
#include <kfm.h>
#include <drag.h>

#include <qlist.h>
#include "ToolController.h"
#include "Ruler.h"
#include "GObject.h"
#include "CommandHistory.h"
#include "MainView.h"

class QwViewport;
class Canvas;
class GDocument;
class PStateManager;
class ScriptDialog;
class LayerDialog;
class EditPointTool;

#define ID_FILE_NEW              1
#define ID_FILE_OPEN             2
#define ID_FILE_OPEN_RECENT     20
#define ID_FILE_SAVE             4
#define ID_FILE_SAVE_AS          5
#define ID_FILE_CLOSE            6
#define ID_FILE_PRINT            7
#define ID_FILE_NEW_WINDOW       8 
#define ID_FILE_INFO             9
#define ID_FILE_EXIT            10 

#define ID_IMPORT               11
#define ID_EXPORT               12

#define ID_EDIT_UNDO           200
#define ID_EDIT_REDO           201
#define ID_EDIT_CUT            202
#define ID_EDIT_COPY           203
#define ID_EDIT_PASTE          204
#define ID_EDIT_DELETE         205
#define ID_EDIT_SELECT_ALL     206
#define ID_EDIT_PROPERTIES     207

#define ID_VIEW_OUTLINE        301
#define ID_VIEW_NORMAL         302
#define ID_VIEW_LAYERS         305
#define ID_VIEW_RULER          310
#define ID_VIEW_GRID           311

#define ID_LAYOUT_PAGE         400
#define ID_LAYOUT_GRID         401

#define ID_ARRANGE_ALIGN       500
#define ID_ARRANGE_FRONT       501
#define ID_ARRANGE_BACK        502
#define ID_ARRANGE_1_FORWARD   503
#define ID_ARRANGE_1_BACK      504
#define ID_ARRANGE_GROUP       505
#define ID_ARRANGE_UNGROUP     506

#define ID_TRANSFORM_POSITION  600
#define ID_TRANSFORM_DIMENSION 601
#define ID_TRANSFORM_ROTATION  602
#define ID_TRANSFORM_MIRROR    603

#define ID_EFFECTS_PATHTEXT    700

#define ID_EXTRAS_OPTIONS      800
#define ID_EXTRAS_CLIPART      801
#define ID_EXTRAS_SCRIPTS      802

#define ID_HELP_HELP           900
#define ID_HELP_ABOUT_APP      901
#define ID_HELP_ABOUT_KDE      902

#define ID_TOOL_SELECT        1001
#define ID_TOOL_EDITPOINT     1002
#define ID_TOOL_FREEHAND      1003
#define ID_TOOL_LINE          1004
#define ID_TOOL_BEZIER        1005
#define ID_TOOL_RECTANGLE     1006
#define ID_TOOL_POLYGON       1007
#define ID_TOOL_ELLIPSE       1008
#define ID_TOOL_TEXT          1009
#define ID_TOOL_ZOOM          1010
#define ID_TOOL_PATHTEXT      1011

#define ID_TOOL_EP_MOVE       1100
#define ID_TOOL_EP_ADD        1101
#define ID_TOOL_EP_DEL        1102
#define ID_TOOL_EP_JOIN       1103
#define ID_TOOL_EP_SPLIT      1104

class KIllustrator : public KTopLevelWidget, public MainView {
  Q_OBJECT

public:
  enum TransferDirection { Transfer_Get, Transfer_Put };

  KIllustrator (const char* url = NULL);
  ~KIllustrator ();
  
  GDocument* activeDocument () { return document; }
  Canvas* getCanvas () { return canvas; }

protected:
  QSize sizeHint () const { return QSize (700, 500); }
  void closeEvent (QCloseEvent*);
  
public slots:
  void menuCallback (int item);

  void setPenColor (const QBrush& b);
  void setFillColor (const QBrush& b);

  void slotKFMJobDone ();
  void dropActionSlot (KDNDDropZone* dzone);

  void showCursorPosition (int x, int y);
  void showCurrentMode (const char* msg);

  void updateZoomFactor (float zFactor);

  void updateRecentFiles ();
  void updateSettings ();

protected slots:
  void toolSelected (int id);
  void zoomFactorSlot (int);
  void selectColor (int flag, const QBrush& b);
  void setUndoStatus(bool undoPossible, bool redoPossible);

  void popupForSelection (int x, int y);
  void popupForObject (int x, int y, GObject* obj);

  void resetTools ();

private:
  static bool closeWindow (KIllustrator* win);
  static void quit ();

  void saveFile ();
  void saveAsFile ();
  bool askForSave ();

  void setupMainView ();
  void initToolBars ();
  void initMenu ();
  void initStatusBar ();

  void about (int id);

  void openFile (const char* fname);
  void openURL (const char* url);
  void exportToFile ();
  void importFromFile ();

  void setFileCaption (const char* fname);

  void showTransformationDialog (int id);

  KToolBar *toolbar, *toolPalette, *colorPalette, *editPointToolbar;
  KStatusBar* statusbar;
  KMenuBar* menubar;
  
  QGridLayout *gridLayout;

  QPopupMenu *file, *edit, *view, *layout,
    *arrangement, *effects, *extras, *help, *popupMenu;
  QPopupMenu *openRecent;
  ToolController *tcontroller;
  QwViewport *viewport;
  GDocument *document;
  Canvas *canvas;
  Ruler *hRuler, *vRuler;
  ScriptDialog *scriptDialog;
  LayerDialog *layerDialog;
  EditPointTool *editPointTool;

  // direction of file transfer
  TransferDirection ioDir;
  // the KFM connection
  KFM *kfmConn;
  // the drop zone
  KDNDDropZone *dropZone;

  CommandHistory cmdHistory;

  QString localFile;
  QArray<float> zFactors;
  PStateManager* psm;

  static QList<KIllustrator> windows;
  static bool previewHandlerRegistered;
};

#endif
