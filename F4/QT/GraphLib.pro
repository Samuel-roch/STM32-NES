#-------------------------------------------------
#
# Project created by QtCreator 2022-06-03T20:15:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GraphLib
TEMPLATE = app

INCLUDEPATH += $$PWD/ ../Core/Libs/Graph_Lib
INCLUDEPATH += $$PWD/ ../Core
INCLUDEPATH += $$PWD/ hw_sim/

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
        display.cpp \
    ../Core/App/Gui/splash/logo.c \
    ../Core/App/Gui/app_gui.c \
    ../Core/App/Gui/splash/splash.c \
    ../Core/App/Gui/home/home.c \
    ../Core/Libs/Graph_lib/core/lv_disp.c \
    ../Core/Libs/Graph_lib/core/lv_event.c \
    ../Core/Libs/Graph_lib/core/lv_group.c \
    ../Core/Libs/Graph_lib/core/lv_indev.c \
    ../Core/Libs/Graph_lib/core/lv_indev_scroll.c \
    ../Core/Libs/Graph_lib/core/lv_obj.c \
    ../Core/Libs/Graph_lib/core/lv_obj_class.c \
    ../Core/Libs/Graph_lib/core/lv_obj_draw.c \
    ../Core/Libs/Graph_lib/core/lv_obj_pos.c \
    ../Core/Libs/Graph_lib/core/lv_obj_scroll.c \
    ../Core/Libs/Graph_lib/core/lv_obj_style.c \
    ../Core/Libs/Graph_lib/core/lv_obj_style_gen.c \
    ../Core/Libs/Graph_lib/core/lv_obj_tree.c \
    ../Core/Libs/Graph_lib/core/lv_refr.c \
    ../Core/Libs/Graph_lib/core/lv_theme.c \
    ../Core/Libs/Graph_lib/draw/arm2d/lv_gpu_arm2d.c \
    ../Core/Libs/Graph_lib/draw/gd32_ipa/lv_gpu_gd32_ipa.c \
    ../Core/Libs/Graph_lib/draw/nxp/pxp/lv_draw_pxp_blend.c \
    ../Core/Libs/Graph_lib/draw/nxp/pxp/lv_gpu_nxp_pxp.c \
    ../Core/Libs/Graph_lib/draw/nxp/pxp/lv_gpu_nxp_pxp_osa.c \
    ../Core/Libs/Graph_lib/draw/nxp/vglite/lv_draw_vglite_arc.c \
    ../Core/Libs/Graph_lib/draw/nxp/vglite/lv_draw_vglite_blend.c \
    ../Core/Libs/Graph_lib/draw/nxp/vglite/lv_draw_vglite_rect.c \
    ../Core/Libs/Graph_lib/draw/nxp/vglite/lv_gpu_nxp_vglite.c \
    ../Core/Libs/Graph_lib/draw/nxp/lv_gpu_nxp.c \
    ../Core/Libs/Graph_lib/draw/sdl/lv_draw_sdl.c \
    ../Core/Libs/Graph_lib/draw/sdl/lv_draw_sdl_arc.c \
    ../Core/Libs/Graph_lib/draw/sdl/lv_draw_sdl_bg.c \
    ../Core/Libs/Graph_lib/draw/sdl/lv_draw_sdl_composite.c \
    ../Core/Libs/Graph_lib/draw/sdl/lv_draw_sdl_img.c \
    ../Core/Libs/Graph_lib/draw/sdl/lv_draw_sdl_label.c \
    ../Core/Libs/Graph_lib/draw/sdl/lv_draw_sdl_layer.c \
    ../Core/Libs/Graph_lib/draw/sdl/lv_draw_sdl_line.c \
    ../Core/Libs/Graph_lib/draw/sdl/lv_draw_sdl_mask.c \
    ../Core/Libs/Graph_lib/draw/sdl/lv_draw_sdl_polygon.c \
    ../Core/Libs/Graph_lib/draw/sdl/lv_draw_sdl_rect.c \
    ../Core/Libs/Graph_lib/draw/sdl/lv_draw_sdl_stack_blur.c \
    ../Core/Libs/Graph_lib/draw/sdl/lv_draw_sdl_texture_cache.c \
    ../Core/Libs/Graph_lib/draw/sdl/lv_draw_sdl_utils.c \
    ../Core/Libs/Graph_lib/draw/stm32_dma2d/lv_gpu_stm32_dma2d.c \
    ../Core/Libs/Graph_lib/draw/sw/lv_draw_sw.c \
    ../Core/Libs/Graph_lib/draw/sw/lv_draw_sw_arc.c \
    ../Core/Libs/Graph_lib/draw/sw/lv_draw_sw_blend.c \
    ../Core/Libs/Graph_lib/draw/sw/lv_draw_sw_dither.c \
    ../Core/Libs/Graph_lib/draw/sw/lv_draw_sw_gradient.c \
    ../Core/Libs/Graph_lib/draw/sw/lv_draw_sw_img.c \
    ../Core/Libs/Graph_lib/draw/sw/lv_draw_sw_layer.c \
    ../Core/Libs/Graph_lib/draw/sw/lv_draw_sw_letter.c \
    ../Core/Libs/Graph_lib/draw/sw/lv_draw_sw_line.c \
    ../Core/Libs/Graph_lib/draw/sw/lv_draw_sw_polygon.c \
    ../Core/Libs/Graph_lib/draw/sw/lv_draw_sw_rect.c \
    ../Core/Libs/Graph_lib/draw/sw/lv_draw_sw_transform.c \
    ../Core/Libs/Graph_lib/draw/swm341_dma2d/lv_gpu_swm341_dma2d.c \
    ../Core/Libs/Graph_lib/draw/lv_draw.c \
    ../Core/Libs/Graph_lib/draw/lv_draw_arc.c \
    ../Core/Libs/Graph_lib/draw/lv_draw_img.c \
    ../Core/Libs/Graph_lib/draw/lv_draw_label.c \
    ../Core/Libs/Graph_lib/draw/lv_draw_layer.c \
    ../Core/Libs/Graph_lib/draw/lv_draw_line.c \
    ../Core/Libs/Graph_lib/draw/lv_draw_mask.c \
    ../Core/Libs/Graph_lib/draw/lv_draw_rect.c \
    ../Core/Libs/Graph_lib/draw/lv_draw_transform.c \
    ../Core/Libs/Graph_lib/draw/lv_draw_triangle.c \
    ../Core/Libs/Graph_lib/draw/lv_img_buf.c \
    ../Core/Libs/Graph_lib/draw/lv_img_cache.c \
    ../Core/Libs/Graph_lib/draw/lv_img_decoder.c \
    ../Core/Libs/Graph_lib/font/lv_font.c \
    ../Core/Libs/Graph_lib/font/lv_font_dejavu_16_persian_hebrew.c \
    ../Core/Libs/Graph_lib/font/lv_font_fmt_txt.c \
    ../Core/Libs/Graph_lib/font/lv_font_loader.c \
    ../Core/Libs/Graph_lib/font/lv_font_montserrat_10.c \
    ../Core/Libs/Graph_lib/font/lv_font_montserrat_12.c \
    ../Core/Libs/Graph_lib/font/lv_font_montserrat_12_subpx.c \
    ../Core/Libs/Graph_lib/font/lv_font_montserrat_14.c \
    ../Core/Libs/Graph_lib/font/lv_font_montserrat_16.c \
    ../Core/Libs/Graph_lib/font/lv_font_montserrat_18.c \
    ../Core/Libs/Graph_lib/font/lv_font_montserrat_20.c \
    ../Core/Libs/Graph_lib/font/lv_font_montserrat_22.c \
    ../Core/Libs/Graph_lib/font/lv_font_montserrat_24.c \
    ../Core/Libs/Graph_lib/font/lv_font_montserrat_26.c \
    ../Core/Libs/Graph_lib/font/lv_font_montserrat_28.c \
    ../Core/Libs/Graph_lib/font/lv_font_montserrat_28_compressed.c \
    ../Core/Libs/Graph_lib/font/lv_font_montserrat_30.c \
    ../Core/Libs/Graph_lib/font/lv_font_montserrat_32.c \
    ../Core/Libs/Graph_lib/font/lv_font_montserrat_34.c \
    ../Core/Libs/Graph_lib/font/lv_font_montserrat_36.c \
    ../Core/Libs/Graph_lib/font/lv_font_montserrat_38.c \
    ../Core/Libs/Graph_lib/font/lv_font_montserrat_40.c \
    ../Core/Libs/Graph_lib/font/lv_font_montserrat_42.c \
    ../Core/Libs/Graph_lib/font/lv_font_montserrat_44.c \
    ../Core/Libs/Graph_lib/font/lv_font_montserrat_46.c \
    ../Core/Libs/Graph_lib/font/lv_font_montserrat_48.c \
    ../Core/Libs/Graph_lib/font/lv_font_montserrat_8.c \
    ../Core/Libs/Graph_lib/font/lv_font_simsun_16_cjk.c \
    ../Core/Libs/Graph_lib/font/lv_font_unscii_16.c \
    ../Core/Libs/Graph_lib/font/lv_font_unscii_8.c \
    ../Core/Libs/Graph_lib/hal/lv_hal_disp.c \
    ../Core/Libs/Graph_lib/hal/lv_hal_indev.c \
    ../Core/Libs/Graph_lib/hal/lv_hal_tick.c \
    ../Core/Libs/Graph_lib/layouts/flex/lv_flex.c \
    ../Core/Libs/Graph_lib/layouts/grid/lv_grid.c \
    ../Core/Libs/Graph_lib/libs/bmp/lv_bmp.c \
    ../Core/Libs/Graph_lib/libs/ffmpeg/lv_ffmpeg.c \
    ../Core/Libs/Graph_lib/libs/freetype/lv_freetype.c \
    ../Core/Libs/Graph_lib/libs/fsdrv/lv_fs_cbfs.c \
    ../Core/Libs/Graph_lib/libs/fsdrv/lv_fs_fatfs.c \
    ../Core/Libs/Graph_lib/libs/fsdrv/lv_fs_posix.c \
    ../Core/Libs/Graph_lib/libs/fsdrv/lv_fs_stdio.c \
    ../Core/Libs/Graph_lib/libs/fsdrv/lv_fs_win32.c \
    ../Core/Libs/Graph_lib/libs/gif/gifdec.c \
    ../Core/Libs/Graph_lib/libs/gif/lv_gif.c \
    ../Core/Libs/Graph_lib/libs/png/lodepng.c \
    ../Core/Libs/Graph_lib/libs/png/lv_png.c \
    ../Core/Libs/Graph_lib/libs/qrcode/lv_qrcode.c \
    ../Core/Libs/Graph_lib/libs/qrcode/qrcodegen.c \
    ../Core/Libs/Graph_lib/libs/rlottie/lv_rlottie.c \
    ../Core/Libs/Graph_lib/libs/sjpg/lv_sjpg.c \
    ../Core/Libs/Graph_lib/libs/sjpg/tjpgd.c \
    ../Core/Libs/Graph_lib/libs/tiny_ttf/lv_tiny_ttf.c \
    ../Core/Libs/Graph_lib/misc/lv_anim.c \
    ../Core/Libs/Graph_lib/misc/lv_anim_timeline.c \
    ../Core/Libs/Graph_lib/misc/lv_area.c \
    ../Core/Libs/Graph_lib/misc/lv_async.c \
    ../Core/Libs/Graph_lib/misc/lv_bidi.c \
    ../Core/Libs/Graph_lib/misc/lv_color.c \
    ../Core/Libs/Graph_lib/misc/lv_fs.c \
    ../Core/Libs/Graph_lib/misc/lv_gc.c \
    ../Core/Libs/Graph_lib/misc/lv_ll.c \
    ../Core/Libs/Graph_lib/misc/lv_log.c \
    ../Core/Libs/Graph_lib/misc/lv_lru.c \
    ../Core/Libs/Graph_lib/misc/lv_malloc_builtin.c \
    ../Core/Libs/Graph_lib/misc/lv_math.c \
    ../Core/Libs/Graph_lib/misc/lv_mem.c \
    ../Core/Libs/Graph_lib/misc/lv_memcpy_builtin.c \
    ../Core/Libs/Graph_lib/misc/lv_printf.c \
    ../Core/Libs/Graph_lib/misc/lv_style.c \
    ../Core/Libs/Graph_lib/misc/lv_style_gen.c \
    ../Core/Libs/Graph_lib/misc/lv_templ.c \
    ../Core/Libs/Graph_lib/misc/lv_timer.c \
    ../Core/Libs/Graph_lib/misc/lv_tlsf.c \
    ../Core/Libs/Graph_lib/misc/lv_txt.c \
    ../Core/Libs/Graph_lib/misc/lv_txt_ap.c \
    ../Core/Libs/Graph_lib/misc/lv_utils.c \
    ../Core/Libs/Graph_lib/others/file_explorer/lv_file_explorer.c \
    ../Core/Libs/Graph_lib/others/fragment/lv_fragment.c \
    ../Core/Libs/Graph_lib/others/fragment/lv_fragment_manager.c \
    ../Core/Libs/Graph_lib/others/gridnav/lv_gridnav.c \
    ../Core/Libs/Graph_lib/others/ime/lv_ime_pinyin.c \
    ../Core/Libs/Graph_lib/others/imgfont/lv_imgfont.c \
    ../Core/Libs/Graph_lib/others/monkey/lv_monkey.c \
    ../Core/Libs/Graph_lib/others/msg/lv_msg.c \
    ../Core/Libs/Graph_lib/others/snapshot/lv_snapshot.c \
    ../Core/Libs/Graph_lib/themes/basic/lv_theme_basic.c \
    ../Core/Libs/Graph_lib/themes/default/lv_theme_default.c \
    ../Core/Libs/Graph_lib/themes/mono/lv_theme_mono.c \
    ../Core/Libs/Graph_lib/widgets/animimg/lv_animimg.c \
    ../Core/Libs/Graph_lib/widgets/arc/lv_arc.c \
    ../Core/Libs/Graph_lib/widgets/bar/lv_bar.c \
    ../Core/Libs/Graph_lib/widgets/btn/lv_btn.c \
    ../Core/Libs/Graph_lib/widgets/btnmatrix/lv_btnmatrix.c \
    ../Core/Libs/Graph_lib/widgets/calendar/lv_calendar.c \
    ../Core/Libs/Graph_lib/widgets/calendar/lv_calendar_header_arrow.c \
    ../Core/Libs/Graph_lib/widgets/calendar/lv_calendar_header_dropdown.c \
    ../Core/Libs/Graph_lib/widgets/canvas/lv_canvas.c \
    ../Core/Libs/Graph_lib/widgets/chart/lv_chart.c \
    ../Core/Libs/Graph_lib/widgets/checkbox/lv_checkbox.c \
    ../Core/Libs/Graph_lib/widgets/colorwheel/lv_colorwheel.c \
    ../Core/Libs/Graph_lib/widgets/dropdown/lv_dropdown.c \
    ../Core/Libs/Graph_lib/widgets/img/lv_img.c \
    ../Core/Libs/Graph_lib/widgets/imgbtn/lv_imgbtn.c \
    ../Core/Libs/Graph_lib/widgets/keyboard/lv_keyboard.c \
    ../Core/Libs/Graph_lib/widgets/label/lv_label.c \
    ../Core/Libs/Graph_lib/widgets/led/lv_led.c \
    ../Core/Libs/Graph_lib/widgets/line/lv_line.c \
    ../Core/Libs/Graph_lib/widgets/list/lv_list.c \
    ../Core/Libs/Graph_lib/widgets/menu/lv_menu.c \
    ../Core/Libs/Graph_lib/widgets/meter/lv_meter.c \
    ../Core/Libs/Graph_lib/widgets/msgbox/lv_msgbox.c \
    ../Core/Libs/Graph_lib/widgets/objx_templ/lv_objx_templ.c \
    ../Core/Libs/Graph_lib/widgets/roller/lv_roller.c \
    ../Core/Libs/Graph_lib/widgets/slider/lv_slider.c \
    ../Core/Libs/Graph_lib/widgets/span/lv_span.c \
    ../Core/Libs/Graph_lib/widgets/spinbox/lv_spinbox.c \
    ../Core/Libs/Graph_lib/widgets/spinner/lv_spinner.c \
    ../Core/Libs/Graph_lib/widgets/switch/lv_switch.c \
    ../Core/Libs/Graph_lib/widgets/table/lv_table.c \
    ../Core/Libs/Graph_lib/widgets/tabview/lv_tabview.c \
    ../Core/Libs/Graph_lib/widgets/textarea/lv_textarea.c \
    ../Core/Libs/Graph_lib/widgets/tileview/lv_tileview.c \
    ../Core/Libs/Graph_lib/widgets/win/lv_win.c


