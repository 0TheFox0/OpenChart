#include "openchart.h"
#include <QDebug>
#include <QTime>
OpenChart::OpenChart(QWidget *parent) :
    QWidget(parent)
{
    qsrand(QTime::currentTime().msec());

    m_mayor =0;
    m_maxValue = 0;
    m_menor = 0;

    m_type = BarraMultiple;
    m_sombra = true;
    m_useLeyenda = true;
    m_tipoLeyenda = Vertical;

    m_title = "Un titulo";

    QVector<float>v1;
    v1 << 10 << 20;
    QVector<float>v2;
    v2 << -10 << -20;
    addItem("holaaaaaaaaaaaaaaaaaaaaaaaaaa",v1);
    addItem("hola-adios",v1);
    addItem("adios",v2);

    addMulibarColor("Compras",Qt::darkRed);
    addMulibarColor("Ventas",Qt::darkGreen);
}

QSize OpenChart::minimumSizeHint() const
{
    return QSize(200,100);
}

void OpenChart::addItem(QString nombre, float value)
{
    int r = qrand()%255;
    int g = qrand()%255;
    int b = qrand()%255;
    addItem(nombre,value,QColor(r,g,b));
}

void OpenChart::addItem(QString nombre, float value, QColor color)
{
    ChartPiece p;
    p.nombre = nombre;
    p.addValue(value);
    p.color = color;
    pieces.append(p);

    m_mayor = qMax(m_mayor,(int)value);
    m_menor = qMin(m_menor,(int)value);
    m_maxValue = qMax(m_mayor, -m_menor);
}

void OpenChart::addItem(QString nombre, QVector<float> values)
{
    int r = qrand()%255;
    int g = qrand()%255;
    int b = qrand()%255;
    addItem(nombre,values,QColor(r,g,b));
}

void OpenChart::addItem(QString nombre, QVector<float> values, QColor color)
{
    ChartPiece p;
    p.nombre = nombre;
    p.addValue(values);
    p.color = color;
    pieces.append(p);

    for(int i=0;i<values.size();i++)
    {
        float value = values.at(i);
        m_mayor = qMax(m_mayor,(int)value);
        m_menor = qMin(m_menor,(int)value);
    }
    m_maxValue = qMax(m_mayor, -m_menor);
}

void OpenChart::addMulibarColor(QString nombre, QColor color)
{
    QPair<QString , QColor> par;
    par.first = nombre;
    par.second = color;
    multibarColors.append(par);
}

void OpenChart::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setFont(m_letra);

   // painter.drawRect(0,0,width()-1,height()-1);

    int fontH = painter.fontMetrics().height();

    m_left = 5;
    m_rigth = this->width();
    if(m_valuesEnY)
    {
        QString s = QString::number(m_maxValue);
        if(m_menor<0)
            s.prepend("-");
        m_left += painter.fontMetrics().width(s);
    }
    if(m_useLeyenda)
    {
        int maxLength = 0;
        if(m_tipoLeyenda == Vertical && m_type != BarraMultiple)
        {
            for(int i=0;i< pieces.size(); i++)
                maxLength = qMax(maxLength,painter.fontMetrics().width(QString("%1 : %2").arg(pieces.at(i).nombre).arg(pieces.at(i).value())));
        }
        else if(m_tipoLeyenda == Vertical && m_type == BarraMultiple)
        {
            for(int i=0;i< multibarColors.size(); i++)
                maxLength = qMax(maxLength,painter.fontMetrics().width(multibarColors.at(i).first));
        }
        maxLength+= fontH + 10;
        m_rigth-=maxLength;
    }
    m_width = m_rigth - m_left;

    m_top = 0;
    m_bottom = this->height();
    if(m_labels)
    {
        if(m_type == BarraMultiple)
        {
            if(m_mayor >= 0 && m_menor >=0)
                m_bottom -= fontH;
            else if(m_mayor <= 0 && m_menor <=0)
                m_top+=fontH;
            else
            {
                m_top+=fontH;
                m_bottom -= fontH;
            }
        }
        else if(m_type == Barras)
        {
            if(m_mayor<=0)
                m_top+=fontH;
            if(m_menor>=0)
                m_bottom-=fontH;
        }
    }
    if(m_values)
    {
        if(m_mayor >0)
            m_top+=fontH;
        if(m_menor<0)
            m_bottom -=fontH;
    }
    if(m_usingTitle)
    {
        QFont title = m_letra;
        title.setItalic(true);
        title.setBold(true);
        title.setUnderline(true);
        painter.save();
        painter.setFont(title);
        m_top+=fontH;
        m_top+=3;
        painter.drawText(this->width()/2-painter.fontMetrics().width(m_title)/2,fontH,m_title);
        painter.restore();
    }
    m_heigth = m_bottom - m_top;

    int range = m_mayor;
    if(m_menor <0)
        range -= m_menor;

    if(m_mayor == m_maxValue)
    {
        double pPerc = (m_mayor*100)/range;
        double p = pPerc/100.0;
        double pieceHeigth = m_heigth*p;
        m_xAxis = m_top + pieceHeigth;
    }
    else
    {
        double pPerc = (m_menor*100)/range;
        double p = pPerc/100.0;
        double pieceHeigth = m_heigth*p;
        m_xAxis = m_bottom + pieceHeigth;
    }

