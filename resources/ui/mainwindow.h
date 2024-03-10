//
// Created by 赵润朔 on 2024/3/10.
//

#ifndef RSMESH_MAINWINDOW_H
#define RSMESH_MAINWINDOW_H

#include <QMainWindow>

namespace rsmesh::ui {
    QT_BEGIN_NAMESPACE
    namespace Ui { class MainWindow; }
    QT_END_NAMESPACE

    class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);

        ~MainWindow() override;

    private:
        Ui::MainWindow *ui;
    };
} // rsmesh::ui

#endif //RSMESH_MAINWINDOW_H
