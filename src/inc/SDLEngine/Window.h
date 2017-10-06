#pragma once

#include "Memory.h"
#include <SDL.h>
//SDL_CreateWindow(const char *title, int x, int y, int w, int h, Uint32 flags)
namespace ExpUtils{
namespace SDL{
  class Window{
    friend class Renderer;
  public:
    Window();
    ~Window();
    Window(SDL_Window* wind);
    Window(Window&& wind);
    Window(const char *title, int x, int y, int w, int h, Uint32 flags);

    void operator=(Window&& rhs);

    std::shared_ptr<SDL_Window> get();

    SDL_Renderer* getRenderer();
    SDL_Surface* getWindowSurface();

  protected:
    //SDL_Window* window;
    std::shared_ptr<SDL_Window> window;
  private:
    Window(const Window&) = default;


  };
}
}
