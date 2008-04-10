GC.disable

require 'Krita'
require 'KritaDockFactory'
require 'korundum4'

class FlibBookListModel < Qt::AbstractListModel
    def initialize( list )
        super(nil)
        @list = list
    end
    def rowCount( index )
        return @list.size
    end
    def data( index, role )
        if( role == Qt::DisplayRole )
            return Qt::Variant.new( File.basename( @list[index.row] ) )
        end
        return Qt::Variant.new
    end
    def removeFile( idx )
    end
    def addFile( file )
      beginInsertRows( Qt::ModelIndex.new, @list.size, @list.size )
      @list << file
      endInsertRows()
    end
    def fileAt( idx )
      return @list[idx.row]
    end
    def removeFile( idx )
      beginRemoveRows( Qt::ModelIndex.new, idx.row, idx.row )
      @list.delete_at(idx.row)
      endRemoveRows()
    end
end

class FlipBookWidget < Qt::Widget
    slots 'slotAdd()', 'slotRemove()', 'slotSelected(const QModelIndex& )'
    def initialize(parent)
        super(parent)
        
        # Setup the UI
        @gridLayout = Qt::GridLayout.new(self)
        @gridLayout.objectName = "gridLayout"
        @buttonAdd = KDE::PushButton.new(self)
        @buttonAdd.objectName = "buttonAdd"
        @buttonAdd.icon = KDE::Icon.new("list-add")
        connect(@buttonAdd, SIGNAL('released()'), self, SLOT('slotAdd()'))
        
        @gridLayout.addWidget(@buttonAdd, 0, 0, 1, 1)
        
        @buttonRemove = KDE::PushButton.new(self)
        @buttonRemove.objectName = "buttonRemove"
        @buttonRemove.icon = KDE::Icon.new("list-remove")
        connect(@buttonRemove, SIGNAL('released()'), self, SLOT('slotRemove()'))
        
        @gridLayout.addWidget(@buttonRemove, 0, 1, 1, 1)
        
        @horizontalSpacer = Qt::SpacerItem.new(172, 20, Qt::SizePolicy::Expanding, Qt::SizePolicy::Minimum)
        
        @gridLayout.addItem(@horizontalSpacer, 0, 2, 1, 1)
        
        @filesList = Qt::ListView.new(self)
        @filesList.objectName = "filesList"
        connect(@filesList, SIGNAL('clicked(const QModelIndex&)'), self, SLOT('slotSelected(const QModelIndex&)'))
        connect(@filesList, SIGNAL('activated(const QModelIndex&)'), self, SLOT('slotSelected(const QModelIndex&)'))
        list = [ ]
        
        @model = FlibBookListModel.new( list )
        @filesList.model = @model
        
        @gridLayout.addWidget(@filesList, 1, 0, 1, 3)
    end
    def slotAdd
        file = KDE::FileDialog::getOpenFileName()
        return if( file == "")
        # add the file
        @model.addFile( file )
    end
    def slotRemove
        @model.removeFile( @filesList.currentIndex)
    end
    def slotSelected(item)
        puts @model.fileAt(item)
        Krita.document().openUrl( @model.fileAt(item) )
    end
end
def createDockWidget
    wdg = Qt::DockWidget.new(Qt::Object::tr("Flipbook"))
    label = FlipBookWidget.new(wdg)
    wdg.setWidget(label)
    voidptr = Qt::Internal.smoke2kross(wdg)
    ko = Kross::Object::fromVoidPtr(voidptr)
    return ko
end
