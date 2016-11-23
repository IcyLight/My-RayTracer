
#include "readfile.h"

string curfilename;

//LoadMode loadMode = LoadMode::HwLoad;

bool readvals(stringstream &s, const int numvals, float* values)
{
	for (int i = 0; i < numvals; i++) {
		s >> values[i];
		if (s.fail()) {
			cout << "Failed reading value " << i << " will skip\n";
			return false;
		}
	}
	return true;
}
void HWload(const char* filename, Scene* scene)
{
	string str, cmd;
	ifstream in;
	in.open(filename);
	if (in.is_open()) {
		Matieral curMatieral;
		TransformStack.push(MyTransform(mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1)));
		getline(in, str);
		while (in) {
			if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#')) {
				// Ruled out comment and blank lines 

				stringstream s(str);
				s >> cmd;
				int i;
				float values[10];

				bool validinput;



				if (cmd == "point") {
					validinput = readvals(s, 6, values); // Position/color for lts.
					if (validinput)
					{
						Light* l = new Light(vec3(values[0], values[1], values[2]), MyColor(values[3], values[4], values[5], 0), LightType::Point);
						scene->LightArray.push_back(l);
					}

				}
				else if (cmd == "directional")
				{
					validinput = readvals(s, 6, values); // Position/color for lts.
					if (validinput)
					{
						//原本的光的位置向量作为方向向量
						Light* l = new Light(vec3(values[0], values[1], values[2]), MyColor(values[3], values[4], values[5], 0), LightType::Dirctional);
						scene->LightArray.push_back(l);
					}
				}

				else if (cmd == "ambient") {
					validinput = readvals(s, 3, values); // Position/color for lts.
					if (validinput)
					{
						//curMatieral.ambient = MyColor((BYTE)((BYTE)(values[0] * 255)), (BYTE)((BYTE)(values[1] * 255)), (BYTE)((BYTE)(values[2] * 255)), 0);
						curMatieral.ambient = MyColor(values[0], values[1], values[2], 0);
						/*
						Light l = Light(vec3(0,0,0), MyColor( (BYTE)(values[0] * 255),(BYTE)(values[1] * 255),(BYTE)(values[2] * 255),0 ), DefaultLightIntensity, LightType::ambient);
						scene->LightArray.push_back(l);
						*/
					}
				}
				else if (cmd == "diffuse") {
					validinput = readvals(s, 3, values); // Position/color for lts.
					if (validinput)
					{

						//curMatieral.diffuse = MyColor( (BYTE)((BYTE)(values[0] * 255)), (BYTE)((BYTE)(values[1] * 255)), (BYTE)((BYTE)(values[2] * 255)),0 );
						curMatieral.diffuse = MyColor(values[0], values[1], values[2], 0);
					}


				}
				else if (cmd == "specular") {
					validinput = readvals(s, 3, values); // Position/color for lts.
					if (validinput)
					{

						//curMatieral.specular = MyColor( (BYTE)(values[0] * 255), (BYTE)(values[1] * 255), (BYTE)(values[2] * 255),0 );
						curMatieral.specular = MyColor(values[0], values[1], values[2], 0);
					}
				}
				else if (cmd == "emission") {
					validinput = readvals(s, 3, values); // Position/color for lts.
					if (validinput)
					{

						//curMatieral.emission = MyColor( (BYTE)(values[0] * 255), (BYTE)(values[1] * 255), (BYTE)(values[2] * 255),0 );
						curMatieral.emission = MyColor(values[0], values[1], values[2], 0);
					}
				}
				else if (cmd == "shininess") {
					validinput = readvals(s, 1, values); // Position/color for lts.
					if (validinput)
					{

						curMatieral.shininess = values[0];
					}
				}
				else if (cmd == "size") {
					validinput = readvals(s, 2, values); // Position/color for lts.
					if (validinput)
					{
						HWCamera.ScreenWidth = values[0];
						HWCamera.ScreenHeight = values[1];
					}
				}
				else if (cmd == "camera") {
					validinput = readvals(s, 10, values); // Position/color for lts.
					if (validinput)
					{
						HWCamera = Camera(values[9], values[9], 1, vec3(values[0], values[1], values[2]), vec3(values[3], values[4], values[5]), vec3(values[6], values[7], values[8]));
					}
				}

				else if (cmd == "sphere" || cmd == "vertex" || cmd == "tri") {

					if (cmd == "sphere")
					{
						scene->MatieralArray.push_back(new Matieral(curMatieral));
						validinput = readvals(s, 4, values); // Position/color for lts.
						if (validinput)
						{
							Sphere* s = new Sphere(vec3(values[0], values[1], values[2]), values[3], scene->MatieralArray[scene->MatieralArray.size() - 1], TransformStack.top());
							//TransformStack.top().TransVec(s.pos);
							//scene->sphArray.push_back(s);
							scene->GeometryArray.push_back(s);

						}
					}
					else if (cmd == "vertex")
					{
						validinput = readvals(s, 3, values); // Position/color for lts.
						if (validinput)
						{
							Vertex* vertex = new Vertex(vec3(values[0], values[1], values[2]));
							scene->vertexArray.push_back(vertex);
						}
					}
					else if (cmd == "tri")
					{

						scene->MatieralArray.push_back(new Matieral(curMatieral));
						validinput = readvals(s, 3, values); // Position/color for lts.
						if (validinput)
						{
							Triangle* tri = new Triangle(scene->vertexArray[values[0]], scene->vertexArray[values[1]], scene->vertexArray[values[2]], scene->MatieralArray[scene->MatieralArray.size() - 1], TransformStack.top());
							//scene->triArray.push_back(tri);
							scene->GeometryArray.push_back(tri);
						}
					}

				}

				else if (cmd == "vertexnormal" || cmd == "trinormal")  //作业不需要，以后再加上来
				{

				}

				else if (cmd == "translate") {
					validinput = readvals(s, 3, values); // Position/color for lts.
					if (validinput)
					{
						mat4 mt;
						mt = Translate(vec3(values[0], values[1], values[2]));
						TransformStack.top().trans = TransformStack.top().trans* mt;
					}

				}
				else if (cmd == "scale") {
					validinput = readvals(s, 3, values); // Position/color for lts.
					if (validinput)
					{
						mat4 mt;
						mt = Scale(vec3(values[0], values[1], values[2]));
						TransformStack.top().trans = TransformStack.top().trans* mt;


					}
				}
				else if (cmd == "rotate") {
					validinput = readvals(s, 4, values); // Position/color for lts.
					if (validinput)
					{
						mat4 mt;
						mt = Rotate(values[3], vec3(values[0], values[1], values[2]));
						TransformStack.top().trans = TransformStack.top().trans* mt;
						int i = 0;

					}
				}

				// I include the basic push/pop code for matrix stacks
				else if (cmd == "pushTransform") {
					TransformStack.push(TransformStack.top());
				}
				else if (cmd == "popTransform") {
					TransformStack.pop();
					if (TransformStack.size() == 0)
					{
						TransformStack.push(MyTransform());
					}
				}
				else if (cmd == "output") {
					string filename;
					s >> filename;
					curfilename = filename;
				}

				else {

				}
			}
			getline(in, str);
		}


	}
	else {
		cerr << "Unable to Open Input Data File " << filename << "\n";
		throw 2;
	}
}
void Objload(const char* filename, Scene* scene)
{
	string str, cmd;
	ifstream in ;
	
	in.open(filename);
	if (in.is_open()) {
		Matieral* curMatieral = nullptr;
		TransformStack.push(MyTransform(mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1)));
		getline(in, str);
		float values[9];
		while (in) {
			if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#'))
			{
				stringstream s(str);
				s >> cmd;

				if (cmd == "f")
				{
					//scene->MatieralArray.push_back(new Matieral(curMatieral));
					for (int i = 0; i < 3; i++)
					{
						Vertex* vet;
						int pos, uv, nor;
						string st;
						s >> st;
						sscanf(&st[0], "%d/%d/%d", &pos, &uv, &nor);
						vec3 vpos = pos>0?scene->vPosArray[pos - 1]:vec3(0,0,0);
						vec3 puv = uv>0?scene->vUVArray[uv - 1]:vec3(0,0,0);
						vec3 pnor = nor>0?scene->vNorArray[nor - 1]:vec3(0,0,0);
						vet = new Vertex( vpos, puv, pnor);
						scene->vertexArray.push_back(vet);
						int u = 0;
					}
					int top = scene->vertexArray.size();
					Vertex* v1 = scene->vertexArray[top - 1];
					Vertex* v2 = scene->vertexArray[top - 2];
					Vertex* v3 = scene->vertexArray[top - 3];
					Triangle* t = new Triangle(
						v1,v2,v3,
						curMatieral, TransformStack.top());
					scene->GeometryArray.push_back(t);
					scene->TriangleArray.push_back(t);
				}
				else if (cmd == "v")
				{
					bool validinput = readvals(s, 3, values);
					if (validinput)
					{
						scene->vPosArray.push_back(vec3(values[0], values[1], values[2]));
					}
					
				}
				else if (cmd == "vt")
				{
					bool validinput = readvals(s, 3, values);
					if (validinput)
					{
						scene->vUVArray.push_back(vec3(values[0], values[1], values[2]));
					}

				}
				else if (cmd == "vn")
				{
					bool validinput = readvals(s, 3, values);
					if (validinput)
					{
						scene->vNorArray.push_back(vec3(values[0], values[1], values[2]));
					}

				}
				else if (cmd =="mtllib")
				{
					string mtlAddress;
					s >> mtlAddress;
					mtlLoad(mtlAddress.data(), scene);
				}
				else if (cmd == "usemtl")
				{
					string mtlname;
					s >> mtlname;
					bool flag = false;
					for (vector<Matieral*>::iterator i = scene->MatieralArray.begin(); i != scene->MatieralArray.end(); i++)
					{
						if ((*i)->name == mtlname)
						{
							curMatieral = *i;
							flag = true;
						}
					}
					if (!flag)
					{

					}
				}
			}
			getline(in, str);
		}

	}
}

