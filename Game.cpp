// NickyGame.cpp: A program using the TL-Engine

#include <TL-Engine.h>
#include <iostream>// TL-Engine include file and namespace
#include <sstream>
using namespace tle;

enum GameState {Landing,Roaming,Finish};
struct ship
{
	IModel* model;
	float realSpeed = 0.0f;
	float maxThrust = 6.0f;
	float upThrust = 0.0f;
	int PlayerState = Landing;
	int fuel = 4000;
};
void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine( kTLX );
	myEngine->StartWindowed();
	ICamera* myCamera = myEngine->CreateCamera(kFPS);

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder( "D:\\ProgramData\\TL-Engine\\Media" );

	/**** Set up your scene here ****/
	/*Meshes*/
	IMesh* Playership = myEngine->LoadMesh("Cube.x");
	IMesh* skyboxMesh = myEngine->LoadMesh("Skybox_SciFi.x");
	IMesh* floorMesh = myEngine->LoadMesh("floor.x");
	

	IModel* Floor = floorMesh->CreateModel(0, 0, 0);
	IModel* Skybox = skyboxMesh->CreateModel(0, -5, 0);
	IFont* myFont = myEngine->LoadFont("Font1.bmp");
	ship Player;
	Player.model = Playership->CreateModel(0, 500, 0);
	myCamera->AttachToParent(Player.model);
	
	Player.model->Scale(0.1);

	float Gravity = -0.1f;
	float frameTime = myEngine->Timer();
	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{

		// Draw the scene
		myEngine->DrawScene();
		
		frameTime = myEngine->Timer();
		float frameSpeed = frameTime * 350;
		
		if (Player.PlayerState == Landing)
		{
			if (myEngine->KeyHeld(Key_W) && Player.fuel > 0)
			{
				if (Player.upThrust < Player.maxThrust)
				{
					Player.upThrust = Player.upThrust + 0.0001;
					Player.fuel--;
				}
			}
			else if (Player.upThrust > Gravity)
			{
				Player.upThrust = Player.upThrust - 0.0001;
			}
			stringstream outText;
			outText << Player.upThrust << "                                                   FUEL LEFT:" << Player.fuel;
			myFont->Draw(outText.str(), 700, 200);
			
			Player.realSpeed = (Gravity + Player.upThrust);
			Player.model->MoveY(Player.realSpeed * frameSpeed);
		
			if (Player.model->GetY() - 5 <= 0)
			{
				if (Player.realSpeed < -0.09)
				{
					Player.model->SetY(100);
				}
				else Player.PlayerState = Roaming;
			}
		}
		
		if (Player.PlayerState == Roaming)
		{
			if (myEngine->KeyHeld(Key_W))
			{
				Player.model->MoveZ(0.01f * frameSpeed);
			}

		}
	}
	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
