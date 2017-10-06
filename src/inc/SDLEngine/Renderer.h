#ifndef _SDL_RENDERER_H
#define _SDL_RENDERER_H

#include <SDL.h>
#include "Window.h"
// #include "Texture.h"

// void SDL_DestroyRendererCustom(SDL_Renderer* rend);
namespace ExpUtils{
namespace SDL{
  class Renderer{
  //friend class Texture;
  public:
    Renderer();
    Renderer(const Window& window, int index, Uint32 flags);
      //SDL_CreateRenderer(SDL_Window *window, int index, Uint32 flags)
    Renderer(Renderer&& rend);
    Renderer(SDL_Renderer* rend);

    std::shared_ptr<SDL_Renderer> get();

    void present();
    void clear();
    void fill(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    //void copy(Texture texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect);
    //SDL_RenderCopy(SDL_Renderer *renderer, SDL_Texture *texture, const SDL_Rect *srcrect, const SDL_Rect *dstrect)

    void operator=(Renderer&& rhs);

    SDL_Renderer* getRaw();
  protected:
  private:


    std::shared_ptr<SDL_Renderer> renderer;
  };
} // namespace SDL
} // namespace ExpUtils
#endif
