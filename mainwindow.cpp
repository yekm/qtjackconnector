#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "jackmodel.h"
#include "booleaneditor.h"
#include "verticaltextheader.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tableView->setShowGrid(false);
    //ui->tableView->horizontalHeader()->setMinimumSectionSize(10);
    //ui->tableView->verticalHeader()->setMinimumSectionSize(10);

    m_jack_model = new JackModel(this);
    ui->tableView->setModel(m_jack_model);

    BooleanEditor *be = new BooleanEditor(this);
    ui->tableView->setItemDelegate(be);

    VerticalTextHeader *vth = new VerticalTextHeader(ui->tableView);
    ui->tableView->setHorizontalHeader(vth);

    updateView();
}

void MainWindow::updateView()
{
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();
    adjustSize();
}

MainWindow::~MainWindow()
{
    delete ui;
}
