#include "Urho3DAll.h"
#include "Resources.h"


Model* CellModel;
Material* CellMaterial;


Model* SelectionModel;
Material* SelectionMaterial0;
Material* SelectionMaterial1;


Model* BallModel;
Material* WhiteBallMaterial;
Material* BlackBallMaterial;
Material* RedBallMaterial;
Material* OrangeBallMaterial;
Material* YellowBallMaterial;
Material* GreenBallMaterial;
Material* BlueBallMaterial;
Material* IndigoBallMaterial;
Material* VioletBallMaterial;


static void CreateBallMaterials(ResourceCache* cache)
{
    Material* ballMaterial = cache->GetResource<Material>("Materials/Ball.xml");

    SharedPtr<Material> newMaterial(ballMaterial->Clone());
    newMaterial->SetName("WhiteBall");
    cache->AddManualResource(newMaterial);
    WhiteBallMaterial = newMaterial;

    newMaterial = ballMaterial->Clone();
    newMaterial->SetName("BlackBall");
    newMaterial->SetShaderParameter("MatDiffColor", Color::BLACK);
    cache->AddManualResource(newMaterial);
    BlackBallMaterial = newMaterial;

    newMaterial = ballMaterial->Clone();
    newMaterial->SetName("RedBall");
    newMaterial->SetShaderParameter("MatDiffColor", Color(1.0f, 0.0f, 0.0f));
    cache->AddManualResource(newMaterial);
    RedBallMaterial = newMaterial;

    newMaterial = ballMaterial->Clone();
    newMaterial->SetName("OrangeBall");
    newMaterial->SetShaderParameter("MatDiffColor", Color(1.0f, 0.5f, 0.0f));
    cache->AddManualResource(newMaterial);
    OrangeBallMaterial = newMaterial;

    newMaterial = ballMaterial->Clone();
    newMaterial->SetName("YellowBall");
    newMaterial->SetShaderParameter("MatDiffColor", Color(1.0f, 1.0f, 0.0f));
    cache->AddManualResource(newMaterial);
    YellowBallMaterial = newMaterial;

    newMaterial = ballMaterial->Clone();
    newMaterial->SetName("GreenBall");
    newMaterial->SetShaderParameter("MatDiffColor", Color(0.0f, 1.0f, 0.0f));
    cache->AddManualResource(newMaterial);
    GreenBallMaterial = newMaterial;

    newMaterial = ballMaterial->Clone();
    newMaterial->SetName("BlueBall");
    newMaterial->SetShaderParameter("MatDiffColor", Color(0.0f, 1.0f, 1.0f));
    cache->AddManualResource(newMaterial);
    BlueBallMaterial = newMaterial;

    newMaterial = ballMaterial->Clone();
    newMaterial->SetName("IndigoBall");
    newMaterial->SetShaderParameter("MatDiffColor", Color(0.0f, 0.0f, 1.0f));
    cache->AddManualResource(newMaterial);
    IndigoBallMaterial = newMaterial;

    newMaterial = ballMaterial->Clone();
    newMaterial->SetName("VioletBall");
    newMaterial->SetShaderParameter("MatDiffColor", Color(1.0f, 0.0f, 1.0f));
    cache->AddManualResource(newMaterial);
    VioletBallMaterial = newMaterial;
}


void LoadResources(ResourceCache* cache)
{
    CellModel = cache->GetResource<Model>("Models/Cell.mdl");
    CellMaterial = cache->GetResource<Material>("Materials/Cell.xml");

    SelectionModel = cache->GetResource<Model>("Models/Selection.mdl");
    SelectionMaterial0 = cache->GetResource<Material>("Materials/SelectionWhite.xml");
    SelectionMaterial1 = cache->GetResource<Material>("Materials/SelectionBlack.xml");
    
    BallModel = cache->GetResource<Model>("Models/Ball.mdl");
    CreateBallMaterials(cache);
}
