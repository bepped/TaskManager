#ifndef TASKDIALOG_H
#define TASKDIALOG_H

#include <QDialog>
#include <QVector>

#include "Task.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QGroupBox;
class QComboBox;
class QSpinBox;
class QPushButton;
class QString;
class QDateTimeEdit;
class QLineEdit;
class QRadioButton;
class QCheckBox;
QT_END_NAMESPACE

class TaskEditor : public QDialog
{   
    Q_OBJECT

    const QString SAVE_LABEL;
    const QString IGNORE_LABEL;
    const QString DELETE_LABEL;
    Task& task;

    QLabel *typeTaskLabel;
    QLabel *frequencyLabel;
    QLabel *timeLabel;
    QGroupBox *taskBox;
    QComboBox *typeTaskCombo;

    QRadioButton *freqSecondsRadio;
    QRadioButton *freqHoursRadio;
    QRadioButton *freqDaysRadio;

    QSpinBox *freqSecondsSpinBox;
    QSpinBox *freqHoursSpinBox;


    QCheckBox *freqLunCheckBox;
    QCheckBox *freqMarCheckBox;
    QCheckBox *freqMerCheckBox;
    QCheckBox *freqGioCheckBox;
    QCheckBox *freqVenCheckBox;
    QCheckBox *freqSabCheckBox;
    QCheckBox *freqDomCheckBox;

    QPushButton *opSaveButton;
    QPushButton *opIgnoreButton;
    QPushButton *opDeleteButton;
    QDateTimeEdit *dateTimeEdit;
    QLineEdit *typeTaskEdit;


    void openTaskEditor();
    void fillTypeTaskArea();
    void fillFrequencyArea();
    void fillDateTimeArea();
    void refillValues();
    QBitArray setBitDays();
    void save();
    bool okToSave();
    void delet();

public:
    TaskEditor(Task &task);

private slots:

};

#endif // TASKDIALOG_H
