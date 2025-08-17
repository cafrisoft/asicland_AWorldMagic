#ifndef PROJECTTREEWIDGET_H
#define PROJECTTREEWIDGET_H

#include <QWidget>
#include <QFileSystemModel>
#include <QTreeView>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QItemSelectionModel>
#include <QModelIndexList>

class ProjectTreeWidgetEx : public QWidget
{
    Q_OBJECT
public:
    explicit ProjectTreeWidgetEx(const QString& path, QWidget *parent = nullptr);

private slots:
    void displayFilePath(const QItemSelection &selected);


private:
    QFileSystemModel* m_model;
    QTreeView* m_tree;
    QWidget *m_emptyWidget;
    QPushButton *m_changeFolderButton;
    QPushButton *m_CaptionBtn;

public:
    void changeFolder();
    void showContextMenu(const QPoint &pos);
    void menuItem1Clicked();
    void menuItem2Clicked();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event);

};

#endif // PROJECTTREEWIDGET_H
