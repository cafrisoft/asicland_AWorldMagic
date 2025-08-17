#ifndef CREATEPROJECTLEFTMENU_H
#define CREATEPROJECTLEFTMENU_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMouseEvent>
// 전방 선언을 추가합니다.
class CreateProjectMain;
class CreateProjectGeneral;
//class CreateIOWirePort;

class CreateProjectLeftMenu : public QWidget
{
    Q_OBJECT
public:
          // 콜백 함수의 타입을 정의
    typedef std::function<void(QString)> CallbackType;

    explicit CreateProjectLeftMenu(CallbackType callback, QWidget *parent = nullptr);

private:
    QLabel *createIOLabel;
    CreateProjectGeneral* generalWidget;
    //CreateIOWirePort* wirePortWidget;
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
    CreateProjectMain *_Main;
    void SetTargetWiget(CreateProjectMain* pMain){
        _Main = pMain;
    }


    void SetLinkWindow(CreateProjectGeneral* generalWidget/*, CreateIOWirePort* wirePortWidget*/);
    bool removeDir(const QString &dirPath);
};

#endif // CREATEPROJECTLEFTMENU_H
