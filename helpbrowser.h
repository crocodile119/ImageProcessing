#ifndef HELPBROWSER_H
#define HELPBROWSER_H

#include <QTextBrowser>
#include <QHelpEngine>
#include <QDebug>

class HelpBrowser : public QTextBrowser
{
public:
    HelpBrowser(QHelpEngine* helpEngine, QWidget* parent = 0);
    QVariant loadResource (int type, const QUrl& name);
private:
    QHelpEngine* helpEngine;
};

#endif // HELPBROWSER_H
