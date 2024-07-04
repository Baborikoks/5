#ifndef SHAPES_H
#define SHAPES_H

#include <QColor>
#include <QPoint>
#include <QVector>
#include <QPainter>
#include <QRegion>

class Shapes
{
public:
    enum ShapeType { LineType, PolylineType, RectangleType, EllipseType };
    virtual ~Shapes() {}
    virtual void draw(QPainter &painter) = 0;
    virtual void scale(double xScale, double yScale) = 0;
    virtual bool contains(const QPoint &point) const = 0;
    virtual void moveBy(int dx, int dy) = 0;
    virtual void rotate(int angle) = 0;
    virtual void resize(int width, int height) = 0;
    virtual QString getName() const = 0;
    virtual ShapeType getType() const = 0;
    void setStartPoint(const QPoint &point) { start = point; }
    void setEndPoint(const QPoint &point) { end = point; }
    void setColor(const QColor &c) { color = c; }
    void setPenWidth(int width) { penWidth = width; }
    QPoint getStartPoint() const { return start; }
    QPoint getEndPoint() const { return end; }
    virtual int width() const = 0;
    virtual int height() const = 0;

protected:
    Shapes(const QColor &color, int penWidth)
        : color(color), penWidth(penWidth) {}
    QPoint start, end;
    QColor color;
    int penWidth;

};

class Line : public Shapes
{
public:
    Line(const QColor &color, int penWidth) : Shapes(color, penWidth) {}
    void draw(QPainter &painter) override;
    void scale(double xScale, double yScale) override;
    bool contains(const QPoint &point) const override;
    ShapeType getType() const override;
    QString getName() const override;
    int width() const override;
    int height() const override;
    void moveBy(int dx, int dy) override;
    void rotate(int angle) override;
    void resize(int width, int height) override;
};

class Polyline : public Shapes
{
public:
    Polyline(const QColor &color, int penWidth) : Shapes(color, penWidth) {}
    void draw(QPainter &painter) override;
    void scale(double xScale, double yScale) override;
    bool contains(const QPoint &point) const override;
    void addPoint(const QPoint &point) { points.append(point); }
    ShapeType getType() const override;
    QString getName() const override;
    int width() const override;
    int height() const override;
    void moveBy(int dx, int dy) override;
    void rotate(int angle) override;
    void resize(int width, int height) override;

private:
    QVector<QPoint> points;

};

class Rect : public Shapes
{
public:
    Rect(const QColor &color, int penWidth) : Shapes(color, penWidth) {}
    void draw(QPainter &painter) override;
    void scale(double xScale, double yScale) override;
    bool contains(const QPoint &point) const override;
    ShapeType getType() const override;
    QString getName() const override;
    int width() const override;
    int height() const override;
    void moveBy(int dx, int dy) override;
    void rotate(int angle) override;
    void resize(int width, int height) override;


};

class Ellipse : public Shapes
{
public:
    Ellipse(const QColor &color, int penWidth) : Shapes(color, penWidth) {}
    void draw(QPainter &painter) override;
    void scale(double xScale, double yScale) override;
    bool contains(const QPoint &point) const override;
    ShapeType getType() const override;
    QString getName() const override;
    int width() const override;
    int height() const override;
    void moveBy(int dx, int dy) override;
    void rotate(int angle) override;
    void resize(int width, int height) override;
};

#endif // SHAPES_H
