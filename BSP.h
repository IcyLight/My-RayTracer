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
	bool(*SplitFunc)(const HyperPlane* plane, const HyperPlane* Object, vector<HyperPlane*>* Out_ObjectsContainer, vector<Vertex*>* Out_VertexsContainer) //物体容器和顶点容器，分别盛放切割产生的物体和顶点，返回是否成功切割
>
class BSPNode
{
public:
	vector<HyperPlane*> frontObjects;
	vector<HyperPlane*> behindObjects;
	vector<HyperPlane*> spanObjects;
	
	//切割出来的三角形是由BSP树生成的，单独放在一个容器里，这些由BSP树负责释放内存，其他不由BSP树负责
	vector<HyperPlane*> splitedObjects;
	vector<Vertex*> splitedVertexs; //原本不存在，由BSP树分割出来的顶点

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
					vector<HyperPlane*> SplitedObjects;
					if (SplitFunc(plane, leafObjects[i], &SplitedObjects, &splitedVertexs))
					{
						for (int j = 0; j < SplitedObjects.size(); j++)
						{
							BSP_Case j_relation = GetRelationFunc(SplitedObjects[j], plane);
							
							switch (j_relation)
							{
							case BSP_Case::front:
								splitedObjects.push_back(SplitedObjects[j]);
								frontObjects.push_back(SplitedObjects[j]);
								break;
							case BSP_Case::behind:
								splitedObjects.push_back(SplitedObjects[j]);
								behindObjects.push_back(SplitedObjects[j]);
								break;
							default:
								splitedObjects.push_back(SplitedObjects[j]);
								spanObjects.push_back(SplitedObjects[j]);
								printf("AA");
								break;
							}

						}
					}
					else
					{
						spanObjects.push_back(leafObjects[i]);
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

	void GetTreeSplitedObjects(list<HyperPlane*>* Out_ObjectContainer)
	{
		Out_ObjectContainer->insert(Out_ObjectContainer->end(), splitedObjects.begin(), splitedObjects.end());
		if (frontNode != nullptr)
		{
			frontNode->GetTreeSplitedObjects(Out_ObjectContainer);
		}
		if (behindNode != nullptr)
		{
			behindNode->GetTreeSplitedObjects(Out_ObjectContainer);
		}
		return;
	}
};


template<typename HyperPlane,typename Vertex,
	BSP_Case (*GetRelationFunc)(const HyperPlane* object,const HyperPlane* HyperPlane), 
	bool(*SplitFunc)(const HyperPlane* plane, const HyperPlane* Object, vector<HyperPlane*>* Out_Objects, vector<Vertex*>* Out_Vertexs)>
class BSPTree
{
public:
	BSPNode<HyperPlane, Vertex,GetRelationFunc, SplitFunc>* root;
	int maxdepth;
	float BlanceRatio;
	bool isSplitHyperPlaneTriangle;

	vector<HyperPlane*> BSPObjects;

	
	BSPTree() { this->root = nullptr; }
	BSPTree(vector<HyperPlane*> Objects, int depth = 7, float BlanceRatio = 0.9f, bool isSplitHyperPlaneTriangle = false)
	{
		HyperPlane* blanceobject = BSPNode<HyperPlane,Vertex, GetRelationFunc, SplitFunc>::GetBlanceObject(&Objects,BlanceRatio);
		root = new BSPNode<HyperPlane, Vertex, GetRelationFunc, SplitFunc>(Objects, blanceobject,isSplitHyperPlaneTriangle);
		this->BlanceRatio = BlanceRatio;
		this->isSplitHyperPlaneTriangle = isSplitHyperPlaneTriangle;
		this->maxdepth = depth;
		buildBSPTree(depth, root);
		this->BSPObjects = Objects;
		list<HyperPlane*> splitedObjectList;
		root->GetTreeSplitedObjects(&splitedObjectList);
		BSPObjects.insert(BSPObjects.end(), splitedObjectList.begin(), splitedObjectList.end());

	}

	void buildBSPTree(int depth,BSPNode<HyperPlane, Vertex,GetRelationFunc, SplitFunc>* node)
	{
		if (depth < 0) return;

		HyperPlane* f_object = BSPNode<HyperPlane, Vertex,GetRelationFunc, SplitFunc>::GetBlanceObject(&(node->frontObjects), BlanceRatio);
		HyperPlane* b_object = BSPNode<HyperPlane, Vertex,GetRelationFunc, SplitFunc>::GetBlanceObject(&(node->behindObjects),BlanceRatio);

		if (f_object != nullptr)
		{
			BSPNode<HyperPlane, Vertex,GetRelationFunc, SplitFunc>* f_node = new BSPNode<HyperPlane, Vertex,GetRelationFunc, SplitFunc>(node->frontObjects, f_object,isSplitHyperPlaneTriangle);
			node->frontNode = f_node;
			buildBSPTree(depth - 1, f_node);
		}
		if (b_object != nullptr)
		{
			BSPNode<HyperPlane, Vertex, GetRelationFunc, SplitFunc>* b_node = new BSPNode<HyperPlane, Vertex,GetRelationFunc, SplitFunc>(node->behindObjects, b_object,isSplitHyperPlaneTriangle);
			node->behindNode = b_node;
			buildBSPTree(depth - 1, b_node);
		}
	}


};


