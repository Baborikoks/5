#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "shapes.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , animationTimer(new QTimer(this))
{
    ui->setupUi(this);
    CreateMenu();
    CreateActions();
    Connect();
    statusBar()->showMessage(tr("Готово"));

}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::CreateMenu()
{
    openAct = new QAction(QIcon(":/res/img/otc.png"),"Открыть");
    saveAct = new QAction(QIcon(":/res/img/sav.png"),"Сохранить");
    exitAct = new QAction(QIcon(":/res/img/vxd.png"),"Выход");

    penWidthAct = new QAction(QIcon(":/res/img/lw.png"),"&Ширина пера");
    colorAct = new QAction(QIcon(":/res/img/col.png"),"&Цвет");

    lineAct = new QAction(QIcon(":/res/img/line.png"),"&Линия");
    polylineAct = new QAction(tr("&Polyline"));
    rectAct = new QAction(QIcon(":/res/img/rect.png"),"&Прямоугольник");
    ellipseAct = new QAction(QIcon(":/res/img/ell.png"),"&Эллипс");

    deleteAct = new QAction(QIcon(":/res/img/del.png"),"&Удалить");
    moveAct = new QAction(QIcon(":/res/img/dvig.png"),"&Двигать");
    rotateAct = new QAction(QIcon(":/res/img/rotat.png"),"&Повернуть");
    resizeAct = new QAction(QIcon(":/res/img/izm.png"),"&Изменить размер");

}
void MainWindow::CreateActions()
{
    fileMenu = menuBar()->addMenu(tr("&Файл"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);
    editMenu = menuBar()->addMenu(tr("&Настройки"));
    editMenu->addAction(penWidthAct);
    editMenu->addAction(colorAct);
    editMenu->addAction(deleteAct);
    editMenu->addAction(moveAct);
    editMenu->addAction(rotateAct);
    editMenu->addAction(resizeAct);
    shapeMenu = menuBar()->addMenu(tr("&Фигуры"));
    shapeMenu->addAction(lineAct);
    shapeMenu->addAction(polylineAct);
    shapeMenu->addAction(rectAct);
    shapeMenu->addAction(ellipseAct);



}

void MainWindow::Connect()
{
    connect(openAct,&QAction::triggered,this,&MainWindow::openFile);
    connect(saveAct,&QAction::triggered,this,&MainWindow::saveFile);
    connect(colorAct, &QAction::triggered, this, &MainWindow::setColor);
    connect(exitAct,&QAction::triggered, this,&MainWindow::exitFromFile);
    connect(penWidthAct, &QAction::triggered, this, &MainWindow::setPenWidth);
    connect(animationTimer, &QTimer::timeout, this, &MainWindow::animateShape);
    connect(lineAct, &QAction::triggered, this, &MainWindow::selectShape);
    connect(polylineAct, &QAction::triggered, this, &MainWindow::selectShape);
    connect(rectAct, &QAction::triggered, this, &MainWindow::selectShape);
    connect(ellipseAct, &QAction::triggered, this, &MainWindow::selectShape);
    connect(deleteAct, &QAction::triggered, this, &MainWindow::deleteShape);
    connect(moveAct, &QAction::triggered, this, &MainWindow::moveShape);
    connect(rotateAct, &QAction::triggered, this, &MainWindow::rotateShape);
    connect(resizeAct, &QAction::triggered, this, &MainWindow::resizeShape);

}
void MainWindow::selectShape()
{
    QAction *action = qobject_cast<QAction*>(sender());
    if (action == lineAct)
        currentShape = LineType;
    else if (action == polylineAct)
        currentShape = PolylineType;
    else if (action == rectAct)
        currentShape = RectangleType;
    else if (action == ellipseAct)
        currentShape = EllipseType;

    updateStatusBar();
}
void MainWindow::updateStatusBar() // строка снизу
{
    QString shapeName;
    switch (currentShape) {
    case LineType: shapeName = tr("Линия"); break;
    case PolylineType: shapeName = tr("Polyline"); break;
    case RectangleType: shapeName = tr("Прямоугольник"); break;
    case EllipseType: shapeName = tr("Эллипс"); break;
    }
    statusBar()->showMessage(shapeName);
}

void MainWindow::paintEvent(QPaintEvent *event) // метод для рисования
{
    QMainWindow::paintEvent(event);
    QPainter painter(this);
    for (Shapes *shape : shapes) {
        shape->draw(painter);
    }
    if (tempShape) {
        tempShape->draw(painter);
    }
}

void MainWindow::resizeEvent(QResizeEvent *event) // изменение размера фигур любых
{
    QMainWindow::resizeEvent(event);
    double xScale = double(event->size().width()) / event->oldSize().width();
    double yScale = double(event->size().height()) / event->oldSize().height();
    for (Shapes *shape : shapes) {
        shape->scale(xScale, yScale);
    }
    update();
}


void MainWindow::saveFile() // сохранение файла
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Сохранить файл"), "", tr("Images (*.png *.xpm *.jpg)"));
    if (fileName.isEmpty()) {
        return;
    } else {
        QPixmap pixmap(size());
        render(&pixmap);
        pixmap.save(fileName);
    }
}

