#include <iostream>
#include <SFML/Graphics.hpp>
#include <windows.h>
#include <string>

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

    DWORD scoreboard = 0x810933F0;

    DWORD address = 0x803A125C; // This is the address that we want to read from
    int value = 0; // This will store our value. In my case, its an integer, which is the timer
    DWORD pid; //This will store our Process ID, used to read/write into the memory
    HWND hwnd = 0; //Finally a handle to our window

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
