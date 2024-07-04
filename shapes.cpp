#include "shapes.h"
#include <QPainterPath>
#include <QPolygonF>
#include <cmath>

// линия
void Line::draw(QPainter &painter) // рисование линии
{
    painter.setPen(QPen(color, penWidth));
    painter.drawLine(start, end);
}

void Line::scale(double xScale, double yScale) // размер линии
{
    end.setX(start.x() + (end.x() - start.x()) * xScale);
    end.setY(start.y() + (end.y() - start.y()) * yScale);
}

bool Line::contains(const QPoint &point) const // проверка на точку
{
    QPainterPath path;
    path.moveTo(getStartPoint());
    path.lineTo(getEndPoint());
    return path.contains(point);
}

Shapes::ShapeType Line::getType() const // тип фигуры
{
    return Shapes::LineType;
}

int Line::width() const // ширина и высота
{
    return std::abs(end.x() - start.x());
}

int Line::height() const
{
    return std::abs(end.y() - start.y());
}

void Line::moveBy(int dx, int dy) // не працуе
{
    start += QPoint(dx, dy);
    end += QPoint(dx, dy);
}

void Line::rotate(int angle) // не працуе
{
    Q_UNUSED(angle);
}
void Line::resize(int width, int height) // не працуе
{
    end.setX(start.x() + width);
    end.setY(start.y() + height);
}

// Polyline
void Polyline::draw(QPainter &painter)
{
    painter.setPen(QPen(color, penWidth));
    painter.drawPolyline(points.constData(), points.size());
}

void Polyline::scale(double xScale, double yScale)
{
    for (QPoint &point : points)
    {
        point.setX(start.x() + (point.x() - start.x()) * xScale);
        point.setY(start.y() + (point.y() - start.y()) * yScale);
    }
}

bool Polyline::contains(const QPoint &point) const
{
    QPolygonF polygon(points);
    return polygon.containsPoint(point, Qt::OddEvenFill);
}

Shapes::ShapeType Polyline::getType() const
{
    return Shapes::PolylineType;
}

int Polyline::width() const
{
    if (points.isEmpty())
        return 0;
    int minX = points[0].x(), maxX = points[0].x();
    for (const QPoint &point : points)
    {
        if (point.x() < minX) minX = point.x();
        if (point.x() > maxX) maxX = point.x();
    }
    return maxX - minX;
}

int Polyline::height() const
{
    if (points.isEmpty())
        return 0;
    int minY = points[0].y(), maxY = points[0].y();
    for (const QPoint &point : points)
    {
        if (point.y() < minY) minY = point.y();
        if (point.y() > maxY) maxY = point.y();
    }
    return maxY - minY;
}

void Polyline::moveBy(int dx, int dy)
{
    for (QPoint &point : points)
    {
        point += QPoint(dx, dy);
    }
}

void Polyline::rotate(int angle)
{
    double radians = angle * M_PI / 180.0;
    double cosA = std::cos(radians);
    double sinA = std::sin(radians);
    for (QPoint &point : points)
    {
        int x = point.x() - start.x();
        int y = point.y() - start.y();
        point.setX(start.x() + x * cosA - y * sinA);
        point.setY(start.y() + x * sinA + y * cosA);
    }
}

void Polyline::resize(int width, int height)
{
    if (points.size() < 2) return;
    QPoint lastPoint = points.last();
    lastPoint.setX(start.x() + width);
    lastPoint.setY(start.y() + height);
    points.last() = lastPoint;
}

// Прямоугольник
void Rect::draw(QPainter &painter)
{
    painter.setPen(QPen(color, penWidth));
    painter.drawRect(QRect(start, end));
}

void Rect::scale(double xScale, double yScale)
{
    end.setX(start.x() + (end.x() - start.x()) * xScale);
    end.setY(start.y() + (end.y() - start.y()) * yScale);
}

bool Rect::contains(const QPoint &point) const
{
    return QRect(start, end).contains(point);
}

Shapes::ShapeType Rect::getType() const
{
    return Shapes::RectangleType;
}

int Rect::width() const
{
    return std::abs(end.x() - start.x());
}

int Rect::height() const
{
    return std::abs(end.y() - start.y());
}

void Rect::moveBy(int dx, int dy)
{
    start += QPoint(dx, dy);
    end += QPoint(dx, dy);
}

void Rect::rotate(int angle)
{
    QPoint center = (start + end) / 2;
    double radians = angle * M_PI / 180.0;
    double cosA = std::cos(radians);
    double sinA = std::sin(radians);

    auto rotatePoint = [&](QPoint &point) {
        int x = point.x() - center.x();
        int y = point.y() - center.y();
        point.setX(center.x() + x * cosA - y * sinA);
        point.setY(center.y() + x * sinA + y * cosA);
    };

    rotatePoint(start);
    rotatePoint(end);
}

void Rect::resize(int width, int height)
{
    end.setX(start.x() + width);
    end.setY(start.y() + height);
}


// Эллипс
void Ellipse::draw(QPainter &painter)
{
    painter.setPen(QPen(color, penWidth));
    painter.drawEllipse(QRect(start, end));
}

void Ellipse::scale(double xScale, double yScale)
{
    end.setX(start.x() + (end.x() - start.x()) * xScale);
    end.setY(start.y() + (end.y() - start.y()) * yScale);
}

bool Ellipse::contains(const QPoint &point) const
{
    QRect rect(start, end);
    QPainterPath path;
    path.addEllipse(rect);
    return path.contains(point);
}

Shapes::ShapeType Ellipse::getType() const
{
    return Shapes::EllipseType;
}

int Ellipse::width() const
{
    return std::abs(end.x() - start.x());
}

int Ellipse::height() const
{
    return std::abs(end.y() - start.y());
}

void Ellipse::moveBy(int dx, int dy)
{
    start += QPoint(dx, dy);
    end += QPoint(dx, dy);
}

void Ellipse::rotate(int angle)
{
    QPoint center = (start + end) / 2;
    double radians = angle * M_PI / 180.0;
    double cosA = std::cos(radians);
    double sinA = std::sin(radians);

    auto rotatePoint = [&](QPoint &point) {
        int x = point.x() - center.x();
        int y = point.y() - center.y();
        point.setX(center.x() + x * cosA - y * sinA);
        point.setY(center.y() + x * sinA + y * cosA);
    };

    rotatePoint(start);
    rotatePoint(end);
}

void Ellipse::resize(int width, int height)
{
    end.setX(start.x() + width);
    end.setY(start.y() + height);
}


//геттеры
QString Line::getName() const {
    return "Line";
}

QString Rect::getName() const {
    return "Rectangle";
}

QString Ellipse::getName() const {
    return "Ellipse";
}

QString Polyline::getName() const {
    return "Polyline";
}
