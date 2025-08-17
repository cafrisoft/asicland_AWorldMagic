//-----------------------------------------------------------------------------
// File: typegroup.cpp
//-----------------------------------------------------------------------------
// Project: Kactus 2
// Author: Antti Kamppi
// Date: 11.07.2011
//
// Description:
// Contains the check boxes to set the type options for VLNVDialer.
//-----------------------------------------------------------------------------

#include "typegroup.h"

#include <UI/Widgets/tagEditor/FlowLayout.h>

#include <QIcon>

//-----------------------------------------------------------------------------
// Function: TypeGroup::TypeGroup()
//-----------------------------------------------------------------------------
TypeGroup::TypeGroup(QWidget *parent):
FilterGroup(tr("Type"), parent),
componentBox_(QIcon(":resources/Res/hw-component.png"), QString(), this),
busBox_(QIcon(":resources/Res/bus-def.png"), QString(), this),
catalogBox_(QIcon(":resources/Res/catalog.png"), QString(), this),
apiComBox_(QIcon(":resources/Res/new-com_definition.png"), QString(), this),
advancedBox_(QIcon(":resources/Res/hw-design.png"), QString(), this),
options_()
{
    setupButton(&componentBox_, tr("Component"));
    setupButton(&busBox_, tr("Bus"));
    setupButton(&catalogBox_, tr("Catalog"));
    setupButton(&apiComBox_, tr("API/COM definition"));
    setupButton(&advancedBox_, tr("Advanced"));

	componentBox_.setChecked(true);
	busBox_.setChecked(true);
    catalogBox_.setChecked(true);
    apiComBox_.setChecked(true);
	advancedBox_.setChecked(false);

	connect(&componentBox_, SIGNAL(clicked(bool)), this, SLOT(onComponentChange(bool)), Qt::UniqueConnection);
	connect(&busBox_, SIGNAL(clicked(bool)), this, SLOT(onBusChange(bool)), Qt::UniqueConnection);
    connect(&catalogBox_, SIGNAL(clicked(bool)), this, SLOT(onCatalogChange(bool)), Qt::UniqueConnection);
	connect(&apiComBox_, SIGNAL(clicked(bool)), this, SLOT(onApiComChange(bool)), Qt::UniqueConnection);
    connect(&advancedBox_, SIGNAL(clicked(bool)), this, SLOT(onAdvancedChange(bool)), Qt::UniqueConnection);    

    setupLayout();
}

//-----------------------------------------------------------------------------
// Function: TypeGroup::setType()
//-----------------------------------------------------------------------------
void TypeGroup::setTypes(Utils::TypeOptions options)
{
    componentBox_.setChecked(options.components_);
    busBox_.setChecked(options.buses_);
    catalogBox_.setChecked(options.catalogs_);
    apiComBox_.setChecked(options.apis_);
    advancedBox_.setChecked(options.advanced_);

    options_ = options;
    emit optionsChanged(options_);
}

//-----------------------------------------------------------------------------
// Function: TypeGroup::getType()
//-----------------------------------------------------------------------------
Utils::TypeOptions TypeGroup::getTypes() const
{
    Utils::TypeOptions options;
    options.components_ = componentBox_.isChecked();
    options.buses_ = busBox_.isChecked();
    options.catalogs_ = catalogBox_.isChecked();
    options.apis_ = apiComBox_.isChecked();
    options.advanced_ = advancedBox_.isChecked();

    return options;
}

//-----------------------------------------------------------------------------
// Function: TypeGroup::selectAll()
//-----------------------------------------------------------------------------
void TypeGroup::selectAll(bool select)
{
    Utils::TypeOptions options;
    options.components_ = select;
    options.buses_ = select;
    options.catalogs_ = select;
    options.apis_ = select;
    options.advanced_ = select;

    setTypes(options);
}

//-----------------------------------------------------------------------------
// Function: TypeGroup::onComponentChange()
//-----------------------------------------------------------------------------
void TypeGroup::onComponentChange(bool checked)
{
	options_.components_ = checked;
	emit optionsChanged(options_);
}

//-----------------------------------------------------------------------------
// Function: TypeGroup::onBusChange()
//-----------------------------------------------------------------------------
void TypeGroup::onBusChange(bool checked)
{
	options_.buses_ = checked;
	emit optionsChanged(options_);
}

//-----------------------------------------------------------------------------
// Function: TypeGroup::onCatalogChange()
//-----------------------------------------------------------------------------
void TypeGroup::onCatalogChange(bool checked)
{
    options_.catalogs_ = checked;
    emit optionsChanged(options_);
}

//-----------------------------------------------------------------------------
// Function: TypeGroup::onApiComChange()
//-----------------------------------------------------------------------------
void TypeGroup::onApiComChange(bool checked)
{
    options_.apis_ = checked;
    emit optionsChanged(options_);
}

//-----------------------------------------------------------------------------
// Function: TypeGroup::onAdvancedChange()
//-----------------------------------------------------------------------------
void TypeGroup::onAdvancedChange(bool checked)
{
	options_.advanced_ = checked;
	emit optionsChanged(options_);
}

//-----------------------------------------------------------------------------
// Function: TypeGroup::setupLayout()
//-----------------------------------------------------------------------------
void TypeGroup::setupLayout()
{
    auto* layout = new FlowLayout(this, 2, 1, 1);
    layout->addWidget(&busBox_);
    layout->addWidget(&catalogBox_);
    layout->addWidget(&componentBox_);
    layout->addWidget(&apiComBox_);
    layout->addWidget(&advancedBox_);
}
