/*
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "KoReportChartPlugin.h"
#include "KoReportItemChart.h"
#include "KoReportDesignerItemChart.h"
#include "KoReportPluginInfo.h"

KoReportChartPlugin::KoReportChartPlugin()
{
    KoReportPluginInfo *info = new KoReportPluginInfo();
    info->setEntityName("report:chart");
    info->setIconName("office-chart-area");
    info->setUserName(i18n("Chart"));
    info->setPriority(10);
    setInfo(info);
}

KoReportChartPlugin::~KoReportChartPlugin()
{

}

QObject* KoReportChartPlugin::createRendererInstance(QDomNode& element)
{
    return new KoReportItemChart(element);
}

QObject* KoReportChartPlugin::createDesignerInstance(QDomNode& element, KoReportDesigner* designer, QGraphicsScene* scene)
{
    return new KoReportDesignerItemChart(element, designer, scene);
}

QObject* KoReportChartPlugin::createDesignerInstance(KoReportDesigner* designer, QGraphicsScene* scene, const QPointF& pos)
{
    return new KoReportDesignerItemChart(designer, scene, pos);
}

