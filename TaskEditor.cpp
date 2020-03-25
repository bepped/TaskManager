#include "TaskEditor.h"

#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QObject>
#include <QDateTimeEdit>
#include <QLineEdit>
#include <QPalette>
#include <QRadioButton>
#include <QCheckBox>
#include <QArrayData>

#include "Task.h"

TaskEditor::TaskEditor(Task& task)
    : SAVE_LABEL{"Salva"}, IGNORE_LABEL{"Ignora"}, DELETE_LABEL{"Elimina"},
      task{task}
{
    fillTypeTaskArea();
    fillFrequencyArea();
    fillDateTimeArea();
    if (this->task.getState() == ACTIVE)
        refillValues();
    openTaskEditor();

    connect(opSaveButton, &QAbstractButton::clicked, this, &TaskEditor::save );
    connect(opIgnoreButton, &QAbstractButton::clicked, this, &QDialog::reject);
    connect(opDeleteButton, &QAbstractButton::clicked, this, &TaskEditor::delet);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(taskBox);
    setLayout(mainLayout);

    setWindowTitle(tr("TaskEditor"));
    resize(350, 350);

}

void TaskEditor::fillTypeTaskArea()
{
    typeTaskLabel = new QLabel(tr("Tipo task: "));
    typeTaskCombo = new QComboBox;
    typeTaskCombo->addItems({"Messaggio", "Verifica file "});
    typeTaskEdit = new QLineEdit();
}

void TaskEditor::fillFrequencyArea()
{
    frequencyLabel = new QLabel("Frequenza:");
    freqSecondsRadio = new QRadioButton("Secondi");
    freqHoursRadio = new QRadioButton("Ore");
    freqDaysRadio = new QRadioButton("Giorni");

    freqSecondsSpinBox = new QSpinBox;
    freqSecondsSpinBox->setRange(2, 3599);
    freqSecondsSpinBox->setSuffix(" s");
    freqSecondsSpinBox->setValue(15);
    freqSecondsSpinBox->setToolTip("range [2 - 3599] secs");
    freqSecondsSpinBox->setToolTipDuration(5000);

    freqHoursSpinBox = new QSpinBox;
    freqHoursSpinBox->setRange(1, 23);
    freqHoursSpinBox->setSuffix(" ore");
    freqHoursSpinBox->setValue(1);
    freqHoursSpinBox->setToolTip("range [1 - 23] ore");
    freqHoursSpinBox->setToolTipDuration(5000);

    freqLunCheckBox = new QCheckBox("Lunedì");
    freqMarCheckBox = new QCheckBox("Martedì");
    freqMerCheckBox = new QCheckBox("Mercoledì");
    freqGioCheckBox = new QCheckBox("Giovedì");
    freqVenCheckBox = new QCheckBox("Venerdì");
    freqSabCheckBox = new QCheckBox("Sabato");
    freqDomCheckBox = new QCheckBox("Domenica");    
}

void TaskEditor::fillDateTimeArea()
{
    timeLabel = new QLabel("Ora di avvio: ");
    dateTimeEdit = new QDateTimeEdit(QDateTime::currentDateTime());
    dateTimeEdit->setDisplayFormat("dd-MM-yyyy HH:mm:ss");
}

void TaskEditor::refillValues()
{
    typeTaskCombo->setCurrentIndex(task.getType());
    typeTaskEdit->insert(task.getWhat());

    if (task.getFrequency() == SECONDS) {
        freqSecondsRadio->setChecked(true);
        freqSecondsSpinBox->setValue(task.getPeriod());
    }

    if (task.getFrequency() == HOURS) {
        freqHoursRadio->setChecked(true);
        freqHoursSpinBox->setValue(task.getPeriod());
    }

    if (task.getFrequency() == DAYS) {
        freqDaysRadio->setChecked(true);
    }

    QBitArray days = task.getDays();
    freqLunCheckBox->setChecked(days[0]);
    freqMarCheckBox->setChecked(days[1]);
    freqMerCheckBox->setChecked(days[2]);
    freqGioCheckBox->setChecked(days[3]);
    freqVenCheckBox->setChecked(days[4]);
    freqSabCheckBox->setChecked(days[5]);
    freqDomCheckBox->setChecked(days[6]);

    dateTimeEdit->setDate(task.getStartRun().date() );
    dateTimeEdit->setTime(task.getStartRun().time() );
}