//       painter.setPen(Qt::red);
//       painter.drawRect(m_left,m_top,m_width,m_heigth);
//       painter.drawLine(m_left,m_xAxis,m_rigth,m_xAxis);
    switch (m_type) {
    case Barras:
        drawBar(&painter);
        drawAxis(&painter);
        break;
    case BarraMultiple:
        drawMultiBar(&painter);
        drawAxis(&painter);
    default:
        break;
    }
}

void OpenChart::drawBar(QPainter *painter)
{
    painter->save();


    double pDist = 15;
    double pieceWidth = (m_width-(pieces.size())*pDist)/pieces.size();

    QPen pen;
    pen.setWidth(2);

    int range = m_mayor;
    if(m_menor <0)
        range -= m_menor;

    for (int i=0;i<pieces.size();i++)
    {
        bool isPositive = pieces.at(i).value()>=0;

        double pPerc = (pieces.at(i).value()*100)/range;
        double p = pPerc/100.0;
        double pieceHeigth = m_heigth*p;
        if(pieceHeigth<0)
            pieceHeigth = -pieceHeigth;

        int pieceXPos = pDist+i*(pieceWidth + pDist);

        if (m_sombra)
        {
            painter->setPen(Qt::NoPen);
            painter->setBrush(Qt::darkGray);

            if(isPositive)
                painter->drawRect(m_left+pieceXPos-pDist/2,m_xAxis,pieceWidth,-pieceHeigth+pDist/2);
            else
                painter->drawRect(m_left+pieceXPos-pDist/2,m_xAxis,pieceWidth,pieceHeigth-pDist/2);
        }

        if(isPositive)
        {
            QLinearGradient gradient(m_left+m_width/2,m_xAxis-pieceHeigth-180,m_left+m_width/2,m_xAxis);
            gradient.setColorAt(0,Qt::black);
            gradient.setColorAt(1,pieces.at(i).color);
            painter->setBrush(gradient);
        }
        else
        {
            QLinearGradient gradientNeg(m_left+m_width/2,m_xAxis,m_left+m_width/2,m_xAxis+pieceHeigth+180);
            gradientNeg.setColorAt(1,Qt::black);
            gradientNeg.setColorAt(0,pieces.at(i).color);
            painter->setBrush(gradientNeg);
        }

        pen.setColor(pieces.at(i).color);
        painter->setPen(pen);

        if(isPositive)
            painter->drawRect(m_left+pieceXPos , m_xAxis , pieceWidth , -pieceHeigth);
        else
            painter->drawRect(m_left+pieceXPos , m_xAxis , pieceWidth , pieceHeigth);

        QString label = QString::number(pieces.at(i).value());

        painter->setPen(Qt::SolidLine);
        int fontHeigth = painter->fontMetrics().height();
        if(m_labels)
        {
            QString s = pieces.at(i).nombre;
            if(painter->fontMetrics().width(s)> pieceWidth)
            {
                s = "...";
                for (int a = 0; a<pieces.at(i).nombre.size();a++)
                {
                    if(painter->fontMetrics().width(s)>= pieceWidth-10)
                        break;
                    s.insert(a,pieces.at(i).nombre.at(a));
                }
            }
            if(isPositive)
                painter->drawText(m_left+pieceXPos+pieceWidth/2-painter->fontMetrics().width(s)/2,m_xAxis+fontHeigth,s);
            else
                painter->drawText(m_left+pieceXPos+pieceWidth/2-painter->fontMetrics().width(s)/2,m_xAxis-fontHeigth/2,s);
        }
        if(m_values)
        {
            if(isPositive)
                painter->drawText(m_left+pieceXPos+pieceWidth/2-painter->fontMetrics().width(label)/2,
                                  m_xAxis-pieceHeigth-fontHeigth/2+2,
                                  label);
            else
                painter->drawText(m_left+pieceXPos+pieceWidth/2-painter->fontMetrics().width(label)/2,
                                  m_xAxis+pieceHeigth+fontHeigth-1,
                                  label);
        }
    }
    painter->restore();
    if(m_useLeyenda)
        drawLeyendaVertical(painter);
}

