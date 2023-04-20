#ifndef DISPLAY_H
#define DISPLAY_H

#include <QWidget>

namespace Ui {
class Display;
}

class Display : public QWidget
{
    Q_OBJECT

public:
    explicit Display(QWidget *parent = 0);
    ~Display();
    QImage m_screenBuffer;
    int16_t m_last_x = 0;
    int16_t m_last_y = 0;
    bool m_left_button_down = false;

    void flush();


private:
    Ui::Display *ui;

protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // DISPLAY_H
