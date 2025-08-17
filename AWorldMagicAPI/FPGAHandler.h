//-----------------------------------------------------------------------------
// File: FPGAHandler.h
//-----------------------------------------------------------------------------
// Project: AWM
// Author: H.G.P
// Date: 11.06.2024
//
// Description:
// FPGAHandler is the class that implements the services to manage the IP-XACT library.
//-----------------------------------------------------------------------------

#ifndef FPGAHANDLER_H
#define FPGAHANDLER_H

#include <QString>
#include <QStringList>
#include <QList>
#include <QSharedPointer>
#include <QObject>

#include <AWorldMagicAPI/ProjectLibraryLoader.h>
#include <UI/FPGATreeWidget/FPGATreeModel.h>


class FPGATreeModel;
class MessageMediator;

//-----------------------------------------------------------------------------
//! FPGAHandler is the class that implements the services to manage the IP-XACT library.
//-----------------------------------------------------------------------------
class FPGAHandler : public QObject
{
    Q_OBJECT

public:
    static FPGAHandler* getInstance();

    static void initialize(MessageMediator* messageChannel, QObject* parent = nullptr);

    //! No copying
    FPGAHandler(const FPGAHandler &other) = delete;

    //! The destructor
    virtual ~FPGAHandler() = default;

    //! Search for IP-Xact files in the file system and add them to library
    void searchForFPGAFiles();

    FPGATreeModel* getModel();

public slots:
    /*! Open the specified component design
     *
     *      @param [in] filePath     Identifies the view for the FPGA file.
     *
    */
    void onOpenFPGA(QString const& filePath);

signals:
    //! Signal that user wants to open the specified component for editing.
    void openFPGA(const QString& filePath);

    //! Inform both models that the content must be reset.
    void resetModel();

private slots:

private:

    //-----------------------------------------------------------------------------
    // The private functions used by public class methods
    //-----------------------------------------------------------------------------

    /*! The constructor
 *
 *      @param [in] dialer  The dialer that provides search options.
 *      @param [in] parent  The parent widget of this instance.
 */
    FPGAHandler(MessageMediator* messageChannel, QObject* parent = nullptr);

    //! Connect the signals and slots that keep the data models synchronized.
    void syncronizeModels();

    //! Loads all available FPGA Files.
    void loadFPGAFIles();

    //! Resets the tree and hierarchy model.
    void resetModels();

    //-----------------------------------------------------------------------------
    // Data.
    //-----------------------------------------------------------------------------
    static FPGAHandler* FPGAHandler_instance_;
    
    //! Message channel for errors/informations.
    MessageMediator* messageChannel_;

    //! Loads the library content.
    ProjectLibraryLoader loader_;

    //! The model for the tree view
    FPGATreeModel* treeModel_;
};

#endif // FPGAHANDLER_H
