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

	prevMousePos = { 0,0 };

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
	delete mesh1;
	delete mesh2;
	delete mesh3;

	delete entity1;
	delete entity2;
	delete entity3;
	delete entity4;
	delete entity5;
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
	CreateBasicGeometry();

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
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
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W)); // Transpose for HLSL!

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
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V)); // Transpose for HLSL!

	// Create the Projection matrix
	// - This should match the window's aspect ratio, and also update anytime
	//    the window resizes (which is already happening in OnResize() below)
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		(float)width / height,		// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
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

	// Set up the vertices of the triangle we would like to draw
	// - We're going to copy this array, exactly as it exists in memory
	//    over to a DirectX-controlled data structure (the vertex buffer)
	Vertex triangleVertices[] =
	{
		{ XMFLOAT3(+0.0f, +1.0f, +0.0f), red },
		{ XMFLOAT3(+1.0f, -1.0f, +0.0f), blue },
		{ XMFLOAT3(-1.0f, -1.0f, +0.0f), yellow }
	};

	// Set up the indices, which tell us which vertices to use and in which order
	// - This is somewhat redundant for just 3 vertices (it's a simple example)
	// - Indices are technically not required if the vertices are in the buffer 
	//    in the correct order and each one will be used exactly once
	// - But just to see how it's done...
	int triangleIndices[] = { 0, 1, 2 };

	mesh1 = new Mesh(triangleVertices, 3, triangleIndices, 3, device);

	Vertex squareVertices[] =
	{
		{ XMFLOAT3(-1.0f, +1.0f, +0.0f), red },
		{ XMFLOAT3(+1.0f, +1.0f, +0.0f), green },
		{ XMFLOAT3(-1.0f, -1.0f, +0.0f), blue },
		{ XMFLOAT3(+1.0f, -1.0f, +0.0f), yellow }
	};

	int squareIndices[] = { 0, 1, 2, 3, 2, 1 };

	mesh2 = new Mesh(squareVertices, 4, squareIndices, 6, device);

	Vertex starVertices[] =
	{
		{ XMFLOAT3(0.0f, 1.2f, +0.0f), yellow },
		{ XMFLOAT3(0.5f, -1.3f, +0.0f), yellow },
		{ XMFLOAT3(-0.3f, -0.3f, +0.0f), yellow },

		{ XMFLOAT3(-1.5f, 0.2f, +0.0f), yellow },
		{ XMFLOAT3(1.5f, 0.2f, +0.0f), yellow },
		{ XMFLOAT3(0.0f, -0.7f, +0.0f), yellow },

		{ XMFLOAT3(-0.5f, -1.3f, +0.0f), yellow }
	};

	int starIndices[] = { 0, 1, 2, 3, 4, 5, 6, 2, 5 };

	mesh3 = new Mesh(starVertices, 7, starIndices, 9, device);

	//Assign meshes to entities
	entity1 = new Entity(mesh1);
	entity2 = new Entity(mesh2);
	entity3 = new Entity(mesh3);
	entity4 = new Entity(mesh1);
	entity5 = new Entity(mesh2);

	//Set entities' starting positions
	entity1->SetPostion(DirectX::XMFLOAT3(1.0f, 1.5f, 0.0f));
	entity2->SetPostion(DirectX::XMFLOAT3(-1.0f, -2.0f, 0.0f));
	entity3->SetPostion(DirectX::XMFLOAT3(-2.0f, -1.0f, 0.0f));
	entity4->SetPostion(DirectX::XMFLOAT3(3.0f, 0.0f, 0.0f));
	entity5->SetPostion(DirectX::XMFLOAT3(-3.0f, 0.0f, 0.0f));
}


// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();

	// Update our projection matrix since the window size changed
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,	// Field of View Angle
		(float)width / height,	// Aspect ratio
		0.1f,				  	// Near clip plane distance
		100.0f);			  	// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();

	//Rotate entity1
	XMFLOAT3 e1CurrentRotation = entity1->GetRotation();
	e1CurrentRotation.z += 0.3f * deltaTime;
	entity1->SetRotation(e1CurrentRotation);

	//Move entity2 to the right
	entity2->Move(0.2f * deltaTime, 0.0f, 0.0f);

	//Move enitity3 diagonally up to the right
	entity3->Move(0.1f * deltaTime, 0.05f * deltaTime, 0.0f);

	//Move entity4 to the left while spinning it clockwise
	XMFLOAT3 e4CurrentRotation = entity4->GetRotation();
	e4CurrentRotation.z -= 0.2f * deltaTime;
	entity4->SetRotation(e4CurrentRotation);
	entity4->Move(-0.2f * deltaTime, 0.0f, 0.0f);

	//Scale entity5 vertically
	XMFLOAT3 e5CurrentScale = entity5->GetScale();
	e5CurrentScale.y += 0.1f * deltaTime;
	entity5->SetScale(e5CurrentScale);
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

	// Send data to shader variables
	//  - Do this ONCE PER OBJECT you're drawing
	//  - This is actually a complex process of copying data to a local buffer
	//    and then copying that entire buffer to the GPU.  
	//  - The "SimpleShader" class handles all of that for you.

	//-------------------------------------Entity1

	XMMATRIX trans1 = XMMatrixTranslation(entity1->GetPosition().x, entity1->GetPosition().y, entity1->GetPosition().z);
	XMMATRIX rot1 = XMMatrixRotationRollPitchYaw(entity1->GetRotation().x, entity1->GetRotation().y, entity1->GetRotation().z);
	XMMATRIX scale1 = XMMatrixScaling(entity1->GetScale().x, entity1->GetScale().y, entity1->GetScale().z);

	XMMATRIX world1 = scale1 * rot1 * trans1;
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(world1));

	vertexShader->SetMatrix4x4("world", worldMatrix);
	vertexShader->SetMatrix4x4("view", viewMatrix);
	vertexShader->SetMatrix4x4("projection", projectionMatrix);

	// Once you've set all of the data you care to change for
	// the next draw call, you need to actually send it to the GPU
	//  - If you skip this, the "SetMatrix" calls above won't make it to the GPU!
	vertexShader->CopyAllBufferData();

	// Set the vertex and pixel shaders to use for the next Draw() command
	//  - These don't technically need to be set every frame...YET
	//  - Once you start applying different shaders to different objects,
	//    you'll need to swap the current shaders before each draw
	vertexShader->SetShader();
	pixelShader->SetShader();

	// Set buffers in the input assembler
	//  - Do this ONCE PER OBJECT you're drawing, since each object might
	//    have different geometry.
	UINT stride1 = sizeof(Vertex);
	UINT offset1 = 0;

	ID3D11Buffer* vertexBuffer1 = entity1->GetMesh()->GetVertexBuffer();

	context->IASetVertexBuffers(0, 1, &vertexBuffer1, &stride1, &offset1);
	context->IASetIndexBuffer(entity1->GetMesh()->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	// Finally do the actual drawing
	//  - Do this ONCE PER OBJECT you intend to draw
	//  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
	//  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
	//     vertices in the currently set VERTEX BUFFER
	context->DrawIndexed(
		3,     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);    // Offset to add to each index when looking up vertices

	//-------------------------------------Entity2

	XMMATRIX trans2 = XMMatrixTranslation(entity2->GetPosition().x, entity2->GetPosition().y, entity2->GetPosition().z);
	XMMATRIX rot2 = XMMatrixRotationRollPitchYaw(entity2->GetRotation().x, entity2->GetRotation().y, entity2->GetRotation().z);
	XMMATRIX scale2 = XMMatrixScaling(entity2->GetScale().x, entity2->GetScale().y, entity2->GetScale().z);

	XMMATRIX world2 = scale2 * rot2 * trans2;
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(world2));

	vertexShader->SetMatrix4x4("world", worldMatrix);
	vertexShader->SetMatrix4x4("view", viewMatrix);
	vertexShader->SetMatrix4x4("projection", projectionMatrix);

	// Once you've set all of the data you care to change for
	// the next draw call, you need to actually send it to the GPU
	//  - If you skip this, the "SetMatrix" calls above won't make it to the GPU!
	vertexShader->CopyAllBufferData();

	// Set the vertex and pixel shaders to use for the next Draw() command
	//  - These don't technically need to be set every frame...YET
	//  - Once you start applying different shaders to different objects,
	//    you'll need to swap the current shaders before each draw
	vertexShader->SetShader();
	pixelShader->SetShader();

	// Set buffers in the input assembler
	//  - Do this ONCE PER OBJECT you're drawing, since each object might
	//    have different geometry.
	UINT stride2 = sizeof(Vertex);
	UINT offset2 = 0;

	ID3D11Buffer* vertexBuffer2 = entity2->GetMesh()->GetVertexBuffer();

	context->IASetVertexBuffers(0, 1, &vertexBuffer2, &stride2, &offset2);
	context->IASetIndexBuffer(entity2->GetMesh()->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	// Finally do the actual drawing
	//  - Do this ONCE PER OBJECT you intend to draw
	//  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
	//  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
	//     vertices in the currently set VERTEX BUFFER
	context->DrawIndexed(
		6,     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);    // Offset to add to each index when looking up vertices

	//-------------------------------------Entity3

	XMMATRIX trans3 = XMMatrixTranslation(entity3->GetPosition().x, entity3->GetPosition().y, entity3->GetPosition().z);
	XMMATRIX rot3 = XMMatrixRotationRollPitchYaw(entity3->GetRotation().x, entity3->GetRotation().y, entity3->GetRotation().z);
	XMMATRIX scale3 = XMMatrixScaling(entity3->GetScale().x, entity3->GetScale().y, entity3->GetScale().z);

	XMMATRIX world3 = scale3 * rot3 * trans3;
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(world3));

	vertexShader->SetMatrix4x4("world", worldMatrix);
	vertexShader->SetMatrix4x4("view", viewMatrix);
	vertexShader->SetMatrix4x4("projection", projectionMatrix);

	// Once you've set all of the data you care to change for
	// the next draw call, you need to actually send it to the GPU
	//  - If you skip this, the "SetMatrix" calls above won't make it to the GPU!
	vertexShader->CopyAllBufferData();

	// Set the vertex and pixel shaders to use for the next Draw() command
	//  - These don't technically need to be set every frame...YET
	//  - Once you start applying different shaders to different objects,
	//    you'll need to swap the current shaders before each draw
	vertexShader->SetShader();
	pixelShader->SetShader();

	// Set buffers in the input assembler
	//  - Do this ONCE PER OBJECT you're drawing, since each object might
	//    have different geometry.
	UINT stride3 = sizeof(Vertex);
	UINT offset3 = 0;

	ID3D11Buffer* vertexBuffer3 = entity3->GetMesh()->GetVertexBuffer();

	context->IASetVertexBuffers(0, 1, &vertexBuffer3, &stride3, &offset3);
	context->IASetIndexBuffer(entity3->GetMesh()->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	// Finally do the actual drawing
	//  - Do this ONCE PER OBJECT you intend to draw
	//  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
	//  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
	//     vertices in the currently set VERTEX BUFFER
	context->DrawIndexed(
		9,     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);    // Offset to add to each index when looking up vertices

	//-------------------------------------Entity4

	XMMATRIX trans4 = XMMatrixTranslation(entity4->GetPosition().x, entity4->GetPosition().y, entity4->GetPosition().z);
	XMMATRIX rot4 = XMMatrixRotationRollPitchYaw(entity4->GetRotation().x, entity4->GetRotation().y, entity4->GetRotation().z);
	XMMATRIX scale4 = XMMatrixScaling(entity4->GetScale().x, entity4->GetScale().y, entity4->GetScale().z);

	XMMATRIX world4 = scale4 * rot4 * trans4;
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(world4));

	vertexShader->SetMatrix4x4("world", worldMatrix);
	vertexShader->SetMatrix4x4("view", viewMatrix);
	vertexShader->SetMatrix4x4("projection", projectionMatrix);

	// Once you've set all of the data you care to change for
	// the next draw call, you need to actually send it to the GPU
	//  - If you skip this, the "SetMatrix" calls above won't make it to the GPU!
	vertexShader->CopyAllBufferData();

	// Set the vertex and pixel shaders to use for the next Draw() command
	//  - These don't technically need to be set every frame...YET
	//  - Once you start applying different shaders to different objects,
	//    you'll need to swap the current shaders before each draw
	vertexShader->SetShader();
	pixelShader->SetShader();

	// Set buffers in the input assembler
	//  - Do this ONCE PER OBJECT you're drawing, since each object might
	//    have different geometry.
	UINT stride4 = sizeof(Vertex);
	UINT offset4 = 0;

	ID3D11Buffer* vertexBuffer4 = entity4->GetMesh()->GetVertexBuffer();

	context->IASetVertexBuffers(0, 1, &vertexBuffer1, &stride4, &offset4);
	context->IASetIndexBuffer(entity4->GetMesh()->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	// Finally do the actual drawing
	//  - Do this ONCE PER OBJECT you intend to draw
	//  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
	//  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
	//     vertices in the currently set VERTEX BUFFER
	context->DrawIndexed(
		3,     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);    // Offset to add to each index when looking up vertices

	//-------------------------------------Entity5

	XMMATRIX trans5 = XMMatrixTranslation(entity5->GetPosition().x, entity5->GetPosition().y, entity5->GetPosition().z);
	XMMATRIX rot5 = XMMatrixRotationRollPitchYaw(entity5->GetRotation().x, entity5->GetRotation().y, entity5->GetRotation().z);
	XMMATRIX scale5 = XMMatrixScaling(entity5->GetScale().x, entity5->GetScale().y, entity5->GetScale().z);

	XMMATRIX world5 = scale5 * rot5 * trans5;
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(world5));

	vertexShader->SetMatrix4x4("world", worldMatrix);
	vertexShader->SetMatrix4x4("view", viewMatrix);
	vertexShader->SetMatrix4x4("projection", projectionMatrix);

	// Once you've set all of the data you care to change for
	// the next draw call, you need to actually send it to the GPU
	//  - If you skip this, the "SetMatrix" calls above won't make it to the GPU!
	vertexShader->CopyAllBufferData();

	// Set the vertex and pixel shaders to use for the next Draw() command
	//  - These don't technically need to be set every frame...YET
	//  - Once you start applying different shaders to different objects,
	//    you'll need to swap the current shaders before each draw
	vertexShader->SetShader();
	pixelShader->SetShader();

	// Set buffers in the input assembler
	//  - Do this ONCE PER OBJECT you're drawing, since each object might
	//    have different geometry.
	UINT stride5 = sizeof(Vertex);
	UINT offset5 = 0;

	ID3D11Buffer* vertexBuffer5 = entity5->GetMesh()->GetVertexBuffer();

	context->IASetVertexBuffers(0, 1, &vertexBuffer2, &stride5, &offset5);
	context->IASetIndexBuffer(entity5->GetMesh()->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	 /*Finally do the actual drawing
	  - Do this ONCE PER OBJECT you intend to draw
	  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
	  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
	     vertices in the currently set VERTEX BUFFER*/
	context->DrawIndexed(
		6,     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);    // Offset to add to each index when looking up vertices

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