#pragma once

#ifdef _WIN32

#include <stdio.h>

#define OPEN_DESIGN_TEST 0
#define AWM_PROJECT 1
#define CREATE_PROJECT_TEST 1
#define MYMAINWINDOW_TEST 0
#define CREATEDESIGNGENERAL_20231206 1
#define LIBRARYINTERFACE_TEST_20231206 0
#define LIBRARYHANDLER_WRITEMODELTOFILE_20231206 1
#define AWELEMENTWINDOW_20231213 1
#define LIBWIDGET_20231213 1
#define DOCKWIDGETHANDLER_20231228 1
#define PROTECTION_ENABLE_20231229 1			// Document modification control
#define LIBRARYDOCK_DISABLE_20231229 1			// Library dock disable control
#define COMPONENTEDITORCHOICESITEM_DISABLE_20231229 1                // Component editor Choices disable control
#define COMPONENTEDITORMEMMAPSITEM_DISABLE_20231229 1            // Component editor Memory maps disable control
#define COMPONENTEDITORADDRSPACESITEM_DISABLE_20231229 1         // Component editor Address spaces disable control
#define INSTANTIATIONSITEM_DISABLE_20231229 1                                        // Component editor Instantiations disable control
#define COMPONENTEDITORVIEWSITEM_DISABLE_20231229 0                      // Component editor View disable control
#define COMPONENTEDITORSYSTEMVIEWSITEM_DISABLE_20231229 1        // Component editor System view disable control
#define COMPONENTEDITORBUSINTERFACESITEM_DISABLE_20231229 0      // Component editor Bus interface disable control
#define COMPONENTEDITORINDIRECTINTERFACESITEM_DISABLE_20231229 1      // Component editor IndirectInterfaces disable control
#define COMPONENTEDITORCHANNELSITEM_DISABLE_20231229 1               // Component editor Channels disable control
#define REMAPSTATESITEM_DISABLE_20231229 1                                           // Component editor Remap states disable control
#define COMPONENTEDITORCPUSITEM_DISABLE_20231229 1                       // Component editor Cpus disable control
#define COMPONENTEDITOROTHERCLOCKSITEM_DISABLE_20231229 1        // Component editor Other clock drivers disable control
#define RESETTYPESITEM_DISABLE_20231229 1                                                // Component editor Reset types disable control
#define COMPONENTEDITORAPIINTERFACESITEM_DISABLE_20231229 1          // Component editor API Interfaces disable control
#define COMPONENTEDITORCOMINTERFACESITEM_DISABLE_20231229 1      // Component editor Com Interfaces disable control
#define COMPONENTEDITORSWPROPERTIESITEM_DISABLE_20231229 1       // Component editor Software Properties disable control
#define PRODUCTHIERARCHY_IP_ONLY_20231229 0                                          // Product hierarchy IP only support control
#define LIBRARYTREE_CONTEXT_SUPPORT_20231229 1                                   // LibraryTreeView contextMenu support control
#define HIERARCHYWIDGET_SUPPORT_20231229 0                                           // HierarchyWidget support control
#define DIALERWIDGET_SUPPORT_20231229 1                                           // DialerWidget hide support control
#define PREVIEW_HIDE_SUPPORT_20231229 0                                           // Preview hide support control
#define MEMORYDESIGNMENU_SUPPORT_20231229 0                                           // Open memory design menu support control
#define NEWDESIGNMENU_SUPPORT_20231229 0                                           // New design menu support control
#define SWDESIGNMENU_SUPPORT_20231229 0                                           // SW design menu support control
#define SYSTEMDESIGNMENU_SUPPORT_20231229 0                                           // System design menu support control
#define BUSMENU_SUPPORT_20231229 0                                           // Bus menu support control
#define CATALOGMENU_SUPPORT_20231229 0                                           // Catalog menu support control
#define COMDEFMENU_SUPPORT_20231229 0                                           // COM definition menu support control
#define DELETEMENU_SUPPORT_20231229 0                                           // Delete menu support control
#define EXPORTMENU_SUPPORT_20231229 1                                           // Export menu support control (Library)
#define EXPORTMENU_PRJ_SUPPORT_20240111 0                                           // Export menu support control (Project)
#define IMPORTMENU_PRJ_SUPPORT_20240111 0                                           // Import menu support control (Project)
#define SHOWERRORSMENU_SUPPORT_20231229 0                                           // Show errors menu support control
#define ADDSIGNALSMENU_SUPPORT_20231229 0                                           // Add signals menu support control
#define APIDEFMENU_SUPPORT_20231229 0                                           // API definition menu support control
#define SYSTEMMENU_SUPPORT_20231229 0                                           // System menu support control
#define XMLMENU_SUPPORT_20231229 0                                           // XML menu support control
#define CONTAININGFOLDERMENU_SUPPORT_20231229 0                                           // Containing folder menu support control
#define EXPANDMENU_SUPPORT_20231229 0                                           // Expand menu support control
#define CODEEDITORSETTINGSPAGE_SUPPORT_20240103 0                                           // Code editor settings page support control
#define LANGUAGEHIGHLIGHTER_SUPPORT_20240103 0                                           // Language highlighter support control
#define TO_DO_ERROR_20240104 1                                           // To-do error support control
#define DIAGRAM_TOOLS_SUPPORT_20240104 1                                           // Diagram tools support control
#define LOG_OUT_SUPPORT_20240104 1                                           // Log out support control
#define DESIGN_SCRIPT_ENABLE_20240125 1                                           // Design generation script support control
#define PROJECT_PARSELIBRARY_ENABLE_20240125 0                                           // Project parse library support control
#define PLUGIN_TOOLS_SUPPORT_20240905 0                                           // External plugin tools support control


typedef unsigned long key_t;
typedef unsigned long sem_t;

#define shm_open(a, b, c) 0
#define ftruncate(fd, size) 

#define PROT_READ 0
#define PROT_WRITE 0
#define MAP_SHARED 0
static inline sem_t* sem_open(const char* name, int mode=0, int flag = 0, int arg4=0) {
	return 0;
}

#define SEM_FAILED NULL


static inline void* mmap(void* a, int size, int mode, int shared, int fd, int flag) 
{
	return NULL;
}

#define munmap(data, size)

#define MAP_FAILED NULL

#define sem_close(sem)
#define sem_unlink(name)

static inline void close(int fd) {

}

#define shm_unlink(name)
#define sem_wait(name)
#define sem_post(name)

#endif
