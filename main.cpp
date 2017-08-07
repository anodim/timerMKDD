//image
#include <iostream>
#include <SFML/Graphics.hpp>
#include <windows.h>
#include <string>
#include <fstream>

#define FONT_TITLE_SIZE 40
#define FONT_SPLIT_SIZE 27

#define IMG_SIZE 0.90f

using namespace std;
using namespace sf;

/* =================== GLOBAL =================== */
Texture img;
Sprite img_s;
Font arial;

Text txt("Total -- 00:00:000",arial,FONT_TITLE_SIZE);
Text txt("PB    -- 00:00:000",arial,FONT_TITLE_SIZE);

Text txtCircuit[16];
Text txtDelta[16];
Text txtTempsCircuit[16];
Text txtSomme("+00:00:000",arial,FONT_SPLIT_SIZE);

int pb[17];
int newPb[17];
int win_largeur;
int win_hauteur;
RenderWindow *win;

DWORD tpsTotalGC;
DWORD scoreboard;
DWORD circuit[16];
DWORD address;

DWORD pid;
HWND hwnd;
HANDLE phandle;

int value;
int fait[16];
int somme_pb;
int nbFait;

int minute;
int sec;
int milli;

char buff[100];

ifstream fichier("pbMKDD.txt");
/* ============================================== */

/* ================== FONCTION ================== */
void littleBigEndian (int &x)
{
    x = ((x >> 24) & 0xffL) | ((x >> 8) & 0xff00L) | ((x << 8) & 0xff0000L)
        | ((x << 24) & 0xff000000L);
}

