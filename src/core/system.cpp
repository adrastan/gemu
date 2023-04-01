#include <string>

#include "system.h"
#include "logger.h"

System::System(const std::string file_path) :
    memory(std::make_unique<Memory>()),
    cpu(std::make_unique<Cpu>()),
    timers(std::make_unique<Timers>()),
    sound_controller(std::make_unique<Sound>()),
    interrupt(std::make_unique<Interrupt>())
{
    this->cart = nullptr;
    this->rom_file = nullptr;
    this->opcode = std::make_unique<Opcode>(this, memory.get(), cpu.get());
    this->lcd_controller = std::make_unique<LCDController>(memory.get());
    this->memory->lcd_controller = this->lcd_controller.get();
    this->load_cart_from_file(file_path);
    this->interrupt->memory = this->memory.get();
    this->interrupt->cpu = this->cpu.get();
    this->memory->lcd_controller->interrupt = this->interrupt.get();
    this->cpu->memory = this->memory.get();
}

System::~System() {}

void System::power_on()
{
    if (this->cart == nullptr)
    {
        Logger::log("Could not power on. Cart not initialised.");
        return;
    }

    this->init_SDL();
    this->lcd_controller->screen_surface = this->screen_surface;
    this->lcd_controller->renderer = this->renderer;
    this->is_running = true;

    Logger::log("Starting main game loop.");
    while (this->is_running)
    {
        this->draw_frames();
    }
}

void System::draw_frames()
{
    while (this->lcd_controller->fps_count <= 70224)
    {
        this->poll_events();
        this->next_op();
    }
    this->lcd_controller->draw_frame();
    this->lcd_controller->fps_count -= 70224;
}

void System::poll_events()
{
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            // case SDL_KEYDOWN:
            // case SDL_KEYUP: update_joypad(&event->key); break;
            case SDL_QUIT:
                this->power_off();
                break;
        }
    }
}

void System::next_op()
{
    this->counter = this->cpu->delay;
    this->cpu->delay = 0;
    if (!this->halt)
    {
        this->opcode->next();
        this->counter += System::op_cycles[this->opcode->current()];
        this->opcode->process();
    }
    else
    {
        this->counter += 4;
    }
    this->lcd_controller->fps_count += this->counter;
    this->lcd_controller->update(this->counter);
    this->sound_controller->update(this->counter);
    this->timers->update(this->counter);
    this->check_interrupts();
}

void System::check_interrupts()
{

}

void System::power_off()
{
    Logger::log("Powering off.");
    this->is_running = false;
}

void System::load_cart_from_file(const std::string file_path)
{
    this->rom_file = std::make_unique<File>(file_path);

    if (this->rom_file->buf == nullptr)
    {
        Logger::log("Can't open rom file.");
        return;
    }

    this->cart = std::make_unique<Cartridge>(this->rom_file->buf, this->rom_file->size);
    this->cart->print();

    this->memory->cart = this->cart.get();
    this->lcd_controller->cart = this->cart.get();
}

void System::init_SDL() {
    Logger::log("Initialising SDL..");

    //Initialize SDL Video
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::string s = SDL_GetError();
        Logger::log("SDL could not initialize! SDL_Error: " + s);
        return;
    }

    //Initialise SDL Audio
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        std::string s = SDL_GetError();
        Logger::log("SDL could not initialize! SDL_Error: " + s);
        return;
    }

    SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &sdl_window, &renderer);
    this->screen_surface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 24, 0, 0, 0, 0);
}

void System::stop_SDL()
{
    SDL_DestroyWindow(this->sdl_window);
    SDL_Quit();
}
