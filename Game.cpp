#include "Urho3DAll.h"
#include "Ball.h"
#include "Cell.h"
#include "Board.h"
#include "Path.h"
#include "Game.h"
#include "Resources.h"


DEFINE_APPLICATION_MAIN(Game)


Game::Game(Context* context) :
    Application(context),
    cameraTarget_(0.0f, 0.0f, 0.0f)
{
}


void Game::Setup()
{
    engineParameters_["WindowTitle"] = GetTypeName();
    engineParameters_["LogName"] = GetSubsystem<FileSystem>()->GetAppPreferencesDir("urho3d", "logs") + GetTypeName() + ".log";
    engineParameters_["FullScreen"] = false;
    engineParameters_["Headless"] = false;
    engineParameters_["WindowWidth"] = 800;
    engineParameters_["WindowHeight"] = 600;
    engineParameters_["ResourcePaths"] = "Data;CoreData";
}


void Game::Start()
{
    SetRandomSeed(Time::GetSystemTime());
    Ball::RegisterObject(context_);
    Cell::RegisterObject(context_);
    Board::RegisterObject(context_);
    Path::RegisterObject(context_);

    ResourceCache* cache = GetSubsystem<ResourceCache>();
    LoadResources(cache);
    GetSubsystem<Input>()->SetMouseVisible(true);

    CreateScene();
    SetupViewport();
    SubscribeToEvents();
    CreateUI();

    XMLFile* xmlFile = cache->GetResource<XMLFile>("UI/DefaultStyle.xml");
    DebugHud* debugHud = engine_->CreateDebugHud();
    debugHud->SetDefaultStyle(xmlFile);

}



void Game::CreateUIWindow()
{
    UIElement* uiRoot = GetSubsystem<UI>()->GetRoot();
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    XMLFile* style = cache->GetResource<XMLFile>("UI/DefaultStyle.xml");
    uiRoot->SetDefaultStyle(style);
    window_ = new Window(context_);
    uiRoot->AddChild(window_);

    window_->SetMinSize(384, 192);
    window_->SetLayout(LM_VERTICAL, 6, IntRect(6, 6, 6, 6));
    window_->SetAlignment(HA_CENTER, VA_CENTER);
    window_->SetName("Window");

    UIElement* titleBar = new UIElement(context_);
    titleBar->SetMinSize(0, 24);
    titleBar->SetVerticalAlignment(VA_TOP);
    titleBar->SetLayoutMode(LM_HORIZONTAL);

    Text* windowTitle = new Text(context_);
    windowTitle->SetName("WindowTitle");
    windowTitle->SetText("Lines");

    Button* buttonClose = new Button(context_);
    buttonClose->SetName("CloseButton");

    titleBar->AddChild(windowTitle);
    titleBar->AddChild(buttonClose);

    window_->AddChild(titleBar);

    window_->SetStyleAuto();
    windowTitle->SetStyleAuto();
    buttonClose->SetStyle("CloseButton");

    SubscribeToEvent(buttonClose, E_RELEASED, HANDLER(Game, HandleClosePressed));
}


void Game::HandleClosePressed(StringHash eventType, VariantMap& eventData)
{
    window_->SetVisible(false);
}


