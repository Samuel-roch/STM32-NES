#include "display.h"
#include "ui_display.h"
#include <QMouseEvent>

Display::Display(QWidget *parent) :
    QWidget(parent),
    m_screenBuffer(480, 272, QImage::Format_RGB32),
    ui(new Ui::Display)
{
    ui->setupUi(this);
    setMouseTracking(true);
}

Display::~Display()
{
    delete ui;
}

void Display::flush()
{
    ui->frame->setPixmap(QPixmap::fromImage(m_screenBuffer));
}

void Display::mousePressEvent(QMouseEvent *event)
{
    m_last_x = event->pos().x();
    m_last_y = event->pos().y();
    m_left_button_down = true;
}

void Display::mouseReleaseEvent(QMouseEvent *event)
{
    m_last_x = event->pos().x();
    m_last_y = event->pos().y();
    m_left_button_down = false;
}

void Display::mouseMoveEvent(QMouseEvent *event)
{
    m_last_x = event->pos().x();
    m_last_y = event->pos().y();
}
