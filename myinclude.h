#ifndef MYINCLUDE_H
#define MYINCLUDE_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <memory>
#include <cassert>

#include <QAbstractItemView>
#include <QApplication>

#include <QCheckBox>
#include <QComboBox>
#include <QCommonStyle>

#include <QDesktopWidget>
#include <QDir>
#include <QDockWidget>


#include <QFormLayout>
#include <QFileDialog>
#include <QFontDialog>
#include <QFont>

#include <QGridLayout>

#include <QHBoxLayout>
#include <QHeaderView>
#include <QInputDialog>
#include <QLayout>
#include <QLineEdit>
#include <QList>
#include <QListView>
#include <QListWidget>
#include <QListWidgetItem>

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>

#include <QPrintDialog>
#include <QPrinter>
#include <QPrinterInfo>

#include <QPixmap>
#include <QProcess>

#include <QSqlDatabase>
#include <QSqlError>
#include <QString>
#include <QSizePolicy>
#include <QStyle>

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTimer>
#include <QTreeView>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QTreeWidgetItemIterator>
#include <QTextEdit>
#include <QTextDocument>
#include <QTextDocumentFragment>
#include <QTextList>
#include <QTextListFormat>

#include <QSettings>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlField>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlResult>

#include <QStyleFactory>
#include <QStringList>
#include <QStatusBar>
#include <QWidget>

#include <QFrame>
#include <QLabel>
#include <QToolBar>
#include <QGroupBox>
#include <QPushButton>
#include <QMouseEvent>
#include <QScreen>
#include <QRect>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>

#include <QModelIndex>

#include <QDropEvent>
#include <QVBoxLayout>
#include <QWindow>


#include <QColor>
#include <QPalette>
#include <QColorDialog>
#include <QColormap>
#include <windows.h>

//  Okay, some macros.
#define POUT(text)(std::cout << text << std::endl)
#define POUTNLF(text)(std::cout << text << "  ")

//  Yes, I know.  This could slow things down but for now I don't care.
#endif // MYINCLUDE_H
