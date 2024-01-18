#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDateEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setFixedSize(640, 480);
    setupUI();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(2);
    tableWidget->setRowCount(0);
    QStringList headers;
    headers << "Дата прорезывания" << "Позиция зуба";
    tableWidget->setHorizontalHeaderLabels(headers);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    QPushButton *addButton = new QPushButton("Добавить запись", this);
    QPushButton *removeButton = new QPushButton("Удалить запись", this);
    QPushButton *saveButton = new QPushButton("Сохранить", this);
    QPushButton *openButton = new QPushButton("Открыть", this);

    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(removeButton);
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(openButton);

    connect(addButton, &QPushButton::clicked, this, &MainWindow::addEntry);
    connect(removeButton, &QPushButton::clicked, this, &MainWindow::removeEntry);
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::saveFile);
    connect(openButton, &QPushButton::clicked, this, &MainWindow::openFile);

    mainLayout->addWidget(tableWidget);
    mainLayout->addLayout(buttonLayout);
}

void MainWindow::addEntry()
{
    int row = tableWidget->rowCount();
    tableWidget->insertRow(row);

    QDateEdit *dateEdit = new QDateEdit(this);
    QLineEdit *positionLineEdit = new QLineEdit(this);

    tableWidget->setCellWidget(row, 0, dateEdit);
    tableWidget->setCellWidget(row, 1, positionLineEdit);
}

void MainWindow::removeEntry()
{
    int currentRow = tableWidget->currentRow();
    if (currentRow != -1)
    {
        tableWidget->removeRow(currentRow);
    }
}

void MainWindow::saveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить файл", "", "Text File (*.txt)");
    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream stream(&file);
            for (int i = 0; i < tableWidget->rowCount(); ++i)
            {
                QDateEdit *dateEdit = qobject_cast<QDateEdit*>(tableWidget->cellWidget(i, 0));
                QLineEdit *positionLineEdit = qobject_cast<QLineEdit*>(tableWidget->cellWidget(i, 1));

                if (dateEdit && positionLineEdit)
                {
                    stream << dateEdit->date().toString("dd.MM.yyyy") << "\t" << positionLineEdit->text() << "\n";
                }
            }
            file.close();
        }
    }
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Открыть файл", "", "Text File (*.txt)");
    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream stream(&file);
            while (!stream.atEnd())
            {
                QStringList line = stream.readLine().split('\t');
                if (line.size() == 2)
                {
                    QDate date = QDate::fromString(line[0], "dd.MM.yyyy");
                    QString position = line[1];
                    int row = tableWidget->rowCount();
                    tableWidget->insertRow(row);
                    QDateEdit *dateEdit = new QDateEdit(this);
                    dateEdit->setDate(date);
                    QLineEdit *positionLineEdit = new QLineEdit(this);
                    positionLineEdit->setText(position);
                    tableWidget->setCellWidget(row, 0, dateEdit);
                    tableWidget->setCellWidget(row, 1, positionLineEdit);
                }
            }
            file.close();
        }
    }
}
