#ifndef CREATEIOLEFTMENU_H
#define CREATEIOLEFTMENU_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMouseEvent>
// 전방 선언을 추가합니다.
class CreateIOMain;
class CreateIOGeneral;
class CreateIOWirePort;

class CreateIOLeftMenu : public QWidget
{
    Q_OBJECT
public:
          // 콜백 함수의 타입을 정의
    typedef std::function<void(QString)> CallbackType;

    explicit CreateIOLeftMenu(CallbackType callback, QWidget *parent = nullptr);

private:
    QLabel *createIOLabel;
    CreateIOGeneral* generalWidget;
    CreateIOWirePort* wirePortWidget;
private:
    CallbackType m_callback;

signals:
    void GeneralInfoInput();
    void WirePortInput();
    void CreateIoInfo();
    void CancelIO();

private slots:  // 슬롯 섹션을 추가합니다.
    void handleGeneralInfoInput();
    void handleWirePortInput();
    void handleCreateIoInfo();
    void handleCancelIO();


public :
    CreateIOMain *ioMain;
    void SetTargetWiget(CreateIOMain* pMain){
        ioMain = pMain;
    }
    void SetLinkWindow(CreateIOGeneral* generalWidget, CreateIOWirePort* wirePortWidget);
    bool removeDir(const QString &dirPath);
};

#endif // CREATEIOLEFTMENU_H
