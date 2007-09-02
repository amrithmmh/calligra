/*
 *  Copyright (c) 2007 Cyrille Berger <cberger@cberger.net>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "kis_filters_list_dynamic_program.h"

#include <QWidget>

#include <klocale.h>

// Dynamic Brush lib includes
#include "kis_dynamic_coloring.h"
#include "kis_dynamic_program_factory_registry.h"
#include "kis_dynamic_shape.h"
#include "kis_dynamic_transformation.h"

// Filterslist program includes
#include "kis_filters_list_dynamic_program_editor.h"

class Factory {
    public:
        Factory()
        {
            KisDynamicProgramFactoryRegistry::instance()->add( new KisFiltersListDynamicProgramFactory );
        }
};

static Factory factory;

KisFiltersListDynamicProgram::~KisFiltersListDynamicProgram()
{
    for(QList<KisDynamicTransformation*>::iterator transfo = beginTransformation();
        transfo != endTransformation(); ++transfo)
    {
        delete * transfo;
    }
}

void KisFiltersListDynamicProgram::apply(KisDynamicShape* shape, KisDynamicColoring* coloringsrc, const KisPaintInformation& adjustedInfo)
{
     // First apply the transfo to the dab source
    for(QList<KisDynamicTransformation*>::iterator transfo = beginTransformation();
        transfo != endTransformation(); ++transfo)
    {
        (*transfo)->transformBrush(shape, adjustedInfo);
    }

    // Then to the coloring source
    for(QList<KisDynamicTransformation*>::iterator transfo = beginTransformation();
        transfo != endTransformation(); ++transfo)
    {
        (*transfo)->transformColoring(coloringsrc, adjustedInfo);
    }

}

QWidget* KisFiltersListDynamicProgram::createEditor(QWidget* /*parent*/)
{
    return new KisFiltersListDynamicProgramEditor(this);
}

void KisFiltersListDynamicProgram::fromXML(const QDomElement& elt)
{
    KisFiltersListDynamicProgram::fromXML(elt);
    
}

void KisFiltersListDynamicProgram::toXML(QDomDocument& doc, QDomElement& rootElt) const
{
    KisFiltersListDynamicProgram::toXML(doc, rootElt);
    
}

//--- KisFiltersListDynamicProgramFactory ---//

KisFiltersListDynamicProgramFactory::KisFiltersListDynamicProgramFactory() :
    KisDynamicProgramFactory("filterslist", i18n("Filters list"))
{
}

KisDynamicProgram* KisFiltersListDynamicProgramFactory::program(QString name)
{
    return new KisFiltersListDynamicProgram(name);
}

