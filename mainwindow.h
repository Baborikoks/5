#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "shapes.h"
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QToolBar>
#include <QMessageBox>
#include <QComboBox>
#include <QColor>
#include <QPixmap>
#include <QRadioButton>
#include <QButtonGroup>
#include <QActionGroup>
#include <QContextMenuEvent>
#include <QStatusBar>
#include <QLabel>
#include <QListWidget>
#include <QFileDialog>
#include <QTextStream>
#include <sstream>
#include <string>
#include <QColorDialog>
#include <QInputDialog>
#include <QTimer>
#include <QCloseEvent>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void animateShape();
    void startAnimation();
    void stopAnimation();
    void saveFile();
    void openFile();
    void exitFromFile();
    void setColor();
    void setPenWidth();
    void selectShape();
    void updateStatusBar();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void deleteShape();
    void moveShape();
    void rotateShape();
    void resizeShape();
    void selectShapeFromList();


    void updateShapeList();
protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);


private:
    void CreateMenu();
    void CreateActions();
    void Connect();

    QMenu *fileMenu;
    QAction *openAct;
    QAction *saveAct;
    QAction *exitAct;

    QMenu *editMenu;
    QAction *penWidthAct;
    QAction *colorAct;

    QMenu *shapeMenu;
    QAction *lineAct;
    QAction *polylineAct;
    QAction *rectAct;
    QAction *ellipseAct;
    QColor currentColor= Qt::black;
    int currentPenWidth = 1;
    Ui::MainWindow *ui;

    enum ShapeType { LineType, PolylineType, RectangleType, EllipseType };
    ShapeType currentShape = LineType;
    QVector<Shapes*> shapes;
    Shapes* tempShape = nullptr;
    bool drawing = false;

    void contextMenuEvent(QContextMenuEvent *event) override;
    void startShapeAnimation(Shapes* shape);
    void stopShapeAnimation(Shapes* shape);

    QTimer *animationTimer;
    Shapes* animatedShape = nullptr;
    int animationStep = 0;
    Shapes* selectedShape = nullptr;
    Shapes* findShapeAt(const QPoint &point);
    void selectShapeAt(const QPoint &point);

    QAction *deleteAct;
    QAction *moveAct;
    QAction *rotateAct;
    QAction *resizeAct;
    QListWidget *shapeListWidget;

};
#endif // MAINWINDOW_H
