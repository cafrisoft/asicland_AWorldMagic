#ifndef CREATEIPLEFTMENU_H
#define CREATEIPLEFTMENU_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMouseEvent>

// 전방 선언을 추가합니다.
class CreateIPMain;
class CreateIpPortMap;

class CreateIpLeftMenu : public QWidget
{
    Q_OBJECT
public:
    explicit CreateIpLeftMenu(QWidget *parent , CreateIpPortMap* ipPortMap);

private:
    QLabel *createIPLabel;
    CreateIpPortMap* _ipPortMap;

signals:
    void GeneralInfoInput();
    void PortMapInput();
    void FileSetInput();
    void CreateIPInfo();
    void CancelIP();

private slots:  // 슬롯 섹션을 추가합니다.
    void handleGeneralInfoInput();
    void handleFileSetInput();
    void handlePortMapInput();
    void handleCreateIPInfo();
    void handleCancelIP();

public :
    CreateIPMain *ipMain;
    void SetTargetWiget(CreateIPMain* pMain){
        ipMain = pMain;
    }
};

#endif // CREATEIPLEFTMENU_H
