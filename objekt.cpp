#include "objekt.h"
#include "space.h"

Objekt::Objekt(sf::RenderWindow *App, Space* s, int pozice_x, int pozice_y, Objekt *vlastnik){
	this->pozice_x = pozice_x;
	this->pozice_y = pozice_y;
	sirka = 0;
	vyska = 0;
	this->app = App;
	space = s;
	pocitadlo = 0;
	this->vlastnik = vlastnik;
}
Objekt::Objekt(const Objekt &o){
	pozice_x = o.pozice_x;
	pozice_y = o.pozice_y;
	sirka = o.sirka;
	vyska = o.vyska;
	Image = o.Image;
	sprite = o.sprite;
	app = o.app;
	space = o.space;
}
void Objekt::nastavPozici(int x, int y){
	pozice_x = x;
	pozice_y = y;
}
void Objekt::Draw(){
	sprite.SetPosition((float)pozice_x, (float)pozice_y);
	this->app->Draw(sprite);
	//std::cout << "vykresluji na pozici: " << pozice_x << ":" << pozice_y << std::endl;
}
sf::RenderWindow * Objekt::getApp(){
	return app;
}
sf::Sprite Objekt::getSprite(){
	return sprite;
}
int Objekt::getPoziceX(){
	return pozice_x;
}
int Objekt::getPoziceY(){
	return pozice_y;
}
void Objekt::posunLeft(){
	if(pozice_x > 0)
		pozice_x -= 1;
}
void Objekt::posunRight(){
	if(pozice_x < (int)app->GetWidth() - sirka)
		pozice_x += 1;
}
void Objekt::posunUp(){
	if(pozice_y > 0)
		pozice_y -= 1;
}
void Objekt::posunDown(){
	//if(pozice_y < (int)app->GetHeight() - vyska)
		pozice_y += 1;
}
/*************************************/
void Objekt::posunLeftD(){
	if(pozice_x > 0)
		pozice_x -= 2;
}
void Objekt::posunRightD(){
	if(pozice_x < (int)app->GetWidth() - sirka)
		pozice_x += 2;
}
void Objekt::posunUpD(){
	if(pozice_y > 0)
		pozice_y -= 2;
}
void Objekt::posunDownD(){
	if(pozice_y < (int)app->GetHeight() - vyska)
		pozice_y += 2;
}
/***************************************/
void Objekt::posun(int x, int y){
	pozice_x += x;
	pozice_y += y;
}
void Objekt::otoc(){
	sprite.SetRotation(180);
}
void Objekt::vystrel(){
	if(pocitadlo == 0)
		space->addController(new ControllerStrely(new Strela(app, space, pozice_x + sirka/2, pozice_y + 5, this)));
	pocitadlo++;
	if(pocitadlo == 10)
		pocitadlo = 0;
}

Lod::Lod(sf::RenderWindow *App, Space* s, int pozice_x, int pozice_y, Objekt *vlastnik) : Objekt(App, s, pozice_x, pozice_y){
	Image.LoadFromFile("raketa.tga");
	sprite.SetImage(Image);
	//sprite.SetColor(sf::Color(255,255,255,128));
	vyska = Image.GetHeight();
	sirka = Image.GetWidth();
	//std::cout << "vytvoreni objektu Lod" << std::endl;
}	

Skudce::Skudce(sf::RenderWindow *App, Space* s, int pozice_x, int pozice_y, Objekt *vlastnik) : Objekt(App, s, pozice_x, pozice_y){
	Image.LoadFromFile("raketa2.tga");
	sprite.SetImage(Image);
	//sprite.SetColor(sf::Color(255,255,255,128));
	vyska = Image.GetHeight();
	sirka = Image.GetWidth();
	//std::cout << "vytvoreni objektu Skudce" << std::endl;
}

Strela::Strela(sf::RenderWindow *App, Space* s, int pozice_x, int pozice_y, Objekt *vlastnik) : Objekt(App, s, pozice_x, pozice_y, vlastnik){
	Image.LoadFromFile("strela.tga");
	sprite.SetImage(Image);

	//std::cout << "vytvoreni objektu Strela" << std::endl;
}