void Game::CreateUI()
{
    CreateUIWindow();

    {
        Button* button = new Button(context_);
        button->SetMinHeight(24);
        window_->AddChild(button);
        button->SetStyleAuto();
        {
            Text* t = new Text(context_);
            button->AddChild(t);
            t->SetStyle("Text");
            t->SetAlignment(HA_CENTER, VA_CENTER);
            t->SetText("Easy");
        }
        SubscribeToEvent(button, E_RELEASED, HANDLER(Game, HandleEasyPressed));
    }

    {
        Button* button = new Button(context_);
        button->SetMinHeight(24);
        window_->AddChild(button);
        button->SetStyleAuto();
        {
            Text* t = new Text(context_);
            button->AddChild(t);
            t->SetStyle("Text");
            t->SetAlignment(HA_CENTER, VA_CENTER);
            t->SetText("Normal");
        }
        SubscribeToEvent(button, E_RELEASED, HANDLER(Game, HandleNormalPressed));
    }

    {
        Button* button = new Button(context_);
        button->SetMinHeight(24);
        window_->AddChild(button);
        button->SetStyleAuto();
        {
            Text* t = new Text(context_);
            button->AddChild(t);
            t->SetStyle("Text");
            t->SetAlignment(HA_CENTER, VA_CENTER);
            t->SetText("Hard");
        }
        SubscribeToEvent(button, E_RELEASED, HANDLER(Game, HandleHardPressed));
    }

    {
        Button* button = new Button(context_);
        button->SetMinHeight(24);
        window_->AddChild(button);
        button->SetStyleAuto();
        {
            Text* t = new Text(context_);
            button->AddChild(t);
            t->SetStyle("Text");
            t->SetAlignment(HA_CENTER, VA_CENTER);
            t->SetText("Expert");
        }
        SubscribeToEvent(button, E_RELEASED, HANDLER(Game, HandleExpertPressed));
    }

    {
        Button* button = new Button(context_);
        button->SetMinHeight(24);
        window_->AddChild(button);
        button->SetStyleAuto();
        {
            Text* t = new Text(context_);
            button->AddChild(t);
            t->SetStyle("Text");
            t->SetAlignment(HA_CENTER, VA_CENTER);
            t->SetText("Master");
        }
        SubscribeToEvent(button, E_RELEASED, HANDLER(Game, HandleMasterPressed));
    }

    {
        Button* button = new Button(context_);
        button->SetMinHeight(24);
        window_->AddChild(button);
        button->SetStyleAuto();
        {
            Text* t = new Text(context_);
            button->AddChild(t);
            t->SetStyle("Text");
            t->SetAlignment(HA_CENTER, VA_CENTER);
            t->SetText("Exit");
        }
        SubscribeToEvent(button, E_RELEASED, HANDLER(Game, HandleExitPressed));
    }

    UIElement* uiRoot = GetSubsystem<UI>()->GetRoot();
    ResourceCache* cache = GetSubsystem<ResourceCache>();

    {
        Text* t = new Text(context_);
        uiRoot->AddChild(t);
        t->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 30);
        t->SetColor(Color(0.0f, 0.0f, 0.0f));
        t->SetName("Score");
        t->SetAlignment(HA_LEFT, VA_TOP);
    }
    {
        Text* t = new Text(context_);
        uiRoot->AddChild(t);
        t->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 30);
        t->SetColor(Color(0.0f, 0.0f, 0.0f));
        t->SetName("Colors");
        t->SetAlignment(HA_LEFT, VA_BOTTOM);
    }
    {
        Text* t = new Text(context_);
        uiRoot->AddChild(t);
        t->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 30);
        t->SetColor(Color(0.0f, 0.0f, 0.0f));
        t->SetName("LineLength");
        t->SetAlignment(HA_RIGHT, VA_BOTTOM);
    }
    {
        Text* t = new Text(context_);
        uiRoot->AddChild(t);
        t->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 30);
        t->SetColor(Color(0.0f, 0.0f, 0.0f));
        t->SetName("Record");
        t->SetAlignment(HA_RIGHT, VA_TOP);
    }

}


void Game::HandleEasyPressed(StringHash eventType, VariantMap& eventData)
{
    //if (!window_->IsVisible())  // wtf invisible cotrols recieve keyboard events
    //    return;
    boardNode_->RemoveAllChildren();
    boardNode_->RemoveAllComponents();
    Board* board = boardNode_->CreateComponent<Board>();
    board->Init(D_EASY);
    window_->SetVisible(false);
}


void Game::HandleNormalPressed(StringHash eventType, VariantMap& eventData)
{
    boardNode_->RemoveAllChildren();
    boardNode_->RemoveAllComponents();
    Board* board = boardNode_->CreateComponent<Board>();
    board->Init(D_NORMAL);
    window_->SetVisible(false);
}

void Game::HandleHardPressed(StringHash eventType, VariantMap& eventData)
{
    boardNode_->RemoveAllChildren();
    boardNode_->RemoveAllComponents();
    Board* board = boardNode_->CreateComponent<Board>();
    board->Init(D_HARD);
    window_->SetVisible(false);
}


void Game::HandleExpertPressed(StringHash eventType, VariantMap& eventData)
{
    boardNode_->RemoveAllChildren();
    boardNode_->RemoveAllComponents();
    Board* board = boardNode_->CreateComponent<Board>();
    board->Init(D_EXPERT);
    window_->SetVisible(false);
}


void Game::HandleMasterPressed(StringHash eventType, VariantMap& eventData)
{
    boardNode_->RemoveAllChildren();
    boardNode_->RemoveAllComponents();
    Board* board = boardNode_->CreateComponent<Board>();
    board->Init(D_MASTER);
    window_->SetVisible(false);
}


void Game::HandleExitPressed(StringHash eventType, VariantMap& eventData)
{
    engine_->Exit();
}


void Game::SetupViewport()
{
    Renderer* renderer = GetSubsystem<Renderer>();
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    SharedPtr<Viewport> viewport(new Viewport(context_, scene_, cameraNode_->GetComponent<Camera>()));
    renderer->SetViewport(0, viewport);
    RenderPath* effectRenderPath = viewport->GetRenderPath();
    effectRenderPath->Append(cache->GetResource<XMLFile>("PostProcess/FXAA3.xml"));
}


