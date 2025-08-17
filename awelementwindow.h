#ifndef AWELEMENTWINDOW_H
#define AWELEMENTWINDOW_H

#include <QWidget>
#include <QSplitter>
#include <QVBoxLayout>

class LibraryHandler;
//class LibraryTreeWidget;
//class HierarchyWidget;
class AWElementWindow : public QWidget
{
    Q_OBJECT
public:
    explicit AWElementWindow(LibraryHandler* libraryHandler, QWidget *parent = nullptr);


private:
    //! The instance to manage library access.
    LibraryHandler* libraryHandler_;

    //! The widget containing the library items in a tree-like view.
//    LibraryTreeWidget* treeWidget_;

    //! The widget that contains the GUI items for the hierarchy tab.
//    HierarchyWidget* hierarchyWidget_;

signals:
protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // AWELEMENTWINDOW_H
