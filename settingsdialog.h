#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include "myinclude.h"
#include "mlocal.h"

class SettingsDialog : public QDialog
{
public:                                     //  Public Data.
    QStringList     *   pKeyList;           //  List of keys.

private:                                    //  Private Data
    MainWindow      *   pMain;              //  To avoid recasting everytime.
    QSettings       *   pSettings;          //  Pointer to the QSettings Object
                                            //  from MainWindow.
    QVBoxLayout     *   pWindowLayout;      //  Layouts.
    QFormLayout     *   pFormLayout;
    QHBoxLayout     *   pButtonLayout;

    QPushButton     *   pCancelButton;      //  Buttons.
    QPushButton     *   pCloseButton;
    QPushButton     *   pDefaultsButton;
    QPushButton     *   pEditorFontButton;
    QPushButton     *   pSaveButton;

    QCheckBox       *   pFullScreen;
    QLineEdit       *   pDBName;
    QLineEdit       *   pHostName;
    QLineEdit       *   pHostAddress;
    QLineEdit       *   pPortNumber;
    QLineEdit       *   pUserName;
    QLineEdit       *   pUserPassword;
    QLineEdit       *   pEditorFont;        //  Used as an example.

    QFont               oEditorFont;
    QFont               oDefaultFont;

    QPalette            oDefaultPalette;

public:                                     //  Public Members.
//  Member Functions.
    SettingsDialog(QWidget *);
    ~SettingsDialog();
	virtual bool	LoadKeys();

    virtual bool    SetKeyPair(std::string , std::string);      //  Add or remove map elements.
    virtual bool    SetKeyPair(std::string , bool);
    virtual bool    SetKeyPair(std::string , QFont);

    virtual bool    RemoveKeyPair(std::string);
    virtual std::string ReturnValue(std::string);
    virtual QFont       ReturnFontValue(std::string);
    virtual bool        ReturnBoolValue(std::string);
    virtual void    OnCreate();
    virtual bool    UpdateSettings();

private:                                    //  Private Members.
    virtual void    InitWidgets();
    virtual void    SetLayout();
    virtual void    SetSlots();
	virtual void 	DumpSettings();
    virtual bool    WriteDefaults();        //  Write out defaults first time if nothing there.
    virtual void    FillEditFields();

public slots:
    virtual void    CloseAction();
    virtual void    CancelAction();
    virtual void    DefaultAction();
    virtual void    SaveAction();
    virtual void    EditorFontDialog();
};
#endif // SETTINGSDIALOG_H
