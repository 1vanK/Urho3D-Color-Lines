#pragma once


class Game : public Application
{
    OBJECT(Game);

public:
    Game(Context* context);

    virtual void Setup();
    virtual void Start();

    void ClearBoard();
    void CreateBoard();



protected:
    void SetLogoVisible(bool enable);
    SharedPtr<Scene> scene_;
    SharedPtr<Node> cameraNode_;
    SharedPtr<Node> lightNode_;
    Vector3 cameraTarget_;
    IntVector2 RandomEmptyCell();
    void CreateBall(BallColor color, int x, int y);
    void CreateBall(BallColor color, IntVector2 cell);
    Vector3 CellToWorld(int x, int y);
    void CreateButton3D(Vector3 pos);

private:
    void CreateScene();
    void SetupViewport();
    void MoveCamera(float timeStep);
    void SubscribeToEvents();
    void HandleUpdate(StringHash eventType, VariantMap& eventData);
    void HandlePostRenderUpdate(StringHash eventType, VariantMap& eventData);
    bool Raycast(float maxDistance, Vector3& hitPos, Drawable*& hitDrawable);
    void CreateUI();
    void CreateUIWindow();
    Window* window_;
    void HandleClosePressed(StringHash eventType, VariantMap& eventData);
    void HandleExitPressed(StringHash eventType, VariantMap& eventData);
    void HandleEasyPressed(StringHash eventType, VariantMap& eventData);
    void HandleNormalPressed(StringHash eventType, VariantMap& eventData);
    void HandleHardPressed(StringHash eventType, VariantMap& eventData);
    void HandleExpertPressed(StringHash eventType, VariantMap& eventData);
    void HandleMasterPressed(StringHash eventType, VariantMap& eventData);

    Node* boardNode_;
};
