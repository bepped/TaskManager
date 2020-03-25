#ifndef TASK_H
#define TASK_H

#include <QTime>
#include <QDebug>
#include <QFile>
#include <QBitArray>
#include <QString>

enum State {ACTIVE, EMPTY };
enum Frequency {SECONDS, HOURS, DAYS };
enum Type {PRINTSTRING, CHECKFILE};
enum DayWeek {WEEKFIRSTDAY, WEEKLASTDAY=6};

class Task
{
    enum State state;
    int type;
    int frequency;
    int period;
    QString what;
    QBitArray days;
    QDateTime start_run;
    QDateTime next_run;
    bool always_run;

    void run();
    void initNextRun();
    qint64 daysToNextRun(int from_day);
    void setNextRunPerDays();
    void  setNextRunPerSecs();

public:
    Task();
    Task(int type, int freq, int period, const QString& what,
         const QBitArray& days, const QDateTime &next);
    Task(const Task& task);
    Task& operator=(const Task& task);

    void run_if();
    const Task& save();

    enum State getState() const;
    int getType() const ;
    int getFrequency() const;
    int getPeriod() const;
    const QString& getWhat() const;
    const QBitArray& getDays() const;
    const QDateTime& getStartRun() const;
    const QDateTime& getNextRun() const;
};

#endif // TASK_H