void TaskEditor::save()
{
    if (!okToSave() ) return;

    int type = typeTaskCombo->currentIndex();
    Frequency frequency = SECONDS;
    if (freqHoursRadio->isChecked() )
        frequency = HOURS;

    if (freqDaysRadio->isChecked() )
        frequency = DAYS;

    QBitArray days(7);
    days = setBitDays();

    QDateTime next_run = dateTimeEdit->dateTime();

    int period = frequency == SECONDS ? freqSecondsSpinBox->value()
                                      : freqHoursSpinBox->value();

    QString what = typeTaskEdit->text();
    Task task = Task{type, frequency, period, what, days, next_run};

    this->task = task.save();

    accept();
}

bool TaskEditor::okToSave()
{
    QPalette palette;
    palette.setColor(QPalette::Base, Qt::red);
    QString what = typeTaskEdit->text();
    if (what.trimmed().isEmpty()) {
        typeTaskEdit->setPalette(palette);
        return false;
    }

    if (!freqSecondsRadio->isChecked() &&
        !freqHoursRadio->isChecked() &&
        !freqDaysRadio->isChecked()) {
        return false;
    }

    bool checked = true;
    if (freqDaysRadio->isChecked() ) {
        checked = freqLunCheckBox->isChecked()
                || freqMarCheckBox->isChecked()
                || freqMerCheckBox->isChecked()
                || freqGioCheckBox->isChecked()
                || freqVenCheckBox->isChecked()
                || freqSabCheckBox->isChecked()
                || freqDomCheckBox->isChecked();
    }
    return checked;
}

void TaskEditor::delet()
{
    task = Task();
    accept();
}
QBitArray TaskEditor::setBitDays()
{
    QBitArray days(7);
    days[0] = freqLunCheckBox->isChecked();
    days[1] = freqMarCheckBox->isChecked();
    days[2] = freqMerCheckBox->isChecked();
    days[3] = freqGioCheckBox->isChecked();
    days[4] = freqVenCheckBox->isChecked();
    days[5] = freqSabCheckBox->isChecked();
    days[6] = freqDomCheckBox->isChecked();
    return days;
}

void TaskEditor::openTaskEditor()
{
    taskBox = new QGroupBox(tr("Definizione task"));
    opSaveButton = new QPushButton(SAVE_LABEL);
    opIgnoreButton = new QPushButton(IGNORE_LABEL);
    opDeleteButton = new QPushButton(DELETE_LABEL);

    QGridLayout *taskBoxLayout = new QGridLayout;

    int r = 1;
    taskBoxLayout->addWidget(typeTaskLabel, r, 1);
    taskBoxLayout->addWidget(typeTaskCombo, r, 2);
    taskBoxLayout->addWidget(typeTaskEdit, r, 3);

    ++r;
    taskBoxLayout->addWidget(frequencyLabel, r, 1);
    taskBoxLayout->addWidget(freqSecondsRadio, r, 2);
    taskBoxLayout->addWidget(freqSecondsSpinBox, r, 3);
    ++r;
    taskBoxLayout->addWidget(freqHoursRadio, r, 2);
    taskBoxLayout->addWidget(freqHoursSpinBox, r, 3);
    ++r;
    taskBoxLayout->addWidget(freqDaysRadio, r, 2);
    taskBoxLayout->addWidget(freqLunCheckBox, r, 3);
    ++r;
    taskBoxLayout->addWidget(freqMarCheckBox, r, 3);
    ++r;
    taskBoxLayout->addWidget(freqMerCheckBox, r, 3);
    ++r;
    taskBoxLayout->addWidget(freqGioCheckBox, r, 3);
    ++r;
    taskBoxLayout->addWidget(freqVenCheckBox, r, 3);
    ++r;
    taskBoxLayout->addWidget(freqSabCheckBox, r, 3);
    ++r;
    taskBoxLayout->addWidget(freqDomCheckBox, r, 3);

    ++r;
    taskBoxLayout->addWidget(timeLabel, r, 1);
    taskBoxLayout->addWidget(dateTimeEdit, r, 2);

    ++r;
    taskBoxLayout->addWidget(opSaveButton, r, 3);
    taskBoxLayout->addWidget(opIgnoreButton, r, 2);
    taskBoxLayout->addWidget(opDeleteButton, r, 1);

    taskBoxLayout->setColumnStretch(4, 1);
    taskBoxLayout->setRowStretch(5, 1);

    taskBox->setLayout(taskBoxLayout);
}
