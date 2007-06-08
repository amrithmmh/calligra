/*
   Copyright (c) 2004 Boudewijn Rempt <boud@valdyas.org>
   Copyright (c) 2005 Sven Langkamp <sven.langkamp@gmail.com>

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

#ifndef KARBON_TOOL_REGISTRY_H_
#define KARBON_TOOL_REGISTRY_H_

#include <q3valuevector.h>
#include <karbon_export.h>
class KActionCollection;
class KarbonView;
class KarbonAbstractToolFactory;

/**
 * A registry where new tool plugins can register themselves.
 */
class KARBONBASE_EXPORT KarbonToolRegistry : private Q3ValueVector<KarbonAbstractToolFactory*>
{
public:
	virtual ~KarbonToolRegistry();

	static KarbonToolRegistry* instance();

	void createTools( KActionCollection * ac, KarbonView* view );
	void add(KarbonAbstractToolFactory* factory);

private:
	KarbonToolRegistry();
	KarbonToolRegistry(const KarbonToolRegistry&);
	KarbonToolRegistry operator=(const KarbonToolRegistry&);

	static KarbonToolRegistry *m_singleton;
};

#endif // KARBON_TOOL_REGISTRY_H_

