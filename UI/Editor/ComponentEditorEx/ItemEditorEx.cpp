//-----------------------------------------------------------------------------
// File: ItemEditorEx.cpp
//-----------------------------------------------------------------------------
// Project: AWM
// Author: P.H.G
// Date: 17.07.2024
//
// Description:
// ItemEditor is a base class for editors in Component Editor module..
//-----------------------------------------------------------------------------

#include <IPXACTmodels/Component/Component.h>

#include <KactusAPI/LibraryInterface.h>

#include "ItemEditorEx.h"


//-----------------------------------------------------------------------------
// Function: ItemEditorEx::ItemEditorEx()
//-----------------------------------------------------------------------------
ItemEditorEx::ItemEditorEx( QSharedPointer<Component> component, LibraryInterface* handler, QWidget *parent):
QWidget(parent), 
component_(component),
handler_(handler)
{

}

//-----------------------------------------------------------------------------
// Function: ItemEditorEx::~ItemEditorEx()
//-----------------------------------------------------------------------------
ItemEditorEx::~ItemEditorEx()
{

}

//-----------------------------------------------------------------------------
// Function: ItemEditorEx::component()
//-----------------------------------------------------------------------------
QSharedPointer<Component> ItemEditorEx::component() const
{
    return component_;
}

//-----------------------------------------------------------------------------
// Function: confirmEditorChange()
//-----------------------------------------------------------------------------
bool ItemEditorEx::confirmEditorChange()
{
    // By default, we always allow the editor to be changed.
    return true;
}

//-----------------------------------------------------------------------------
// Function: ItemEditorEx::setProtection()
//-----------------------------------------------------------------------------
void ItemEditorEx::setProtection(bool locked)
{
    setDisabled(locked);
}

//-----------------------------------------------------------------------------
// Function: ItemEditorEx::handler()
//-----------------------------------------------------------------------------
LibraryInterface* ItemEditorEx::handler() const
{
    return handler_;
}