void MainWindow::openFile() // открытия файла
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Oткрыть файл"), "", tr("Images (*.png *.xpm *.jpg)"));
    if (fileName.isEmpty()) {
        return;
    } else {
        QPixmap pixmap;
        pixmap.load(fileName);
        QPainter painter(this);
        painter.drawPixmap(0, 0, pixmap);
    }
}

void MainWindow::exitFromFile() //выход
{
    close();
}

void MainWindow::setColor() // цвет
{
    QColor color = QColorDialog::getColor(currentColor,this,tr("Выберите цвет:"));
    if(color.isValid())
    {
        currentColor = color;
    }
}

void MainWindow::setPenWidth() // размер пера
{
    bool ok;
    int width = QInputDialog::getInt(this, tr("Введите ширину пера"), tr("Ширина пера:"), currentPenWidth, 1, 10, 1, &ok);
    if (ok) {
        currentPenWidth = width;
    }
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event) // кнопки для анимации (ниже методы для остановки и запуска)
{
    QMenu contextMenu(this);
    QAction *stopAnimationAct = contextMenu.addAction(tr("Остановить анимацию"));
    connect(stopAnimationAct, &QAction::triggered, this, &MainWindow::stopAnimation);
    contextMenu.exec(event->globalPos());
}

// Реализация слота для запуска анимации
void MainWindow::startAnimation()
{
    if (animatedShape == nullptr) {
        animatedShape = selectedShape;
        if (animatedShape != nullptr) {
            animationStep = 0;
            animationTimer->start(100);
        }
    }
}

// Реализация слота для остановки анимации
void MainWindow::stopAnimation()
{
    if (animatedShape != nullptr) {
        animationTimer->stop();
        animatedShape = nullptr;
    }
}

void MainWindow::animateShape() //анимация для различных фигур
{
    if (animatedShape == nullptr) return;

    QPoint startPoint = animatedShape->getStartPoint();
    QPoint endPoint = animatedShape->getEndPoint();

    switch (animatedShape->getType()) {
    case Shapes::LineType:
        // Линия уходит вверх и возвращается
        if (animationStep < 10) {
            startPoint.setY(startPoint.y() - 5);
            endPoint.setY(endPoint.y() - 5);
        } else if (animationStep < 40) {
            startPoint.setY(startPoint.y() + 5);
            endPoint.setY(endPoint.y() + 5);
        }
        break;
    case Shapes::RectangleType:
        // Прямоугольник уходит вправо и возвращается
        if (animationStep < 20) {
            startPoint.setX(startPoint.x() + 5);
            endPoint.setX(endPoint.x() + 5);
        } else if (animationStep < 40) {
            startPoint.setX(startPoint.x() - 5);
            endPoint.setX(endPoint.x() - 5);
        }
        break;
    case Shapes::EllipseType:

        if (animationStep < 20) {
            startPoint.setX(startPoint.x() - 5);
            endPoint.setX(endPoint.x() - 5);
        } else if (animationStep < 40) {
            startPoint.setX(startPoint.x() + 5);
            endPoint.setX(endPoint.x() + 5);
        }
        break;
    default:
        break;
    }

    animatedShape->setStartPoint(startPoint);
    animatedShape->setEndPoint(endPoint);

    // Принудительно перерисовываем окно
    update();

    // Останавливаем анимацию после завершения цикла
    if (++animationStep >= 40) {
        animationStep = 0;
        animationTimer->stop();
        animatedShape = nullptr;
    }
}

