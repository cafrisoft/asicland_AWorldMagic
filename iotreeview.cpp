#include "iotreeview.h"

IOTreeView::IOTreeView(QWidget *parent)
    : QTreeView{parent}
{
    // 루트 항목 옆에 데코레이션 (예: +/-) 을 표시
    setRootIsDecorated(true);

    // 트리 뷰에서 각 항목의 배경색을 번갈아 바꿔서 표시
    setAlternatingRowColors(true);

    // 아래와 같이 라인을 표시하는 스타일을 설정할 수도 있습니다.
    /*
        setStyleSheet("QTreeView::branch {"
                      "background: palette(base);"
                      "margin: 0px;"
                      "padding: 0px;"
                      "}"
                      "QTreeView::branch:has-siblings:adjoins-item {"
                      "border-image: url(vline.png) 0;"
                      "}"
                      "QTreeView::branch:has-siblings:!adjoins-item {"
                      "border-image: url(branch-more.png) 0;"
                      "}"
                      "QTreeView::branch:has-children:has-siblings:closed,"
                      "QTreeView::branch:closed:has-children:has-siblings {"
                      "border-image: none;"
                      "image: url(branch-closed.png);"
                      "}"
                      "QTreeView::branch:open:has-children:has-siblings,"
                      "QTreeView::branch:open:has-children:!has-siblings {"
                      "border-image: none;"
                      "image: url(branch-open.png);"
                      "}"); */
    /*스타일 시트에서 사용되는 branch-closed.png, branch-open.png, vline.png, branch-more.png 등의 이미지 파일들은 실제 프로젝트에서 사용하려면 해당 이미지들을 제공하셔야 합니다.*/

}
