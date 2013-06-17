#ifndef OPENCHART_H
#define OPENCHART_H
#include "global_header.h"
#include <QtWidgets>
#include "qchartpiece.h"

struct roundLabelPoints
{
    QString name;
    QPointF start;
    QPointF intersec;
    float value;
};

class SHAREDLIBSHARED_EXPORT OpenChart : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(Tipos Tipo READ Tipo WRITE setTipo)
    Q_PROPERTY(bool useTitle READ usingTitle WRITE enableTitle)
    Q_PROPERTY(QString Title READ Title WRITE setTitle)
    Q_PROPERTY(QFont Letra READ Letra WRITE setLetra)
    Q_PROPERTY(bool Sombra READ Sombra WRITE verSombra)
    Q_PROPERTY(bool Etiquetas READ Etiquetas WRITE verEtiquetas)
    Q_PROPERTY(bool Valores READ Valores WRITE verValores)
    Q_PROPERTY(bool ValoresEjeY READ ValoresEjeY WRITE verValoresEjeY)
    Q_PROPERTY(bool Leyenda READ LeyendaEnabled WRITE EnableLeyenda)
    Q_PROPERTY(Tipo_leyenda TipoLeyenda READ TipoLeyenda WRITE setTipoleyenda)
    Q_PROPERTY(QFont LetraLeyenda READ LetraLeyenda WRITE setLetraLeyenda)
    Q_PROPERTY(bool useAnimation READ useAnimation WRITE setAnimation)
    Q_PROPERTY(int animationDuration READ animationDuration WRITE setAnimationDuration)
public:
    OpenChart(QWidget *parent = 0);
    QSize minimumSizeHint() const;


    Q_ENUMS(Tipos)
    enum Tipos {Barras , BarraMultiple , Sectores_2D, Sectores_3D , Lineas};

    Tipos Tipo(){return m_type;}
    void setTipo(Tipos t){m_type = t;
                          this->repaint();}

    bool usingTitle(){return m_usingTitle;}
    void enableTitle(bool b){m_usingTitle = b; this->repaint();}

    QString Title(){return m_title;}
    void setTitle(QString s){m_title = s; this->repaint();}

    bool LeyendaEnabled(){return m_useLeyenda;}
    void EnableLeyenda(bool b){m_useLeyenda = b;
                               this->repaint();}
    Q_ENUMS(Tipo_leyenda)
    enum Tipo_leyenda {Vertical , Circular};

    Tipo_leyenda TipoLeyenda(){return m_tipoLeyenda;}
    void setTipoleyenda(Tipo_leyenda t){m_tipoLeyenda = t;
                                        this->repaint();}

    bool Sombra(){return m_sombra;}
    void verSombra(bool b){m_sombra = b;
                           this->repaint();}

    bool Etiquetas(){return m_labels;}
    void verEtiquetas(bool b){m_labels = b;
                              this->repaint();}

    bool Valores(){return m_values;}
    void verValores(bool b){m_values = b;
                            this->repaint();}

    bool ValoresEjeY(){return m_valuesEnY;}
    void verValoresEjeY(bool b){m_valuesEnY = b;
                                this->repaint();}

    QFont LetraLeyenda(){return m_letraLeyenda;}
    void setLetraLeyenda(QFont f){m_letraLeyenda = f;
                                  this->repaint();}

    QFont Letra(){return m_letra;}
    void setLetra(QFont f){m_letra = f;
                           this->repaint();}

    bool useAnimation(){return m_animation;}
    void setAnimation(bool b){m_animation = b;this->repaint();}

    int animationDuration(){return m_aniDuration;}
    void setAnimationDuration(int i){m_aniDuration = i;}

    void addItem(QString nombre, float value);
    void addItem(QString nombre, float value, QColor color);

    void addItem(QString nombre, QVector<float> values);
    void addItem(QString nombre, QVector<float> values, QColor color);
    void addMulibarColor(QString nombre , QColor color);
    void addLineaStop(QString s){lineasStops.append(s);}
    void addLineaStops(QStringList sl){lineasStops = sl;}

    void Clear();
protected:
    void paintEvent(QPaintEvent *);
    void drawBar(QPainter* painter);
    void drawMultiBar(QPainter* painter);
    void drawLines(QPainter* painter);
    void drawPie(QPainter* painter);
private:
    int m_left , m_top , m_bottom , m_rigth , m_width , m_heigth, m_xAxis;
    int m_maxValue, m_mayor , m_menor;
    Tipos m_type;
    bool m_usingTitle;
    QString m_title;

    bool m_useLeyenda;
    Tipo_leyenda m_tipoLeyenda;

    bool m_sombra;
    bool m_labels;
    bool m_values;
    bool m_valuesEnY;
    bool m_animation;
    int m_aniDuration;

    QFont m_letraLeyenda;
    QFont m_letra;

    QVector<QChartPiece> pieces;
    QVector<QPair<QString , QColor> >multibarColors;
    QStringList lineasStops;

    void drawAxis(QPainter* painter);
    void drawLeyendaVertical(QPainter* painter);
    void drawLeyendaCircular(QPainter* painter);
    void drawYValues(QPainter* painter);
    QPointF GetPoint(double angle, double R1 = 0, double R2 = 0);

    Q_PROPERTY(float percent READ percent WRITE setPercent)
    float percent(){return m_percent;}
    void setPercent(float f);
    void startAnimation();

    float m_percent;
};

#endif
