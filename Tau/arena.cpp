#include "arena.h"
#include <QDebug>

Arena::Arena()
{
    radius = permRadius;

    bkg = new QGraphicsRectItem(0,0,windowWidth,windowHeight);

    rectangle = QRectF();
    setRect(0,0,0,0);

    grad->setColorAt(0,QColor(0,0,0,0));
    grad->setColorAt(1,QColor::fromRgb(0,0,0,255));
    bkg->setPen(*clear);
    bkg->setBrush(QBrush(*grad));
    bkg->setZValue(-1);
    bkg->setCacheMode(QGraphicsItem::DeviceCoordinateCache);

    //this->setCacheMode(QGraphicsItem::ItemCoordinateCache);
    this->setTransformOriginPoint(windowWidth/2,windowHeight/2);
}

void Arena::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setPen(*redPen);
    painter->setBrush(this->brush);
    painter->drawArc(rectangle, -1440, 2880);
    painter->setPen(*bluePen);
    painter->drawArc(rectangle,1440,2880);
}

void Arena::setPen(const QPen &pen)
{
    bluePen = new QPen(pen);
    bluePen->setCapStyle(Qt::SquareCap);
    bluePen->setColor(QColor::fromRgb(165,165,255));
    redPen = new QPen(pen);
    redPen->setCapStyle(Qt::SquareCap);
    redPen->setColor(QColor::fromRgb(255,165,165));
}

void Arena::setRadius(double rad)
{
    this->update();
    this->radius = rad;
    this->setRect(windowWidth/2-rad,windowHeight/2-rad,rad*2,rad*2);
}

void Arena::startPulse(int bpm)
{
    updateBg = new QTimer();
    updateBg->setTimerType(Qt::PreciseTimer);
    QObject::connect(updateBg,SIGNAL(timeout()),this,SLOT(pulse()));

    if(bpm == 0){
        grad->setColorAt(0,QColor::fromRgb(150,150,150,255));
        bkg->setBrush(QBrush(*grad));
        return;
    }
    pulse();
    updateBg->start(60000/bpm);
}

void Arena::setPermRadius(int prad)
{
    grad->setRadius(prad/0.9);
    bkg->setBrush(QBrush(*grad));
    this->setRect(windowWidth/2-prad,windowHeight/2-prad,prad*2,prad*2);
    this->permRadius = prad;
    this->update();
}

QRectF Arena::boundingRect() const
{
    return QRectF(rectangle.x()-10,rectangle.y()-10,rectangle.width()+20,rectangle.height()+20);
}

void Arena::setRect(qreal left,qreal top, qreal width, qreal height)
{
    this->rectangle.setLeft(left);
    this->rectangle.setTop(top);
    this->rectangle.setWidth(width);
    this->rectangle.setHeight(height);
}

void Arena::pulse()
{
    if(!pulsing)
        return;
    pulsed();

    grad->setColorAt(0,QColor::fromRgb(randomInBound(50,150),randomInBound(50,150),randomInBound(50,150),255));
    bkg->setBrush(QBrush(*grad));

    setRadius(permRadius+pulseDist);
}

void Arena::changeBPM(int i)
{
    if(updateBg->isActive())
        updateBg->stop();

    if(BPM[i]!=0){
        updateBg->start(60000/BPM[i]);
    }else{
        grad->setColorAt(0,QColor::fromRgb(150,150,150,255));
        bkg->setBrush(QBrush(*grad));
    }
}