void init()
{
    // Chargement de l'image
    if(img.loadFromFile("img.png"))
    {
      //img non charger
    }
    img_s.setTexture(img);
    img_s.setScale(IMG_SIZE,IMG_SIZE);
    img_s.setPosition(0,FONT_TITLE_SIZE+FONT_SPLIT_SIZE+15+FONT_SPLIT_SIZE*17); // maj de la pos vertical pour calc de la hauteur de la fenetre

    arial.loadFromFile("../../Font/pixelade.TTF"); //chargemet, de la font

    int imageMax = img.getSize().x/16*IMG_SIZE+30; // calc de la largeur pour le txt
    int textMax = 22*FONT_SPLIT_SIZE*0.45+30; // calc de la largeur pour l'image

    win_largeur = (textMax > imageMax)?textMax:imageMax; // maj de la largeur max
    win_hauteur = img_s.getGlobalBounds().top+img_s.getGlobalBounds().height;

    win = new RenderWindow(VideoMode(win_largeur,win_hauteur+10),"Ano\'s auto-spliter"); // ouverture de la fenetre

    //win->setPosition(Vector2i(1920,0));// a virer <----------------------------------------------------------------------------------------
    win->setFramerateLimit(30); // reduit le framerate, augmentation de perf

    txt.setPosition((win_largeur/2)-(txt.getGlobalBounds().width/2),0); // positionnement du total

    img_s.setPosition((win_largeur/2)-(img_s.getGlobalBounds().width/16)/2,FONT_TITLE_SIZE+FONT_SPLIT_SIZE+15+FONT_SPLIT_SIZE*17); // positionnement definitif de l'image
    img_s.setTextureRect(IntRect(0,0,img_s.getLocalBounds().width/16,img_s.getLocalBounds().height)); // mise en place du masque

    if(fichier)
    {
        string ligne;

        for(int i=0; i<17; i++)
        {
            getline(fichier,ligne);
            pb[i] = atoi(ligne.c_str());
        }
    }
    else
    {
        ofstream file("log.txt",ios_base::app);
        file << "fichier non trouver" << endl;

        for(int i=0; i<17; i++)
        {
            ofstream file2("pbMKDD.txt",ios_base::app);
            file2 << "5999998" << endl;
            pb[i] = 5999998;
        }
    }

     for(int i=0; i<16; i++)
    {
        txtCircuit[i].setCharacterSize(FONT_SPLIT_SIZE);
        txtCircuit[i].setFont(arial);
        txtCircuit[i].setPosition(30,FONT_TITLE_SIZE+FONT_SPLIT_SIZE+i*FONT_SPLIT_SIZE+15);

        txtTempsCircuit[i].setCharacterSize(FONT_SPLIT_SIZE);
        txtTempsCircuit[i].setFont(arial);
        txtTempsCircuit[i].setPosition(30+40,FONT_TITLE_SIZE+FONT_SPLIT_SIZE+i*FONT_SPLIT_SIZE+15);
        txtTempsCircuit[i].setString(" -- ");

        txtDelta[i].setCharacterSize(FONT_SPLIT_SIZE);
        txtDelta[i].setFont(arial);
        txtDelta[i].setPosition(175,FONT_TITLE_SIZE+FONT_SPLIT_SIZE+i*FONT_SPLIT_SIZE+15);
        txtDelta[i].setString("");
    }

    txtSomme.setPosition((win_largeur/2)-(txtSomme.getLocalBounds().width/2),FONT_TITLE_SIZE);
    txtSomme.setString("");

    txtCircuit[0].setString("LC");
    txtCircuit[1].setString("PB");
    txtCircuit[2].setString("BP");
    txtCircuit[3].setString("DDD");
    txtCircuit[4].setString("MB");
    txtCircuit[5].setString("MaC");
    txtCircuit[6].setString("DC");
    txtCircuit[7].setString("WS");
    txtCircuit[8].setString("SL");
    txtCircuit[9].setString("MuC");
    txtCircuit[10].setString("YC");
    txtCircuit[11].setString("DKM");
    txtCircuit[12].setString("WC");
    txtCircuit[13].setString("DDJ");
    txtCircuit[14].setString("BC");
    txtCircuit[15].setString("RR");

    DWORD luigi_circuit = 0x8113F248;
    DWORD peach_beach = 0x8114B748;
    DWORD baby_park = 0x8112D848;
    DWORD dry_dry_desert = 0x811449E8;
    DWORD mushroom_bridge = 0x8114B3C8;
    DWORD mario_circuit = 0x81144538;
    DWORD daisy_cruiser = 0x8114D288;
    DWORD waluigi_stadium = 0x8114D868;
    DWORD sherbet_land = 0x8113AD88;
    DWORD mushroom_city = 0x81145EE8;
    DWORD yoshi_circuit = 0x81142028;
    DWORD dk_mountain = 0x8114B4E0;
    DWORD wario_colosseum = 0x8113DF08;
    DWORD dino_dino_jungle = 0x8113C9E8;
    DWORD bowser_castle = 0x81144428;
    DWORD rainbow_road = 0x810F5F08;

    circuit[0] = luigi_circuit;
    circuit[1] = peach_beach;
    circuit[2] = baby_park;
    circuit[3] = dry_dry_desert;
    circuit[4] = mushroom_bridge;
    circuit[5] = mario_circuit;
    circuit[6] = daisy_cruiser;
    circuit[7] = waluigi_stadium;
    circuit[8] = sherbet_land;
    circuit[9] = mushroom_city;
    circuit[10] = yoshi_circuit;
    circuit[11] = dk_mountain;
    circuit[12] = wario_colosseum;
    circuit[13] = dino_dino_jungle;
    circuit[14] = bowser_castle;
    circuit[15] = rainbow_road;

    tpsTotalGC = 0x803A125C;
    scoreboard = 0x810933F0;

    address = tpsTotalGC;
    value = 0;
    hwnd = 0; // handle

    for(int i=0; i<16; i++)
        fait[i] = 0;

    somme_pb = 0;
    nbFait = 0;
}

void reset()
{
    for(int i=0; i<16; i++)
    {
        fait[i] = 0;
        txtDelta[i].setString("");
        txtTempsCircuit[i].setString(" -- ");
    }
    txt.setString("Total -- 00:00:000");
    txt.setFillColor(Color(255,255,255));
    somme_pb = 0;
    txtSomme.setString("");
    nbFait = 0;
}

void readMemory(int &v)
{
    ReadProcessMemory(phandle,(void*)address,&v,sizeof(value),0); //lecture mem
    littleBigEndian(v); // on remet les valeurs dans le bon sens ;)
}

/* ============================================== */

