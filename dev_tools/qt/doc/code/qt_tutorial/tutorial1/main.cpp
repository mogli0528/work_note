#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsView>

/*
Prereqs:
    -basic knowledge of c++ (pointers and memory management)
    -VERY basic knowledge of Qt (widgets)
Tutorial Topics:
    -QGraphicsScene
    -QGraphicsItem (QGraphicsRectItem)
    -QGraphicsView
*/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 创建一个场景
    QGraphicsScene * scene = new QGraphicsScene();

    // 创建要放入场景中的一个 item
    QGraphicsRectItem * rect = new QGraphicsRectItem();
    rect->setRect(0, 0, 200, 200);

    // 将刚刚创建的 item 添加到场景中
    scene->addItem(rect);

    // 创建一个 view 来显示场景
    QGraphicsView * view = new QGraphicsView(scene);

    view->show();

    return a.exec();
}
