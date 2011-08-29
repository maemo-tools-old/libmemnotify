#include <memnotify/memory_notification.hpp>
#include <QApplication>
#include <QDebug>
class QQ : public QObject
{
    Q_OBJECT;
public slots:
    void d(const QString& s, bool b)
    {
        printf ("* received signal '%s' in state %s\n", s.toAscii().constData(), (b ? "true" : "false"));
	if (s == "EXIT")
	{
		printf ("* testing completed *\n");
		exit(0);
	}
    }
};


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    while (1) {
        MemoryNotification n;
        QQ a;
        QObject::connect(&n, SIGNAL(notified(QString,bool)), &a, SLOT(d(QString,bool)));

        if (!n.setup("./bug-277451.mn"))
            qFatal("cannot load testing memory limits file");

        while ((qrand() % 90)) {
            if (!n.enable())
                qFatal("enable failed");

            if (!n.poll())
                qFatal("poll failed");

            usleep(qrand() % 10000);
            n.disable();
        }

        if (!n.enable())
            qFatal("enable failed");
        usleep(qrand() % 10000);
    }
    return 0;

}
#include "main.moc"
