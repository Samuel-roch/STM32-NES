#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "lvgl.h"
#include "hal_stm32.h"
#include <QDebug>
#include "App/Gui/app_gui.h"

MainWindow * _mw;
extern "C"
{

lv_color_t  FB1[480 * 272];
lv_color_t  FB2[480 * 272];

lv_indev_t * indev_touchpad;
static lv_indev_drv_t indev_drv;

void dev_touch_read(bool * click, int16_t * x, int16_t * y)
{
    *x      = _mw->m_display->m_last_x;
    *y      = _mw->m_display->m_last_y;
    *click  = _mw->m_display->m_left_button_down ? true : false;
}

static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    /*The most simple case (but also the slowest) to put all pixels to the screen one-by-one*/

    int32_t x;
    int32_t y;
    for(y = area->y1; y <= area->y2; y++)
    {
        for(x = area->x1; x <= area->x2; x++)
        {
            _mw->m_display->m_screenBuffer.setPixel(x, y, lv_color_to32(*color_p));
            color_p++;
        }
    }
    _mw->m_display->flush();
}

static void touchpad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    static lv_coord_t last_x = 0;
    static lv_coord_t last_y = 0;

    /*Save the pressed coordinates and the state*/
    if(_mw->m_display->m_left_button_down)
    {
        last_x = _mw->m_display->m_last_x;
        last_y = _mw->m_display->m_last_y;
        data->state = LV_INDEV_STATE_PRESSED;
    }
    else
    {
        data->state = LV_INDEV_STATE_RELEASED;
    }

    /*Set the last pressed coordinates*/
    data->point.x = last_x;
    data->point.y = last_y;
}

 uint32_t HAL_GetTick(void)
 {
    return _mw->m_tick;
 }

}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _mw = this;
    m_display = new Display(ui->widget);

    lv_init();

    static lv_disp_draw_buf_t draw_buf_dsc;
    lv_disp_draw_buf_init(&draw_buf_dsc, FB1, FB2, 480 * 272);   /*Initialize the display buffer*/

    static lv_disp_drv_t disp_drv;                         /*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv);                    /*Basic initialization*/
    disp_drv.hor_res = 480;
    disp_drv.ver_res = 272;
    disp_drv.flush_cb = disp_flush;
    disp_drv.draw_buf = &draw_buf_dsc;
    lv_disp_drv_register(&disp_drv);


    app_gui_init();

    m_tick = 0;
    startTimer(5);
    m_elapsedTimer.start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event)
    m_tick += 5;
    lv_timer_handler();
}

void MainWindow::on_dial_actionTriggered(int action)
{
    qDebug() << action;
}
