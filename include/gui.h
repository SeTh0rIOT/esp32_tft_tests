#include <lvgl.h>
#include <TFT_eSPI.h>

extern TFT_eSPI tft;
extern lv_color_t buf[TFT_HEIGHT * 10];
extern lv_obj_t *screenMain;
extern lv_obj_t *btn2;
extern lv_obj_t *btn3;
extern lv_obj_t *slider_label;
extern lv_obj_t *slider;
extern lv_obj_t *tabview;
extern lv_obj_t *homeTab;
extern lv_obj_t *profileTab;
extern lv_obj_t *chartTab;
extern lv_obj_t *miscTab;
extern long interval;
extern bool led_one;
extern int value;

void tftInit();
void initDisplay();
void initDriver();
void screenInit();
void createButton(lv_obj_t *parent, lv_obj_t *button, int x, int y, int w, int h, const char *text, bool flag);
void guiInit();
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);
void my_event_cb();
void create_dropdown_1(lv_obj_t *parent,int x,int y,int w);
void event_handler_dd(lv_event_t * event);
void create_slider_1(lv_obj_t *parent,int x,int y,int w);
void slider_event_cb(lv_event_t *event);
void show_image();
void lv_example_tabview_2(lv_obj_t *parent);