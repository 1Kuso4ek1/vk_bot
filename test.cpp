#include "vk_bot.h"
int main()
{
  vk_bot bot("token");
  for(;;)
  {
    bot.Update();
    if(bot.GetLastMessage() == u8"ping")
    {
      bot.Send(u8"Pong!", 2000000001);
    }
  }
}
