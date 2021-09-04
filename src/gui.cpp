#include "gui.h"

TFT_eSPI tft = TFT_eSPI();

static const uint16_t screenWidth = TFT_HEIGHT;
static const uint16_t screenHeight = TFT_WIDTH;

LV_IMG_DECLARE(back_ground);

static lv_disp_draw_buf_t draw_buf;
lv_color_t buf[TFT_HEIGHT * 10];

lv_obj_t *screenMain;
lv_obj_t *btn2;
lv_obj_t *btn3;
lv_obj_t *dd;
lv_obj_t *slider;
lv_obj_t *slider_label;

bool led_one;
long interval = 1000;
int value = 128;

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t *)&color_p->full, w * h, true);
  tft.endWrite();

  lv_disp_flush_ready(disp);
}

void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
  uint16_t touchX, touchY;

  bool touched = tft.getTouch(&touchX, &touchY, 600);

  if (!touched)
  {
    data->state = LV_INDEV_STATE_REL;
  }
  else
  {
    data->state = LV_INDEV_STATE_PR;

    data->point.x = touchX;
    data->point.y = touchY;
  }
}

void screenInit()
{
  screenMain = lv_obj_create(NULL);
  lv_scr_load(screenMain);
}

void tftInit()
{
  tft.begin();
  tft.setRotation(3);
  uint16_t calData[5] = {262, 3640, 327, 3428, 5};
  ;
  tft.setTouch(calData);
  lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * 10);
}

void initDisplay()
{
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);
}

void initDriver()
{
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);
}

void guiInit()
{
  show_image();
  createButton(screenMain, btn2, 2, 2, 64, 44, "LED", true); // (where to create, which objekt, x pos , y pos, width, height, text, toggle)
  createButton(screenMain, btn3, 2, 120, 40, 40, LV_SYMBOL_LEFT, false);
  create_dropdown_1(screenMain, 80, 3, 96);
  create_slider_1(screenMain, 80, 60, 255);
}

void my_event_cb(lv_event_t *event)
{
  lv_event_code_t code = lv_event_get_code(event);

  if (code == LV_EVENT_CLICKED)
  {
    LV_LOG_USER("Clicked");
  }
  else if (code == LV_EVENT_VALUE_CHANGED)
  {
    LV_LOG_USER("Toggled");
    led_one = !led_one;
  }
}

void createButton(lv_obj_t *parent, lv_obj_t *button, int x, int y, int w, int h, const char *text, bool flag)
{

  static lv_style_t style_btn_green_def;
  lv_style_init(&style_btn_green_def);
  lv_style_set_bg_opa(&style_btn_green_def, LV_OPA_COVER);

  lv_style_set_bg_color(&style_btn_green_def, lv_palette_main(LV_PALETTE_GREY));
  lv_style_set_bg_grad_color(&style_btn_green_def, lv_palette_lighten(LV_PALETTE_GREY, 10));
  lv_style_set_bg_grad_dir(&style_btn_green_def, LV_GRAD_DIR_VER);

  lv_style_set_outline_width(&style_btn_green_def, 2);
  lv_style_set_outline_color(&style_btn_green_def, lv_color_hex(0xffffff));
  lv_style_set_outline_pad(&style_btn_green_def, 0);

  static lv_style_t style_btn_green_pr;
  lv_style_init(&style_btn_green_pr);
  lv_style_set_bg_color(&style_btn_green_pr, lv_color_hex(0x00ff00));
  lv_style_set_bg_opa(&style_btn_green_pr, LV_OPA_COVER);
  lv_style_set_bg_grad_color(&style_btn_green_pr, lv_color_hex(0x000000));
  lv_style_set_bg_grad_dir(&style_btn_green_pr, 0);
  lv_style_set_text_color(&style_btn_green_pr, lv_color_hex(0x000000));

  button = lv_btn_create(parent);
  lv_obj_add_event_cb(button, my_event_cb, LV_EVENT_ALL, NULL);
  if (flag)
    lv_obj_add_flag(button, LV_OBJ_FLAG_CHECKABLE);
  lv_obj_set_width(button, w);
  lv_obj_set_height(button, h);
  lv_obj_set_pos(button, x, y);
  lv_obj_add_style(button, &style_btn_green_def, 0);
  lv_obj_add_style(button, &style_btn_green_pr, 1);

  lv_obj_t *labelbtn2 = lv_label_create(button);
  lv_label_set_text(labelbtn2, text);
  lv_obj_center(labelbtn2);
}

void event_handler_dd(lv_event_t *event)
{
  lv_event_code_t code = lv_event_get_code(event);
  lv_obj_t *obj = lv_event_get_target(event);
  if (code == LV_EVENT_VALUE_CHANGED)
  {
    if (lv_dropdown_get_selected(obj) == 0)
    {
      interval = 500;
    }
    else if (lv_dropdown_get_selected(obj) == 1)
    {
      interval = 50;
    }
    else if (lv_dropdown_get_selected(obj) == 2)
    {
      interval = 5;
    }
    else if (lv_dropdown_get_selected(obj) == 3)
    {
      interval = 1;
    }
  }
}

void create_dropdown_1(lv_obj_t *parent, int x, int y, int w)
{
  static lv_style_t style_dd_def;
  lv_style_init(&style_dd_def);
  lv_style_set_outline_width(&style_dd_def, 2);
  lv_style_set_outline_color(&style_dd_def, lv_color_hex(0xffffff));
  lv_style_set_outline_pad(&style_dd_def, 1);

  dd = lv_dropdown_create(parent);
  lv_dropdown_set_options(dd, "1 Hz\n"
                              "10 Hz\n"
                              "100 Hz\n"
                              "on");
  lv_obj_set_pos(dd, x, y);
  lv_obj_set_width(dd, w);
  lv_obj_add_style(dd, &style_dd_def, 0);

  lv_obj_add_event_cb(dd, event_handler_dd, LV_EVENT_ALL, NULL);
}

void slider_event_cb(lv_event_t *event)
{
  lv_event_code_t code = lv_event_get_code(event);
  if (code == LV_EVENT_VALUE_CHANGED)
  {
    static char buf2[5];
    int value_perc;
    value = lv_slider_get_value(slider);
    value_perc = map(value, 1, 255, 1, 100);
    snprintf(buf2, 5, "%u%%", value_perc);
    lv_label_set_text(slider_label, buf2);
  }
}

void create_slider_1(lv_obj_t *parent, int x, int y, int w)
{
  static lv_style_t style_sl_def;
  lv_style_init(&style_sl_def);
  lv_style_set_outline_width(&style_sl_def, 2);
  lv_style_set_outline_color(&style_sl_def, lv_color_hex(0xffffff));
  lv_style_set_outline_pad(&style_sl_def, 0);

  slider = lv_slider_create(parent);
  lv_obj_set_width(slider, w);
  lv_obj_set_pos(slider, x, y);
  lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_ALL, NULL);
  lv_slider_set_range(slider, 1, 255);
  lv_slider_set_value(slider, value, LV_ANIM_OFF);
  lv_obj_add_style(slider, &style_sl_def, 0);
  lv_obj_add_style(slider, &style_sl_def, LV_PART_KNOB);

  slider_label = lv_label_create(parent);
  lv_label_set_text(slider_label, "50%");
  lv_obj_set_pos(slider_label, x + w + 20, y);
}

void show_image()
{
  lv_obj_t *img_src = lv_img_create(screenMain);
  lv_img_set_src(img_src, &back_ground);
  lv_obj_set_pos(img_src, 0, 0);
}