void Game::CreateScene()
{
    ResourceCache* cache = GetSubsystem<ResourceCache>();

    scene_ = new Scene(context_);
    scene_->CreateComponent<Octree>();
    scene_->CreateComponent<DebugRenderer>();

    lightNode_ = scene_->CreateChild("DirectionalLight");
    lightNode_->SetDirection(Vector3(1.0f, -1.0f, 3.0f));
    Light* light = lightNode_->CreateComponent<Light>();
    light->SetColor(Color(0.5f, 0.5f, 0.5f));
    light->SetLightType(LIGHT_DIRECTIONAL);
    light->SetCastShadows(true);
    light->SetShadowBias(BiasParameters(0.00025f, 0.5f));
    light->SetShadowCascade(CascadeParameters(10.0f, 50.0f, 200.0f, 0.0f, 0.8f));
    //light->SetShadowIntensity(0.5f);

    Node* zoneNode = scene_->CreateChild("Zone");
    Zone* zone = zoneNode->CreateComponent<Zone>();
    zone->SetBoundingBox(BoundingBox(-1000.0f, 1000.0f));
    zone->SetAmbientColor(Color(0.5f, 0.5f, 0.5f));
    zone->SetFogColor(Color(0.4f, 0.5f, 0.8f));
    zone->SetFogStart(100.0f);
    zone->SetFogEnd(300.0f);

    boardNode_ = scene_->CreateChild();
    boardNode_->SetPosition(Vector3(0.0f, 0.0f, 40.0f));

    //CreateBoard();

    Graphics* graphics = GetSubsystem<Graphics>();
    float screenWidth = (float)graphics->GetWidth();
    float screenHeight = (float)graphics->GetHeight();

    /*UIElement* root = GetSubsystem<UI>()->GetRoot();
    Sprite* crosshairSprite = new Sprite(context_);
    crosshairSprite->SetTexture(cache->GetResource<Texture2D>("Textures/Crosshair.png"));
    root->AddChild(crosshairSprite);
    crosshairSprite->SetPosition(screenWidth / 2, screenHeight / 2);
    crosshairSprite->SetSize(IntVector2(128, 128));
    crosshairSprite->SetHotSpot(IntVector2(64, 64));
    crosshairSprite->SetScale(0.5f);*/

    cameraTarget_ = boardNode_->GetPosition();

    cameraNode_ = scene_->CreateChild("Camera");
    cameraNode_->CreateComponent<Camera>();
    cameraNode_->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
    cameraNode_->LookAt(cameraTarget_);

    //CreateButton3D(Vector3(0, 0, 4));

/*    Node* ballNode = scene_->CreateChild();
    Ball* ball = ballNode->CreateComponent<Ball>();
    ball->Init(BC_WHITE);
    ballNode->SetPosition(Vector3(0, 0, 40));*/
 
}

void Game::CreateButton3D(Vector3 pos)
{
/*    ResourceCache* cache = GetSubsystem<ResourceCache>();
    Node* node = scene_->CreateChild();
    node->SetPosition(pos);
    node->SetScale(0.3f);
    node->SetRotation(Quaternion(-90, 0, 0));
    StaticModel* staticModel = node->CreateComponent<StaticModel>();
    staticModel->SetModel(buttonBottomModel_);
    staticModel->SetMaterial(buttonBottomMaterial_);
    staticModel->SetCastShadows(true);
    Node* textNode = node->CreateChild();
    textNode->SetPosition(Vector3(0, 0.0, 0.0));
    textNode->SetRotation(Quaternion(90, 0, 0));
    textNode->SetScale(3);
    Text3D* text = textNode->CreateComponent<Text3D>();
    text->SetFont(cache->GetResource<Font>("Fonts/BlueHighway.ttf"), 40);
    //text->SetTextAlignment(HorizontalAlignment::HA_CENTER);
    text->SetAlignment(HA_CENTER, VA_CENTER);
    text->SetText("Privet");
    Node* nodeTop = node->CreateChild();
    staticModel = nodeTop->CreateComponent<StaticModel>();
    staticModel->SetModel(buttonTopModel_);
    staticModel->SetMaterial(buttonTopMaterial_);
    //staticModel->SetCastShadows(true);
    */
}