int main()
{
    init();

    while(win->isOpen()) // on quitte le prg quand on ferme la fenetre
    {
        Event event;
        while(win->pollEvent(event)) // boucle des events
        {
            if(Keyboard::isKeyPressed(Keyboard::Escape))
                win->close();
            if(event.type == Event::Closed)
                win->close();
        }

        minute = 0;
        sec = 0;
        milli = 0;

        // on trouve dolphin lorsqu'il s'ouvre
        if(!hwnd)
        {
            hwnd = FindWindow(NULL,"Dolphin 5.0");
        }

        // save id du process + access read
        phandle = 0;
        if(!phandle)
        {
            GetWindowThreadProcessId(hwnd,&pid); // get process id
            phandle = OpenProcess(PROCESS_VM_READ,0,pid); // Get permission to read
        }

        address = scoreboard;
        readMemory(value);

        int scoreboard_v = value;

        sprintf(buff,"%d",value); // code pouris pour corriger un bug

        if(scoreboard_v < 0) // scorboard_v est negatif lors du menu
        {
            reset();
        }
        else
        {
            for(int i=0; i<16; i++)
            {
                address = circuit[i];
                readMemory(value);

                int value2;
                if(fait[i] != 1)
                {
                    address = circuit[i] + 0x20;

                    readMemory(value2);
                    if(value2 == scoreboard_v)
                    {
                        value = value2;
                    }
                }

                if(value == scoreboard_v && fait[i] != 1 && scoreboard_v != 0)
                {
                    fait[i] = 1;
                    nbFait++;

                    img_s.setTextureRect(IntRect(img_s.getLocalBounds().width*nbFait,0,img_s.getLocalBounds().width,img_s.getLocalBounds().height));

                    newPb[i] = value;
                    minute = (value/1000)/60;
                    sec = (value/1000)%60;
                    milli = value - (value/1000)*1000;

                    sprintf(buff,"%s %02d:%02d:%03d",((string)txtTempsCircuit[i].getString()).c_str(),minute,sec,milli);
                    txtTempsCircuit[i].setString(buff);

                    txtDelta[i].setPosition(txtTempsCircuit[i].getGlobalBounds().left+txtTempsCircuit[i].getGlobalBounds().width+15
                                            ,FONT_TITLE_SIZE+FONT_SPLIT_SIZE+i*FONT_SPLIT_SIZE+15);

                    if(pb[i] != 5999998)
                    {
                        int delta = value - pb[i];

                        somme_pb += delta;

                        minute = abs((somme_pb/1000)/60);
                        sec = abs((somme_pb/1000)%60);
                        milli = abs(somme_pb - (somme_pb/1000)*1000);

                        if(somme_pb > 0)
                        {
                            txtSomme.setFillColor(Color(255,0,0));
                            sprintf(buff,"+%02d:%02d:%03d",minute,sec,milli);
                        }
                        else
                        {
                            txtSomme.setFillColor(Color(0,255,0));
                            sprintf(buff,"-%02d:%02d:%03d",minute,sec,milli);
                        }

                        txtSomme.setString(buff);

                        minute = abs((delta/1000)/60);
                        sec = abs((delta/1000)%60);

                        if(delta > 0)
                        {
                            txtDelta[i].setFillColor(Color(255,0,0));
                            sprintf(buff,"+%02d:%02d",minute,sec);
                        }
                        else
                        {
                            txtDelta[i].setFillColor(Color(0,255,0));
                            sprintf(buff,"-%02d:%02d",minute,sec);
                        }

                        txtDelta[i].setString(buff);
                    }
                    else
                    {
                        txtDelta[i].setFillColor(Color(216,175,31));
                        sprintf(buff,"%02d:%02d",minute,sec);
                        txtDelta[i].setString(buff);
                    }

                }
            }

            address = tpsTotalGC;
            readMemory(value);

            minute = (value/1000)/60;
            sec = (value/1000)%60;
            milli = value - (value/1000)*1000;

            if(fait[15] == 1) // fin
            {
                int tmp = 0;
                for(int i=0; i<16; i++)
                {
                    tmp += newPb[i];
                }
                newPb[16] = value;
                if(tmp == newPb[16])
                {
                    txt.setFillColor(Color(255,0,0));
                    if(pb[16]>newPb[16])
                    {
                        txt.setFillColor(Color(0,255,0));
                        ofstream file("pbMKDD.txt",ios::out | ios::trunc);
                        for(int i=0; i<17; i++)
                        {
                            // ecrire nouveau fichier
                            file << newPb[i] << endl;
                        }
                    }
                }
            }

            char buff[100];

            sprintf(buff,"Total -- %02d:%02d:%03d",minute,sec,milli);

            txt.setString(buff);
        }

        win->clear();
        win->draw(img_s);
        win->draw(txt);
        win->draw(txtSomme);
        for(int i=0; i<16; i++)
        {
            win->draw(txtCircuit[i]);
            win->draw(txtTempsCircuit[i]);
            win->draw(txtDelta[i]);
        }
        win->display();
    }

    delete(win);

    return 0;
}
