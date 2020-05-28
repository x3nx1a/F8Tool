#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QStyleFactory>
#include <QCoreApplication>
#include <qlibraryinfo.h>
#include <QtGlobal>
#include <stdio.h>
#include <stdlib.h>
#include "formmain.h"

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	QByteArray localMsg = msg.toLocal8Bit();
	QString txt;
	switch (type) {
	case QtDebugMsg:
		txt = QString("Debug: %1\r\n").arg(msg);
		break;
	case QtWarningMsg:
		txt = QString("Warning: %1\r\n").arg(msg);
		break;
	case QtCriticalMsg:
		txt = QString("Critical: %1\r\n").arg(msg);
		break;
	case QtFatalMsg:
		txt = QString("Fatal: %1\r\n").arg(msg);
		break;
	}
	QFile outFile("DebugLog.txt");
	outFile.open(QIODevice::WriteOnly | QIODevice::Append);
	QTextStream ts(&outFile);
	ts << txt << endl;
}

int main(int argc, char *argv[])
{
	QString executable = argv[0];
	QString executablePath = executable.mid(0, executable.lastIndexOf("\\"));
	QString installPathPlugins = QLibraryInfo::location(QLibraryInfo::PluginsPath);
	QCoreApplication::removeLibraryPath(installPathPlugins);
	QCoreApplication::addLibraryPath(installPathPlugins);
	QCoreApplication::addLibraryPath(executablePath);
	QCoreApplication::addLibraryPath(executablePath + "/sqldrivers");
    QApplication a(argc, argv);
	FormMain w;
	a.setStyle(QStyleFactory::create("Fusion"));
    w.show();

    a.setQuitOnLastWindowClosed(false);
    
    return a.exec();
}