void Game::MoveCamera(float timeStep)
{
    Input* input = GetSubsystem<Input>();

    const float MOVE_SPEED = 20.0f;
    const float MOUSE_SENSITIVITY = 0.1f;

    Graphics* graphics = GetSubsystem<Graphics>();

    float screenWidth = (float)graphics->GetWidth();
    float screenHeight = (float)graphics->GetHeight();
    IntVector2 mousePos = input->GetMousePosition();
    float deltaX = mousePos.x_ / screenWidth - 0.5f;
    float deltaY = mousePos.y_ / screenHeight - 0.5f;

    cameraTarget_ = boardNode_->GetPosition();
    cameraTarget_.x_ += MOUSE_SENSITIVITY * deltaX * cameraTarget_.z_;
    cameraTarget_.y_ -= MOUSE_SENSITIVITY * deltaY * cameraTarget_.z_;

    //LOGWARNING("MOUSE POS" + String(mousePos.y_));

/*    IntVector2 mouseMove = input->GetMouseMove();
    cameraTarget_.x_ += MOUSE_SENSITIVITY * mouseMove.x_;
    cameraTarget_.y_ -= MOUSE_SENSITIVITY * mouseMove.y_;*/
    //pitch_ = Clamp(pitch_, 0.0f, 180.0f);

    //cameraNode_->SetWorldRotation(Quaternion(pitch_, roll_, 0.0f));
    cameraNode_->LookAt(cameraTarget_);
    // сделать LoocTO а мышкой двигать точку зрения
    /*
    if (input->GetKeyDown('W'))
        boardNode_->Translate(Vector3::BACK * MOVE_SPEED * timeStep, TS_WORLD);
    if (input->GetKeyDown('S'))
        boardNode_->Translate(Vector3::FORWARD * MOVE_SPEED * timeStep, TS_WORLD);*/

    if (input->GetKeyPress(KEY_F2))
        GetSubsystem<DebugHud>()->ToggleAll();

/*    if (input->GetKeyPress(KEY_SPACE))
    {
        boardNode_->RemoveAllChildren();
        boardNode_->RemoveAllComponents();
        Board* board = boardNode_->CreateComponent<Board>();
        //board->Init(D_MASTER);
        //board->Init(D_EASY);
        board->Init(D_NORMAL);
//        cameraTarget_ = boardNode_->GetPosition();
    }*/

    if (input->GetKeyPress(KEY_ESC))
    {
        window_->SetVisible(!window_->IsVisible());
    }


    if (input->GetMouseButtonPress(MOUSEB_LEFT))
    {
        Vector3 hitPos;
        Drawable* hitDrawable;
        if (Raycast(300.0f, hitPos, hitDrawable))
        {
            Node* hitNode = hitDrawable->GetNode();
            if (hitNode->GetName() == "Ball")
            {
                Ball* ball = hitNode->GetComponent<Ball>();
                Board* board = boardNode_->GetComponent<Board>();
                board->HandleClickBall(ball);
            }
            else if (hitNode->GetName() == "Cell")
            {
                Cell* cell = hitNode->GetComponent<Cell>();
                Board* board = boardNode_->GetComponent<Board>();
                board->HandleClickCell(cell);
            }
            else if (hitNode->GetName() == "Selection")
            {
                Board* board = boardNode_->GetComponent<Board>();
                board->HandleClickSelection();
            }
        }
    }

}


void Game::SubscribeToEvents()
{
    SubscribeToEvent(E_UPDATE, HANDLER(Game, HandleUpdate));
    SubscribeToEvent(E_POSTRENDERUPDATE, HANDLER(Game, HandlePostRenderUpdate));

}


void Game::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
    using namespace Update;

    float timeStep = eventData[P_TIMESTEP].GetFloat();

    MoveCamera(timeStep);
}



void Game::HandlePostRenderUpdate(StringHash eventType, VariantMap& eventData)
{
    //GetSubsystem<Renderer>()->DrawDebugGeometry(true);

}


bool Game::Raycast(float maxDistance, Vector3& hitPos, Drawable*& hitDrawable)
{
    hitDrawable = nullptr;

    UI* ui = GetSubsystem<UI>();
    IntVector2 pos = ui->GetCursorPosition();
    
    //if (!ui->GetCursor()->IsVisible() || ui->GetElementAt(pos, true))
    //    return false;

    Graphics* graphics = GetSubsystem<Graphics>();
    Camera* camera = cameraNode_->GetComponent<Camera>();
    Ray cameraRay = camera->GetScreenRay((float)pos.x_ / graphics->GetWidth(), (float)pos.y_ / graphics->GetHeight());
    PODVector<RayQueryResult> results;
    RayOctreeQuery query(results, cameraRay, RAY_TRIANGLE, maxDistance, DRAWABLE_GEOMETRY);
    scene_->GetComponent<Octree>()->RaycastSingle(query);
    if (results.Size())
    {
        RayQueryResult& result = results[0];
        hitPos = result.position_;
        hitDrawable = result.drawable_;
        return true;
    }

    return false;
}
