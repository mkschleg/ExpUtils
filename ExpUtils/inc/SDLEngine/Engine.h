#pragma once

#include "SDLEngine/Window.h"
#include "SDLEngine/Renderer.h"

#include <SDL.h>
#include <SDL_image.h>

namespace ExpUtils{
namespace SDL{

  class Engine{
    
  public:
    void onInit();
    // void onEvent(const SDL::Event& event);
    void onLoop();
    void onRender();
    void onCleanup();

    Uint64 currentTime;
    Uint64 lastTime;
    int frames;
    
    static Engine& getInstance();
    static int getHeight();
    static int getWidth();
    static const int SCREEN_WIDTH = 640;
    static const int SCREEN_HEIGHT = 480;
  private:
    //Tell whether or not the engine is running
    bool running;
    //SDL_Window reference
    // SDL_Window *window;
    Window window;
    //SDL_Surface for the window
    SDL_Surface *screenSurface;
    //SDL_Renderer
    // SDL_Renderer *renderer;
    std::shared_ptr<SDL::Renderer> renderer;

    /**************
    Custom Processing/Variables
    **************/
    SDL_Surface *imageSurface;
    std::shared_ptr<Texture> texture;
    std::shared_ptr<Texture> t2;
    
    //Function to load media on creation of the engine.
    void loadMedia();
    //Function to load a surface.
    // SDL_Surface *loadSurface(const std::string&);
    // SDL_Texture *loadTexture(const std::string&);
    
    void onQuit() override;
    void onKeyDown(SDL_Keycode kcode, SDL_Scancode scode, uint16_t mod) override;
    void onWindowClose() override;

  };

}
}
