//-----------------------------------------------------------------------------
// File: firmnessgroup.cpp
//-----------------------------------------------------------------------------
// Project: Kactus 2
// Author: Antti Kamppi
// Date: 11.07.2011
//
// Description:
// Contains the check boxes to set the re-usability options for VLNVDialer.
//-----------------------------------------------------------------------------

#include "firmnessgroup.h"

#include <KactusAPI/utils.h>

#include <UI/Widgets/tagEditor/FlowLayout.h>

//-----------------------------------------------------------------------------
// Function: FirmnessGroup::FirmnessGroup()
//-----------------------------------------------------------------------------
FirmnessGroup::FirmnessGroup(QWidget *parent):
FilterGroup(tr("Firmness"), parent),
templateBox_(QIcon(":/resources/Res/template.png"), QString(), this),
mutableBox_(QIcon(":/resources/Res/mutable.png"), QString(), this),
fixedBox_(QIcon(":/resources/Res/fixed.png"), QString(), this),
options_()
{
    setupButton(&templateBox_, tr("Template"));
    setupButton(&mutableBox_, tr("Mutable"));
    setupButton(&fixedBox_, tr("Fixed"));

	templateBox_.setChecked(true);
	mutableBox_.setChecked(true);
	fixedBox_.setChecked(true);

    connect(&templateBox_, SIGNAL(clicked(bool)), this, SLOT(onTemplateChanged(bool)), Qt::UniqueConnection);
    connect(&mutableBox_, SIGNAL(clicked(bool)), this, SLOT(onMutableChanged(bool)), Qt::UniqueConnection);
    connect(&fixedBox_, SIGNAL(clicked(bool)), this, SLOT(onConfigurationChanged(bool)), Qt::UniqueConnection);

    auto layout = new FlowLayout(this, 2, 1, 1);
    layout->addWidget(&templateBox_);
    layout->addWidget(&mutableBox_);
    layout->addWidget(&fixedBox_);
}

//-----------------------------------------------------------------------------
// Function: FirmnessGroup::setFirmness()
//-----------------------------------------------------------------------------
void FirmnessGroup::setFirmness(Utils::FirmnessOptions options)
{    
    templateBox_.setChecked(options.templates_);
    mutableBox_.setChecked(options.mutable_);
    fixedBox_.setChecked(options.fixed_);

    options_ = options;
    emit optionsChanged(options_);
}

//-----------------------------------------------------------------------------
// Function: FirmnessGroup::getFirmness()
//-----------------------------------------------------------------------------
Utils::FirmnessOptions FirmnessGroup::getFirmness() const
{
    Utils::FirmnessOptions options;
    options.templates_ = templateBox_.isChecked();
    options.mutable_ = mutableBox_.isChecked();
    options.fixed_ = fixedBox_.isChecked();
    return options;
}

//-----------------------------------------------------------------------------
// Function: FirmnessGroup::selectAll()
//-----------------------------------------------------------------------------
void FirmnessGroup::selectAll(bool select)
{
    Utils::FirmnessOptions options;
    options.templates_ = select;
    options.mutable_ = select;
    options.fixed_ = select;

    setFirmness(options);
}

//-----------------------------------------------------------------------------
// Function: FirmnessGroup::onTemplateChanged()
//-----------------------------------------------------------------------------
void FirmnessGroup::onTemplateChanged(bool checked)
{
	options_.templates_ = checked;
	emit optionsChanged(options_);
}

//-----------------------------------------------------------------------------
// Function: FirmnessGroup::onMutableChanged()
//-----------------------------------------------------------------------------
void FirmnessGroup::onMutableChanged(bool checked)
{
	options_.mutable_ = checked;
	emit optionsChanged(options_);
}

//-----------------------------------------------------------------------------
// Function: FirmnessGroup::onConfigurationChanged()
//-----------------------------------------------------------------------------
void FirmnessGroup::onConfigurationChanged(bool checked)
{
	options_.fixed_ = checked;
	emit optionsChanged(options_);
}
