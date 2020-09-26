#pragma once
#include <vector>
#include <string>
#include "../Common/GeometryDefines.h"
#include <DirectXCollision.h>
#include <map>

struct RenderItem;

class ISceneTree
{
public:
	virtual void Init(std::vector<RenderItem*>& render_items) = 0;
	virtual void Load(std::string& file) = 0;
	virtual void Save(std::string& file) = 0;
	virtual std::map<int, std::vector<RenderItem*>> Culling(const DirectX::BoundingFrustum& frustum) = 0;
};