HEADERS += \
        mainwindow.h \
    display.h \
    hw_sim/hal_stm32.h \
    ../Core/App/Gui/app_gui.h \
    ../Core/App/Gui/splash/splash.h \
    ../Core/App/Gui/home/home.h \
    ../Core/Libs/Graph_lib/core/lv_disp.h \
    ../Core/Libs/Graph_lib/core/lv_event.h \
    ../Core/Libs/Graph_lib/core/lv_group.h \
    ../Core/Libs/Graph_lib/core/lv_indev.h \
    ../Core/Libs/Graph_lib/core/lv_indev_scroll.h \
    ../Core/Libs/Graph_lib/core/lv_obj.h \
    ../Core/Libs/Graph_lib/core/lv_obj_class.h \
    ../Core/Libs/Graph_lib/core/lv_obj_draw.h \
    ../Core/Libs/Graph_lib/core/lv_obj_pos.h \
    ../Core/Libs/Graph_lib/core/lv_obj_scroll.h \
    ../Core/Libs/Graph_lib/core/lv_obj_style.h \
    ../Core/Libs/Graph_lib/core/lv_obj_style_gen.h \
    ../Core/Libs/Graph_lib/core/lv_obj_tree.h \
    ../Core/Libs/Graph_lib/core/lv_refr.h \
    ../Core/Libs/Graph_lib/core/lv_theme.h \
    ../Core/Libs/Graph_lib/draw/arm2d/lv_gpu_arm2d.h \
    ../Core/Libs/Graph_lib/draw/gd32_ipa/lv_gpu_gd32_ipa.h \
    ../Core/Libs/Graph_lib/draw/nxp/pxp/lv_draw_pxp_blend.h \
    ../Core/Libs/Graph_lib/draw/nxp/pxp/lv_gpu_nxp_pxp.h \
    ../Core/Libs/Graph_lib/draw/nxp/pxp/lv_gpu_nxp_pxp_osa.h \
    ../Core/Libs/Graph_lib/draw/nxp/vglite/lv_draw_vglite_arc.h \
    ../Core/Libs/Graph_lib/draw/nxp/vglite/lv_draw_vglite_blend.h \
    ../Core/Libs/Graph_lib/draw/nxp/vglite/lv_draw_vglite_rect.h \
    ../Core/Libs/Graph_lib/draw/nxp/vglite/lv_gpu_nxp_vglite.h \
    ../Core/Libs/Graph_lib/draw/nxp/lv_gpu_nxp.h \
    ../Core/Libs/Graph_lib/draw/sdl/lv_draw_sdl.h \
    ../Core/Libs/Graph_lib/draw/sdl/lv_draw_sdl_composite.h \
    ../Core/Libs/Graph_lib/draw/sdl/lv_draw_sdl_img.h \
    ../Core/Libs/Graph_lib/draw/sdl/lv_draw_sdl_layer.h \
    ../Core/Libs/Graph_lib/draw/sdl/lv_draw_sdl_mask.h \
    ../Core/Libs/Graph_lib/draw/sdl/lv_draw_sdl_priv.h \
    ../Core/Libs/Graph_lib/draw/sdl/lv_draw_sdl_rect.h \
    ../Core/Libs/Graph_lib/draw/sdl/lv_draw_sdl_stack_blur.h \
    ../Core/Libs/Graph_lib/draw/sdl/lv_draw_sdl_texture_cache.h \
    ../Core/Libs/Graph_lib/draw/sdl/lv_draw_sdl_utils.h \
    ../Core/Libs/Graph_lib/draw/stm32_dma2d/lv_gpu_stm32_dma2d.h \
    ../Core/Libs/Graph_lib/draw/sw/lv_draw_sw.h \
    ../Core/Libs/Graph_lib/draw/sw/lv_draw_sw_blend.h \
    ../Core/Libs/Graph_lib/draw/sw/lv_draw_sw_dither.h \
    ../Core/Libs/Graph_lib/draw/sw/lv_draw_sw_gradient.h \
    ../Core/Libs/Graph_lib/draw/swm341_dma2d/lv_gpu_swm341_dma2d.h \
    ../Core/Libs/Graph_lib/draw/lv_draw.h \
    ../Core/Libs/Graph_lib/draw/lv_draw_arc.h \
    ../Core/Libs/Graph_lib/draw/lv_draw_img.h \
    ../Core/Libs/Graph_lib/draw/lv_draw_label.h \
    ../Core/Libs/Graph_lib/draw/lv_draw_layer.h \
    ../Core/Libs/Graph_lib/draw/lv_draw_line.h \
    ../Core/Libs/Graph_lib/draw/lv_draw_mask.h \
    ../Core/Libs/Graph_lib/draw/lv_draw_rect.h \
    ../Core/Libs/Graph_lib/draw/lv_draw_transform.h \
    ../Core/Libs/Graph_lib/draw/lv_draw_triangle.h \
    ../Core/Libs/Graph_lib/draw/lv_img_buf.h \
    ../Core/Libs/Graph_lib/draw/lv_img_cache.h \
    ../Core/Libs/Graph_lib/draw/lv_img_decoder.h \
    ../Core/Libs/Graph_lib/font/lv_font.h \
    ../Core/Libs/Graph_lib/font/lv_font_fmt_txt.h \
    ../Core/Libs/Graph_lib/font/lv_font_loader.h \
    ../Core/Libs/Graph_lib/font/lv_symbol_def.h \
    ../Core/Libs/Graph_lib/hal/lv_hal.h \
    ../Core/Libs/Graph_lib/hal/lv_hal_disp.h \
    ../Core/Libs/Graph_lib/hal/lv_hal_indev.h \
    ../Core/Libs/Graph_lib/hal/lv_hal_tick.h \
    ../Core/Libs/Graph_lib/layouts/flex/lv_flex.h \
    ../Core/Libs/Graph_lib/layouts/grid/lv_grid.h \
    ../Core/Libs/Graph_lib/libs/bmp/lv_bmp.h \
    ../Core/Libs/Graph_lib/libs/ffmpeg/lv_ffmpeg.h \
    ../Core/Libs/Graph_lib/libs/freetype/lv_freetype.h \
    ../Core/Libs/Graph_lib/libs/fsdrv/lv_fsdrv.h \
    ../Core/Libs/Graph_lib/libs/gif/gifdec.h \
    ../Core/Libs/Graph_lib/libs/gif/lv_gif.h \
    ../Core/Libs/Graph_lib/libs/png/lodepng.h \
    ../Core/Libs/Graph_lib/libs/png/lv_png.h \
    ../Core/Libs/Graph_lib/libs/qrcode/lv_qrcode.h \
    ../Core/Libs/Graph_lib/libs/qrcode/qrcodegen.h \
    ../Core/Libs/Graph_lib/libs/rlottie/lv_rlottie.h \
    ../Core/Libs/Graph_lib/libs/sjpg/lv_sjpg.h \
    ../Core/Libs/Graph_lib/libs/sjpg/tjpgd.h \
    ../Core/Libs/Graph_lib/libs/sjpg/tjpgdcnf.h \
    ../Core/Libs/Graph_lib/libs/tiny_ttf/lv_tiny_ttf.h \
    ../Core/Libs/Graph_lib/libs/tiny_ttf/stb_rect_pack.h \
    ../Core/Libs/Graph_lib/libs/tiny_ttf/stb_truetype_htcw.h \
    ../Core/Libs/Graph_lib/misc/lv_anim.h \
    ../Core/Libs/Graph_lib/misc/lv_anim_timeline.h \
    ../Core/Libs/Graph_lib/misc/lv_area.h \
    ../Core/Libs/Graph_lib/misc/lv_assert.h \
    ../Core/Libs/Graph_lib/misc/lv_async.h \
    ../Core/Libs/Graph_lib/misc/lv_bidi.h \
    ../Core/Libs/Graph_lib/misc/lv_color.h \
    ../Core/Libs/Graph_lib/misc/lv_fs.h \
    ../Core/Libs/Graph_lib/misc/lv_gc.h \
    ../Core/Libs/Graph_lib/misc/lv_ll.h \
    ../Core/Libs/Graph_lib/misc/lv_log.h \
    ../Core/Libs/Graph_lib/misc/lv_lru.h \
    ../Core/Libs/Graph_lib/misc/lv_malloc_builtin.h \
    ../Core/Libs/Graph_lib/misc/lv_math.h \
    ../Core/Libs/Graph_lib/misc/lv_mem.h \
    ../Core/Libs/Graph_lib/misc/lv_memcpy_builtin.h \
    ../Core/Libs/Graph_lib/misc/lv_printf.h \
    ../Core/Libs/Graph_lib/misc/lv_style.h \
    ../Core/Libs/Graph_lib/misc/lv_style_gen.h \
    ../Core/Libs/Graph_lib/misc/lv_templ.h \
    ../Core/Libs/Graph_lib/misc/lv_timer.h \
    ../Core/Libs/Graph_lib/misc/lv_tlsf.h \
    ../Core/Libs/Graph_lib/misc/lv_txt.h \
    ../Core/Libs/Graph_lib/misc/lv_txt_ap.h \
    ../Core/Libs/Graph_lib/misc/lv_types.h \
    ../Core/Libs/Graph_lib/misc/lv_utils.h \
    ../Core/Libs/Graph_lib/others/file_explorer/lv_file_explorer.h \
    ../Core/Libs/Graph_lib/others/fragment/lv_fragment.h \
    ../Core/Libs/Graph_lib/others/gridnav/lv_gridnav.h \
    ../Core/Libs/Graph_lib/others/ime/lv_ime_pinyin.h \
    ../Core/Libs/Graph_lib/others/imgfont/lv_imgfont.h \
    ../Core/Libs/Graph_lib/others/monkey/lv_monkey.h \
    ../Core/Libs/Graph_lib/others/msg/lv_msg.h \
    ../Core/Libs/Graph_lib/others/snapshot/lv_snapshot.h \
    ../Core/Libs/Graph_lib/themes/basic/lv_theme_basic.h \
    ../Core/Libs/Graph_lib/themes/default/lv_theme_default.h \
    ../Core/Libs/Graph_lib/themes/mono/lv_theme_mono.h \
    ../Core/Libs/Graph_lib/themes/lv_themes.h \
    ../Core/Libs/Graph_lib/widgets/animimg/lv_animimg.h \
    ../Core/Libs/Graph_lib/widgets/arc/lv_arc.h \
    ../Core/Libs/Graph_lib/widgets/bar/lv_bar.h \
    ../Core/Libs/Graph_lib/widgets/btn/lv_btn.h \
    ../Core/Libs/Graph_lib/widgets/btnmatrix/lv_btnmatrix.h \
    ../Core/Libs/Graph_lib/widgets/calendar/lv_calendar.h \
    ../Core/Libs/Graph_lib/widgets/calendar/lv_calendar_header_arrow.h \
    ../Core/Libs/Graph_lib/widgets/calendar/lv_calendar_header_dropdown.h \
    ../Core/Libs/Graph_lib/widgets/canvas/lv_canvas.h \
    ../Core/Libs/Graph_lib/widgets/chart/lv_chart.h \
    ../Core/Libs/Graph_lib/widgets/checkbox/lv_checkbox.h \
    ../Core/Libs/Graph_lib/widgets/colorwheel/lv_colorwheel.h \
    ../Core/Libs/Graph_lib/widgets/dropdown/lv_dropdown.h \
    ../Core/Libs/Graph_lib/widgets/img/lv_img.h \
    ../Core/Libs/Graph_lib/widgets/imgbtn/lv_imgbtn.h \
    ../Core/Libs/Graph_lib/widgets/keyboard/lv_keyboard.h \
    ../Core/Libs/Graph_lib/widgets/label/lv_label.h \
    ../Core/Libs/Graph_lib/widgets/led/lv_led.h \
    ../Core/Libs/Graph_lib/widgets/line/lv_line.h \
    ../Core/Libs/Graph_lib/widgets/list/lv_list.h \
    ../Core/Libs/Graph_lib/widgets/menu/lv_menu.h \
    ../Core/Libs/Graph_lib/widgets/meter/lv_meter.h \
    ../Core/Libs/Graph_lib/widgets/msgbox/lv_msgbox.h \
    ../Core/Libs/Graph_lib/widgets/objx_templ/lv_objx_templ.h \
    ../Core/Libs/Graph_lib/widgets/roller/lv_roller.h \
    ../Core/Libs/Graph_lib/widgets/slider/lv_slider.h \
    ../Core/Libs/Graph_lib/widgets/span/lv_span.h \
    ../Core/Libs/Graph_lib/widgets/spinbox/lv_spinbox.h \
    ../Core/Libs/Graph_lib/widgets/spinner/lv_spinner.h \
    ../Core/Libs/Graph_lib/widgets/switch/lv_switch.h \
    ../Core/Libs/Graph_lib/widgets/table/lv_table.h \
    ../Core/Libs/Graph_lib/widgets/tabview/lv_tabview.h \
    ../Core/Libs/Graph_lib/widgets/textarea/lv_textarea.h \
    ../Core/Libs/Graph_lib/widgets/tileview/lv_tileview.h \
    ../Core/Libs/Graph_lib/widgets/win/lv_win.h \
    ../Core/Libs/Graph_lib/lv_api_map.h \
    ../Core/Libs/Graph_lib/lv_conf.h \
    ../Core/Libs/Graph_lib/lv_conf_internal.h \
    ../Core/Libs/Graph_lib/lv_conf_kconfig.h \
    ../Core/Libs/Graph_lib/lvgl.h


FORMS += \
        mainwindow.ui \
    display.ui

DISTFILES +=
