#pragma once
#include <vector>
#include <algorithm>
using namespace std;




enum BSP_Case
{
	front,behind,span,coincident
};

//SplitFunc返回分割出的形状数量,若错误则返回0
template<typename HyperPlane, typename Vertex, 
	BSP_Case(*GetRelationFunc)(const HyperPlane* object, const HyperPlane* HyperPlane), 
	void(*HyperPlane::SplitFunc)(const HyperPlane* plane, const HyperPlane* Object, vector<HyperPlane*>* Out_Objects, vector<Vertex*>* Out_Vertexs)>
class BSPNode
{
public:
	vector<HyperPlane*> frontObjects;
	vector<HyperPlane*> behindObjects;
	vector<HyperPlane*> spanObjects;
	
	//切割出来的三角形是由BSP树生成的，单独放在一个容器里，由BSP树负责释放内存
	vector<HyperPlane*> splitedFrontObjects;
	vector<HyperPlane*> splitedBehindObjects;

	HyperPlane* Object;
	BSPNode* frontNode;
	BSPNode* behindNode;

	BSPNode(){}

	BSPNode(vector<HyperPlane*> leafObjects,HyperPlane* plane,bool isSplit = false)
	{
		this->Object = plane;
		this->frontNode = nullptr;
		this->behindNode = nullptr;
		for (int i=0;i<leafObjects.size();++i)
		{

			BSP_Case relation = GetRelationFunc(leafObjects[i] , plane);
			switch (relation)
			{
			case BSP_Case::front:
				frontObjects.push_back(leafObjects[i]);
				break;
			case BSP_Case::behind:
				behindObjects.push_back(leafObjects[i]);
				break;
			case BSP_Case::coincident:
			case BSP_Case::span:
				if (isSplit)
				{
					vector<plane*> HyperPlaneArray;
					vector<Vertex*> VertexArray;

					leafObjects[i]::SplitFunc(HyperPlane, &p_Objects);
					for (int i = 0; i < ObjectCount; i++)
					{
						BSP_Case i_relation = GetRelationFunc(&p_Objects[i], plane);
						switch (i_relation)
						{
						case BSP_Case::front:
							splitedFrontObjects.push_back(&p_Objects[i]);
							break;
						case BSP_Case::behind:
							splitedBehindObjects.push_back(&p_Objects[i]);
							break;
						}

					}
					
					
				}
				else
				{
					spanObjects.push_back(leafObjects[i]);
				}
				break;
			default:
				break;
			}
		}
	}

	static HyperPlane* GetBlanceObject(const vector<HyperPlane*>* Objects, float blanceRatio)
	{
		if ((*Objects).empty())
		{
			return nullptr;
		}
		for (int i = 0; i < Objects->size(); ++i)
		{
			int frontCount = 0;
			int behindCount = 0;
			for (int j = 0; j < Objects->size(); ++j)
			{
				BSP_Case relation = GetRelationFunc( (*Objects)[j], (*Objects) [i]);
				switch (relation)
				{
				case BSP_Case::front:
					++frontCount;
					break;
				case BSP_Case::behind:
					++++behindCount;
					break;
				default:
					break;
				}
			}
			if (frontCount == 0 || behindCount == 0) continue;
			float ratio = frontCount > behindCount ? (float)behindCount / frontCount : (float)frontCount / behindCount;
			if (ratio > blanceRatio)
			{
				return (*Objects) [i];
			}
		}
		return (*Objects) [0];
	}
};


template<typename HyperPlane,typename Vertex,
	BSP_Case (*GetRelationFunc)(const HyperPlane* object,const HyperPlane* HyperPlane), 
	void(*HyperPlane::SplitFunc)(const HyperPlane* plane, const HyperPlane* Object, vector<HyperPlane*>* Out_Objects, vector<Vertex*>* Out_Vertexs)>
class BSPTree
{
public:
	BSPNode<HyperPlane, GetRelationFunc, SplitFunc>* root;
	int maxdepth;
	float BlanceRatio;
	bool isSplitHyperPlaneriangle;

	
	BSPHyperPlaneree(){}
	BSPHyperPlaneree(vector<HyperPlane*> Objects, int depth = 7, float BlanceRatio = 0.9f, bool isSplitHyperPlaneriangle = false)
	{
		HyperPlane* blanceobject = BSPNode<HyperPlane,Vertex, GetRelationFunc, SplitFunc>::GetBlanceObject(&Objects,BlanceRatio);
		root = new BSPNode<HyperPlane, Vertex, GetRelationFunc, SplitFunc>(Objects, blanceobject);
		this->BlanceRatio = BlanceRatio;
		this->isSplitHyperPlaneriangle = isSplitHyperPlaneriangle;
		buildHyperPlaneree(depth, root);
	}

	void buildHyperPlaneree(int depth,BSPNode<HyperPlane, Vertex,GetRelationFunc, SplitFunc>* node)
	{
		if (depth < 0) return;

		HyperPlane* f_object = BSPNode<HyperPlane, Vertex,GetRelationFunc, SplitFunc>::GetBlanceObject(&(node->frontObjects), BlanceRatio);
		HyperPlane* b_object = BSPNode<HyperPlane, GetRelationFunc, SplitFunc>::GetBlanceObject(&(node->behindObjects),BlanceRatio);

		if (f_object != nullptr)
		{
			BSPNode<HyperPlane, Vertex,GetRelationFunc, SplitFunc>* f_node = new BSPNode<HyperPlane, Vertex,GetRelationFunc, SplitFunc>(node->frontObjects, f_object,isSplitHyperPlaneriangle);
			node->frontNode = f_node;
			buildHyperPlaneree(depth - 1, f_node);
		}
		if (b_object != nullptr)
		{
			BSPNode<HyperPlane, Vertex, GetRelationFunc, SplitFunc>* b_node = new BSPNode<HyperPlane, Vertex,GetRelationFunc, SplitFunc>(node->behindObjects, b_object,isSplitHyperPlaneriangle);
			node->behindNode = b_node;
			buildHyperPlaneree(depth - 1, b_node);
		}
	}
};


