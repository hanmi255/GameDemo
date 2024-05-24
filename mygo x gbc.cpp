#include <graphics.h>
#include <conio.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <iostream>
#include <Windows.h>
#include <vector>
#include <string>
#include <list>
#include <queue>
#include <set>
#include <easyx.h>
#include <chrono>
#include <thread>
#include <random>
#include <sstream>
#include <iomanip>
#include <tchar.h>
using namespace std;

// ��Ļ����
std::vector<LPCTSTR> credits = {
    _T("��Ȼ    �鳤"),
    _T("init() playerMove() callchess(int n)"),
    _T("hanmi	 ������"),
    _T("Lead Actress: Mary Brown"),
    _T("С��	 ������"),
    _T("gameView() ishasdown() isWin() gameLoop()"),
    _T("С��ͨ	 ��Ϣ��"),
    _T("menuView() setView() UI"),
    _T("��	 ��Ϣ��"),
    _T("pauseView()  int Pop()"),
    _T("��Ժ�	 ������ ��Ŀ����"),
    _T("init() playerMove() callchess(int n)"),
    _T("���Ӷ��߽�	 ������"),
    _T("winView()"),
    _T("��ɶ����Ҳ���	 ���鳤"),
    _T("gameView_ShowMap()"),
    _T("����	 �ල��"),
    _T("playerMove() callchess(int n)"),
    _T("����	 ������ ��Ŀ����"),
    _T("insertcemetery(int chess) drawCemetery()"),
    
};


// ����ȫ�ֱ���
int flag = 0;
ExMessage m = { 0 };
IMAGE cqp; // ����ͼ��
IMAGE cqp_n; // ��ͣ������ͼ��
IMAGE testchess[10]; // ����ͼ��
IMAGE testchess_mask[10]; // ��������ͼ��
IMAGE testchess_n[10]; // ����ͼ��
IMAGE testchess_n_mask[10]; // ��������ͼ��
TCHAR chesssign[10][30] = { _T("res/Ma1.jpg"), _T("res/Ma2.jpg"), _T("res/Ma3.jpg"), _T("res/Ma4.jpg"), _T("res/Ma5.jpg"), _T("res/Ga1.jpg"), _T("res/Ga2.jpg"), _T("res/Ga3.jpg"), _T("res/Ga4.jpg"), _T("res/Ga5.jpg") };
TCHAR chesssign_mask[10][30] = { _T("res/Mb1.jpg"), _T("res/Mb2.jpg"), _T("res/Mb3.jpg"), _T("res/Mb4.jpg"), _T("res/Mb5.jpg"), _T("res/Gb1.jpg"), _T("res/Gb2.jpg"), _T("res/Gb3.jpg"), _T("res/Gb4.jpg"), _T("res/Gb5.jpg") };


/*
��������
    101,102,103,104,105��ʾMygo�����Դ���Ĺ�ص�����
    001,002,003,004,005��ʾMygo�������Դ���Ĺ�ص�����
    106,107,108,109,110��ʾGBC�����Դ���Ĺ�ص�����
    006,007,008,009,010��ʾGBC�������Դ���Ĺ�ص�����

    ��λ����ʾ�ܷ����Ĺ�أ���λʮλ��ʾ���ӱ��
*/
int Chess;
int map[19][19]; // ��������

// ���̺����Ӳ���(��������չ�Զ�������)
const int board_start_x = 352; // ������ʼX����
const int board_start_y = 48; // ������ʼY����
const int cell_size = 32; // ���̵�Ԫ���С
const int piece_size = 30; // ���Ӵ�С
const int piece_half_size = piece_size / 2; // ���Ӵ�С��һ��

//Ĺ������
int mygo_cemetery[10];
int GBC_cemetery[10];
int mygo_chess = 0;//mygo��ǰĹ����������
int GBC_chess = 0;//GBC��ǰĹ����������

IMAGE baiban_n;
bool mluo = true;
int startX1 = 50; // Player A ��Ĺ����ʼX����
int startY1 = 700; // Player A ��Ĺ����ʼY����
int startX2 = 690; // Player B ��Ĺ����ʼX����
int startY2 = 700; // Player B ��Ĺ����ʼY����
int gap = 40; // ����֮��ļ��
int* chess1 = new int;

// ����ȫ�ֱ���
std::queue<int> chessqueue; // ���Ӷ���
int gradeSize = 19; // ���̴�С
int margin_x; // ˮƽ�߾�
int margin_y; // ��ֱ�߾�
float chessSize = 60; // ���Ӵ�С
bool playerFlag; // true: Mygo�ߣ�false��GBC��
IMAGE menuimg;
IMAGE gameimg;
IMAGE setimg;
IMAGE winimg;
IMAGE pauseimg;
IMAGE chessimg[10];
IMAGE baiban;//���������Ӵ���Ĺ���ڸ�����
int round1 = 1;
ExMessage msg;
int PieceType;
MOUSEMSG msg2; // ȫ�ֱ��������ڴ洢�����Ϣ


// ������ʹ�õ�ö��
enum ViewState {
    MENU,
    GAME,
    SETTINGS,
    EXIT
};

// ��������
int chessMap[19][19]; 

// ��������
void gameView();
void setView();
void menuView();
void gameView_ShowMap();
void winView(int i);
void pauseView(IMAGE& bk, Mix_Chunk* click2Sound, Mix_Chunk* click6Sound);
int isWin(int Chess, int x, int y);
int ishasdown(int x, int y);
void insertcemetery(int chess);
int iscontinuous(int x, int y);
int GetRandomNum(int min, int max);// ��ȡ��������
int get_center_position(int start, int index, int cell_size);// ����������������λ��
void drawCemetery();
void initGameView();
void gameLoop();
void finalizeGameView();
void gbc_win();
void mygo_win();
void gameSet(IMAGE& bk, Mix_Chunk* click2Sound, Mix_Chunk* click6Sound);
void GameRules();
void displayScrollingCredits();
int getchess(int i);

// ��Чȫ�ֱ���
std::vector<Mix_Chunk*> clickSounds; // �洢�����Ч
std::vector<Mix_Music*> bgMusics; // �洢��������
int clickVolume = 50; // �����Ч����
int bgVolume = 50; // ������������
int clickx2 = clickVolume + 414;
int bgmx1 = bgVolume + 414;
Mix_Chunk* clickSound = nullptr;
Mix_Chunk* click5Sound = nullptr;
Mix_Chunk* click2Sound = nullptr;
Mix_Chunk* click6Sound = nullptr;

//��ʼ��-----------------------------------------------------------------------------

// ��ʼ��SDL2_mixer
bool initSDL2Mixer() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL2��ʼ��ʧ��: " << SDL_GetError() << std::endl;
        return false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL2_mixer��ʼ��ʧ��: " << Mix_GetError() << std::endl;
        return false;
    }

    return true;
}

// ������Ч
Mix_Chunk* loadSoundEffect(const char* path) {
    Mix_Chunk* sound = Mix_LoadWAV(path);
    if (sound == nullptr) {
        std::cerr << "��Ч�ļ�����ʧ��: " << Mix_GetError() << std::endl;
    }
    return sound;
}

// ���ر�������
Mix_Music* loadMusic(const char* path) {
    Mix_Music* music = Mix_LoadMUS(path);
    if (music == nullptr) {
        std::cerr << "�������ּ���ʧ��: " << Mix_GetError() << std::endl;
    }
    return music;
}

// �ͷ���Ч
void freeSoundEffect(Mix_Chunk* sound) {
    if (sound != nullptr) {
        Mix_FreeChunk(sound);
    }
}

// �ͷű�������
void freeMusic(Mix_Music* music) {
    if (music != nullptr) {
        Mix_FreeMusic(music);
    }
}

// ��ʼ������
void chushihua() {
    for (int i = 0; i < 19; i++) {
        for (int j = 0; j < 19; j++) {
            map[i][j] = 0; // ������������λ����Ϊ0
        }
    }
    for (int i = 0; i < 10; i++) {
        mygo_cemetery[i] = 0;
        GBC_cemetery[i] = 0;
    }
    mygo_chess = 0;//mygo��ǰĹ����������
    GBC_chess = 0;//GBC��ǰĹ����������

}

