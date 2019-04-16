#pragma once

#include <config/Config.h>
#include <touch/TouchTypes.hpp>
#include <util/optional.hpp>

#include <chrono>

namespace gfx
{
  template<typename ScreenDriver>
  struct ScreenSaver
  {
    public:
      ScreenSaver(ScreenDriver* driver) :
        mDriver(driver)
      {
        mLastTouch = std::chrono::system_clock::now();
        pinMode(TFT_LED, OUTPUT);
        digitalWrite(TFT_LED, LOW);    // LOW to turn backlight on - pcb version 01-02-00
      }

      void operator()()
      {
        auto now = std::chrono::system_clock::now();
        if (std::chrono::duration_cast<std::chrono::minutes>(now - mLastTouch).count() > MinsBeforeScreenSleep)
        {
          switchScreen(false);
        }
        else
        {
          switchScreen(true);
        }
      }

      template<typename T>
      bool tapped(const T& tapEvt)
      {
        if (!tapEvt)
        {
          return mCurrentState;
        }
        auto tapEvent = *tapEvt;
        if (tapEvent.state == decltype(tapEvent.state)::Tap)
        {
          mLastTouch = std::chrono::system_clock::now();
        }
        return mCurrentState;
      }

      void activate()
      {
        mLastTouch = std::chrono::system_clock::now();
      }

    private:

      void switchScreen(bool on)
      {
        if (on == mCurrentState)
        {
          return;
        }
        ScreenOnOffSwitch(mDriver, on);
        mCurrentState = on;
      }

      std::chrono::system_clock::time_point mLastTouch;
      bool mCurrentState = true;
      ScreenDriver* mDriver;

  };
} // namespace gfx