// Метод для нахождения фигуры по точке
Shapes* MainWindow::findShapeAt(const QPoint &point)
{
    for (Shapes* shape : shapes) {
        if (shape->contains(point)) {
            return shape;
        }
    }
    return nullptr;
}



// Обработка событий мыши
void MainWindow::mousePressEvent(QMouseEvent *event) //создание новой фигуры, выбор фигуры и запуск анимации
{
    if (event->button() == Qt::LeftButton) {
        if (currentShape == PolylineType) {
            if (!drawing) {
                tempShape = new Polyline(currentColor, currentPenWidth);
                shapes.push_back(tempShape);
                drawing = true;
            }
            static_cast<Polyline*>(tempShape)->addPoint(event->pos());
        } else {
            switch (currentShape) {
            case LineType:
                tempShape = new Line(currentColor, currentPenWidth);
                break;
            case RectangleType:
                tempShape = new Rect(currentColor, currentPenWidth);
                break;
            case EllipseType:
                tempShape = new Ellipse(currentColor, currentPenWidth);
                break;
            default:
                break;
            }
            if (tempShape) {
                tempShape->setStartPoint(event->pos());
                tempShape->setColor(currentColor);
                tempShape->setPenWidth(currentPenWidth);
                shapes.push_back(tempShape);
                drawing = true;
            }
        }
    } else if (event->button() == Qt::RightButton) {
        selectShapeAt(event->pos());
        if (selectedShape) {
            startAnimation();
        }
    }
    update();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) // визуализация рисования
{
    if (drawing && tempShape) {
        tempShape->setEndPoint(event->pos());
        update();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) // отпускается правая кнопка миши и завершается рисование
{
    if (event->button() == Qt::LeftButton && drawing) {
        if (tempShape && currentShape != PolylineType) {
            tempShape->setEndPoint(event->pos());
        }
        tempShape = nullptr;
        drawing = false;
        update();
    }
}
void MainWindow::deleteShape() //удаление фигуры
{
    if (selectedShape != nullptr) {
        shapes.removeOne(selectedShape);
        delete selectedShape;
        selectedShape = nullptr;
        update();
    }
}

void MainWindow::moveShape() // перемещение фигуры
{
    if (!selectedShape) return;

    bool ok;
    int dx = QInputDialog::getInt(this, tr("Переместить форму"), tr("Введите смещение по  оси X:"), 0, -1000, 1000, 1, &ok);
    if (!ok) return;
    int dy = QInputDialog::getInt(this, tr("Переместить форму"), tr("Введите смещение по оси Y:"), 0, -1000, 1000, 1, &ok);
    if (!ok) return;

    selectedShape->moveBy(dx, dy);
    update();
}

void MainWindow::rotateShape() // угол поворота (поворачивает только на 90 градусов)
{
    if (!selectedShape) return;

    bool ok;
    int angle = QInputDialog::getInt(this, tr("Изменить размер формы"), tr("Введите угол поворота:"), 0, -360, 360, 1, &ok);
    if (!ok) return;

    selectedShape->rotate(angle);
    update();
}

void MainWindow::resizeShape() // изменение размера
{
    if (!selectedShape) return;

    bool ok;
    int width = QInputDialog::getInt(this, tr("Изменть размеры формы"), tr("Введите новую ширину:"), selectedShape->width(), 1, 10000, 1, &ok);
    if (!ok) return;
    int height = QInputDialog::getInt(this, tr("Изменить размеры формы"), tr("Введите новую высоту:"), selectedShape->height(), 1, 10000, 1, &ok);
    if (!ok) return;

    selectedShape->resize(width, height);
    update();
}

void MainWindow::selectShapeFromList()
{

}
void MainWindow::selectShapeAt(const QPoint &point) //поиск фигуры по координатам
{
    for (Shapes *shape : shapes) {
        if (shape->contains(point)) {
            selectedShape = shape;
            updateStatusBar();
            return;
        }
    }
    selectedShape = nullptr;
    updateStatusBar();
}

void MainWindow::updateShapeList() // обновление Фигур
{
    shapeListWidget->clear();
    for (Shapes *shape : shapes) {
        shapeListWidget->addItem(shape->getName());
    }
}
