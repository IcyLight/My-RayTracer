#pragma once
#include <vector>
#include <algorithm>
using namespace std;




enum BSP_Case
{
	front,behind,span,coincident
};

template<typename T , BSP_Case(*GetRelationFunc)(const T* object, const T* hyperplane)>
class BSPNode
{
public:
	vector<T*> frontObjects;
	vector<T*> behindObjects;
	vector<T*> spanObjects;
	T* Object;
	BSPNode* frontNode;
	BSPNode* behindNode;

	BSPNode(){}

	BSPNode(vector<T*> leafObjects,T* hyperplane)
	{
		this->Object = hyperplane;
		this->frontNode = nullptr;
		this->behindNode = nullptr;
		for (int i=0;i<leafObjects.size();++i)
		{

			BSP_Case relation = GetRelationFunc(leafObjects[i] , hyperplane);
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
				spanObjects.push_back(leafObjects[i]);
				break;
			default:
				break;
			}
		}
	}

	static T* GetBlanceObject(const vector<T*>* Objects, float blanceRatio)
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


template<typename T,BSP_Case (*GetRelationFunc)(const T* object,const T* hyperplane) > 
class BSPTree
{
public:
	BSPNode<T, GetRelationFunc>* root;
	int maxdepth;
	float BlanceRatio;


	BSPTree(){}
	BSPTree(vector<T*> Objects,int depth=7,float BlanceRatio=0.9f)
	{
		T* blanceobject = BSPNode<T, GetRelationFunc>::GetBlanceObject(&Objects,BlanceRatio);
		root = new BSPNode<T, GetRelationFunc>(Objects, blanceobject);
		this->BlanceRatio = BlanceRatio;
		buildTree(depth, root);
	}

	void buildTree(int depth,BSPNode<T,GetRelationFunc>* node )
	{
		if (depth < 0) return;

		T* f_object = BSPNode<T, GetRelationFunc>::GetBlanceObject(&(node->frontObjects), BlanceRatio);
		T* b_object = BSPNode<T, GetRelationFunc>::GetBlanceObject(&(node->behindObjects),BlanceRatio);

		if (f_object != nullptr)
		{
			BSPNode<T, GetRelationFunc>* f_node = new BSPNode<T, GetRelationFunc>(node->frontObjects, f_object);
			node->frontNode = f_node;
			buildTree(depth - 1, f_node);
		}
		if (b_object != nullptr)
		{
			BSPNode<T, GetRelationFunc>* b_node = new BSPNode<T, GetRelationFunc>(node->behindObjects, b_object);
			node->behindNode = b_node;
			buildTree(depth - 1, b_node);
		}
	}
};


