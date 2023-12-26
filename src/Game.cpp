#include "Game.h"

Entity *builder = new Entity();

TTF_Font *Sans;
SDL_Color White = {255, 255, 255, 255};

int debugMessageCount = 0;

Game::~Game() {
}
Game::Game() {
}

std::string toString(int number) {
    std::stringstream strs;
    strs << number;
    return strs.str();
}

std::string toStringf(float number) {
    std::stringstream strs;
    strs << number;
    return strs.str();
}
void Game::init(const char *title, int x, int y, int width, int height, int flags) {
    isRunning = false;

    if(SDL_Init(SDL_INIT_EVERYTHING) < SDL_FALSE) {
        std::cout << "SDL ERROR" << std::endl;
        std::cout << SDL_GetError() << std::endl;
    }

    if((window = SDL_CreateWindow(title, x, y, width, height, flags)) == nullptr) {
        std::cout << "WINDOW ERROR" << std::endl;
        std::cout << SDL_GetError() << std::endl;
    }

    int rendererFlags = 0;
    // rendererFlags = SDL_RENDERER_PRESENTVSYNC + SDL_RENDERER_ACCELERATED;


    if((renderer = SDL_CreateRenderer(window, -1, rendererFlags)) == nullptr) {
        std::cout << "RENDERER ERROR" << std::endl;
        std::cout << SDL_GetError() << std::endl;
    }

    if(TTF_Init() < SDL_FALSE) {
        std::cout << "TTF ERROR" << std::endl;
        std::cout << SDL_GetError() << std::endl;
    }

    Sans = TTF_OpenFont("coure.fon", 20);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    builder->position.x = 400;
    builder->position.y = 300;

    SDL_Surface *tmpSurface = IMG_Load("assets/builder.png");

    SDL_Texture *tmpTexture = SDL_CreateTextureFromSurface(renderer, tmpSurface);

    SDL_QueryTexture(tmpTexture, 0, 0, &builder->position.w, &builder->position.h);

    builder->texture = tmpTexture;

    SDL_FreeSurface(tmpSurface);

    camera.x = 0;
    camera.y = 0;
    isRunning = true;
}

void Game::handleEvents() {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;

        case SDL_KEYDOWN:
        {
            switch(event.key.keysym.sym) {
            case SDLK_ESCAPE:
                isRunning = false;
                break;
            case SDLK_w:
                camera.y -= 100 / zoom;
                break;
            case SDLK_a:
                camera.x -= 100 / zoom;
                break;
            case SDLK_s:
                camera.y += 100 / zoom;
                break;
            case SDLK_d:
                camera.x += 100 / zoom;
                break;
            case SDLK_r:
                camera.x = 0;
                camera.y = 0;
                zoom = 1.0f;
                break;
            }
            break;
        }

        case SDL_MOUSEWHEEL:
        {
            zoom += event.wheel.y * 0.2f;
            if(zoom < 0.1) {
                zoom = 0.1;
            }
            else if(zoom > 10.0) {
                zoom = 10.0;
            }
            break;
        }

        case SDL_MOUSEBUTTONDOWN:
        {
            if(LEFT_MOUSE_BUTTON_ID == event.button.button) {
                clicked.x = event.button.x;
                clicked.y = event.button.y;
            };
            if(RIGHT_MOUSE_BUTTON_ID == event.button.button) {
                scroll = 1;
                scrollx = event.button.x;
                scrolly = event.button.y;
            }
            break;
        }

        case SDL_MOUSEBUTTONUP:
        {
            if(RIGHT_MOUSE_BUTTON_ID == event.button.button) {
                scroll = 0;
                scrollx = 0;
                scrolly = 0;
            }
            break;
        }

        case SDL_MOUSEMOTION:
        {
            if(scroll) {
                camera.x += (scrollx - event.button.x); //* zoom;		
                camera.y -= (scrolly - event.button.y);// * zoom;	
                scrollx = event.button.x;
                scrolly = event.button.y;
            }
            break;
        }

        default:
            break;
        }
    };
}

void Game::update(int tick) {
    (void)tick;
    builder->update();
}

void Game::render(int tick) {
    (void)tick;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    int fieldSize = sizeof field.field[0] / sizeof(int);

    int drawSize = 5;
    SDL_FRect activeField;
    activeField.w = zoom * drawSize;
    activeField.h = zoom * drawSize;


    for(int x = 0; x < fieldSize; x++) {
        for(int y = 0; y < fieldSize; y++) {

            activeField.x = (x * activeField.w) + (WINDOW_WIDTH / 2) - (camera.x * zoom);
            activeField.y = (y * activeField.w) + (WINDOW_HEIGTH / 2) + (camera.y * zoom);

            SDL_SetRenderDrawColor(renderer, 255 - x * 30, 0, 255 - y * 30, 255);
            SDL_RenderDrawRectF(renderer, &activeField);
        }
    }

    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);

    SDL_RenderDrawPoint(renderer, WINDOW_WIDTH / 2, WINDOW_HEIGTH / 2);
    SDL_RenderDrawPoint(renderer, clicked.x, clicked.y);

    //renderDebug();

    builder->render(renderer);

    SDL_RenderPresent(renderer);
}

void Game::renderDebug() {
    int x = 0;

    renderString("SDL Ticks " + toString(SDL_GetTicks()), x, getDebugY(20), 20, Sans, White);
    renderString("ticks " + toString(ticks), x, getDebugY(20), 20, Sans, White);
    renderString("frames " + toString(frames), x, getDebugY(20), 20, Sans, White);
    renderString("zoom " + toStringf(zoom), x, getDebugY(20), 20, Sans, White);

    renderString("camera X " + toString(camera.x) + " Y " + toString(camera.y), x, getDebugY(20), 20, Sans, White);
    renderString("clicked X " + toString(clicked.x) + " Y " + toString(clicked.y), clicked.x, clicked.y, 20, Sans, White);

    debugMessageCount = 0;
}

int Game::getDebugY(int size) {
    return (debugMessageCount++ * size) + (size / 10);
}

void Game::renderString(std::string message, int x, int y, int size, TTF_Font *font, SDL_Color color) {

    SDL_Surface *tmpSurface = TTF_RenderText_Solid(font, message.c_str(), color);

    SDL_Texture *debugMessageTexture = SDL_CreateTextureFromSurface(renderer, tmpSurface);

    SDL_Rect position;

    position.x = x;
    position.y = y;

    position.w = strlen(message.c_str()) * (size / 2);
    position.h = size;

    SDL_RenderCopy(renderer, debugMessageTexture, NULL, &position);

    if(debugMessageTexture != nullptr) {
        SDL_DestroyTexture(debugMessageTexture);
    }
    if(tmpSurface != nullptr) {
        SDL_FreeSurface(tmpSurface);
    }
}

void Game::clean() {
    if(Sans != nullptr) {
        TTF_CloseFont(Sans);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool Game::running() {
    return isRunning;
}
