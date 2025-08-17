//-----------------------------------------------------------------------------
// File: SettingsDialog.cpp
//-----------------------------------------------------------------------------
// Project: Kactus 2
// Author: Joni-Matti Maatta
// Date: 10.6.2011
//
// Description:
// Settings dialog.
//-----------------------------------------------------------------------------

#include "SettingsDialog.h"

#include "GeneralSettingsPage.h"
#if CODEEDITORSETTINGSPAGE_SUPPORT_20240103 == 1
#include "CodeEditorSettingsPage.h"
#endif
#include "NamingPolicySettingsPage.h"
#include "PluginSettingsPage.h"
#include "FileTypeSettingsPage.h"
#include "k2envvariablesettingspage.h"
#include "ComponentEditorSettingsPage.h"

//-----------------------------------------------------------------------------
// Function: SettingsDialog()
//-----------------------------------------------------------------------------
SettingsDialog::SettingsDialog(QWidget* parent): 
PropertyPageDialog(QSize(24, 24), 1, VIEW_LIST, APPLY_ALL, parent)
{
    setWindowTitle(tr("Settings"));

    addPage(QIcon(":resources/Res/configuration.png"), tr("General"),
        new GeneralSettingsPage(settings_));

    addPage(QIcon(":resources/Res/naming-policy.png"), tr("Naming Policies"),
        new NamingPolicySettingsPage(settings_));

#if CODEEDITORSETTINGSPAGE_SUPPORT_20240103 == 1
    addPage(QIcon(":resources/Res/settings-code_editor.png"), tr("Code Editor"),
            new CodeEditorSettingsPage(settings_));
#endif

    addPage(QIcon(":resources/Res/settings-visibilities.png"), tr("Component Editor Visibilities"),
        new ComponentEditorSettingsPage(settings_));

    addPage(QIcon(":resources/Res/settings-file_types.png"), tr("File Types"),
            new FileTypeSettingsPage(settings_));

    addPage(QIcon(":resources/Res/settings-plugins.png"), tr("Plugins"),
            new PluginSettingsPage(settings_));

     addPage(QIcon(":resources/Res/settings-environment_vars.png"), tr("Environment variables"),
		 new K2EnvVariableSettingsPage(settings_));

    finalizePages();
    resize(sizeHint().width(), minimumHeight());
}
