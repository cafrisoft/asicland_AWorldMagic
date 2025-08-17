/*
 *      Created on: 11.6.2024
 *      Author: P.H.G
 *      Filename: FPGATreeModel.h
 *      Project: AWM
 *      Description :
 *          The model that contains the FPGA Items to display.
 */

#ifndef FPGATREEMODEL_H
#define FPGATREEMODEL_H

#include <QFileSystemModel>


//-----------------------------------------------------------------------------
//! The model that contains the FPGA Items to display.
//-----------------------------------------------------------------------------
class FPGATreeModel : public QFileSystemModel
{
    Q_OBJECT

public:

	/*! The constructor
    *
	*      @param [in] parent           The owner of this class
	*
    */
    FPGATreeModel(QObject* parent = 0);

	//! The destructor
    virtual ~FPGATreeModel();

private:

signals:
    //! Open the selected FPGA
    void openFPGA(QString const& filePath);

    //! Items have changed.
    void refresh();

public slots:
    //! Open the selected FPGA.
    void onOpenFPGA(QModelIndex const& index);

    //! Reset the model.
    void onResetModel();

protected:
};

#endif // FPGATREEMODEL_H