void mtlLoad(const char* filename, Scene* scene)
{
	string str, cmd;
	ifstream in;
	in.open(filename);
	if (in.is_open()) {
		getline(in, str);
		float values[9];
		Matieral* loadingMat = NULL;
		while (in) {
			if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#'))
			{
				stringstream s(str);
				s >> cmd;
				if (cmd == "newmtl")
				{
					string mtlname;
					s >> mtlname;
					loadingMat = new Matieral(mtlname);
					scene->MatieralArray.push_back(loadingMat);


				}
				else if (cmd == "Ka")  //ambient
				{
					bool validinput = readvals(s, 3, values);
					if (validinput)
					{
						loadingMat->ambient = MyColor(values[0], values[1], values[2],0);
					}
				}
				else if (cmd == "Kd") //diffuse
				{
					bool validinput = readvals(s, 3, values);
					if (validinput)
					{
						loadingMat->diffuse = MyColor(values[0], values[1], values[2], 0);
					}
				}
				else if (cmd == "Ks") //specular
				{
					bool validinput = readvals(s, 3, values);
					if (validinput)
					{
						loadingMat->specular = MyColor(values[0], values[1], values[2], 0);
					}
				}
				else if (cmd == "Ke") //emission
				{
					bool validinput = readvals(s, 3, values);
					if (validinput)
					{
						loadingMat->ambient = MyColor(values[0], values[1], values[2], 0);
					}
				}
				else if (cmd == "Tf") //transmission filter
				{

				}
				else if (cmd == "illum") //Light Model
				{
					/*
					model           Property Editor

					0		Color on and Ambient off
					1		Color on and Ambient on
					2		Highlight on
					3		Reflection on and Ray trace on
					4		Transparency: Glass on
					Reflection: Ray trace on
					5		Reflection: Fresnel on and Ray trace on
					6		Transparency: Refraction on
					Reflection: Fresnel off and Ray trace on
					7		Transparency: Refraction on
					Reflection: Fresnel on and Ray trace on
					8		Reflection on and Ray trace off
					9		Transparency: Glass on
					Reflection: Ray trace off
					10		Casts shadows onto invisible surfaces
					*/
				}
				else if (cmd == "d")
				{

				}
				else if (cmd == "Ns") //specular highligh
				{

				}
				else if (cmd == "Ni")
				{
					
				}
				else if (cmd == "map_Ka")
				{
					string address;
					s >> address;
					FIBITMAP* tex = FreeImage_Load(FIF_TARGA, address.data());
					int height = FreeImage_GetHeight(tex);
					int width = FreeImage_GetWidth(tex);
					loadingMat->map_Ka = new Texture(tex,width,height);
				}
				else if (cmd == "map_Kd")
				{
					string address;
					s >> address;
					
					FIBITMAP* tex = FreeImage_Load(FIF_TARGA, address.data());
					int height = FreeImage_GetHeight(tex);
					int width = FreeImage_GetWidth(tex);
					loadingMat->map_Kd = new Texture(tex, width, height);
				}
				else if (cmd == "map_Ks")
				{
					string address;
					s >> address;
					FIBITMAP* tex = FreeImage_Load(FIF_TARGA, address.data());
					int height = FreeImage_GetHeight(tex);
					int width = FreeImage_GetWidth(tex);
					loadingMat->map_Ks = new Texture(tex, width, height);
				}
				else if (cmd == "map_Ns")
				{
					
				}
				else if (cmd == "map_bump")
				{
					string address;
					s >> address;
					FIBITMAP* tex = FreeImage_Load(FIF_TARGA, address.data());
					int height = FreeImage_GetHeight(tex);
					int width = FreeImage_GetWidth(tex);
					loadingMat->map_bump = new Texture(tex, width, height);
				}

			}
			getline(in, str);
		}
	}
}


void readfile(const char* filename, Scene* scene,LoadMode loadMode)
{
	printf("读取模型与材质中。。。\n");
	if (loadMode == LoadMode::HwLoad)
	{
		HWload(filename,scene);
	}
	else if (loadMode == LoadMode::ObjLoad)
	{
		Objload(filename, scene);
	}
	printf("读取模型与材质读取完成\n");
	printf("构建BSP树中。。。\n");
	
	scene->bsptree = TriangleBSPTree(scene->TriangleArray,13,0.95,true);
	printf("BSP树构建完成\n");

}

