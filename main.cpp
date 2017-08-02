#include <iostream>
#include <SFML/Graphics.hpp>
#include <windows.h>
#include <string>
#include <fstream>

void littleBigEndian (int &x)
{
    x = ((x >> 24) & 0xffL) | ((x >> 8) & 0xff00L) | ((x << 8) & 0xff0000L)
        | ((x << 24) & 0xff000000L);
}

using namespace std;
using namespace sf;

int main()
{
    char buff[100];
    RenderWindow win(VideoMode(260,600),"Ano\'s auto-spliter");

    win.setFramerateLimit(20);

    Font arial;
    arial.loadFromFile("../../Font/cour.ttf");
    Text txt(" Temps totals    -- 00 : 00 : 000",arial,22);
    txt.setPosition(0,22);
    Text txtCircuit[16];
    Text txtDelta[16];
    Text txtSomme("",arial,22);

    for(int i=0; i<16; i++)
    {
        txtCircuit[i].setCharacterSize(22);
        txtCircuit[i].setFont(arial);
        txtCircuit[i].setPosition(0,(3+i)*22);

        txtDelta[i].setCharacterSize(22);
        txtDelta[i].setFont(arial);
        txtDelta[i].setPosition(180,(3+i)*22);
    }

    txtSomme.setPosition(180,44);

    txtCircuit[0].setString(" LC  --");
    txtCircuit[1].setString(" PB  --");
    txtCircuit[2].setString(" BP  --");
    txtCircuit[3].setString(" DDD --");
    txtCircuit[4].setString(" MB  --");
    txtCircuit[5].setString(" MaC --");
    txtCircuit[6].setString(" DC  --");
    txtCircuit[7].setString(" WS  --");
    txtCircuit[8].setString(" SL  --");
    txtCircuit[9].setString(" MuC --");
    txtCircuit[10].setString(" YC  --");
    txtCircuit[11].setString(" DKM --");
    txtCircuit[12].setString(" WC  --");
    txtCircuit[13].setString(" DDJ --");
    txtCircuit[14].setString(" BC  --");
    txtCircuit[15].setString(" RR  --");

    DWORD tpsTotalGC = 0x803A125C;
    DWORD scoreboard = 0x810933F0;

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

    DWORD circuit[16];

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

    DWORD address = 0x803A125C; // This is the address that we want to read from
    int value = 0; // This will store our value. In my case, its an integer, which is the timer
    DWORD pid; //This will store our Process ID, used to read/write into the memory
    HWND hwnd = 0; //Finally a handle to our window

    int fait[16];
    for(int i=0; i<16; i++)
        fait[i] = 0;

    int somme_pb = 0;
    while(win.isOpen())
    {
        Event event;
        while(win.pollEvent(event))
        {
            if(Keyboard::isKeyPressed(Keyboard::Escape))
                win.close();
            if(event.type == Event::Closed)
                win.close();
        }

        hwnd = FindWindow(NULL,"Dolphin 5.0");

        sprintf(buff," proc find : %d",hwnd); // pareil
        txt.setString(buff);

        HANDLE phandle = 0;

        GetWindowThreadProcessId(hwnd,&pid); //Get the process id and place it in pid

        sprintf(buff," pid find : %d",pid); // pareil
        txtCircuit[0].setString(buff);

        phandle = OpenProcess(PROCESS_VM_READ,0,pid); //Get permission to read
        sprintf(buff," phandle : %d",phandle); // pareil
        txtCircuit[1].setString(buff);


        address = scoreboard;
        ReadProcessMemory(phandle,(void*)address,&value,sizeof(value),0);
        littleBigEndian(value);
        sprintf(buff," mem? : %d",value); // pareil

        txtCircuit[2].setString(buff);
        address = 0x80378000;
        ReadProcessMemory(phandle,(void*)address,&value,sizeof(value),0);

        sprintf(buff," adress mk : %d",value); // pareil

        txtCircuit[3].setString(buff);

        sprintf(buff," error : %d",GetLastError()); // pareil

        txtCircuit[4].setString(buff);

        littleBigEndian(value);
        int scoreboard_v = value;

        sprintf(buff,"%d",value); // pareil

        for(int i=0; i<16; i++)
        {
            address = circuit[i];
            ReadProcessMemory(phandle,(void*)address,&value,sizeof(value),0);
            littleBigEndian(value);

            int value2;
            if(fait[i] != 1)
            {
                address = circuit[i] + 0x20;
                ReadProcessMemory(phandle,(void*)address,&value2,sizeof(value2),0);
                littleBigEndian(value2);
                if(value2 == scoreboard_v)
                {
                    value = value2;
                }

            }
        }


        win.clear();
        win.draw(txt);
        win.draw(txtSomme);
        for(int i=0; i<5; i++)
        {
            win.draw(txtCircuit[i]);
            //win.draw(txtDelta[i]);
        }
        win.display();
    }

    return 0;
}
