#include <memnotify/memory_notification.hpp>
#include <QApplication>
#include <QWidget>
#include <QDebug>

void test(const QString& name, const bool state)
{
    qDebug() <<"Notified" <<name <<state;
}

int main(int argc, char **argv) {
    QApplication app(argc,argv);
    MEMNOTIFY::MemoryNotification &memnotify = MEMNOTIFY::MemoryNotification::defaultObject();    
//    qDebug() <<"Setup " <<memnotify.setup();
    memnotify.addObserver(test);
    qDebug() <<"Enable " <<memnotify.enable();

    qDebug() <<"POLL" <<memnotify.poll();
    QWidget window;
    window.show();
    app.exec();
}
