#include "vk_bot.h"
int main()
{
  vk_bot bot("token");
  for(;;)
  {
    bot.Update();
    if(bot.GetLastMessage() == "ping")
    {
      bot.Send("Pong!", 2000000001);
    }
  }
}
