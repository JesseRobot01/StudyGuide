//
// Created by Jesse on 30 aug. 2023.
//

#ifndef STUDYGUIDE_MAINWINDOW_H
#define STUDYGUIDE_MAINWINDOW_H

#include <QMainWindow>
#include "PreferenceWindow.h"
#include "guide/GuideData.h"
#include<QCloseEvent>


class Guide;

QT_BEGIN_NAMESPACE

namespace Ui {
    class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

    void processGuide(GuideData::Data guide);

private slots:

    void on_actionPreference_triggered();

    void on_actionOpen_File_triggered();

    void on_actionSave_Guide_As_triggered();

    void on_actionAbout_triggered();

    void on_actionSave_triggered();
    void on_guideSwitcher_tabCloseRequested(int tab);


private:
    Ui::MainWindow *ui;
    PreferenceWindow *preferenceWindow;

    QVector<Guide *> guides;

    void addGuide(Guide *guide, const QString &name);

    void closeEvent(QCloseEvent *event) override;

    void saveGuideAs(GuideData::Data guide);

    void saveGuide(GuideData::Data guide);
};


#endif //STUDYGUIDE_MAINWINDOW_H
