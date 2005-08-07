/***************************************************************************
 * scriptcontainer.cpp
 * This file is part of the KDE project
 * copyright (C)2004-2005 by Sebastian Sauer (mail@dipe.org)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 * You should have received a copy of the GNU Library General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 ***************************************************************************/

#include "scriptcontainer.h"
#include "../api/object.h"
#include "../api/list.h"
#include "../api/interpreter.h"
#include "../api/script.h"
#include "../main/manager.h"
#include "mainmodule.h"

using namespace Kross::Api;

namespace Kross { namespace Api {

    /// @internal
    class ScriptContainerPrivate
    {
        public:

            /**
            * The \a Script instance the \a ScriptContainer uses
            * if initialized. It will be NULL as long as we
            * didn't initialized it what will be done on
            * demand.
            */
            Script* m_script;

            /**
            * The unique name the \a ScriptContainer is
            * reachable as.
            */
            QString m_name;

            /**
            * The scripting code.
            */
            QString m_code;

            /**
            * The name of the interpreter. This could be
            * something like "python" for the python
            * binding.
            */
            QString m_interpretername;
    };

}}

ScriptContainer::ScriptContainer(const QString& name)
    : MainModule(name)
    , d( new ScriptContainerPrivate() ) // initialize d-pointer class
{
    kdDebug() << QString("ScriptContainer::ScriptContainer() name='%1'").arg(name) << endl;

    d->m_script = 0;
    d->m_name = name;
}

ScriptContainer::~ScriptContainer()
{
    finalize();
    delete d;
}

const QString& ScriptContainer::getName()
{
    return d->m_name;
}

const QString& ScriptContainer::getCode()
{
    return d->m_code;
}

void ScriptContainer::setCode(const QString& code)
{
    finalize();
    d->m_code = code;
}

const QString& ScriptContainer::getInterpreterName()
{
    return d->m_interpretername;
}

void ScriptContainer::setInterpreterName(const QString& name)
{
    finalize();
    d->m_interpretername = name;
}

Object::Ptr ScriptContainer::execute()
{
    if(! d->m_script)
        initialize();
    return d->m_script->execute();
}

const QStringList& ScriptContainer::getFunctionNames()
{
    return d->m_script ? d->m_script->getFunctionNames() : QStringList();
}

Object::Ptr ScriptContainer::callFunction(const QString& functionname, List::Ptr arguments)
{
    if(functionname.isEmpty())
        throw RuntimeException(i18n("No functionname defined for ScriptContainer::callFunction()."));

    if(! d->m_script)
        initialize();
    return d->m_script->callFunction(functionname, arguments);
}

const QStringList& ScriptContainer::getClassNames()
{
    return d->m_script ? d->m_script->getClassNames() : QStringList();
}

Object::Ptr ScriptContainer::classInstance(const QString& name)
{
    if(! d->m_script)
        initialize();
    return d->m_script->classInstance(name);
}

void ScriptContainer::initialize()
{
    finalize();
    Interpreter* interpreter = Manager::scriptManager()->getInterpreter(d->m_interpretername);
    if(! interpreter)
        throw TypeException(i18n("Unknown interpreter '%1' on ScriptContainer::execute()").arg(d->m_interpretername));
    d->m_script = interpreter->createScript(this);
}

void ScriptContainer::finalize()
{
    delete d->m_script;
    d->m_script = 0;
}