// ���ô���λ�ú���
void centerWindow() {
    HWND hwnd = GetHWnd();
    RECT rect;
    GetWindowRect(hwnd, &rect);

    int windowWidth = rect.right - rect.left;
    int windowHeight = rect.bottom - rect.top;

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    int posX = (screenWidth - windowWidth) / 2;
    int posY = (screenHeight - windowHeight) / 2;

    SetWindowPos(hwnd, HWND_TOP, posX, posY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

//��Ϸ���溯��-----------------------------------------------------------------------------

/*
    menuView()
    ������: С��ͨ
    ����: easyx
        1. չʾѡ��: ������Ϸ �������� �������а� �˳���Ϸ
        2. �����������
        3. ִ�ж�Ӧ����
            ������Ϸ: ������Ϸ����
            ��������: �������ý���
            �������а�: �������а����(�����ڴ�)
            �˳���Ϸ: ��������
    ����: void
    ����ֵ: void
*/
// ���˵���ͼ����
void menuView() {
    initgraph(1280, 800); // ��ʼ��ͼ�δ���
    centerWindow(); // ���д���

    IMAGE background;
    loadimage(&background, _T("res/menu3.jpg")); // ���ر���ͼƬ

    ViewState viewState = MENU; // ��ʼ��ͼ״̬Ϊ�˵�

    if (!initSDL2Mixer()) {
        return;
    }

    // ���ص����Ч�ͱ�������
    clickSounds.push_back(loadSoundEffect("res/click1.mp3"));
    clickSounds.push_back(loadSoundEffect("res/click2.mp3"));
    bgMusics.push_back(loadMusic("res/bg1.mp3"));
    bgMusics.push_back(loadMusic("res/bg2.mp3"));

    Mix_Volume(-1, clickVolume); // ���õ����Ч����
    Mix_VolumeMusic(bgVolume); // ���ñ�����������

    if (!bgMusics.empty() && bgMusics[0] != nullptr) {
        Mix_PlayMusic(bgMusics[0], -1); // ѭ�����ű�������
    }

    putimage(0, 0, &background); // ��ʾ����ͼƬ
    FlushBatchDraw(); // ˢ�»�ͼ

    while (viewState != EXIT) {
        // ����������
        if (_kbhit()) {
            char ch = _getch();
            if (ch == 27) { // ����ESC���˳�
                viewState = EXIT;
            }
        }

        // ��������
        if (MouseHit()) {
            MOUSEMSG msg = GetMouseMsg();
            if (msg.uMsg == WM_LBUTTONDOWN) {
                if (msg.x >= 160 && msg.x <= 432) {
                    if (msg.y >= 472 && msg.y <= 508) {
                        // ���ŵ����Ч
                        if (!clickSounds.empty() && clickSounds[0] != nullptr) {
                            Mix_PlayChannel(-1, clickSounds[0], 0);
                        }
                        viewState = GAME; // �л�����Ϸ��ͼ
                    }
                    else if (msg.y >= 565 && msg.y <= 603) {
                        // ���ŵ����Ч
                        if (!clickSounds.empty() && clickSounds[0] != nullptr) {
                            Mix_PlayChannel(-1, clickSounds[0], 0);
                        }
                        viewState = SETTINGS; // �л���������ͼ
                    }
                    else if (msg.y >= 660 && msg.y <= 696) {
                        // ���ŵ����Ч
                        if (!clickSounds.empty() && clickSounds[0] != nullptr) {
                            Mix_PlayChannel(-1, clickSounds[0], 0);
                        }
                        viewState = EXIT; // �˳�
                    }
                }
            }
        }

        // ��������ͣ
        POINT cursor;
        GetCursorPos(&cursor);
        ScreenToClient(GetHWnd(), &cursor);

        static bool hover_played = false;
        if (cursor.x >= 160 && cursor.x <= 432) {
            if ((cursor.y >= 472 && cursor.y <= 508) ||
                (cursor.y >= 565 && cursor.y <= 603) ||
                (cursor.y >= 660 && cursor.y <= 696)) {
                if (!hover_played) {
                    if (!clickSounds.empty() && clickSounds[1] != nullptr) {
                        Mix_PlayChannel(-1, clickSounds[1], 0); // ������ͣ��Ч
                    }
                    hover_played = true;
                }
            }
            else {
                hover_played = false;
            }
        }
        else {
            hover_played = false;
        }

        // ������ͼ״̬�л���ͼ
        switch (viewState) {
        case GAME:
            gameView();
            putimage(0, 0, &background);
            FlushBatchDraw();
            viewState = MENU;
            break;

        case SETTINGS:
            setView();
            putimage(0, 0, &background);
            FlushBatchDraw();
            viewState = MENU;
            break;

        case EXIT:
            break;

        default:
            break;
        }
    }

    // �ͷ���Ч�ͱ�������
    for (auto sound : clickSounds) {
        freeSoundEffect(sound);
    }

    for (auto music : bgMusics) {
        freeMusic(music);
    }

    Mix_CloseAudio();
    SDL_Quit();
    closegraph();
}


/*
    setView()
    ������:С��ͨ
    ����:
        1. չʾ���ԸĶ�����Ϸ�б���
        2. �����������
        3. ִ�ж�Ӧ����
            ��Ч������/�رգ�
            ����������С
            ѡ�����̴�С
    ����: void
    ����ֵ: void
*/
// ���ý���
void setView() {
    initgraph(1280, 800); // ��ʼ��ͼ�δ���
    centerWindow(); // ���д���

    IMAGE bg;
    loadimage(&bg, _T("res/setview.jpg")); // ���ر���ͼƬ

    IMAGE buffer(1280, 800);
    setbkmode(TRANSPARENT); // ���ñ���ģʽΪ͸��
    setlinestyle(PS_SOLID, 5, NULL); // ����������ʽ
    settextcolor(YELLOW); // ����������ɫ
    LOGFONT font;
    gettextstyle(&font);

    int width = 1280, height = 800;
    int posX = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
    int posY = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
    SetWindowPos(GetHWnd(), HWND_TOP, posX, posY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    putimage(0, 0, &bg); // ��ʾ����ͼƬ

    int a2change = clickVolume;
    int a1change = bgVolume;
    int y1 = 228;
    int y2 = 368;

    bool dragging1 = false, dragging2 = false;
    int dragStartX1 = 0, dragStartX2 = 0;

    IMAGE imga1, imga2;
    loadimage(&imga1, _T("res/bott1.png"));
    loadimage(&imga2, _T("res/bott1.png"));

    while (true) {
        BeginBatchDraw();
        putimage(0, 0, &bg); // ��ʾ����ͼƬ

        if (MouseHit()) {
            MOUSEMSG msg = GetMouseMsg();

            if (msg.uMsg == WM_LBUTTONDOWN) {
                if (msg.x >= bgmx1 && msg.x <= bgmx1 + imga1.getwidth() && msg.y >= y1 && msg.y <= y1 + imga1.getheight()) {
                    dragging1 = true;
                    dragStartX1 = msg.x - bgmx1;
                }
                if (msg.x >= clickx2 && msg.x <= clickx2 + imga2.getwidth() && msg.y >= y2 && msg.y <= y2 + imga2.getheight()) {
                    dragging2 = true;
                    dragStartX2 = msg.x - clickx2;
                }
                if (msg.x >= 419 && msg.x <= 622 && msg.y >= 566 && msg.y <= 696) {
                    if (!clickSounds.empty() && clickSounds[0] != nullptr) {
                        Mix_PlayChannel(-1, clickSounds[0], 0);
                    }
                    menuView(); // �������˵�               
                }
                // ������Ƿ���ָ���ľ���������
                if (msg.x >= 142 && msg.x <= 345 && msg.y >= 566 && msg.y <= 669) {
                    if (!clickSounds.empty() && clickSounds[0] != nullptr) {
                        Mix_PlayChannel(-1, clickSounds[0], 0);
                    }
                    GameRules(); // ����˵��
                    break; // �˳�ѭ��
                }
                //���������Ļ
                if (msg.x >= 1172 && msg.x <= 1255 && msg.y >= 691 && msg.y <= 781) {
                    if (!clickSounds.empty() && clickSounds[1] != nullptr) {
                        Mix_PlayChannel(-1, clickSounds[1], 0);
                    }
                    displayScrollingCredits(); // ����˵��
                    break; // �˳�ѭ��
                }



            }


            if (msg.uMsg == WM_MOUSEMOVE) {
                if (dragging1) {
                    int newX1 = msg.x - dragStartX1;
                    int deltaX1 = newX1 - 464;
                    if (deltaX1 >= -129 && deltaX1 <= 129) {
                        bgmx1 = newX1;
                        a1change = deltaX1 / 2 + 64;
                    }
                }
                if (dragging2) {
                    int newX2 = msg.x - dragStartX2;
                    int deltaX2 = newX2 - 464;
                    if (deltaX2 >= -129 && deltaX2 <= 129) {
                        clickx2 = newX2;
                        a2change = deltaX2 / 2 + 64;
                    }
                }
            }

            if (msg.uMsg == WM_LBUTTONUP) {
                dragging1 = false;
                dragging2 = false;
            }
        }

        clickVolume = a2change;
        bgVolume = a1change;

        putimage(bgmx1, y1, &imga1);
        putimage(clickx2, y2, &imga2);

        TCHAR str1[50], str2[50];
        _stprintf_s(str1, _T("%d"), a1change);
        _stprintf_s(str2, _T("%d"), a2change);
        outtextxy(616, 233, str1);
        outtextxy(616, 374, str2);

        EndBatchDraw();

        if (_kbhit()) {
            if (_getch() == 27) {
                break;
            }
        }
    }

    closegraph();
}

//��ͣ���������
void gameSet(IMAGE& bk, Mix_Chunk* click2Sound, Mix_Chunk* click6Sound) {
    // ���ر���ͼƬ
    IMAGE bg;
    loadimage(&bg, _T("res/gameSet.jpg"));
    centerWindow();
    IMAGE buffer(1280, 800);
    setbkmode(TRANSPARENT); // ���ñ���ģʽΪ͸��
    setlinestyle(PS_SOLID, 5, NULL); // ����������ʽ
    settextcolor(YELLOW); // ����������ɫ
    LOGFONT font;
    gettextstyle(&font);

    putimage(0, 0, &bg); // ��ʾ����ͼƬ

    int a2change = clickVolume;
    int a1change = bgVolume;
    int y1 = 228;
    int y2 = 368;

    bool dragging1 = false, dragging2 = false;
    int dragStartX1 = 0, dragStartX2 = 0;

    IMAGE imga1, imga2;
    loadimage(&imga1, _T("res/bott1.png"));
    loadimage(&imga2, _T("res/bott1.png"));

    while (true) { // ����ѭ���ȴ��û�����
        BeginBatchDraw();
        putimage(0, 0, &bg); // ��ʾ����ͼƬ

        MOUSEMSG msg = GetMouseMsg();

        if (msg.uMsg == WM_LBUTTONDOWN) {
            if (msg.x >= bgmx1 && msg.x <= bgmx1 + imga1.getwidth() && msg.y >= y1 && msg.y <= y1 + imga1.getheight()) {
                dragging1 = true;
                dragStartX1 = msg.x - bgmx1;
            }
            if (msg.x >= clickx2 && msg.x <= clickx2 + imga2.getwidth() && msg.y >= y2 && msg.y <= y2 + imga2.getheight()) {
                dragging2 = true;
                dragStartX2 = msg.x - clickx2;
            }
            if (msg.x >= 419 && msg.x <= 622 && msg.y >= 566 && msg.y <= 696) {
                if (!clickSounds.empty() && clickSounds[0] != nullptr) {
                    Mix_PlayChannel(-1, clickSounds[0], 0);
                }
                // ���ı������ֵ����ȫ�ֱ���
                clickVolume = a2change;
                bgVolume = a1change;
                Mix_Volume(-1, clickVolume);
                Mix_VolumeMusic(bgVolume);
                return; // ������ͣ����               
            }
        }

        if (msg.uMsg == WM_MOUSEMOVE) {
            if (dragging1) {
                int newX1 = msg.x - dragStartX1;
                int deltaX1 = newX1 - 464;
                if (deltaX1 >= -129 && deltaX1 <= 129) {
                    bgmx1 = newX1;
                    a1change = deltaX1 / 2 + 64;
                }
            }
            if (dragging2) {
                int newX2 = msg.x - dragStartX2;
                int deltaX2 = newX2 - 464;
                if (deltaX2 >= -129 && deltaX2 <= 129) {
                    clickx2 = newX2;
                    a2change = deltaX2 / 2 + 64;
                }
            }
        }

        if (msg.uMsg == WM_LBUTTONUP) {
            dragging1 = false;
            dragging2 = false;
        }

        putimage(bgmx1, y1, &imga1);
        putimage(clickx2, y2, &imga2);

        TCHAR str1[50], str2[50];
        _stprintf_s(str1, _T("%d"), a1change);
        _stprintf_s(str2, _T("%d"), a2change);
        outtextxy(616, 233, str1);
        outtextxy(616, 374, str2);

        EndBatchDraw();

        if (_kbhit()) {
            if (_getch() == 27) {
                break;
            }
        }
    }

    // ���ı������ֵ����ȫ�ֱ���
    clickVolume = a2change;
    bgVolume = a1change;
    Mix_Volume(-1, clickVolume);
    Mix_VolumeMusic(bgVolume);

    closegraph();
}

//��Ϸ˵������
void GameRules()
{
    IMAGE rules;
    loadimage(&rules, _T("res/rules.jpg")); // ���ز���˵��ͼƬ

    // ���ͼƬ�Ƿ�ɹ�����
    if (rules.getwidth() == 0 || rules.getheight() == 0) {
        printf("Failed to load rules.jpg\n");
        return;
    }

    // ȷ��ͼ�δ����ѳ�ʼ��
    initgraph(1280, 800);
    centerWindow();  // ȷ�� centerWindow ��������ȷʵ��

    // ����һ������Ļ��С��ͬ�Ļ�����
    IMAGE buffer(1280, 800);
    SetWorkingImage(&buffer); // ���û�ͼ������Ŀ��Ϊ������

    putimage(0, 0, &rules); // �ڻ���������ʾ����˵��ͼƬ
    SetWorkingImage(); // ��ԭ��ͼ������Ŀ��Ϊ��Ļ

    // ����������ͼģʽ
    BeginBatchDraw();

    while (true)
    {
        // �����������ݸ��Ƶ���Ļ
        putimage(0, 0, &buffer);

        // ����Ƿ���������¼�
        if (MouseHit())
        {
            MOUSEMSG mouseMsg = GetMouseMsg();
            if (mouseMsg.uMsg == WM_LBUTTONDOWN)
            {
                // ������Ƿ���ָ���ľ���������
                if (mouseMsg.x >= 949 && mouseMsg.x <= 1054 && mouseMsg.y >= 643 && mouseMsg.y <= 698)
                {
                    setView(); // ���� setView ����
                    break; // �˳�ѭ��
                }
            }
        }

        // ˢ����Ļ����ʾ����������
        FlushBatchDraw();
    }

    // ����������ͼģʽ
    EndBatchDraw();

    closegraph();
}

/*
    gameView_ShowMap()
    ������:��ɶ��
    ����:easyx
        ��ӡ���̡���ͣ��ʱ��غ�����Ĺ�ء��ٻ��������б���������������
        ����int iscontinuous()��������������ֵ�����������������͸����
    ����: void
    ����ֵ: void
*/
// ��ʾ��Ϸ��ͼ����
void gameView_ShowMap() {
    loadimage(&gameimg, L"res/gameView.jpg", 1280, 800);
    putimage(0, 0, &gameimg);
}

/*
    pauseView()
    ������: ��
    ����: easyx ��ͣʱ���¼ ��ӡ��ͣ���� �û����Ե��������Ϸ���˳���Ϸ
    ����: void
    ����ֵ: void
*/
// ��ͣ����
void pauseView(IMAGE& bk, Mix_Chunk* click2Sound, Mix_Chunk* click6Sound) {
    loadimage(&pauseimg, _T("res/pauseView.jpg")); // ������ͣ����ͼ��

    // ȷ������ˢ��
    BeginBatchDraw();
    putimage(0, 0, &pauseimg); // ��ʾ��ͣ����
    EndBatchDraw();
    FlushBatchDraw();

    auto start = std::chrono::high_resolution_clock::now(); // ��¼��ʼʱ��

    while (true) { // ����ѭ���ȴ��û�����
        if (MouseHit()) { // ����Ƿ��������Ϣ
            msg2 = GetMouseMsg(); // ��ȡ�����Ϣ
            if (msg2.uMsg == WM_LBUTTONDOWN && msg2.x >= 518 && msg2.x <= 763 && msg2.y >= 193 && msg2.y <= 640) {
                // ��������λ��
                if (msg2.x >= 550 && msg2.x <= 730 && msg2.y >= 242 && msg2.y <= 308) {
                    // ������Ϸ��ť����
                    // ���ŵ����Ч
                    if (click2Sound != nullptr) {
                        Mix_PlayChannel(-1, click2Sound, 0);
                    }
                    auto end = std::chrono::high_resolution_clock::now(); // ��¼����ʱ��
                    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(); // ������ͣʱ��                    

                    // �ָ�ԭ����
                    BeginBatchDraw();
                    putimage(0, 0, &bk);
                    EndBatchDraw();
                    FlushBatchDraw();
                    break; // �˳�ѭ��
                }
                else if (msg2.x >= 550 && msg2.x <= 730 && msg2.y >= 492 && msg2.y <= 557) {
                    // ���ŵ����Ч
                    if (click6Sound != nullptr) {
                        Mix_PlayChannel(-1, click6Sound, 0);
                    }
                    menuView(); // ������ҳ��ť����                    
                    break; // �˳�ѭ��
                }
                else if (msg2.x >= 517 && msg2.x <= 762 && msg2.y >= 322 && msg2.y <= 427) {
                    // ���ð�ť����
                    // ���ŵ����Ч
                    if (click6Sound != nullptr) {
                        Mix_PlayChannel(-1, click6Sound, 0);
                    }
                    // �������ý���
                    gameSet(bk, click2Sound, click6Sound);
                    // ȷ������ˢ��
                    BeginBatchDraw();
                    putimage(0, 0, &pauseimg); // ������ʾ��ͣ����
                    EndBatchDraw();
                    FlushBatchDraw();
                }
            }
        }
    }
}


//��Ϸ����-----------------------------------------------------------------------------
/*
    gameView()/init()/playerMove()/callchess(int n)
    ������:��Ժ������ӡ��鳤��С��
    ����:
         easyx
        1. ��ʼ����Ϸ����,ֱ�ӳ�ʼ��
        while(1)
        {
            2. չʾ����
            3. ��ʾ��ǰ���ӷ�
            3. �����������:
                �ж����������
                    ���ֵ�����̽��㣬����
                    �����Ӷ��в��գ������������ӣ����꽻��
                    �����ֺ����ٻ����ٵ��Ĺ�������ӣ�ʹ֮�������
            4. ���Ӻ�Ľ�� = ����������Ӧλ���������
            5. �жϽ�����(���ú���hasdown())
                ���ӳɹ�: ������Ч
                ����ʧ��: ʧ����Ч
            6. ʤ���ж�:(���ú���isWin())
                ����Ϸʤ��: չʾʤ������
        }
    ����: void
    ����ֵ: void
*/

//��Ϸ�����ʼ��
void initGameView() {
    // ��ʼ������
    initgraph(1280, 800); // ��ʹ��EX_SHOWCONSOLE��־
    centerWindow();
    cleardevice();
    chushihua(); // ��ʼ������
    loadimage(&cqp, _T("res/gameView.jpg"), 1280, 800); // ���ر���ͼ��
    loadimage(&cqp_n, _T("res/gameView.jpg"), 1280, 800); // ���ر���ͼ��
    putimage(0, 0, &cqp); // ��ʾ����ͼ��
    loadimage(&baiban, L"res/baiban.jpg", 70, 70);

    // ��ʼ��SDL2_mixer
    if (!initSDL2Mixer()) {
        return;
    }

    // ���ص����Ч
    clickSound = loadSoundEffect("res/click4.mp3");
    if (clickSound == nullptr) {
        std::cerr << "�����Ч����ʧ��" << std::endl;
    }

    // ����������Ч
    click5Sound = loadSoundEffect("res/click5.mp3");
    if (click5Sound == nullptr) {
        std::cerr << "������Ч����ʧ��" << std::endl;
    }

    // ������ͣ��ť��Ч
    click2Sound = loadSoundEffect("res/click2.mp3");
    if (click2Sound == nullptr) {
        std::cerr << "��ͣ��ť��Ч����ʧ��" << std::endl;
    }

    // �����˳���Ϸ��Ч
    click6Sound = loadSoundEffect("res/click6.mp3");
    if (click6Sound == nullptr) {
        std::cerr << "�˳���Ϸ��Ч����ʧ��" << std::endl;
    }

    // ���õ����Ч����
    Mix_VolumeChunk(clickSound, clickVolume);
    Mix_VolumeChunk(click5Sound, clickVolume);
    Mix_VolumeChunk(click2Sound, clickVolume);
    Mix_VolumeChunk(click6Sound, clickVolume);
}

//��Ϸ��ѭ��
void gameLoop() {
    int xuanzi = 0;
    bool zhong = true;
    int test = 1;
    int* chess = new int;
    *chess = 0;

    while (zhong) {
        BeginBatchDraw(); // ��ʼ��������

        if (flag % 2 == 0 && test == 1 && mluo == true) {
            *chess = GetRandomNum(101, 105);
            int c = *chess % 10 - 1;
            loadimage(&testchess[c], chesssign[c], 60, 60);
            loadimage(&testchess_mask[c], chesssign_mask[c], 60, 60);
            putimage(1160, 70, &baiban);
            putimage(1160, 70, &testchess_mask[c], SRCAND);
            putimage(1160, 70, &testchess[c], SRCPAINT);
            loadimage(&testchess_n[c], chesssign[c], piece_size, piece_size);
            loadimage(&testchess_n_mask[c], chesssign_mask[c], piece_size, piece_size);
            test++; // ��ȡ������ӱ��
            mluo = true;
        }
        else if (flag % 2 == 1 && test == 0 && mluo == true) {
            *chess = GetRandomNum(106, 110);
            int c = *chess % 10 - 1;
            if (c == -1) {
                c = 9;
            }
            loadimage(&testchess[c], chesssign[c], 60, 60);
            loadimage(&testchess_mask[c], chesssign_mask[c], 60, 60);
            putimage(1160, 70, &baiban);
            putimage(1160, 70, &testchess_mask[c], SRCAND);
            putimage(1160, 70, &testchess[c], SRCPAINT);
            loadimage(&testchess_n[c], chesssign[c], piece_size, piece_size);
            loadimage(&testchess_n_mask[c], chesssign_mask[c], piece_size, piece_size);
            test--; // ��ȡ������ӱ��
            mluo = true;
        }

        if (peekmessage(&m, EX_MOUSE | EX_KEY)) {
            if (m.message == WM_KEYDOWN) {
                if (m.vkcode == VK_ESCAPE) {
                    // ��ȡ��ǰ��Ļ���ݣ����ڻָ���ʾ
                    getimage(&cqp, 0, 0, 1280, 800);
                    pauseView(cqp, click2Sound, click6Sound); // ������ͣ����
                    EndBatchDraw(); // ������������
                    continue; // ��������ѭ����������������
                }
            }
            if (mygo_chess < 10 || GBC_chess < 10) {
                if (m.message == WM_RBUTTONDOWN) {
                    insertcemetery(*chess);
                    drawCemetery();

                    // ����Ĺ�غ�����µ�����
                    if (flag % 2 == 0 && mluo == true) {
                        *chess = GetRandomNum(101, 105);
                        int c = *chess % 10 - 1;
                        loadimage(&testchess[c], chesssign[c], 60, 60);
                        loadimage(&testchess_mask[c], chesssign_mask[c], 60, 60);
                        putimage(1160, 70, &baiban);
                        putimage(1160, 70, &testchess_mask[c], SRCAND);
                        putimage(1160, 70, &testchess[c], SRCPAINT);
                        loadimage(&testchess_n[c], chesssign[c], piece_size, piece_size);
                        loadimage(&testchess_n_mask[c], chesssign_mask[c], piece_size, piece_size); // ��ȡ������ӱ��
                        mluo = true;
                    }
                    else if (flag % 2 == 1 && mluo == true) {
                        *chess = GetRandomNum(106, 110);
                        int c = *chess % 10 - 1;
                        if (c == -1) {
                            c = 9;
                        }
                        loadimage(&testchess[c], chesssign[c], 60, 60);
                        loadimage(&testchess_mask[c], chesssign_mask[c], 60, 60);
                        putimage(1160, 70, &baiban);
                        putimage(1160, 70, &testchess_mask[c], SRCAND);
                        putimage(1160, 70, &testchess[c], SRCPAINT);
                        loadimage(&testchess_n[c], chesssign[c], piece_size, piece_size);
                        loadimage(&testchess_n_mask[c], chesssign_mask[c], piece_size, piece_size); // ��ȡ������ӱ��
                        mluo = true;
                    }
                }
            }

            if (m.message == WM_LBUTTONDOWN) {
                // ����Ƿ�����ͣ����
                if (m.x >= 53 && m.x <= 256 && m.y >= 49 && m.y <= 152) {
                    // ���ŵ����Ч
                    if (click2Sound != nullptr) {
                        Mix_PlayChannel(-1, click2Sound, 0);
                    }
                    // ��ȡ��ǰ��Ļ���ݣ����ڻָ���ʾ
                    getimage(&cqp, 0, 0, 1280, 800);
                    pauseView(cqp, click2Sound, click6Sound); // ������ͣ����
                    // �ָ���Ϸ��������»��Ʊ���
                    putimage(0, 0, &cqp);
                    EndBatchDraw(); // ������������
                    continue; // ��������ѭ����������������
                }

                // ������ӱ��
                for (int i = 0; i < 19; i++) {
                    for (int j = 0; j < 19; j++) {
                        int center_x = get_center_position(board_start_x, i, cell_size);
                        int center_y = get_center_position(board_start_y, j, cell_size);
                        if (m.x > center_x - piece_half_size && m.x < center_x + piece_half_size && m.y > center_y - piece_half_size - j && m.y < center_y + piece_half_size - j) {
                            if (ishasdown(i, j)) {
                                map[i][j] = *chess % 100;
                                int c = *chess % 10 - 1;
                                if (c == -1) {
                                    c = 9;
                                }
                                if (c >= 0 && c < 10) {
                                    putimage(center_x - piece_half_size, center_y - piece_half_size - j, &testchess_n_mask[c], SRCAND);
                                    putimage(center_x - piece_half_size, center_y - piece_half_size - j, &testchess_n[c], SRCPAINT);
                                    mluo = true;
                                    // ���ŵ����Ч
                                    if (clickSound != nullptr) {
                                        Mix_PlayChannel(-1, clickSound, 0);
                                    }
                                    if (c < 5 && m.message != WM_RBUTTONDOWN) {
                                        test = 0;
                                    }
                                    if (c >= 5 && m.message != WM_RBUTTONDOWN) {
                                        test = 1;
                                    }
                                    if (isWin(*chess, i, j) == 1) {
                                        // �ر�ͼ�δ���
                                        closegraph();
                                        // �ͷű�������
                                        Mix_HaltMusic();
                                        zhong = false; // ��Ϸ����
                                        winView(1);
                                    }
                                    if (isWin(*chess, i, j) == 2) {
                                        // GBC��ʤ
                                        // �ر�ͼ�δ���
                                        closegraph();
                                        // �ͷű�������
                                        Mix_HaltMusic();
                                        zhong = false; // ��Ϸ����
                                        winView(2);
                                    }
                                    if (iscontinuous(i, j) > 0) {
                                        // ����������Ч
                                        if (click5Sound != nullptr) {
                                            Mix_PlayChannel(-1, click5Sound, 0);
                                        }
                                        // �����������������
                                        cleardevice();
                                        putimage(0, 0, &cqp_n); // ���»��Ʊ���
                                        drawCemetery(); // ����Ĺ��
                                        for (int k = 0; k < 19; k++) {
                                            for (int l = 0; l < 19; l++) {
                                                if (map[k][l] != 0) {
                                                    int center_x = get_center_position(board_start_x, k, cell_size);
                                                    int center_y = get_center_position(board_start_y, l, cell_size);
                                                    int c = map[k][l] % 10 - 1;
                                                    if (c == -1) {
                                                        c = 9;
                                                    }
                                                    putimage(center_x - piece_half_size, center_y - piece_half_size - l, &testchess_n_mask[c], SRCAND);
                                                    putimage(center_x - piece_half_size, center_y - piece_half_size - l, &testchess_n[c], SRCPAINT);
                                                }
                                            }
                                        }
                                        IMAGE zhaohuan;
                                        loadimage(&zhaohuan, L"res/zhaohuan.jpg", 151, 101);
                                        putimage(1068, 498, &zhaohuan);
                                        // ֹͣˢ�����̣�����Ĺ���ٻ����Ӳ���
                                        EndBatchDraw();
                                        bool luoziw = true;
                                        while (luoziw) {
                                            // ���mygo��Ĺ��Ϊ�գ���ֱ�������غ�
                                            if (mygo_chess == 0 && flag % 2 == 0) {
                                                BeginBatchDraw();
                                                flag++;
                                                luoziw = false;
                                            }
                                            // ���GBC��Ĺ��Ϊ�գ���ֱ�������غ�
                                            else if (GBC_chess == 0 && flag % 2 == 1) {
                                                BeginBatchDraw();
                                                flag++;
                                                luoziw = false;
                                            }
                                            cout << "��������" << endl;
                                            if (peekmessage(&m, EX_MOUSE)) {
                                                if (m.message == WM_LBUTTONDOWN) {
                                                    for (int i = 0; i < 10; i++) {
                                                        // ��mygo��Ĺ�ز���
                                                        if (m.x > startX1 + i * gap - 12 && m.x < startX1 + i * gap + 20 && m.y > startY1 && m.y < startY1 + 24 && flag % 2 == 0) {
                                                            cout << "����mygoĹ�ص�" << i + 1 << "��λ��" << endl;
                                                            getchess(i);
                                                            *chess = *chess1;
                                                            int n = *chess1 % 10 - 1;
                                                            if (n == -1) {
                                                                n = 9;
                                                            }
                                                            loadimage(&testchess[n], chesssign[n], 60, 60);
                                                            loadimage(&testchess_mask[n], chesssign_mask[n], 60, 60);
                                                            putimage(1160, 70, &baiban);
                                                            putimage(1160, 70, &testchess_mask[n], SRCAND);
                                                            putimage(1160, 70, &testchess[n], SRCPAINT);
                                                            loadimage(&testchess_n[n], chesssign[n], piece_size, piece_size);
                                                            loadimage(&testchess_n_mask[n], chesssign_mask[n], piece_size, piece_size);
                                                            luoziw = false;
                                                        }
                                                    }

                                                    for (int i = 0; i < 10; i++) {
                                                        // ��GBC��Ĺ�ز���
                                                        if (m.x > startX2 + i * gap - 12 && m.x < startX2 + i * gap + 20 && m.y > startY1 && m.y < startY1 + 24 && flag % 2 == 1) {
                                                            cout << "����GBCĹ�ص�" << i + 1 << "��λ��" << endl;
                                                            getchess(i);
                                                            *chess = *chess1;
                                                            int n = *chess1 % 10 - 1;
                                                            if (n == -1) {
                                                                n = 9;
                                                            }
                                                            loadimage(&testchess[n], chesssign[n], 60, 60);
                                                            loadimage(&testchess_mask[n], chesssign_mask[n], 60, 60);
                                                            putimage(1160, 70, &baiban);
                                                            putimage(1160, 70, &testchess_mask[n], SRCAND);
                                                            putimage(1160, 70, &testchess[n], SRCPAINT);
                                                            loadimage(&testchess_n[n], chesssign[n], piece_size, piece_size);
                                                            loadimage(&testchess_n_mask[n], chesssign_mask[n], piece_size, piece_size);
                                                            luoziw = false;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                        continue;
                                    }
                                    flag++;
                                }
                                else {
                                    cout << "��������������Χ: " << c << endl;
                                }
                            }
                        }
                    }
                }
            }
        }
        EndBatchDraw(); // ������������
    }

    delete chess; // �ͷŶ�̬������ڴ�
}

//��Ϸ��Դ�ͷ�
void finalizeGameView() {
    // �ͷŵ����Ч��������Ч
    freeSoundEffect(clickSound);
    freeSoundEffect(click5Sound);
    freeSoundEffect(click2Sound);
    freeSoundEffect(click6Sound);

    // �ر�SDL2_mixer
    Mix_CloseAudio();
    SDL_Quit();
}

//ԭ������
void gameView() {
    initGameView();
    gameLoop();
    finalizeGameView();
}

//����Ϊ��Ĺ����ȡ�����Ӳ�ˢ�½���
int getchess(int i) {
    //����һ���µ�ָ��ռ�
    int* chess = new int;
    //��ָ��ָ���Ԫ�ظ�ֵ
    *chess = 0;
    //�±�
    int index;
    //���ڴ�ӡ����
    int c;
    //�˴�Ϊmygo��Ĺ�ز���
    if (flag % 2 == 0) {
        //��ѡ������Ԫ�ع�0
        for (int j = 0; j < mygo_chess; j++) {
            if (i == j) {
                *chess = mygo_cemetery[j];
                index = i;
                mygo_cemetery[j] = 0;
            }
        }

        //��ѡ�����Ӻ�ߵ�Ԫ��ǰ��һλ
        for (int z = 0; z < mygo_chess; z++) {
            if (z != 0 && z > i) {
                mygo_cemetery[z - 1] = mygo_cemetery[z];
            }
        }
        //�����鳤��-1�����һλ��0
        mygo_cemetery[mygo_chess] = 0;
        mygo_chess--;
        for (int x = 0; x < mygo_chess; x++) {
            int c = mygo_cemetery[x] % 10 - 1;
            if (c == -1) {
                c = 9;
            }
            //���»���Ĺ��������
            loadimage(&testchess_n[c], chesssign[c], piece_size, piece_size);
            loadimage(&testchess_n_mask[c], chesssign_mask[c], piece_size, piece_size);
            putimage(startX1 + i * gap, startY1, &testchess_mask[c], SRCAND);
            putimage(startX1 + i * gap, startY1, &testchess[c], SRCPAINT);
            cout << "������Ĺ��" << endl;
        }
    }
    //�˴�ΪGBC��Ĺ�ز���
    if (flag % 2 == 1) {
        //��ѡ������Ԫ�ع�0
        for (int j = 0; j < GBC_chess; j++) {
            if (i == j) {
                *chess = GBC_cemetery[j];
                index = i;
                GBC_cemetery[j] = 0;
            }
        }
        //��ѡ�����Ӻ�ߵ�Ԫ��ǰ��һλ
        for (int z = 0; z < GBC_chess; z++) {
            if (z != 0 && z > i) {
                GBC_cemetery[z - 1] = GBC_cemetery[z];
            }
        }
        //�����鳤��-1�����һλ��0
        GBC_cemetery[GBC_chess] = 0;
        GBC_chess--;
        for (int x = 0; x < GBC_chess; x++) {
            int c = GBC_cemetery[x] % 10 - 1;
            if (c == -1) {
                c = 9;
            }
            //���»���Ĺ��������
            loadimage(&testchess_n[c], chesssign[c], piece_size, piece_size);
            loadimage(&testchess_n_mask[c], chesssign_mask[c], piece_size, piece_size);
            putimage(startX2 + i * gap, startY2, &testchess_mask[c], SRCAND);
            putimage(startX2 + i * gap, startY2, &testchess[c], SRCPAINT);
            cout << "������Ĺ��" << endl;
        }
    }
    //ˢ������
    cleardevice();
    putimage(0, 0, &cqp_n); // ���»��Ʊ���
    drawCemetery();// ����Ĺ��
    for (int k = 0; k < 19; k++) {
        for (int l = 0; l < 19; l++) {
            if (map[k][l] != 0) {
                int center_x = get_center_position(board_start_x, k, cell_size);
                int center_y = get_center_position(board_start_y, l, cell_size);
                int c = map[k][l] % 10 - 1;
                if (c == -1) {
                    c = 9;
                }
                putimage(center_x - piece_half_size, center_y - piece_half_size - l, &testchess_n_mask[c], SRCAND);
                putimage(center_x - piece_half_size, center_y - piece_half_size - l, &testchess_n[c], SRCPAINT);
            }
        }
    }
    //����һ��ָ��ָ��Ԫ�ص�ֵ
    *chess1 = *chess;
    return *chess;
}



//��������-----------------------------------------------------------------------------
/*
    iscontinuous()
    ������:hanmi 
    ����:  �������������������ӵı�ʾֵ �ж������Ƿ��������������������
    ����:
        void
    ����ֵ:
        0��ʾ����������
        3��ʾ������3����ͬ����
        4��ʾ������4����ͬ����
        5��ʾ������5����ͬ����
*/

// �����ֺ���
int iscontinuous(int x, int y) {
    int count = 0;
    int PieceType = map[x][y];
    int index_x[3];
    int index_y[3];

    // ���ˮƽ�����Ƿ������
    for (int i = x - 2, j = y; i <= x + 2; i++) {
        if (i >= 0 && i < 19 && map[i][j] == PieceType && map[i][j] != 0) {
            index_x[count] = i;
            index_y[count] = j;
            count++;

            if (count == 3) {
                // ������һ�����������Ӵ���Ĺ��

                for (int p = 0; p < count; p++) {
                    map[index_x[p]][index_y[p]] = 0;
                }
                return count;
            }
        }
        else {
            count = 0;
        }
    }

    count = 0;
    // ��鴹ֱ�����Ƿ������
    for (int i = x, j = y - 2; j <= y + 2; j++) {
        if (j >= 0 && j < 19 && map[i][j] == PieceType && map[i][j] != 0) {
            index_x[count] = i;
            index_y[count] = j;
            count++;

            if (count == 3) {
                // ������һ�����������Ӵ���Ĺ��

                for (int p = 0; p < count; p++) {
                    map[index_x[p]][index_y[p]] = 0;
                }
                return count;
            }
        }
        else {
            count = 0;
        }
    }

    count = 0;
    // ������Խ��߷����Ƿ������
    for (int i = x - 2, j = y + 2; i <= x + 2 && j >= y - 2; i++, j--) {
        if (i >= 0 && i < 19 && j >= 0 && j < 19 && map[i][j] == PieceType && map[i][j] != 0) {
            index_x[count] = i;
            index_y[count] = j;
            count++;

            if (count == 3) {
                // ������һ�����������Ӵ���Ĺ��

                for (int p = 0; p < count; p++) {
                    map[index_x[p]][index_y[p]] = 0;
                }
                return count;
            }
        }
        else {
            count = 0;
        }
    }

    count = 0;
    // ��鸱�Խ��߷����Ƿ������
    for (int i = x - 2, j = y - 2; i <= x + 2 && j <= y + 2; i++, j++) {
        if (i >= 0 && i < 19 && j >= 0 && j < 19 && map[i][j] == PieceType && map[i][j] != 0) {
            index_x[count] = i;
            index_y[count] = j;
            count++;

            if (count == 3) {
                // ������һ�����������Ӵ���Ĺ��

                for (int p = 0; p < count; p++) {
                    map[index_x[p]][index_y[p]] = 0;
                }
                return count;
            }
        }
        else {
            count = 0;
        }
    }

    return 0;
}

// ����������������λ��
int get_center_position(int start, int index, int cell_size) {
    return start + index * cell_size;
}

// ��ȡ��������
int GetRandomNum(int min, int max) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(min, max);
    return dis(gen);
}

/*
    ishasdown()
    ������:����
    ����: �жϵ�����Ƿ�Ϊ�Ϸ����ӵ�
    ����:
        void
    ����ֵ:
        0��ʾ���Ϸ� (�����Ѿ�����)
        1��ʾ�Ϸ�
*/
// ����λ���Ƿ��Ѿ�������
int ishasdown(int i, int j) {
    if (map[i][j] == 0) {
        return 1; // ��λ��û������
    }
    else {
        cout << "�˴�������" << endl << map[i][j];
        return 0; // ��λ����������
    }
}

/*
    isWin()
    ������: ����
    ����: �������������������ӵı�ʾֵ �ж������Ƿ��ʤ
    ����:
        void
    ����ֵ:
        0��ʾû�л�ʤ
        1��ʾMygoʤ��
        2��ʾGBCʤ��
*/
// �ж��Ƿ��ʤ����
int isWin(int chess, int x, int y) {
    if (flag % 2 == 0) {
        int count = 0;
        int s[5] = { 0, 0, 0, 0, 0 };
        // �жϺ����Ƿ������������ͬ����
        for (int i = x - 4, j = y; i <= x + 4; i++) {
            if (i >= 0 && i < 19 && s[0] != map[i][j] && s[1] != map[i][j] && s[2] != map[i][j] && s[3] != map[i][j] && s[4] != map[i][j] && map[i][j] <= 5 && map[i][j] != 0) {
                s[count] = map[i][j];
                count++;
            }
            else {
                for (int n = 0; n < 5; n++) {
                    s[n] = 0;
                }
                count = 0;
            }
            if (count == 5) {
                return 1; // Mygo��ʤ
            }
        }
        count = 0;
        int s1[5] = { 0, 0, 0, 0, 0 };
        // �ж������Ƿ������������ͬ����
        for (int i = x, j = y - 4; j <= y + 4; j++) {
            if (j >= 0 && j < 19 && s1[0] != map[i][j] && s1[1] != map[i][j] && s1[2] != map[i][j] && s1[3] != map[i][j] && s1[4] != map[i][j] && map[i][j] <= 5 && map[i][j] != 0) {
                s1[count] = map[i][j];
                count++;
            }
            else {
                for (int n = 0; n < 5; n++) {
                    s1[n] = 0;
                }
                count = 0;
            }
            if (count == 5) {
                return 1; // Mygo��ʤ
            }
        }
        count = 0;
        int s2[5] = { 0, 0, 0, 0, 0 };
        // �ж����Խ����Ƿ������������ͬ����
        for (int i = x - 4, j = y + 4; i <= x + 4 && j >= y - 4; i++, j--) {
            if (i >= 0 && i < 19 && j >= 0 && j < 19 && s2[0] != map[i][j] && s2[1] != map[i][j] && s2[2] != map[i][j] && s2[3] != map[i][j] && s2[4] != map[i][j] && map[i][j] <= 5 && map[i][j] != 0) {
                s2[count] = map[i][j];
                count++;
            }
            else {
                for (int n = 0; n < 5; n++) {
                    s2[n] = 0;
                }
                count = 0;
            }
            if (count == 5) {
                return 1; // Mygo��ʤ
            }
        }
        count = 0;
        int s3[5] = { 0, 0, 0, 0, 0 };
        // �жϸ��Խ����Ƿ������������ͬ����
        for (int i = x - 4, j = y - 4; i <= x + 4 && j <= y + 4; i++, j++) {
            if (i >= 0 && i < 19 && j >= 0 && j < 19 && s3[0] != map[i][j] && s3[1] != map[i][j] && s3[2] != map[i][j] && s3[3] != map[i][j] && s3[4] != map[i][j] && map[i][j] <= 5 && map[i][j] != 0) {
                s3[count] = map[i][j];
                count++;
            }
            else {
                for (int n = 0; n < 5; n++) {
                    s3[n] = 0;
                }
                count = 0;
            }
            if (count == 5) {
                return 1; // Mygo��ʤ
            }
        }
    }
    else if (flag % 2 == 1) {
        int count4 = 0;
        int s4[5] = { 0, 0, 0, 0, 0 };
        // �жϺ����Ƿ������������ͬ����
        for (int i = x - 4, j = y; i <= x + 4; i++) {
            if (i >= 0 && i < 19 && s4[0] != map[i][j] && s4[1] != map[i][j] && s4[2] != map[i][j] && s4[3] != map[i][j] && s4[4] != map[i][j] && map[i][j] > 5 && map[i][j] != 0) {
                s4[count4] = map[i][j];
                count4++;
            }
            else {
                for (int n = 0; n < 5; n++) {
                    s4[n] = 0;
                }
                count4 = 0;
            }
            if (count4 == 5) {
                return 2; // GBC��ʤ
            }
        }
        count4 = 0;
        int s5[5] = { 0, 0, 0, 0, 0 };
        // �ж������Ƿ������������ͬ����
        for (int i = x, j = y - 4; j <= y + 4; j++) {
            if (j >= 0 && j < 19 && s5[0] != map[i][j] && s5[1] != map[i][j] && s5[2] != map[i][j] && s5[3] != map[i][j] && s5[4] != map[i][j] && map[i][j] > 5 && map[i][j] != 0) {
                s5[count4] = map[i][j];
                count4++;
            }
            else {
                for (int n = 0; n < 5; n++) {
                    s5[n] = 0;
                }
                count4 = 0;
            }
            if (count4 == 5) {
                return 2; // GBC��ʤ
            }
        }
        count4 = 0;
        int s6[5] = { 0, 0, 0, 0, 0 };
        // �ж����Խ����Ƿ������������ͬ����
        for (int i = x - 4, j = y + 4; i <= x + 4 && j >= y - 4; i++, j--) {
            if (i >= 0 && i < 19 && j >= 0 && j < 19 && s6[0] != map[i][j] && s6[1] != map[i][j] && s6[2] != map[i][j] && s6[3] != map[i][j] && s6[4] != map[i][j] && map[i][j] > 5 && map[i][j] != 0) {
                s6[count4] = map[i][j];
                count4++;
            }
            else {
                for (int n = 0; n < 5; n++) {
                    s6[n] = 0;
                }
                count4 = 0;
            }
            if (count4 == 5) {
                return 2; // GBC��ʤ
            }
        }
        count4 = 0;
        int s7[5] = { 0, 0, 0, 0, 0 };
        // �жϸ��Խ����Ƿ������������ͬ����
        for (int i = x - 4, j = y - 4; i <= x + 4 && j <= y + 4; i++, j++) {
            if (i >= 0 && i < 19 && j >= 0 && j < 19 && s7[0] != map[i][j] && s7[1] != map[i][j] && s7[2] != map[i][j] && s7[3] != map[i][j] && s7[4] != map[i][j] && map[i][j] > 5 && map[i][j] != 0) {
                s7[count4] = map[i][j];
                count4++;
            }
            else {
                for (int n = 0; n < 5; n++) {
                    s7[n] = 0;
                }
                count4 = 0;
            }
            if (count4 == 5) {
                return 2; // GBC��ʤ
            }
        }
    }
    return 0; // û�л�ʤ
}

/*
    ������:����
    ����:
        ���õ������Ӵ���Ĺ����
    ����:
        Chess
    ����ֵ:
        void
*/
//����Ĺ��
void insertcemetery(int chess) {
    if (mygo_chess < 10) {
        if (chess >= 101 && chess <= 105) {
            // Player A
            mygo_cemetery[mygo_chess] = chess;
            mygo_chess++;
            flag++;
        }
    }

    if (GBC_chess < 10) {
        if (chess >= 106 && chess <= 110) {
            // Player B
            GBC_cemetery[GBC_chess] = chess;
            GBC_chess++;
            flag++;
        }
    }

}

//Ĺ�����ӻ���
void drawCemetery() {
    int startX1 = 50; // Player A ��Ĺ����ʼX����
    int startY1 = 700; // Player A ��Ĺ����ʼY����
    int startX2 = 690; // Player B ��Ĺ����ʼX����
    int startY2 = 700; // Player B ��Ĺ����ʼY����
    int gap = 40; // ����֮��ļ��

    // ���� Player A ��Ĺ��
    for (int i = 0; i < mygo_chess; i++) {
        int c = mygo_cemetery[i] % 10 - 1;
        if (c == -1) c = 9;
        putimage(startX1 + i * gap, startY1, &testchess_n_mask[c], SRCAND);
        putimage(startX1 + i * gap, startY1, &testchess_n[c], SRCPAINT);
    }

    // ���� Player B ��Ĺ��
    for (int i = 0; i < GBC_chess; i++) {
        int c = GBC_cemetery[i] % 10 - 1;
        if (c == -1) c = 9;
        putimage(startX2 + i * gap, startY2, &testchess_n_mask[c], SRCAND);
        putimage(startX2 + i * gap, startY2, &testchess_n[c], SRCPAINT);
    }
}


//����-----------------------------------------------------------------------------

//GBCʤ������
void gbc_win() {
    // ʹ��˫���弼��
    BeginBatchDraw();
    centerWindow();
    // ��֡����ÿ��֡��
    const int total_frames = 2165;
    const double fps = 23.967;
    const int frame_delay = static_cast<int>(1000 / fps); // ÿ֡�ӳ�ʱ��

    // ��ʼ��SDL2_mixer
    if (!initSDL2Mixer()) {
        return;
    }

    Mix_Music* bgMusic = loadMusic("res/bg3.mp3");
    if (bgMusic == nullptr) {
        std::cerr << "�������ּ���ʧ��" << std::endl;
        return;
    }

    Mix_VolumeMusic(bgVolume); // ���ñ�����������
    Mix_PlayMusic(bgMusic, -1); // ѭ�����ű�������

    for (int i = 0; i < total_frames; ++i) {
        stringstream ss;
        ss << "gbccar/GBC" << setw(4) << setfill('0') << i << ".jpg";
        string filename = ss.str();

        // ת���ļ���ΪLPCTSTR����
        wstring wide_filename = wstring(filename.begin(), filename.end());
        LPCTSTR w_filename = wide_filename.c_str();

        // ����
        //cout << "����ͼƬ: " << filename << endl;

        // ����ͼƬ
        IMAGE img;
        loadimage(&img, w_filename);

        // ���ͼƬ�Ƿ���سɹ�
        if (img.getwidth() == 0 || img.getheight() == 0) {
            cout << "ͼƬ����ʧ��: " << filename << endl;
            continue;
        }

        // ��ʾͼƬ
        putimage(0, 0, &img);
        //cout << "��ʾͼƬ: " << filename << endl;

        // ˢ����Ļ
        FlushBatchDraw();

        // �ȴ���һ֡
        Sleep(frame_delay);

        // ���������������⵽����ת��������
        if (MouseHit()) {
            // ��ȡ�����Ϣ
            ExMessage msg = getmessage(EX_MOUSE);
            if (msg.message == WM_LBUTTONDOWN) {
                // ������������
                EndBatchDraw();
                // ����������
                menuView();
                // ֹͣ���ű�������
                Mix_HaltMusic();
                freeMusic(bgMusic);
                return;
            }
        }
    }
    menuView();
    // ������������
    EndBatchDraw();
    // ֹͣ���ű�������
    Mix_HaltMusic();
    freeMusic(bgMusic);
}

//Mygoʤ������
void mygo_win() {
    // ʹ��˫���弼��
    BeginBatchDraw();
    centerWindow();
    // ��֡����ÿ��֡��
    const int total_frames = 2165;
    const double fps = 23.967;
    const int frame_delay = static_cast<int>(1000 / fps); // ÿ֡�ӳ�ʱ��

    // ��ʼ��SDL2_mixer
    if (!initSDL2Mixer()) {
        return;
    }

    Mix_Music* bgMusic = loadMusic("res/bg2.mp3");
    if (bgMusic == nullptr) {
        std::cerr << "�������ּ���ʧ��" << std::endl;
        return;
    }

    Mix_VolumeMusic(bgVolume); // ���ñ�����������
    Mix_PlayMusic(bgMusic, -1); // ѭ�����ű�������

    for (int i = 0; i < total_frames; ++i) {
        stringstream ss;
        ss << "mygocar/Mygo" << setw(4) << setfill('0') << i << ".jpg";
        string filename = ss.str();

        // ת���ļ���ΪLPCTSTR����
        wstring wide_filename = wstring(filename.begin(), filename.end());
        LPCTSTR w_filename = wide_filename.c_str();

        // ����
        //cout << "����ͼƬ: " << filename << endl;

        // ����ͼƬ
        IMAGE img;
        loadimage(&img, w_filename);

        // ���ͼƬ�Ƿ���سɹ�
        if (img.getwidth() == 0 || img.getheight() == 0) {
            cout << "ͼƬ����ʧ��: " << filename << endl;
            continue;
        }

        // ��ʾͼƬ
        putimage(0, 0, &img);
        //cout << "��ʾͼƬ: " << filename << endl;

        // ˢ����Ļ
        FlushBatchDraw();

        // �ȴ���һ֡
        Sleep(frame_delay);

        // ���������������⵽����ת��������
        if (MouseHit()) {
            // ��ȡ�����Ϣ
            ExMessage msg = getmessage(EX_MOUSE);
            if (msg.message == WM_LBUTTONDOWN) {
                // ������������
                EndBatchDraw();
                // ����������
                menuView();
                // ֹͣ���ű�������
                Mix_HaltMusic();
                freeMusic(bgMusic);
                return;
            }
        }
    }
    menuView();
    // ������������
    EndBatchDraw();
    // ֹͣ���ű�������
    Mix_HaltMusic();
    freeMusic(bgMusic);
}

// �������
void winView(int i) {
    if (i == 1) {
        initgraph(1280, 800);
        mygo_win();
    }
    else if (i == 2) {
        initgraph(1280, 800);
        gbc_win();
    }
}

//��ͷ����
void logo() {
    // ʹ��˫���弼��
    BeginBatchDraw();
    centerWindow();
    // ��֡����ÿ��֡��
    const int total_frames = 445;
    const double fps = 45;
    const int frame_delay = static_cast<int>(1000 / fps); // ÿ֡�ӳ�ʱ��

    

    for (int i = 0; i < total_frames; ++i) {
        stringstream ss;
        ss << "logo/Logo" << setw(4) << setfill('0') << i << ".jpg";
        string filename = ss.str();

        // ת���ļ���ΪLPCTSTR����
        wstring wide_filename = wstring(filename.begin(), filename.end());
        LPCTSTR w_filename = wide_filename.c_str();

        // ����
        //cout << "����ͼƬ: " << filename << endl;

        // ����ͼƬ
        IMAGE img;
        loadimage(&img, w_filename);

        // ���ͼƬ�Ƿ���سɹ�
        if (img.getwidth() == 0 || img.getheight() == 0) {
            cout << "ͼƬ����ʧ��: " << filename << endl;
            continue;
        }

        // ��ʾͼƬ
        putimage(0, 0, &img);
        //cout << "��ʾͼƬ: " << filename << endl;

        // ˢ����Ļ
        FlushBatchDraw();

        // �ȴ���һ֡
        Sleep(frame_delay);

        // ���������������⵽����ת��������
        if (MouseHit()) {
            // ��ȡ�����Ϣ
            ExMessage msg = getmessage(EX_MOUSE);
            if (msg.message == WM_LBUTTONDOWN) {
                // ������������
                EndBatchDraw();
                // ����������
                menuView();                              
                return;
            }
        }
    }
    menuView();
    // ������������
    EndBatchDraw();
}

// ������Ļ
void displayScrollingCredits() {
    // ��ʼ��ͼ�δ��ڣ����÷ֱ���Ϊ1280x800
    initgraph(1280, 800);
    // ���ô��ھ��У��˹��ܿ�����Ҫ����ʵ�֣�����ͼ�οⲻͬ��
    // centerWindow();
    // ����˫�����ͼģʽ
    BeginBatchDraw();

    // ��Ļ��ʼλ��
    int yOffset = 800;

    // ��ѭ��
    while (true) {
        // ����
        cleardevice();

        // �����������ɫ
        settextstyle(30, 0, _T("Arial"));
        settextcolor(WHITE);

        // ����ÿ�����ֵĸ߶�
        int lineHeight = textheight(_T("A"));

        // �����о�
        int lineSpacing = lineHeight + 20;

        // ������Ļ
        for (size_t i = 0; i < credits.size(); ++i) {
            int yPosition = yOffset + i * lineSpacing;
            int textWidth = textwidth(credits[i]);
            int xPosition = (getmaxx() - textWidth) / 2; // �������λ��
            outtextxy(xPosition, yPosition, credits[i]);
        }

        // ����λ��
        yOffset -= 2; // ÿ�θ��������ƶ�2������

        // ѭ����Ļ
        if (yOffset + credits.size() * lineSpacing < 0) {
            yOffset = 800;
        }

        // ����û�����
        if (_kbhit()) {
            char ch = _getch();
            if (ch == 27) { // ���� ESC ��
                break;
            }
        }
        // ��������
        if (MouseHit()) {
            MOUSEMSG msg = GetMouseMsg();
            if (msg.uMsg == WM_LBUTTONDOWN) {
                break;
            }
        }

        // ��ʱ�����ٶ�
        Sleep(30);

        // ��ʾ����
        FlushBatchDraw();
    }

    // �ر�ͼ�δ���
    closegraph();

    // ���� setView ����
    setView();
}


//��ʼ-----------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    initgraph(1280, 800); // ��ʼ��ͼ�δ���
    
    logo();
    //����
    //gbc_win();
    //mygo_win();
    return 0;
}
