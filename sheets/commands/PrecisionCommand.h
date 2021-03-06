/* This file is part of the KDE project
   Copyright 2005-2006 Stefan Nikolaus <stefan.nikolaus@kdemail.net>

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
   Boston, MA 02110-1301, USA.
*/

#ifndef CALLIGRA_SHEETS_PRECISION_COMMAND
#define CALLIGRA_SHEETS_PRECISION_COMMAND

#include "AbstractRegionCommand.h"

namespace Calligra
{
namespace Sheets
{

/**
 * \ingroup Commands
 * \brief Sets the decimal precision level.
 */
class PrecisionCommand : public AbstractRegionCommand
{
public:
    PrecisionCommand();

    void setReverse(bool reverse) override;

protected:
    bool mainProcessing() override;
    bool postProcessing() override;
};

} // namespace Sheets
} // namespace Calligra

#endif // CALLIGRA_SHEETS_PRECISION_COMMAND
