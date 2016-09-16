
#include "readfile.h"

string curfilename;


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

void readfile(const char* filename)
{
	string str, cmd;
	ifstream in;
	in.open(filename);
	if (in.is_open()) {
			Matieral defaultMatieral;
		// I need to implement a matrix stack to store transforms.  
		// This is done using standard STL Templates 
		//stack <mat4> transfstack;   //这个矩阵堆栈主要是与输入格式配合
		//transfstack.push(mat4(1.0));  // identity
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
						Light l = Light(vec3(values[0], values[1], values[2]), MyColor(values[3],values[4],values[5],0), DefaultLightIntensity, LightType::Point);
						curScene.LightArray.push_back(l);
					}
					
				}
				else if (cmd == "directional")
				{
					validinput = readvals(s, 6, values); // Position/color for lts.
					if (validinput)
					{
						//原本的光的位置向量作为方向向量
						Light l = Light(vec3(values[0], values[1], values[2]), MyColor(values[3],values[4],values[5],0 ), DefaultLightIntensity, LightType::Dirctional);
						curScene.LightArray.push_back(l);
					}
				}

				else if (cmd == "ambient") {
					validinput = readvals(s, 3, values); // Position/color for lts.
					if (validinput)
					{
						//curScene.defaultMatieral.ambient = MyColor((BYTE)((BYTE)(values[0] * 255)), (BYTE)((BYTE)(values[1] * 255)), (BYTE)((BYTE)(values[2] * 255)), 0);
						curScene.defaultMatieral.ambient = MyColor(values[0], values[1], values[2], 0);
						/*
						Light l = Light(vec3(0,0,0), MyColor( (BYTE)(values[0] * 255),(BYTE)(values[1] * 255),(BYTE)(values[2] * 255),0 ), DefaultLightIntensity, LightType::ambient);
						curScene.LightArray.push_back(l);
						*/
					}
				}
				else if (cmd == "diffuse") {
					validinput = readvals(s, 3, values); // Position/color for lts.
					if (validinput)
					{
						
						//curScene.defaultMatieral.diffuse = MyColor( (BYTE)((BYTE)(values[0] * 255)), (BYTE)((BYTE)(values[1] * 255)), (BYTE)((BYTE)(values[2] * 255)),0 );
						curScene.defaultMatieral.diffuse = MyColor(values[0], values[1], values[2], 0);
					}
					

				}
				else if (cmd == "specular") {
					validinput = readvals(s, 3, values); // Position/color for lts.
					if (validinput)
					{
						
						//curScene.defaultMatieral.specular = MyColor( (BYTE)(values[0] * 255), (BYTE)(values[1] * 255), (BYTE)(values[2] * 255),0 );
						curScene.defaultMatieral.specular = MyColor(values[0], values[1], values[2], 0);
					}
				}
				else if (cmd == "emission") {
					validinput = readvals(s, 3, values); // Position/color for lts.
					if (validinput)
					{
						
						//curScene.defaultMatieral.emission = MyColor( (BYTE)(values[0] * 255), (BYTE)(values[1] * 255), (BYTE)(values[2] * 255),0 );
						curScene.defaultMatieral.emission = MyColor(values[0], values[1], values[2], 0);
					}
				}
				else if (cmd == "shininess") {
					validinput = readvals(s, 1, values); // Position/color for lts.
					if (validinput)
					{
						
						curScene.defaultMatieral.shininess = values[0];
					}
				}
				else if (cmd == "size") {
					validinput = readvals(s, 2, values); // Position/color for lts.
					if (validinput)
					{
						ScreenWidth = values[0];
						ScreenHeight = values[1];
					}
				}
				else if (cmd == "camera") {
					validinput = readvals(s, 10, values); // Position/color for lts.
					if (validinput)
					{
						curCamera = Camera(values[9], values[9], 1, vec3(values[0], values[1], values[2]), vec3(values[3], values[4], values[5]), vec3(values[6], values[7], values[8]));
					}
				}

				else if (cmd == "sphere" || cmd == "vertex" || cmd == "tri") {

					if (cmd == "sphere")
					{
						curScene.MatieralArray.push_back(curScene.defaultMatieral);
						validinput = readvals(s, 4, values); // Position/color for lts.
						if (validinput)
						{
							Sphere* s = new Sphere(vec3(values[0], values[1], values[2]), values[3], curScene.MatieralArray[curScene.MatieralArray.size()-1], TransformStack.top());
							//TransformStack.top().TransVec(s.pos);
							//curScene.sphArray.push_back(s);
							curScene.ObjectArray.push_back(s);
							
						}
					}
					else if (cmd == "vertex")
					{
						validinput = readvals(s, 3, values); // Position/color for lts.
						if (validinput)
						{
							vec3 vertex = vec3(values[0], values[1], values[2]);
							curScene.vertexArray.push_back(vertex);
						}
					}
					else if(cmd == "tri")
					{
						curScene.MatieralArray.push_back(curScene.defaultMatieral);
						validinput = readvals(s, 3, values); // Position/color for lts.
						if (validinput)
						{
							Triangle* tri = new Triangle(curScene.vertexArray[values[0]], curScene.vertexArray[values[1]], curScene.vertexArray[values[2]], curScene.MatieralArray[curScene.MatieralArray.size() - 1], TransformStack.top());
							//curScene.triArray.push_back(tri);
							curScene.ObjectArray.push_back(tri);

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
						mt = Rotate(values[3],vec3(values[0], values[1], values[2]));
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

