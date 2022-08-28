#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "SDL.h"
#include "cpu.h"
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <string>
#include "sound.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionOpen_ROM_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open File", QDir::rootPath(), "");
    if (fileName == nullptr) return;

    try {
        QByteArray ba = fileName.toLocal8Bit();
        init(ba.data());
    } catch (const std::exception& e) {
        QMessageBox box;
        box.setText(e.what());
        box.exec();
        return;
    }
}

void MainWindow::init(char *game)
{
    prepare_file(game);
    //Initialize SDL Video
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_DestroyWindow(sdl_window);
        SDL_Quit();
        return;
    }

    //Initialise SDL Audio
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        SDL_DestroyWindow(sdl_window);
        SDL_Quit();
        return;
    }

    SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &sdl_window, &renderer);
    screen_surface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 24, 0, 0, 0, 0);
    start_cpu(game);
    init_sound();
    start_main_loop();
    SDL_DestroyWindow(sdl_window);
    SDL_Quit();
}