void OpenChart::drawMultiBar(QPainter *painter)
{
    double pDist = 15;
    double iDist = 4;
    double pieceWidth = (m_width-(pieces.size())*pDist)/pieces.size();

    QPen pen;
    pen.setWidth(3);

    int range = m_mayor;
    if(m_menor <0)
        range -= m_menor;

    for (int i=0;i<pieces.size();i++)
    {
        while(multibarColors.size() < pieces.at(i).getValues().size())
        {
            int r = qrand()%255;
            int g = qrand()%255;
            int b = qrand()%255;
            QPair<QString , QColor> par;
            par.first="";
            par.second = QColor(r,g,b);
            multibarColors.append(par);
        }
        double subWidth = (pieceWidth-((pieces.at(i).getValues().size()-1)*iDist))/pieces.at(i).getValues().size();

        for(int a=pieces.at(i).getValues().size()-1;a>-1;a--)
        {
            bool isPositive = pieces.at(i).getValues().at(a) >=0;

            double pPerc = (pieces.at(i).getValues().at(a)*100)/range;
            double p = pPerc/100.0;
            double pieceHeigth = m_heigth*p;
            if(pieceHeigth<0)
                pieceHeigth = -pieceHeigth;

            int pieceXPos = pDist+i*(pieceWidth + pDist) + (a*(iDist + subWidth));

            int blockXPos = pDist+i*(pieceWidth + pDist);

            if (m_sombra)
            {
                painter->setPen(Qt::NoPen);
                painter->setBrush(Qt::darkGray);

                if(isPositive)
                    painter->drawRect(m_left+pieceXPos-pDist/2,m_xAxis,subWidth,-pieceHeigth+pDist/2);
                else
                    painter->drawRect(m_left+pieceXPos-pDist/2,m_xAxis,subWidth,pieceHeigth-pDist/2);
            }

            if(isPositive)
            {
                QLinearGradient gradient(m_left+m_width/2,m_xAxis-pieceHeigth-180,m_left+m_width/2,m_xAxis);
                gradient.setColorAt(0,Qt::black);
                gradient.setColorAt(1,multibarColors.at(a).second);
                painter->setBrush(gradient);
            }
            else
            {
                QLinearGradient gradientNeg(m_left+m_width/2,m_xAxis,m_left+m_width/2,m_xAxis+pieceHeigth+180);
                gradientNeg.setColorAt(1,Qt::black);
                gradientNeg.setColorAt(0,multibarColors.at(a).second);
                painter->setBrush(gradientNeg);
            }

            pen.setColor(multibarColors.at(a).second);
            painter->setPen(pen);

            if(isPositive)
                painter->drawRect(m_left+pieceXPos , m_xAxis , subWidth , -pieceHeigth);
            else
                painter->drawRect(m_left+pieceXPos , m_xAxis , subWidth , pieceHeigth);

            QString label = QString::number(pieces.at(i).getValues().at(a));

            painter->setPen(Qt::SolidLine);
            int fontHeigth = painter->fontMetrics().height();

            if(m_values)
            {
                if(isPositive)
                    painter->drawText(m_left+pieceXPos+subWidth/2-painter->fontMetrics().width(label)/2,
                                      m_xAxis-pieceHeigth-fontHeigth/2,
                                      label);
                else
                    painter->drawText(m_left+pieceXPos+subWidth/2-painter->fontMetrics().width(label)/2,
                                      m_xAxis+pieceHeigth+fontHeigth,
                                      label);
            }
            if(m_labels)
            {
                QString s = pieces.at(i).nombre;
                if(painter->fontMetrics().width(s)> pieceWidth)
                {
                    s = "...";
                    for (int a = 0; a<pieces.at(i).nombre.size();a++)
                    {
                        if(painter->fontMetrics().width(s)>= pieceWidth-10)
                            break;
                        s.insert(a,pieces.at(i).nombre.at(a));
                    }
                }
                if(m_mayor<=0)
                    painter->drawText(m_left+blockXPos+pieceWidth/2-painter->fontMetrics().width(s)/2,m_top-fontHeigth/2+3,s);
                else
                    painter->drawText(m_left+blockXPos+pieceWidth/2-painter->fontMetrics().width(s)/2,height()-fontHeigth/2+3,s);
            }
        }
    }
    if(m_useLeyenda)
        drawLeyendaVertical(painter);
}

