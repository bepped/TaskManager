#include "Task.h"

Task::Task():
    state{EMPTY}, type{PRINTSTRING}, frequency{SECONDS}, period{15},  what{QString()},
    days(7), start_run{QDateTime()}, next_run{QDateTime()}, always_run{false}
{

}

Task::Task(int typ, int freq, int period, const QString& wha,
           const QBitArray& days, const QDateTime& next):
    state{ACTIVE}, type{typ}, frequency{freq}, period{period},
    what{wha}, days{days}, start_run{next}, next_run{next},
    always_run{days.count(false) == days.size()}
{
}

Task::Task(const Task& task)
{
    state = task.state;
    type = task.type;
    frequency = task.frequency;
    period = task.period;
    what = task.what;
    days = task.days;
    start_run = task.start_run;
    next_run = task.next_run;
    always_run = task.always_run;
}

Task& Task::operator=(const Task& task)
{
    this->state = task.state;
    this->type = task.type;
    this->frequency = task.frequency;
    this->period = task.period;
    this->what = task.what;
    this->days = task.days;
    this->start_run = task.start_run;
    this->next_run = task.next_run;
    this->always_run = task.always_run;
    return *this;
}

enum State Task::getState() const
{
    return state;
}

int Task::getType() const
{
    return type;
}

int Task::getFrequency() const
{
    return frequency;
}

int Task::getPeriod() const
{
    return period;
}

const QString& Task::getWhat() const
{
    return what;
}

const QBitArray& Task::getDays() const
{
    return days;
}

const QDateTime& Task::getStartRun() const
{
    return start_run;
}

const QDateTime& Task::getNextRun() const
{
    return next_run;
}

void Task::run_if()
{
    if (state == EMPTY) return;

    QDateTime dateTime = QDateTime::currentDateTime();
    int day = dateTime.date().dayOfWeek() - 1;
    bool runnable = days[day] || always_run;
    if (runnable && next_run <= dateTime) {
        run();
        if (frequency == DAYS)
            setNextRunPerDays();
        else
            setNextRunPerSecs();
    }
}

void Task::run()
{
    QString now = QDateTime::currentDateTime().toString({"dd-MM-yyyy HH:mm:ss"});
    if (type == PRINTSTRING) {
        qDebug() << "Data " << now << ": " << what;
        return;
    }

    if (!QFile(what).exists()) {
        qDebug() << "Data " << now << ": file " << what << " inesistente";
    }
}

const Task &Task::save()
{
    QDateTime now = QDateTime::currentDateTime();
    QDate today = now.date();
    if (next_run.date() > today ) {
        int day = next_run.date().dayOfWeek() - 1;
        next_run = next_run.addDays( daysToNextRun(day) );
    }
    else
        initNextRun();
    return *this;
}

qint64 Task::daysToNextRun(int from_day) {
    if (from_day > WEEKLASTDAY) from_day = WEEKFIRSTDAY;
    qint64 inc = 0;
    if (!always_run) {
        while (!days[from_day++]) {
            ++inc;
            if (from_day > WEEKLASTDAY)
                from_day = WEEKFIRSTDAY;
        }
    }
    return inc;
}


void Task::setNextRunPerDays() {
    next_run = next_run.addDays(1);
    int day = next_run.date().dayOfWeek() - 1;
    next_run = next_run.addDays(daysToNextRun(day));
    next_run.setTime(start_run.time());
}

void Task::setNextRunPerSecs() {
    qint64 secs = frequency == SECONDS ? period : period * 3600;
    QDate before = next_run.date();
    next_run = next_run.addSecs(secs);
    if (always_run)
        return;

    QDate after  = next_run.date();
    if (after > before) { //passato al giorno successivo
        int day = after.dayOfWeek() - 1;
        if (!days[day]){
            next_run = next_run.addDays(daysToNextRun(day));
            next_run.setTime(start_run.time());
        }
    }
}

void Task::initNextRun()
{
    QDateTime now = QDateTime::currentDateTime();
    QDate today = now.date();
    if (next_run.date() < today ) {
        next_run.setDate(today);
    }

    if (frequency == DAYS) {
        int day = next_run.date().dayOfWeek() - 1;
        next_run = next_run.addDays(daysToNextRun(day));
        next_run.setTime(start_run.time());
        if (next_run.date() == today && next_run.time() < now.time() ) {
            //perso il primo giro
            qint64 inc = daysToNextRun(today.dayOfWeek());
            next_run = next_run.addDays(inc);
        }
        return;
    }

    if (!always_run ) {
        int day = next_run.date().dayOfWeek() - 1;
        next_run = next_run.addDays(daysToNextRun(day));
    }
    qint64 secs = frequency == SECONDS ? period : period * 3600;
    QTime time = QTime::currentTime();
    if (next_run.time() > time)
        return;

    int s = QTime(0, 0).secsTo(next_run.time());
    int t = QTime(0, 0).secsTo(time );
    int a = (t - s) % secs;
    next_run.setTime(time);
    next_run = next_run.addSecs(a);
    QDate dt  = next_run.date();
    int day = dt.dayOfWeek() - 1;
    if (dt > today) { //passato al giorno successivo
        if (!days[day] && !always_run){
            next_run = next_run.addDays(daysToNextRun(day));
            next_run.setTime(start_run.time());
        }
    }
}
