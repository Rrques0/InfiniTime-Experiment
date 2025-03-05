#include "WatchFaceDarkrai.h"
#include <cstdio>
#include <chrono>

// Declare your Darkrai image array from your darkrai.c file.
extern const uint8_t darkrai_map[];

// Create an image descriptor for Darkrai.
static lv_img_dsc_t darkrai_img_dsc;

using namespace Pinetime::Applications::Screens;

WatchFaceDarkrai::WatchFaceDarkrai(Controllers::DateTime& dt,
                                   Controllers::MotionController& motionCtrl)
  : dateTimeController(dt),
    motionController(motionCtrl),
    backgroundImg(nullptr),
    stepLabel(nullptr),
    timeLabel(nullptr),
    taskRefresh(nullptr)
{
  // Clear the screen.
  lv_obj_clean(lv_scr_act());

  // Initialize the Darkrai image descriptor.
  // (Assume your original converted image is 120×120 pixels.)
  darkrai_img_dsc.header.always_zero = 0;
  darkrai_img_dsc.header.w = 120;       // original width
  darkrai_img_dsc.header.h = 120;       // original height
  darkrai_img_dsc.data_size = 120 * 120 * 2; // 2 bytes per pixel for RGB565
  darkrai_img_dsc.header.cf = LV_IMG_CF_TRUE_COLOR;
  darkrai_img_dsc.data = darkrai_map;

  // Create an LVGL image object for the background.
  backgroundImg = lv_img_create(lv_scr_act(), nullptr);
  lv_img_set_src(backgroundImg, &darkrai_img_dsc);
  // Zoom by 2× so 120×120 becomes 240×240.
  lv_img_set_zoom(backgroundImg, 512);
  // Center the image on the screen.
  lv_obj_align(backgroundImg, lv_scr_act(), LV_ALIGN_CENTER, 0, 0);

  // Create the step counter label in the top left.
  stepLabel = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_text(stepLabel, "Steps: 0");
  lv_obj_align(stepLabel, lv_scr_act(), LV_ALIGN_IN_TOP_LEFT, 10, 10);

  // Create the time label in the bottom center.
  timeLabel = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_text(timeLabel, "00:00");
  lv_obj_align(timeLabel, lv_scr_act(), LV_ALIGN_IN_BOTTOM_MID, 0, -10);

  // Create a task to refresh the watch face (update time and step count).
  taskRefresh = lv_task_create([](lv_task_t* task) {
    auto wf = static_cast<WatchFaceDarkrai*>(task->user_data);
    wf->Refresh();
  }, LV_DISP_DEF_REFR_PERIOD, LV_TASK_PRIO_MID, this);

  // Initial refresh.
  Refresh();
}

WatchFaceDarkrai::~WatchFaceDarkrai() {
  if(taskRefresh) {
    lv_task_del(taskRefresh);
    taskRefresh = nullptr;
  }
  lv_obj_clean(lv_scr_act());
}

void WatchFaceDarkrai::Refresh() {
  // Update time.
  uint8_t hour = dateTimeController.Hours();
  uint8_t minute = dateTimeController.Minutes();
  char timeText[16];
  snprintf(timeText, sizeof(timeText), "%02d:%02d", hour, minute);
  lv_label_set_text(timeLabel, timeText);
  lv_obj_align(timeLabel, lv_scr_act(), LV_ALIGN_IN_BOTTOM_MID, 0, -10);

  // Update step count.
  auto steps = motionController.NbSteps();
  if (steps.IsUpdated()) {
    char stepText[32];
    snprintf(stepText, sizeof(stepText), "Steps: %lu", steps.Get());
    lv_label_set_text(stepLabel, stepText);
    lv_obj_align(stepLabel, lv_scr_act(), LV_ALIGN_IN_TOP_LEFT, 10, 10);
  }
}
