#include "EngineImp.h"
#include "DeferredRenderPipeline.h"
#include "ZBufferRenderPipeline.h"
#include "../SceneTree/SceneTree.h"

CEngine::CEngine(EngineInitParam& init_param)
{
	if (init_param.UseDeferredRendering)
	{
		m_render_pipeline = std::make_unique<CDeferredRenderPipeline>(init_param.HInstance, init_param.HWnd);
	}
	else
	{
		//m_render_pipeline = std::make_unique<CZBufferRenderPipeline>(init_param.HInstance, init_param.HWnd);
	}
	m_scene_tree = std::make_unique<QuadTree::CQuadTree>();
}

CEngine::~CEngine()
{

}

bool CEngine::Initialize()
{
	return m_render_pipeline->Initialize();
}

void CEngine::OnResize()
{
	m_render_pipeline->OnResize();
}

void CEngine::Update(const GameTimer& gt)
{
	if (m_render_pipeline->IsCameraDirty())
	{
		m_render_pipeline->UpdateCamera(gt);
		auto culling_res = m_scene_tree->Culling(m_render_pipeline->GetCameraFrustum());
		m_render_pipeline->ClearVisibleRenderItems();
		m_render_pipeline->PushVisibleModels((int)RenderLayer::Occluder, culling_res);
	}
	
	m_render_pipeline->Update(gt);
}

void CEngine::Draw(const GameTimer& gt)
{
	m_render_pipeline->Draw(gt);
}

void CEngine::PushModels(std::vector<RenderItem*>& render_items)
{
	m_scene_tree->Init(render_items);
	m_render_pipeline->PushMats(render_items);
}

bool CEngine::InitDirect3D()
{
	return m_render_pipeline->InitDirect3D();
}

void CEngine::Debug()
{
	m_render_pipeline->Debug();
}

void CEngine::PitchCamera(float rad)
{
	m_render_pipeline->PitchCamera(rad);
}

void CEngine::RotateCameraY(float rad)
{
	m_render_pipeline->RotateCameraY(rad);
}

void CEngine::MoveCamera(float dis)
{
	m_render_pipeline->MoveCamera(dis);
}

void CEngine::StrafeCamera(float dis)
{
	m_render_pipeline->StrafeCamera(dis);
}