void OpenChart::drawLines(QPainter *painter)
{
}

void OpenChart::drawAxis(QPainter *painter)
{
    painter->save();

    QPen pen;
    pen.setWidth(3);
    pen.setColor(Qt::darkGray);
    painter->setPen(pen);

    int top = 1;
    if(m_usingTitle)
        top+=painter->fontMetrics().height();

    painter->drawLine(m_left,top+2,m_left,this->height()-4);
    painter->drawLine(m_left,m_xAxis,m_left+m_width,m_xAxis);

    //painter->setPen(Qt::SolidLine);
    pen.setWidth(1);
    painter->setPen(pen);
    if(m_mayor >0 )
    {
        painter->drawLine(m_left,top,m_left+4,top+9);
        painter->drawLine(m_left,top,m_left-4,top+9);
    }
    if(m_menor < 0)
    {
        painter->drawLine(m_left,this->height()-1,m_left+4,this->height()-11);
        painter->drawLine(m_left,this->height()-1,m_left-4,this->height()-11);
    }

    painter->restore();
}

void OpenChart::drawLeyendaVertical(QPainter *painter)
{
    int dist = 5;
    if(m_type != BarraMultiple)
    {
        for (int i=pieces.size()-1;i>=0;i--)
        {
            painter->setBrush(pieces.at(i).color);
            float x = m_rigth+dist;
            float y = m_top+dist+i*(painter->fontMetrics().height()+2*dist);
            painter->drawRect(x,y,painter->fontMetrics().height(),painter->fontMetrics().height());
            QString s = pieces.at(i).nombre + " : " + QString::number(pieces.at(i).value());
            painter->drawText(x+painter->fontMetrics().height()+dist,y+painter->fontMetrics().height()/2+dist,s);
        }
    }
    else
    {
        for (int i = multibarColors.size()-1;i>=0;i--)
        {
            painter->setBrush(multibarColors.at(i).second);
            float x = m_rigth+dist;
            float y = m_top+dist+i*(painter->fontMetrics().height()+2*dist);
            painter->drawRect(x,y,painter->fontMetrics().height(),painter->fontMetrics().height());
            QString s = multibarColors.at(i).first;
            painter->drawText(x+painter->fontMetrics().height()+dist,y+painter->fontMetrics().height()/2+dist,s);
        }
    }
}
