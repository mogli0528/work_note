#include <QApplication>
#include "MyRect.h"
#include <QGraphicsScene>
#include <QGraphicsView>

/*
Tutorial Topics:
    -events (keyPressEvent() and QKeyEvent)
    -event propogation system
    -QDebug

QGraphicsItem::ItemIsFocusable
     支持键盘输入 focus (也就是说它是个 input item). 
     使能这个 flag 将会允许这个 item(比如 rect) 接受 foucs, 这样带来的好处是它可以重复
     给 QGraphicsItem::keyPressEvent() 和 QGraphicsItem::keyReleaseEvent() 函数
     传递按键事件. 
void QGraphicsItem::setFocus(Qt::FocusReason focusReason = Qt::OtherFocusReason)
    将 keyboard input 集中到这个 item 上.  
    focusReason 参数用来解释什么引起 foucs 到 items.
    Only enabled items that set the ItemIsFocusable flag can accept keyboard focus.
    If this item is not visible, not active, or not associated with a scene, 
    it will not gain immediate input focus. However, it will be registered 
    as the preferred focus item for its subtree of items, should it later become visible.
    As a result of calling this function, this item will receive a focus 
    in event with focusReason. If another item already has focus, that item 
    will first receive a focus out event indicating that it has lost input focus.
*/

int main(int argc, char *argv[]){
    QApplication a(argc, argv);

    // create a scene
    QGraphicsScene * scene = new QGraphicsScene();

    // create an item to add to the scene
    MyRect * rect = new MyRect();
    rect->setRect(0,0,100,100); // change the rect from 0x0 (default) to 100x100 pixels

    // add the item to the scene
    scene->addItem(rect);

    // make rect focusable
    // 也就是说只有当鼠标的光标在矩形框内时才接收键盘输入.标志位设置好之后还需手动开启 focus.   
    rect->setFlag(QGraphicsItem::ItemIsFocusable);
    rect->setFocus();

    // create a view to visualize the scene
    QGraphicsView * view = new QGraphicsView(scene);

    // show the view
    view->show();

    return a.exec();
}
