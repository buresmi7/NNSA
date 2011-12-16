#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

class Raketa{
private:
	double poloha_x;
	double poloha_y;
	float Width;
	float Height;
	sf::Sprite Sprite;
	sf::Image Image;
	sf::RenderWindow *App;//odkaz na okno do ktereho ma vykreslovat
public:
	Raketa(sf::RenderWindow &App){		

		this->App = &App;	
		std::cout << "vytvoreni rakety" << std::endl;
	}
	Raketa(const Raketa &r){
		Width = r.Width;
		Height = r.Height;
		Sprite = r.Sprite;
		App = r.App;
		Image = r.Image;
		std::cout << "kopiruji raketu" << std::endl;
	}
	void Init(){
		Image.LoadFromFile("raketa.tga");
		Sprite.SetImage(Image);

		Width  = Sprite.GetSize().x;
		Height = Sprite.GetSize().y;

		poloha_x = App->GetWidth()/2;
		poloha_y = App->GetHeight()/2;

		Sprite.SetCenter(Width/2, Height/2);
		Sprite.Rotate(90);
		std::cout << "inicializuji raketu" << std::endl;
	}
	void Draw(){
		Sprite.SetPosition(poloha_x, poloha_y);
		this->App->Draw(Sprite);		
		std::cout << "vykresluji" << std::endl;
	}
	bool PosunVlevo(){
		if(poloha_x < Sprite.GetSize().x/2)
			return false;
		poloha_x -= 10*App->GetFrameTime();
		return true;
	}
	bool PosunVpravo(){
		if(poloha_x > App->GetWidth() - Sprite.GetSize().x/2)
			return false;
		poloha_x += 10*App->GetFrameTime();
		return true;
	}
	bool PosunNahoru(){
		if(poloha_y < Sprite.GetSize().y/2)
			return false;
		poloha_y -= 10*App->GetFrameTime();
		return true;
	}
	double getPolohaX(){
		return poloha_x;
	}
	double getPolohaY(){
		return poloha_y;
	}
};
