#ifndef WATCHFACE_DARKRAI_H
#define WATCHFACE_DARKRAI_H

#include <lvgl/lvgl.h>
#include "displayapp/screens/Screen.h"
#include "components/datetime/DateTimeController.h"
#include "components/motion/MotionController.h"

namespace Pinetime {
  namespace Applications {
    namespace Screens {

      class WatchFaceDarkrai : public Screen {
      public:
        WatchFaceDarkrai(Controllers::DateTime& dateTimeController,
                         Controllers::MotionController& motionController);
        ~WatchFaceDarkrai() override;
        void Refresh() override;

      private:
        Controllers::DateTime& dateTimeController;
        Controllers::MotionController& motionController;
        lv_obj_t* backgroundImg;
        lv_obj_t* stepLabel;
        lv_obj_t* timeLabel;
        lv_task_t* taskRefresh;
      };

    } // namespace Screens
  } // namespace Applications
} // namespace Pinetime

#endif // WATCHFACE_DARKRAI_H
