#include "Game.h"
#include "Vertex.h"

// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore(
		hInstance,		   // The application's handle
		"DirectX Game",	   // Text for the window's title bar
		1280,			   // Width of the window's client area
		720,			   // Height of the window's client area
		true)			   // Show extra stats (fps) in title bar?
{
	indexBuffer;
	vertexBuffer;
	vertexShader = 0;
	pixelShader = 0;

	dLight1 = {};
	dLight2 = {};

	meshCount = 4;
	entityCount = 4;

	prevMousePos = { 0,0 };

	samplerStruct = {};

#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.\n");
#endif
	
}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{

	// Delete our simple shader objects, which
	// will clean up their own internal DirectX stuff
	delete vertexShader;
	delete pixelShader;

	//Delete meshes
	for (auto& m : meshes) delete m;

	delete material1;
	delete material2;

	for (auto& e : entities) delete e;

	//Delete camera
	delete gameCamera;

	wallTexture->Release();
	cliffTexture->Release();
	samplerState->Release();
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later
	LoadShaders();
	CreateMatrices();

	//Load Textures
	DirectX::CreateWICTextureFromFile(device, context, L"Cliff.jpg", 0, &cliffTexture);
	DirectX::CreateWICTextureFromFile(device, context, L"StoneWall.jpg", 0, &wallTexture);

	samplerStruct.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerStruct.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerStruct.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerStruct.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerStruct.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&samplerStruct, &samplerState);

	CreateBasicGeometry();

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//initialize camera
	gameCamera = new Camera();

	// Create the View matrix
	// - In an actual game, recreate this matrix every time the camera 
	//    moves (potentially every frame)
	// - We're using the LOOK TO function, which takes the position of the
	//    camera and the direction vector along which to look (as well as "up")
	// - Another option is the LOOK AT function, to look towards a specific
	//    point in 3D space
	XMVECTOR pos = XMVectorSet(0, 0, -5, 0);
	XMVECTOR dir = XMVectorSet(0, 0, 1, 0);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	XMMATRIX V = XMMatrixLookToLH(
		pos,     // The position of the "camera"
		dir,     // Direction the camera is looking
		up);     // "Up" direction in 3D space (prevents roll)
	DirectX::XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V)); // Transpose for HLSL!

	gameCamera->UpdateProjectionMatrix((float)width / height);
	projectionMatrix = gameCamera->GetProjectionMatrix();

	dLight1 = { XMFLOAT4(0.1f, 0.1f, 0.1f, 0.1f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, -1.0f, 0.0f) };
	dLight2 = { XMFLOAT4(0.1f, 0.1f, 0.1f, 0.1f), XMFLOAT4(0.4f, 0.8f, 0.35f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) };
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files using
// my SimpleShader wrapper for DirectX shader manipulation.
// - SimpleShader provides helpful methods for sending
//   data to individual variables on the GPU
// --------------------------------------------------------
void Game::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device, context);
	vertexShader->LoadShaderFile(L"VertexShader.cso");

	pixelShader = new SimplePixelShader(device, context);
	pixelShader->LoadShaderFile(L"PixelShader.cso");
}



// --------------------------------------------------------
// Initializes the matrices necessary to represent our geometry's 
// transformations and our 3D camera
// --------------------------------------------------------
void Game::CreateMatrices()
{
	// Set up world matrix
	// - In an actual game, each object will need one of these and they should
	//    update when/if the object moves (every frame)
	// - You'll notice a "transpose" happening below, which is redundant for
	//    an identity matrix.  This is just to show that HLSL expects a different
	//    matrix (column major vs row major) than the DirectX Math library
	XMMATRIX W = XMMatrixIdentity();
	DirectX::XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W)); // Transpose for HLSL!
}


// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateBasicGeometry()
{
	// Create some temporary variables to represent colors
	// - Not necessary, just makes things more readable
	XMFLOAT4 red = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	XMFLOAT4 yellow = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);

	meshes.push_back(new Mesh("cube.obj", device));

	meshes.push_back(new Mesh("sphere.obj", device));

	Vertex starVertices[] =
	{
		{ XMFLOAT3(0.0f, 1.2f, +0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(0.5f, -1.3f, +0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(-0.3f, -0.3f, +0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },

		{ XMFLOAT3(-1.5f, 0.2f, +0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(1.5f, 0.2f, +0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(0.0f, -0.7f, +0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },

		{ XMFLOAT3(-0.5f, -1.3f, +0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) }
	};

	int starIndices[] = { 0, 1, 2, 3, 4, 5, 6, 2, 5 };

	meshes.push_back(new Mesh(starVertices, 7, (UINT*)starIndices, 9, device));

	meshes.push_back(new Mesh("helix.obj", device));

	material1 = new Material(pixelShader, vertexShader, cliffTexture, samplerState);
	material2 = new Material(pixelShader, vertexShader, wallTexture, samplerState);

	//Assign meshes to entities
	for (int i = 0; i < entityCount-1; i++)
	{
		entities.push_back(new Entity(meshes[i], material1));
	}

	entities.push_back(new Entity(meshes[3], material2));

	//Set entities' starting positions
	entities[0]->SetPostion(DirectX::XMFLOAT3(1.0f, 1.5f, 0.0f));
	entities[1]->SetPostion(DirectX::XMFLOAT3(-1.0f, -2.0f, 0.0f));
	entities[2]->SetPostion(DirectX::XMFLOAT3(-2.0f, -1.0f, 0.0f));
	entities[3]->SetPostion(DirectX::XMFLOAT3(3.0f, 0.0f, 0.0f));
}


// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();

	gameCamera->UpdateProjectionMatrix((float)width / height);
	projectionMatrix = gameCamera->GetProjectionMatrix();
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();

	//Call the camera's update method
	gameCamera->Update(deltaTime, totalTime);

	//Rotate entity1
	XMFLOAT3 e1CurrentRotation = entities[0]->GetRotation();
	e1CurrentRotation.z += 0.3f * deltaTime;
	entities[0]->SetRotation(e1CurrentRotation);

	//Move entity2 to the right
	entities[1]->Move(0.2f * deltaTime, 0.0f, 0.0f);

	//Move enitity3 diagonally up to the right
	entities[2]->Move(0.1f * deltaTime, 0.05f * deltaTime, 0.0f);

	//Scale entity5 vertically
	XMFLOAT3 e5CurrentScale = entities[3]->GetScale();
	e5CurrentScale.y += 0.1f * deltaTime;
	entities[3]->SetScale(e5CurrentScale);

	for (size_t i = 0; i < entityCount; i++)
	{
		entities[i]->UpdateWorldMatrix();
	}
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	//Get camera matrices
	viewMatrix = gameCamera->GetViewMatrix();
	projectionMatrix = gameCamera->GetProjectionMatrix();

	// Send data to shader variables
	//  - Do this ONCE PER OBJECT you're drawing
	//  - This is actually a complex process of copying data to a local buffer
	//    and then copying that entire buffer to the GPU.  
	//  - The "SimpleShader" class handles all of that for you.

	for (size_t i = 0; i < entityCount; i++)
	{
		Entity* currentEntity = entities[i];
		ID3D11Buffer* vertexBuffer = currentEntity->GetMesh()->GetVertexBuffer();
		ID3D11Buffer* indexBuffer = currentEntity->GetMesh()->GetIndexBuffer();

		// Set buffers in the input assembler
		//  - Do this ONCE PER OBJECT you're drawing, since each object might
		//    have different geometry.
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

		currentEntity->PrepareMaterial(viewMatrix, projectionMatrix);
		vertexShader = currentEntity->GetMaterial()->GetVertexShader();
		pixelShader = currentEntity->GetMaterial()->GetPixelShader();

		pixelShader->SetShaderResourceView("diffuseTexture", currentEntity->GetMaterial()->GetResourceView());
		pixelShader->SetSamplerState("basicSampler", samplerState);

		pixelShader->SetData(
			"light1",
			&dLight1,
			sizeof(dLight1));

		pixelShader->SetData(
			"light2",
			&dLight2,
			sizeof(dLight2));

		// Finally do the actual drawing
		//  - Do this ONCE PER OBJECT you intend to draw
		//  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
		//  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
		//     vertices in the currently set VERTEX BUFFER
		context->DrawIndexed(
			currentEntity->GetMesh()->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
			0,     // Offset to the first index we want to use
			0);    // Offset to add to each index when looking up vertices
	}

	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(0, 0);

	// Due to the usage of a more sophisticated swap chain effect,
	// the render target must be re-bound after every call to Present()
	context->OMSetRenderTargets(1, &backBufferRTV, depthStencilView);
}


#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
// --------------------------------------------------------
void Game::OnMouseDown(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;

	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hWnd);
}

// --------------------------------------------------------
// Helper method for mouse release
// --------------------------------------------------------
void Game::OnMouseUp(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
// --------------------------------------------------------
void Game::OnMouseMove(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...
	if (buttonState & 0x0001)
	{
		int xRotation = x - prevMousePos.x;
		int yRotation = y - prevMousePos.y;

		gameCamera->RotateCamera(xRotation, yRotation);
	}
	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;
}

// --------------------------------------------------------
// Helper method for mouse wheel scrolling.  
// WheelDelta may be positive or negative, depending 
// on the direction of the scroll
// --------------------------------------------------------
void Game::OnMouseWheel(float wheelDelta, int x, int y)
{
	// Add any custom code here...
}
#pragma endregion