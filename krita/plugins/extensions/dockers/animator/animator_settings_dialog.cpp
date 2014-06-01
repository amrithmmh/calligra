/*
 *  Copyright (c) 2014 Somsubhra Bairi <somsubhra.bairi@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation; version 2 of the License, or(at you option)
 *  any later version.
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

#include "animator_settings_dialog.h"
#include <QCheckBox>
#include <QGridLayout>
#include <kis_config.h>

AnimatorSettingsDialog::AnimatorSettingsDialog(QWidget *parent) :
    QDialog(parent)
{
    this->setWindowTitle("Animator Settings");
    this->setMaximumSize(300, 200);

    KisConfig cfg;
    QCheckBox* autoFrameBreak = new QCheckBox(this);
    autoFrameBreak->setText("Enable auto frame break");
    autoFrameBreak->setChecked(cfg.defAutoFrameBreakEnabled());

    connect(autoFrameBreak, SIGNAL(clicked(bool)), this, SLOT(enableAutoFrameBreak(bool)));
    QGridLayout* mainLayout = new QGridLayout(this);
    mainLayout->addWidget(autoFrameBreak);

    this->setLayout(mainLayout);
}

void AnimatorSettingsDialog::enableAutoFrameBreak(bool enable)
{
    m_model->enableFrameBreaking(enable);
}

void AnimatorSettingsDialog::setModel(KisAnimation *model)
{
    this->m_model = model;
}
