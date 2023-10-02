#include "model.h"
#ifdef IOS_REF
#undef  IOS_REF
#define IOS_REF (*(pManager->GetIOSettings()))
#endif
Model::Model()
{
}

Model::~Model()
{
}
void Model::AddMesh(Mesh* pmesh) {
	m_pmesharr.push_back(pmesh);
}

static void InitializeSdkObjects(FbxManager*& pManager, FbxScene*& pScene);
static void DestroySdkObjects(FbxManager* pManager, bool pExitStatus);
static Mesh* GetContent(const FbxNode* pNode);
static Model GetSceneContent(const FbxScene* pScene);
static bool LoadScene(FbxManager* pManager, FbxDocument* pScene, const char* pFilename);

static bool LoadScene(FbxManager* pManager, FbxDocument* pScene, const char* pFilename)
{
	int lFileMajor, lFileMinor, lFileRevision;
	int lSDKMajor, lSDKMinor, lSDKRevision;
	//int lFileFormat = -1;
	int lAnimStackCount;
	bool lStatus;
	char lPassword[1024];

	// Get the file version number generate by the FBX SDK.
	FbxManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);

	// Create an importer.
	FbxImporter* lImporter = FbxImporter::Create(pManager, "");

	// Initialize the importer by providing a filename.
	const bool lImportStatus = lImporter->Initialize(pFilename, -1, pManager->GetIOSettings());
	lImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);

	if (!lImportStatus)
	{
		FbxString error = lImporter->GetStatus().GetErrorString();
		FBXSDK_printf("Call to FbxImporter::Initialize() failed.\n");
		FBXSDK_printf("Error returned: %s\n\n", error.Buffer());

		if (lImporter->GetStatus().GetCode() == FbxStatus::eInvalidFileVersion)
		{
			FBXSDK_printf("FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);
			FBXSDK_printf("FBX file format version for file '%s' is %d.%d.%d\n\n", pFilename, lFileMajor, lFileMinor, lFileRevision);
		}

		return false;
	}

	FBXSDK_printf("FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);

	if (lImporter->IsFBX())
	{
		FBXSDK_printf("FBX file format version for file '%s' is %d.%d.%d\n\n", pFilename, lFileMajor, lFileMinor, lFileRevision);

		// From this point, it is possible to access animation stack information without
		// the expense of loading the entire file.

		//FBXSDK_printf("Animation Stack Information\n");
		//
		//lAnimStackCount = lImporter->GetAnimStackCount();
		//
		//FBXSDK_printf("    Number of Animation Stacks: %d\n", lAnimStackCount);
		//FBXSDK_printf("    Current Animation Stack: \"%s\"\n", lImporter->GetActiveAnimStackName().Buffer());
		//FBXSDK_printf("\n");
		//
		//for(int i = 0; i < lAnimStackCount; i++)
		//{
		//    FbxTakeInfo* lTakeInfo = lImporter->GetTakeInfo(i);
		//
		//    FBXSDK_printf("    Animation Stack %d\n", i);
		//    FBXSDK_printf("         Name: \"%s\"\n", lTakeInfo->mName.Buffer());
		//    FBXSDK_printf("         Description: \"%s\"\n", lTakeInfo->mDescription.Buffer());
		//
		//    // Change the value of the import name if the animation stack should be imported 
		//    // under a different name.
		//    FBXSDK_printf("         Import Name: \"%s\"\n", lTakeInfo->mImportName.Buffer());
		//
		//    // Set the value of the import state to false if the animation stack should be not
		//    // be imported. 
		//    FBXSDK_printf("         Import State: %s\n", lTakeInfo->mSelect ? "true" : "false");
		//    FBXSDK_printf("\n");
		//}

		// Set the import states. By default, the import states are always set to 
		// true. The code below shows how to change these states.
		IOS_REF.SetBoolProp(IMP_FBX_MATERIAL, true);
		IOS_REF.SetBoolProp(IMP_FBX_TEXTURE, true);
		IOS_REF.SetBoolProp(IMP_FBX_LINK, true);
		IOS_REF.SetBoolProp(IMP_FBX_SHAPE, true);
		IOS_REF.SetBoolProp(IMP_FBX_GOBO, true);
		IOS_REF.SetBoolProp(IMP_FBX_ANIMATION, true);
		IOS_REF.SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
	}

	// Import the scene.
	lStatus = lImporter->Import(pScene);
	if (lStatus == false && lImporter->GetStatus() == FbxStatus::ePasswordError)
	{
		FBXSDK_printf("Please enter password: ");

		lPassword[0] = '\0';

		FBXSDK_CRT_SECURE_NO_WARNING_BEGIN
			scanf("%s", lPassword);
		FBXSDK_CRT_SECURE_NO_WARNING_END

			FbxString lString(lPassword);

		IOS_REF.SetStringProp(IMP_FBX_PASSWORD, lString);
		IOS_REF.SetBoolProp(IMP_FBX_PASSWORD_ENABLE, true);

		lStatus = lImporter->Import(pScene);

		if (lStatus == false && lImporter->GetStatus() == FbxStatus::ePasswordError)
		{
			FBXSDK_printf("\nPassword is wrong, import aborted.\n");
		}
	}

	if (!lStatus || (lImporter->GetStatus() != FbxStatus::eSuccess))
	{
		FBXSDK_printf("********************************************************************************\n");
		if (lStatus)
		{
			FBXSDK_printf("WARNING:\n");
			FBXSDK_printf("   The importer was able to read the file but with errors.\n");
			FBXSDK_printf("   Loaded scene may be incomplete.\n\n");
		}
		else
		{
			FBXSDK_printf("Importer failed to load the file!\n\n");
		}

		if (lImporter->GetStatus() != FbxStatus::eSuccess)
			FBXSDK_printf("   Last error message: %s\n", lImporter->GetStatus().GetErrorString());

		FbxArray<FbxString*> history;
		lImporter->GetStatus().GetErrorStringHistory(history);
		if (history.GetCount() > 1)
		{
			FBXSDK_printf("   Error history stack:\n");
			for (int i = 0; i < history.GetCount(); i++)
			{
				FBXSDK_printf("      %s\n", history[i]->Buffer());
			}
		}
		FbxArrayDelete<FbxString*>(history);
		FBXSDK_printf("********************************************************************************\n");
	}

	// Destroy the importer.
	lImporter->Destroy();

	return lStatus;
}
static void InitializeSdkObjects(FbxManager*& pManager, FbxScene*& pScene)
{
	//The first thing to do is to create the FBX Manager which is the object allocator for almost all the classes in the SDK
	pManager = FbxManager::Create();
	if (!pManager)
	{
		FBXSDK_printf("Error: Unable to create FBX Manager!\n");
		exit(1);
	}
	else FBXSDK_printf("Autodesk FBX SDK version %s\n", pManager->GetVersion());

	//Create an IOSettings object. This object holds all import/export settings.
	FbxIOSettings* ios = FbxIOSettings::Create(pManager, IOSROOT);
	pManager->SetIOSettings(ios);

	//Load plugins from the executable directory (optional)
	FbxString lPath = FbxGetApplicationDirectory();
	pManager->LoadPluginsDirectory(lPath.Buffer());

	//Create an FBX scene. This object holds most objects imported/exported from/to files.
	pScene = FbxScene::Create(pManager, "My Scene");
	if (!pScene)
	{
		FBXSDK_printf("Error: Unable to create FBX scene!\n");
		exit(1);
	}
}
static void DestroySdkObjects(FbxManager* pManager, bool pExitStatus)
{
	//Delete the FBX Manager. All the objects that have been allocated using the FBX Manager and that haven't been explicitly destroyed are also automatically destroyed.
	if (pManager) pManager->Destroy();
	//if( pExitStatus ) FBXSDK_printf("Program Success!\n");
}
static Mesh* GetContent(const FbxNode* pNode) {
	FbxMesh* pMesh = (FbxMesh*)pNode->GetNodeAttribute();
	FbxVector4* controlPoints = pMesh->GetControlPoints();
	int controlPointsCount = pMesh->GetControlPointsCount();
	FbxVector4 translate = pNode->LclTranslation.Get();
	FbxVector4 scaling = pNode->LclScaling.Get();
	FbxVector4 rotation = pNode->LclRotation.Get();
	FbxAMatrix transform = FbxAMatrix(translate * 0.01f, FbxVector4(0,0,0,0), FbxVector4(1.0f,1.0f,1.0f,1.0f));
	int* temp = pMesh->GetPolygonVertices();
	uint32_t verticesIndexArrLength = pMesh->GetPolygonVertexCount();
	uint32_t* verticesIndexArr = new uint32_t[verticesIndexArrLength];
	memcpy(verticesIndexArr, temp, sizeof(uint32_t) * verticesIndexArrLength);
	FbxVector4* positions = pMesh->GetControlPoints();

	fbxsdk::FbxGeometryElementNormal* pNormal = pMesh->GetElementNormal(0);
	vector3* normals = new vector3[verticesIndexArrLength];
	FbxVector4 normal;
	for (int i = 0; i < verticesIndexArrLength; i++) {
		if (pNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex) {
			int id = -1;
			if (pNormal->GetReferenceMode() == FbxGeometryElement::eDirect) {
				id = i;
			}
			else if (pNormal->GetReferenceMode() == FbxGeometryElement::eIndexToDirect) {
				id = pNormal->GetIndexArray().GetAt(i);
			}
			normal = pNormal->GetDirectArray().GetAt(id);
			normals[i].x = normal[0];
			normals[i].y = normal[1];
			normals[i].z = normal[2];
		}
	}

	fbxsdk::FbxGeometryElementUV* pUV = pMesh->GetElementUV(0);
	vector2* uvs = new vector2[verticesIndexArrLength];
	FbxVector4 uv;
	for (int i = 0; i < verticesIndexArrLength; i++) {
		if (pUV->GetMappingMode() == FbxGeometryElement::eByPolygonVertex) {
			int id = -1;
			if (pUV->GetReferenceMode() == FbxGeometryElement::eDirect) {
				id = i;
			}
			else if (pUV->GetReferenceMode() == FbxGeometryElement::eIndexToDirect) {
				id = pUV->GetIndexArray().GetAt(i);
			}
			uv = pUV->GetDirectArray().GetAt(id);
			uvs[i].x = uv[0];
			uvs[i].y = uv[1];
		}
	}

	fbxsdk::FbxGeometryElementTangent* pTangents = pMesh->GetElementTangent(0);
	if (pTangents == nullptr) {
		pTangents = pMesh->CreateElementTangent();
		pTangents->SetMappingMode(FbxGeometryElement::eByPolygonVertex);
		pTangents->SetReferenceMode(FbxGeometryElement::eDirect);
		bool status = pMesh->GenerateTangentsDataForAllUVSets(pTangents);
		if (!status) {
			FBXSDK_printf("Warning: tangent calculation failed, all tangents set to (0,0,0,0).");
		}
	}
	vector4* tangents = new vector4[verticesIndexArrLength];
	FbxVector4 tangent;
	for (int i = 0; i < verticesIndexArrLength; i++) {
		if (pTangents->GetMappingMode() == FbxGeometryElement::eByPolygonVertex) {
			int id = -1;
			if (pTangents->GetReferenceMode() == FbxGeometryElement::eDirect) {
				id = i;
			}
			else if (pTangents->GetReferenceMode() == FbxGeometryElement::eIndexToDirect) {
				id = pTangents->GetIndexArray().GetAt(i);
			}
			tangent = pTangents->GetDirectArray().GetAt(id);
			tangents[i].x = tangent[0];
			tangents[i].y = tangent[1];
			tangents[i].z = tangent[2];
			tangents[i].w = tangent[3];
			//std::cout << normals[i].x << normals[i].y << normals[i].z << std::endl;
		}
	}

	vector3* mpositions = new vector3[controlPointsCount];
	for (int i = 0; i < controlPointsCount; i++) {
		FbxVector4 pos = (*(positions + i)) + translate;
		
		mpositions[i].x = pos[0];
		mpositions[i].y = pos[1];
		mpositions[i].z = pos[2];
	}
	const char* name = pNode->GetName();
	Mesh* pmesh = new Mesh(mpositions, verticesIndexArr, normals, uvs,
		tangents, verticesIndexArrLength, pMesh->GetPolygonCount(), name);
	return pmesh;
}
static Model GetSceneContent(const FbxScene* pScene) {
	FbxNode* lNode = pScene->GetRootNode();
	//int rootNodeNum = pScene->GetNodeCount();
	Model model;
	//for (int no = 0; no < rootNodeNum; no++) {
		if (lNode) {
			//std::cout << (lNode)->GetName() << std::endl;
			for (int i = 0; i < (lNode)->GetChildCount(); i++) {
				Mesh* pmesh = GetContent((lNode)->GetChild(i));
				model.AddMesh(pmesh);
			}
		}
	//}
	return model;
}

Model Model::LoadModel(const char* filename)
{
	FbxManager* lSdkManager = nullptr;
	FbxScene* lScene = nullptr;
	bool lResult;

	InitializeSdkObjects(lSdkManager, lScene);
	FbxGeometryConverter clsConverter(lSdkManager);
	FbxString lFilePath(filename);

	if (lFilePath.IsEmpty()) {
		lResult = false;
		FBXSDK_printf("\n\nUsage: ImportScene <FBX file name>\n\n");
	}
	else {
		//FBXSDK_printf("\n\nFile: %s\n\n", lFilePath.Buffer());
		lResult = LoadScene(lSdkManager, lScene, lFilePath.Buffer());
	}
	if (lResult == false) {
		FBXSDK_printf("\n\nAn error occurred while loading the scene...");
	}
	else {
		if (!clsConverter.Triangulate(lScene, true)) {
			FBXSDK_printf("\n\nFbx cannot be triangulated, render failed...");
		}
		fbxsdk::FbxAxisSystem opengl = fbxsdk::FbxAxisSystem::eOpenGL;
		opengl.ConvertScene(lScene);
		//FBXSDK_printf(laxis);
	}
	Model model = GetSceneContent(lScene);
	DestroySdkObjects(lSdkManager, lResult);
	return model;
}
