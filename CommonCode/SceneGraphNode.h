#ifndef _SCENEGRAPH_NODE_H_
#define _SCENEGRAPH_NODE_H_ 
#include "ShaderObjectInterface.h"

namespace geomUtils
{
	class SceneGraphNode
	{
		mutable struct Payload
		{
			tRectagleF _extents;
			tVectorF _position;
			tVectorOfFloats _matrix;
		} _data;

		const std::string _key;
		tShaderObjectInterfacePtr _node;
	public:
		SceneGraphNode(tShaderObjectInterfacePtr node, std::string& key) : _node(node), _key(key)
		{
			_data._matrix.resize(16); //4x4
			_data._position.x =_data._position.y=_data._position.z = 0.0;
			geomUtils::makeIdentityMatrix(_data._matrix);
		}
		bool operator<(const SceneGraphNode& rhs) const
		{
			return _key < rhs._key;
		}

		const tVectorOfFloats& getMatrix() const { return _data._matrix; }
		void setMatrix(const geomUtils::tVectorOfFloats& m)	
		{ 
			geomUtils::matrixMultiply(m, _data._matrix, _data._matrix); 
			(*_node)(_data._matrix);
		}
		const tVectorF& getPosition() const {return _data._position;}
		void translate(const tVectorF& v) 
		{ 
			_data._position = v; //save position
			if (geomUtils::translate(v.x, v.y, v.z, _data._matrix))
				(*_node)(_data._matrix);
		}

	    const geomUtils::tRectagleF& getMinMax(geomUtils::tVectorF& min, geomUtils::tVectorF& max) const
		{
			_node->getMinMax(min, max);
			_data._extents.ul.x = min.x;
			_data._extents.ul.y = min.y;
			_data._extents.ul.z = min.z;

			_data._extents.lr.x = max.x;
			_data._extents.lr.y = max.y;
			_data._extents.lr.z = max.z;
			return _data._extents;
		}
	};
typedef TR1_NAMESPACE::shared_ptr<SceneGraphNode> tSceneGraphNodePtr;
typedef TR1_NAMESPACE::shared_ptr<const SceneGraphNode> tSceneGraphNodeConstPtr;
};
#endif