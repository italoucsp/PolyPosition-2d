#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;
//VARIABLES DE LAS QUE CONSTA LA PISTA
int const widthscreen = 1024;
int const heightscreen = 768;
int width_seg = 200;
int pista = 2000;
//DISEÑO
float camd = 0.84;

class ejeH {

public:
    float x, y, z;
    float x1,y1,w;
    float scale,curve;
    ejeH()
    {
        curve = 0;
        x = 0;
        y = 0;
        z = 0;
    }

    void cordenates_Scre(int camx , int camy , int camz)
    {
        scale = camd/(z - camz);
        x1 = (1 + scale*(x - camx)) * widthscreen/2;
        y1 = (1 - scale*(y - camy)) * heightscreen/2;
        w =  scale * pista * widthscreen/2;
    }

};

void DrawObj(sf::RenderWindow &app,sf::Texture &texture,int posx,int posy){
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setPosition(posx, posy);
    app.draw(sprite);
}
void draw_race(sf::RenderWindow &w,  sf::Color c, int x1, int y1, int wid1, int x2, int y2, int wid2)
{
    sf::ConvexShape shape(4);
    shape.setFillColor(c);
    shape.setPoint(0, sf::Vector2f(x1 - wid1, y1));
    shape.setPoint(1, sf::Vector2f(x2 - wid2, y2));
    shape.setPoint(2, sf::Vector2f(x2 + wid2, y2));
    shape.setPoint(3, sf::Vector2f(x1 + wid1, y1));
    w.draw(shape);
}

class Game{
public:
        int pos = 0,FPS = 100;
        int posx = 0;
        int aceleracion = 0;
        int freno = 1;
        int cont = -1;

        sf::Texture arbol;

    int posxmove(vector<ejeH> lines){
        if(aceleracion!=0){
            cont ++;
            if(lines[cont].curve > 0.0)return -13;
            if(lines[cont].curve < 0.0)return 13;
            return 0;
        }
    }
    void Controls(sf::RenderWindow &app){
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) app.close();
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) aceleracion-=freno;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {if(aceleracion!=0)posx+=22;}
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {if(aceleracion!=0)posx-=22;}
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) aceleracion = 200;
    }

    void Run(sf::RenderWindow &app,int N,vector<ejeH> lines,vector<ejeH> clips){

        //Objetos que iran apareciendo
        arbol.loadFromFile("Image/prueba.png");

        while(app.isOpen())
        {
            app.setFramerateLimit(FPS);

            if (aceleracion <= 0)freno = 0;
            else freno = 2;
            pos += aceleracion; posx += posxmove(lines);

            sf::Event e;
            while(app.pollEvent(e))
            {
                if(e.type == sf::Event:: Closed)
                    app.close();
            }
            Controls(app);
            app.clear();
            int star_pos = pos/width_seg;
            float x = 0,dx=0;

            app.clear(sf::Color(0, 185, 179));

            for( int n = star_pos;n  < star_pos + 300; n++ )
            {
                ejeH &l = lines[n%N], &cl =clips[n%N];
                l.cordenates_Scre(posx - x,1500,pos); cl.cordenates_Scre(posx - x,1500,pos);
                x+=dx;
                dx+=l.curve;

                sf::Color grass = (n/3)%2 ? sf::Color(16,200,16) : sf::Color (0,154,0);
                sf::Color rumble = (n/3)%2 ? sf::Color(255,255,255) : sf::Color  (255,0,0);
                sf::Color road = (n/3)%2 ? sf::Color(93,93,93) : sf::Color (78,78,78);
                sf::Color lad = (n/3)%2 ? sf::Color(255,255,255) : sf::Color (78,78,78);

                ejeH p = lines[(n - 1)% N];

                draw_race(app, grass,0,p.y1,widthscreen*2,0, l.y1,widthscreen);
                draw_race(app, rumble, p.x1,p.y1,p.w*1.2,l.x1,l.y1,l.w*1.2);
                draw_race(app, road,p.x1,p.y1 , p.w , l.x1,l.y1,l.w);
                draw_race(app, lad,p.x1,p.y1 , p.w/18 , l.x1,l.y1,l.w/18);

                //if(p.x1>posx && posx<p.x1+p.w*1.2)aceleracion = 100;

                ejeH cli = clips[(n - 1)% N];

                DrawObj(app,arbol,cli.x1,cl.y1);

            }
            app.display();
        }
    }
};
int main()
{
    sf::RenderWindow app(sf::VideoMode(widthscreen, heightscreen), "POOL POSITION");

    vector<ejeH> lines;
    vector<ejeH> clips;
    for(int i = 0; i < 3200; i++)
    {
        ejeH line;
        ejeH clip;
        line.z = clip.z = i * width_seg;

        if(i > 500 && i < 700) line.curve = 1.5;
        if(i > 1000 && i < 1400) line.curve = -2.5;
        if(i>1600)line.curve = 0.7;

        clips.push_back(clip);
        lines.push_back(line);
    }
    int N = lines.size();
    Game Poly;
    Poly.Run(app,N,lines,clips);
    return 0;
}

